
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

//LED stuff
FASTLED_USING_NAMESPACE
#define DATA_PIN    2
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    47				 // 47 for gate 1, 209 for gate 2
CRGB leds[NUM_LEDS];
bool     flashing;
const uint8_t FLASH_DURATION = 1000; // milliseconds
uint32_t lastMeasureTime;
uint32_t lastStartTime;
bool a = true;
uint8_t patten = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  60

bool RaceStart = false;


CRGB Green 	= CRGB(0, 255, 0);
CRGB Red 	= CRGB(255, 0, 0);
CRGB Blue 	= CRGB(0, 0, 255);
CRGB Purple = CRGB(153, 0, 76);
CRGB Yellow = CRGB(255, 120, 0);
CRGB none 	= CRGB(0, 0, 0);

CRGB co = none;

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Race Net";
const char* password = "11111111112222222222bbbbbb";
IPAddress   apIP(10, 10, 10, 11);		//Gate 1 = 10.10.10.11, Gate 2 = 10.10.10.12
IPAddress   gateway(10, 10, 10, 1);
IPAddress   subnet(255, 255, 255, 0);


const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "d";
const char* PARAM_INPUT_3 = "input3";

#include "configHTML.h"


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() 
{
	Serial.begin(9600);

  	FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  	FastLED.setBrightness(BRIGHTNESS);
	lastMeasureTime = 0;
  	flashing        = false;
 
  	//WiFi.mode(WIFI_AP_STA);
  	//WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  	//WiFi.softAP(ssid, password);
  	WiFi.config(apIP, gateway, subnet);
  	WiFi.begin(ssid, password);


	for( int i = 0; i < NUM_LEDS; ++i) 
	{
		leds[i] = CRGB(255, 0, 0);
	}
	FastLED.show();
	lastMeasureTime = millis();
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
		if (millis() - lastMeasureTime > 5000) 
		{
			patten = 1;
			break;
		}
	}
	for( int i = 0; i < NUM_LEDS; ++i) 
	{
		leds[i] = CRGB(0, 255, 0);
		}
	FastLED.show();

  
  	// Send web page with input fields to client
  	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  	{
    	request->send_P(200, "text/html", index_html);
	});

	server.on("/css.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send_P(200, "text/css", index_css);
    });

  	// Send a GET request to <ESP_IP>/get?state=<inputMessage>
  	server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) 
  	{
    	String inputMessage;
    	String inputMessage2;
    	String inputParam;
    	// GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    	if (request->hasParam(PARAM_INPUT_1)) 
    	{
      		inputMessage = request->getParam(PARAM_INPUT_1)->value();
      		inputParam = PARAM_INPUT_1;
      		if(inputMessage == "1")
      		{
        		flashing = false;
       		 	patten = 1;
        		Serial.println("Ranbow");
      		}
      		if(inputMessage == "2")
      		{
        		flashing = false;
        		for( int i = 0; i < NUM_LEDS; ++i) 
        		{
         	 		leds[i] = CRGB(255, 0, 0);
        		}
        		FastLED.show();
    		}
      		if(inputMessage == "3")
      		{
        		patten = 0;
        		flashing = true;
      		}
    	}
    	// GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    	else if (request->hasParam(PARAM_INPUT_3)) 
    	{
      		inputMessage = request->getParam(PARAM_INPUT_3)->value();
      		inputParam = PARAM_INPUT_3;
      		String c = getValue(inputMessage, ':', 0);
      		if(c == "f")
      		{
        		Serial.println(inputMessage);
      		}
      		else if(c == "c")
      		{
        		Serial.println(inputMessage);
        		String col = getValue(inputMessage, ':', 1);
        		if(col == "red")
          			co = Red;
        		else if(col == "green")
          			co = Green;
        		else if(col == "blue")
          			co = Blue;
         		else if(col == "yellow")
          			co = Yellow;
        		else if(col == "purple")
          			co = Purple;
        		else if(col == "off")
          			co = none;
        		else
          			co = none;
        		patten = 0;
        		ResetLEDS();
      		}
      		else if(c == "b")
      		{
        		int Bright = getValue(inputMessage, ':', 1).toInt();
        		FastLED.setBrightness(Bright);
        		FastLED.show();
      		}
      		else if(c == "p")
      		{
        		Serial.println(inputMessage);
        		String col = getValue(inputMessage, ':', 1);
        		if(col == "Color")
        		{
          			patten = 0;
          			flashing = false;
        		}
        		else if(col == "Flash")
        		{
          			patten = 0;
          			flashing = true;
        		}
        		else if(col == "Ranbow")
        		{
          			patten = 1;
          			flashing = false;
        		}
        		else if(col == "Juggle")
        		{
          			patten = 2;
          			flashing = false;
        		}
      		}
      		else
      		{
        		Serial.println(inputMessage);
        		leds[0]  = CRGB(0, 0, 255);
        		FastLED.show();
      		}		
    	}
    	else 
    	{
      		inputMessage = "No message sent";
      		inputParam = "none";
    	}
    	request->send(200, "text/html", inputMessage);
  	});
  	server.onNotFound(notFound);
  	server.begin();
}

void loop() 
{
	if(RaceStart == true)
  	{
    	if (millis() - lastStartTime > 3000)
    	{
      		RaceStart = false;
      		ResetLEDS();
    	}
  	}
  	else
  	{
  	}
  	if (flashing) 
  	{
        if (millis() - lastMeasureTime > FLASH_DURATION) 
        {
          	if (a)
          	{
            	for( int i = 0; i < NUM_LEDS; ++i) 
            	{
              		leds[i] = co;
             	}
             	FastLED.show();
             	a = false;
             	lastMeasureTime = millis();
          	}
          	else
          	{
            	for( int i = 0; i < NUM_LEDS; ++i) 
            	{
              		leds[i] = CRGB(0, 0, 0);
             	}
             	FastLED.show();
             	a = true;
             	lastMeasureTime = millis();
          	}
        }
    }
  	if(patten == 1)
  	{
    	EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    	rainbow();
    	FastLED.show();
  	}
  	else if(patten == 2)
  	{
    	juggle();
    	FastLED.show();
  	}
}

void ResetLEDS()
{
	for( int i = 0; i < NUM_LEDS; ++i) 
	{
		leds[i] = co;
	}
	FastLED.show();
}

String getValue(String data, char separator, int index)
{
	int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) 
    {
        if (data.charAt(i) == separator || i == maxIndex) 
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void rainbow() 
{
	// FastLED's built-in rainbow generator
	fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void juggle()
{
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 20);
	byte dothue = 0;
	for( int i = 0; i < 8; i++) 
	{
    	leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    	dothue += 32;
  	}
}
