#include "MediaManager.h"

#include <memory>
#include "MainApplication.h"

MediaManager::MediaManager() {
    String errorString = this->audioDeviceManager.initialise(0, 2, nullptr, false);
    assert(errorString == "");
    this->playbackThread = std::make_unique<MidiPlaybackThread>(this,100);
    this->playbackThread->startThread();
    this->mediaManagerData.addListener(this);
    this->formatManager.registerBasicFormats();
    this->audioDeviceManager.addAudioCallback(&audioSourcePlayer);
    this->audioSourcePlayer.setSource(&transportSource);
    this->startTimer(100);
}

MediaManager::~MediaManager() {
    this->stopTimer();
    this->mediaManagerData.removeListener(this);
    this->playbackThread->stopThread(100);
    this->playbackThread = nullptr;
    this->sendAllSoundsOff();
    this->closeMidiOutput();
    this->transportSource.setSource(nullptr);
    this->audioSourcePlayer.setSource(nullptr);
    this->audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
}

///=============================================================================
/// The ManagerData callbacks

void MediaManager::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    int mediaType = this->mediaManagerData.getLoadedMediaType();
    if (ident == MediaManagerData::TRANSPORT_PLAYING) {
        bool isPlaying = this->mediaManagerData.getTransportPlaying();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI:  isPlaying ? playMidi()  : pauseMidi();  break;
            case MediaManagerData::MEDIA_AUDIO: isPlaying ? playAudio() : pauseAudio(); break;
            default: break;
        }
    } else if (ident == MediaManagerData::TRANSPORT_GAIN) {
        double gain = mediaManagerData.getTransportGain();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI:  setMidiGain(gain);  break;
            case MediaManagerData::MEDIA_AUDIO: setAudioGain(gain); break;
            default: break;
        }
    } else if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        int tempo = (int) mediaManagerData.getTransportTempo();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI:  setMidiTempo(tempo);  break;
            case MediaManagerData::MEDIA_AUDIO: setAudioTempo(tempo); break;
            default: break;
        }
    } else if (ident == MediaManagerData::TRANSPORT_REWIND) {
        if (this->mediaManagerData.getTransportPlaying()) {
            switch (mediaType) {
                case MediaManagerData::MEDIA_MIDI:  rewindMidi();  break;
                case MediaManagerData::MEDIA_AUDIO: rewindAudio(); break;
                default: break;
            }
        }
    } else if (ident == MediaManagerData::TRANSPORT_POSITION) {
        double position = mediaManagerData.getTransportPosition();
        switch (mediaType) {
            case MediaManagerData::MEDIA_MIDI:  setMidiPlaybackPosition(position);  break;
            case MediaManagerData::MEDIA_AUDIO: setAudioPlaybackPosition(position); break;
            default: break;
        }
    } else if (ident == MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG) {
        this->openMediaFile();
    } else if (ident == MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG) {
        this->openMediaInfoDialog();
    } else if (ident == MediaManagerData::MIDI_OUTPUT_OPEN_ID) {
        // We need to subtract one here
        // since the device ID is the raw device id
        int deviceID = this->mediaManagerData.getMidiOutputOpenID();
        deviceID != 0 ? this->openMidiOutput(deviceID - 1) : this->closeMidiOutput();
    }
}

///=============================================================================
// PlaybackPositionTimer callback

void MediaManager::timerCallback() {
    int mediaType = this->mediaManagerData.getLoadedMediaType();
    switch (mediaType) {
        case MediaManagerData::MEDIA_MIDI:  scrollMidiPlaybackPosition();  break;
        case MediaManagerData::MEDIA_AUDIO: scrollAudioPlaybackPosition(); break;
        default: break;
    }
}

//==============================================================================
// Generic Media Support
//==============================================================================

void MediaManager::openMediaFile() {
    String allowedTypes = MIDI_FILE_TYPE + this->formatManager.getWildcardForAllFormats();
    FileChooser fileChooser ("Open Media Files", File(), allowedTypes);
    if (fileChooser.browseForFileToOpen()) {
        File openedFile = fileChooser.getResult();
        String openedFileExtension = openedFile.getFileExtension();
        MIDI_FILE_TYPE.contains(openedFileExtension) ? loadMidiFile(openedFile) : loadAudioFile(openedFile);
    }
}

