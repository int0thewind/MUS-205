//==============================================================================

#include <random>
#include "MainComponent.h"
#include "MainApplication.h"

MainComponent::MainComponent() :
waveformId(MainComponent::WaveformId::Empty),
deviceManager(MainApplication::getApp().audioDeviceManager),
audioVisualizer(2) {
    // Set up audioVisualizer
    this->addAndMakeVisible(this->audioVisualizer);

    // Set up settingsButton
    this->settingsButton.addListener(this);
    this->addAndMakeVisible(this->settingsButton);

    // Set up playButton
    this->playButton.addListener(this);
    this->drawPlayButton(true);
    this->playButton.setEnabled(false);
    this->addAndMakeVisible(this->playButton);

    // Set up levelLabel
    this->addAndMakeVisible(this->levelLabel);

    // Set up levelSlider
    this->levelSlider.addListener(this);
    this->levelSlider.setRange(0.0, 1.0);
    this->levelSlider.setSliderStyle(Slider::LinearHorizontal);
    this->levelSlider.setTextBoxStyle(Slider::TextBoxLeft, true,
            90, 22);
    this->addAndMakeVisible(levelSlider);
    this->levelSlider.setEnabled(false);

    // Set up freqLabel
    this->addAndMakeVisible(this->freqLabel);

    // Set up freqSlider
    this->freqSlider.addListener(this);
    this->freqSlider.setRange(0.0, 5000.0);
    this->freqSlider.setSliderStyle(Slider::LinearHorizontal);
    this->freqSlider.setTextBoxStyle(Slider::TextBoxLeft, true,
            90, 22);
    this->freqSlider.setSkewFactorFromMidPoint(500.0);
    this->freqSlider.setEnabled(false);

    // Set up waveformLabel
    this->addAndMakeVisible(this->waveformLabel);

    // Set up waveformMenu
    this->waveformMenu.addListener(this);
    this->waveformMenu.addItemList(this->MENU_FIRST_SEC, MainComponent::WaveformId::WhiteNoise);
    this->waveformMenu.addSeparator();
    this->waveformMenu.addItemList(this->MENU_SECOND_SEC, MainComponent::WaveformId::SineWave);
    this->waveformMenu.addSeparator();
    this->waveformMenu.addItemList(this->MENU_THIRD_SEC, MainComponent::WaveformId::LF_ImpulseWave);
    this->waveformMenu.addSeparator();
    this->waveformMenu.addItemList(this->MENU_FOURTH_SEC, MainComponent::WaveformId::BL_ImpulseWave);
    this->waveformMenu.addSeparator();
    this->waveformMenu.addItemList(this->MENU_FIFTH_SEC, MainComponent::WaveformId::WT_SineWave);
    this->waveformMenu.setTextWhenNothingSelected("Waveforms");
    this->addAndMakeVisible(this->waveformMenu);

    // Set up cpuLabel
    this->cpuLabel.setJustificationType(Justification::centredRight);
    this->addAndMakeVisible(this->cpuLabel);

    // Set up cpuUsage
    this->cpuUsage.setJustificationType(Justification::right);
    this->addAndMakeVisible(this->cpuUsage);

    // Let our device manager manage our audio source player
    this->deviceManager.addAudioCallback(&this->audioSourcePlayer);
}

MainComponent::~MainComponent() {
    this->audioSourcePlayer.setSource(nullptr);
    this->deviceManager.removeAudioCallback(&this->audioSourcePlayer);
    this->deviceManager.closeAudioDevice();
}

//==============================================================================
// Component overrides
//==============================================================================

void MainComponent::paint (Graphics& g) {
    // TODO get fill colour
    g.fillAll();
}

void MainComponent::resized() {
    Rectangle<int> globalBound = this->getLocalBounds();
    globalBound.removeFromTop(8);
    globalBound.removeFromLeft(8);
    globalBound.removeFromRight(8);
    
}

void MainComponent::drawPlayButton(bool showPlay) {
    juce::Path path;
    if (showPlay) {
        path.addTriangle(0,0,0,100,86.6,50);
    } else {
        path.addRectangle(0,0,46,100);
        path.addRectangle(54,0,46,100);
    }
    juce::DrawablePath drawable;
    drawable.setPath(path);
    juce::FillType fill(Colours::white);
    drawable.setFill(fill);
    this->playButton.setImages(&drawable);
}

//==============================================================================
// Listener overrides
//==============================================================================

void MainComponent::buttonClicked (Button *button) {
    if (button == &this->playButton) {
        this->drawPlayButton(this->isPlaying());
        this->audioSourcePlayer.setSource(this->isPlaying() ? nullptr : this);
    } else if (button == &this->settingsButton) {
        this->openAudioSettings();
    } else {
        std::cerr << "No button has been selected but a button has been clicked. \n"
                  << "The address of the button is "
                  << button
                  << std::endl;
    }
}

void MainComponent::sliderValueChanged (Slider *slider) {
    if (slider != &this->freqSlider && slider != &this->levelSlider) {
        std::cerr << "No slider has been selected but a slider has been dragged. \n"
                  << "The address of the slider is "
                  << slider
                  << std::endl;
    }
}

void MainComponent::comboBoxChanged (ComboBox *menu) {
    if (menu == &this->waveformMenu) {
        this->waveformId = static_cast<WaveformId>(waveformMenu.getSelectedId());
        this->levelSlider.setEnabled(true);
        this->freqSlider.setEnabled(!(this->waveformId == MainComponent::WaveformId::WhiteNoise ||
                                      this->waveformId == MainComponent::WaveformId::BrownNoise));
        this->playButton.setEnabled(true);
    } else {
        std::cerr << "No menu has been selected but a menu has been clicked. \n"
            << "The address of the menu is "
            << menu
            << std::endl;
    }
}

//==============================================================================
// Timer overrides
//==============================================================================

void MainComponent::timerCallback() {
    std::stringstream ss;
    ss << this->deviceManager.getCpuUsage() << " %" << std::endl;
    this->cpuUsage.setText(ss.str(), NotificationType::dontSendNotification);
}

//==============================================================================
// AudioSource overrides
//==============================================================================

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sr) {
    this->sampleRate = sr;
    this->audioVisualizer.setNumChannels(8);
    this->audioVisualizer.setBufferSize(samplesPerBlockExpected);
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

double MainComponent::getAndUpdatePhase(double p) {
    p += this->freqSlider.getValue() / this->sampleRate;
    return std::fmod(p, 1.0);
}

float MainComponent::randomGenerater(bool fromZero, float number=1.0) {
    assert(number >= 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(fromZero ? 0 : -number, number);
    return dis(gen);
}

float MainComponent::lowPass(const float value, const float prevout, const float alpha) {
    return 0.0;
}

bool MainComponent::isPlaying() {
    return this->audioSourcePlayer.getCurrentSource() != nullptr;
}

void MainComponent::openAudioSettings() {
    // TODO how to add audio settings panel?
    auto audioSettingsPanel = nullptr;
    juce::DialogWindow::LaunchOptions dialogWindow;
    dialogWindow.useNativeTitleBar = true;
    dialogWindow.resizable = false;
    dialogWindow.dialogTitle = "Audio Settings";
    dialogWindow.dialogBackgroundColour = Colours::black;
//    dialogWindow.content.setOwned(audioSettingsPanel.release());
    dialogWindow.launchAsync();
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
