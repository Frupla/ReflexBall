#include <eZ8.h>                          // special encore constants, macros and flash functions
#include <sio.h>                          // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "Z8encore.h"
#include "engine.h"
#include "standalone_timer.h"
#include "levels.h"

#define LVLSIZE 100     // The size of the levels. Should probably be in engine.h
#define STDWINDOWX 10   // The standard placement for the upper left corner for the windows.
#define STDWINDOWY 10   // The standard placement for the upper left corner for the windows.

rom char main_title[13] = " Main Menu ";
rom char main_points[56] = "Start game\nShow controls\nShow high scores";
rom char lvl_title[15] = " Choose level ";
rom char lvl_points[35] = "Level 1\nLevel 2\nLevel 3\nGo back";
rom char highscore_title[13] = " High score ";
rom char help_title[8] = " Help! ";
rom char difficulty_title[13] = " Difficulty ";
rom char difficulty_points[21] = "Hard\nMedium\nEasy ";


//Takes a pointer to a char string title of the menu
// and a char string of all the subpoints seperated by \n
//returns a char output, which is the choice the player made
char menu(rom char *title, rom char *points) {
    /*
     * Separate each new menu point with a newline
    */
    int i;
    char stay_flag = 1, button_flag = 1, button_press, output = 0, number_of_points = 0;

    // Draw the menu
    LEDUpdate();
    window(STDWINDOWX, STDWINDOWY, STDWINDOWX + 40, STDWINDOWY + 15, title, 1);
    gotoxy(STDWINDOWX + 5, STDWINDOWY + 2);
    for (i = 0; points[i] != 0x00; i++) {
        if (points[i] != '\n') {
            printf("%c", points[i]);
        } else {
            number_of_points++;
        	gotoxy(STDWINDOWX + 5, STDWINDOWY + 2 + number_of_points);
		}
    }
    gotoxy(STDWINDOWX + 3, STDWINDOWY + 2);
    printf(">");

    // Navigate the menu
    do {
        LEDUpdate();
        button_press = readKey();
        if (button_flag && button_press) {
            button_flag = 0;
            switch (button_press) {
                case 0x01 :
                    if (output < number_of_points && output >= 0) {
                        output += 1;
                        gotoxy(STDWINDOWX + 3, STDWINDOWY + 2 + output - 1);
                        printf(" ");
                        gotoxy(STDWINDOWX + 3, STDWINDOWY + 2 + output);
                        printf(">");
                        gotoxy(1, 1);
                    }
                    break;
                case 0x04 :
                    if (output <= number_of_points && output > 0) {
                        output -= 1;
                        gotoxy(STDWINDOWX + 3, STDWINDOWY + 2 + output + 1);
                        printf(" ");
                        gotoxy(STDWINDOWX + 3, STDWINDOWY + 2 + output);
                        printf(">");
                        gotoxy(1, 1);
                    }
                    break;
                case 0x02 :
                    stay_flag = 0;
                    break;
            }
        } else if (!(button_press || button_flag)) {
            button_flag = 1;
        }
    } while (stay_flag);

    return output;

}

// Takes nothing and returns difficulty
// Runs menu allowing player to chose a difficulty
char difficulty(){
	return	menu(difficulty_title, difficulty_points) + 1;
}

