# -----------------------------------------------------------------------------
# Copyright (c) 2020-2024 RISC-V Steel contributors
#
# This work is licensed under the MIT License, see LICENSE file for details.
# SPDX-License-Identifier: MIT
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

if(DEFINED STARTUP_DATA_ROM)
  add_compile_definitions(STARTUP_DATA_ROM=1)
endif()

if(DEFINED NO_STARTUP_FILES)
  add_compile_definitions(NO_STARTUP_FILES=1)
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
