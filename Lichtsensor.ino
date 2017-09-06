/* Photocell simple testing sketch. 
 
Connect one end of the photocell to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
Connect LED from pin 11 through a resistor to ground 
For more information see http://learn.adafruit.com/photocells */
 
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int LEDpin = 11;          // connect Red LED to pin 11 (PWM pin)
int LEDbrightness;        // 
int readings[20];          // the analog reading from the sensor divider in an array
int n = 0;
int max_reading = 0;
int min_reading = 10000;

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
}
 
void loop(void) {
  photocellReading = analogRead(photocellPin);  
  
  Remember(photocellReading);
 
  // LED gets brighter the darker it is at the sensor
  // that means we have to -invert- the reading from 0-1023 back to 1023-0
  photocellReading = 1023 - photocellReading;
  //now we have to map 0-1023 to 0-255 since thats the range analogWrite uses
  LEDbrightness = map(photocellReading, 0, 1023, 0, 255);
  analogWrite(LEDpin, LEDbrightness);
 
  delay(50);
}

void Remember(int reading) {
  //makes sure the array keeps looping
  n++;
  if(n == 20) { n=0; }
  readings[n] = reading;

  if( reading > max_reading){
    max_reading = reading;
    Serial.println(String(max_reading));
    }
  if( reading < min_reading){
    min_reading = reading;
    Serial.println(String(min_reading));
    }

  if (n == 19 && (max_reading - min_reading) >= 100){
    Serial.println("SWITCH");
    max_reading = 0;
    min_reading = 10000;
    for( int i = 0; i < 20; i++){
      readings[i]=0;
      }
    delay(2000);
    }
    
}

