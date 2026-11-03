// Receiver.cpp - beginner-friendly FlySky IBus receiver library.
#include "Receiver.h"

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

Receiver::Receiver()
  : port_( Serial1 ), ibus_( Serial1, STANDARD_IBUS_BAUD_BPS )
{
  resetToSafeValues();
}

Receiver::Receiver( HardwareSerial& port )
  : port_( port ), ibus_( port, STANDARD_IBUS_BAUD_BPS )
{
  resetToSafeValues();
}

//-----------------------------------------------------------------------------
// begin: start the serial bus and put all channels at their safe value.
//-----------------------------------------------------------------------------
bool Receiver::begin()
{
  ibus_.begin();
  resetToSafeValues();
  return true;
}

//-----------------------------------------------------------------------------
// update: parse one packet if a full 32-byte packet is already in the
// serial FIFO. Never blocks - safe to call every time through loop().
//-----------------------------------------------------------------------------
void Receiver::update()
{
  if( port_.available() >= PACKET_LENGTH ) {
    if( ibus_.update_buffer() == SUCCESS ) {
      for( int i = 0; i < CHANNEL_COUNT; i++ ) {
        // Internal engine index 0 is the packet header, so channel n
        // lives at index n. (This off-by-one is easy to get wrong!)
        channels_[ i ] = constrain( ibus_.get_eng_unit_item( i + 1 ),
                                    MIN_VALUE, MAX_VALUE );
      }
      lastPacketMs_ = millis();
    }
  }
}

//-----------------------------------------------------------------------------
// channel: value of channel n (1..10), or the safe value 1500 if the
// number is out of range or the signal is lost.
//-----------------------------------------------------------------------------
int Receiver::channel( int n )
{
  if( n < 1 || n > CHANNEL_COUNT ) {
    return CENTER_VALUE;
  }
  if( !connected() ) {
    return CENTER_VALUE;
  }
  return channels_[ n - 1 ];
}

//-----------------------------------------------------------------------------
// connected: true if a valid packet arrived within SIGNAL_TIMEOUT_MS.
//-----------------------------------------------------------------------------
bool Receiver::connected()
{
  return ( millis() - lastPacketMs_ ) < SIGNAL_TIMEOUT_MS;
}

//-----------------------------------------------------------------------------
// millisSincePacket: time since the last valid packet.
//-----------------------------------------------------------------------------
unsigned long Receiver::millisSincePacket()
{
  return millis() - lastPacketMs_;
}

//-----------------------------------------------------------------------------
// resetToSafeValues: all channels to 1500, no packet recorded yet.
//-----------------------------------------------------------------------------
void Receiver::resetToSafeValues()
{
  for( int i = 0; i < CHANNEL_COUNT; i++ ) {
    channels_[ i ] = CENTER_VALUE;
  }
  lastPacketMs_ = 0;
}
