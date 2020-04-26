#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include "MediaManagerData.h"

const juce::Identifier MediaManagerData::MEDIA_MANAGER_DATA_TYPEID {"MediaManagerData"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYING {"TransportPlaying"};
const juce::Identifier MediaManagerData::TRANSPORT_GAIN {"TransportGain"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO {"TransportTempo"};
const juce::Identifier MediaManagerData::TRANSPORT_POSITION {"TransportPosition"};
const juce::Identifier MediaManagerData::TRANSPORT_ENABLED {"TransportEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO_ENABLED {"TransportTempoEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_CLICK_PLAY_PAUSE {"TransportClickPlayPause"};
const juce::Identifier MediaManagerData::TRANSPORT_REWIND {"TransportRewind"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYBACK_DURATION {"TransportPlaybackDuration"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_FILE {"LoadedMediaFile"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_TYPE {"LoadedMediaType"};
const juce::Identifier MediaManagerData::MIDI_OUTPUT_OPEN_ID {"MidiOutputOpenID"};
const juce::Identifier MediaManagerData::INTERNAL_SYNTHESIZER_AVAILABLE {"InternalSynthesizerAvailable"};
const juce::Identifier MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG {"LaunchOpenMediaDialog"};
const juce::Identifier MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG {"LaunchMediaInfoDialog"};

bool MediaManagerData::getTransportEnabled() {
    return (bool) this->valueTreeData.getProperty(TRANSPORT_ENABLED, false);
}

void MediaManagerData::setTransportEnabled(bool shouldBeEnabled) {
    this->valueTreeData.setProperty(TRANSPORT_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportTempoEnabled() {
    return (bool) this->valueTreeData.getProperty(TRANSPORT_TEMPO_ENABLED, false);
}

void MediaManagerData::setTransportTempoEnabled(bool shouldBeEnabled) {
    this->valueTreeData.setProperty(TRANSPORT_TEMPO_ENABLED, shouldBeEnabled, nullptr);
}

bool MediaManagerData::getTransportPlaying() {
    return (bool) this->valueTreeData.getProperty(TRANSPORT_PLAYING, false);
}

void MediaManagerData::setTransportPlaying(bool shouldBePlaying, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_PLAYING, shouldBePlaying, nullptr);
}

double MediaManagerData::getTransportGain() {
    return (double) this->valueTreeData.getProperty(TRANSPORT_GAIN, 1.0);
}

void MediaManagerData::setTransportGain(double gain, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_GAIN, gain, nullptr);
}

double MediaManagerData::getTransportTempo() {
    return (double) this->valueTreeData.getProperty(TRANSPORT_TEMPO, 60.0);
}

void MediaManagerData::setTransportTempo(double tempo, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_TEMPO, tempo, nullptr);
}

double MediaManagerData::getTransportPosition() {
    return (double) this->valueTreeData.getProperty(TRANSPORT_POSITION, 0.0);
}

void MediaManagerData::setTransportPosition(double pos, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_POSITION, pos, nullptr);
}

void MediaManagerData::clickPlayPause(ValueTree::Listener* exclude) {
    int playPauseClickedTime = this->valueTreeData.getProperty(TRANSPORT_CLICK_PLAY_PAUSE, 0);
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_CLICK_PLAY_PAUSE, playPauseClickedTime + 1, nullptr);
}

void MediaManagerData::setTransportRewind(ValueTree::Listener* exclude) {
    int transportRewind = valueTreeData.getProperty(TRANSPORT_REWIND, 0);
    valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_REWIND, transportRewind + 1, nullptr);
}

double MediaManagerData::getPlaybackDuration() {
    return (double) this->valueTreeData.getProperty(TRANSPORT_PLAYBACK_DURATION, 0.0);
}

void MediaManagerData::setPlaybackDuration(double tempo, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, TRANSPORT_PLAYBACK_DURATION, tempo, nullptr);
}

File MediaManagerData::getLoadedMediaFile() {
    return File(this->valueTreeData.getProperty(LOADED_MEDIA_FILE, ""));
}

void MediaManagerData::setLoadedMediaFile(const File& file) {
    this->valueTreeData.setProperty(LOADED_MEDIA_FILE, file.getFullPathName(), nullptr);
}

int MediaManagerData::getLoadedMediaType() {
    return (int) this->valueTreeData.getProperty(LOADED_MEDIA_TYPE, MEDIA_TYPES::MEDIA_NONE);
}

void MediaManagerData::setLoadedMediaType(int mediaType) {
    this->valueTreeData.setProperty(LOADED_MEDIA_TYPE, mediaType, nullptr);
}

int MediaManagerData::getMidiOutputOpenID() {
    return (int) this->valueTreeData.getProperty(MIDI_OUTPUT_OPEN_ID, MEDIA_TYPES::MEDIA_NONE);
}

void MediaManagerData::setMidiOutputOpenID(int ident, ValueTree::Listener* exclude) {
    this->valueTreeData.setPropertyExcludingListener(exclude, MIDI_OUTPUT_OPEN_ID, ident, nullptr);
}

bool MediaManagerData::isInternalSynthesizerAvailable() {
    return (bool) this->valueTreeData.getProperty(INTERNAL_SYNTHESIZER_AVAILABLE, false);
}

void MediaManagerData::setInternalSynthesizerAvailable(bool isAvailable) {
    this->valueTreeData.setProperty(INTERNAL_SYNTHESIZER_AVAILABLE, isAvailable, nullptr);
}

void MediaManagerData::launchOpenMediaDialog() {
    this->valueTreeData.sendPropertyChangeMessage(LAUNCH_OPEN_MEDIA_DIALOG);
 }

void MediaManagerData::launchMediaInfoDialog() {
    this->valueTreeData.sendPropertyChangeMessage(LAUNCH_MEDIA_INFO_DIALOG);
}

//#pragma clang diagnostic pop