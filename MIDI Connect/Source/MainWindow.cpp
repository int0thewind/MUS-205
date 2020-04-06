//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    this->setUsingNativeTitleBar(true);
    this->setResizable(true, true);
    this->setResizeLimits(608, 412, 608*2, 412*2);
    this->centreWithSize(608, 412);
    this->setVisible(true);
    this->mainComponent = std::make_unique<MainContentComponent>();
    this->setContentOwned(mainComponent.get(), true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
    mainComponent->quitting = true;
    JUCEApplication::getInstance()->systemRequestedQuit();
}

