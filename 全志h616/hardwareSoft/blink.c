#include <stdio.h>
#include <wiringPi.h>

#define BEEP 0   //26pin
//#define NUM 18   //26pin
//#define NUM 20   //for Orange Pi Zero 2
//#define NUM 19   //for Orange Pi 4
//#define NUM 28     //40pin

int main (void)
{

	wiringPiSetup () ;
	pinMode (BEEP, OUTPUT) ;
  	digitalWrite (BEEP, HIGH) ;	// On

	return 0;
}
