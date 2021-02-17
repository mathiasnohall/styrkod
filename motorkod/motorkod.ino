#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int switchPin = 13; // the number of the switch pin
int switchState = 0;      // variable for reading the switch's status

const int sensorPin = 2; // the rotation sensor pin
const int maxRotations = 2;
int rotations = 0;

const int maxSpeed = 255; // maxspeed of the motor

const int stopTime = 2; // wait 5 seconds between runs

void setup(void)
{
  // Initialize the stepper driver control pins to output drive mode.
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(sensorPin, INPUT);

  // Start with drivers off, motors coasting.
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);

  // Initialize the serial UART at 9600 bits per second.
  Serial.begin(9600);
}

void runForward(int pwm)
{
  analogWrite(MOT_A1_PIN, pwm);
  digitalWrite(MOT_A2_PIN, LOW);
}

void runReverse(int pwm)
{
  digitalWrite(MOT_A1_PIN, LOW);
  analogWrite(MOT_A2_PIN, pwm);
}

void stopMotor(void)
{
  digitalWrite(MOT_A1_PIN, HIGH);
  digitalWrite(MOT_A2_PIN, HIGH);
  rotations = maxRotations - rotations;
}


bool offButtonPressed()
{
  switchState = digitalRead(switchPin); // read the state of the switch value:
  if (switchState == LOW)
  {
    stopMotor();
    return true;
  }
  return false;
}

void run()
{  
  bool on_state = false;
  while(true){    
    if (digitalRead(sensorPin)==LOW){
      if (on_state==false){
        on_state = true;
        rotations++;
      }
    } else{
      on_state = false;
    }
    
    if (rotations>=maxRotations){
      Serial.println(rotations);
      break;
    }
    
    if (offButtonPressed()){
      break;
    }
  }  
}

void wait(int seconds)
{
  delay(seconds * 1000);    
}

void loop(void)
{
  Serial.println("start");
  
  stopMotor();
  if (offButtonPressed())
  {
    return;
  }
  
  runForward(maxSpeed);
  run();
    
  stopMotor();
  if (offButtonPressed())
  {
    return;
  }
  wait(stopTime);  
  if (offButtonPressed())
  {
    return;
  }

  runReverse(maxSpeed);
  run();
  
  stopMotor();
  wait(stopTime);
}
