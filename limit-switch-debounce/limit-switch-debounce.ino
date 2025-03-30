/*
  Debounce

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the output pin is toggled from LOW to HIGH or HIGH to LOW. There's a
  minimum delay between toggles to debounce the circuit (i.e. to ignore noise).

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.

  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/

// constants won't change. They're used here to set pin numbers:
const int lower_switch_pin = 4;  // the number of the pushbutton pin
const int upper_switch_pin = 7;
const int lower_led_pin = 12;    // the number of the LED pin
const int upper_led_pin = 13;

// Variables will change:
int lower_led_state = HIGH;        // the current state of the output pin
int upper_led_state = HIGH;
int lower_switch_state;            // the current reading from the input pin
int upper_switch_state;
int last_lower_switch_state = LOW;  // the previous reading from the input pin
int last_upper_switch_state = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long last_lower_switch_debounce_time = 0;  // the last time the output pin was 
unsigned long last_upper_switch_debounce_time = 0;
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(lower_switch_pin, INPUT);
  pinMode(upper_switch_pin, INPUT);
  pinMode(lower_led_pin, OUTPUT);
  pinMode(upper_led_pin, OUTPUT);

  // set initial LED state
  digitalWrite(lower_led_pin, lower_led_state);
  digitalWrite(upper_led_pin, upper_led_state);
}

void loop() {
  // read the state of the switch into a local variable:
  int lower_reading = digitalRead(lower_switch_pin);
  int upper_reading = digitalRead(upper_switch_pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (lower_reading != last_lower_switch_state) {
    // reset the debouncing timer
    last_lower_switch_debounce_time = millis();
  }

  if (upper_reading != last_upper_switch_state) {
    // reset the debouncing timer
    last_upper_switch_debounce_time = millis();
  }

  if ((millis() - last_lower_switch_debounce_time) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (lower_reading != lower_switch_state) {
      lower_switch_state = lower_reading;

      // only toggle the LED if the new button state is HIGH
      if (lower_switch_state == HIGH) {
        lower_led_state = !lower_led_state;
      }
    }
  }

  // set the LED:
  digitalWrite(lower_led_pin, lower_led_state);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  last_lower_switch_state = lower_reading;

if ((millis() - last_upper_switch_debounce_time) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (upper_reading != upper_switch_state) {
      upper_switch_state = upper_reading;

      // only toggle the LED if the new button state is HIGH
      if (upper_switch_state == HIGH) {
        upper_led_state = !upper_led_state;
      }
    }
  }

  // set the LED:
  digitalWrite(upper_led_pin, upper_led_state);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  last_upper_switch_state = upper_reading;
}
