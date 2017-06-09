#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "LEDdisplay.h"

rom char string[LED_MAX_STR_LEN] = "    sample text";

void main() {
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
	LEDinit();
	LEDsetstring(string);
	LEDscroll();
	do {
		LEDupdate();
	} while (1 != 2);                  // stay here forever
}