void MediaManager::openMediaInfoDialog() {
    String mediaInfo = this->getMediaInfo();
    if (mediaInfo.isEmpty()) { return; }
    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Media Info", mediaInfo);
}

String MediaManager::getMediaInfo() {
    switch (this->mediaManagerData.getLoadedMediaType()) {
        case MediaManagerData::MEDIA_MIDI:  return getMidiInfo();
        case MediaManagerData::MEDIA_AUDIO: return getAudioInfo();
        default: return "";
    }
}

//==============================================================================
// Audio playback support

void MediaManager::openAudioSettings() {
    // No operation here
}

String MediaManager::getAudioInfo() {
    std::stringstream ss;

    File audioFile = mediaManagerData.getLoadedMediaFile();
    AudioFormatReader* fileReader = this->audioFileReaderSource->getAudioFormatReader();

    ss << "Audio file: "          << audioFile.getFullPathName() << "\n";
    ss << "File size: "           << audioFile.getSize() / 1000000. << " MB" << "\n";
    ss << "Audio format: "        << audioFile.getFileExtension().substring(1).toUpperCase() << "\n";
    ss << "Channels: "            << fileReader->numChannels << "\n";
    ss << "Sample rate: "         << fileReader->sampleRate << "\n";
    // TODO sample frames in audio info?
    ss << "Sample frames: "       << fileReader->lengthInSamples << "\n";
    ss << "Bits per sample: "     << fileReader->bitsPerSample << "\n";
    ss << "Floating point data: " << (fileReader->usesFloatingPointData ? "True" : "False") << "\n";

    return ss.str();
}

void MediaManager::loadAudioFile(File audioFile) {
    if (AudioFormatReader* reader = this->formatManager.createReaderFor(audioFile)) {
        this->loadIntoTransport(reader);
    } else {
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "Error Opening File",
                "The file that you've opened is unsupported", "OK");
        audioFile = File();
    }
    this->mediaManagerData.setLoadedMediaFile(audioFile);
    int type = audioFile == File() ? MediaManagerData::MEDIA_NONE : MediaManagerData::MEDIA_AUDIO;
    this->mediaManagerData.setLoadedMediaType(type);
}

void MediaManager::loadIntoTransport(AudioFormatReader* reader) {
    this->mediaManagerData.setTransportPlaying(false);
    this->mediaManagerData.setTransportPosition(0.0);
    this->clearAudioPlaybackState();
    // Also clear midi playback state
    this->clearMidiPlaybackState();
    this->audioFileReaderSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    this->transportSource.setSource(audioFileReaderSource.get(), 0, nullptr, reader->sampleRate);
    this->mediaManagerData.setPlaybackDuration(transportSource.getLengthInSeconds());
}

void MediaManager::clearAudioPlaybackState() {
    this->transportSource.setSource(nullptr);
    this->audioFileReaderSource = nullptr;
}

//==============================================================================
// MIDI transport callbacks

void MediaManager::playMidi() {
    if (this->playbackThread->isPlaybackAtEnd()) {
        this->setMidiPlaybackPosition(0);
    }
    this->playbackThread->setTempo(this->mediaManagerData.getTransportTempo());
    this->playbackThread->setGain(this->mediaManagerData.getTransportGain());
    this->playbackThread->setPaused(false);
}

void MediaManager::pauseMidi() {
    this->playbackThread->setPaused(true);
}

void MediaManager::setMidiGain(double gain) {
    this->playbackThread->setGain(gain);
}

void MediaManager::setMidiTempo(double tempo) {
    this->playbackThread->setTempo(tempo);
}

void MediaManager::rewindMidi() {
    // TODO need to revise rewindMidi()
    this->setMidiPlaybackPosition(0);
}

void MediaManager::setMidiPlaybackPosition(double position) {
    // TODO need to revise setMidiPlaybackPosition
    this->playbackThread->setPlaybackPosition(position);
}

