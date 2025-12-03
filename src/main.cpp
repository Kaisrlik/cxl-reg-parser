#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GCC13 - #include <format>

#include "dvsec0_cxlevices.hpp"

extern "C"{
#include <pci/pci.h>
}

constexpr int PCIE_EXTENDED_CONF_SIZE = 4096;
std::uint8_t pcie_cfg[PCIE_EXTENDED_CONF_SIZE] = {};

const std::string EXPECTED_LINE {"00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00"};

static int lspci_find_cxl_dvsec()
{
	std::string line;
	const std::string cap{"Capabilities: "};
	const std::string cxldvsec{"Designated Vendor-Specific: Vendor=1e98 ID=0000"};
	int ret = -1;

	while (std::getline(std::cin, line)) {
		if (line.find(cap) == std::string::npos)
			continue;
		if (line.find(cxldvsec) == std::string::npos)
			continue;

		std::size_t pos1 = line.find("[");
		std::size_t pos2 = line.find("]");
		std::string addr = line.substr(pos1 + 1, pos2 - pos1 - 1);
		ret = std::stoul(addr, nullptr, 16);
		std::cout << "CXL DVSEC Capabilities found:(" << addr << "): " << line << '\n';
		break;
	}
	return ret;
}

static int lspci_output_to_array()
{
	const std::string address_section{"00: "};
	constexpr int LSPCIE_NUM_TOKENS = 17;
	std::string line;
	int ret = 1;

	// TODO: add some input error handling
	// expected line format
	// fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	while (std::getline(std::cin, line)) {
		if (line.rfind(address_section, 0) == std::string::npos && ret == 1)
			continue;

		ret = 0;
		std::stringstream ss(line);

		// TODO: add regexp
		if (line.size() == 0)
			return 0;
		if (line.size() < EXPECTED_LINE.size()) {
			std::cout << "Unexpected lenght of input" << std::endl;
			return 1;
		}

		std::string token;
		int address = 0;
		for (int i = 0; i < LSPCIE_NUM_TOKENS; ++i) {
			getline(ss, token, ' ');
			int number = std::stoul(token, nullptr, 16);
			if (i == 0) {
				address = number;
				continue;
			}
			// TODO: add check to avoid out of bound access
			pcie_cfg[address + i - 1] = number;
		}
	}
	return ret;
}

static int pci_get_extended_reg_space(char *bus, std::uint8_t *ext)
{
	struct pci_dev *dev;
	struct pci_access *pacc;
	struct pci_filter filter;
	char *msg;

	pacc = pci_alloc();
	pci_init(pacc);
	pci_filter_init(pacc, &filter);
	pci_scan_bus(pacc);

	if ((msg = pci_filter_parse_slot(&filter, bus)))
		std::cout << "- s" << msg << std::endl;
	std::cout << "Accessing device - " << std::hex << filter.bus <<":"<< filter.slot <<"."<<filter.func << std::endl;
	dev = pci_get_dev(pacc, 0, filter.bus, filter.slot, filter.func);
	if (!dev) {
		fprintf(stderr, "Failed to allocate dev\n");
		return 1;
	}

	memset(ext, 0, PCIE_EXTENDED_CONF_SIZE);
	if (!pci_read_block(dev, 0, ext, PCIE_EXTENDED_CONF_SIZE)) {
		fprintf(stderr, "Failed to read pci extendend config register\n");
		return 2;
	}

	pci_cleanup(pacc);
	return 0;
}

static int find_cxl_dvsec(std::uint8_t *ext)
{
	// TODO: add define
	int i = 0x100;
	while (i != 0) {
		struct pcie_extended_capability_header *pcie_cap_hdr = (struct pcie_extended_capability_header*)&ext[i];
		i = pcie_cap_hdr->next_cap;
		int ext_cap_off = i + sizeof(pcie_extended_capability_header);
		struct dvsec_header1 *dvsec1= (struct dvsec_header1*)&ext[ext_cap_off];
		struct dvsec_header2 *dvsec2 = (struct dvsec_header2*)&ext[ext_cap_off + sizeof(dvsec_header1)];
		// TODO: check size overflow and add defines
		if(dvsec1->dvsec_vendor_id == CXL_VENDOR_ID && dvsec2->dvsec_id == CXL_DVSEC_ID_CXL_DEVICES) {
			std::cout << "CXL DVSEC found on offset 0x" << std::hex << ext_cap_off << std::endl;
			std::cout << "--------------------------------" << std::endl;
			break;
		}
	}
	return i;
}

static void usage_and_exit(int exit_code, const char *app_name)
{
	std::cout << app_name << " [OPTIONS]" << std::endl;
	std::cout << std::endl;
	std::cout << "Application tries to find CXL configuration and prints the parsed output." << std::endl;
	std::cout << std::endl;
	std::cout << "OPTIONS are:" << std::endl;
	std::cout << "    -h              Print this help message and exit." << std::endl;
	std::cout << "    -s              Pass full PCIe address - [domain:]bus:slot:func" << std::endl;
	std::cout << std::endl;
	std::cout << "Example usage:" << std::endl;
	std::cout << "  Directly reading configuration from extended config space of device:" << std::endl;
	std::cout << std::endl;
	std::cout << "    cxl-reg-parser -s 27:00.0" << std::endl;
	std::cout << std::endl;
	std::cout << "  Reading configuration though output of lspci:" << std::endl;
	std::cout << std::endl;
	std::cout << "    cxl-reg-parser <<< $(lspci -s 27:00.0 -vvvv -xxxx)" << std::endl;
	std::cout << std::endl;
	std::cout << "  Reading configuration though output of lspci saved in log file:" << std::endl;
	std::cout << std::endl;
	std::cout << "    lspci -s 27:00.0 -vvvv -xxxx > lscpi.log" << std::endl;
	std::cout << "    cxl-reg-parser < lspci.log" << std::endl;
	exit(exit_code);
}

int main(int argc, char *argv[])
{
	char opt;
	int ret;
	int cap_off = 0;
	bool pci = false;
	char *bus;
	int cxl_dvsec_ptr = 0;

	while ((opt = getopt(argc, argv, "hs:")) != -1) {
		switch (opt) {
		case 'h':
			usage_and_exit(0, argv[0]);
			break;
		case 's':
		        bus = optarg;
			pci = true;
			break;
		default:
			usage_and_exit(1, "Invalid or missing argument");
		}
	}

	// Find device and his dvsec using libpci
	if (pci) {
		if (pci_get_extended_reg_space(bus, pcie_cfg)) {
			std::cout << "Unable to obtain extended config space." << std::endl;
			return 1;
		}
	} else {
		// Use output of lspci -s ? -vvvv -xxxx
		cap_off = lspci_find_cxl_dvsec();
		if (cap_off <= 0) {
			std::cout << "CXL Capabilities not found!" << std::endl;
			exit(1);
		}
		std::cout << "CXL Capabilities ofsset is " << std::hex << cap_off << std::endl;

		ret = lspci_output_to_array();
		if (ret != 0) {
			std::cout << "Address space not found!" << std::endl;
			exit(1);
		}
	}

	if ((cxl_dvsec_ptr = find_cxl_dvsec(pcie_cfg)) == 0) {
		std::cout << "CXL DVSEC not found." << std::endl;
		return 0;
	}

	struct pcie_cxl_dvsec_header *cxl_dvsec = (struct pcie_cxl_dvsec_header *)
		&pcie_cfg[cxl_dvsec_ptr + sizeof(pcie_extended_capability_header)];
	std::cout << *cxl_dvsec << std::endl;
	return 0;
}
