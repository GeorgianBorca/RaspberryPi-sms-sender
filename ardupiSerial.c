	#include "arduPi.h"
	//g++ -lrt -lpthread ardupiSerial.c arduPi.o -o serialTest
	boolean sendAT(const char *AT, const char *response, int timeOut);
	boolean sendSMS(const char *s, char *n);
	
	int main (int argc, char **argv){
		Serial.begin(9600);
		
		while( Serial.available() > 0) Serial.read();
		
		if (!sendAT("AT", "OK", 2000)) {
			if (!sendAT("AT", "OK", 1000)) {
				printf("AT ERROR\n");
				Serial.end();
				return 0;
			}
		}
		
		if (sendAT("AT+CMGF=1", "OK", 3000)) {
			printf("Sending: \"%s\" To: \"%s\"\n", argv[2], argv[1]);
		} else {
			printf("AT ERROR\n");
			Serial.end();
			return 0;
		}
		
		if (sendSMS(argv[2], argv[1])) {
			printf("Sent!\n");
			Serial.end();
			return 0;
		} else {
			printf("Error sending!\n");
			Serial.end();
			return 0;
		}
		
		Serial.end();
		return (0);
	}
		
	boolean sendSMS(const char *s, char *n) {
		while(Serial.available()) Serial.read();
		Serial.print("AT+CMGS=\"");
		Serial.print(n);
		if (!sendAT("\"", ">", 3000)) {
			return false;
		}
		Serial.print(s);
		if (sendAT("\x1A", "OK", 20000)) {
			return true;
		}
		return false;
	}
	
	boolean sendAT(const char *AT, const char *response, int timeOut) {
		char buffer[120];
		memset(buffer, '\0', 120);
		int i = 0;
		boolean Response = false;
		unsigned long now = millis();
		while( Serial.available() > 0) Serial.read();
		Serial.println(AT);
		while (millis() - now <= timeOut) {
			if (Serial.available() > 0) {
				buffer[i] = Serial.read();
				i++;
			}
			
			if (strstr(buffer, response) != NULL) {
				buffer[i] = '\0';
				Response = true;
				break;
			}
			
			if (strstr(buffer, "ERROR")) {
				buffer[i] = '\0';
				Response = false;
				break;
			}
		}
		//printf("%s", buffer);
		//printf("\n");
		return Response;
		
	}