//==============================================================================

#include <random>
#include "MainComponent.h"
#include "MainApplication.h"

MainComponent::MainComponent() :
waveformId(MainComponent::WaveformId::Empty),
deviceManager(MainApplication::getApp().audioDeviceManager),
audioVisualizer(2) {
    this->setSize(600, 400);
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
    this->levelLabel.setJustificationType(Justification::centredRight);
    this->addAndMakeVisible(this->levelLabel);

    // Set up levelSlider
    this->levelSlider.addListener(this);
    this->levelSlider.setRange(0.0, 1.0);
    this->levelSlider.setSliderStyle(Slider::LinearHorizontal);
    this->levelSlider.setTextBoxStyle(Slider::TextBoxLeft, true,
            this->SLIDER_TEXTBOX_WIDTH, this->COMPONENT_HEIGHT);
    this->levelSlider.setEnabled(false);
    this->addAndMakeVisible(levelSlider);

    // Set up freqLabel
    this->freqLabel.setJustificationType(Justification::centredRight);
    this->addAndMakeVisible(this->freqLabel);

    // Set up freqSlider
    this->freqSlider.addListener(this);
    this->freqSlider.setRange(0.0, 5000.0);
    this->freqSlider.setSliderStyle(Slider::LinearHorizontal);
    this->freqSlider.setTextBoxStyle(Slider::TextBoxLeft, true,
            this->SLIDER_TEXTBOX_WIDTH, this->COMPONENT_HEIGHT);
    this->freqSlider.setSkewFactorFromMidPoint(500.0);
    this->freqSlider.setEnabled(false);
    this->addAndMakeVisible(this->freqSlider);

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
    this->cpuUsage.setJustificationType(Justification::centredRight);
    this->addAndMakeVisible(this->cpuUsage);

    // Let our device manager manage our audio source player
    this->deviceManager.addAudioCallback(&this->audioSourcePlayer);

    // Wavetable initialisation
    this->createSineTable(this->sineTable);
    this->wavetable = std::make_unique<WavetableOscillator>(this->sineTable);
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
    g.fillAll(this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
    Rectangle<int> globalBound = this->getLocalBounds();
    globalBound.removeFromTop(this->PADDING);
    globalBound.removeFromLeft(this->PADDING);
    globalBound.removeFromRight(this->PADDING);

    Rectangle<int> upperBound = globalBound.removeFromTop(this->PLAY_BUTTON_SIZE);
    globalBound.removeFromTop(this->PADDING);
    Rectangle<int> lowerBound = globalBound;

    Rectangle<int> upperLeftBound = upperBound.removeFromLeft(this->SETTINGS_BUTTON_WIDTH);
    this->settingsButton.setBounds(upperLeftBound.removeFromTop(this->COMPONENT_HEIGHT));
    upperLeftBound.removeFromTop(this->PADDING);
    this->waveformMenu.setBounds(upperLeftBound);

    upperBound.removeFromLeft(this->PADDING);
    this->playButton.setBounds(upperBound.removeFromLeft(this->PLAY_BUTTON_SIZE));

    upperBound.removeFromLeft(this->PADDING);
    Rectangle<int> upperLabelBound = upperBound.removeFromLeft(this->SLIDER_LABEL_WIDTH);
    this->levelLabel.setBounds(upperLabelBound.removeFromTop(this->COMPONENT_HEIGHT));
    upperLabelBound.removeFromTop(this->PADDING);
    this->freqLabel.setBounds(upperLabelBound);

    this->levelSlider.setBounds(upperBound.removeFromTop(this->COMPONENT_HEIGHT));
    upperBound.removeFromTop(this->PADDING);
    this->freqSlider.setBounds(upperBound);

    Rectangle<int> cpuBound = lowerBound.removeFromBottom(this->COMPONENT_HEIGHT);
    this->audioVisualizer.setBounds(lowerBound);
    this->cpuUsage.setBounds(cpuBound.removeFromRight(this->CPU_USAGE_WIDTH));
    this->cpuLabel.setBounds(cpuBound.removeFromRight(this->CPU_LABEL_WIDTH));
}

void MainComponent::drawPlayButton(bool showPlay) {
    juce::Path path;
    if (showPlay) {
        path.addTriangle(0,0,0,100,86.6,50);
    } else {
        path.addRectangle(0,0,42,100);
        path.addRectangle(58,0,42,100);
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
        this->isPlaying() ? this->stopTimer() : this->startTimer(1000);
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
    ss << std::fixed << std::setprecision(2)
       << this->deviceManager.getCpuUsage() << " %" << std::endl;
    this->cpuUsage.setText(ss.str(), NotificationType::dontSendNotification);
}

//==============================================================================
// AudioSource overrides
//==============================================================================

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sr) {
    this->phase = 0.0;
    this->sampleRate = sr;
    this->audioVisualizer.setBufferSize(samplesPerBlockExpected);
    this->audioVisualizer.setSamplesPerBlock(8);
    this->phaseDelta = this->freqSlider.getValue() / this->sampleRate;
//    this->WAVE_TABLE_SIZE = samplesPerBlockExpected;
//    this->createWaveTables();
}

void MainComponent::releaseResources() {}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
    this->phaseDelta = this->freqSlider.getValue() / this->sampleRate;
    bufferToFill.clearActiveBufferRegion();
    switch (this->waveformId) {
        case WhiteNoise:      whiteNoise(bufferToFill);      break;
        case DustNoise:       dust(bufferToFill);            break;
        case BrownNoise:      brownNoise(bufferToFill);      break;
        case SineWave:        sineWave(bufferToFill);        break;
        case LF_ImpulseWave:
        case LF_SquareWave:
        case LF_SawtoothWave:
        case LF_TriangeWave:  LF_WaveGenerator(bufferToFill); break;
        case BL_ImpulseWave:  BL_sineWaveAdder(bufferToFill, false, 0); break;
        case BL_SquareWave:   BL_sineWaveAdder(bufferToFill, true, 1); break;
        case BL_SawtoothWave: BL_sineWaveAdder(bufferToFill, false, 1); break;
        case BL_TriangeWave:  BL_sineWaveAdder(bufferToFill, true, 2); break;
        case WT_SineWave:
        case WT_ImpulseWave:
        case WT_SquareWave:
        case WT_SawtoothWave:
        case WT_TriangleWave: WT_wave(bufferToFill); break;
        case Empty: break;
    }
    audioVisualizer.pushBuffer(bufferToFill);
}

