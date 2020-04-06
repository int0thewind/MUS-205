#include "MediaManagerData.h"

const juce::Identifier MediaManagerData::MEDIA_MANAGER_DATA_TYPEID {"MediaManagerData"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYING {"TransportPlaying"};
const juce::Identifier MediaManagerData::TRANSPORT_GAIN {"TransportGain"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO {"TransportTempo"};
const juce::Identifier MediaManagerData::TRANSPORT_POSITION {"TransportPosition"};
const juce::Identifier MediaManagerData::TRANSPORT_ENABLED {"TransportEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_TEMPO_ENABLED {"TransportTempoEnabled"};
const juce::Identifier MediaManagerData::TRANSPORT_CLICK_PLAYPAUSE {"TransportClickPlayPlause"};
const juce::Identifier MediaManagerData::TRANSPORT_REWIND {"TransportRewind"};
const juce::Identifier MediaManagerData::TRANSPORT_PLAYBACK_DURATION {"TransportPlaybackDuration"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_FILE {"LoadedMediaFile"};
const juce::Identifier MediaManagerData::LOADED_MEDIA_TYPE {"LoadedMediaType"};
const juce::Identifier MediaManagerData::MIDI_OUTPUT_OPEN_ID {"MidiOutputOpenID"};
const juce::Identifier MediaManagerData::INTERNAL_SYNTH_AVAILABLE {"InternalSynthAvailable"};
const juce::Identifier MediaManagerData::LAUNCH_OPEN_MEDIA_DIALOG {"LaunchOpenMediaDialog"};
const juce::Identifier MediaManagerData::LAUNCH_MEDIA_INFO_DIALOG {"LaunchMediaInfoDialog"};

bool MediaManagerData::getTransportEnabled() {
  return false;
}

void MediaManagerData::setTransportEnabled(bool shouldBeEnabled) {
}

bool MediaManagerData::getTransportTempoEnabled() {
  return false;
}

void MediaManagerData::setTransportTempoEnabled(bool shouldBeEnabled) {
}

bool MediaManagerData::getTransportPlaying() {
  return false;
}

void MediaManagerData::setTransportPlaying(bool shouldBePlaying, ValueTree::Listener* exclude) {
}

double MediaManagerData::getTransportGain() {
  return 0.0;
}

void MediaManagerData::setTransportGain(double gain, ValueTree::Listener* exclude) {
}

double MediaManagerData::getTransportTempo() {
  return 0.0;
}

void MediaManagerData::setTransportTempo(double tempo, ValueTree::Listener* exclude) {
}

double MediaManagerData::getTransportPosition() {
  return 0.0;
}

void MediaManagerData::setTransportPosition(double pos, ValueTree::Listener* exclude) {
}

void MediaManagerData::clickPlayPause(ValueTree::Listener* exclude) {
}

void MediaManagerData::setTransportRewind(ValueTree::Listener* exclude) {
  int val = data.getProperty(TRANSPORT_REWIND, 0);
  data.setPropertyExcludingListener(exclude, TRANSPORT_REWIND, val + 1, nullptr);
}

double MediaManagerData::getPlaybackDuration() {
  return 0.0;
}

void MediaManagerData::setPlaybackDuration(double tempo, ValueTree::Listener* exclude) {
}

File MediaManagerData::getLoadedMediaFile() {
  return File();
}

void MediaManagerData::setLoadedMediaFile(const File& file) {
}

int MediaManagerData::getLoadedMediaType() {
  return 0;
}

void MediaManagerData::setLoadedMediaType(int mediaType) {
}

int MediaManagerData::getMidiOutputOpenID() {
  return 0;
}

void MediaManagerData::setMidiOutputOpenID(int ident, ValueTree::Listener* exclude) {
}

bool MediaManagerData::isInternalSynthAvailable() {
  return false;
}

void MediaManagerData::setInternalSynthAvailable(bool isAvailable) {
}

void MediaManagerData::launchOpenMediaDialog() {
 }

void MediaManagerData::launchMediaInfoDialog() {
}
