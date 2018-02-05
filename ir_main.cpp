/*
*  This sketch sends data via RMT the ESP32 requests to data the RMT.
*
* .....
*
*/
#include <Arduino.h>
#include <IR_ESP32.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

int TV_LigarArray[] =    {1,1,1,0,0,0,0,0 ,1,1,1,0,0,0,0,0 ,0,1,0,0,0,0,0,0 ,1,0,1,1,1,1,1,1 ,1,1};
int TV_AumentarArray[] = {1,1,1,0,0,0,0,0 ,1,1,1,0,0,0,0,0 ,0,1,0,0,1,0,0,0 ,1,0,1,1,0,1,1,1 ,1,1};
int TV_DiminuirArray[] = {1,1,1,0,0,0,0,0 ,1,1,1,0,0,0,0,0 ,0,0,0,0,1,0,0,0 ,1,1,1,1,0,1,1,1 ,1,1};

//Infrared
int headMark = 4550;
int headSpace = 4500;
int zeroMark = 545;
int zeroSpace = 570;
int oneMark = 545;
int oneSpace = 1690;
int endMark = 0;
int endSpace = 0;

//Item para envio
size_t size = (sizeof(rmt_item32_t) * 34 * 100);  
rmt_item32_t* dataLigar = (rmt_item32_t*) malloc(size);
rmt_item32_t* dataAumentar = (rmt_item32_t*) malloc(size);
rmt_item32_t* dataDiminuir = (rmt_item32_t*) malloc(size);

//Botões
#define btnLigar 21
#define btnMais  22
#define btnMenos 23

//Leds
#define led2 25
#define led1 26
#define led3 27

//IR emissor
#define emissorIR 15

//IR receptor
#define receptorIR 13

//DTH
#define pinDHT 5
#define typeDHT DHT11

//LDR
#define pinLDR 34

const char* ssid = "";
const char* pass = "";
const char* brokerUser = "";
const char* brokerPass = "";
const char* broker = "io.adafruit.com";
const int brokerPort = 1883;

const char* topicTemp = "/feeds/temp";
const char* topicTempAr = "/feeds/tempAr";
const char* topicHum = "/feeds/hum";
const char* topicLum = "/feeds/lum";
const char* topicSom = "/feeds/som";

const char* topicLigar = "/feeds/ligar";
const char* topicAumentar = "/feeds/aumentar";
const char* topicDiminuir = "/feeds/diminuir";

DHT dht(pinDHT, typeDHT);

WiFiClient espClient;
PubSubClient client(espClient);

long currentTime, lastTime;
int tempAR = 0;
char msgMQTT[50];

