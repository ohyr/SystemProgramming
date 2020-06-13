
#include <wiringPi.h>

#define GPIO18		1

int main(void)
{
	int i;
	
	wiringPiSetup();
	
	pinMode(GPIO18, PWM_OUTPUT);
	
	while(1){
		for(i=0;i<1024;i++){
			pwmWrite(GPIO18, i);
			delay(1);
		}
		delay(100);
		for(i=1023;i>=0;i--){
			pwmWrite(GPIO18, i);
			delay(1);
		}
	}
	
	return 0;
}

