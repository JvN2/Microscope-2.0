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
 * 
 * Added functionality:
 * + report stppper positions with 3 digits accuracy and time stamp
 * + add start trigger
 * + keep track of extruder stepper positions 
 */

#include "../core/macros.h"
#include "../../Configuration.h"

#if ENABLED(MICROSCOPE_MODE)

#include "../core/serial.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../gcode/parser.h"

float extruder_position[EXTRUDERS] = {0};
float extruder_offset[EXTRUDERS] = {0};
uint8_t i = 0;
static char outstr[15];
unsigned long startMillis = 0;

uint8_t trigger_pin = 58;
uint8_t led_pin = 57;

unsigned long currentMillis = 0;
unsigned long logMillis = 0;
bool logging = false;

void start_logging()
{
  startMillis = millis();
  pinMode(trigger_pin, 1);
  digitalWrite(trigger_pin, 255);
  delay(100);
  digitalWrite(trigger_pin, 0);
  logging = true;
  logMillis = startMillis;
}

void stop_logging()
{
  logging = false;
}

void set_extruder_offset(const uint8_t extruder, const float offset)
{
  extruder_offset[extruder] = offset;
}

void log_extruder_position(const uint8_t old_extruder, const uint8_t new_extruder)
{
  extruder_position[old_extruder] = planner.get_axis_position_mm(E_AXIS);
  extruder_offset[old_extruder] = workspace_offset[E_AXIS];
  current_position[E_AXIS] = extruder_position[new_extruder];
  set_home_offset(E_AXIS, extruder_offset[new_extruder]);
}

void report_actual_axis_position()
{
  dtostrf(0.001 * (millis() - startMillis), 0, 3, outstr);
  SERIAL_ECHOPAIR("t:", outstr);

  dtostrf(LOGICAL_X_POSITION(planner.get_axis_position_mm(X_AXIS)), 0, 3, outstr);
  SERIAL_ECHOPAIR(" X:", outstr);

  dtostrf(LOGICAL_Y_POSITION(planner.get_axis_position_mm(Y_AXIS)), 0, 3, outstr);
  SERIAL_ECHOPAIR(" Y:", outstr);

  dtostrf(LOGICAL_Z_POSITION(planner.get_axis_position_mm(Z_AXIS)), 0, 3, outstr);
  SERIAL_ECHOPAIR(" Z:", outstr);

  extruder_position[active_extruder] = planner.get_axis_position_mm(E_AXIS);
  LOOP_L_N(i, EXTRUDERS)
  {
    set_home_offset(E_AXIS, extruder_offset[i]);
    SERIAL_ECHOPAIR(" E", i);
    dtostrf(LOGICAL_E_POSITION(extruder_position[i]), 0, 3, outstr);
    SERIAL_ECHOPAIR(":", outstr);
  }
  set_home_offset(E_AXIS, extruder_offset[active_extruder]);
  SERIAL_EOL();
}

void print_log()
{
  currentMillis = millis();
  // if ((currentMillis > logMillis) && (logging == true))
  if (logging == true)
  {
    SERIAL_ECHO_START();
    report_actual_axis_position();
    logMillis += 250;
  }
}

#endif
