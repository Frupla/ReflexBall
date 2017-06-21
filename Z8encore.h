#ifndef _Z8encore_H_
#define _Z8encore_H_
#define LED_MAX_STR_LEN 20
	
void timerSetup();
void LEDInit();

void LEDSetString(char *string);
void LEDUpdate();
void LEDScroll();

char readKey();
int readMsec();

#endif /*! _Z8encore_H_ */
