#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_TIME 85
#define DHT11PIN 7

int dht11_val[5]={0,0,0,0,0};

int main(void) {
	if (wiringPiSetupGpio() == -1) {
		exit(1);
	}
	
	piHiPri(0);
	
	printf("sensor start\n");
	
	while (1) {
		uint8_t j = 0;
		uint8_t i;
		
		for (i = 0; i < 5; i++) {
			dht11_val[i]=0;
		}
		
		pinMode(DHT11PIN,OUTPUT);
		digitalWrite(DHT11PIN,LOW);
		delay(18);
		digitalWrite(DHT11PIN,HIGH);
		delayMicroseconds(40);
		pinMode(DHT11PIN,INPUT);
		
		uint8_t lststate = HIGH;
		uint8_t counter = 0;
		for (i = 0; i < MAX_TIME; i++) {
			counter = 0;
			while (digitalRead(DHT11PIN) == lststate) {
				counter++;
				delayMicroseconds(1);
				if (counter == 255) {
					break;
				}
			}
			
			lststate = digitalRead(DHT11PIN);
			
			if (counter == 255) {
				break;
			}
			
			// top 3 transistions are ignored
			if ((i >= 4) && (i % 2 == 0)) {
				dht11_val[j / 8] <<= 1;
				if (counter > 16) {
					dht11_val[j / 8] |= 1;
				}
				j++;
			}
		}
			
		// verify cheksum and print the verified data
		float farenheit;
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[255];
		if ((j >= 40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF))) {
			farenheit = dht11_val[2] * 9. / 5. + 32;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
			printf("%s %d.%d%% %d.%dC\n", buffer, dht11_val[0], dht11_val[1], dht11_val[2], dht11_val[3]);
			fflush(stdout);
			sprintf(buffer, "curl -s 'http://api.thingspeak.com/update?key=ISPL3XL9HDKQ3BKM&1=%d&2=%d' &> /dev/null", dht11_val[0], dht11_val[2]);
			FILE *p = popen(buffer, "r");
			pclose(p);
		}
		delay(60 * 1000);
	}

	return 0;
}

