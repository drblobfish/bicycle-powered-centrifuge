
// digital pin 2 is the hall pin
int hall_pin = 2;
// set number of hall trips for RPM reading (higher improves accuracy)

const int meanSize = 10;
float lastMeasures[meanSize];
int pointer = 0;

bool on_state = false;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the hall pin an input:
  pinMode(hall_pin, INPUT);
}

float start = micros();

void loop() {
  
  // put your main code here, to run repeatedly:
  // preallocate values for tach
  
  
  // counting number of times the hall sensor is tripped
  // but without double counting during the same trip
  
  
  if (digitalRead(hall_pin)==0){
    if (on_state==false){

      float now = micros();

      Serial.println( (60000000/(now-start)));

      start = now;
      
      on_state = true;
      
    }
  } else{
    on_state = false;
  }
}
