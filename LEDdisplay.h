#ifndef _LEDdisplay_H_
#define _LEDdisplay_H_
#define LED_MAX_STR_LEN 100
	
void timersetup();
void LEDinit();
void LEDsetstring(rom char* string);
void LEDupdate();
void LEDscroll();

#endif /*! _LEDdisplay_H_ */
