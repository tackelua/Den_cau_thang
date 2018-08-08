/*
    Name:       Den_cau_thang.ino
    Created:	08-Aug-18 08:47:20
    Author:     GITH\tacke
*/

#define DEBUG			Serial
#define BLYNK_PRINT		DEBUG

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define VERSION 0.1

char ssid[] = "YourNetworkName";	//tên wifi
char pass[] = "YourPassword";		//mật khẩu wifi


char auth[] = "YourAuthToken";		//blynk token

#define THRESHOLD	10
#define SENSOR		A0				//chân gắn với cảm biến dòng
#define RELAY		LED_BUILTIN		//chân gắn với relay

#define BLYNK_BUTTON	V0
bool light_status = false;

void setup()
{
	DEBUG.begin(115200);
	DEBUG.println("HELLO");
	pinMode(SENSOR, INPUT);
	Blynk.begin(auth, ssid, pass);
	DEBUG.println("SYSTEM STARTED");
}

// Add the main program code into the continuous loop() function
void loop()
{
	read_light_status();
	Blynk.run();
}


BLYNK_WRITE(BLYNK_BUTTON) {
	int val = param.asInt();
	DEBUG.print("Blynk Send ");
	DEBUG.println(val);

	if (val != (int)light_status) {
		digitalWrite(RELAY, val);
	}
}
void read_light_status() {
	static unsigned long t = millis() - 1000;
	if (millis() - t > 200) {
		t = millis();

		int a = analogRead(SENSOR);
		//DEBUG.print("Raw sensor: ");
		//DEBUG.println(a);
		
		int d = abs(a - 511);
		//DEBUG.print("Different: ");
		DEBUG.println(d);


		bool s = d > THRESHOLD ? true : false;
		if (s != light_status) {
			light_status = s;
			DEBUG.println("LIGHT STATUS WAS CHANGED");
			Blynk.virtualWrite(BLYNK_BUTTON, light_status);
		}
	}
}