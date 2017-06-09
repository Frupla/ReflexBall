#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines

#define ESC 0x1B

void clrscr(){
	printf("%c[2J", 27);
}

void clreol(){
	printf("%c[K", 27);
}

void gotoxy(int x, int y){
	printf("%c[%d;%dH", 27,y,x);
}

void underline(char on){
	if(on == 49){
		printf("%c[4m", 27);	
	} else{
		printf("%c[24m", 27);	
	}
}

void blink(char on){
	if(on == 49){
		printf("%c[5m", 27);	
	} else{
		printf("%c[25m", 27);	
	}
}

void fgcolor(int foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  int type = 22;             // normal text
	if (foreground > 7) {
	 	type = 1;                // bold text
	 	foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(int background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color      
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(int foreground, int background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
  int type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
  printf("%c[m", ESC);  
}

void reverse(char on){
	if(on == 49){
		printf("%c[7m", 27);
	} else{
		printf("%c[27m", 27);	
	}	
}

int strlength(char* s){
	int i=0;
	while(s[i]!='\0'){
 	i++;}
	return i;
}

//pre: x1 < x2, y1<y2, text < x2-4
void window(char x1, char y1, char x2, char y2, char* text, int style){
	int i;
	int j;
	int n;
	int m;
	char kant[8] = {204, 186, 185, 205, 187, 200, 188, 201};
	if (style){
		kant[0] = 195; kant[1] = 179; kant[2] = 180; kant[3] = 196; kant[4] = 191; kant[5] = 192; kant[6] = 217; kant[7] = 218;
	}
	gotoxy(x1,y1);
	printf("%c",kant[7]);
	printf("%c",kant[1]);
	reverse('1');
	printf("%s",text);
	reverse('0');
	printf("%c",kant[0]);
	//middel part
	for (j = x1+2+strlength(text); j < x2-1;j++){
		printf("%c",kant[3]);
		}
	printf("%c",kant[4]);
	for (i = y1+1; i < y2; i++){
		gotoxy(x1,i);
		printf("%c",kant[1]);
		for (m=x1+1; m <x2;m++){
			printf(" ");
		}
		gotoxy(x2,i);
		printf("%c",kant[1]);
	}
	//last line
	gotoxy(x1,y2);
	printf("%c",kant[5]);
	for (n=x1+1; n <x2; n++){
		printf("%c",kant[3]);
	}
	printf("%c",kant[6]);
	gotoxy(x1+1,y1+1);
}

//moves curser by two integers (x,y)
void moveCursor(int x, int y){
	//+ 48
	if (y >= 0 ){
		printf("%c[%cB", 27, y+48);
	} else {
		printf("%c[%cA", 27, (-y)+48);
	}
	if (x >= 0 ){
		printf("%c[%cC", 27, x+48);
	} else {
		printf("%c[%cD", 27, (-x)+48);
	}
}

void hideCursor(){
	printf("%c[25l", 27);
}
