#include <ESP8266WiFi.h>
#include <Servo.h>
#include "config.h"
Servo servo;
const int PIRSensorOutPin = 2;
const int PIRSensorOutPin2=13;
int toplamKisiSayisi=0;
boolean PIR_Available_1=false;
boolean PIR_Available_2=false;
const char* ssid = "Ayhan";
const char* password = "ayhan2552";
WiFiServer server(80);
AdafruitIO_Feed *counter = io.feed("grafik");
void setup() {
  Serial.begin(115200);
  pinMode(PIRSensorOutPin, INPUT);
  pinMode(PIRSensorOutPin2, INPUT);
  delay(10);
  servo.attach(5);
  Serial.println();
  Serial.println();
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  Serial.print("Baglanılıyor ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi baglandi");
  // Start the server
  server.begin();
  Serial.println("Server baslatildi");
  Serial.print("Bu adrese gidin : ");
  Serial.print("http://");   
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
}
void loop() {
  if(digitalRead(PIRSensorOutPin) == HIGH&&PIR_Available_1==true)
  {
       toplamKisiSayisi++;
       Serial.print("GIRDI! : ");    //Print to serial monitor
       Serial.println(toplamKisiSayisi);
       PIR_Available_1=false;
       io.run();
       Serial.print("sending -> ");
       Serial.println(toplamKisiSayisi);
       counter->save(toplamKisiSayisi);
  }
  else { 
    PIR_Available_1=true;}
  if(digitalRead(PIRSensorOutPin2) == HIGH&&PIR_Available_2==true&&toplamKisiSayisi>1){
       toplamKisiSayisi--;
       Serial.print("CIKTI! : ");    //Print to serial monitor
       Serial.println(toplamKisiSayisi);
       PIR_Available_2=false;
       io.run();
       Serial.print("sending -> ");
       Serial.println(toplamKisiSayisi);
       counter->save(toplamKisiSayisi);
    }
    else{PIR_Available_2=true;}
    
  // Check if a client has connected
  WiFiClient client = server.available();
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 String value = "";
 // Match the request

  if (request.indexOf("/Req=0") != -1)  {
  value="KAPI ACIK";
  servo.write(0);
  }

  if (request.indexOf("/Req=180") != -1)  {
  value="KAPI KAPALI";
  servo.write(180);
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>KAPI KILIDI KONTROL</h1><br><br>");
  client.print("Durum  = ");
  client.print(value); 
  client.println("<br><br>");
  client.println("<a href=\"/Req=0\"\"><button>KAPIYI AC</button></a>");
  client.println("<a href=\"/Req=180\"\"><button>KAPIYI KILITLE</button></a><br/>");
  client.println("</html>");
}
