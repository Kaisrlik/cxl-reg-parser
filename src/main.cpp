#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GCC13 - #include <format>

struct __attribute__((packed)) pcie_cxl_dvsec_header {
	uint32_t pcie_extended_capability_header;
	struct dvsec_header1 {
		uint32_t dvsec_vendor_id : 16;
		uint32_t dvsec_revision_id : 4;
		uint32_t dvsec_len : 12;
	} dvsec_header1;
	struct dvsec_header2 {
		uint16_t dvsec_id;
	} dvsec_header2;
	struct cxl_capability {
		uint16_t cache_capable: 1;
		uint16_t io_capable: 1;
		uint16_t mem_capable: 1;
		uint16_t mem_hwinit_mode: 1;
		uint16_t hdm_count: 1;
		uint16_t cache_writeback_and_invalidate_capable: 1;
		uint16_t cxl_reset_capable: 1;
		uint16_t cxl_reset_timeout: 3;
		uint16_t cxl_reset_mem_clr_capable: 1;
		uint16_t reserved: 1;
		uint16_t multiple_logical_device: 1;
		uint16_t viral_capable: 1;
		uint16_t pm_init_completion_reporting_capable: 1;
	} cxl_capability;
	struct cxl_control {
		uint16_t cache_enable: 1;
		uint16_t io_enable: 1;
		uint16_t mem_enable: 1;
		uint16_t cache_sf_coverage: 5;
		uint16_t cache_sf_granularity: 3;
		uint16_t cache_clean_eviction: 1;
		uint16_t reserved: 2;
		uint16_t viral_enable: 1;
		uint16_t reserved2: 1;
	} cxl_control;
	struct cxl_status {
		uint16_t reserved: 14;
		uint16_t viral_status: 1;
		uint16_t reserved2: 1;
	} cxl_status;
	struct cxl_control2 {
		uint16_t disable_caching: 1;
		uint16_t initiate_cache_writeback_and_invalidation: 1;
		uint16_t initiate_cxl_reset: 1;
		uint16_t cxl_reset_mem_clr_enable: 1;
		uint16_t desired_volatile_hdm_state_after_hot_reset: 1;
		uint16_t reserved: 11;
	} cxl_control2;
	struct cxl_status2 {
		uint16_t cache_invalid: 1;
		uint16_t cxl_reset_complete: 1;
		uint16_t cxl_reset_error: 1;
		uint16_t vilatile_hdm_perservation_error: 1;
		uint16_t reserved: 11;
		uint16_t power_management_initialization_complete: 1;
	} cxl_status2;
	struct cxl_lock {
		uint16_t config_lock: 1;
		uint16_t reserved: 15;
	} cxl_lock;
	struct cxl_capability2 {
		uint16_t cache_size_unit: 4;
		uint16_t fallback_capability: 2;
		uint16_t reserved: 2;
		uint16_t cache_size: 8;
	} cxl_capability2;
	struct cxl_range1_size_high {
		uint32_t memory_size_high;
	} cxl_range1_size_high;
	struct cxl_range1_size_low {
		uint32_t memory_info_valid: 1;
		uint32_t memory_active: 1;
		uint32_t media_type: 3;
		uint32_t memory_class: 3;
		uint32_t desired_interleave: 5;
		uint32_t memory_active_timeout: 3;
		uint32_t reserved: 12;
		uint32_t memory_size_low: 4;
	} cxl_range1_size_low;
	struct cxl_range1_base_high {
		uint32_t memory_base_high;
	} cxl_range1_base_high;
	struct cxl_range1_base_low {
		uint32_t reserved: 28;
		uint32_t memory_base_low: 4;
	} cxl_range1_base_low;
	struct cxl_range2_size_high {
		uint32_t memory_size_high;
	} cxl_range2_size_high;
	struct cxl_range2_size_low {
		uint32_t memory_info_valid: 1;
		uint32_t memory_active: 1;
		uint32_t media_type: 3;
		uint32_t memory_class: 3;
		uint32_t desired_interleave: 5;
		uint32_t memory_active_timeout: 3;
		uint32_t reserved: 12;
		uint32_t memory_size_low: 4;
	} cxl_range2_size_low;
	struct cxl_range2_base_high {
		uint32_t memory_base_high;
	} cxl_range2_base_high;
	struct cxl_range2_base_low {
		uint32_t reserved: 28;
		uint32_t memory_base_low: 4;
	} cxl_range2_base_low;
	// TODO: split it to two classes avoid memory issues
	// Added in DVSEC revision 2
	struct cxl_capability3 {
		uint16_t default_volatile_hdm_state_after_cold_reset: 1;
		uint16_t default_volatile_hdm_state_after_warm_reset: 1;
		uint16_t default_volatile_hdm_state_after_hot_reset: 1;
		uint16_t volatile_hdm_state_after_hot_reset_configurability: 1;
		uint16_t reserved: 12;
	} cxl_capability3;
	uint16_t reserved;