void MediaManager::scrollMidiPlaybackPosition() {
    // TODO how to get the current playing position.
    double sec = this->playbackThread->getPlaybackBeat();
    double dur = this->midiFileDuration;
    double pos = sec / dur;

    this->mediaManagerData.setTransportPosition(pos, this);
    if (pos >= 1.0) {
        this->mediaManagerData.clickPlayPause();
    }
}

//==============================================================================
// Audio transport callbacks

void MediaManager::playAudio() {
    if (this->transportSource.hasStreamFinished()) {
        this->transportSource.setPosition(0.0);
    }
    this->transportSource.setGain((float) this->mediaManagerData.getTransportGain());
    this->transportSource.start();
}

void MediaManager::pauseAudio() {
    this->transportSource.stop();
}

void MediaManager::setAudioGain(double gain) {
    this->mediaManagerData.setTransportGain(gain);
    this->transportSource.setGain((float) this->mediaManagerData.getTransportGain());
}

void MediaManager::setAudioTempo(double tempo) {
    // ! Audio files does not have tempo settings, nothing to do!
}

void MediaManager::rewindAudio() {
    this->transportSource.setPosition(0.0);
}

/// Sets the audio transport player's position.
void MediaManager::setAudioPlaybackPosition(double pos) {
    bool isPlaying = this->mediaManagerData.getTransportPlaying();
    if (pos == 0) {
        this->transportSource.setSource(nullptr);
        this->transportSource.setSource(this->audioFileReaderSource.get());
        if (isPlaying) {
            this->transportSource.start();
        }
    } else {
        if (isPlaying) {
            this->transportSource.stop();
        }
        this->transportSource.setPosition(pos * this->transportSource.getLengthInSeconds());
        if (isPlaying) {
            this->transportSource.start();
        }
    }
}

void MediaManager::scrollAudioPlaybackPosition() {
    double sec = this->transportSource.getCurrentPosition();
    double dur = this->transportSource.getLengthInSeconds();
    double pos = sec / dur;
    this->mediaManagerData.setTransportPosition(pos, this);
    // auto-pause if at end-of-file
    if (pos >= 1.0) {
        this->mediaManagerData.clickPlayPause();
    }
}

///==============================================================================

void MediaManager::openMidiOutput(int dev) {
    this->midiOutputDevice = MidiOutput::openDevice(dev);
    jassert(this->midiOutputDevice != nullptr);
}

void MediaManager::closeMidiOutput() {
    this->midiOutputDevice.reset(nullptr);
}

bool MediaManager::isInternalSynthesizerAvailable() {
    return this->mediaManagerData.isInternalSynthesizerAvailable();
}

///==============================================================================
/// MidiFile Functions

String MediaManager::getMidiInfo() {
    std::stringstream ss;

    File audioFile = mediaManagerData.getLoadedMediaFile();

    ss << "MIDI file: "              << audioFile.getFullPathName() << "\n";
    ss << "File size: "              << audioFile.getSize() << "\n";
    ss << "MIDI file format: level " << audioFile.getFileExtension() << "\n";
    ss << "Number of tracks: "       << this->midiFileNumTracks << "\n";
    ss << "Durations: "              << this->midiFileDuration << "\n";
    // TODO what is the number of messages?
    ss << "Number of messages: "     << this->midiMessageSequence.getNumEvents() << "\n";

    return ss.str();
}

void MediaManager::clearMidiPlaybackState() {
    this->midiFileDuration   = 0.0;
    this->midiFileLength     = 0;
    this->midiFileNumTracks  = 0;
    this->midiFileTimeFormat = 0;
    this->midiMessageSequence.clear();
    this->playbackThread->setPlaybackPosition(0, 0);
    // My added code here: clear the midiOutputOpenID;
    this->mediaManagerData.setMidiOutputOpenID(0);
}