void setupWiFi(){
  delay(100);
  Serial.print("\nConectando... ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }

  Serial.print("\nConectado: ");
  Serial.println(ssid);
}

void reconnect(){

  while(!client.connected()){
    
    Serial.print("\nConectando... ");
    Serial.println(broker);
    if(client.connect("ESP32-1", brokerUser, brokerPass)){
      Serial.print("\nConectado: ");
      Serial.println(broker);  
      client.subscribe(topicLigar);
      client.subscribe(topicAumentar);
      client.subscribe(topicDiminuir);
    }else{
      Serial.print("\nTentando conectar... ");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mesagem recebida: [");
  Serial.print(topic);
  Serial.print("] valor: ");
  char valor[50];
  for(int i=0; i<length; i++){
    valor[i] = (char) payload[i];    
  }

  if(strcmp(topic, topicLigar)==0){
    if(strcmp(valor, "1")==0){
      IR_ESP32.sendRaw(dataLigar, 34);
      digitalWrite(led3, HIGH);
    }else{
      IR_ESP32.sendRaw(dataLigar, 34);
      digitalWrite(led3, LOW);
    }
  }

  if(strcmp(topic, topicAumentar)==0){
    if(strcmp(valor, "1")==0){
      tempAR++;
      IR_ESP32.sendRaw(dataAumentar, 34);
      digitalWrite(led1, HIGH);
      client.publish(topicAumentar, "0");
    }else
      digitalWrite(led1, LOW);
  }

  if(strcmp(topic, topicDiminuir)==0){
    if(strcmp(valor, "1")==0){
      tempAR--;
      IR_ESP32.sendRaw(dataDiminuir, 34);
      digitalWrite(led2, HIGH);
      client.publish(topicDiminuir, "0");
    }else
      digitalWrite(led2, LOW);
  }

  Serial.println(valor);
}

void setup()
{
  //Serial
  Serial.begin(115200);
  setupWiFi();
  client.setServer(broker, brokerPort);
  client.setCallback(callback);

  pinMode(emissorIR, OUTPUT);  
  pinMode(receptorIR, INPUT);  

  pinMode(btnLigar, INPUT);
  pinMode(btnMais, INPUT);
  pinMode(btnMenos, INPUT);

  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT);   

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  
  //IR_ESP32.begin(pinTX, pinRX);
  IR_ESP32.begin(emissorIR, receptorIR);

  rmt_item32_t* item_temp = dataLigar;
  item_temp->level0 = 1;
  item_temp->duration0 = headMark;  
  item_temp->level1 = 0;
  item_temp->duration1 = headSpace;
  item_temp++;
  for(int i = 0; i < 34 ; i++){     
     if(TV_LigarArray[i]==1){
        item_temp->level0 = 1;
        item_temp->duration0 = oneMark;
        item_temp->level1 = 0;
        item_temp->duration1 = oneSpace;
     }else{
        item_temp->level0 = 1;
        item_temp->duration0 = zeroMark;
        item_temp->level1 = 0;
        item_temp->duration1 = zeroSpace;
     }  
     item_temp++;
  }

  item_temp = dataAumentar;
  item_temp->level0 = 1;
  item_temp->duration0 = headMark;  
  item_temp->level1 = 0;
  item_temp->duration1 = headSpace;
  item_temp++;
  for(int i = 0; i < 34 ; i++){     
     if(TV_AumentarArray[i]==1){
        item_temp->level0 = 1;
        item_temp->duration0 = oneMark;
        item_temp->level1 = 0;
        item_temp->duration1 = oneSpace;
     }else{
        item_temp->level0 = 1;
        item_temp->duration0 = zeroMark;
        item_temp->level1 = 0;
        item_temp->duration1 = zeroSpace;
     }  
     item_temp++;
  }

  item_temp = dataDiminuir;
  item_temp->level0 = 1;
  item_temp->duration0 = headMark;    
  item_temp->level1 = 0;
  item_temp->duration1 = headSpace;
  item_temp++;
  for(int i = 0; i < 34 ; i++){     
     if(TV_DiminuirArray[i]==1){
        item_temp->level0 = 1;
        item_temp->duration0 = oneMark;
        item_temp->level1 = 0;
        item_temp->duration1 = oneSpace;
     }else{
        item_temp->level0 = 1;
        item_temp->duration0 = zeroMark;
        item_temp->level1 = 0;
        item_temp->duration1 = zeroSpace;
     }  
     item_temp++;
  }

  
  //DHT
  dht.begin();

  
}

void loop(){  

  int ldr = analogRead(pinLDR);

  Serial.print("Luminosidade: ");
  Serial.print(ldr);
  Serial.println(" LUX ");
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }else{
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("% :: ");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println("*C ");
  }
  
  if( digitalRead(btnLigar) == HIGH ){
    Serial.println("IR::LIGAR");
    digitalWrite(led3, !digitalRead(led3));
    IR_ESP32.sendRaw(dataLigar, 34);
    delay(600);
  }else if( digitalRead(btnMais) == HIGH ){
    Serial.println("IR::AUMENTAR");
    digitalWrite(led1, HIGH);
    IR_ESP32.sendRaw(dataAumentar, 34);
    delay(600);
  }else if( digitalRead(btnMenos) == HIGH ){
    Serial.println("IR::DIMINUIR");
    digitalWrite(led2, HIGH);
    IR_ESP32.sendRaw(dataDiminuir, 34);
    delay(600);
  }else{
    digitalWrite(led2, LOW);
    digitalWrite(led1, LOW);
  }

  if(!client.connected()){
    reconnect();
  }
  client.loop();

  currentTime = millis();
  if(currentTime - lastTime > 10000){
    //LDR
    snprintf(msgMQTT, 75, "%ld", ldr);
    Serial.print("[ENVIANDO] LDR: ");
    Serial.println(msgMQTT);
    client.publish(topicLum, msgMQTT);
    delay(200);
    //Temperatura do AR
    snprintf(msgMQTT, 75, "%ld", tempAR);
    Serial.print("[ENVIANDO] Temp. Ar: ");
    Serial.println(msgMQTT);
    client.publish(topicTempAr, msgMQTT);
    delay(200);
    //Temperatura
    snprintf(msgMQTT, 75, "%lf", t);
    Serial.print("[ENVIANDO] Temp.: ");
    Serial.println(msgMQTT);
    client.publish(topicTemp, msgMQTT);
    delay(200);
    //Humidade
    snprintf(msgMQTT, 75, "%lf", h);
    Serial.print("[ENVIANDO] Humidade: ");
    Serial.println(msgMQTT);
    client.publish(topicHum, msgMQTT);
    delay(200);
    
    lastTime = millis();
  }
  delay(2000);
}