	friend std::ostream& operator<<(std::ostream &os, const pcie_cxl_dvsec_header &e)
	{
		os << "PCIe CXL DVSEC header: {" << std::endl << std::hex
			<< "  .dvsec_header1: {" << std::endl
			<< "    .dvsec_vendor_id: " << (unsigned)e.dvsec_header1.dvsec_vendor_id << std::endl
			<< "    .dvsec_revision_id: " << (unsigned)e.dvsec_header1.dvsec_revision_id << std::endl
			<< "    .dvsec_len: " << (unsigned)e.dvsec_header1.dvsec_len << std::endl
			<< "  }" << std::endl
			<< "  .dvsec_header2: {" << std::endl
			<< "    .dvsec_id: " << (unsigned)e.dvsec_header2.dvsec_id << std::endl
			<< "  }" << std::endl
			<< "  .cxl_capability: {" << std::endl
			<< "    .cache_capable: " << (unsigned)e.cxl_capability.cache_capable << std::endl
			<< "    .io_capable: " << (unsigned)e.cxl_capability.io_capable << std::endl
			<< "    .mem_capable: " << (unsigned)e.cxl_capability.mem_capable << std::endl
			<< "    .mem_hwinit_mode: " << (unsigned)e.cxl_capability.mem_hwinit_mode << std::endl
			<< "    .hdm_count: " << (unsigned)e.cxl_capability.hdm_count << std::endl
			<< "    .cache_writeback_and_invalidate_capable: " << (unsigned)e.cxl_capability.cache_writeback_and_invalidate_capable << std::endl
			<< "    .cxl_reset_capable: " << (unsigned)e.cxl_capability.cxl_reset_capable << std::endl
			<< "    .cxl_reset_timeout: " << (unsigned)e.cxl_capability.cxl_reset_timeout << std::endl
			<< "    .cxl_reset_mem_clr_capable: " << (unsigned)e.cxl_capability.cxl_reset_mem_clr_capable << std::endl
			<< "    .multiple_logical_device: " << (unsigned)e.cxl_capability.multiple_logical_device << std::endl
			<< "    .viral_capable: " << (unsigned)e.cxl_capability.viral_capable << std::endl
			<< "    .pm_init_completion_reporting_capable: " << (unsigned)e.cxl_capability.pm_init_completion_reporting_capable << std::endl
			<< "  }" << std::endl
			<< "  .cxl_control: {" << std::endl
			<< "    .cache_enable: " << (unsigned)e.cxl_control.cache_enable << std::endl
			<< "    .io_enable: " << (unsigned)e.cxl_control.io_enable << std::endl
			<< "    .mem_enable: " << (unsigned)e.cxl_control.mem_enable << std::endl
			<< "    .cache_sf_coverage: " << (unsigned)e.cxl_control.cache_sf_coverage << std::endl
			<< "    .cache_sf_granularity: " << (unsigned)e.cxl_control.cache_sf_granularity << std::endl
			<< "    .cache_clean_eviction: " << (unsigned)e.cxl_control.cache_clean_eviction << std::endl
			<< "    .viral_enable: " << (unsigned)e.cxl_control.viral_enable << std::endl
			<< "  }" << std::endl
			<< "  .cxl_status: {" << std::endl
			<< "    .viral_status: " << (unsigned)e.cxl_status.viral_status << std::endl
			<< "  }" << std::endl
			<< "  .cxl_control2: {" << std::endl
			<< "    .disable_caching: " << (unsigned)e.cxl_control2.disable_caching << std::endl
			<< "    .initiate_cache_writeback_and_invalidation: " << (unsigned)e.cxl_control2.initiate_cache_writeback_and_invalidation << std::endl
			<< "    .initiate_cxl_reset: " << (unsigned)e.cxl_control2.initiate_cxl_reset << std::endl
			<< "    .cxl_reset_mem_clr_enable: " << (unsigned)e.cxl_control2.cxl_reset_mem_clr_enable << std::endl
			<< "    .desired_volatile_hdm_state_after_hot_reset: " << (unsigned)e.cxl_control2.desired_volatile_hdm_state_after_hot_reset << std::endl
			<< "  }" << std::endl
			<< "  .cxl_status2: {" << std::endl
			<< "    .cache_invalid: " << (unsigned)e.cxl_status2.cache_invalid << std::endl
			<< "    .cxl_reset_complete: " << (unsigned)e.cxl_status2.cxl_reset_complete << std::endl
			<< "    .cxl_reset_error: " << (unsigned)e.cxl_status2.cxl_reset_error << std::endl
			<< "    .vilatile_hdm_perservation_error: " << (unsigned)e.cxl_status2.vilatile_hdm_perservation_error << std::endl
			<< "    .power_management_initialization_complete: " << (unsigned)e.cxl_status2.power_management_initialization_complete << std::endl
			<< "  }" << std::endl
			<< "  .cxl_lock: {" << std::endl
			<< "    .config_lock: " << (unsigned)e.cxl_lock.config_lock << std::endl
			<< "  }" << std::endl
			<< "  .cxl_capability2: {" << std::endl
			<< "    .cache_size_unit: " << (unsigned)e.cxl_capability2.cache_size_unit << std::endl
			<< "    .fallback_capability: " << (unsigned)e.cxl_capability2.fallback_capability << std::endl
			<< "    .cache_size: " << (unsigned)e.cxl_capability2.cache_size << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range1_size_high: {" << std::endl
			<< "    .memory_size_high: " << (unsigned)e.cxl_range1_size_high.memory_size_high << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range1_size_low: {" << std::endl
			<< "    .memory_info_valid: " << (unsigned)e.cxl_range1_size_low.memory_info_valid << std::endl
			<< "    .memory_active: " << (unsigned)e.cxl_range1_size_low.memory_active << std::endl
			<< "    .media_type: " << (unsigned)e.cxl_range1_size_low.media_type << std::endl
			<< "    .memory_class: " << (unsigned)e.cxl_range1_size_low.memory_class << std::endl
			<< "    .desired_interleave: " << (unsigned)e.cxl_range1_size_low.desired_interleave << std::endl
			<< "    .memory_active_timeout: " << (unsigned)e.cxl_range1_size_low.memory_active_timeout << std::endl
			<< "    .memory_size_low: " << (unsigned)e.cxl_range1_size_low.memory_size_low << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range1_base_high: {" << std::endl
			<< "    .memory_base_high: " << (unsigned)e.cxl_range1_base_high.memory_base_high << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range1_base_low: {" << std::endl
			<< "    .memory_base_low: " << (unsigned)e.cxl_range1_base_low.memory_base_low << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range2_size_high: {" << std::endl
			<< "    .memory_size_high: " << (unsigned)e.cxl_range2_size_high.memory_size_high << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range2_size_low: {" << std::endl
			<< "    .memory_info_valid: " << (unsigned)e.cxl_range2_size_low.memory_info_valid << std::endl
			<< "    .memory_active: " << (unsigned)e.cxl_range2_size_low.memory_active << std::endl
			<< "    .media_type: " << (unsigned)e.cxl_range2_size_low.media_type << std::endl
			<< "    .memory_class: " << (unsigned)e.cxl_range2_size_low.memory_class << std::endl
			<< "    .desired_interleave: " << (unsigned)e.cxl_range2_size_low.desired_interleave << std::endl
			<< "    .memory_active_timeout: " << (unsigned)e.cxl_range2_size_low.memory_active_timeout << std::endl
			<< "    .memory_size_low: " << (unsigned)e.cxl_range2_size_low.memory_size_low << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range2_base_high: {" << std::endl
			<< "    .memory_base_high: " << (unsigned)e.cxl_range2_base_high.memory_base_high << std::endl
			<< "  }" << std::endl
			<< "  .cxl_range2_base_low: {" << std::endl
			<< "    .memory_base_low: " << (unsigned)e.cxl_range2_base_low.memory_base_low << std::endl
			<< "  }" << std::endl;

			if (e.dvsec_header1.dvsec_revision_id >= 2) {
				os << "  .cxl_capability3: {" << std::endl
				<< "    .default_volatile_hdm_state_after_cold_reset: " << (unsigned)e.cxl_capability3.default_volatile_hdm_state_after_cold_reset << std::endl
				<< "    .default_volatile_hdm_state_after_warm_reset: " << (unsigned)e.cxl_capability3.default_volatile_hdm_state_after_warm_reset << std::endl
				<< "    .default_volatile_hdm_state_after_hot_reset: " << (unsigned)e.cxl_capability3.default_volatile_hdm_state_after_hot_reset << std::endl
				<< "    .volatile_hdm_state_after_hot_reset_configurability: " << (unsigned)e.cxl_capability3.volatile_hdm_state_after_hot_reset_configurability << std::endl
				<< "  }" << std::endl;
			}

		return os;
	}
};

constexpr int PCIE_EXTENDED_CONF_SIZE = 4096;
uint8_t pcie_cfg[PCIE_EXTENDED_CONF_SIZE] = {};

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
		std::string token;
		int address = 0;
		for (int i = 0; i < LSPCIE_NUM_TOKENS; ++i) {
			getline(ss, token, ' ');
			int number = std::stoul(token, nullptr, 16);
			if (i == 0) {
				address = number;
				continue;
			}
			pcie_cfg[address + i - 1] = number;
		}
	}
	return ret;
}

static void usage_and_exit(int exit_code, const char *app_name)
{
	std::cout << app_name << " [OPTIONS]" << std::endl;
	std::cout << std::endl;
	std::cout << "OPTIONS are:" << std::endl;
	std::cout << "    -h              Print this help message and exit." << std::endl;
	exit(exit_code);
}

int main(int argc, char *argv[])
{
	char opt;
	int ret;
	int cxl_dvsec_off = 0;

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
		case 'h':
			usage_and_exit(0, argv[0]);
			break;
		default:
			usage_and_exit(1, "Invalid or missing argument");
		}
	}

	// TODO: this could be found in pcie extended configuration space by finding
	// CXL DVSEC header
	cxl_dvsec_off = lspci_find_cxl_dvsec();
	if (cxl_dvsec_off <= 0) {
		std::cout << "CXL DVSEC Capabilities not found!" << std::endl;
		exit(1);
	}

	// TODO: this could be directly read from device
	ret = lspci_output_to_array();
	if (ret != 0) {
		std::cout << "Address space not found!" << std::endl;
		exit(1);
	}

	struct pcie_cxl_dvsec_header *cxl_dvsec = (struct pcie_cxl_dvsec_header *)&pcie_cfg[cxl_dvsec_off];
	std::cout << *cxl_dvsec << std::endl;

	return 0;
}