void MediaManager::loadMidiFile(File midiFile) {
    MidiFile newMidiFile;
    FileInputStream fileInputStream(midiFile);
    if (fileInputStream.openedOk() && newMidiFile.readFrom(fileInputStream) && newMidiFile.getTimeFormat() > 0) {
        this->loadIntoPlayer(newMidiFile);
    } else {
        AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "Error Opening File",
                                         "The file that you've opened is unsupported", "OK");
        midiFile = File();
    }
    this->mediaManagerData.setLoadedMediaFile(midiFile);
    int type = midiFile == File() ? MediaManagerData::MEDIA_NONE : MediaManagerData::MEDIA_MIDI;
    this->mediaManagerData.setLoadedMediaType(type);
}

void MediaManager::loadIntoPlayer(MidiFile& midiFile) {
    this->mediaManagerData.setTransportPlaying(false);
    this->mediaManagerData.setTransportPosition(0.0);

    this->clearAudioPlaybackState();
    this->clearMidiPlaybackState();
    midiFile.convertTimestampTicksToSeconds();

    for (int track = 0; track < midiFile.getNumTracks(); track++) {
        auto* trackSequence = const_cast<MidiMessageSequence *>(midiFile.getTrack(track));
        this->midiMessageSequence.addSequence(*trackSequence,
                0, 0, trackSequence->getEndTime() + 1);
        this->midiMessageSequence.updateMatchedPairs();
    }

    // TODO what is time format in a midi file?
    this->midiFileNumTracks  = midiFile.getNumTracks();
    this->midiFileTimeFormat = midiFile.getTimeFormat();
    // TODO MIDI file duration vs length?
    this->midiFileDuration   = this->midiMessageSequence.getEndTime();
    this->midiFileLength     = this->midiMessageSequence.getNumEvents();

    DBG("The midi file duration is " + String(midiFileDuration));
    DBG("The midi file length is " + String(midiFileLength));

    this->playbackThread->setPlaybackLimit(midiFileDuration, midiFileLength);
    this->mediaManagerData.setPlaybackDuration(midiFileLength);
}

///==============================================================================
/// MidiMessage Functions

void MediaManager::sendMessage(const MidiMessage& message) {
    ScopedLock sl (sendLock);
    if (midiOutputDevice) {
        this->midiOutputDevice->sendMessageNow(message);
    } else if (isInternalSynthesizerAvailable()) {
        this->playInternalSynthesizer(message);
    }
}

void MediaManager::playInternalSynthesizer(const MidiMessage& message) {
    // TODO Is this function NOP?
}

void MediaManager::sendAllSoundsOff() {
    std::cout << "MediaManager::sendAllSoundsOff(). Output port is " << (this->midiOutputDevice ? "OPEN" : "CLOSED") << "\n";
    for (int i = 1; i <= 16; i++) {
        juce::MidiMessage msg = juce::MidiMessage::allSoundOff(i);
        this->sendMessage(msg);
    }
}

//==============================================================================
// MidiPlaybackClient callbacks

void MediaManager::handleMessage (const MidiMessage &message) {
    this->sendMessage(message);
}

void MediaManager::addMidiPlaybackMessages(MidiPlaybackThread::MidiMessageQueue& queue,
                                           MidiPlaybackThread::PlaybackPosition& position) {
    int index = position.index;
    for (; index < position.length; index++) {
        juce::MidiMessageSequence::MidiEventHolder* ev = midiMessageSequence.getEventPointer(index);
        // skip over non-channel messages
        if (ev->message.getChannel() < 1) {
            continue;
        }
        // skip over noteOffs because we add by pairs with noteOns
        if (ev->message.isNoteOff()) {
            continue;
        }
        // add every message that is at or earlier (e.g. late) than the current time
        if (ev->message.getTimeStamp() <= position.beat) {
            queue.addMessage(new juce::MidiMessage(ev->message));
            if (ev->noteOffObject) {
                queue.addMessage(new juce::MidiMessage(ev->noteOffObject->message));
            }
        }
        else {
            break;
        }
    }
    // index is now the index of the next (future) event or length
    position.index = index;
    if (position.isAtEnd()) {
        std::cout << "Midi playback at end!\n";
    }
}




