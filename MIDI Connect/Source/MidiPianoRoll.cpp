//==============================================================================

#include "MidiPianoRoll.h"

//==============================================================================
// PianoRollNote

PianoRollNote::PianoRollNote(int key, int vel) :
keynum(key), velocity(vel) {}

PianoRollNote::~PianoRollNote() = default;

void PianoRollNote::paint(Graphics& gr) {
    gr.fillAll(Colours::red);
}

//==============================================================================
// MidiPianoRoll

MidiPianoRoll::MidiPianoRoll() {
    this->setFramesPerSecond(FPS);
};

MidiPianoRoll::~MidiPianoRoll() {
    this->clear();
};

void MidiPianoRoll::clear() {
    this->notes.clear();
}

void MidiPianoRoll::paint(Graphics& gr) {
    gr.fillAll(Colours::black);
}

void MidiPianoRoll::update() {
    for (auto note = this->notes.begin(); note != this->notes.end(); note++) {
        Rectangle<int> noteBound = (*note)->getBoundsInParent();
        if ((*note)->haveNoteOff == true) { noteBound.setX(noteBound.getX() + PPF); }
        else { noteBound.setWidth(noteBound.getWidth() + PPF); }
        (*note)->setBounds(noteBound);
        if (noteBound.getX() >= this->getWidth()) {
            this->removeChildComponent(note->get());
            note = this->notes.erase(note);
            if (note == notes.end()) { break; }
        }
    }
}

void MidiPianoRoll::addMidiMessage(const MidiMessage& msg) {
    if (msg.isNoteOn()) {
        int key = msg.getNoteNumber();
        int vel = msg.getVelocity();
        std::unique_ptr<PianoRollNote> pianoNote = std::make_unique<PianoRollNote>(key, vel);
        this->addAndMakeVisible(pianoNote.get());
        pianoNote->setBounds(this->getX(), this->getHeight() * (127 - key) / 128,
                             0, this->getHeight() / 128);
        this->notes.push_back(std::move(pianoNote));
    } else if (msg.isNoteOff()) {
        for (size_t i = this->notes.size() - 1; i >= 0; i--) {
            if (this->notes[i]->haveNoteOff == false &&
                this->notes[i]->keynum == msg.getNoteNumber()) {
                this->notes[i]->haveNoteOff = true; break;
            }
        }
    }
    
}
