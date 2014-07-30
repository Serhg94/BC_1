//-------------------------------------------------------------------
#ifndef __rc_1_main_H__
#define __rc_1_main_H__
//-------------------------------------------------------------------
 #include "conf.h"
#include "CyberLib.h"
#include <avr/wdt.h>
#include "times.h"
#include <String>
//
//

const uint8_t end1 = B11111110;
const uint8_t end2 = B11111101;
String inputString = "";         // сюда будем совать входные строки из эвента порта
bool stringComplete = false;  // покажет конец строки
String inputString3 = "";         // сюда будем совать входные строки из эвента порта
bool stringComplete3 = false;
bool lifeled;
int stat[10][20];
String sets[10];
String butt[10];
String rebs[10];
int buffer_sizes[MK_COUNT+1];
bool alive[MK_COUNT+1];
String buffer[MK_COUNT+1][BUFFER_SIZE];

int looper = 0;
long int lifetm;
 



//-------------------------------------------------------------------
void setup();
int requestStatus(int sn);
char invers(char c);
int checkSet(int sn, String sen);
int sendSet(int sn, String sen, int del);
void deleteTask(int sn);
int main();

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
 
