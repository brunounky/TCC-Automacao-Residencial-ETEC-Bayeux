
#include "WiFiEsp.h"
#include "SoftwareSerial.h"

//-------------------------------------------------------

   #include <dht.h>


#define dht_dpin A1

//-------------------------------------------------------


SoftwareSerial Serial1(6, 7);

char ssid[] = "Bruno Unky Wi-Fi";
char pass[] = "McAllister10376";

int status = WL_IDLE_STATUS;

WiFiEspServer server(80);

RingBuffer buf(8);

int statusLed = LOW;

//-------------------------------------------------------

dht DHT;

//-------------------------------------------------------

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);


  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  WiFi.config(IPAddress(10,0,0,85));


  if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
  }
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
       
}

void loop() {
  

  WiFiEspClient client = server.available();

  if (client) {
    buf.init();
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        buf.push(c);


        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }




        if (buf.endsWith("GET /AN")) {
          digitalWrite(13, HIGH);
          statusLed = 1;
        }
        else { //SENÃO, FAZ
          if (buf.endsWith("GET /AF")) {
            digitalWrite(13, LOW);
            statusLed = 0;
          }
        }



        if (buf.endsWith("GET /BF")) {
          digitalWrite(11, HIGH);
          statusLed = 1;
        }
        else { //SENÃO, FAZ
          if (buf.endsWith("GET /BN")) {
            digitalWrite(11, LOW);
            statusLed = 0;
          }
        }



        if (buf.endsWith("GET /CF")) {
          digitalWrite(10, HIGH);
          statusLed = 1;
        }
        else { //SENÃO, FAZ
          if (buf.endsWith("GET /CN")) {
            digitalWrite(10, LOW);
            statusLed = 0;
          }
        }


        if (buf.endsWith("GET /DF")) {
          digitalWrite(8, HIGH);
          statusLed = 1;
        }
        else { //SENÃO, FAZ
          if (buf.endsWith("GET /DN")) {
            digitalWrite(8, LOW);
            statusLed = 0;
          }
        }

        if (buf.endsWith("GET /EF")) {
          digitalWrite(9, HIGH);
          statusLed = 1;
        }
        else { //SENÃO, FAZ
          if (buf.endsWith("GET /EN")) {
            digitalWrite(9, LOW);
            statusLed = 0;
          }
        }


        if (buf.endsWith("GET /STU")) {


          delay(700);
          DHT.read11(dht_dpin);

          sensorTU(client);

          break;

        }


      }
      
    }
    client.stop();
  }

}

void sendHttpResponse(WiFiEspClient client) {

}

void sensorTU(WiFiEspClient client) { 


  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<html><head></head><body><font size=12><p style='font-family:fantasy'><BR>");
  client.print((int)DHT.temperature);
  client.println(" <code>&deg;</code>C<p>&nbsp;</p><font size=12><p style='font-family:fantasy'>");
  client.print((int)DHT.humidity);
  client.println(" %</p></font></body></html>");

}
