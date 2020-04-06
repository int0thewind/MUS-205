#include "Transport.h"

/// Constructor.
Transport::Transport(const MediaManagerData& mmd) {
}

/// Destructor.
Transport::~Transport() {
}

void Transport::paint(Graphics& gr) {

//  // handy for debugging!
//  gr.setColour(Colours::red);
//  gr.drawRect(playPauseButton.getBounds());
//  gr.drawRect(goToStartButton.getBounds());
//  gr.drawRect(gainButton.getBounds());
//  gr.drawRect(tempoSlider.getBounds());
//  gr.drawRect(gainSlider.getBounds());
}

void Transport::resized() {
}

//============================================================================
// JUCE Component Callbacks

void Transport::buttonClicked(juce::Button* button) {
}

void Transport::sliderValueChanged(juce::Slider* slider) {
}

//============================================================================
// JUCE ValueTree callbacks (listening to the managerData changes)

void Transport::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
}

//============================================================================
// Icon Button Drawing

void Transport::drawPlayButton(juce::DrawableButton& button) {
  juce::DrawablePath triangleDrawable, rectangleDrawable;
  juce::Path trianglePath, rectanglePath;

  button.setImages(&triangleDrawable, nullptr, nullptr, nullptr, &rectangleDrawable);
}

void Transport::drawGoToStartButton(juce::DrawableButton& b) {
  // Juce path drawing done in percentage (100x100)
  juce::DrawablePath image;
  juce::Path path;

  b.setImages(&image);
}

void Transport::drawGainButton(juce::DrawableButton& button, double gain) {
  // Juce path drawing done in percentage (100x100)
  juce::DrawablePath drawable;
  juce::Path p;
  
  // speaker rect from 0 to 30
  p.addRectangle(0, 30, 30, 35);
  // speaker cone from 0 to 45
  p.addTriangle(0, 50, 40, 0, 40, 100);
  // waves start at x=55 spaced 15 apart
  if (gain > 0.1)
    p.addCentredArc(55, 50, 6, 20,  0, 0, 3.14159f, true);
  if (gain > 0.4)
    p.addCentredArc(70, 50, 5, 35,  0, 0, 3.14159f, true);
  if (gain > 0.7)
    p.addCentredArc(85, 50, 5, 50,  0, 0, 3.14159f, true);
  // this makes button image width 100 no matter how many arcs added
  p.startNewSubPath(100, 0);
  drawable.setPath(p);
  drawable.setFill(iconColor);
  button.setImages(&drawable);
}

void Transport::drawCurrentTimeLabel() {
}

void Transport::drawEndTimeLabel() {
}

const juce::String Transport::toFormattedTimeString(const double seconds) {
  return "";
}
