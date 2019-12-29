//fitxer: emed-nodeMCU-1.ino
//cpu: esp8266 nodeMCU
//data: 28.12.2019
//@rPrimTech Jaume Nogués

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "nom_ssid";
char pass[] = "password";

void setup()
{
  // Debug console
  Serial.begin(115200);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "vps249990.ovh.net", 8080);
  //Blynk.begin(auth, ssid, pass, IPAddress(167,86,87,117), 8080);
}

void loop()
{
  Blynk.run();
}
