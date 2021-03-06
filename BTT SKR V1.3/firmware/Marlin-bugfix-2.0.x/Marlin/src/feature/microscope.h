/**
 * MarlinMicroscope Firmware
 * Copyright (C) 2020 John van Noort
 *
 * Based on:
 *
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

// #include "../core/enum.h"

void start_logging();
void stop_logging();
void set_extruder_offset(const uint8_t extruder, const float offset);
void log_extruder_position(const uint8_t old_extruder, const uint8_t new_extruder);
void report_actual_axis_position();
void print_log();
