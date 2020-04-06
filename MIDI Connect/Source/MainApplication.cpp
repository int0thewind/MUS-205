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

const File MainApplication::getRuntimeResourceDirectory() {
#if JUCE_MAC
    return File::getSpecialLocation(juce::File::currentApplicationFile).getChildFile("Contents/Resources");
#endif
#if JUCE_WINDOWS
    return File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getChildFile("Resources");
#endif
#if JUCE_IOS
    return File::getSpecialLocation(juce::File::currentApplicationFile);
#endif
#if JUCE_LINUX
    return File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getChildFile("Resources");
#endif
    // If you hit this assert you need to add the application's resource directory on your OS.
    jassert(false);
}

AudioDeviceManager& MainApplication::getAudioDeviceManager() { return this->audioDeviceManager; }

void MainApplication::closeAllAlertAndDialogWindows() {}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() = default;

const String MainApplication::getApplicationName() { return ProjectInfo::projectName; }

const String MainApplication::getApplicationVersion() { return ProjectInfo::versionString; }

bool MainApplication::moreThanOneInstanceAllowed() { return false; }

void MainApplication::initialise(const String& commandLine) {
    String errorString = this->audioDeviceManager.initialiseWithDefaultDevices(
            0, 2);
    assert(errorString == "");
    this->mainWindow = std::make_unique<MainWindow>(this->getApplicationName());
}

void MainApplication::shutdown() { this->mainWindow = nullptr; }

void MainApplication::systemRequestedQuit() { quit(); }

void MainApplication::anotherInstanceStarted(const String& commandLine) {
  // When another instance of the app is launched while this one is running,
  // this method is invoked, and the commandLine parameter tells you what
  // the other instance's command-line arguments were.
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
