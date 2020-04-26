//==============================================================================
/// @file Transport.h
/// A generic audio transport that can be used for MIDI and audio playback.

#pragma once

#include "MediaManagerData.h"

struct Transport : public Component, public Button::Listener,
public Slider::Listener, public ValueTree::Listener {

    explicit Transport(const MediaManagerData& mmd);

    ~Transport() override;

    MediaManagerData mediaManagerData;

private:

    void resized() override;
    void paint(Graphics& gr) override;

    //============================================================================
    // JUCE component callbacks.

    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;

    //============================================================================
    // JUCE ValueTree callbacks (only valueTreePropertyChanged() is used).

    void valueTreePropertyChanged(ValueTree& tree, const Identifier& ident) final;
    void valueTreeChildAdded(ValueTree& parent, ValueTree& child) final {}
    void valueTreeChildRemoved(ValueTree& parent, ValueTree& child, int index) final {}
    void valueTreeChildOrderChanged(ValueTree& parent, int iOld, int iNew) final {}
    void valueTreeParentChanged(ValueTree& tree) final {}

    //============================================================================
    // Subcomponent drawing.

    void drawPlayPauseButton();
    void drawGoToStartButton();
    void drawGainButton(double gain);
    void drawCurrentTimeLabel();
    void drawEndTimeLabel();
    static String toFormattedTimeString(double seconds);

    //============================================================================
    // Member variables.

    Colour iconColor {Colours::white};
    DrawableButton playPauseButton {"", DrawableButton::ImageFitted};
    DrawableButton goToStartButton {"", DrawableButton::ImageFitted};
    DrawableButton gainButton {"", DrawableButton::ImageFitted};
    Slider gainSlider {Slider::LinearHorizontal, Slider::NoTextBox};
    Slider tempoSlider {Slider::LinearHorizontal, Slider::TextBoxLeft};
    Slider positionSlider {Slider::LinearHorizontal, Slider::NoTextBox};
    Label currentTimeLabel;
    Label endTimeLabel;

    double previousGain = 0.0;

    //============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Transport)
};
