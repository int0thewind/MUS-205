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

void MainApplication::closeAllAlertAndDialogWindows() {}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() = default;

const String MainApplication::getApplicationName() { return ProjectInfo::projectName; }

const String MainApplication::getApplicationVersion() { return ProjectInfo::versionString; }

bool MainApplication::moreThanOneInstanceAllowed() {
    return false;
}

void MainApplication::initialise(const String& commandLine) {
    this->mediaManager = std::make_unique<MediaManager>();
    this->mainWindow = std::make_unique<MainWindow>(this->getApplicationName());
}

void MainApplication::shutdown() { this->mainWindow = nullptr; }

void MainApplication::systemRequestedQuit() { quit(); }

void MainApplication::anotherInstanceStarted(const String& commandLine) {
}

MediaManager* MainApplication::getMediaManager() { return this->mediaManager.get(); }

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
