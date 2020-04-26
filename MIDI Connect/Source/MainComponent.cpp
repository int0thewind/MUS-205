//==============================================================================

#include "MainComponent.h"

#include <memory>
#include "MainApplication.h"

MainContentComponent::MainContentComponent()
: audioManager (MainApplication::getApp().getAudioDeviceManager()) {
    // Set up settingsButton
    settingsButton.setButtonText("Audio Settings...");
    settingsButton.addListener(this);
    this->addAndMakeVisible(settingsButton);

    // Set up messageLogButton
    messageLogButton.setButtonText("Message Log");
    messageLogButton.setRadioGroupId(GROUP_ID);
    messageLogButton.setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnRight);
    messageLogButton.addListener(this);
    this->addAndMakeVisible(messageLogButton);

    // Set up pianoRollButton
    pianoRollButton.setButtonText("Piano Roll");
    pianoRollButton.setRadioGroupId(GROUP_ID);
    pianoRollButton.setConnectedEdges(Button::ConnectedEdgeFlags::ConnectedOnLeft);
    pianoRollButton.addListener(this);
    this->addAndMakeVisible(pianoRollButton);
    
    messageLogButton.setToggleState(true, NotificationType::dontSendNotification);

    // Set up clearButton
    clearButton.setButtonText("Clear");
    clearButton.addListener(this);
    this->addAndMakeVisible(clearButton);

    // midi message log
    // Set it to be visible by default
    this->addChildComponent(midiMessageLog);
    midiMessageLog.setVisible(true);
    // midi piano roll
    // Make it visible but do not add
//    this->addChildComponent(midiPianoRoll);
    midiPianoRoll.setVisible(true);

    midiKeyboardState.addListener(this);

    // Midi keyboard state and component
    midiKeyboardComponent = std::make_unique<MidiKeyboardComponent>(
            this->midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard);
    midiKeyboardComponent->setOctaveForMiddleC(4);
    this->addAndMakeVisible(this->midiKeyboardComponent.get());

    sfZeroAudioProcessor = std::make_unique<sfzero::SFZeroAudioProcessor>();
    sfZeroPlayer.setProcessor(sfZeroAudioProcessor.get());
    audioManager.addAudioCallback(&this->sfZeroPlayer);
    audioManager.addMidiInputCallback("", this);

#ifdef JUCE_MAC
    auto soundFont = MainApplication::getApp().getRuntimeResourceDirectory()
            .getChildFile("G800-A112-Piano1d-2-3f.sfz");
#endif
#ifdef JUCE_LINUX
    auto soundFont = MainApplication::getApp().getRuntimeResourceDirectory()
                    .getChildFile("G800-A112-Piano1d-2-3f.sfz");
#endif
    this->loadSoundFont(soundFont);
    
    // Set the size at last since we have a pointer components that are not initialised.
    this->setSize(608, 412);
}

MainContentComponent::~MainContentComponent() {
    midiKeyboardState.removeListener(this);
    audioManager.removeMidiInputCallback("", this);
    sfZeroPlayer.setProcessor(nullptr);
}

//==============================================================================
// Component overrides

