//==============================================================================
#include "MidiMessageLog.h"

MidiMessageLog::MidiMessageLog() {
}

MidiMessageLog::~MidiMessageLog() {
}

//==============================================================================
/// ListBoxModel overrides.

int MidiMessageLog::getNumRows() {  
  return 0;
}

void MidiMessageLog::paintListBoxItem (int row, Graphics& g, int width, int height, bool)  {
}

//==============================================================================
/// AsyncUpdater override.

void MidiMessageLog::handleAsyncUpdate() {
}

//==============================================================================
// MidiMessageLog methods.

void MidiMessageLog::addMidiMessage (const MidiMessage& message) {
}

void MidiMessageLog::clear() {
}

const String MidiMessageLog::midiMessageString(const MidiMessage& msg) {
  return "";
}

float MidiMessageLog::keynumToFrequency(int keynum) {
  return 0;
}

String MidiMessageLog::keynumToPitch(int keynum) {
  return "";
}

float MidiMessageLog::velocityToAmplitude(int velocity) {
  return 0.0;
}

