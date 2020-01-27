//emed-nodeMCU-9
//10.01.2020
//nodeMCU
//@rPrimTech       Jaume Nogués

#include <JustWifi.h>

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <DHT11.h>
DHT11 sensor(10);

#include <Wire.h>
#include "ClosedCube_HDC1080.h"
ClosedCube_HDC1080 hdc1080;

#include "Seeed_BME280.h" //https://github.com/Seeed-Studio/Grove_BME280
#include <Wire.h>
BME280 bme280;

#define FASTLED_ESP8266_RAW_PIN_ORDER //per utilitzar numeracio GPIO a nodemcu
#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 15
CRGB leds[NUM_LEDS];
int dimmer=0;
int red=0;
int green=0;
int blue=0;
boolean ramdomColorsFlag=0;

int blynkIsDownCount=0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a77b74d2cd2246738e6546580132f726";

// Your WiFi credentials.
// Set password to "" for open networks.
#define ssid1 "JAZZTEL_Wuwu"
#define pass1 "txnh3uweaw3f"

#define ssid2 "emed"
#define pass2 "pitufito*"

#define ssid3 "kkk"
#define pass3 "dembele1"

BlynkTimer timer;

boolean stateGPIO16=0;
boolean stateGPIO0=0;
boolean prevstateGPIO0=0;
boolean stateGPIO2=0;
boolean prevstateGPIO2=0;
boolean stateGPIO13=0;
boolean prevstateGPIO13=0;
boolean stateGPIO12=0;
boolean prevstateGPIO12=0;
boolean stateGPIO14=0;
boolean prevstateGPIO14=0;

void setup()
{
  // Debug console
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  Serial.println(" ");

  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(1000);
  leds[0] = CRGB::Red;
  FastLED.setBrightness(20);
  FastLED.show();
  
  hdc1080.begin(0x40);
  Serial.print("Manufacturer ID=0x");
  Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
  Serial.print("Device ID=0x");
  Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
  printSerialNumber();

  if(!bme280.init()){
    Serial.println("BME280 Device error!");
  }

  jw.setHostname("nodeMCU");
  jw.subscribe(infoCallback);
  jw.enableAP(false);
  jw.enableAPFallback(true);
  jw.enableSTA(true);
  jw.enableScan(true);
  jw.cleanNetworks();
  jw.addNetwork(ssid1, pass1);
  jw.addNetwork(ssid2, pass2);
  jw.addNetwork(ssid3, pass3);
  Serial.println("[WIFI] Connecting Wifi...");
  
  Blynk.config(auth, IPAddress(167,86,87,117), 8080);
 
  timer.setInterval(200L, multiColors);
  timer.setInterval(300L, checkInputsOutputs);
  timer.setInterval(10002L, uptime);
  timer.setInterval(20000L, mesuraDHT);
  timer.setInterval(22000L, mesuraHDC1080);
  timer.setInterval(23000L, mesuraBMP);
  timer.setInterval(30000L, reconnecting);
}

void loop()
{
  jw.loop();
  if (jw.connected() && Blynk.connected())
  {
    Blynk.run();
  }
  timer.run();
}

void reconnecting()
{
   
  if (!Blynk.connected() && jw.connected())
  {
    blynkIsDownCount++;
    BLYNK_LOG("blynk server is down! %d  times", blynkIsDownCount);
    Blynk.connect(10000);
  }
}


void uptime()
{
  Blynk.virtualWrite(V0, millis() / 1000);
}

void mesuraDHT()
{
  uint32_t res = sensor.leer();
  float t=sensor.temperatura_C();
  float h=sensor.humedad();
  
  Serial.print("[TDHT] ");
  Serial.println(t); // temperatura en graus Celsius
  Serial.print("[HDHT] ");
  Serial.println(h); // humitat en %
  Serial.println(" ");

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
}

