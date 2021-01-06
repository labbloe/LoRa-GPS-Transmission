//GPS Rocket Transmit
//Operating Frequencies Allowed without License ranging 902-928MHz
//Operating Frequency of Heltec WiFi LoRa 32 (V2) 868-915Mhz
//NOTE: FCC unlicensed LoRa signals have no legal maximum duty cycle for
//      transmission (time between messages), however, a max singal
//      transmission time per message is limited to 400ms
//      see US915

//Video on setup of basic LoRa communication using LoRa.h
//https://www.youtube.com/watch?v=0xP1h1Qx4Ao&ab_channel=G6EJD-David

//Computer Receiving Antenna: Nooelec NESDR SMArTee v2
//https://www.nooelec.com/store/nesdr-smartee.html
//Jared Allen

#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <LoRa.h>

#define GPS_Rx 22
#define GPS_Tx 23

#define LoRa_CS 18
#define LoRa_RST 14
#define LoRa_IRQ 26

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(9600);
  //gpsSerial.begin(unsigned long baud, uint32-t config, rxPin, txPin)
  //rxPin(22) --> GPS_Tx
  //txPin(23) --> GPS_Rx
  gpsSerial.begin(9600,SERIAL_8N1,GPS_Rx,GPS_Tx); //neo-6m likes 9600 baud rate
  //LoRa.setPins(CS,RESET,IRQ)
  LoRa.setPins(LoRa_CS,LoRa_RST,LoRa_IRQ);
  //Start LoRa at 915MHz
  while(!LoRa.begin(915E6))
    Serial.println("LoRa Activation Failed!");
  Serial.println("LoRa Activation Successful!");

}

void loop() {

  //While information is being streamed in from neo-6m
  while(gpsSerial.available() > 0)
  {
    if(gps.encode(gpsSerial.read()))
    {
      //gps.f_get_position(&latitude,&longitude,
      //Serial.write(gpsData);
      LoRa.beginPacket();
      LoRa.print("Latitude: ");
      //Serial.print(gps.location.lat(),4);
      LoRa.print(gps.location.lat(),4);
      LoRa.print("  Longitude: ");
      LoRa.print(gps.location.lng(),4);
      LoRa.print("  Altitude (meters): ");
      LoRa.print(gps.altitude.meters(),4);
      LoRa.print("  Speed (meters/sec): ");
      LoRa.print(gps.speed.mps(),4);
      LoRa.print("  Course: ");
      LoRa.print(gps.course.deg(),4);
      LoRa.print("°");
      LoRa.println();
      //Serial.println();
      LoRa.endPacket();
    }
    
  }


}
