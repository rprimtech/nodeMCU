//emed-nodeMCU-3
//28.12.2019
//nodeMCU
//@rPrimTech       Jaume Nogués

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT11.h>
DHT11 sensor(10);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "kk";
char pass[] = "dembele1";

BlynkTimer timer;

void setup()
{
  // Debug console
  pinMode(16, OUTPUT);
  Serial.begin(115200);
  Serial.println(" ");

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "elMeuServer.cat", 8080);
  Blynk.begin(auth, ssid, pass, IPAddress(167,86,87,117), 8080);

  timer.setInterval(1000L, intermita);
  timer.setInterval(10000L, uptime);
  timer.setInterval(20000L, mesuraDHT);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void intermita()
{
  digitalWrite(16, !digitalRead(16));
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
  
  Serial.print("Temperatura (C): ");
  Serial.println(t); // temperatura en graus Celsius
  Serial.print("Humitat: ");
  Serial.println(h); // humitat en %

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
}
