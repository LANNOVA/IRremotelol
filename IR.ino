// Include the Arduino Stepper Library
#include <Stepper.h>
#include <IRremote.h>
 
const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
const int stepsPerRevolution = 200;
 
Stepper myStepper(stepsPerRevolution, 4,5,6,7);
 
int lim = 39; //lim is the max number of linear axis steps
int count = 0;
int minstep=28; //~2 degrees at28 to 30
int OldX=0;
int OldY=0;
int NewX=0;
int NewY=0;
int IsHome=0;
int buttonState = 0;
 
int nav(int x, int y);
int goHome();
 
void setup(){
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(true);
  
  myStepper.setSpeed(30);
pinMode(14, OUTPUT); //DC motor
pinMode(15, OUTPUT); //dc motor other direction
pinMode(10, INPUT_PULLUP);
  Serial.begin(9600);
  delay(6000);
//if(IsHome==0){
//goHome();
//}
 
}
  
 
 
void loop(){
  if (IrReceiver.decode())
   {
      Serial.println(IrReceiver.decodedIRData.command);
      if(IrReceiver.decodedIRData.command==21){
        goHome();
        //isHome now set to 1
        }
      if(IrReceiver.decodedIRData.command==64){
        //y axis +
         myStepper.step(10);
         
        }
      if(IrReceiver.decodedIRData.command==25){
        //move towards center
         myStepper.step(-10);
        }
      if(IrReceiver.decodedIRData.command==7){
        //counter clock wize
      analogWrite(14,0);
      analogWrite(15, 150); //motor beeps if its too low
      delay(30);
      analogWrite(15,0);
        }
      if(IrReceiver.decodedIRData.command==9){
        //step clockwize
          analogWrite(15,0);
      analogWrite(14, 150); //motor beeps if its too low
      delay(30);
      analogWrite(14,0);
      }
      IrReceiver.resume();
   }
} //end void loop()
 
 
 
 
int nav(int x, int y){
//x is infinite, y is limited. if y is negative the limit is 0.
// y is allso max limited at IDK, like 70 turns, so figure out the limit there too.
int difX=NewX-OldX;
int difY=NewY-OldY;
 
  if(NewX > OldX){
       analogWrite(14, 55); //motor beeps if its too low
delay(difX*60);
analogWrite(14,0);  
    }
   if(NewX < OldX){
      analogWrite(15, 55); //motor beeps if its too low
      delay(difX*60);
      analogWrite(15,0);
    }
   
    if(NewX == OldX){
       analogWrite(14,0);
       analogWrite(15,0);
      }
 
 if(NewY < OldY){
  if(difY<0){
    difY=0;
    goHome();
    }else{
     myStepper.step(difY);
  }
 }
 if(NewY > OldY){
     myStepper.step(difY);
  }
     if(NewY == OldY){
        myStepper.halt();
      }
 OldX=NewX;
OldY=NewY;
 
  return 0;
  }
 
int goHome(){
  buttonState = digitalRead(10);
while(buttonState==HIGH){
  //Serial.println("HIGH");
   myStepper.step(stepsPerRevolution); //going home
    Serial.println("High");
 
   buttonState = digitalRead(10);
  if(buttonState==LOW) {
  Serial.println("LOW");
     //move to center
   for (int i =0; i<20; i++){
   myStepper.step(-stepsPerRevolution);
   Serial.print("i step count: ");
   Serial.println(i);
  }
  myStepper.halt();
  IsHome=1;
  }
}
  return IsHome;
  }    
