#include "MediaManager.h"
#include "MainApplication.h"

MediaManager::MediaManager() {
}

MediaManager::~MediaManager() {
}

///=============================================================================
/// The ManagerData callbacks

void MediaManager::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
  auto mediaType = managerData.getLoadedMediaType();
}

///=============================================================================
// PlaybackPositionTimer callback

void MediaManager::timerCallback() {
}

//==============================================================================
// Generic Media Support
//==============================================================================

void MediaManager::openMediaFile() {
}

void MediaManager::openMediaInfoDialog() {
}

const String MediaManager::getMediaInfo() {
  return "";
}

//==============================================================================
// Audio playback support

void MediaManager::openAudioSettings() {
}

const String MediaManager::getAudioInfo() {
  return "";
}

void MediaManager::loadAudioFile(File audioFile) {
}

void MediaManager::loadIntoTransport(AudioFormatReader* reader) {
}

void MediaManager::clearAudioPlaybackState() {
}

//==============================================================================
// MIDI transport callbacks

void MediaManager::playMidi() {
  std::cout << "MediaManager::playMidi()\n";
}

void MediaManager::pauseMidi() {
  std::cout << "MediaManager::pauseMidi()\n";
}

void MediaManager::setMidiGain(double gain) {
  std::cout << "MediaManager::setMidiGain(" << gain << ")\n";

}

void MediaManager::setMidiTempo(double tempo) {
  std::cout << "MediaManager::setMidiTempo(" << tempo << ")\n";
}

void MediaManager::rewindMidi() {
  std::cout << "MediaManager::rewindMidi()\n";
}

void MediaManager::setMidiPlaybackPosition(double position) {
  std::cout << "MediaManager::setMidiPlaybackPosition("<< position <<")\n";
}

void MediaManager::scrollMidiPlaybackPosition() {
  std::cout << "MediaManager::scrollMidiPlaybackPosition()\n";
}

/// Audio transport callbacks

void MediaManager::playAudio() {
  std::cout << "MediaManager::playAudio()\n";
  // If the transportSource has finished playing set its position to 0.0
  if (transportSource.hasStreamFinished()) {
    transportSource.setPosition(0.0);
  }
  // Set the transportSource's gain to the managerData's gain.
  transportSource.setGain(managerData.getTransportGain());
  // Start the transport source.
  transportSource.start();
}

void MediaManager::pauseAudio() {
  std::cout << "MediaManager::pauseAudio()\n";
  // Stop the transportSource.
  transportSource.stop();
}

void MediaManager::setAudioGain(double gain) {
  std::cout << "MediaManager::setAudioGain(" << gain << ")\n";
  // Set the transportSource's gain to the managerData's gain.
  transportSource.setGain(managerData.getTransportGain());
}

void MediaManager::setAudioTempo(double tempo) {
  std::cout << "MediaManager::setAudioTempo(" << tempo << ")\n";
  // nothing to do!
}

void MediaManager::rewindAudio() {
  std::cout << "MediaManager::rewindAudio()\n";
  // set the transportSource's position back to 0.0.
  transportSource.setPosition(0.0);
}

/// Sets the audio transport player's position.
void MediaManager::setAudioPlaybackPosition(double pos) {
  auto playing = managerData.getTransportPlaying();
  std::cout << "media manager: receiving position:" << pos
  << ", playing:" << managerData.getTransportPlaying() << "\n" ;
  if (pos == 0) {
    // setSource(0) stops popping on rewind and play
    transportSource.setSource(0);
    transportSource.setSource(audioFileReaderSource.get());
    if (playing) transportSource.start();
  }
  else {
    // std::cout << "transport position=" << position << "\n";
    if (playing) transportSource.stop(); // not sure why this is necessary!
    transportSource.setPosition(pos * transportSource.getLengthInSeconds());
    if (playing) transportSource.start();
  }
}

void MediaManager::scrollAudioPlaybackPosition() {
  double sec = transportSource.getCurrentPosition();
  double dur = transportSource.getLengthInSeconds();
  double pos = sec/dur;
  std::cout << "pbPos="<<sec<<", pbDur="<<dur<<", pbrat="<<pos<<"\n";
  managerData.setTransportPosition(pos, this);
  // auto-pause if at end-of-file
  if (pos >= 1.0)
    managerData.clickPlayPause();
}

///==============================================================================

void MediaManager::openMidiOutput(int dev) {
  // Call MidiOutput::openDevice and reset the midiOutputDevice to it.
  midiOutputDevice = MidiOutput::openDevice(dev);
  jassert(midiOutputDevice != nullptr);
}

void MediaManager::closeMidiOutput() {
  // Set the midiOutputDevice to nullptr.
  midiOutputDevice.reset(nullptr);
}

bool MediaManager::isInternalSynthAvailable() {
  return false;
}


///==============================================================================
/// MidiFile Functions

const String MediaManager::getMidiInfo() {
  return "";
}

void MediaManager::clearMidiPlaybackState() {
}

void MediaManager::loadMidiFile(File midiFile) {
}

void MediaManager::loadIntoPlayer(MidiFile& midifile) {
}

///==============================================================================
/// MidiMessage Functions

void MediaManager::sendMessage(const MidiMessage& message) {
  ScopedLock sl (sendLock);
  if (midiOutputDevice) {
    midiOutputDevice->sendMessageNow(message);
  }
  else if (isInternalSynthAvailable()) {
    playInternalSynth(message);
  }
}

void MediaManager::playInternalSynth(const MidiMessage& message) {
}

void MediaManager::sendAllSoundsOff() {
  std::cout << "Sending allSoundsOff, output port is "
  << (midiOutputDevice.get() ? "OPEN" : "CLOSED") << "\n";
  for (int i = 1; i <= 16; i++) {
    juce::MidiMessage msg = juce::MidiMessage::allSoundOff(i);
    sendMessage(msg);
  }
}

//==============================================================================
// MidiPlaybackClient callbacks

void MediaManager::handleMessage (const MidiMessage &message) {
  sendMessage(message);
}

void MediaManager::addMidiPlaybackMessages(MidiPlaybackThread::MidiMessageQueue& queue,
                                           MidiPlaybackThread::PlaybackPosition& position) {
  int index = position.index;
  for (; index < position.length; index++) {
    juce::MidiMessageSequence::MidiEventHolder* ev = sequence.getEventPointer(index);
    // skip over non-channel messages
    if (ev->message.getChannel() < 1)
      continue;
    // skip over noteOffs because we add by pairs with noteOns
    if (ev->message.isNoteOff())
      continue;
    // add every message that is at or earlier (e.g. late) than the current time
    if (ev->message.getTimeStamp() <= position.beat) {
      queue.addMessage(new juce::MidiMessage(ev->message));
      if (ev->noteOffObject) {
        queue.addMessage(new juce::MidiMessage(ev->noteOffObject->message));
      }
    }
    else
      break;
  }
  // index is now the index of the next (future) event or length
  position.index = index;
  if (position.isAtEnd())
    std::cout << "Midi playback at end!\n";
}




