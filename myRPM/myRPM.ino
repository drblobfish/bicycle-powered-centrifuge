
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
  
  

  while(true){
    if (digitalRead(hall_pin)==0){
      if (on_state==false){

        float now = micros();

        //Serial.println(now);
        //Serial.println(start);
        //Serial.println(now-start);
        
        lastMeasures[pointer]= now-start;
        start = now;
        
        //printLastMeasures();
        //printInfo();
        mean_speed();
        
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
    
  Serial.println( 60000000/duration_mili);
  }

void printInfo(){
    //float time_passed = ((end_time-start)/1000000.0);
  }
