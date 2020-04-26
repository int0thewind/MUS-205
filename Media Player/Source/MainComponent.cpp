//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

//==============================================================================
// Component overrides

MainComponent::MainComponent() {
    // media manager data
    this->mediaManagerData = MainApplication::getApp().getMediaManager()->getManagerData();
    this->mediaManagerData.addListener(this);

    // set up open button
    this->openButton.addListener(this);
    this->openButton.setButtonText("Open...");
    this->addAndMakeVisible(openButton);

    // set up info button
    this->infoButton.addListener(this);
    this->infoButton.setButtonText("Media Info...");
    this->infoButton.setEnabled(false);
    this->addAndMakeVisible(infoButton);

    // midiOutputMenu
    this->midiOutputMenu.addListener(this);
    this->midiOutputMenu.setTextWhenNothingSelected("Select Midi Output");
    this->addAndMakeVisible(midiOutputMenu);

    // transport
    this->transport = std::make_unique<Transport>(this->mediaManagerData);
    this->transport->setEnabled(false);
    this->addAndMakeVisible(transport.get());

    this->setSize(392, 120);
}

MainComponent::~MainComponent() = default;

void MainComponent::paint(Graphics& gr) {
    gr.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
    Rectangle<int> globalBound = this->getLocalBounds();
    globalBound.removeFromTop(8);
    globalBound.removeFromBottom(8);
    globalBound.removeFromLeft(8);
    globalBound.removeFromRight(8);

    Rectangle<int> firstRow = globalBound.removeFromTop(24);
    globalBound.removeFromTop(8);
    Rectangle<int> secondRow = globalBound;

    this->openButton.setBounds(firstRow.removeFromLeft(90));
    this->infoButton.setBounds(firstRow.removeFromRight(90));
    int firstRowRemoveAmount = (firstRow.getWidth() - 180) / 2;
    firstRow.removeFromLeft(firstRowRemoveAmount);
    firstRow.removeFromRight(firstRowRemoveAmount);
    this->midiOutputMenu.setBounds(firstRow);

    int amountToRemove = (secondRow.getWidth() - transport->getWidth()) / 2;
    secondRow.removeFromLeft(amountToRemove);
    secondRow.removeFromRight(amountToRemove);
    this->transport->setBounds(secondRow);
}

//==============================================================================
// JUCE Listener overrides

void MainComponent::buttonClicked(Button* button) {
    if (button == &openButton) {
        this->mediaManagerData.launchOpenMediaDialog();
    } else if (button == &infoButton) {
        this->mediaManagerData.launchMediaInfoDialog();
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &midiOutputMenu) {
        // We don't need to subtract one here
        // We are sending the raw midi device id to the value tree.
        this->mediaManagerData.setMidiOutputOpenID(menu->getSelectedId());
    }
}

//==============================================================================
// ValueTree::Listener overrides

void MainComponent::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    int mediaType = mediaManagerData.getLoadedMediaType();
    if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        bool isPaused = !this->mediaManagerData.getTransportPlaying();
        this->openButton.setEnabled(isPaused);
        if (mediaType == MediaManagerData::MEDIA_MIDI) {
            this->midiOutputMenu.setEnabled(isPaused);
        }
    } else if (ident == MediaManagerData::LOADED_MEDIA_TYPE) {
        if (mediaType == MediaManagerData::MEDIA_AUDIO) {
            this->infoButton.setEnabled(true);
            this->midiOutputMenu.setEnabled(false);
            this->mediaManagerData.setTransportEnabled(true);
            this->mediaManagerData.setTransportTempoEnabled(false);
        } else if (mediaType == MediaManagerData::MEDIA_MIDI) {
            bool isMidiOpened = (bool) this->mediaManagerData.getMidiOutputOpenID();
            this->infoButton.setEnabled(true);
            this->midiOutputMenu.setEnabled(true);
            this->mediaManagerData.setTransportEnabled(isMidiOpened);
            this->mediaManagerData.setTransportTempoEnabled(isMidiOpened);
        } else if (mediaType == MediaManagerData::MEDIA_NONE) {
            this->infoButton.setEnabled(false);
            this->midiOutputMenu.setEnabled(false);
            this->mediaManagerData.setTransportEnabled(false);
            this->mediaManagerData.setTransportTempoEnabled(false);
        }
    } else if (ident == MediaManagerData::MIDI_OUTPUT_OPEN_ID) {
        int midiOpen = this->mediaManagerData.getMidiOutputOpenID();
        if (mediaType == MediaManagerData::MEDIA_MIDI) {
            this->mediaManagerData.setTransportEnabled(midiOpen);
            this->mediaManagerData.setTransportTempoEnabled(midiOpen);
        }
    }
}