void MainContentComponent::paint(Graphics& g) {
    g.fillAll(this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainContentComponent::resized() {
    Rectangle<int> globalBound = getLocalBounds();
    globalBound.removeFromTop(8);
    globalBound.removeFromBottom(8);
    globalBound.removeFromLeft(8);
    globalBound.removeFromRight(8);
    int middlePoint = globalBound.getWidth() / 2;

    Rectangle<int> firstLine = globalBound.removeFromTop(24);
    settingsButton.setBounds(firstLine.removeFromLeft(120));
    clearButton.setBounds(firstLine.removeFromRight(60));
    firstLine.removeFromLeft(middlePoint - 120 - 90);
    firstLine.removeFromRight(middlePoint - 60 - 90);
    messageLogButton.setBounds(firstLine.removeFromLeft(90));
    pianoRollButton.setBounds(firstLine);

    globalBound.removeFromTop(8);
    midiKeyboardComponent->setBounds(globalBound.removeFromTop(64));
    globalBound.removeFromTop(8);
    midiMessageLog.setBounds(globalBound);
    midiPianoRoll.setBounds(globalBound);
}

//==============================================================================
// Listener overrides

void MainContentComponent::buttonClicked(Button* button) {
    if (button == &this->settingsButton) {
        this->openAudioSettings();
    } else if (button == &this->messageLogButton) {
        if (!messageLogButton.getToggleState()) {
            this->messageLogButton.setToggleState(true, NotificationType::dontSendNotification);
            this->midiMessageLog.clear();
            this->midiKeyboardState.reset();
            this->addChildComponent(midiMessageLog);
            this->removeChildComponent(&midiPianoRoll);
            this->repaint();
        }
    } else if (button == &this->pianoRollButton) {
        if (!pianoRollButton.getToggleState()) {
            this->pianoRollButton.setToggleState(true, NotificationType::dontSendNotification);
            this->midiPianoRoll.clear();
            this->midiKeyboardState.reset();
            this->addChildComponent(midiPianoRoll);
            this->removeChildComponent(&midiMessageLog);
            this->repaint();
        }
    } else if (button == &this->clearButton) {
        if (this->messageLogButton.getToggleState()) {
            this->midiMessageLog.clear();
        } else if (this->pianoRollButton.getToggleState()) {
            this->midiPianoRoll.clear();
        }
    } else {
        std::cerr << "ERROR! No button has been clicked, but the main component received a callback request!\n"
                  << "The name of the button is " << button->getName() << "\n"
                  << "The ID of the button is " << button->getComponentID() << std::endl;
    }
}

void MainContentComponent::showMidiMessage(const MidiMessage& message) {
    midiMessageLog.addMidiMessage(message);
    midiPianoRoll.addMidiMessage(message);
}

void MainContentComponent::playMidiMessage(const MidiMessage& message) {
    sfZeroPlayer.getMidiMessageCollector().addMessageToQueue(message);
}

void MainContentComponent::handleNoteOn(MidiKeyboardState*, int chan, int note, float vel) {
    MidiMessage newMidiMessage = MidiMessage::noteOn(chan, note, vel);
    newMidiMessage.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    playMidiMessage(newMidiMessage);
    showMidiMessage(newMidiMessage);
}

void MainContentComponent::handleNoteOff(MidiKeyboardState*, int chan, int note, float vel) {
    MidiMessage newMidiMessage = MidiMessage::noteOff(chan, note, vel);
    newMidiMessage.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
    playMidiMessage(newMidiMessage);
    showMidiMessage(newMidiMessage);
}

//==============================================================================
// MidiCallback overrides

// NOTE: this callback is not called by the main message thread so it should not
// modify any GUI components directly without locking the main message thread.
void MainContentComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage &message) {
    if (this->quitting == true || message.isActiveSense()) { return; }
    auto mainComponentSafePointer = SafePointer<MainContentComponent>(this);
    std::function<void()> messageProcessor = [=]() {
        if (mainComponentSafePointer != nullptr) {
            if (message.isNoteOnOrOff()) {
                mainComponentSafePointer.getComponent()->midiKeyboardState.processNextMidiEvent(message);
            } else {
                mainComponentSafePointer.getComponent()->showMidiMessage(message);
                mainComponentSafePointer.getComponent()->playMidiMessage(message);
            }
        }
    };
    
//    const MessageManagerLock mmLock;
    juce::MessageManager::callAsync(messageProcessor);
}

//==============================================================================
// Internal Synth code
//

void MainContentComponent::loadSoundFont(juce::File& fontFile) {
    jassert(fontFile.existsAsFile());
    sfZeroAudioProcessor->setSfzFileThreaded(&fontFile);
    std::cout << "Sound file loaded from " << fontFile.getFullPathName() << std::endl;
}

void MainContentComponent::openAudioSettings() {
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettingsPanel = std::make_unique<juce::AudioDeviceSelectorComponent>(this->audioManager, 0, 2, 0, 2, true, true, true, true);
    audioSettingsPanel->setSize(500, 270);
    juce::DialogWindow::LaunchOptions dialogWindow;
    dialogWindow.useNativeTitleBar = true;
    dialogWindow.resizable = false;
    dialogWindow.dialogTitle = "Audio Settings";
    dialogWindow.dialogBackgroundColour = this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    // TODO: why we must do release() instead of get()?
    dialogWindow.content.setOwned(audioSettingsPanel.release());
    dialogWindow.launchAsync();
}
