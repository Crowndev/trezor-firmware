/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (c) SatoshiLabs
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "display.h"
#include "memzero.h"

extern void main_clean_exit();

void __shutdown(void) {
  printf("SHUTDOWN\n");
  main_clean_exit();
}

#define COLOR_FATAL_ERROR RGB16(0x7F, 0x00, 0x00)

void __attribute__((noreturn))
__fatal_error(const char *expr, const char *msg, const char *file, int line,
              const char *func) {
  display_orientation(0);
  display_backlight(255);
  display_print_color(COLOR_WHITE, COLOR_FATAL_ERROR);
  display_printf("\nFATAL ERROR:\n");
  printf("\nFATAL ERROR:\n");
  if (expr) {
    display_printf("expr: %s\n", expr);
    printf("expr: %s\n", expr);
  }
  if (msg) {
    display_printf("msg : %s\n", msg);
    printf("msg : %s\n", msg);
  }
  if (file) {
    display_printf("file: %s:%d\n", file, line);
    printf("file: %s:%d\n", file, line);
  }
  if (func) {
    display_printf("func: %s\n", func);
    printf("func: %s\n", func);
  }
#ifdef GITREV
  display_printf("rev : %s\n", XSTR(GITREV));
  printf("rev : %s\n", XSTR(GITREV));
#endif
  display_printf("\nPlease contact TREZOR support.\n");
  printf("\nPlease contact TREZOR support.\n");
  hal_delay(3000);
  __shutdown();
  for (;;)
    ;
}

void __attribute__((noreturn))
error_shutdown(const char *line1, const char *line2, const char *line3,
               const char *line4) {
  display_clear();
  display_bar(0, 0, DISPLAY_RESX, DISPLAY_RESY, COLOR_FATAL_ERROR);
  int y = 32;
  if (line1) {
    display_text(8, y, line1, -1, FONT_NORMAL, COLOR_WHITE, COLOR_FATAL_ERROR);
    printf("%s\n", line1);
    y += 32;
  }
  if (line2) {
    display_text(8, y, line2, -1, FONT_NORMAL, COLOR_WHITE, COLOR_FATAL_ERROR);
    printf("%s\n", line2);
    y += 32;
  }
  if (line3) {
    display_text(8, y, line3, -1, FONT_NORMAL, COLOR_WHITE, COLOR_FATAL_ERROR);
    printf("%s\n", line3);
    y += 32;
  }
  if (line4) {
    display_text(8, y, line4, -1, FONT_NORMAL, COLOR_WHITE, COLOR_FATAL_ERROR);
    printf("%s\n", line4);
    y += 32;
  }
  y += 32;
  display_text(8, y, "Please unplug the device.", -1, FONT_NORMAL, COLOR_WHITE,
               COLOR_FATAL_ERROR);
  printf("\nPlease unplug the device.\n");
  display_backlight(255);
  hal_delay(5000);
  __shutdown();
  for (;;)
    ;
}

void hal_delay(uint32_t ms) { usleep(1000 * ms); }

uint8_t HW_ENTROPY_DATA[HW_ENTROPY_LEN];

void collect_hw_entropy(void) { memzero(HW_ENTROPY_DATA, HW_ENTROPY_LEN); }
