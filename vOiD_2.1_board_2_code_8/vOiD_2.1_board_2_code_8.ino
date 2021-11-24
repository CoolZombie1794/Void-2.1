//6
//7 : Update - attached ir sensor
//8 : Update - bug fixes - removal of delays caused by temp and humidity system



#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);



#include <IRremote.h>
#define irPin 8
 
IRrecv irrecv(irPin);
decode_results results;

#define echopin  A1 // echo pin
#define trigpin A2 // Trigger pin


int timer = 0;
int bconst = 0;

int chk = 0;
int set = 35;

const int analogInPin = A0;                               //lights
int sensorValue = 0;                                      //lights
int outputValue = 0;                                      //lights

#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX

int bt_ir_data;

void setup() {

 pinMode (trigpin, OUTPUT);
 pinMode (echopin, INPUT );

Serial.begin(9600); // start serial communication at 9600bps


irrecv.enableIRIn();

  
BT_Serial.begin(9600); 
delay(200);



pinMode(4, OUTPUT);                                        //led blinking
pinMode(5, OUTPUT);                                        //lights   
pinMode(6, OUTPUT);                                        //Buzzer

  dht.begin();
  
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.print("Void 2.0, By:");
  lcd.setCursor(0,1);
  lcd.print("Harshit Singh");
  delay(4000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.setCursor(0,1);
  lcd.print("Humidity = ");
  lcd.setCursor(9,0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(13,1);
  lcd.print("%");

}

void loop() {

bconst = 0;
timer = timer + 1;

if(BT_Serial.available() > 0){     //if some date is sent, reads it and saves in state     
bt_ir_data = BT_Serial.read(); 
Serial.println(bt_ir_data);      
}


if (irrecv.decode(&results )) {
      Serial.println(results.value, HEX);
      
      switch (results.value) {
        
         case 33456255:            // button blue frwrd auto - lights
            bt_ir_data = 13;
            break;
 
         case 33439935:            // button back lights off
            bt_ir_data = 12;
            break;
               
         case 33472575:            // button frwrd lights on
            bt_ir_data = 11;
            break;
 
         case 33448095:            // button vol + buzz on
            bconst = 1;
            break;
 
         //case 33464415:            // button vol - buzz off
           // bconst = 2;
           // break;
 
         
         }      
   irrecv.resume();
   }

   
if (timer == 20){

  int  h = dht.readHumidity();
  int  t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;}

    float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(hic);
  Serial.print(F("C "));

  lcd.setCursor(7,0);
  lcd.print(t);
  lcd.setCursor(11,1);
  lcd.print(h);
  timer = 0;
  }



//===============================================================================
//                        Blinking Light
//=============================================================================== 

digitalWrite(4, HIGH);
delay(100);
digitalWrite(4, LOW);
delay(100);

//===============================================================================
//                             Lights
//=============================================================================== 
 {
     if(bt_ir_data == 11){digitalWrite(5, HIGH);}                   //On
else if(bt_ir_data == 12){digitalWrite(5, LOW);}                    //Off
else if(bt_ir_data == 13){                                          //Auto
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  {
       if (outputValue > 80){digitalWrite(5, LOW);}
  else if (outputValue <= 80){digitalWrite(5, HIGH);}
  }
 }
 }

//===============================================================================
//                             Buzzer
//===============================================================================
{
     if(bt_ir_data == 14){
                          digitalWrite(6, HIGH);
                          delay(500);
                          digitalWrite(6, LOW);
                          delay(500);
                        
                          
                          }
else if(bt_ir_data == 15){
             digitalWrite(6, LOW);}

      if (bconst == 1){
                          digitalWrite(6, HIGH);
                          delay(5000);
                          digitalWrite(6, LOW);    
                          }
      }

bt_data_sending();

}                                                                     //void loop ends

s



 
void bt_data_sending(){
//===============================================================================
//                              BT data sending
//=============================================================================== 

  int  h = dht.readHumidity();
  int  t = dht.readTemperature();
 BT_Serial.print(t); //send distance to MIT App
 BT_Serial.print(";");
 BT_Serial.print(h); //send distance to MIT App 
 BT_Serial.println(";");


}










 
