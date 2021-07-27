//including the libraries (the LiquidCrystal_I2C library has been modified to work under linux by us)

#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*

//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
LiquidCrystal_I2C lcd(0x3E,2,1,0,4,5,6,7); // 0x3E is the I2C bus address of the backpack

// digital pin 2 is the hall effect sensor pin
int hall_pin = 2;

// radius of the wheel to compute the rcf
float radius_wheel = 17;

// We do  a gliding average on the last <meanSize> measures
// So the period of the wheek is stored in a array of size <meanSize>
const int meanSize = 1;
float lastMeasures[meanSize];
// We use a pointer to move in this array
int pointer = 0;

// on_state is set to true when the sensor sense the magnet
// it is used by the arduino to know when it is sensing the magnet for the 1st time
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

// start is the time in microsecond of the last time the sensor started sensing the magnet
float start = micros();

// we store the time in millisecond of the last time the lcd screen was refreshed
float time_last_refresh = millis();
float min_refresh_period = 500;

void loop() {
  if (digitalRead(hall_pin)==0){ // if we sense the magnet
    if (on_state==false){// if we previously didn't sense it

      float now = micros();
      
      lastMeasures[pointer]= now-start;// we put the duration of one rotation in the array
      start = now;
      
      pointer +=1;// increment the pointer or put it to 0
      if (pointer == meanSize){
        pointer = 0;
        }
      on_state = true;//put in memory that we are sensing the magnet

      if ((millis()-time_last_refresh)>min_refresh_period){
        //if duration since the last refresh of the screen is bigger than 0.5 sec
        mean_speed(); //refresh the screen
        time_last_refresh = millis();
        }
      
    }
  } else{
    on_state = false;
  }
}

void printLastMeasures(){// print the data array values separated by a comma
  for (int i=0;i<meanSize;i++){
    Serial.print(lastMeasures[i]);
    Serial.print(",");
    }
    Serial.println("");
  }

void mean_speed(){
  // compute the mean of the values
  float sum = 0;
  for (int i=0;i<meanSize;i++){
    sum += lastMeasures[i];
    }

  // compute the rpm and rcf
  float duration_mili = sum/meanSize;
  float rpm = 60000000/duration_mili;
  float rcf = 1.118 * rpm*rpm*radius_wheel/100000;

  //print everything on the lcd screen
  Serial.println(rpm );
  lcd.clear();
  lcd.print("rpm : ");
  lcd.print(int(rpm));
  //second line
  lcd.setCursor(0, 1);
  lcd.print("rcf : ");
  lcd.print(int(rcf));
  }
