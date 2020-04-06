//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

//==============================================================================
// Component overrides

MainComponent::MainComponent() : midiOutputMenu(*this) {
}

MainComponent::~MainComponent() {
}

void MainComponent::paint (Graphics& gr) {
}

void MainComponent::resized() {
}

//==============================================================================
// JUCE Listener overrides

void MainComponent::buttonClicked(Button* button) {
}

void MainComponent::comboBoxChanged(ComboBox* menu) {
}

//==============================================================================
// ValueTree::Listener overrides

void MainComponent::valueTreePropertyChanged(juce::ValueTree& tree,
					     const juce::Identifier& ident) {
}
