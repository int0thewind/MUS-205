//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

//==============================================================================
// MainApplication members

MainApplication& MainApplication::getApp() {
  MainApplication* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
  assert(app != nullptr);
  return *app;
}

const File MainApplication::getRuntimeResourceDirectory() {
  return File();
}

AudioDeviceManager& MainApplication::getAudioDeviceManager() {
  return audioDeviceManager;
}

void MainApplication::closeAllAlertAndDialogWindows() {
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
}

void MainApplication::shutdown() {}

void MainApplication::systemRequestedQuit() {
  quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) {
  // When another instance of the app is launched while this one is running,
  // this method is invoked, and the commandLine parameter tells you what
  // the other instance's command-line arguments were.
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