//Takes a char indicating which level should be played.
//Returns an int indicating the score at the end of the game.
//Runs a game & controls LEDs.
int startGame(char lvl){
	player_t player[3];
	ball_t ball[5];
	int i,j,n;
	breakable_t breakable[LVLSIZE];
	char button, whatDidTheyHit;
	int score = 0;
    int max_score = 0;
    int time1 = 1, time2 = 15;
	char string[LED_MAX_STR_LEN] = " x/x";
	string[0] = 0x7F; // places a heart shape at the beginning of the string
	LEDScrolloff(); //stops the scrolling
    //Choose level
	player[0].lives = difficulty();
	clrscr();
    switch (lvl) {
        case 0x01:
            level1(breakable, ball, player);
            initiateWithCeiling();
            break;
        case 0x02:
            level2(breakable, ball, player);
            initiateWithoutCeiling();
            break;
        case 0x03:
            level3(breakable, ball, player);
            initiateWithCeiling();
            break;
        default:
            player[0].lives = 0;
    }
	max_score = findMaxScore(breakable);
	drawMap(player, ball, breakable);

	string[1] = player[0].lives + 0x30;
	string[3] = player[0].lives + 0x30;
	timer1Setup();
	LEDSetString(string);

	gotoxy(5,62);
    printf("score:");
    gotoxy(5,63);
    printf("out of:   %d", max_score);
	do {
        do {
            do {
				//reset timer
				if (timer1() < 50) {
            		time1 = 1;
            		time2 = 15;
        		}
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            button = readKey();
            //Then move the player
            LEDUpdate();
            playerMovement(button, player);
            LEDUpdate();
            //Reenter the above while - loop
            time1=timer1();
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball

        whatDidTheyHit = ballMovement(ball, player, breakable, lvl);
        LEDUpdate();
        if (string[1]-0x30 != player[0].lives) {
            string[1] = player[0].lives+0x30;
            LEDSetString(string);
        }
        //Checks if it hit anything, and if so, how much
        if (whatDidTheyHit & 0x0F) {
            score += (whatDidTheyHit & 0x0F);
            gotoxy(15, 62);
                printf("%d", score);
            if (score >= max_score) {
                    player[0].lives = 0;
                }
        }
        //Checks if any balls died
        if (whatDidTheyHit & 0xF0) {
            player[0].lives -= (whatDidTheyHit & 0x10)>>4;
            player[0].lives -= (whatDidTheyHit & 0x20)>>5;
            player[0].lives -= (whatDidTheyHit & 0x40)>>6;
            player[0].lives -= (whatDidTheyHit & 0x80)>>7;
            for (j = 0; ball[j].whatIsThis != 0x00; j++) {
                gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball[j].x1), EIGHTEEN_FOURTEEN_TO_INT(ball[j].y1));
                printf(" ");
            }
            if (whatDidTheyHit & 0x10) {
                ball[0].x1 = ball[0].xs;
                ball[0].y1 = ball[0].ys;
            }
            if (whatDidTheyHit & 0x20) {
                ball[1].x1 = ball[1].xs;
                ball[1].y1 = ball[1].ys;
            }
            if (whatDidTheyHit & 0x40) {
                ball[2].x1 = ball[2].xs;
                ball[2].y1 = ball[2].ys;
            }
            if (whatDidTheyHit & 0x80) {
                ball[3].x1 = ball[3].xs;
                ball[3].y1 = ball[3].ys;
            }
            //LEDSetString(string);
        }

        LEDUpdate();

        //Reenter above loop
        time2 = timer1() + 15;

    } while (player[0].lives);
    return score;
}


//Takes a pointer to integer score and a pointer to an array of ints highscores.
//Find the lowest value in the array. If it is lower than the score, the score replaces the lowest value in the array.
void addHighscore(int * score, int * highscore){
    int i;
    int positionof_lowest = 0;
    for(i = 1; i < 5; i++){
        if(highscore[positionof_lowest] > highscore[i]){
            positionof_lowest = i;  
        }
    }
    if (*score > highscore[positionof_lowest]){
        highscore[positionof_lowest] = *score;
        gotoxy(20,62);
        printf("You made the highscore");
    }
}


//Takes a pointer to an array of highscores and a char indicating where it should be drawn.
//Returns nothing.
//Sorts the contents of the array and prints it.
void bubbleSortAndPrint(int * highscore, int howManySpacesToTheLeft){
	int i, j, temp;
    for(i = 0; i < 4; i++){
        for (j = 0; j < 4-i; j++){
            if(highscore[j] < highscore[j+1]){
                temp = highscore[j+1];
                highscore[j+1] = highscore[j];
                highscore[j] = temp; 
            }
        }     
   	}
	for (i = 0; i < 5; i++){
        gotoxy(12 + howManySpacesToTheLeft,12 +i);
        printf("%d.  %d", i+1, highscore[i]);
    }
}


//Takes three arrays of highscores.
//Returns nothing.
//Prints the three highscores using bubbleSortAndPrint
void printHighscore(int * highscore1, int * highscore2, int * highscore3){
    int flag = 1, i;
	// bubble sort
	window(STDWINDOWX, STDWINDOWY, STDWINDOWX + 40, STDWINDOWY + 15, highscore_title, 1);
	gotoxy(12,11);
	printf("Level 1:  Level 2:  Level 3:");
	bubbleSortAndPrint(highscore1, 0);
	bubbleSortAndPrint(highscore2, 10);
	bubbleSortAndPrint(highscore3, 20);   
    do{
        LEDUpdate();
        if (readKey() == 0x00){
            flag = 0;
        }
    }while(!readKey() || (flag));
    for (i = 0; i >=23; i++){
         gotoxy(20+i,62);
         printf(" ");
    }
}

