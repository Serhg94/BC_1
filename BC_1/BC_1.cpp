/*
 * BC_1.cpp
 *
 * Created: 25.07.2014 17:12:04
 *  Author: Serhg
 */ 


#include <avr/io.h>
#include "main.h"

void setup(){
	wdt_enable(WDTO_500MS);
	D2_Out;
	D2_Low;
	sei(); // разрешаем прерывани€
	init(); // инициализаци€ таймеров ардуины
	UART_Init(115200);
	D13_Out;
	lifeled = true;
	for(int i=0;i<=MK_COUNT;i++){
		buffer_sizes[i] = 0;
		alive[i] = 0;
	}
}

//-1 не изменилось 0-не прочиталось, номер изменившегос€ 02/0000111100/212/1111/30/29/1111/226/222/
int requestStatus(int sn)
{
	uint8_t stats[20];
	int statsize = 0;
	const uint8_t buf[6] = {(uint8_t)sn, (uint8_t)sn, (uint8_t)sn, (uint8_t)sn, end1, end2};
	D0_High;
	D2_High;
	delay(1);
	UART_SendArray(buf, 6);
	delay(1);
	D2_Low;
	D0_Low;
	delay(5);
	while (UART_ReadByte(stats[statsize]))
	{
		//stats[statsize] = (uint8_t)Serial1.read();
		statsize++;
		if (statsize > 12) statsize = 0;
		if ((statsize > 1)&&(stats[statsize-2]==end1)&&(stats[statsize-1]==end1))
		{
			break;
		}
	}
	if (statsize == 12)
	{
		if (stats[0]==sn)
		if ((uint8_t)(0-(stats[0]+stats[1]+stats[2]+stats[3]+stats[4]+stats[5]+stats[6]+stats[7]+stats[8])) == (uint8_t)stats[9])
		{
			
			bool changed = false;
			String str = "";
			if (stats[1]&B10000000) str += '1'; else str += '0';
			if (stats[1]&B01000000) str += '1'; else str += '0';
			if (stats[1]&B00100000) str += '1'; else str += '0';
			if (stats[1]&B00010000) str += '1'; else str += '0';
			if (stats[1]&B00001000) str += '1'; else str += '0';
			if (stats[1]&B00000100) str += '1'; else str += '0';
			if (stats[1]&B00000010) str += '1'; else str += '0';
			if (stats[1]&B00000001) str += '1'; else str += '0';
			if (stats[2]&B01000000) str += '1'; else str += '0';
			if (stats[2]&B00100000) str += '1'; else str += '0';
			if (stats[2]&B00010000) str += '1'; else str += '0';
			if (stats[2]&B00001000) str += '1'; else str += '0';
			if (stats[2]&B00000100) str += '1'; else str += '0';
			if (stats[2]&B00000010) str += '1'; else str += '0';
			if (stats[2]&B00000001) str += '1'; else str += '0';
			if (sets[sn] != str) changed = true;
			sets[sn] = str;
			stat[sn][0] = 0;
			stat[sn][0] |= (int)stats[3] << 8;
			stat[sn][0] |= stats[4];
			str = "";
			if (stats[5]&B10000000) str += '1'; else str += '0';
			if (stats[5]&B01000000) str += '1'; else str += '0';
			if (stats[5]&B00100000) str += '1'; else str += '0';
			if (stats[5]&B00010000) str += '1'; else str += '0';
			if (rebs[sn] != str) changed = true;
			rebs[sn] = str;
			stat[sn][1] = stats[6];
			stat[sn][2] = stats[7];
			str = "";
			if (stats[8]&B10000000) str += '1'; else str += '0';
			if (stats[8]&B01000000) str += '1'; else str += '0';
			if (stats[8]&B00100000) str += '1'; else str += '0';
			if (stats[8]&B00010000) str += '1'; else str += '0';
			if (butt[sn] != str) changed = true;
			butt[sn] = str;
			if (changed == false) return -1;
			return sn;
		}
	}
	//Serial.println(String(looper)+"->"+String(statsize));
	return 0;
}
char invers(char c){
	if (c=='1')
	return '0';
	return '1';
}


