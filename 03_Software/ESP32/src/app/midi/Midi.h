
#ifndef CODE_HARDWAREV2_SRC_APP_MIDI_MIDI_H_
#define CODE_HARDWAREV2_SRC_APP_MIDI_MIDI_H_


void Midi_init();

void Midi_sendCmd(int len, const char * data);

void Midi_sendD1CmdOn();

void Midi_sendD2CmdOn();

void Midi_sendD3CmdOn();

void Midi_sendD1CmdOff();

void Midi_sendD2CmdOff();

void Midi_sendD3CmdOff();

#endif /* CODE_HARDWAREV2_SRC_APP_MIDI_MIDI_H_ */