//==============================================================================
// Audio Utilities
//==============================================================================

double MainComponent::getNextPhaseAndUpdate() {
    double prevPhase = this->phase;
    this->phase = std::fmod(this->phase + this->phaseDelta, 1.0);
    return prevPhase;
}

float MainComponent::randomGenerator(bool fromZero, float number) {
    assert(number >= 0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(fromZero ? 0 : -number, number);
    return dis(gen);
}

float MainComponent::lowPassFilter(float value, float previousOutput, float alpha) {
    return previousOutput + (alpha * (value - previousOutput));
}

bool MainComponent::isPlaying() {
    return this->audioSourcePlayer.getCurrentSource() != nullptr;
}

void MainComponent::openAudioSettings() {
    std::unique_ptr<juce::AudioDeviceSelectorComponent> audioSettingsPanel =
            std::make_unique<juce::AudioDeviceSelectorComponent>(this->deviceManager,
                    0, 2, 0, 2,
                    true, false,
                    true, false);
    audioSettingsPanel->setSize(500, 270);
    juce::DialogWindow::LaunchOptions dialogWindow;
    dialogWindow.useNativeTitleBar = true;
    dialogWindow.resizable = false;
    dialogWindow.dialogTitle = "Audio Settings";
    dialogWindow.dialogBackgroundColour = Colours::black;
    dialogWindow.content.setOwned(audioSettingsPanel.release());
    dialogWindow.launchAsync();
}

//==============================================================================
// Noise
//==============================================================================

void MainComponent::whiteNoise(const AudioSourceChannelInfo& bufferToFill) {
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        for (int i = 0; i < bufferToFill.numSamples; ++i) {
            channelData[i] = MainComponent::randomGenerator(false, (float) this->levelSlider.getValue());
        }
    }
}

void MainComponent::dust(const AudioSourceChannelInfo& bufferToFill) {
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        for (int i = 0; i < bufferToFill.numSamples; ++i) {
            double probability = (2 * this->freqSlider.getValue()) / this->sampleRate;
            if (MainComponent::randomGenerator(true, 1.0) <= probability) {
                channelData[i] = (float) this->levelSlider.getValue();
            }
        }
    }
}

void MainComponent::brownNoise(const AudioSourceChannelInfo& bufferToFill) {
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        channelData[0] = MainComponent::randomGenerator(false, (float) this->levelSlider.getValue());
        for (int i = 1; i < bufferToFill.numSamples; ++i) {
            channelData[i] = MainComponent::lowPassFilter(channelData[i - 1],
                MainComponent::randomGenerator(false,(float) this->levelSlider.getValue()),
                (float) this->levelSlider.getValue());
        }
    }
}

