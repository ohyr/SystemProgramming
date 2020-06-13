#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/resource.h>
#include <wiringPi.h>
 
#define trigPin 1    //gpio 21
#define echoPin 29    //gpio 18
 
//old #define trigPin 21    //gpio 5
//old #define echoPin 4    //gpio  J16-pin3 GPIO 23
 
int main(void)
{
    int distance=0;
    int pulse = 0;
    
    long startTime;
    long travelTime;
    
    if(wiringPiSetup () == -1)
    {
        printf("Unable GPIO Setup"); 
        return 1;
    }
        
    pinMode (trigPin, OUTPUT);
    pinMode (echoPin, INPUT);
    
    for(;;)
    {
        digitalWrite (trigPin, LOW);
        usleep(2);
        digitalWrite (trigPin, HIGH);
        usleep(20);
        digitalWrite (trigPin, LOW);
        
        printf("1\n");
        
        while(digitalRead(echoPin) == LOW){
			startTime = micros();
		}
		printf("1\n");
        
        
        while(digitalRead(echoPin) == HIGH){
			travelTime = micros() - startTime;
        }
        printf("1\n");
        
        
        int distance = travelTime / 58;
        
        printf( "Distance: %dcm\n", distance);
        delay(200);
    }
}
