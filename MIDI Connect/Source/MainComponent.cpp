//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

MainContentComponent::MainContentComponent()
: audioManager (MainApplication::getApp().getAudioDeviceManager()) {  
}

MainContentComponent::~MainContentComponent() {
}

//==============================================================================
// Component overrides

void MainContentComponent::paint (Graphics& g) {
}

void MainContentComponent::resized() {
}

//==============================================================================
// Listener overrides

void MainContentComponent::buttonClicked(Button* button ) {
}

void MainContentComponent::showMidiMessage(const MidiMessage& message) {
}

void MainContentComponent::playMidiMessage(const MidiMessage& message) {
}

void MainContentComponent::handleNoteOn(MidiKeyboardState*, int chan, int note, float vel) {
}

void MainContentComponent::handleNoteOff(MidiKeyboardState*, int chan, int note, float vel) {
}

//==============================================================================
// MidiCallback overrides

// NOTE: this callback is not called by the main message thread so it should not
// modify any GUI components directly without locking the main message thread.
void MainContentComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage &message) {
}

//==============================================================================
// Internal Synth code
//

void MainContentComponent::loadSoundFont(juce::File& fontFile) {
}

void MainContentComponent::openAudioSettings() {
}
