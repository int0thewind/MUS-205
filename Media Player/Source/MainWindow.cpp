//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(const String& name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
    setUsingNativeTitleBar(true);
    setResizable(false, false);
    centreWithSize(392, 120);
    mainComponent = std::make_unique<MainComponent>();
    setContentOwned(mainComponent.get(), true);
    this->setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

