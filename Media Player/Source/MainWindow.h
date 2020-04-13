//==============================================================================
// @file MainWindow.h
// Defines the app's main window instance.

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

/// The main application window.
class MainWindow  : public DocumentWindow {
public:
  
    explicit MainWindow(const String& name);

    //============================================================================
    // DocumentWindow overrides

    void closeButtonPressed() override;
  
private:
    std::unique_ptr<MainComponent> mainComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
