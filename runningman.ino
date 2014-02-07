/*
runningman.ino
2014-06-06 <jpa458 at gmail dot com> http://opensource.org/licenses/mit-license.php
*/
#include <JeeLib.h>


const unsigned int  PROGRAM_LENGTH  = 50; //# of periods
const byte LED = 9;
const unsigned int PERIOD = 60000;
const unsigned int SWITCH_DELAY = 400;
const int DIFFICULTY_LEVEL = 1;

Port relays (1);

/*
Machine program starts at 0,8 km/h
Increments are 0,1 km/h
*/

int program[PROGRAM_LENGTH]={8,16,16,16, 6, //warm up 0,8 > 1,6 > 3,2 > 4,8 > 6,4 > 7,0 
                             0, 0,10,16, 0,              // 7,0 > 7,0 > 8,0 > 9,6 > 9,6
                           -26, 0,10,16, 0,              
                           -26, 0,10,16, 0,              
                           -26, 0,10,16, 0,              
                           -26, 0,10,16, 0,
                           -26, 0,10,16, 0,
                           -26, 0,10,16, 0,
                           -26, 0,10,16, 0,
                           -26,-6,-16,-16,0};         // 7,0 > 6,4 > 4,8 > 3,2 > 3,2 
int counter=0;
 
void setup () {
    relays.digiWrite(LOW);
    relays.mode(OUTPUT);
    relays.digiWrite2(LOW);
    relays.mode2(OUTPUT);
    
  Serial.begin(57600);
  Serial.println("\n[Sport Program]");
  delay(10000);
  
}

// turn the on-board LED on or off
static void led (bool on) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, on ? 0 : 1); // inverted logic
}

void speedUp(){
  Serial.println("Send Pulse : Up");
  relays.digiWrite(HIGH);
  led(true);
  delay(SWITCH_DELAY/2);
  led(false);
  relays.digiWrite(LOW);
  delay(SWITCH_DELAY/2);

}

void slowDown(){
  Serial.println("Send Pulse : Down");
  relays.digiWrite2(HIGH);
  led(true);
  delay(SWITCH_DELAY/2);
  led(false);
  relays.digiWrite2(LOW);
  delay(SWITCH_DELAY/2);


}

void loop () {
  if (counter < PROGRAM_LENGTH) {
    Serial.print("Period : ");
    Serial.println(counter);
    
    
    int value = program[counter];
    int adjustedValue = abs(value) + DIFFICULTY_LEVEL;
    
    for (int j = 0; j < adjustedValue ; j++){
     (value > 0) ? speedUp() : slowDown();
    }
    
    if (value != 0) {
         Serial.println("Custom Delay");
         unsigned int delayValue = PERIOD - adjustedValue*SWITCH_DELAY; 
         Serial.println(delayValue);
         delay(delayValue);
    } else {
         Serial.println("Default Delay");
     delay(PERIOD);
    }
    counter++;
  } else {
   delay(1000);
   Serial.println("Done");
  }
}
