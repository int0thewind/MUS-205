//==============================================================================

#include "MainApplication.h"
#include "MainWindow.h"
#include "MainComponent.h"

/// MainMenuBarModel provides the application menubar's command set. On the Mac
/// the menubar will be installed at the top of the screen and on Windows/Linux
/// it will be at the top of the window.
struct MainApplication::MainMenuBarModel : public MenuBarModel {
  
  /// The MainMenuBarModel constructor.
  /// Your method should call the function setApplicationCommandManagerToWatch()
  /// and pass it a pointer to the application's commandManager instance.
  MainMenuBarModel() {
  }
  
  /// This method returns a juce::StringArray containing all the menubar's menu names.
  /// Our app has a single menu, called "Texts".
  StringArray getMenuBarNames() override {
    return {};
  }
  
  /// This method returns a juce::PopupMenu filled with the menu command items
  /// for a given menuName passed to it.
  /// When this method receives the menuName "Texts" it should take the following steps:
  /// * Allocate a PopupMenu on the stack.
  /// * For each text id LATIN_TEXT, CYRILLIC_TEXT and GREEK_TEXT call
  /// juce::PopupMenu.addCommandItem() and pass it a pointer to the app's
  /// commandManager and the current text id.
  /// * Return the menu.
  PopupMenu getMenuForIndex (int, const String& menuName) override {
    PopupMenu menu;
    return menu;
  }
  
  // Our app uses a command manager so this function should be empty.
  void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override {
  }
};

//==============================================================================

void MainApplication::getAllCommands (juce::Array<juce::CommandID> &commands) {
  const CommandID ids[] = {
  };
  commands.addArray(ids, 0);
}

void MainApplication::getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo &result) {
}

bool MainApplication::perform (const InvocationInfo &info) {
  return true;  // Return true if the command was handled
}

MainApplication& MainApplication::getApp() {
  MainApplication* const app = dynamic_cast<MainApplication*>(JUCEApplication::getInstance());
  assert(app != nullptr);
  return *app;
}

MenuBarModel* MainApplication::getMenuBarModel() {
  return mainMenuBarModel.get();
}

//==============================================================================
// JUCEApplication overrides

MainApplication::MainApplication() {
}

const String MainApplication::getApplicationName() {
  return "";
}

const String MainApplication::getApplicationVersion() {
  return "";
}

bool MainApplication::moreThanOneInstanceAllowed() {
  return false;
}

void MainApplication::initialise(const String& commandLine) {
  // Set mainMenuBarModel to a new MainMenuBarModel

  // Call registerAllCommandsForTarget to add all the application
  // commands. See getAllCommands(), getCommandInfo().

  // On mac add the app's menubar to the top of the screen.
  // (On windows and linux it is added to the main window.)

  // Create our MainWindow.
  mainWindow = std::make_unique<MainWindow>(getApplicationName());
}

void MainApplication::shutdown() {
}

void MainApplication::systemRequestedQuit() {
  quit();
}

void MainApplication::anotherInstanceStarted(const String& commandLine) {
}

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainApplication)
