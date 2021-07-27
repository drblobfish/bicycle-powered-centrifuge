#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x3E,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article

// digital pin 2 is the hall pin
int hall_pin = 2;
// set number of hall trips for RPM reading (higher improves accuracy)

float radius_wheel = 17;

const int meanSize = 1;
float lastMeasures[meanSize];
int pointer = 0;

bool on_state = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the hall pin an input:
  pinMode(hall_pin, INPUT);

  // Set off LCD module
   lcd.begin (16,2); // 16 x 2 LCD module
   lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
   lcd.setBacklight(HIGH);
}

float start = micros();

float time_refresh = millis();
float min_refresh_period = 500;

void loop() {
  
  // put your main code here, to run repeatedly:
  // preallocate values for tach
  
  
  // counting number of times the hall sensor is tripped
  // but without double counting during the same trip
  
  

  while(true){
    if (digitalRead(hall_pin)==0){
      if (on_state==false){

        float now = micros();

        //Serial.println(now);
        //Serial.println(start);
        //Serial.println(now-start);
        
        lastMeasures[pointer]= now-start;
        start = now;
        

        if ((millis()-time_refresh)>min_refresh_period){
          mean_speed();
          time_refresh = millis();
          }
        
        
        pointer +=1;
        if (pointer == meanSize){
          pointer = 0;
          }
        on_state = true;
        
        break;
        
      }
    } else{
      on_state = false;
    }
  }
}

void printLastMeasures(){
  for (int i=0;i<meanSize;i++){
    Serial.print(lastMeasures[i]);
    Serial.print(",");
    }
    Serial.println("");
  }

void mean_speed(){
  float sum = 0;
  for (int i=0;i<meanSize;i++){
    sum += lastMeasures[i];
    }

  float duration_mili = sum/meanSize;
  float rpm = 60000000/duration_mili;
  float rcf = 1.118 * rpm*rpm*radius_wheel/100000;
    
  Serial.println(rpm );
  lcd.clear(); // Set cursor to 0,0
  lcd.print("rpm : ");
  lcd.print(int(rpm));
  
  lcd.setCursor(0, 1);
  lcd.print("rcf : ");
  lcd.print(int(rcf));
  }

void printInfo(){
    //float time_passed = ((end_time-start)/1000000.0);
  }
