//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

MainWindow::MainWindow(String name)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons) {
  centreWithSize (600, 400);
  setVisible(true);
}

//==============================================================================
// DocumentWindow overrides

void MainWindow::closeButtonPressed() {
  // when the main window is closed signal the app to exit
  JUCEApplication::getInstance()->systemRequestedQuit();
}

