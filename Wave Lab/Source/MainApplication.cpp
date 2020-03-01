//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

//==============================================================================
// MainApplication members

MainApplication& MainApplication::getApp() {
    auto* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
    assert(app != nullptr);
    return *app;
}

void MainApplication::closeAllAlertAndDialogWindows() {
  // delete any open alert or dialog windows.
}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() {
}

const String MainApplication::getApplicationName() {
    return ProjectInfo::projectName;
}

const String MainApplication::getApplicationVersion() {
    return ProjectInfo::versionString;
}

bool MainApplication::moreThanOneInstanceAllowed() {
    return false;
}

void MainApplication::initialise(const String& commandLine) {
    // initialize the audio device manager
    String errorString = this->audioDeviceManager.initialiseWithDefaultDevices(0, 2);
    // use jassert to ensure audioError is empty
    assert(errorString == "");
    // Create the application window.
    this->mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void MainApplication::shutdown() {
    // Delete our main window
    this->mainWindow = nullptr;
}

void MainApplication::systemRequestedQuit() {
    this->closeAllAlertAndDialogWindows();
    this->quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) {
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
