//include libraries
#include <Wire.h>  
#include <LiquidCrystal_I2C.h> // Using version 1.2.1

//register lcd panel
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

//register varables
int photocellPin0 = 0;                // the cell and 10K pulldown are connected to a0
int photocellPin1 = 1;                // the cell and 10K pulldown are connected to a1
int photocellReading[2];              // the analog reading from the sensor dividers
int readings[10][2];                  // the analog reading from the sensor0 dividers
int n = 0;                            // number counter for remembering in array
int max_reading[] = {0,0};            // maximum value read by sensors
int min_reading[] = {10000, 10000};   // minumum value read by sensors
boolean paper = false;                // used to switch the lcd

void setup(void) {
  // register lcd, sixteen characters across 2 lines
  lcd.begin(16,2); 
  lcd.backlight();
  
  // Only used for debug
  //Serial.begin(9600);   
}
 
void loop(void) {
  //reads the value of the sensors
  photocellReading[0] = analogRead(photocellPin0); 
  //Serial.println("0 : " + String(photocellReading[0])); 
  photocellReading[1] = analogRead(photocellPin1);  
  //Serial.println("1 : " + String(photocellReading[1])); 
  
  //remember the values read and process them
  n++;
  if(n == 10) { n=0; }
  Remember(photocellReading);

  CheckForSwitch();
  delay(50);
}

void Remember(int reading[]) {
  //checks which sensor is read and writes the data to array
  readings[n][0] = reading[0];
  //Serial.println("0 : " + String(reading[0])); 
  readings[n][1] = reading[1];
  //Serial.println("1 : " + String(reading[1])); 
  
  //checks if any maximum or minumum values are read, if so write them into the array
  for(int i = 0; i < 2; i++){
    if( reading[i] > max_reading[i]){
      //Serial.println("max: " + String(i) + " : " + String(max_reading[i]) + " old");
      max_reading[i] = reading[i];
      //Serial.println("max: " + String(i) + " : " + String(max_reading[i]));
      }
    if( reading[i] < min_reading[i]){
      //Serial.println("min: " + String(i) + " : " + String(min_reading[i]) + " old");
      min_reading[i] = reading[i];
      //Serial.println("min: " + String(i) + " : " + String(min_reading[i]));
    }
  }
}
  
void CheckForSwitch(){
  //check if the values are extreme enough to switch
  for(int i = 0; i < 2; i++){
    if (max_reading[i] - min_reading[i] >= 50){
      //initiates the switch
      //Serial.println("SWITCH");

      // switch text
      //Serial.println("Clear lcd");
      lcd.clear();
      //checks the state of the display, then switches to the other
      if (paper) {
        lcd.setCursor(0,0);
        lcd.print("|--Wood Here!");
        lcd.setCursor(0,1);
        lcd.print("V Paper Here!--V");
        paper = !paper;
      } else {
        lcd.setCursor(0,0);
        lcd.print("|-Paper Here!");
        lcd.setCursor(0,1);
        lcd.print("V  Wood Here!--V");
        paper = !paper;
      }

      //resets values to default
      max_reading[0] = 0;
      min_reading[0] = 10000;
      max_reading[1] = 0;
      min_reading[1] = 10000;
      for( int j = 0; j < 10; j++){
        readings[j][i]=0;
      }
    delay(500);
    }
  }
}
