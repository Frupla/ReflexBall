#ifndef _LEDdisplay_H_
#define _LEDdisplay_H_
#define LED_MAX_STR_LEN 100
	
void timerSetup();
void LEDInit();
void LEDSetString(rom char* string);
void LEDUpdate();
void LEDScroll();

#endif /*! _LEDdisplay_H_ */
