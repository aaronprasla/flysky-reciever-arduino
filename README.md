# FlyskyReceiverArduino

A beginner-friendly Arduino library for reading channel values from a
FlySky IBus RC receiver (e.g. the FlySky FS-IA10B).

## Quick start

1. Connect the receiver's **IBUS** output pin to the Arduino's **RX1** pin
   (both share the same ground).
2. Add a `Receiver` object, start it in `setup()`, and call `update()` in
   `loop()`:

```cpp
#include <Receiver.h>

Receiver receiver;          // uses Serial1 at 115200 baud

void setup() {
  receiver.begin();
}

void loop() {
  receiver.update();                    // read a packet if one is ready
  int roll = receiver.channel(1);       // 1000 = left, 1500 = center, 2000 = right

  if (receiver.connected()) {
    // signal is good
  }
}
```

See `examples/ReceiverBasics` for a complete sketch that prints all 10
channels and blinks the onboard LED when the signal is lost.

## The API

| Function | What it does |
|---|---|
| `begin()` | Starts the serial bus. Call once in `setup()`. |
| `update()` | Reads a packet if one is fully available. Call in `loop()`. **Never blocks.** |
| `channel(n)` | Value of channel `n` (1–10), clamped to 1000–2000. |
| `connected()` | `true` if a valid packet arrived within the last 100 ms. |
| `millisSincePacket()` | Milliseconds since the last valid packet. |

Constants:

| Constant | Value | Meaning |
|---|---|---|
| `Receiver::CHANNEL_COUNT` | 10 | Number of exposed channels |
| `Receiver::MIN_VALUE` | 1000 | Minimum channel value |
| `Receiver::CENTER_VALUE` | 1500 | Center / neutral value |
| `Receiver::MAX_VALUE` | 2000 | Maximum channel value |

## Channels

| Channel | Typical control |
|---|---|
| 1 | Roll |
| 2 | Pitch |
| 3 | Throttle |
| 4 | Yaw |
| 5–10 | Pot / extra channels |

Channel numbers are **1-based**, matching the labels on your transmitter.
(Many IBus libraries are 0-based where channel 0 is the packet header —
this library hides that for you.)

## Signal loss

When no valid packet has arrived for 100 ms, `connected()` returns
`false` and **every channel snaps to the safe value 1500** (neutral).
This means a disconnected transmitter can never leave your model
stuck at its last throttle or stick position.

## Troubleshooting

- **No signal (`connected()` is always false):** check the wiring
  (receiver IBUS → Arduino RX1, common ground), and make sure the
  receiver is set to IBus mode at 115200 baud.
- **Channel values stuck at 1500:** the signal is lost — see above.
- **Weird values on "channel 1":** make sure you're using
  `channel(1)` for roll. If you copied code from an older 0-based
  library, `channel(0)` is always 1500 in this library (out of range).
- **Want more than 10 channels?** The receiver transmits 14; change
  `CHANNEL_COUNT` in `Receiver.h` and expose more channels as needed.

## Advanced / internal

`src/IBus.h` / `src/IBus.cpp` contain the low-level IBus protocol engine
(`rc_ibus_c`). Regular users never need to touch it — it exists so
`Receiver` can decode packets.
