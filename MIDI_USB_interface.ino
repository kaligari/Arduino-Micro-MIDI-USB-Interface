#include <USB-MIDI.h>
USING_NAMESPACE_MIDI;

typedef USBMIDI_NAMESPACE::usbMidiTransport __umt;
typedef MIDI_NAMESPACE::MidiInterface<__umt> __ss;
__umt usbMIDI(0); // cableNr
__ss MIDICoreUSB((__umt&)usbMIDI);

typedef Message<MIDI_NAMESPACE::DefaultSettings::SysExMaxSize> MidiMessage;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial);

void setup()
{
  MIDICoreUSB.begin(MIDI_CHANNEL_OMNI);
  MIDICoreSerial.begin(MIDI_CHANNEL_OMNI);

  MIDICoreUSB.setHandleSystemExclusive(OnUSBSysEx);
  MIDICoreSerial.setHandleSystemExclusive(OnSerialSysEx);
}

void loop()
{
  if(MIDICoreUSB.read()){
    MIDICoreSerial.send(MIDICoreUSB.getType(), MIDICoreUSB.getData1(), MIDICoreUSB.getData2(), MIDICoreUSB.getChannel());
  } else if(MIDICoreSerial.read()){
    MIDICoreUSB.send(MIDICoreSerial.getType(), MIDICoreSerial.getData1(), MIDICoreSerial.getData2(), MIDICoreSerial.getChannel());
  }
}

void OnUSBSysEx(byte* data, unsigned length) {
  MIDICoreSerial.sendSysEx(length, data, true);
}

void OnSerialSysEx(byte* data, unsigned length) {
  MIDICoreUSB.sendSysEx(length, data, true);
}
