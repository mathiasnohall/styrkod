// This program assumes that:
//
//  1. A DRV8833 DC motor driver module is connected to pins 5, 6.
//  2. A motor is attached to the driver.
//  3. The serial console on the Arduino IDE is set to 9600 baud communications speed.

// ================================================================================
// Define constant values and global variables.

// Define the pin numbers on which the outputs are generated.
#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int switchPin = 13; // the number of the switch pin
int switchState = 0;  // variable for reading the switch's status

const int maxSpeed = 200;

// ================================================================================
/// Configure the hardware once after booting up.  This runs once after pressing
//// reset or powering up the board.
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

// ================================================================================
/// Set the current on a motor channel using PWM and directional logic.
/// Changing the current will affect the motor speed, but please note this is
/// not a calibrated speed control.  This function will configure the pin output
/// state and return.
///
/// \param pwm    PWM duty cycle ranging from -255 full reverse to 255 full forward
/// \param IN1_PIN  pin number xIN1 for the given channel
/// \param IN2_PIN  pin number xIN2 for the given channel

void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0) {  // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);

  } else { // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}
// ================================================================================
/// Set the current on both motors.
///
/// \param pwm_A  motor A PWM, -255 to 255

void set_motor_current(int pwm)
{
  set_motor_pwm(pwm, MOT_A1_PIN, MOT_A2_PIN);

  // Print a status message to the console.
  Serial.print("Set motor A PWM = ");
  Serial.print(pwm);
}


// ================================================================================
/// Run one iteration of the main event loop.  The Arduino system will call this
/// function over and over forever.
void loop(void)
{
   Serial.println("Start");
   switchState = digitalRead(switchPin); // read the state of the switch value:
   if(switchState == LOW){    
    Serial.println("stäng av med knapp");    
    digitalWrite(MOT_A1_PIN, LOW);
    digitalWrite(MOT_A2_PIN, LOW);
    return;
   }
 
  int delayTime = 50; //milliseconds between each speed step

  set_motor_current(50);
  
  //Accelerates the motor
  Serial.println("Starta acceleration");
  for(int i = 50; i < maxSpeed + 1; i++){
    set_motor_current(i);
    delay(50);
  }

  // Full speed forward.
  Serial.println("max speed");
  set_motor_current(maxSpeed);
  delay(5000);
  
  // Stop.
  Serial.println("stop");
  set_motor_current(0);

}