//==============================================================================
// Sine Wave
//==============================================================================

void MainComponent::sineWave(const AudioSourceChannelInfo& bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    for (int i = 0; i < bufferToFill.numSamples; ++i) {
        firstChannelData[i] = (float) (std::sin(2 * M_PI * this->getNextPhaseAndUpdate()) * this->levelSlider.getValue());
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

//==============================================================================
// Low Frequency Waveforms
//==============================================================================

void MainComponent::LF_WaveGenerator(const AudioSourceChannelInfo &bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    for (int i = 0; i < bufferToFill.numSamples; ++i) {
        switch (this->waveformId) {
            case LF_ImpulseWave:  firstChannelData[i] = this->LF_impulseWave();  break;
            case LF_SquareWave:   firstChannelData[i] = this->LF_squareWave();   break;
            case LF_SawtoothWave: firstChannelData[i] = this->LF_sawtoothWave(); break;
            case LF_TriangeWave:  firstChannelData[i] = this->LF_triangleWave(); break;
            default:
                std::cerr << "Error! Wrong function call! \n"
                          << "Other function instead of low frequency waves called the function."
                          << std::endl; break;
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

float MainComponent::LF_impulseWave() {
    double currentPhase = this->getNextPhaseAndUpdate();
    double nextPhase = this->phase;
    auto level =  (float) this->levelSlider.getValue();
    return currentPhase >= nextPhase ? (float) level : 0.0f;
}

float MainComponent::LF_squareWave() {
    auto level = (float) this->levelSlider.getValue();
    return this->getNextPhaseAndUpdate() <= 0.5 ? -level : level;
}

float MainComponent::LF_sawtoothWave() {
    return (float) ((this->getNextPhaseAndUpdate() * 2 - 1) * this->levelSlider.getValue());
}

float MainComponent::LF_triangleWave() {
    double p = this->getNextPhaseAndUpdate();
    return (float) ((p <= 0.5 ? 4 * p - 1 : 3 - 4 * p) * this->levelSlider.getValue());
}

//==============================================================================
// Band Limited Waveforms
//==============================================================================

void MainComponent::BL_sineWaveAdder(const AudioSourceChannelInfo &bufferToFill, bool oddHarmonics, int amplitudeFactor) {
    const double baseFrequency = this->freqSlider.getValue();
    if (baseFrequency < 20.0) { return; }
    const double initialPhase = this->phase;
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    if (baseFrequency >= 1.0) {
        for (int h = 1; h * baseFrequency <= sampleRate / 2; oddHarmonics ? h += 2 : h++) {
            this->phase = initialPhase;
            double amp = this->levelSlider.getValue() / std::pow(h, amplitudeFactor);
            for (int i = 0; i < bufferToFill.numSamples; ++i) {
                firstChannelData[i] += (float) (std::sin(TWO_PI * getNextPhaseAndUpdate() * h) * amp);
            }
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

//==============================================================================
// WaveTable Synthesis
//==============================================================================

void inline MainComponent::WT_wave(const AudioSourceChannelInfo& bufferToFill) {
    switch (this->waveformId) {
        case WT_SineWave:     WT_Sine(bufferToFill);     break;
        case WT_ImpulseWave:  WT_Impulse(bufferToFill);  break;
        case WT_SquareWave:   WT_Square(bufferToFill);   break;
        case WT_SawtoothWave: WT_Saw(bufferToFill);      break;
        case WT_TriangleWave: WT_Triangle(bufferToFill); break;
        default:
            std::cerr << "ERROR! Wrong function called wavetable generator."
                      << std::endl;
    }
}

void MainComponent::WT_Sine(const AudioSourceChannelInfo& bufferToFill) {
    this->wavetable->setFrequency((float) this->freqSlider.getValue(), (float) this->sampleRate);
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    for (int i = 0; i < bufferToFill.numSamples; i++) {
        firstChannelData[i] = (float) (this->wavetable->getNextSample() * this->levelSlider.getValue());
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

void MainComponent::WT_Impulse(const AudioSourceChannelInfo& bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    const double baseFrequency = this->freqSlider.getValue();
    if (baseFrequency < 20.0) { return; }
    for (int h = 1; h * baseFrequency <= sampleRate / 2; ++h) {
        this->wavetable->setFrequency((float) (h * baseFrequency), (float) this->sampleRate);
        for (int i = 0; i < bufferToFill.numSamples; i++) {
            firstChannelData[i] += (float) (this->wavetable->getNextSample() * this->levelSlider.getValue());
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

void MainComponent::WT_Square(const AudioSourceChannelInfo& bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    const double baseFrequency = this->freqSlider.getValue();
    if (baseFrequency < 20.0) { return; }
    for (int h = 1; h * baseFrequency <= sampleRate / 2; h += 2) {
        this->wavetable->setFrequency((float) (h * baseFrequency), (float) this->sampleRate);
        for (int i = 0; i < bufferToFill.numSamples; i++) {
            firstChannelData[i] += (float) (this->wavetable->getNextSample() * this->levelSlider.getValue() / h);
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

void MainComponent::WT_Saw(const AudioSourceChannelInfo& bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    const double baseFrequency = this->freqSlider.getValue();
    if (baseFrequency < 20.0) { return; }
    for (int h = 1; h * baseFrequency <= sampleRate / 2; ++h) {
        this->wavetable->setFrequency((float) (h * baseFrequency), (float) this->sampleRate);
        for (int i = 0; i < bufferToFill.numSamples; i++) {
            firstChannelData[i] += (float) (this->wavetable->getNextSample() * this->levelSlider.getValue() / h);
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

void MainComponent::WT_Triangle(const AudioSourceChannelInfo& bufferToFill) {
    float* firstChannelData = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    const double baseFrequency = this->freqSlider.getValue();
    if (baseFrequency < 20.0) { return; }
    const double level = this->levelSlider.getValue();
    for (int h = 1; h * baseFrequency <= sampleRate / 2; h += 2) {
        this->wavetable->setFrequency((float) (h * baseFrequency), (float) this->sampleRate);
        for (int i = 0; i < bufferToFill.numSamples; i++) {
            firstChannelData[i] += (float) (this->wavetable->getNextSample() * level / std::pow(h, 2));
        }
    }
    for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        std::memcpy(channelData, firstChannelData, bufferToFill.numSamples * sizeof(float));
    }
}

// Create a sine wave table
void MainComponent::createSineTable(AudioSampleBuffer& waveTable) {
    waveTable.setSize(1, this->WAVE_TABLE_SIZE + 1);
    waveTable.clear();
    float* samples = waveTable.getWritePointer(0);
    double p = 0.0;
    double d = TWO_PI / (double) (this->WAVE_TABLE_SIZE - 1);
    for (auto i = 0; i < this->WAVE_TABLE_SIZE; ++i) {
        samples[i] += (float) std::sin(p);
        p += d;
    }
    samples[this->WAVE_TABLE_SIZE] = samples[0];
}

//// Create an inpulse wave table
//void MainComponent::createImpulseTable(AudioSampleBuffer& waveTable) {
//    waveTable.setSize(1, this->tableSize + 1);
//    waveTable.clear();
//    float* samples = waveTable.getWritePointer(0);
//    samples[1] = 1.0;
//    samples[this->tableSize] = samples[0];
//}
//
//// Create a square wave table
//void MainComponent::createSquareTable(AudioSampleBuffer& waveTable) {
//    waveTable.setSize(1, this->tableSize + 1);
//    waveTable.clear();
//    float* samples = waveTable.getWritePointer(0);
//    double p = 0.0;
//    auto d = TWO_PI / (double) (this->tableSize - 1);
//    for (auto i = 0; i < this->tableSize; ++i) {
//        samples[i] += p >= 0.5 ? 1 : -1;
//        p += d;
//    }
//    samples[this->tableSize] = samples[0];
//}
//
//// Create a sawtooth wave table
//void MainComponent::createSawtoothTable(AudioSampleBuffer& waveTable) {
//    waveTable.setSize(1, this->tableSize + 1);
//    waveTable.clear();
//    float* samples = waveTable.getWritePointer(0);
//    double p = 0.0;
//    auto d = TWO_PI / (double) (this->tableSize - 1);
//    for (auto i = 0; i < this->tableSize; ++i) {
//        samples[i] += (float) (p * 2 - 1);
//        p += d;
//    }
//    samples[this->tableSize] = samples[0];
//}
//
//// Create a triangle wave table
//void MainComponent::createTriangleTable(AudioSampleBuffer& waveTable) {
//    waveTable.setSize(1, this->tableSize + 1);
//    waveTable.clear();
//    float* samples = waveTable.getWritePointer(0);
//    double p = 0.0;
//    auto d = TWO_PI / (double) (this->tableSize - 1);
//    for (auto i = 0; i < this->tableSize; ++i) {
//        samples[i] += (float) (p <= 0.5 ? 4 * p - 1 : 3 - 4 * p);
//        p += d;
//    }
//    samples[this->tableSize] = samples[0];
//}