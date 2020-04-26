//==============================================================================
/// @file MediaManagerData.h
/// Wrapper class for manipulating the ValueTree properties of a Transport.

#pragma once

#include "ValueTreeWrapper.h"

struct MediaManagerData : public ValueTreeWrapper {
    static const juce::Identifier MEDIA_MANAGER_DATA_TYPEID;
    static const juce::Identifier TRANSPORT_PLAYING;
    static const juce::Identifier TRANSPORT_GAIN;
    static const juce::Identifier TRANSPORT_TEMPO;
    static const juce::Identifier TRANSPORT_POSITION;
    static const juce::Identifier TRANSPORT_ENABLED;
    static const juce::Identifier TRANSPORT_TEMPO_ENABLED;
    static const juce::Identifier TRANSPORT_CLICK_PLAY_PAUSE;
    static const juce::Identifier TRANSPORT_REWIND;
    static const juce::Identifier TRANSPORT_PLAYBACK_DURATION;
    static const juce::Identifier LOADED_MEDIA_FILE;
    static const juce::Identifier LOADED_MEDIA_TYPE;
    static const juce::Identifier MIDI_OUTPUT_OPEN_ID;
    static const juce::Identifier INTERNAL_SYNTHESIZER_AVAILABLE;
    static const juce::Identifier LAUNCH_OPEN_MEDIA_DIALOG;
    static const juce::Identifier LAUNCH_MEDIA_INFO_DIALOG;
  
    enum MEDIA_TYPES {
        MEDIA_NONE=0,
        MEDIA_MIDI,
        MEDIA_AUDIO
    };

    MediaManagerData() : ValueTreeWrapper(juce::ValueTree(MEDIA_MANAGER_DATA_TYPEID)) {}

    /**
     * For the value tree class that also represents the media manager data, to check whether it is valid or not,
     * we also have to check whether our class has the MEDIA_MANAGER_DATA_TYPEID property.
     * @return true if our value tree is a valid media manager data.
     */
    bool isValid() const override {
        return valueTreeData.isValid() && valueTreeData.hasType(MEDIA_MANAGER_DATA_TYPEID);
    }

    bool getTransportEnabled();

    void setTransportEnabled(bool shouldBeEnabled);

    bool getTransportTempoEnabled();

    void setTransportTempoEnabled(bool shouldBeEnabled);

    bool getTransportPlaying();

    void setTransportPlaying(bool shouldBePlaying, ValueTree::Listener* exclude = nullptr);

    double getTransportGain();

    void setTransportGain(double gain, ValueTree::Listener* exclude = nullptr);

    double getTransportTempo();

    void setTransportTempo(double tempo, ValueTree::Listener* exclude = nullptr);

    double getTransportPosition();

    /**
     * This method set the current position value to the value tree
     * Note that the position value is not the true duration,
     * but the relative position that is always less or equal to 1.0
     * @param pos The relative position to set
     * @param exclude the value tree listener to exclude to prevent infinite calling
     */
    void setTransportPosition(double pos, ValueTree::Listener* exclude = nullptr);

    void clickPlayPause(ValueTree::Listener* exclude = nullptr);

    void setTransportRewind(ValueTree::Listener* exclude = nullptr);

    double getPlaybackDuration();

    void setPlaybackDuration(double tempo, ValueTree::Listener* exclude = nullptr);

    File getLoadedMediaFile();

    void setLoadedMediaFile(const File& file);

    int getLoadedMediaType();

    void setLoadedMediaType(int type);

    int getMidiOutputOpenID();

    void setMidiOutputOpenID(int ident, ValueTree::Listener* exclude = nullptr);

    bool isInternalSynthesizerAvailable();

    // TODO never used this function. Where should this be used?
    void setInternalSynthesizerAvailable(bool isAvailable);

    void launchOpenMediaDialog();

    void launchMediaInfoDialog();
};