//1 - выполнено, 0- не выполнено
int checkSet(int sn, String sen){
	if (((sen[0]=='2')||(sen[0]==sets[sn][0]))&&
	((sen[1]=='2')||(sen[1]==sets[sn][1]))&&
	((sen[2]=='2')||(sen[2]==sets[sn][2]))&&
	((sen[3]=='2')||(sen[3]==sets[sn][3]))&&
	((sen[4]=='2')||(sen[4]==sets[sn][4]))&&
	((sen[5]=='2')||(sen[5]==sets[sn][5]))&&
	((sen[6]=='2')||(sen[6]==sets[sn][6]))&&
	((sen[7]=='2')||(sen[7]==sets[sn][7]))&&
	((sen[8]=='2')||(sen[8]==sets[sn][8]))&&
	((sen[9]=='2')||(sen[9]==sets[sn][9]))&&
	((sen[10]=='2')||(sen[10]==sets[sn][10]))&&
	((sen[11]=='2')||(sen[11]==sets[sn][11]))&&
	((sen[12]=='2')||(sen[12]==sets[sn][12]))&&
	((sen[13]=='2')||(sen[13]==sets[sn][13]))&&
	((sen[14]=='2')||(sen[14]==sets[sn][14])))
	return 1;
	return 0;
}
//1 - отправил, 0- не отправил
int sendSet(int sn, String sen, int del)
{
	uint8_t buf[6] = {B00000000, B00000000, B00000000, B00000000, end1, end2};
	buf[0] = (uint8_t)sn;
	if ((sen[0] == '2')&&(sets[sn][0]=='1')) buf[1] |= B10000000;
	if ((sen[1] == '2')&&(sets[sn][1]=='1')) buf[1] |= B01000000;
	if ((sen[2] == '2')&&(sets[sn][2]=='1')) buf[1] |= B00100000;
	if ((sen[3] == '2')&&(sets[sn][3]=='1')) buf[1] |= B00010000;
	if ((sen[4] == '2')&&(sets[sn][4]=='1')) buf[1] |= B00001000;
	if ((sen[5] == '2')&&(sets[sn][5]=='1')) buf[1] |= B00000100;
	if ((sen[6] == '2')&&(sets[sn][6]=='1')) buf[1] |= B00000010;
	if ((sen[7] == '2')&&(sets[sn][7]=='1')) buf[1] |= B00000001;
	if ((sen[8] == '2')&&(sets[sn][8]=='1')) buf[2] |= B01000000;
	if ((sen[9] == '2')&&(sets[sn][9]=='1')) buf[2] |= B00100000;
	if ((sen[10] == '2')&&(sets[sn][10]=='1')) buf[2] |= B00010000;
	if ((sen[11] == '2')&&(sets[sn][11]=='1')) buf[2] |= B00001000;
	if ((sen[12] == '2')&&(sets[sn][12]=='1')) buf[2] |= B00000100;
	if ((sen[13] == '2')&&(sets[sn][13]=='1')) buf[2] |= B00000010;
	if ((sen[14] == '2')&&(sets[sn][14]=='1')) buf[2] |= B00000001;
	if ((sen[0] == '1')) buf[1] |= B10000000;
	if ((sen[1] == '1')) buf[1] |= B01000000;
	if ((sen[2] == '1')) buf[1] |= B00100000;
	if ((sen[3] == '1')) buf[1] |= B00010000;
	if ((sen[4] == '1')) buf[1] |= B00001000;
	if ((sen[5] == '1')) buf[1] |= B00000100;
	if ((sen[6] == '1')) buf[1] |= B00000010;
	if ((sen[7] == '1')) buf[1] |= B00000001;
	if ((sen[8] == '1')) buf[2] |= B01000000;
	if ((sen[9] == '1')) buf[2] |= B00100000;
	if ((sen[10] == '1')) buf[2] |= B00010000;
	if ((sen[11] == '1')) buf[2] |= B00001000;
	if ((sen[12] == '1')) buf[2] |= B00000100;
	if ((sen[13] == '1')) buf[2] |= B00000010;
	if ((sen[14] == '1')) buf[2] |= B00000001;
	buf[3] = 0-(buf[0]+buf[1]+buf[2]);
	//Serial.write(buf, 5);
	D0_High;
	D2_High;
	delay(1);
	UART_SendArray(buf, 6);
	delay(1);
	D2_Low;
	D0_Low;
	return 1;
}