void checkInputsOutputs()
{
  digitalWrite(16,stateGPIO16);
  stateGPIO16=!stateGPIO16;

  stateGPIO0=digitalRead(0);
  if (stateGPIO0!=prevstateGPIO0)
  {
    if (stateGPIO0==0) Blynk.virtualWrite(V3,255);
    if (stateGPIO0==1) Blynk.virtualWrite(V3,0);
  }
  prevstateGPIO0=stateGPIO0;

  stateGPIO2=digitalRead(2);
  if (stateGPIO2!=prevstateGPIO2)
  {
    if (stateGPIO2==0) Blynk.virtualWrite(V4,255);
    if (stateGPIO2==1) Blynk.virtualWrite(V4,0);
  }
  prevstateGPIO2=stateGPIO2;

  //********************Polsador GPIO12 (S1) a sortida GPIO13 (H3)***************************
  stateGPIO12=digitalRead(12);
  if (prevstateGPIO12==0 && stateGPIO12==1)
  {
    bool temp=digitalRead(13);
    if (temp){ digitalWrite(13,LOW); Blynk.virtualWrite(V5,0);}
    if (!temp){ digitalWrite(13,HIGH); Blynk.virtualWrite(V5,255);}
    prevstateGPIO12=1;
  }
  if (prevstateGPIO12==1 && stateGPIO12==0)
  {
    prevstateGPIO12=0;
  }
 
  stateGPIO13=digitalRead(13);
  if (stateGPIO13!=prevstateGPIO13)
  {
    if (stateGPIO13==0) Blynk.virtualWrite(V5,0);
    if (stateGPIO13==1) Blynk.virtualWrite(V5,255);
  }
  prevstateGPIO13=stateGPIO13;
  //*************************************************************************************
  
 //***************************Sensor finestra oberta/tancada***************************** 
  stateGPIO14=digitalRead(14);
  if (stateGPIO14!=prevstateGPIO14)
  {
    if (stateGPIO14==0)  Blynk.virtualWrite(V6,"TANCADA");
    if (stateGPIO14==1) 
      {
        Blynk.notify("Finestra OBERTA!!");//Max. 1 notificació cada 5 segons 
        Blynk.virtualWrite(V6,"OBERTA"); 
        //Blynk.email("jnogues@gmail.com", "ALERTA!!", "Finestra OBERTA!!");//Max. 1 mail cada 5 segons
       }
   }
  prevstateGPIO14=stateGPIO14;
 //*************************************************************************************
  
} 

BLYNK_CONNECTED() {
    Blynk.syncAll();
    Serial.print("[WIFI] Connected to SSID: ");
    Serial.println(WiFi.SSID());
}

void mesuraHDC1080()
{
  
  float T_HDC=hdc1080.readTemperature();
  float H_HDC=hdc1080.readHumidity();
  Serial.print("[THDC] ");
  Serial.println(T_HDC);
  Serial.print("[HHDC] ");
  Serial.println(H_HDC);
  Serial.println(" ");
  Blynk.virtualWrite(V7,T_HDC);
  Blynk.virtualWrite(V8,H_HDC); 
}

void printSerialNumber() {
  Serial.print("Device Serial Number=");
  HDC1080_SerialNumber sernum = hdc1080.readSerialNumber();
  char format[12];
  sprintf(format, "%02X-%04X-%04X", sernum.serialFirst, sernum.serialMid, sernum.serialLast);
  Serial.println(format);
}

void mesuraBMP()
{
  float T_BMP=bme280.getTemperature();
  float H_BMP=bme280.getHumidity();
  float P_BMP=bme280.getPressure()/100;//Pa-->hPa
  //get and print temperature
  Serial.print("[TBMP] ");
  Serial.println(T_BMP);
  //get and print humidity data
  Serial.print("[HBMP] ");
  Serial.println(H_BMP);
  //get and print atmospheric pressure data
  Serial.print("[PBMP] ");
  Serial.println(P_BMP);
  Serial.println(" ");
  Blynk.virtualWrite(V9,T_BMP);
  Blynk.virtualWrite(V10,H_BMP);
  Blynk.virtualWrite(V11,P_BMP);  
}

//dimmer
BLYNK_WRITE(16)
{
  dimmer = param[0].asInt();
  for(int i=0;i<NUM_LEDS;i++)
  { 
    FastLED.setBrightness(  dimmer );
    FastLED.show();
  }
}

//zergba
BLYNK_WRITE(17)
{
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i].setRGB( red,green,blue);
    FastLED.show();
    ramdomColorsFlag=0;
  }
}

//llistat de colors
//http://fastled.io/docs/3.1/struct_c_r_g_b.html
BLYNK_WRITE (14)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB:: Black;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (18)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB:: DeepSkyBlue;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (19)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::DarkOliveGreen;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (20)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Red;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (21)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Green;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (22)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Blue;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (23)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::White;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}
BLYNK_WRITE (24)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::MistyRose;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}     
BLYNK_WRITE (25)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Salmon;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}     
BLYNK_WRITE (26)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Orange;
    FastLED.show();
    ramdomColorsFlag=0;
  }
}  
BLYNK_WRITE (27)
{
    leds[0] = CRGB::Orange;
    leds[1] = CRGB::Salmon;
    leds[2] = CRGB::MistyRose;
    leds[3] = CRGB::Blue;
    leds[4] = CRGB::DarkOliveGreen;
    leds[5] = CRGB::Orange;
    leds[6] = CRGB::Salmon;
    leds[7] = CRGB::MistyRose;
    FastLED.show();
    ramdomColorsFlag=0;
} 

BLYNK_WRITE (28)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB:: Black;
    FastLED.show();
    ramdomColorsFlag=0;
  }
} 

BLYNK_WRITE (29)
{
  int kk=param.asInt();
  if (kk==1) ramdomColorsFlag=!ramdomColorsFlag;
  if (ramdomColorsFlag==0) 
  { 
    for(int i=0;i<NUM_LEDS;i++)
    {
    leds[i] = CRGB:: Black;
    FastLED.show();
    }   
  }
} 


