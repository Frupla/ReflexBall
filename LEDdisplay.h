#ifndef _LEDdisplay_H_
#define _LEDdisplay_H_
#define LED_MAX_STR_LEN 100
	
void timerSetup();
void LEDInit();

void LEDSetString(rom
char*);
void LEDUpdate();
void LEDScroll();

int readMsec();

#endif /*! _LEDdisplay_H_ */
