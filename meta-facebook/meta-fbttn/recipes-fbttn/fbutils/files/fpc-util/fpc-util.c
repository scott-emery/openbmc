/*
 * fpc-util
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <stdint.h>
#include <pthread.h>
#include <facebook/bic.h>
#include <openbmc/pal.h>
#include <openbmc/ipmi.h>

static void
print_usage_help(void) {

  int sku = 0;
  sku = pal_get_iom_type();
  //SKU : 2 type7
  if (sku == 2) {
    printf("Usage: fpc-util <slot1> --ext1 <warning/off>\n");
    printf("Usage: fpc-util <slot1> --ext2 <warning/off>\n");
  }

  printf("       fpc-util <sled> --fault <on/off/blinking/auto/status>\n");
  printf("       fpc-util <slot1> --identify <on/off>\n");
}

int
main(int argc, char **argv) {

  uint8_t slot_id;
  char tstr[64] = {0};

  if (argc != 4 ) {
    goto err_exit;
  }

  if (!strcmp(argv[1], "slot1")) {
    slot_id = 1;
  } else if (!strcmp(argv[1] , "sled")) {
    slot_id = 0;
  } else {
    goto err_exit;
  }

  if (!strcmp(argv[2], "--ext2")) {
	  int LED = 0;
    printf("fpc-util: Enable/Disable miniSAS2 Error LED\n");
    if (!strcmp(argv[3], "warning")) {
		LED = 0;
		return pal_minisas_led(1,LED);

	} else if (!strcmp(argv[3], "off")) {

		LED = 1;
	    return pal_minisas_led(1,LED);
	}
    return -1;// to do pal_minisas_led()

  } else if (!strcmp(argv[2], "--ext1")) {

	  int LED = 0;
    printf("fpc-util: Enable/Disable miniSAS1 Error LED\n");

    if (!strcmp(argv[3], "warning")) {
		LED = 0;

		return pal_minisas_led(0,LED);

	} else if (!strcmp(argv[3], "off")) {
		LED = 1;

    return pal_minisas_led(0,LED);
	}
    return -1;// to do pal_minisas_led()

  } else if (!strcmp(argv[2], "--fault")) {

    if (strcmp(argv[1] , "sled"))
	    goto err_exit;

		int mode  = 0;

		if (!strcmp(argv[3], "on")) {

			printf("fpc-util: Enable fault LED manually\n");
			mode = 1;
			return pal_fault_led_mode(1,mode);

    } else if (!strcmp(argv[3], "off")) {

			printf("fpc-util: Disable fault LED manually\n");
			mode = 1;
			return pal_fault_led_mode(0,mode);

		} else if (!strcmp(argv[3], "blinking")) {

      printf("fpc-util: Enable fault LED Blinking\n");
      mode = 1;
      return pal_fault_led_mode(2,mode);

    } else if (!strcmp(argv[3], "auto")) {
			printf("fpc-util: Auto Mode\n");
			mode = 2;
			return pal_fault_led_mode(0,mode);

		} else if (!strcmp(argv[3], "status")) {

      printf("Fault LED Status: %s\n", (pal_fault_led_mode(3,3) == 0) ? "auto":"manual" );
      return 0;

    }
    return -1;

  } else if (!strcmp(argv[2], "--identify")) {

    printf("fpc-util: identification for %s is %s\n", argv[1], argv[3]);

    if (slot_id == 1) {
      sprintf(tstr, "identify_slot1");
    }

    return pal_set_key_value(tstr, argv[3]);

  } else {
    goto err_exit;
  }

  return 0;
err_exit:
  print_usage_help();
  return -1;
}