// Takes nothing.
// Returns nothing.
// Draw the window show controls.
void showControls() {
    int i, flag = 1;
    window(STDWINDOWX, STDWINDOWY, STDWINDOWX + 40, STDWINDOWY + 15, help_title, 1);
    gotoxy(12, 11);
    printf("You have three buttons:");
    gotoxy(12, 12);
    printf(" < - Left   ^ - Action  > - Right\r\n");
    gotoxy(12, 14);
    printf("Hit these:   With this:");
    gotoxy(12, 16);
    fgcolor(5);
    for (i = 0; i < 8; i++) {
        printf("%c", BREAKABLETEXTURE);
    }
    gotoxy(30, 16);
    fgcolor(0x0a);
    printf("%c", BALLTEXTURE);
    fgcolor(STDTEXTCOLOR);
    gotoxy(12, 18);
    printf("This will make the paddle larger:");
	gotoxy(12,19);
	fgcolor(5);
	for(i = 0; i<8; i++){
		printf("%c", WIDERPADDLETEXTURE);
	}
	fgcolor(STDTEXTCOLOR);
	gotoxy(12, 20);
    printf("This will give you more health:"); // to a maximum of 9
	gotoxy(12,21);
	fgcolor(5);
	for (i = 0; i < 8; i++) {
        printf("%c", MORELIFETEXTURE);
	}
    fgcolor(STDTEXTCOLOR);
	gotoxy(12, 22);
	printf("This will give you more balls:"); // to a maxiumum of 4
	gotoxy(12,23);
	fgcolor(5);
	for(i = 0; i < 8; i++){
		printf("%c", MOREBALLSTEXTURE);
    }
	fgcolor(STDTEXTCOLOR);
	do {
        LEDUpdate();
        if (readKey() == 0x00) {
            flag = 0;
        }
    } while (!readKey() || (flag));
}


//Takes the int arrays of highscores for each level
//Teturns the score in the form of an int
//Calls a function to draw the level menu and starts a game if that is selected
int lvlMenu(int * highscore1, int * highscore2, int * highscore3) {
    char output = 0;
    int score = 0;

    output = menu(lvl_title, lvl_points);

    switch (output) {
        case 0:
            clrscr();
            LEDScrolloff();

            score = startGame(0x01);

            addHighscore(&score, highscore1);
            printHighscore(highscore1, highscore2, highscore3);
            break;
        case 1:
            clrscr();
            LEDScrolloff();

            score = startGame(0x02);

            addHighscore(&score, highscore2);
            printHighscore(highscore1, highscore2, highscore3);
            break;
        case 2:
            clrscr();
            LEDScrolloff();

            score = startGame(0x03);

            addHighscore(&score, highscore3);
            printHighscore(highscore1, highscore2, highscore3);
            break;
        case 3:
            clrscr();
            break;
    }
    return score;
}


void main() {
    int highscore1[5] = {0,0,0,0,0};
    int highscore2[5] = {0,0,0,0,0};
    int highscore3[5] = {0,0,0,0,0};
    int score = 0;
    char output = 0;
    char string[LED_MAX_STR_LEN] = "    Main menu";
    init_uart(_UART0, _DEFFREQ, _DEFBAUD);  // set-up UART0 to 57600, 8n1
    clrscr();
    LEDInit();
    LEDScrollon();
    LEDSetString(string);
	
    do {
        fgcolor(STDTEXTCOLOR);
        clrscr();
        //new text on LED
		string[0] = ' ';
		string[1] = ' ';
		string[2] = ' ';
		string[3] = ' ';
        string[4] = 'M';
		string[5] = 'a';
		string[6] = 'i';
		string[7] = 'n';
		string[8] = ' ';
		string[9] = 'm';
		string[10] = 'e';
		string[11] = 'n';
		string[12] = 'u';
		string[13] = '\0';
		LEDSetString(string);

        output = menu(main_title, main_points);

        switch (output) {
            case 0:
                clrscr();
                //new text on LED
				string[0] = ' ';
				string[1] = ' ';
				string[2] = ' ';
				string[3] = ' ';
   				string[4] = 'L';
				string[5] = 'e';
				string[6] = 'v';
				string[7] = 'e';
				string[8] = 'l';
				string[9] = ' ';
				string[10] = 's';
				string[11] = 'e';
				string[12] = 'l';
				string[13] = 'e';
				string[14] = 'c';
				string[15] = 't';
				string[16] = '\0';
				LEDSetString(string);

                score = lvlMenu(highscore1, highscore2, highscore3);

                LEDScrollon(); // Restart scroll
                break;
            case 1:
                //new text on LED
				string[0] = ' ';
				string[1] = ' ';
				string[2] = ' ';
				string[3] = ' ';
                string[4] = 'C';
				string[5] = 'o';
				string[6] = 'n';
				string[7] = 't';
				string[8] = 'r';
				string[9] = 'o';
				string[10] = 'l';
				string[11] = 's';
				string[12] = '\0';
                LEDSetString(string);
                showControls();
                break;
            case 2:
                //new text on LED
				string[0] = ' ';
				string[1] = ' ';
				string[2] = ' ';
				string[3] = ' ';
                string[4] = 'H';
				string[5] = 'i';
				string[6] = 'g';
				string[7] = 'h';
				string[8] = ' ';
				string[9] = 's';
				string[10] = 'c';
				string[11] = 'o';
				string[12] = 'r';
				string[13] = 'e';
				string[14] = '\0';
				LEDSetString(string);
                printHighscore(highscore1, highscore2, highscore3);
                break;
        }
    } while (1);
}

