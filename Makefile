# ----------------------------------------------------------------------------
# Copyright (c) 2020-2024 RISC-V Steel contributors
#
# This work is licensed under the MIT License, see LICENSE file for details.
# SPDX-License-Identifier: MIT
# ----------------------------------------------------------------------------

MAKEFLAGS += --no-print-directory

ifdef TOOLCHAIN_PREFIX
TOOLCHAIN_PREFIX_FLAG = -DTOOLCHAIN_PREFIX=${TOOLCHAIN_PREFIX}
endif

ifdef PREFIX
TOOLCHAIN_PREFIX_FLAG = -DTOOLCHAIN_PREFIX="${PREFIX}/bin/riscv32-unknown-elf-"
endif

all: debug

debug:
	@mkdir -p build
	@cd build && cmake -Wno-dev -DCMAKE_BUILD_TYPE=Debug ${FLAGS} ${TOOLCHAIN_PREFIX_FLAG} ..
	@$(MAKE) -C build

release:
	@mkdir -p build
	@cd build && cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release ${FLAGS} ${TOOLCHAIN_PREFIX_FLAG} ..
	@$(MAKE) -C build

clean:
	@rm -rf build dump
	@echo "Build directory deleted."

.PHONY: debug release clean
