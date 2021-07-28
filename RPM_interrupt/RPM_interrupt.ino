#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library

LiquidCrystal_I2C lcd(0x3E,2,1,0,4,5,6,7); // 0x3E is the I2C bus address of the backpack


int hallsensor = 2;                // Hall sensor at pin 2

volatile byte counter;

unsigned int rpm;

unsigned long passedtime;

void isr()

 {

   //Each rotation, this interrupt function is run twice, so take that into consideration for

   //calculating RPM

   //Update count

      counter++;
 }
void setup()

 {Serial.begin(9600);

 lcd.begin (16,2); // 16 x 2 LCD module
   lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
   lcd.setBacklight(HIGH);

   //Intiates Serial communications

   attachInterrupt(digitalPinToInterrupt(hallsensor), isr, RISING); //Interrupts are called on Rise of Input

   pinMode(hallsensor, INPUT); //Sets hallsensor as input

   counter = 0;

   rpm = 0;

   passedtime = 0; //Initialise the values

 }
 void loop()
 {
   delay(1000);//Update RPM every second

   detachInterrupt(0); //Interrupts are disabled

   rpm = 60000/(millis() - passedtime)*counter;

   passedtime = millis();

   counter = 0;

   Serial.print("RPM=");
   Serial.println(rpm); //Print out result to monitor

   lcd.clear();
   lcd.print("RPM=");
   lcd.print(rpm);

   attachInterrupt(0, isr, RISING);   //Restart the interrupt processing

  }
