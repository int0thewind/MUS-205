//==============================================================================
#include "MidiMessageLog.h"

MidiMessageLog::MidiMessageLog() :
    ListBox("MidiMessageLog", this) {
}

MidiMessageLog::~MidiMessageLog() {
}

//==============================================================================
/// ListBoxModel overrides.

int MidiMessageLog::getNumRows() {
    return this->messageLog.size();
}

void MidiMessageLog::paintListBoxItem(int row, Graphics& g, int width, int height, bool) {
    if (row > this->getNumRows()) { return; }
    g.setColour(Colours::white);
    g.setFont(15.0);
    Rectangle<int> textBound = Rectangle<int>(4, 0, width, height);
    g.drawText(this->messageLog[row], textBound, Justification::centredLeft);
}

//==============================================================================
/// AsyncUpdater override.

void MidiMessageLog::handleAsyncUpdate() {
    this->updateContent();
    this->scrollToEnsureRowIsOnscreen(this->getNumRows());
    this->repaint();
}

//==============================================================================
// MidiMessageLog methods.

void MidiMessageLog::addMidiMessage(const MidiMessage& message) {
    this->messageLog.add(this->convertMidiMessageToString(message));
//    this->messageLog.add(message.getDescription());
    this->triggerAsyncUpdate();
}

void MidiMessageLog::clear() {
    this->messageLog.clear();
    this->triggerAsyncUpdate();
}

const String MidiMessageLog::convertMidiMessageToString(const MidiMessage& msg) {
    std::stringstream ss;
    // Message is note on and not on drum track
    if (msg.isNoteOn() && msg.getChannel() != 10) {
        int noteNumber = msg.getNoteNumber();
        ss << "[" << msg.getTimeStamp() << "] Note On: channel=" << msg.getChannel()
        << ", key=" << noteNumber
        << ", velocity=" << ((int) msg.getVelocity())
        << ", pitch=" << keynumToPitch(noteNumber)
        << ", freq=" << keynumToFrequency(noteNumber);
    }
    // Message is note off and not on drum track
    else if (msg.isNoteOff() && msg.getChannel() != 10) {
        int noteNumber = msg.getNoteNumber();
        ss << "[" << msg.getTimeStamp() << "] Note Off: channel=" << msg.getChannel()
        << ", key=" << noteNumber
        << ", velocity=" << ((int) msg.getVelocity());
    }
    // Message is note on or off and on drum track
    else if (msg.isNoteOnOrOff() && msg.getChannel() == 10) {
        int noteNumber = msg.getNoteNumber();
        ss << "[" << msg.getTimeStamp() << "] " << (msg.isNoteOn() ? "Note On" : "Note Off")
        << ": channel=" << msg.getChannel()
        << ", rhythm instrument=" << msg.getRhythmInstrumentName(noteNumber)
        << ", key=" << noteNumber
        << ", velocity=" << ((int) msg.getVelocity());
    }
    // Message is program change
    else if (msg.isProgramChange()) {
        ss << "[" << msg.getTimeStamp() << "] Program Change: channel=" << msg.getChannel()
        << ", number=" << msg.getProgramChangeNumber();
    }
    // Message is pitch wheel
    else if (msg.isPitchWheel()) {
        ss << "[" << msg.getTimeStamp() << "] Pitch Wheel: channel=" << msg.getChannel()
        << ", value=" << msg.getPitchWheelValue();
    }
    // Message is control change
    else if (msg.isController()) {
        ss << "[" << msg.getTimeStamp() << "] Control Change: channel=" << msg.getChannel()
        << ", name=" << msg.getControllerName(msg.getControllerNumber())
        << ", value=" << msg.getControllerNumber();
    }
    // Message is after tough
    else if (msg.isAftertouch()) {
        ss << "[" << msg.getTimeStamp() << "] After Touch: channel=" << msg.getChannel()
        << ", value=" << msg.getAfterTouchValue();
    }
    // Message is channel pressure
    else if (msg.isChannelPressure()) {
        ss << "[" << msg.getTimeStamp() << "] Channel Pressure: channel=" << msg.getChannel()
        << ", value=" << msg.getChannelPressureValue();
    }
    else {
        ss << msg.getDescription();
    }
    return ss.str();
}

float MidiMessageLog::keynumToFrequency(int keynum) {
    return 440.0 * std::pow(2.0, (((float) keynum - 69.0) / 12.0));
}

String MidiMessageLog::keynumToPitch(int keynum) {
    const static StringArray keyOctaves = {"00", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    const static StringArray keyLetters = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    return keyLetters[keynum % 12] + keyOctaves[keynum / 12];
}

float MidiMessageLog::velocityToAmplitude(int velocity) {
    return (float) velocity / 127.0;
}

