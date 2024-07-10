# -----------------------------------------------------------------------------
# Copyright (c) 2020-2024 RISC-V Steel contributors
#
# This work is licensed under the MIT License, see LICENSE file for details.
# SPDX-License-Identifier: MIT
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

enable_language(C CXX ASM)

if(DEFINED STARTUP_DATA_ROM)
  add_compile_definitions(STARTUP_DATA_ROM=1)
endif()

if(DEFINED NO_STARTUP_FILES)
  add_compile_definitions(NO_STARTUP_FILES=1)
endif()

if(NOT DEFINED APP_ARCH)
  set(APP_ARCH "rv32izicsr")
endif()

if(NOT DEFINED APP_ABI)
  set(APP_ABI "ilp32")
endif()

# Give TOOLCHAIN_PREFIX a default value if its definition is missing
if(NOT DEFINED TOOLCHAIN_PREFIX)
  message(STATUS "[INFO] TOOLCHAIN_PREFIX set to default value: riscv32-unknown-elf-")
  set(TOOLCHAIN_PREFIX riscv32-unknown-elf-)
endif()

# RISC-V Assembler
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)

# RISC-V C Compiler
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)

# RISC-V C++ Compiler
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

# Find GCC
find_program(PATH_CMAKE_C_COMPILER
  ${CMAKE_C_COMPILER}
  PATHS ENV PATH
  REQUIRED
)

# Print whether GCC was found
if(NOT PATH_CMAKE_C_COMPILER)
  message(FATAL_ERROR "[ERROR] Could not find CMAKE_C_COMPILER: " ${PATH_CMAKE_C_COMPILER})
else()
  message(STATUS "[INFO] Compiling with GCC: " ${PATH_CMAKE_C_COMPILER})
endif()

set(SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/boot.S
  ${CMAKE_CURRENT_LIST_DIR}/trap_vector.S
  ${CMAKE_CURRENT_LIST_DIR}/trap_handlers.c
)

set(HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/csr.h
  ${CMAKE_CURRENT_LIST_DIR}/globals.h
  ${CMAKE_CURRENT_LIST_DIR}/gpio.h
  ${CMAKE_CURRENT_LIST_DIR}/mtimer.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel.h
  ${CMAKE_CURRENT_LIST_DIR}/spi.h
  ${CMAKE_CURRENT_LIST_DIR}/uart.h
)

add_library(steel STATIC ${SOURCES} ${HEADERS})

target_compile_options(steel
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -mstrict-align
    -march=${APP_ARCH}
    -mabi=${APP_ABI}
    -ffreestanding
    -ffunction-sections
    -fdata-sections
)

target_include_directories(steel
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
