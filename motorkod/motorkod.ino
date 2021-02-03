#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int switchPin = 13; // the number of the switch pin
int switchState = 0;      // variable for reading the switch's status

const int sensorPin = 2; // the rotation sensor pin
int rotations = 0;

const int startSpeed = 155; // startspeed of the motor
const int maxSpeed = 255; // maxspeed of the motor
const int runTime = 2; // time to run at max speed in seconds

const int stopTime = 2; // wait 5 seconds between runs

const int delayTime = 10; // milliseconds between each speed step

void setup(void)
{
  // Initialize the stepper driver control pins to output drive mode.
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);

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
  rotations = 0;
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

bool run(int seconds)
{  
  for (int i = 0; i < seconds; i++)
  {
    if (offButtonPressed())
    {
      return false; // do not run any more
    }
    for(int y = 0; y < 10000; y++){
      rotations += analogRead(sensorPin);
    }
    Serial.println(rotations);
    delay(1000);
  }
  return true;
}

bool wait(int seconds)
{
  for (int i = 0; i < seconds; i++)
  {
    if (offButtonPressed())
    {
      return false; // do not run any more
    }
    delay(1000);   
  }
  return true;
}

void loop(void)
{
  if (offButtonPressed())
  {
    return;
  }

  rotations += digitalRead(sensorPin);
  Serial.println(rotations);
 
}
