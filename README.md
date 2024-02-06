# Overview

This project should help to read CXL DVSEC registers by passing information from
CXL DVSEC stored in the extended PCIe configuration space.

# Dependencies:

Debian like system:

    apt install meson ninja-build libpci-dev

# Usage

Nowadays, there are two major way how to use the tool - passing output of
`lspci` or accessing extended configuration space directly.

Examples bellow show example of usage for CXL device being on address `27:00.0`.

    cxl-reg-parser -s 27:00.0

Similarly, using `lscpi`:

    cxl-reg-parser <<< $(lspci -s 27:00.0 -vvvv -xxxx)

or

    lspci -s 27:00.0 -vvvv -xxxx > lscpi.log
    cxl-reg-parser < lspci.log
