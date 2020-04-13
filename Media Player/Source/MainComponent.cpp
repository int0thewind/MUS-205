//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

//==============================================================================
// Component overrides

MainComponent::MainComponent() {
    // set up open button
    openButton.addListener(this);
    openButton.setButtonText("Open...");
    this->addAndMakeVisible(openButton);

    // set up info button
    infoButton.addListener(this);
    infoButton.setButtonText("Media Info...");
    infoButton.setEnabled(false);
    this->addAndMakeVisible(infoButton);

    // midiOutputMenu
    midiOutputMenu.addListener(this);
    midiOutputMenu.setTextWhenNothingSelected("Select Midi Output");
    this->addAndMakeVisible(midiOutputMenu);

    // media manager data
    mediaManagerData.addListener(this);

    // transport
    transport = std::make_unique<Transport>(this->mediaManagerData);
    transport->setEnabled(false);
    // TODO add this component as a listener?
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

//    int amountToRemove = (secondRow.getWidth() - transport->getWidth()) / 2;
//    secondRow.removeFromLeft(amountToRemove);
//    secondRow.removeFromRight(amountToRemove);
//    this->transport->setBounds(secondRow);
}

//==============================================================================
// JUCE Listener overrides

void MainComponent::buttonClicked(Button* button) {
    if (button == &openButton) {

    } else if (button == &infoButton) {

    } else {
        std::cerr << "WARNING! Nothing has been clicked but the function is invoked!" << std::endl;
    }
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
    if (menu == &midiOutputMenu) {

    } else {
        std::cerr << "WARNING! Nothing has been selected but the function is invoked!" << std::endl;
    }
}

//==============================================================================
// ValueTree::Listener overrides

void MainComponent::valueTreePropertyChanged(juce::ValueTree& tree,
					     const juce::Identifier& ident) {
}