void multiColors()
{
  if (ramdomColorsFlag==1)
  {
    FastLED.setBrightness(100 );
    red = random(50,100);
    green = random(10,50);
    blue = random(50,100);
    //FastLED.setBrightness(random(0,255));
    for(int i=0;i<NUM_LEDS;i++)
    {
      leds[i].setRGB( red,green,blue);
      FastLED.show();
    }
  }
}

//**************************funcions de justwifi***********************
void infoWifi() {

    if (WiFi.isConnected()) {

        uint8_t * bssid = WiFi.BSSID();

        Serial.printf("[WIFI] MODE STA -------------------------------------\n");
        Serial.printf("[WIFI] SSID  %s\n", WiFi.SSID().c_str());
        Serial.printf("[WIFI] BSSID %02X:%02X:%02X:%02X:%02X:%02X\n",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]
        );
        Serial.printf("[WIFI] CH    %d\n", WiFi.channel());
        Serial.printf("[WIFI] RSSI  %d\n", WiFi.RSSI());
        Serial.printf("[WIFI] IP    %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[WIFI] MAC   %s\n", WiFi.macAddress().c_str());
        Serial.printf("[WIFI] GW    %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("[WIFI] MASK  %s\n", WiFi.subnetMask().toString().c_str());
        Serial.printf("[WIFI] DNS   %s\n", WiFi.dnsIP().toString().c_str());
        #if defined(ARDUINO_ARCH_ESP32)
            Serial.printf("[WIFI] HOST  %s\n", WiFi.getHostname());
        #else
            Serial.printf("[WIFI] HOST  %s\n", WiFi.hostname().c_str());
        #endif
        Serial.printf("[WIFI] ----------------------------------------------\n");

    }

    if (WiFi.getMode() & WIFI_AP) {

        Serial.printf("[WIFI] MODE AP --------------------------------------\n");
        Serial.printf("[WIFI] SSID  %s\n", jw.getAPSSID().c_str());
        Serial.printf("[WIFI] IP    %s\n", WiFi.softAPIP().toString().c_str());
        Serial.printf("[WIFI] MAC   %s\n", WiFi.softAPmacAddress().c_str());
        Serial.printf("[WIFI] ----------------------------------------------\n");

    }

}

void infoCallback(justwifi_messages_t code, char * parameter) {

    // -------------------------------------------------------------------------

    if (code == MESSAGE_TURNING_OFF) {
        Serial.printf("[WIFI] Turning OFF\n");
    }

    if (code == MESSAGE_TURNING_ON) {
        Serial.printf("[WIFI] Turning ON\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_SCANNING) {
        Serial.printf("[WIFI] Scanning\n");
    }

    if (code == MESSAGE_SCAN_FAILED) {
        Serial.printf("[WIFI] Scan failed\n");
    }

    if (code == MESSAGE_NO_NETWORKS) {
        Serial.printf("[WIFI] No networks found\n");
    }

    if (code == MESSAGE_NO_KNOWN_NETWORKS) {
        Serial.printf("[WIFI] No known networks found\n");
    }

    if (code == MESSAGE_FOUND_NETWORK) {
        Serial.printf("[WIFI] %s\n", parameter);
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_CONNECTING) {
        Serial.printf("[WIFI] Connecting to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECT_WAITING) {
        // too much noise
    }

    if (code == MESSAGE_CONNECT_FAILED) {
        Serial.printf("[WIFI] Could not connect to %s\n", parameter);
    }

    if (code == MESSAGE_CONNECTED) {
        infoWifi();
    }

    if (code == MESSAGE_DISCONNECTED) {
        Serial.printf("[WIFI] Disconnected\n");
    }

    // -------------------------------------------------------------------------

    if (code == MESSAGE_ACCESSPOINT_CREATED) {
        infoWifi();
    }

    if (code == MESSAGE_ACCESSPOINT_DESTROYED) {
        Serial.printf("[WIFI] Disconnecting access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_CREATING) {
        Serial.printf("[WIFI] Creating access point\n");
    }

    if (code == MESSAGE_ACCESSPOINT_FAILED) {
        Serial.printf("[WIFI] Could not create access point\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_WPS_START) {
        Serial.printf("[WIFI] WPS started\n");
    }

    if (code == MESSAGE_WPS_SUCCESS) {
        Serial.printf("[WIFI] WPS succeded!\n");
    }

    if (code == MESSAGE_WPS_ERROR) {
        Serial.printf("[WIFI] WPS failed\n");
    }

    // ------------------------------------------------------------------------

    if (code == MESSAGE_SMARTCONFIG_START) {
        Serial.printf("[WIFI] Smart Config started\n");
    }

    if (code == MESSAGE_SMARTCONFIG_SUCCESS) {
        Serial.printf("[WIFI] Smart Config succeded!\n");
    }

    if (code == MESSAGE_SMARTCONFIG_ERROR) {
        Serial.printf("[WIFI] Smart Config failed\n");
    }

};
