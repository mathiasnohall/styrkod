#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int switchPin = 13; // the number of the switch pin
int switchState = 0;      // variable for reading the switch's status

const int sensorPin = 2; // the rotation sensor pin
const int maxRotations = 10;
int rotations = 0;

const int maxSpeed = 255; // maxspeed of the motor
const int startSpeed = 105; // startspeed of the motor

const int stopTime = 2000; // wait stopTime miliseconds between runs

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
}


bool checkOffButtonPressed()
{
  switchState = digitalRead(switchPin); // read the state of the switch value:
  if (switchState == LOW)
  {
    stopMotor();
    rotations = 0;
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
    
    if(checkOffButtonPressed()){
      break;
    }
  }  
}

void wait()
{
  rotations = 0;
  stopMotor();
  delay(stopTime);
  checkOffButtonPressed();
}

void accelerateForward()
{
  for(int i = startSpeed; i <= maxSpeed; i += 20)
  {
    runForward(i);
    delay(100);
  }
}

void decelerateForward()
{
  for(int i = maxSpeed - 20; i >= startSpeed; i -= 20)
  {
    runForward(i);
    delay(100);
  }
}

void accelerateReverse()
{
  for(int i = startSpeed; i <= maxSpeed; i += 20)
  {
    runReverse(i);
    delay(100);
  }
}

void decelerateReverse()
{
  for(int i = maxSpeed - 20; i >= startSpeed; i -= 20)
  {
    runReverse(i);
    delay(100);
  }
}

void loop(void)
{
  if(checkOffButtonPressed()){
    return;
  }

  Serial.println("start");
  accelerateForward();
  run();
  decelerateForward();
    
  wait();
  
  if(checkOffButtonPressed()){
    return;
  }

  accelerateReverse();
  run();  
  decelerateReverse();
  
  wait();
}
