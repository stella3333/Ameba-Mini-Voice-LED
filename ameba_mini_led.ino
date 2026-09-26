/* Ameba Mini voice-controlled LED receiver.
 * Change the pin values and active level to match your board's official pinout.
 */
#include <Arduino.h>

// AMB82-MINI official pin map: Arduino pin 23 = PF9 = LED_B,
// Arduino pin 24 = PE6 = LED_G.
const int BLUE_LED_PIN = 23;
const int GREEN_LED_PIN = 24;
const bool LED_ACTIVE_LOW = false;

bool blueOn = false;
bool greenOn = false;
String inputLine;

void writeLed(int pin, bool on) {
  digitalWrite(pin, LED_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW));
}

void reportState() {
  Serial.print("STATE BLUE=");
  Serial.print(blueOn ? "ON" : "OFF");
  Serial.print(" GREEN=");
  Serial.println(greenOn ? "ON" : "OFF");
}

void setState(bool newBlueOn, bool newGreenOn) {
  blueOn = newBlueOn;
  greenOn = newGreenOn;
  writeLed(BLUE_LED_PIN, blueOn);
  writeLed(GREEN_LED_PIN, greenOn);
  reportState();
}

void blinkThreeTimes() {
  Serial.println("ACK BLINK_THREE");
  for (int count = 0; count < 3; count++) {
    blueOn = true;
    greenOn = true;
    writeLed(BLUE_LED_PIN, true);
    writeLed(GREEN_LED_PIN, true);
    delay(250);
    blueOn = false;
    greenOn = false;
    writeLed(BLUE_LED_PIN, false);
    writeLed(GREEN_LED_PIN, false);
    delay(250);
  }
  reportState();
}

void handleCommand(String command) {
  command.trim();
  command.toUpperCase();

  if (command == "LEFT_ON") {
    Serial.println("ACK LEFT_ON");
    setState(true, greenOn);
  } else if (command == "RIGHT_ON") {
    Serial.println("ACK RIGHT_ON");
    setState(blueOn, true);
  } else if (command == "LEFT_OFF") {
    Serial.println("ACK LEFT_OFF");
    setState(false, greenOn);
  } else if (command == "RIGHT_OFF") {
    Serial.println("ACK RIGHT_OFF");
    setState(blueOn, false);
  } else if (command == "ALL_OFF") {
    Serial.println("ACK ALL_OFF");
    setState(false, false);
  } else if (command == "BLINK_THREE") {
    blinkThreeTimes();
  } else if (command == "STATUS") {
    Serial.println("ACK STATUS");
    reportState();
  } else if (command.length() > 0) {
    Serial.println("ERROR UNKNOWN_COMMAND");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  inputLine.reserve(32);
  setState(false, false);
  Serial.println("READY AMEBA_MINI_LED");
  reportState();
}

void loop() {
  while (Serial.available() > 0) {
    char received = static_cast<char>(Serial.read());
    if (received == '\n' || received == '\r') {
      if (inputLine.length() > 0) {
        handleCommand(inputLine);
        inputLine = "";
      }
    } else if (inputLine.length() < 31) {
      inputLine += received;
    } else {
      inputLine = "";
      Serial.println("ERROR COMMAND_TOO_LONG");
    }
  }
}