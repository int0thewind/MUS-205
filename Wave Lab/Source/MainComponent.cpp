//==============================================================================

#include "MainComponent.h"
#include "MainApplication.h"

MainComponent::MainComponent()
: deviceManager (MainApplication::getApp().audioDeviceManager), audioVisualizer(2) {
}

MainComponent::~MainComponent() {
}

//==============================================================================
// Component overrides
//==============================================================================

void MainComponent::paint (Graphics& g) {
}

void MainComponent::resized() {
}

void MainComponent::drawPlayButton(juce::DrawableButton& button, bool showPlay) {
  juce::Path path;
  if (showPlay) {
    // draw the triangle
  }
  else {
    // draw the two bars
  }
  juce::DrawablePath drawable;
  drawable.setPath(path);
  juce::FillType fill (Colours::white);
  drawable.setFill(fill);
  button.setImages(&drawable);
}

//==============================================================================
// Listener overrides
//==============================================================================

void MainComponent::buttonClicked (Button *button) {
}

void MainComponent::sliderValueChanged (Slider *slider) {
}

void MainComponent::comboBoxChanged (ComboBox *menu) {
}

//==============================================================================
// Timer overrides
//==============================================================================

void MainComponent::timerCallback() {
}

//==============================================================================
// AudioSource overrides
//==============================================================================

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
}

void MainComponent::releaseResources() {
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
  bufferToFill.clearActiveBufferRegion();
  switch (waveformId) {
    case WhiteNoise:      whiteNoise(bufferToFill);   break;
    case DustNoise:       dust(bufferToFill);         break;
    case BrownNoise:      brownNoise(bufferToFill);   break;
    case SineWave:        sineWave(bufferToFill);     break;
    case LF_ImpulseWave:  LF_impulseWave(bufferToFill);  break;
    case LF_SquareWave:   LF_squareWave(bufferToFill);   break;
    case LF_SawtoothWave: LF_sawtoothWave(bufferToFill); break;
    case LF_TriangeWave:  LF_triangleWave(bufferToFill); break;
    case BL_ImpulseWave:  BL_impulseWave(bufferToFill);  break;
    case BL_SquareWave:   BL_squareWave(bufferToFill);   break;
    case BL_SawtoothWave: BL_sawtoothWave(bufferToFill); break;
    case BL_TriangeWave:  BL_triangleWave(bufferToFill); break;
    case WT_SineWave:
    case WT_ImpulseWave:
    case WT_SquareWave:
    case WT_SawtoothWave:
    case WT_TriangleWave:
      WT_wave(bufferToFill);
      break;
    case Empty:
      break;
  }
  audioVisualizer.pushBuffer(bufferToFill);
}

//==============================================================================
// Audio Utilities
//==============================================================================

double MainComponent::phasor() {
  double p = phase;
  return p;
}

float MainComponent::ranSamp() {
  return 0.0;
}

float MainComponent::ranSamp(const float mul) {
  return (ranSamp() * mul);
}

float MainComponent::lowPass(const float value, const float prevout, const float alpha) {
  return 0.0;
}

bool MainComponent::isPlaying() {
  return false;
}

void MainComponent::openAudioSettings() {
}

void MainComponent::createWaveTables() {
  createSineTable(sineTable);
  oscillators.push_back(std::make_unique<WavetableOscillator>(sineTable));
  createImpulseTable(impulseTable);
  oscillators.push_back(std::make_unique<WavetableOscillator>(impulseTable));
  createSquareTable(squareTable);
  oscillators.push_back(std::make_unique<WavetableOscillator>(squareTable));
  createSawtoothTable(sawtoothTable);
  oscillators.push_back(std::make_unique<WavetableOscillator>(sawtoothTable));
  createTriangleTable(triangleTable);
  oscillators.push_back(std::make_unique<WavetableOscillator>(triangleTable));
}

//==============================================================================
// Noise
//==============================================================================

// White Noise

void MainComponent::whiteNoise (const AudioSourceChannelInfo& bufferToFill) {
}

// Dust

void MainComponent::dust (const AudioSourceChannelInfo& bufferToFill) {
}

// Brown Noise

void MainComponent::brownNoise (const AudioSourceChannelInfo& bufferToFill) {
}

//==============================================================================
// Sine Wave
//==============================================================================

void MainComponent::sineWave (const AudioSourceChannelInfo& bufferToFill) {
}

//==============================================================================
// Low Frequency Waveforms
//==============================================================================

/// Impulse wave

void MainComponent::LF_impulseWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Square wave

void MainComponent::LF_squareWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Sawtooth wave

void MainComponent::LF_sawtoothWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Triangle wave

void MainComponent::LF_triangleWave (const AudioSourceChannelInfo& bufferToFill) {
}

//==============================================================================
// Band Limited Waveforms
//==============================================================================

/// Impulse (pulse) wave

/// Synthesized by summing sin() over frequency and all its harmonics at equal
/// amplitude. To make it band limited only include harmonics that are at or
/// below the nyquist limit.
void MainComponent::BL_impulseWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Square wave

/// Synthesized by summing sin() over all ODD harmonics at 1/harmonic amplitude.
/// To make it band limited only include harmonics that are at or below the
/// nyquist limit.
void MainComponent::BL_squareWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Sawtooth wave
///
/// Synthesized by summing sin() over all harmonics at 1/harmonic amplitude. To make
/// it band limited only include harmonics that are at or below the nyquist limit.
void MainComponent::BL_sawtoothWave (const AudioSourceChannelInfo& bufferToFill) {
}

/// Triangle wave
///
/// Synthesized by summing sin() over all ODD harmonics at 1/harmonic**2 amplitude.
/// To make it band limited only include harmonics that are at or below the
/// Nyquist limit.
void MainComponent::BL_triangleWave (const AudioSourceChannelInfo& bufferToFill) {
}

//==============================================================================
// WaveTable Synthesis
//==============================================================================

// The audio block loop
void inline MainComponent::WT_wave(const AudioSourceChannelInfo& bufferToFill) {
}

// Create a sine wave table
void MainComponent::createSineTable(AudioSampleBuffer& waveTable) {
  waveTable.setSize (1, tableSize + 1);
  waveTable.clear();
  auto* samples = waveTable.getWritePointer (0);
  auto phase = 0.0;
  auto phaseDelta = MathConstants<double>::twoPi / (double) (tableSize - 1);
  for (auto i = 0; i < tableSize; ++i) {
    samples[i] += std::sin(phase);
    phase += phaseDelta;
  }
  samples[tableSize] = samples[0];
}

// Create an inpulse wave table
void MainComponent::createImpulseTable(AudioSampleBuffer& waveTable) {
}

// Create a square wave table
void MainComponent::createSquareTable(AudioSampleBuffer& waveTable) {
}

// Create a sawtooth wave table
void MainComponent::createSawtoothTable(AudioSampleBuffer& waveTable) {
}

// Create a triagle wave table
void MainComponent::createTriangleTable(AudioSampleBuffer& waveTable) {
}
