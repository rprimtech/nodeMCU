1. Dissenya l'electrònica, el firmware i l'app de mòbil per controlar una sala de consultes mèdiques amb els següents dispositius: 

* Encesa i apagada via app i automàtica de un punt de llum, **H1** a GPIO0.
* Encesa i apagada via app i automàtica de un punt de llum, **H2** a GPIO2.
* Encesa i apagada mitjançant pulsador físic **S1** a GPIO12 i automàtica de un punt de llum, **H3** a GPI13. L'app visualitzarà l'estat de H3.
* Sensor de finestra oberta, **F** a GPIO14. A l'app sortirà un text indicant si està oberta o tancada.
* Sensor de nivell de llum, **LL** a ADC0.
* Sensor de temperatura DHT11 o DHT22, **T** a GPIO10 o bé sensor de temperatura DS18B20, **T** a GPIO10.
* A l'app es graficarà els valors mesurats pels diferents sensors.
* L'obertura d'una de les finestres enviarà una notificació a l'app i al correu electrònic.


2. Ara amb l'ajuda de [node-red](http://localhost:1880) configura les seguents caracteristiques:

* El punt de llum H1 s'encendrà automàticament a les 8:00 hores de cada matí de dilluns a divendres.
* El punts de llum H1 i H2 s'apagarà automàticament a les 21:00 hores.
* API per encendre el llum H1 (GPIO0) http://167.86.87.117:8080/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/update/D0?value=0
* API per apagar el llum H1 (GPIO0) http://167.86.87.117:8080/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX/update/D0?value=1

Primer flow: **node-red-1**

![](images/node-red-1.png)




