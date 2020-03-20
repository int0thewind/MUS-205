//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

MainContentComponent::MainContentComponent()
: audioManager (MainApplication::getApp().getAudioDeviceManager()) {
    // Set up settingsButton
    this->settingsButton.setButtonText("Audio Settings ...");
    this->settingsButton.addListener(this);
    this->addAndMakeVisible(this->settingsButton);

    // Set up messageLogButton
    this->messageLogButton.setButtonText("Message Log");
    this->messageLogButton.setRadioGroupId(GROUP_ID, NotificationType::dontSendNotification);
    this->messageLogButton.setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
    this->messageLogButton.addListener(this);
    this->addAndMakeVisible(this->messageLogButton);

    // Set up pianoRollButton
    this->pianoRollButton.setButtonText("Piano Roll");
    this->pianoRollButton.setRadioGroupId(GROUP_ID, NotificationType::dontSendNotification);
    this->pianoRollButton.setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
    this->pianoRollButton.addListener(this);
    this->addAndMakeVisible(this->pianoRollButton);

    // Set up clearButton
    this->clearButton.setButtonText("Clear");
    this->clearButton.addListener(this);
    this->addAndMakeVisible(this->clearButton);

    // Midi
    
}

MainContentComponent::~MainContentComponent() {
    this->keyboardState.removeListener(this);
    this->audioManager.removeMidiInputCallback("", this);
    this->sfZeroPlayer.setProcessor(nullptr);
}

//==============================================================================
// Component overrides

void MainContentComponent::paint (Graphics& g) {
    g.fillAll(this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainContentComponent::resized() {
}

//==============================================================================
// Listener overrides

void MainContentComponent::buttonClicked(Button* button) {
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
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettingsPanel =
            std::make_unique<juce::AudioDeviceSelectorComponent>(this->audioManager,
                    0, 2, 0, 2,
                    true, false,
                    true, false);
    audioSettingsPanel->setSize(500, 270);
    juce::DialogWindow::LaunchOptions dialogWindow;
    dialogWindow.useNativeTitleBar = true;
    dialogWindow.resizable = false;
    dialogWindow.dialogTitle = "Audio Settings";
    dialogWindow.dialogBackgroundColour = Colours::black;
    dialogWindow.content.setOwned(audioSettingsPanel.release());
    dialogWindow.launchAsync();
}
