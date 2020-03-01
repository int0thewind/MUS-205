//==============================================================================
// MainWindow.cpp
// This file defines the class representing our app's main window instance.
//==============================================================================

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

/// This class implements the applicatoin window that contains an
/// instance of our MainContentComponent class.

class MainWindow  : public DocumentWindow
{
public:
  
    /// Constructor.  Your method should perform the following actions:
    /// * Set the window to use the native title bar for whatever OS the app is on.
    /// * Create the main component and assign it to be the main content component
    ///   of the window. The window should NOT conform to the main component when
    ///   the content component changes size. See: ResizableWindow::setContentOwned().
    /// * The window should be resizable and should use a bottom right corner sizer.
    /// * The resize limits of the window should be a minimum 600 width and 400
    ///   height and double those values as the maximum.
    /// * The window should appear in the center of the screen with its minimum
    ///   width and height.
    /// * Set its visibility to true.
    explicit MainWindow (const String& name);

    //==============================================================================
    // DocumentWindow overrides

    /// This is called when the user tries to close this window. Here,
    /// we'll just ask the app to quit when this happens, but you can
    /// change this to do whatever you need.
    void closeButtonPressed() override;

private:

    /**
     * The MainComponent instance that the window holds.
     */
    std::unique_ptr<MainComponent> mainComponent;

    /**
     * The minimum width of the main window
     */
    const int MIN_WIDTH = 600;

    /**
     * The minimum height of the main window
     */
    const int MIN_HEIGHT = 400;

    /**
     * The maximum height and width is 2 times mininum width and height
     */
    const int FACTOR = 2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
