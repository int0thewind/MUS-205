#include "Transport.h"

/// Constructor.
Transport::Transport(const MediaManagerData& mmd) {
    this->mediaManagerData = mmd;
    this->mediaManagerData.addListener(this);

    this->drawPlayPauseButton();
    this->playPauseButton.setClickingTogglesState(true);
    this->playPauseButton.addListener(this);
    this->addAndMakeVisible(this->playPauseButton);

    this->drawGoToStartButton();
    this->goToStartButton.addListener(this);
    this->addAndMakeVisible(this->goToStartButton);

    double gain = this->mediaManagerData.getTransportGain();
    this->drawGainButton(gain);
    this->gainButton.addListener(this);
    this->addAndMakeVisible(this->gainButton);

    this->gainSlider.setRange(0.0, 1.0);
    this->gainSlider.setValue(gain);
    this->gainSlider.addListener(this);
    this->addAndMakeVisible(this->gainSlider);

    this->tempoSlider.setSliderStyle(Slider::LinearBar);
    this->tempoSlider.setTextValueSuffix("bpm");
    this->tempoSlider.setRange(40, 208);
    this->tempoSlider.setValue(this->mediaManagerData.getTransportTempo());
    this->tempoSlider.setNumDecimalPlacesToDisplay(1);
    this->addAndMakeVisible(this->tempoSlider);
    this->tempoSlider.addListener(this);

    this->positionSlider.setRange(0.0, 1.0);
    this->positionSlider.setValue(0.0);
    this->positionSlider.addListener(this);
    this->addAndMakeVisible(this->positionSlider);

    this->currentTimeLabel.setJustificationType(Justification::centredRight);
    this->currentTimeLabel.setFont(12.0);
    this->currentTimeLabel.setText("00:00", NotificationType::dontSendNotification);
    this->addAndMakeVisible(this->currentTimeLabel);

    this->endTimeLabel.setJustificationType(Justification::centredLeft);
    this->endTimeLabel.setFont(12.0);
    this->endTimeLabel.setText("00:00", NotificationType::dontSendNotification);
    this->addAndMakeVisible(this->endTimeLabel);

    this->setSize(250, 78);
    this->setVisible(true);
}

/// Destructor.
Transport::~Transport() = default;

void Transport::paint(Graphics& gr) {
    gr.setColour(Colours::white);
    gr.drawRoundedRectangle(this->getLocalBounds().toFloat(), 8, 2);
}

void Transport::resized() {
    Rectangle<int> globalBound = this->getLocalBounds();
    globalBound.removeFromRight(6);
    globalBound.removeFromLeft(6);
    globalBound.removeFromTop(6);
    globalBound.removeFromBottom(6);

    Rectangle<int> firstLine = globalBound.removeFromTop(36);
    Rectangle<int> secondLine = globalBound.removeFromBottom(24);

    int sliderWidth = (firstLine.getWidth() - 36 - 24 - 24 - 6 - 6) / 2;
    Rectangle<int> tempoSliderBound = firstLine.removeFromLeft(sliderWidth)
            .removeFromBottom(30).removeFromTop(24);
    this->tempoSlider.setBounds(tempoSliderBound);
    Rectangle<int> gainSliderBound = firstLine.removeFromRight(sliderWidth)
            .removeFromBottom(30).removeFromTop(24);
    this->gainSlider.setBounds(gainSliderBound);
    firstLine.removeFromLeft(6);
    firstLine.removeFromRight(6);
    Rectangle<int> goToStartButtonBound = firstLine.removeFromLeft(24)
            .removeFromBottom(30).removeFromTop(24);
    this->goToStartButton.setBounds(goToStartButtonBound);
    Rectangle<int> gainButtonBounds = firstLine.removeFromRight(24)
            .removeFromBottom(30).removeFromTop(24);
    this->gainButton.setBounds(gainButtonBounds);
    this->playPauseButton.setBounds(firstLine);

    this->currentTimeLabel.setBounds(secondLine.removeFromLeft(44));
    this->endTimeLabel.setBounds(secondLine.removeFromRight(44));
    this->positionSlider.setBounds(secondLine);
}

//============================================================================
// JUCE Component Callbacks

void Transport::buttonClicked(juce::Button* button) {
    if (button == &this->playPauseButton) {
        bool isPlaying = this->mediaManagerData.getTransportPlaying();
        this->mediaManagerData.setTransportPlaying(!isPlaying, this);
    } else if (button == &this->goToStartButton) {
        this->positionSlider.setValue(0.0, NotificationType::dontSendNotification);
        this->mediaManagerData.setTransportRewind();
    } else if (button == &this->gainButton) {
        double currentGain = mediaManagerData.getTransportGain();
        if (currentGain != 0.0) {
            this->previousGain = currentGain;
            currentGain = 0.0;
        } else {
            currentGain = this->previousGain;
        }
        this->mediaManagerData.setTransportGain(currentGain);
        this->drawGainButton(currentGain);
    }
}

