//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(const String& name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    this->setUsingNativeTitleBar(true);
    this->setResizable(true, true);
    this->setResizeLimits(this->MIN_WIDTH, this->MIN_HEIGHT,
            this->MIN_WIDTH * this->FACTOR, this->MIN_HEIGHT * this->FACTOR);
    this->centreWithSize (this->MIN_WIDTH, this->MIN_HEIGHT);
    this->setVisible(true);
    this->mainComponent = std::make_unique<MainComponent>();
    this->setContentOwned(mainComponent.release(), true);
//    this->setContentComponentSize(this->MIN_WIDTH, this->MIN_HEIGHT);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
  // when the main window is closed signal the app to exit
  JUCEApplication::getInstance()->systemRequestedQuit();
}
