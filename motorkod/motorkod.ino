#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int switchPin = 13; // the number of the switch pin
int switchState = 0;      // variable for reading the switch's status

const int startSpeed = 180; // startspeed of the motor
const int maxSpeed = 255; // maxspeed of the motor
const int runTime = 1; // time to run at max speed in seconds

const int stopTime = 5000; // wait 5000 ms between runs

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
  Serial.println("run forward at " + pwm);
}

void runReverse(int pwm)
{
  digitalWrite(MOT_A1_PIN, LOW);
  analogWrite(MOT_A2_PIN, pwm);
  Serial.println("run in reverse at " + pwm);
}

void stopMotor(void)
{
  digitalWrite(MOT_A1_PIN, HIGH);
  digitalWrite(MOT_A2_PIN, HIGH);
  Serial.println("stop motor");
}


bool offButtonPressed()
{
  switchState = digitalRead(switchPin); // read the state of the switch value:
  if (switchState == LOW)
  {
    Serial.println("turn off");
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
    delay(1000);    
  }
  return true;
}

void loop(void)
{
  Serial.println("Start");
  if (offButtonPressed())
  {
    return;
  }

  Serial.println("Start acceleration");
  for (int i = startSpeed; i <= maxSpeed; i++)
  {
    runForward(i);
    delay(delayTime);
  }

  // Full speed forward.
  Serial.println("run at max speed for runTime seconds");
  if (!run(runTime))
  {
    return;
  }

  // Decelerate the motor and stop
  for (int i = maxSpeed - 1; i >= 0; i--)
  {
    runForward(i);
  }


  stopMotor();
  Serial.println("stop for stopTime seconds");
  delay(stopTime);
  if (offButtonPressed())
  {
    return;
  }
  
  for (int i = startSpeed; i <= maxSpeed; i++)
  {
    runReverse(i);
    delay(delayTime);
  }

  // Full speed reverse.
  Serial.println("run reverse at max speed for runTime seconds");
  if (!run(runTime))
  {
    return;
  }

  // Decelerate the motor and stop
  for (int i = maxSpeed - 1; i >= 0; i--)
  {
    runReverse(i);
  }

  Serial.println("stop for stopTime seconds");
  stopMotor();
  delay(stopTime);
}