void Transport::sliderValueChanged(juce::Slider* slider) {
    // IMPORTANT! All operations in this methods which change the value tree
    // should pass in the exclude component, or it would trigger the slider change again
    // causing an infinite loop!
    double currentSliderValue = slider->getValue();
    if (slider == &this->tempoSlider) {
        this->mediaManagerData.setTransportTempo(currentSliderValue, this);
    } else if (slider == &this->gainSlider) {
        this->mediaManagerData.setTransportGain(currentSliderValue, this);
        this->drawGainButton(currentSliderValue);
    } else if (slider == &this->positionSlider) {
        this->mediaManagerData.setTransportPosition(currentSliderValue, this);
        this->drawCurrentTimeLabel();
    }
}

//============================================================================
// JUCE ValueTree callbacks (listening to the mediaManagerData changes)

void Transport::valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& ident) {
    if (ident == MediaManagerData::TRANSPORT_ENABLED) {
        this->setEnabled(this->mediaManagerData.getTransportEnabled());
    }
    else if (ident == MediaManagerData::TRANSPORT_TEMPO_ENABLED) {
        this->tempoSlider.setEnabled(this->mediaManagerData.getTransportTempoEnabled());
    }
    else if (ident == MediaManagerData::TRANSPORT_GAIN) {
        this->gainSlider.setValue(this->mediaManagerData.getTransportGain(), NotificationType::dontSendNotification);
    }
    else if (ident == MediaManagerData::TRANSPORT_TEMPO) {
        this->tempoSlider.setValue(this->mediaManagerData.getTransportTempo(), NotificationType::dontSendNotification);
    }
    else if (ident == MediaManagerData::TRANSPORT_CLICK_PLAY_PAUSE) {
        this->playPauseButton.triggerClick();
    }
    else if (ident == MediaManagerData::TRANSPORT_POSITION) {
        this->positionSlider.setValue(this->mediaManagerData.getTransportPosition(), NotificationType::dontSendNotification);
        this->drawCurrentTimeLabel();
    }
    else if (ident == MediaManagerData::TRANSPORT_PLAYBACK_DURATION) {
        this->drawEndTimeLabel();
    }
}

//============================================================================
// Icon Button Drawing

void Transport::drawPlayPauseButton() {
    juce::DrawablePath triangleDrawable, rectangleDrawable;
    juce::Path trianglePath, rectanglePath;

    trianglePath.addTriangle(0,0,0,100,86.6,50);
    rectanglePath.addRectangle(0,0,42,100);
    rectanglePath.addRectangle(58,0,42,100);

    triangleDrawable.setPath(trianglePath);
    triangleDrawable.setFill(this->iconColor);
    rectangleDrawable.setPath(rectanglePath);
    rectangleDrawable.setFill(this->iconColor);

    this->playPauseButton.setImages(&triangleDrawable, nullptr, nullptr, nullptr, &rectangleDrawable);
}

void Transport::drawGoToStartButton() {
    juce::DrawablePath image;
    juce::Path path;

    path.addRectangle(13.4, 0, 13.4, 100);
    path.addTriangle(100, 0, 100, 100, 13.4, 50);

    image.setPath(path);
    image.setFill(this->iconColor);

    this->goToStartButton.setImages(&image);
}

void Transport::drawGainButton(double gain) {
    juce::DrawablePath drawable;
    juce::Path p;
    // Add the shape of speaker, comprising a rectangle and a triangle
    p.addRectangle(0, 30, 30, 35);
    p.addTriangle(0, 50, 40, 0, 40, 100);
    // Add the arcs. Indicating the level of gain.
    if (gain > 0.1)
        p.addCentredArc(55, 50, 6, 20,
                0, 0, 3.14159f, true);
    if (gain > 0.4)
        p.addCentredArc(70, 50, 5, 35,
                0, 0, 3.14159f, true);
    if (gain > 0.7)
        p.addCentredArc(85, 50, 5, 50,
                0, 0, 3.14159f, true);
    p.startNewSubPath(100, 0);
    drawable.setPath(p);
    drawable.setFill(this->iconColor);
    this->gainButton.setImages(&drawable);
}

void Transport::drawCurrentTimeLabel() {
    double currentTime = this->mediaManagerData.getPlaybackDuration() * this->positionSlider.getValue();
    String currentTimeString = Transport::toFormattedTimeString(currentTime);
    this->currentTimeLabel.setText(currentTimeString, NotificationType::sendNotificationAsync);
}

void Transport::drawEndTimeLabel() {
    double endTime = this->mediaManagerData.getPlaybackDuration();
    String endTimeString = Transport::toFormattedTimeString(endTime);
    this->endTimeLabel.setText(endTimeString, NotificationType::sendNotificationAsync);
}

juce::String Transport::toFormattedTimeString(const double seconds) {
    std::stringstream ss;
    int s = juce::roundToIntAccurate(seconds);
    int minute = s / 60;
    int second = s % 60;
    std::string minuteString = std::to_string(minute);
    std::string secondString = std::to_string(second);

    ss << (minuteString.length() == 1 ? "0" : "") << minuteString << ":"
       << (secondString.length() == 1 ? "0" : "") << secondString;

    return ss.str();
}
