#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <windows.h>

unsigned int delayTime = 0;

char date[6];

unsigned int registers[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09 };

void interrupt(*oldTimer)(...);
void interrupt(*oldAlarm)(...);
void interrupt(*testAlarm)(...);

void getCurrentTime();
void setCurrentTime();
void setDelay(unsigned int);
void setAlarm();
void inputTime();
int bcdToDec(int bcd);
int decToBcd(int dec);

void interrupt newTimer(...) {
	delayTime++;

	outp(0x20, 0x20);
	outp(0xA0, 0x20);
}

void interrupt newAlarm(...) {
	printf("Alarm");
	oldAlarm();
}

int main() {
	int delay;
	while (1) {
		printf("Enter:\n");
		printf("1. to get current time\n");
		printf("2. to set current time\n");
		printf("3. to set delay\n");
		printf("4. to set alarm\n");
		printf("0. to exit\n");

		switch (getch()) {
		case '1':
			getCurrentTime();
			break;
		case '2':
			setCurrentTime();
			break;
		case '3':
			fflush(stdin);
			printf("Input delay (in millisecond): ");
			scanf("%d", &delay);
			setDelay(delay);
			break;
		case '4':
			setAlarm();
			break;
		case '0':
			return 0;
		default:
			system("cls");
			break;
		}
	}
}

void getCurrentTime() {
	int decDate[6];
	int i = 0;
	char *monthTxt[] = { "January", "Februart", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	system("cls");

	for (i = 0; i < 6; i++)
	{
		outp(0x70, registers[i]);
		date[i] = inp(0x71);
	}

	for (i = 0; i < 6; i++)
	{
		decDate[i] = bcdToDec(date[i]);
	}

	printf(" %2d:%2d:%2d", decDate[2], decDate[1], decDate[0]);
	printf(" %s, %2d, 20%2d\n", monthTxt[decDate[4] - 1], decDate[3], decDate[5]);
}

void setCurrentTime() {
	inputTime();

	disable();

	do {
		outp(0x70, 0xA);
	} while (inp(0x71) & 0x80);

	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x80);

	for (int i = 0; i < 4; i++) {
		outp(0x70, registers[i]);
		outp(0x71, date[i]);
	}

	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0x7F);

	enable();

	system("cls");
}

void setDelay(unsigned int ms) {
	disable();

	oldTimer = getvect(0x70);
	setvect(0x70, newTimer);

	enable();

	outp(0xA1, inp(0xA1) & 0xFE); //razmaskir linii signala zaprosa

	outp(0x70, 0xB); // Vkl periodich prerivania
	outp(0x71, inp(0x71) | 0x40);

	delayTime = 0;
	while (delayTime <= ms);

	puts("Delay's end");
	setvect(0x70, oldTimer);
	return;
}

void setAlarm() {
	inputTime();

	disable();

	do {
		outp(0x70, 0xA);
	} while (inp(0x71) & 0x80);

	outp(0x70, 0x05);
	outp(0x71, date[2]);

	outp(0x70, 0x03);
	outp(0x71, date[1]);

	outp(0x70, 0x01);
	outp(0x71, date[0]);

	outp(0xA1, inp(0xA1) & 0xFE); //razmaskir linii signala zaprosa

	outp(0x70, 0xB);
	outp(0x71, (inp(0x71) | 0x20)); //razreshit prer ot budilnika

	testAlarm = getvect(0x4A);
	if(testAlarm != newAlarm) {
	    setvect(0x4A, newAlarm);
	    oldAlarm = testAlarm;
	    puts("Alarm installing");
	} else
	    puts("Alarm reinstalling");

	enable();

}

void inputTime() {
	int tmp;

	do {
		fflush(stdin);
		printf("Input hours: ");
		scanf("%i", &tmp);
	} while ((tmp > 23 || tmp < 0));
	date[2] = decToBcd(tmp);

	do {
		fflush(stdin);
		printf("Input minutes: ");
		scanf("%i", &n);
	} while (tmp > 59 || tmp < 0);
	date[1] = decToBcd(tmp);

	do {
		fflush(stdin);
		printf("Input seconds: ");
		scanf("%i", &tmp);
	} while (tmp > 59 || tmp < 0);
	date[0] = decToBcd(tmp);
}

int bcdToDec(int bcd) {
	return ((bcd / 16 * 10) + (bcd % 16));
}

int decToBcd(int dec) {
	return ((dec / 10 * 16) + (dec % 10));
}