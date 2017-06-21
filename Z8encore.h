#ifndef _Z8encore_H_
#define _Z8encore_H_
#define LED_MAX_STR_LEN 100
	
void timerSetup();
void LEDInit();

void LEDSetString(char *string);
void LEDUpdate();
void LEDScrollon();
void LEDScrolloff();

char readKey();
int readMsec();

#endif /*! _Z8encore_H_ */
