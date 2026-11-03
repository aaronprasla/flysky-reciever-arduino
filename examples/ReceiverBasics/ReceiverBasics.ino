// FlyskyReceiverArduino example: print all 10 channels and blink the onboard
// LED (pin 13) whenever the receiver signal is lost.
//
// Wiring: connect the receiver's "IBUS" pin to the Arduino's RX1 pin.
//
// Open the Serial Monitor at 115200 baud to see the channel values.

#include <Receiver.h>

// One receiver object. It uses Serial1 at 115200 baud by default.
Receiver receiver;

void setup() {
  Serial.begin(115200);      // for printing to the Serial Monitor
  receiver.begin();          // start reading from the receiver
  pinMode(13, OUTPUT);       // onboard LED
}

void loop() {
  // Ask the library to read a packet if one is available.
  // Call this every time through loop(). It never blocks.
  receiver.update();

  // Print the channels once every 100 ms.
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 100) {
    lastPrint = millis();

    if (receiver.connected()) {
      // Channel values are 1000 (min) to 2000 (max), 1500 = center.
      // Channel 1 = roll, 2 = pitch, 3 = throttle, 4 = yaw,
      // 5-10 = pots / extra channels.
      for (int i = 1; i <= Receiver::CHANNEL_COUNT; i++) {
        Serial.print("Ch");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(receiver.channel(i));
        Serial.print("   ");
      }
      Serial.println();
    } else {
      Serial.println("SIGNAL LOST - all channels at safe value (1500)");
    }
  }

  // Blink the LED while the signal is lost.
  digitalWrite(13, receiver.connected() ? LOW : HIGH);
}
