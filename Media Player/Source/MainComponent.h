//==============================================================================
/// @file MainComponent.h
/// Defines the class representing our main window content component.

#pragma once

#include "MidiPlaybackThread.h"
#include "MediaManager.h"
#include "MainApplication.h"

/// A ComboBox that dynamically populates its menu with the available MIDI
/// outputs. Invoked each time the user clicks on the menu.
struct MidiOutputMenu : public ComboBox {
    void mouseDown(const MouseEvent& me) override {
        // save the current selection
        String sel = getSelectedItemText();
        // remove menu's existing midi port list (they could be stale...)
        clear(NotificationType::dontSendNotification);
        // get current midi outputs
        const StringArray devs (MidiOutput::getDevices());
        addItemList(devs, 1); // item id's are 1 based
        ComboBox::mouseDown(me);
        // if user didnt select a new port use the previous one
        if (!getSelectedId()) {
            selectItemWithText(sel);
        }
    }

    String getSelectedItemText() {
        auto index = getSelectedItemIndex();
        return (index >= 0) ? getItemText(index) : "";
    }
    void selectItemWithText(const String& text) {
        for (auto i = 0; i < getNumItems(); ++i)
            if (getItemText(i) == text)
                return setSelectedItemIndex(i, dontSendNotification);
    }
};

struct MainComponent : public Component, public ComboBox::Listener,
public Button::Listener, public ValueTree::Listener {

    MainComponent();
    ~MainComponent() override;

private:

    //============================================================================
    // Component overrides.

    void paint (Graphics&) override;
    void resized() override;
    void comboBoxChanged(ComboBox* combo) override ;
    void buttonClicked(Button* button ) override ;

    //============================================================================
    // ValueTree::Listener overrides (only valueTreePropertyChanged() is used).

    void valueTreePropertyChanged(ValueTree& tree, const Identifier& ident) final ;
    void valueTreeChildAdded(ValueTree& tree, ValueTree& child) final {}
    void valueTreeChildRemoved(ValueTree& tree, ValueTree& child, int index) final {}
    void valueTreeChildOrderChanged(ValueTree& parent, int oldIndex, int newIndex) final {}
    void valueTreeParentChanged(ValueTree& tree) final {}

    MediaManagerData mediaManagerData;
    std::unique_ptr<Transport> transport;
    TextButton openButton {"openButton"};
    TextButton infoButton {"infoButton"};
    MidiOutputMenu midiOutputMenu {};

    //============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