void deleteTask(int sn){
	if(buffer_sizes[sn]>1)
	for(int i=0;i<buffer_sizes[sn]-1;i++)
	buffer[sn][i]=buffer[sn][i+1];
	if(buffer_sizes[sn]>0)
	buffer_sizes[sn]--;
}

int main(void)
{
    while(1)
    {
          looper++;
          wdt_reset();
          char inChar;
          while (UART_ReadByte(&inChar))
          {
	          // читаем по байтно из порта
	          //char inChar = (char)Serial.read();
	          inputString += inChar;
	          // ѕолучили знак конца строки - идем обрабатывать
	          if (inChar == '\n') {
		          stringComplete = true;
	          }
          }
          
          
          if (stringComplete)
          {
	          if (inputString.substring(0,3)=="clr")
	          {
		          for(int i=0; i<10; i++)
		          butt[i]="----";
	          }
	          else
	          if (inputString.substring(2,5)=="set")
	          {
		          int num = inputString.substring(0,2).toInt();
		          bool check = true;
		          String sen = inputString.substring(5,inputString.length());
		          sen.replace('\n', '2');
		          while(sen.length()<15) sen+='2';
		          for(int i=0; i<sen.length(); i++){
			          if ((sen[i]!='1')&&(sen[i]!='0')&&(sen[i]!='2')&&(sen[i]!='3')) check = false;
			          if (sen[i]=='3') sen[i]=invers(sets[num][i]);
		          }
		          if (check){
			          buffer[num][buffer_sizes[num]] = sen;
			          buffer_sizes[num]++;
			          if (buffer_sizes[num]>BUFFER_SIZE)
			          buffer_sizes[num] = 0;
		          }
		          //  int str = sendSet(inputString.substring(0,2).toInt(), inputString.substring(5,inputString.length()), SEND_SETS_DELAY);
	          }
	          inputString = "";
	          
	          stringComplete = false;
          }
          //  if (stringComplete3)
          //  {
          //    if (inputString3.substring(0,3)=="clr")
          //    {
          //      for(int i=0; i<10; i++)
          //        butt[i]="----";
          //    }
          //    else
          //    if (inputString3.substring(2,5)=="set")
          //    {
          //      int str = sendSet(inputString3.substring(0,2).toInt(), inputString3.substring(5,inputString3.length()), SEND_SETS_DELAY);
          //    }
          //    inputString3 = "";
          //    stringComplete3 = false;
          //  }
          if ((buffer_sizes[looper]>0)&&(checkSet(looper, buffer[looper][0])==1)&&(alive[looper]==true))
          {
	          deleteTask(looper);
          }
          if ((buffer_sizes[looper]>0)&&(alive[looper]==true))
          {
	          //Serial.println( alive[looper]);
	          int str = sendSet(looper, buffer[looper][0], SEND_SETS_DELAY);
          }
          {
	          int sn=requestStatus(looper);
	          if (sn==looper)
	          {
		          String str = String(sn)+'/'+sets[sn]+'/'+stat[sn][0]+'/'+rebs[sn]+'/'+stat[sn][1]+'/'+stat[sn][2]+'/'+butt[sn]+'/';
		          Serial.println(str);
		          Serial.flush();
		          //      Serial3.println(str);
		          //      Serial3.flush();
	          }
	          if (sn==0) alive[looper] = false;
	          else alive[looper] = true;
          }
          if (looper == MK_COUNT)
          {
	          looper = 0;
	          D13_Inv;
          }
    }
}