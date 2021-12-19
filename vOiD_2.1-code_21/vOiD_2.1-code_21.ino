//09 : Update - Now it will stop if any obstacle is in from of it, even in rc/ line following mode
//10 : Update - Button added - force stop
//11 : Update - sends dist to app - 1
//12 : Update - sends dist to app - 2
//13 : Update - addition of another BT module, removed sending data to app
//14 : Update - Sends dist to app - 3 - Successful!!!
//15 : Update - bug fixes
//16 : Update - fixed issue of malfunctioning of bot speed
//17 : Update - Fixed ir remote not functioning
//18 : Update - Stops the bot from crashing if ir remote forward button is used - 1
//19 : Update - Stops the bot from crashing if ir remote forward button is used - 2 - Successful!!!
//20 : Update - Bug Fixes - Prevenyting the bot from malfunctioning while using ir remote
//21 : Update - removed some delays


#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(2, 3); // RX, TX

#include <IRremote.h>
#define RECV_PIN A5
IRrecv irrecv(RECV_PIN);
decode_results results;

int ifconst = 10;


#define enA 10//Enable1 L298 Pin enA 
#define in1 9 //Motor1  L298 Pin in1 
#define in2 8 //Motor1  L298 Pin in1 
#define in3 7 //Motor2  L298 Pin in1 
#define in4 6 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 

#define servo A4

#define R_S A0 //ir sensor Right
#define L_S A1 //ir sensor Left

#define echo A2    //Echo pin
#define trigger A3 //Trigger pin

int distance_L, distance_F = 30, distance_R;
long distance;
int set = 35;
int chk = 0;


int bt_ir_data; // variable to receive data from the serial port and IRremote
int Speed = 130;  
int mode=0;
int IR_data;


int timer = 0;
int timer2 = 0;

int btsend = 0;
int btdt = 0;

void setup(){ // put your setup code here, to run once
pinMode(11, OUTPUT);                                        //led blinking


pinMode(R_S, INPUT); // declare if sensor as input  
pinMode(L_S, INPUT); // declare ir sensor as input

pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  

pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

irrecv.enableIRIn(); // Start the receiver
irrecv.blink13(true);

Serial.begin(9600); // start serial communication at 9600bps
BT_Serial.begin(9600); 

pinMode(servo, OUTPUT);

 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }

 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
delay(500);
}



void loop(){  

timer += 1;
timer2 += 1;

if(BT_Serial.available() > 0){  //if some date is sent, reads it and saves in state     
bt_ir_data = BT_Serial.read(); 
Serial.println(bt_ir_data);     
if((260 > bt_ir_data)&&(bt_ir_data > 39)){Speed = bt_ir_data;}      
}

if (irrecv.decode(&results )) {
      Serial.println(results.value, HEX);
      
      switch (results.value) {

        case 33441975:            // Power button mode 0
            Stop();
            bt_ir_data = -1;
            mode = 0;
            break;
            
        case 33446055:            // Mode button mode 1
            Stop();
            bt_ir_data = -1;
            mode = 1;
            break;
        case 33454215:            // Mute button mode 2
            Stop();
            bt_ir_data = -1;
            mode = 2;
            break;

    distance_F = Ultrasonic_read();
if((distance_F<set) && (chk==1)){chk = 2; Stop();}
if( distance_F>set){chk = 0;}

        if((mode == 0)&&(chk==0)){
          
          case 33478695:            // button 2 FORDWARD
            if (distance_F>set){
            bt_ir_data = 1;
            break;
            }
            else if (distance_F<set){
              Stop();
              break;
            }
            
         case 33435855:            // button 4 LEFT
            bt_ir_data = -1;
            turnLeft();
            break;
               
         case 33468495:            // button 5 STOP
            bt_ir_data = -1;
            Stop();
            break;
 
         case 33452175:            // button 6 RIGHT
            bt_ir_data = -1;
            turnRight();
            break;
 
         case 33484815:            // button 8 BACK
            Stop();
            bt_ir_data = -1;
            backward();
            break;}
         }      
   irrecv.resume();
   }




if(timer == 100){
  btsend = Ultrasonic_read();
if (btsend > 200){btsend = 200;};
  btdt = btsend + 270;
  BT_Serial.print(btdt);
  BT_Serial.print(";");
  timer = 0;
}


//===============================================================================
//                             Mode Selection
//=============================================================================== 
     if(bt_ir_data == 8){mode=0; Stop();}    //Manual Android Application and IR Remote Control Command   
else if(bt_ir_data == 9){mode=1; Speed=130;} //Auto Line Follower Command
else if(bt_ir_data ==10){mode=2; Speed=130;} //Auto Obstacle Avoiding Command
//===============================================================================




//===============================================================================
//                             Speed controlling
//=============================================================================== 
analogWrite(enA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 
//=============================================================================== 




if(mode==0){     
//===============================================================================
//                          Key Control Command
//=============================================================================== 

distance_F = Ultrasonic_read();
if((distance_F<set) && (chk==1)){chk = 2; Stop();}
if(distance_F>set){chk = 0;}

     if((bt_ir_data == 1) && (chk==0)){chk = 1; forward(); }  // if the bt_data is '1' the DC motor will go forward
else if(bt_ir_data == 2){backward();}  // if the bt_data is '2' the motor will Reverse
else if(bt_ir_data == 3){turnLeft();}  // if the bt_data is '3' the motor will turn left
else if(bt_ir_data == 4){turnRight();} // if the bt_data is '4' the motor will turn right
else if(bt_ir_data == 5){Stop(); }     // if the bt_data '5' the motor will Stop

//===============================================================================
//                          Voice Control Command
//===============================================================================    
else if(bt_ir_data == 6){turnLeft();  delay(500);  bt_ir_data = 5;}
else if(bt_ir_data == 7){turnRight(); delay(500);  bt_ir_data = 5;}
}

if(mode==1){    
//===============================================================================
//                          Line Follower Control
//===============================================================================  

distance_F = Ultrasonic_read();
if((distance_F<set) && (chk==1)){chk = 2; Stop();}
if(distance_F>set){chk = 0;}


if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0) && (chk==0)){chk = 1;forward();}  //if Right Sensor and Left Sensor are at White color then it will call forward function
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnRight();}//if Right Sensor is Black and Left Sensor is White then it will call turn Right function  
if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnLeft();} //if Right Sensor is White and Left Sensor is Black then it will call turn Left function
if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){Stop();}     //if Right Sensor and Left Sensor are at Black color then it will call Stop function

