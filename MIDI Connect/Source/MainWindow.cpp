//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    this->setUsingNativeTitleBar(true);
    this->setResizable(true, true);
    this->setResizeLimits(this->MIN_WIDTH, this->MIN_HEIGHT,
                          this->MIN_WIDTH * this->FACTOR, this->MIN_HEIGHT * this->FACTOR);
    this->centreWithSize (this->MIN_WIDTH, this->MIN_HEIGHT);
    this->setVisible(true);
    this->mainComponent = std::make_unique<MainContentComponent>();
    this->setContentOwned(mainComponent.release(), true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
}

