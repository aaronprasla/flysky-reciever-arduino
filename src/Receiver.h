// Receiver.h - beginner-friendly FlySky IBus receiver library.
//
// Typical usage:
//
//   #include <Receiver.h>
//
//   Receiver receiver;        // uses Serial1 at 115200 baud by default
//
//   void setup() {
//     receiver.begin();
//   }
//
//   void loop() {
//     receiver.update();                  // call this every loop
//     int roll = receiver.channel(1);     // 1000 = left, 1500 = center, 2000 = right
//     if (receiver.connected()) {
//       // a valid receiver packet arrived within the last 100 ms
//     }
//   }
//
// When the signal is lost, every channel snaps to the safe value 1500.

#ifndef FLYSKY_RECEIVER_H_
#define FLYSKY_RECEIVER_H_

#include <Arduino.h>
#include "IBus.h"

class Receiver {

 public:
  // How many channels this library exposes.
  // The FlySky receiver transmits 14 channels; only the first 10
  // are exposed here. Change this constant if you need more later.
  static const int CHANNEL_COUNT = 10;

  // Channel value range. 1000 = minimum, 1500 = center, 2000 = maximum.
  static const int MIN_VALUE    = 1000;
  static const int CENTER_VALUE = 1500;
  static const int MAX_VALUE    = 2000;

  // Creates a receiver that reads from Serial1 at 115200 baud
  // (the standard FlySky IBus configuration).
  Receiver();

  // Advanced: create a receiver on a different hardware serial port.
  explicit Receiver( HardwareSerial& port );

  // Starts the serial bus. Call once in setup(). Returns true on success.
  bool begin();

  // Reads a receiver packet if one is fully available.
  // Call this as often as you like in loop(). It never blocks:
  // if no complete packet is in the buffer, it returns immediately.
  void update();

  // Returns the value of channel n (1 = roll ... 10 = ch10).
  // The value is between MIN_VALUE (1000) and MAX_VALUE (2000).
  // Returns CENTER_VALUE (1500) if the number is out of range
  // or the signal is currently lost.
  int channel( int n );

  // Returns true if a valid packet was received within the last 100 ms.
  bool connected();

  // Milliseconds since the last valid packet
  // (the whole uptime if no packet has arrived yet).
  unsigned long millisSincePacket();

 private:

  HardwareSerial& port_;
  rc_ibus_c       ibus_;

  int channels_[ CHANNEL_COUNT ];   // last good value for each channel
  unsigned long lastPacketMs_;      // millis() of last valid packet

  // A signal older than this is considered lost.
  static const unsigned long SIGNAL_TIMEOUT_MS = 100;

  void resetToSafeValues();
};

#endif  // FLYSKY_RECEIVER_H_
