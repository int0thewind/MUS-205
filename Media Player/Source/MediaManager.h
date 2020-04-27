#include <cstddef>
//==============================================================================
/// @file MediaManager.h
/// A class providing support for audio/midi playback.

#pragma once

#include "MediaManagerData.h"
#include "MidiPlaybackThread.h"

struct MediaManager :
public MidiPlaybackThread::MidiPlaybackClient, public ValueTree::Listener, public Timer
{
    MediaManager();

    ~MediaManager() override;

    MediaManagerData& getManagerData() { return mediaManagerData; }

private:

    //============================================================================
    // ValueTree::Listener overrides (only valueTreePropertyChanged() is used)

    void valueTreePropertyChanged(ValueTree& tree, const Identifier& ident) final ;

    void valueTreeChildAdded(ValueTree& tree, ValueTree& child) final {}
    void valueTreeChildRemoved(ValueTree& tree, ValueTree& child, int index) final {}
    void valueTreeChildOrderChanged(ValueTree& tree, int oldc, int newc) final {}
    void valueTreeParentChanged(ValueTree& tree) final {}

    //============================================================================
    // Timer overrides

    void timerCallback() override;

    //============================================================================
    // Generic Media Support

    void openMediaFile();

    void openMediaInfoDialog();

    String getMediaInfo();

    //==============================================================================
    // Audio playback support

    __unused void openAudioSettings();

    String getAudioInfo();

    void playAudio();

    void pauseAudio();

    void setAudioGain(double gain);

    __unused void setAudioTempo(double tempo);

    void rewindAudio();

    void setAudioPlaybackPosition(double pos);

    /**
     * This method controls the transport's position slider during playback.
     * It is being called automatically and periodically by the timer callback.
     * @see timerCallback()
     */
    void scrollAudioPlaybackPosition();

    void loadAudioFile(File file) ;

    void loadIntoTransport(AudioFormatReader* reader) ;

    void clearAudioPlaybackState();

    //============================================================================
    // MIDI playback support

    void playMidi();

    void pauseMidi();

    void setMidiGain(double gain);

    void setMidiTempo(double tempo);

    void setMidiPlaybackPosition(double position);

    void scrollMidiPlaybackPosition();

    void rewindMidi();

    String getMidiInfo();

    bool isInternalSynthesizerAvailable();

    void openMidiOutput(int dev);

    void closeMidiOutput();

    void loadMidiFile(File file);

    void loadIntoPlayer(MidiFile& midiFile);

    void clearMidiPlaybackState();

    void playInternalSynthesizer(const MidiMessage& message);

    void sendAllSoundsOff();

    void sendMessage(const MidiMessage& message);

    //==============================================================================
    // MidiPlaybackClient overrides

    void addMidiPlaybackMessages(MidiPlaybackThread::MidiMessageQueue &queue,
                               MidiPlaybackThread::PlaybackPosition &position) override;

    void handleMessage(const MidiMessage& midiMessage) override;


    //==============================================================================
    // Audio member data

    AudioDeviceManager audioDeviceManager;

    AudioFormatManager formatManager;

    AudioSourcePlayer audioSourcePlayer;

    AudioTransportSource transportSource;

    std::unique_ptr<juce::AudioFormatReaderSource> audioFileReaderSource;

    //==============================================================================
    // MIDI member data.

    std::unique_ptr<MidiOutput> midiOutputDevice;

    std::unique_ptr<MidiPlaybackThread> playbackThread;

    MediaManagerData mediaManagerData;

    int midiFileNumTracks {0};

    int midiFileTimeFormat {0};

    int midiFileLength {0};

    double midiFileDuration {0.0};

    MidiMessageSequence midiMessageSequence;

    CriticalSection sendLock;

    const String MIDI_FILE_TYPE {"*.mid;*.midi;"};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MediaManager)
};

