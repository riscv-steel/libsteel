# -----------------------------------------------------------------------------
# Copyright (c) 2020-2024 RISC-V Steel contributors
#
# This work is licensed under the MIT License, see LICENSE file for details.
# SPDX-License-Identifier: MIT
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.15)

project(libsteel)

target_include_directories(steel
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)
