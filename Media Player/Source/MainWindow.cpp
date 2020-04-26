//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(const String& name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    this->setUsingNativeTitleBar(true);
    this->setResizable(false, false);
    this->centreWithSize(392, 120);
    this->mainComponent = std::make_unique<MainComponent>();
    this->setContentOwned(mainComponent.get(), true);
    this->setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

