/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

//#include <avr/wdt.h>


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Pivko"
#define WLAN_PASS       "qwerty01234"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "derdek"
#define AIO_KEY         "d980f0e0257640298fbd982929a0ccb4"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
//Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Red = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Red");
Adafruit_MQTT_Subscribe Blue = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Blue");
Adafruit_MQTT_Subscribe Green = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Green");
Adafruit_MQTT_Subscribe Relay1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1");
Adafruit_MQTT_Subscribe Relay2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");
Adafruit_MQTT_Subscribe Relay3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay3");
Adafruit_MQTT_Subscribe Relay4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
  //wdt_enable(WDTO_8S);
  Serial.begin(115200);
  delay(10);
  //pinMode(2, OUTPUT);  // Relay 1
  pinMode(16, OUTPUT); // Relay 2
  pinMode(12, OUTPUT); // Relay 3
  pinMode(13, OUTPUT); // Relay 4
  pinMode(4, OUTPUT);  // RED
  pinMode(5, OUTPUT);  // BLUE
  pinMode(14, OUTPUT); // GREEN
  //digitalWrite(4, 1);
  //digitalWrite(5, 1);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Red);
  mqtt.subscribe(&Blue);
  mqtt.subscribe(&Green);
  //mqtt.subscribe(&Relay1);
  mqtt.subscribe(&Relay2);
  mqtt.subscribe(&Relay3);
  mqtt.subscribe(&Relay4);
}

uint32_t x=0;

bool val=0;
int redCol=0;
int blueCol=0;
int greenCol=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500))) {
    if (subscription == &Red) {
      Serial.print(F("Red: "));
      redCol = atoi((char *)Red.lastread);
      Serial.println(f);
      analogWrite(4,f);
    }
    else if (subscription == &Blue) {
      Serial.print(F("Blue: "));
      blueCol = atoi((char *)Blue.lastread);
      Serial.println(f);
      analogWrite(5,f);
    }
    else if (subscription == &Green) {
      Serial.print(F("Green: "));
      greenCol = atoi((char *)Green.lastread);
      Serial.println(f);
      analogWrite(14,f);
    }
    else if (subscription == &Relay1) {
      Serial.print(F("Relay1: "));
      bool f = atoi((char *)Relay1.lastread);
      Serial.println(f);
      val = f;
      //analogWrite(2,!f);
    }
    else if (subscription == &Relay2) {
      Serial.print(F("Relay2: "));
      bool f = atoi((char *)Relay2.lastread);
      Serial.println(f);
      digitalWrite(16,!f);
    }
    else if (subscription == &Relay3) {
      Serial.print(F("Relay3: "));
      bool f = atoi((char *)Relay3.lastread);
      Serial.println(f);
      digitalWrite(12,!f);
    }
    else if (subscription == &Relay4) {
      Serial.print(F("Relay4: "));
      bool f = atoi((char *)Relay4.lastread);
      Serial.println(f);
      digitalWrite(13,!f);
    }
  }

  if (val)
  {
    // RED 4
    // BLUE 5
    // GREEN 14
    for (int i=0; i<255; i++)
    {
      analogWrite(4,255-i);
      analogWrite(14,i);
      delay(50);
    }
    for (int i=0; i<255; i++)
    {
      analogWrite(14,255-i);
      analogWrite(5,i);
      delay(50);
    }
    for (int i=0; i<255; i++)
    {
      analogWrite(5,255-i);
      analogWrite(4,i);
      delay(50);
    }
  }
  
  // Now we can publish stuff!
  /*
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  */
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }*/
  
  delay(10);
  //wdt_reset();
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         retries=3;
         delay(10000);
         //while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