if(bt_ir_data == 16){mode=0; Stop();delay(2000);} 

} 

if(mode==2){    
//===============================================================================
//                          Obstacle Avoiding Control
//===============================================================================     
 
 
 
 distance_F = Ultrasonic_read();
 Serial.print("S=");Serial.println(distance_F);
  if (distance_F > set){forward();}
    else{Check_side();}

    if(bt_ir_data == 16){mode=0; Stop();delay(2000);} 
}

//===============================================================================
//                             Blinking LED
//=============================================================================== 
if (timer2 == 100){digitalWrite(11, HIGH);}
else if (timer2 == 200){digitalWrite(11, LOW);timer2 = 0;}
//===============================================================================


delay(1);
}                                                                                   //Void loop ends

void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50);                   // Refresh cycle of servo
}


//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echo, HIGH);
  return distance / 29 / 2;
}

void compareDistance(){
       if (distance_L > distance_R){
  turnLeft();
  delay(350);
  }
  else if (distance_R > distance_L){
  turnRight();
  delay(350);
  }
  else{
  backward();
  delay(300);
  turnRight();
  delay(600);
  }
}

void Check_side(){
    Stop();
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_L = Ultrasonic_read();
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_R = Ultrasonic_read();
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();

}






//===============================================================================
//                              Movement controls
//===============================================================================

void forward(){//forward
digitalWrite(in1, HIGH); //Right Motor forward Pin 
digitalWrite(in2, LOW);  //Right Motor backward Pin 
digitalWrite(in3, LOW);  //Left Motor backward Pin 
digitalWrite(in4, HIGH); //Left Motor forward Pin 
 
}

void backward(){ //backward
digitalWrite(in1, LOW);  //Right Motor forward Pin 
digitalWrite(in2, HIGH); //Right Motor backward Pin 
digitalWrite(in3, HIGH); //Left Motor backward Pin 
digitalWrite(in4, LOW);  //Left Motor forward Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW);  //Right Motor forward Pin 
digitalWrite(in2, HIGH); //Right Motor backward Pin  
digitalWrite(in3, LOW);  //Left Motor backward Pin 
digitalWrite(in4, HIGH); //Left Motor forward Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Right Motor forward Pin 
digitalWrite(in2, LOW);  //Right Motor backward Pin 
digitalWrite(in3, HIGH); //Left Motor backward Pin 
digitalWrite(in4, LOW);  //Left Motor forward Pin 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Right Motor forward Pin 
digitalWrite(in2, LOW); //Right Motor backward Pin 
digitalWrite(in3, LOW); //Left Motor backward Pin 
digitalWrite(in4, LOW); //Left Motor forward Pin 
}
