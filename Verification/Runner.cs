using System.Threading;

namespace Verification
{
    public class Runner
    {
        public Runner()
        {
            setup();
        }

        public void Run()
        {
            while (true)
            {
                loop();
            }
        }

        void analogWrite(int input1, int input2)
        {

        }

        void digitalWrite(int input1, int input2)
        {

        }

        void pinMode(int input, string input2)
        {

        }

        int digitalRead(int port)
        {
            return HIGH;
        }

        void delay(int time)
        {
            Thread.Sleep(time);
        }

        const int LOW = 0;
        const int HIGH = 1;

        const string OUTPUT = "OUTPUT";


        // This program assumes that:
        //
        //  1. A DRV8833 DC motor driver module is connected to pins 5, 6.
        //  2. A motor is attached to the driver.
        //  3. The serial console on the Arduino IDE is set to 9600 baud communications speed.

        // ================================================================================
        // Define the pin numbers on which the outputs are generated.
        const int MOT_A1_PIN  = 5;
        const int MOT_A2_PIN = 6;

        const int switchPin = 13; // the number of the switch pin
        int switchState = 0;      // variable for reading the switch's status

        const int startSpeed = 180; // startspeed of the motor
        const int maxSpeed = 255; // maxspeed of the motor
        const int runTime = 1; // time to run at max speed

        const int stopTime = 5000; // wait 5000 ms between runs

        const int delayTime = 10; // milliseconds between each speed step

        // ================================================================================
        /// Configure the hardware once after booting up.  This runs once after pressing
        //// reset or powering up the board.
        void setup()
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
            if (pwm < 0)
            { // reverse speeds
                analogWrite(IN1_PIN, -pwm);
                digitalWrite(IN2_PIN, LOW);
            }
            else
            { // stop or forward
                digitalWrite(IN1_PIN, LOW);
                analogWrite(IN2_PIN, pwm);
            }
        }

        // ================================================================================
        /// Set the current on both motors.
        ///
        /// \param pwm motor  PWM, -255 to 255
        void set_motor_current(int pwm)
        {
            set_motor_pwm(pwm, MOT_A1_PIN, MOT_A2_PIN);

            // Print a status message to the console.
            Serial.println("Set motor A PWM = " + pwm);
        }

        bool offButtonPressed()
        {
            switchState = digitalRead(switchPin); // read the state of the switch value:
            if (switchState == LOW)
            {
                Serial.println("turn off");
                digitalWrite(MOT_A1_PIN, LOW);
                digitalWrite(MOT_A2_PIN, LOW);
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

        // ================================================================================
        /// Run one iteration of the main event loop.  The Arduino system will call this
        /// function over and over forever.
        void loop()
        {
            Serial.println("Start");
            if (offButtonPressed())
            {
                return;
            }

            //Accelerate the motor
            Serial.println("Start acceleration");
            for (int i = startSpeed; i <= maxSpeed; i++)
            {
                set_motor_current(i);
                delay(delayTime);
            }

            // Full speed forward.
            Serial.println("run at max speed for runTime seconds");
            if (!run(runTime))
            {
                return;
            }

            // Decelerate the motor and stop
            for (int i = maxSpeed; i <= 0; i--)
            {
                set_motor_current(i);
            }
            Serial.println("stop for stopTime seconds");
            delay(stopTime);
            if (offButtonPressed())
            {
                return;
            }

            //Run in reverse
            //Accelerate the motor
            Serial.println("Start acceleration");
            for (int i = -startSpeed; i >= -maxSpeed; i--)
            {
                set_motor_current(i);
                delay(delayTime);
            }

            // Full speed reverse.
            Serial.println("run at max speed for runTime seconds");
            if (!run(runTime))
            {
                return;
            }

            // Decelerate the motor and stop
            for (int i = -maxSpeed; i >= 0; i++)
            {
                set_motor_current(i);
            }

            Serial.println("stop for stopTime seconds");
            set_motor_current(0);
            delay(stopTime);
        }

    }
}
