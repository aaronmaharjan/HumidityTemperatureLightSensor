
// including some libraries, but also have downloaded other libraries
#include <ESP8266HTTPClient.h>
#include "DHT.h"
#include <ESP8266WiFi.h>


// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// I have edited apikey ssid and pass as I am uploading this to Github publicly..
String apiKey = "NOTREALONE";     //  Enter your Write API key from ThingSpeak
// set for home configuration
const char *ssid =  "TEMPNAME";     // replace with your wifi ssid and wpa2 key
const char *pass =  "TempPass124";
const char* server = "api.thingspeak.com";
int sensorPin = A0; // select the input pin for LDR

int sensorValue = 0; // variable to store the value coming from the sensor


#define DHTPIN 2     // what digital pin we're connected to
//due to a bug even though I am connected to D4 pin on the NODEMCU I will declare pin2 instead
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V   
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
//I followed the instructions above


// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
 Serial.begin(9600);
 Serial.println("DHTxx test!");

 dht.begin();
 WiFi.begin(ssid, pass);

 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
 }
}
void loop() 
{
      float l = analogRead(sensorPin); // read the luminosity value from the sensor
      float h = dht.readHumidity();    // reading humidity
      float t = dht.readTemperature(); //reading temperature
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            //posting data to thingspeak
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(l);
                             postStr += "\r\n\r\n";
                             
  
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             //printing data to COM screen
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.print("Lux :");
                             Serial.print(l);
                             Serial.println("%. Send to Thingspeak."); 
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 25 seconds
  delay(25000);
}
