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

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() = default;

const String MainApplication::getApplicationName() { return ProjectInfo::projectName; }

const String MainApplication::getApplicationVersion() { return ProjectInfo::versionString; }

bool MainApplication::moreThanOneInstanceAllowed() { return false; }

void MainApplication::initialise(const String& commandLine) {
    this->mediaManager = std::make_unique<MediaManager>();
    this->mainWindow = std::make_unique<MainWindow>(this->getApplicationName());
}

void MainApplication::shutdown() { this->mainWindow = nullptr; }

void MainApplication::systemRequestedQuit() {
    MainApplication::closeAllWindows();
    MainApplication::quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) { }

MediaManager* MainApplication::getMediaManager() { return this->mediaManager.get(); }

void MainApplication::closeAllWindows() {
    std::vector<std::unique_ptr<AlertWindow>> alertWindows;
    std::vector<std::unique_ptr<DialogWindow>> dialogWindows;
    Desktop& desktop = Desktop::getInstance();
    for (int i = 0; i < desktop.getNumComponents(); i++) {
        if (auto alertWindow = dynamic_cast<AlertWindow*> (desktop.getComponent(i))) {
            alertWindows.push_back(std::unique_ptr<AlertWindow>(alertWindow));
        } else if (auto dialogWindow = dynamic_cast<DialogWindow*>(desktop.getComponent(i))) {
            dialogWindows.push_back(std::unique_ptr<DialogWindow>(dialogWindow));
        }
    }
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
