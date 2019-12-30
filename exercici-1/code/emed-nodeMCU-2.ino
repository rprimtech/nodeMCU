//emed-nodeMCU-2
//28.12.2019
//nodeMCU
//@rPrimTech       Jaume Nogués

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxx";

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

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "vps249990.ovh.net", 8080);
  Blynk.begin(auth, ssid, pass, IPAddress(167,86,87,117), 8080);

  timer.setInterval(1000L, intermita);
  timer.setInterval(10000L, uptime);
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
