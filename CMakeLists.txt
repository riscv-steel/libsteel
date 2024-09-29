# -----------------------------------------------------------------------------
# Copyright (c) 2020-2024 RISC-V Steel contributors
#
# This work is licensed under the MIT License, see LICENSE file for details.
# SPDX-License-Identifier: MIT
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project(libsteel)

set(HEADERS
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/csr.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/globals.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/gpio.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/mtimer.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/spi.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel/uart.h
  ${CMAKE_CURRENT_LIST_DIR}/libsteel.h
)

add_library(steel STATIC ${HEADERS})

set_target_properties(steel PROPERTIES LINKER_LANGUAGE C)

target_include_directories(steel
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)
