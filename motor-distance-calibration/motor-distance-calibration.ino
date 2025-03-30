#include <AccelStepper.h>

//global constants
const int O_DIR = 2;                     //output pin to define motor direction
const int O_STEP = 3;                    //output pin to define how many motor steps to take 
const int I_LOWER_SWITCH = 4;            //input pin to define lower limit switch
const int I_UPPER_SWITCH = 7;            //input pin to define upper limit switch
const int O_LOWER_LED = 12;              //output pin to light up lower LED when limit switch hit
const int O_UPPER_LED = 13;              //output pin to light up upper LED when limit switch hit
const unsigned long DEBOUNCE_DELAY = 50; //debounce time in milliseconds; increase this value if LEDs flicker
const int INITIAL_LED_STATE = LOW;       
const int INITIAL_SWITCH_STATE = LOW;
const int MOTOR_INTERFACE_TYPE = AccelStepper::DRIVER;
const int CALIBRATION_SPEED = 200;
const int OPERATIONAL_SPEED = 4000;
const int ACCELERATION = 100;
const int STOP_ACCELERATION = 300;
const int STEP_SIZE = 10;

//global structs
typedef struct limit_switch_struct{
  int switch_pin;
  int led_pin;
  int switch_state;
  int led_state;
  int last_switch_state;
  unsigned long last_switch_debounce_time;
  long limit;
}limit_switch;

//global variables
limit_switch lower_switch = {I_LOWER_SWITCH, O_LOWER_LED, INITIAL_SWITCH_STATE, INITIAL_LED_STATE, INITIAL_SWITCH_STATE, 0, -1};
limit_switch upper_switch = {I_UPPER_SWITCH, O_UPPER_LED, INITIAL_SWITCH_STATE, INITIAL_LED_STATE, INITIAL_SWITCH_STATE, 0, -1};
AccelStepper myStepper(MOTOR_INTERFACE_TYPE, O_STEP, O_DIR);
bool calibrated = false;
int direction = -1; //find the negative limit first to set home point

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  // put your setup code here, to run once:
  pinMode(I_LOWER_SWITCH, INPUT);
  pinMode(O_LOWER_LED, OUTPUT);
  digitalWrite(O_LOWER_LED, lower_switch.led_state);

  pinMode(I_UPPER_SWITCH, INPUT);
  pinMode(O_UPPER_LED, OUTPUT);
  digitalWrite(O_UPPER_LED, upper_switch.led_state);

  myStepper.setMaxSpeed(OPERATIONAL_SPEED);
	myStepper.setAcceleration(ACCELERATION);
	myStepper.setSpeed(direction * CALIBRATION_SPEED);
  // TODO: logic to move until you hit a limit switch?
	//myStepper.moveTo(-20000); //find the negative limit first to set home point
}

long stop_now()
{
  long current_position = myStepper.currentPosition();
  //myStepper.setAcceleration(direction * - 1 * STOP_ACCELERATION);
  myStepper.stop();
  //myStepper.runToPosition();
  return current_position;
}

bool read_pin(limit_switch &my_switch)
{
  bool is_my_switch_hit = false;
  int reading = digitalRead(my_switch.switch_pin);
  if(reading != my_switch.last_switch_state)
  {
    my_switch.last_switch_debounce_time = millis();
  }

  if((millis() - my_switch.last_switch_debounce_time) > DEBOUNCE_DELAY)
  {
    if(reading != my_switch.switch_state)
    {
      my_switch.switch_state = reading;
      if(my_switch.switch_state == HIGH)
      {
        long current_position = stop_now();
        my_switch.led_state = !my_switch.led_state;
        my_switch.limit = current_position;
        is_my_switch_hit = true;
      }
    }
  }

  digitalWrite(my_switch.led_pin, my_switch.led_state);
  my_switch.last_switch_state = reading;
  return is_my_switch_hit;
}

void loop() {
  // put your main code here, to run repeatedly:
  bool is_lower_hit = read_pin(lower_switch);
  if(is_lower_hit)
  {
    direction = direction * -1;
    Serial.print("lower switch hit, changing direction to ");
    Serial.println(direction);
    calibrated = true; //expected to hit upper first, so hitting this means we're done calibrating
    //myStepper.setSpeed(direction * OPERATIONAL_SPEED);
  }
  bool is_upper_hit = read_pin(upper_switch);
  if(is_upper_hit)
  {
    direction = direction * -1;
    Serial.print("upper switch hit, changing direction to ");
    Serial.println(direction);
  }

  if(!calibrated && !is_lower_hit && !is_upper_hit)
  {
    myStepper.move(direction * STEP_SIZE);
    myStepper.setSpeed(direction * CALIBRATION_SPEED);
  }
  else if(calibrated)
  {
    if(direction < 0)
    {
      myStepper.moveTo(upper_switch.limit);
    }
    else
    {
      myStepper.moveTo(lower_switch.limit);
    }

    if (myStepper.distanceToGo() == 0)
    {
      direction = direction * -1;
      //myStepper.setSpeed(direction * OPERATIONAL_SPEED);
    }
  }

  myStepper.run();
}
