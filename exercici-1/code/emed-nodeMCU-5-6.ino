//emed-nodeMCU-5-6
//10.01.2020
//nodeMCU
//@rPrimTech       Jaume Nogués

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <DHT11.h>
DHT11 sensor(10);

#include "Seeed_BME280.h" //https://github.com/Seeed-Studio/Grove_BME280
#include <Wire.h>
BME280 bme280;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "emed";
char pass[] = "pitufito";

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

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "vps249990.ovh.net", 8080);
  Blynk.begin(auth, ssid, pass, IPAddress(167,86,87,117), 8080);

  if(!bme280.init()){
    Serial.println("BME280 Device error!");
  }

  timer.setInterval(300L, checkInputsOutputs);
  timer.setInterval(10002L, uptime);
  //timer.setInterval(20000L, mesuraDHT);
  timer.setInterval(23000L, mesuraBMP);
}

void loop()
{
  Blynk.run();
  timer.run();
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
        //Blynk.email("pepito@gmail.com", "ALERTA!!", "Finestra OBERTA!!");//Max. 1 mail cada 5 segons
       }
   }
  prevstateGPIO14=stateGPIO14;
 //*************************************************************************************
  
} 

BLYNK_CONNECTED() {
    Blynk.syncAll();
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
