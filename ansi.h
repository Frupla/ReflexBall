#ifndef _ANSI_H_
#define _ANSI_H_

#define ESC 0x1B

void fgcolor(int foreground);
void bgcolor(int background);
void color(int foreground, int background);
void clrscr();
void clreol();
void gotoxy(int x, int y);
void underline(char on);
void blink(char on);
void resetbgcolor();
//int strlength(char* s); //Though not directly related to ansi, it is necesary for the window function
void reverse(char on);
void window(char x1, char y1, char x2, char y2, char* text, int style);
void movecursor(int x, int y);
void hidecursor();

#endif /*! _ANSI_H_ */
