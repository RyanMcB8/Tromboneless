/** @file       Widgets.cpp
 *  @author     Ryan McBride
 *  @brief      A file which defines the functions which have been declared
 *              within the `Widgets.hpp` file.
*/

/* Adding the necessary header files. */
#include    "Widgets.hpp"

/* ========================================================================================== */
/*                                                                                            */
/*                                   SliderWithLabel                                          */
/*                                                                                            */
/* ========================================================================================== */

SliderWithLabel::InitialiseLabelErrors_t SliderWithLabel::CreateLabel(LabelPositions_t position,
            juce::String phrase,
            juce::NotificationType NotificationEnable){
                
    switch(position){
        case(UpperLeft):
            labelSetup(&UpperLeftLabel, phrase, NotificationEnable);
            UpperLeftLabel.setJustificationType(juce::Justification::Flags::bottomLeft);
            addAndMakeVisible (UpperLeftLabel);
            break;
        
        case(UpperCentre):
            labelSetup(&UpperCentreLabel, phrase, NotificationEnable);
            UpperCentreLabel.setJustificationType(juce::Justification::Flags::centredBottom);
            addAndMakeVisible (UpperCentreLabel);
            break;

        case(UpperRight):
            labelSetup(&UpperRightLabel, phrase, NotificationEnable);
            UpperRightLabel.setJustificationType(juce::Justification::Flags::bottomRight);
            addAndMakeVisible (UpperRightLabel);
            break;

        case(MiddleLeft):
            labelSetup(&MiddleLeftLabel, phrase, NotificationEnable);
            MiddleLeftLabel.setJustificationType(juce::Justification::Flags::centredLeft);
            addAndMakeVisible (MiddleLeftLabel);
            break;

        case(MiddleRight):
            labelSetup(&MiddleRightLabel, phrase, NotificationEnable);
            MiddleRightLabel.setJustificationType(juce::Justification::Flags::centredRight);
            addAndMakeVisible (MiddleRightLabel);
            break;

        case(LowerLeft):
            labelSetup(&LowerLeftLabel, phrase, NotificationEnable);
            LowerLeftLabel.setJustificationType(juce::Justification::Flags::topLeft);
            addAndMakeVisible (LowerLeftLabel);
            break;
        
        case(LowerCentre):
            labelSetup(&LowerCentreLabel, phrase, NotificationEnable);
            LowerCentreLabel.setJustificationType(juce::Justification::Flags::centredTop);
            addAndMakeVisible (LowerCentreLabel);
            break;

        case(LowerRight):
            labelSetup(&LowerRightLabel, phrase, NotificationEnable);
            LowerRightLabel.setJustificationType(juce::Justification::Flags::topRight);
            addAndMakeVisible (LowerRightLabel);
            break;

        default:
            return PositonNotAccepted;                
    }
    
    
    return LabelOK;
}

void SliderWithLabel::resized()
{
    /* Finding the local bounds for the entire object. */
    juce::Rectangle<int> area = getLocalBounds();
    
    /* Initialisation of the bounds for each section. */
    juce::Rectangle<int> TopLabelBounds;
    juce::Rectangle<int> CentreLabelBounds;
    juce::Rectangle<int> BottomLabelBounds;

    /* Top labels. */
    TopLabelBounds = area.removeFromTop(std::min(area.getHeight()*0.3, (double) topLabelBounds));
    juce::Rectangle<int> LeftLabelBounds = TopLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    UpperLeftLabel.setBounds(LeftLabelBounds);

    juce::Rectangle<int> RightLabelBounds = TopLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    UpperRightLabel.setBounds(RightLabelBounds);
    
    UpperCentreLabel.setBounds(TopLabelBounds);

    /* Bottom labels. */
    BottomLabelBounds = area.removeFromBottom(std::min(area.getHeight()*0.5, (double) bottomLabelBounds));
    LeftLabelBounds = BottomLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    LowerLeftLabel.setBounds(LeftLabelBounds);

    RightLabelBounds = BottomLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    LowerRightLabel.setBounds(RightLabelBounds);
    
    LowerCentreLabel.setBounds(BottomLabelBounds);

    /* Middle labels. */
    CentreLabelBounds = area;
    LeftLabelBounds = CentreLabelBounds.removeFromLeft(std::min(area.getWidth()*0.2, (double) leftLabelBounds));
    MiddleLeftLabel.setBounds(LeftLabelBounds);

    RightLabelBounds = CentreLabelBounds.removeFromRight(std::min(area.getWidth()*0.2, (double) rightLabelBounds));
    MiddleRightLabel.setBounds(RightLabelBounds);


    /* Adding slider */
    slider.setBounds(area);
}

void SliderWithLabel::labelSetup(juce::Label* Label, juce::String phrase,
                juce::NotificationType NotificationEnable){
    Label->setText(phrase, NotificationEnable);
    Label->setEditable(false);
    addAndMakeVisible (Label);
}


void SliderWithLabel::setTopLabelBounds(int bound){
    topLabelBounds = bound;
    return;
}

int SliderWithLabel::getTopLabelBounds(void){
    return topLabelBounds;
}

void SliderWithLabel::setLeftLabelBounds(int bound){
    leftLabelBounds = bound;
    return;
}

int SliderWithLabel::getLeftLabelBounds(void){
    return leftLabelBounds;
}

void SliderWithLabel::setRightLabelBounds(int bound){
    rightLabelBounds = bound;
    return;
}

int SliderWithLabel::getRightLabelBounds(void){
    return rightLabelBounds;
}

void SliderWithLabel::setBottomLabelBounds(int bound){
    bottomLabelBounds = bound;
    return;
}

int SliderWithLabel::getBottomLabelBounds(void){
    return bottomLabelBounds;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrationSlider                                        */
/*                                                                                            */
/* ========================================================================================== */

 CalibrationSlider::CalibrationSlider() :   SliderWithLabel(juce::Slider::TwoValueHorizontal){
    this->slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    this->slider.addListener(this);
}

CalibrationSlider::~CalibrationSlider(){
    this->slider.removeListener(this);
}

 void CalibrationSlider::setMinDifference(float difference){
     minDistance = std::min((float) abs(difference), (float) (slider.getMaxValue() - slider.getMinValue()));
     return;
 }

float CalibrationSlider::getMinDifference(void){
    return minDistance;
}

 void CalibrationSlider::sliderValueChanged(juce::Slider* sliderChanged) 
 {
     auto min = sliderChanged->getMinValue();
     auto max = sliderChanged->getMaxValue();
 
     /* Checking if the values are out of range of each other. */
     if (max - min < minDistance)
     {   
         /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
         if (max >= sliderChanged->getMaximum()){
             max = sliderChanged->getMaximum();
             sliderChanged->setMaxValue(max);
             sliderChanged->setMinValue(max - minDistance);
             return;
         }
 
         /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
         else if (min <= sliderChanged->getMinimum()){
             min = sliderChanged->getMinimum();
             sliderChanged->setMinValue(min);
             sliderChanged->setMaxValue(min + minDistance);
             return;
         }
         
         /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
         else if (sliderChanged->getThumbBeingDragged() == 1){
             sliderChanged->setMaxValue(min + minDistance);
             return;
         }
 
         else{
             sliderChanged->setMinValue(max - minDistance);
             return;
         }
     }
     return;
 }

/* ========================================================================================== */
/*                                                                                            */
/*                                      DualRotarySlider                                      */
/*                                                                                            */
/* ========================================================================================== */

DualRotarySlider::DualRotarySlider(){
    /* Setting the styles of the rotary sliders.*/
    minSlider.setSliderStyle(juce::Slider::Rotary);
    maxSlider.setSliderStyle(juce::Slider::Rotary);

    /* Adding limits to the slider.*/
    minSlider.setRotaryParameters(minAngle, (maxAngle - minAngleDifference), true);
    maxSlider.setRotaryParameters((minAngle + minAngleDifference), maxAngle, true);

    /* Removing the textbox from the sliders. */
    minSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    maxSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    /* Setting the initial values of the sliders. */
    minSlider.setValue(std::max((float)(0.2*maxLimit), (float)minLimit));
    maxSlider.setValue(std::max((float) (0.75*maxLimit), (float)(minLimit + minDifference)));

    /* Setting the range the sliders can reach. */
    minSlider.setRange(minLimit, (maxLimit - minDifference), interval);
    maxSlider.setRange((minLimit + minDifference), maxLimit, interval);

    /* Adding a listener to both the sliders. */
    minSlider.addListener(this);
    maxSlider.addListener(this);

    /* Showing the slider. */
    addAndMakeVisible (maxSlider);
    addAndMakeVisible (minSlider);
    addAndMakeVisible (rotaryLabel);
}

DualRotarySlider::~DualRotarySlider(){
    minSlider.removeListener(this);
    maxSlider.removeListener(this);

}


 void DualRotarySlider::sliderValueChanged(juce::Slider* sliderChanged) 
 {
    float min = minSlider.getValue();
    float max = maxSlider.getValue();
 
    /* Checking if the values are out of range of each other. */
    if (max - min < minDifference)
    {   
        /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
        if (max >= maxLimit){
            max = maxLimit;
            maxSlider.setValue(max);
            minSlider.setValue(max - minDifference);
            return;
        }

        /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
        else if (min <= minLimit){
            min = minLimit;
            minSlider.setValue(min);
            maxSlider.setValue(min + minDifference);
            return;
        }
        
        /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
        else if (sliderChanged == &minSlider){
            maxSlider.setValue(min + minDifference);
            return;
        }

        else{
            minSlider.setValue(max - minDifference);
            return;
        }
    }
    return;
}

void DualRotarySlider::resized()
{
    /* Finding the local bounds for the entire object. */
    juce::Rectangle<int> totalArea = getLocalBounds();
    labelHeight = std::max((float)(totalArea.getHeight()*0.1), (float)50);
    
    juce::Rectangle<int> labelBound = totalArea.removeFromBottom(labelHeight);
    rotaryLabel.setBounds(labelBound);
    
    float bounds = std::min(totalArea.getWidth(), totalArea.getHeight());
    maxSlider.setBounds(totalArea.withSizeKeepingCentre(bounds*maxSliderRadius, bounds*maxSliderRadius));
    minSlider.setBounds(totalArea.withSizeKeepingCentre(bounds*minSliderRadius, bounds*minSliderRadius));
    
}

void DualRotarySlider::setMinDifference(float difference){
    
    /*  Instantly limit the differece to be a maximum of 80% and minimum of 0%. */
    minDifference = std::min((float)((maxLimit-minLimit) * 0.8), (float)(abs(difference)));
    updateMinAngleDifference();
    return;
}

float DualRotarySlider::getMinDifference(void){
    return minDifference;
}

void DualRotarySlider::setMinSliderRadius(float radius){
    minSliderRadius = radius;
    return;
}

float DualRotarySlider::getMinSliderRadius(void){
    return minSliderRadius; 
}

void DualRotarySlider::setMaxSliderRadius(float radius){
    maxSliderRadius = radius;
    return;
}

float DualRotarySlider::getMaxSliderRadius(void){
    return maxSliderRadius;
}

void  DualRotarySlider::updateMinAngleDifference(void){
    minAngleDifference = (maxAngle - minAngle)*(minDifference/(maxLimit - minLimit));
}

void DualRotarySlider::setLabelHeight(float height){
    labelHeight = height;
    return;
}

float DualRotarySlider::getLabelHeight(void){
    return labelHeight;
}

void DualRotarySlider::setMinLimit(float limit){
    minLimit = std::min(limit, getMaxLimit()-minDifference);
    return;
}

float DualRotarySlider::getMinLimit(void){
    return minLimit;
}

void DualRotarySlider::setMaxLimit(float limit){
    maxLimit = std::max(limit, getMinLimit()+minDifference);;
    return;
}

float DualRotarySlider::getMaxLimit(void){
    return maxLimit;
}

void DualRotarySlider::setInterval(float step){
    interval = std::min(step, (maxLimit - minLimit)/2);
    return;
}

float DualRotarySlider::getInterval(void){
    return interval;
}

void DualRotarySlider::setMinAngle(float angle){
    minAngle = std::min(angle, getMaxAngle() - minAngleDifference);
    return;
}

float DualRotarySlider::getMinAngle(void){
    return minAngle;
}

void DualRotarySlider::setMaxAngle(float angle){
    maxAngle = std::max(angle, getMinAngle() + minAngleDifference);
    return;
}

float DualRotarySlider::getMaxAngle(void){
    return maxAngle;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                          Barometer                                         */
/*                                                                                            */
/* ========================================================================================== */

Barometer::Barometer(){
    minSlider.addListener(this);
    maxSlider.addListener(this);

    /* Applying the styles. */
    minSlider.setLookAndFeel(&arrow);
    maxSlider.setLookAndFeel(&arrow);

    /* Adding the label */
    rotaryLabel.setText((const juce::String)"Relative pressure calibration\n for mouthpiece",
                        juce::dontSendNotification);
    rotaryLabel.setJustificationType(juce::Justification::Flags::centred);
    addAndMakeVisible(rotaryLabel);

}

Barometer::~Barometer(){
    minSlider.removeListener(this);
    maxSlider.removeListener(this);

    /* Removing the styles. */
    minSlider.setLookAndFeel(nullptr);
    maxSlider.setLookAndFeel(nullptr);
}

void Barometer::paint (juce::Graphics& g){
    /* Clearing any paint from before. */
    // g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); 

    juce::Rectangle <int> area = getLocalBounds();
    juce::Rectangle <float> outlineRectangle = area.toFloat();
    g.setColour(edgeColour);
    g.fillRoundedRectangle(outlineRectangle , (float) 20);

    /* Not drawing over the area of where the label is. */
    juce::Rectangle<int> labelArea = area.removeFromBottom(getLabelHeight());

    /* Adding a margin around the box.*/
    area = area.withSizeKeepingCentre((float)(area.getWidth()*0.95), (float)(area.getHeight()*0.95));

    /* Finding the radius to draw. */
    float radius = std::min(area.getWidth(), area.getHeight());
    
    float X = area.getX();
    float Y = area.getY();

    float difference =  abs(area.getWidth() - area.getHeight());
    if (area.getWidth() > area.getHeight()){
        X = area.getX() + difference/2;
    }
    else if (area.getWidth() < area.getHeight()){
        Y = area.getY() + difference/2;
    }

    /* Defining the colour to draw in and drawing shapes. */
    g.setColour(backgroundColour);
    g.fillEllipse(X, Y, radius, radius);

    /* Adding a boarder colour around it. */
    g.setColour(boarderColour);
    float thickness = 5;
    g.drawEllipse(X, Y, radius-(thickness/2), radius-(thickness/2), thickness);

    /* Adding notches to the background. */
    float centreX = area.getCentreX();
    float centreY = area.getCentreY();

    g.setColour(textColour);
    int nNotches = (getMaxLimit() - getMinLimit())/getInterval();
    int counter = (int) (getMinLimit()*10);
    float textWidth = 20;
    float textHeight = 15;
    for (int i = 0; i <= nNotches; i++)
    {
        juce::Path rotated;
        rotated.startNewSubPath(centreX, centreY - (radius*relativeOuterRadius));
        rotated.lineTo(centreX, centreY - (radius*relativeInnerRadius));
        float plotAngle = ((getMaxAngle() - getMinAngle()) * i / nNotches) + getMinAngle();

        /* Checking if there is a step of 0.5 appearing.*/
        if (0 == (counter % 5)){
            X = (float) (centreX + radius*relativeLabelRadius*sin(plotAngle) - 1.5*thickness);
            Y = (float) (centreY - radius*relativeLabelRadius*cos(plotAngle)- thickness);
            g.setFont(12.0f); 	
            g.drawText((const juce::String&)(juce::String(0.5 * (int)(counter/5))),
                        X, Y,
                        textWidth, textHeight,
                        juce::Justification::left, true  );

        }

        // rotated.applyTransform(juce::AffineTransform::rotation (minAngle, centreX, centreY));
        rotated.applyTransform(juce::AffineTransform::rotation (plotAngle, centreX, centreY));

        g.strokePath (rotated, juce::PathStrokeType (1.0f));
        counter += (int)(getInterval()*10);
    }

    /* enclosing the notches in a circle */
    juce::Path p;
    p.startNewSubPath(centreX + radius*relativeInnerRadius*sin(getMinAngle()), centreY - radius*relativeInnerRadius*cos(getMinAngle()));
    p.addCentredArc(centreX, centreY,
                    radius*relativeInnerRadius, radius*relativeInnerRadius,
                    0, getMinAngle(), getMaxAngle());
    /* Drawing the arc. */
    g.strokePath(p, juce::PathStrokeType (1.0f));

    p.clear();

    p.startNewSubPath(centreX + radius*relativeOuterRadius*sin(getMinAngle()), centreY - radius*relativeOuterRadius*cos(getMinAngle()));
    p.addCentredArc(centreX,  centreY,
                  radius*relativeOuterRadius, radius*relativeOuterRadius,
                  0, getMinAngle(), getMaxAngle());
    /* Drawing the outer arc. */
    g.strokePath(p, juce::PathStrokeType (1.0f));

    /* Adding the maximum and minimum labels to the screen.*/
    textWidth = 25;
    textHeight = 15;
    g.setFont(18.0f);
    g.drawText ((const juce::String) getMinLimit(),
                 (float) (centreX + radius*relativeOuterRadius*sin(getMinAngle())), (float) (centreY - radius*relativeOuterRadius*cos(getMinAngle())),
                 (float)textWidth, (float) textHeight,
                 juce::Justification::centred, true);

    g.drawText ((const juce::String) getMaxLimit(),
                 (float) (centreX - radius*relativeOuterRadius*sin(getMinAngle()) - textWidth), (float) (centreY - radius*relativeOuterRadius*cos(getMinAngle())),
                 (float)textWidth, (float) textHeight,
                 juce::Justification::centred, true);
}

void Barometer::sliderValueChanged(juce::Slider* sliderChanged){
    float min = minSlider.getValue();
    float max = maxSlider.getValue();

    /* Checking if the values are out of range of each other. */
    if (max - min < getMinDifference())
    {   
        /* Checking if the maximum value is already at the maximum. If it is, only the minimum finger is moved. */
        if (max >= getMaxLimit()){
            max = getMaxLimit();
            maxSlider.setValue(max);
            minSlider.setValue(max - getMinDifference());
            return;
        }

        /* Checking if the minimum value is already at the minimum. If it is, only the maximum finger is moved. */
        else if (min <= getMinLimit()){
            min = getMinLimit();
            minSlider.setValue(min);
            maxSlider.setValue(min + getMinDifference());
            return;
        }
        
        /* If neither finger is on the edge, onlt the finger not being dragged is adjusted to ensure the minimum range is maintained. */
        else if (sliderChanged == &minSlider){
            maxSlider.setValue(min + getMinDifference());
            return;
        }

        else{
            minSlider.setValue(max - getMinDifference());
            return;
        }
    }
    trombonelessParameters.lowPressure = min;
    trombonelessParameters.highPressure = max;
    return;

}

void Barometer::setBackgroundColour(juce::Colour colour){
    backgroundColour = colour;
    return;
}

juce::Colour Barometer::getBackgroundColour(void){
    return backgroundColour;
}

void Barometer::setTextColour(juce::Colour colour){
    textColour = colour;
    return;
}

juce::Colour Barometer::getTextColour(void){
    return textColour;
}

void Barometer::setBoarderColour(juce::Colour colour){
    boarderColour = colour;
    return;
}

juce::Colour Barometer::getBoarderColour(void){
    return boarderColour;
}

void Barometer::setEdgeColour(juce::Colour colour){
    edgeColour = colour;
    return;
}

juce::Colour Barometer::getEdgeColour(void){
    return edgeColour;
}

void Barometer::setOuterRadius(float radius){
    relativeOuterRadius = radius;
}

float Barometer::getOuterRadius(void){
    return relativeOuterRadius;
}

void Barometer::setInnerRadius(float radius){
    relativeInnerRadius = radius;
}

float Barometer::getInnerRadius(void){
    return relativeInnerRadius;
}

void Barometer::setLabelRadius(float radius){
    relativeLabelRadius = radius;
}

float Barometer::getLabelRadius(void){
    return relativeLabelRadius;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                   verticalMixSlider                                        */
/*                                                                                            */
/* ========================================================================================== */

 verticalMixSlider::verticalMixSlider() : SliderWithLabel(juce::Slider::LinearVertical){
        this->slider.setRange(-9, 9, 3);                     /* Setting the maximum and minimum gain values for each band. */
        this->slider.setValue(0, juce::dontSendNotification);  /* Setting the initial value to be 1 so that there is no change in gain. */
        this->slider.setPopupMenuEnabled(false);
        this->setTopLabelBounds(30);
        this->setLeftLabelBounds(1);
        this->setRightLabelBounds(1);
        this->setBottomLabelBounds(30); 
        this->slider.addListener(this);
    };

verticalMixSlider::~verticalMixSlider(){
    this->slider.removeListener(this);
}

 void verticalMixSlider::sliderValueChanged(juce::Slider* sliderChanged) {
        /* Add any code for responding to slider values being changed here.*/
        if (sliderChanged){}
    }

/* ========================================================================================== */
/*                                                                                            */
/*                                       Equalizer                                            */
/*                                                                                            */
/* ========================================================================================== */

 Equalizer::Equalizer(char numberOfSliders): verticalMixSlider(){
    /* If there is no change in the number of sliders, the system will use the standard range. */
    if(numberOfSliders == 10){
        frequencyLabels.add ((juce::String) "0", (juce::String) "31.25");
        prevLabel = 31.25;
    }

    nSliders = numberOfSliders;
    /* Looping through every slider, creating it, setting its attributes adn adding it to an array. */
    for (char i=0; i < nSliders ; i++){
        
        /* Adding all n sliders which will be used. */
        eqSliders.add(new verticalMixSlider());
        // eqSliders[i]->slider.setPopupDisplayEnabled(true, true, this, 1000);
        eqSliders[i]->slider.setLookAndFeel(&customLook);
        eqSliders[i]->slider.addListener(this);
        
        if(numberOfSliders == 10){
            newLabel = (prevLabel+prevLabel);
            if (newLabel >= 1000){
                // juce::String newString = (newLabel/1000) + "k"
                frequencyLabels.add (((juce::String) (newLabel/1000))+ "k");
            }
            else{
                frequencyLabels.add ((juce::String) (newLabel));
            }
            prevLabel = newLabel; 
        }
        
        /* Adding the appropriate labels to the slider. */
        eqSliders[i]->CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, frequencyLabels[i+1]);
        eqSliders[i]->CreateLabel(SliderWithLabel::LabelPositions_t::LowerCentre, frequencyLabels[i]);
    }
    
}

 Equalizer::~Equalizer(){
    for (char i=0; i < nSliders; i++){
        eqSliders[i]->slider.setLookAndFeel(nullptr);
        eqSliders[i]->slider.removeListener(this);
    }
}

void Equalizer::resized()
{
    /* Finding the local bounds for the entire object. */
    juce::Rectangle<int> totalArea = getLocalBounds();
    juce::Rectangle<int> sliderBound;
    
    /* Looping through and having the sliders share the same amount of space each. */
    for (char i=0; i < nSliders ; i++){
        sliderBound = totalArea.removeFromLeft((totalArea.getWidth()) / (nSliders - i));
        eqSliders[i]->setBounds(sliderBound);
        addAndMakeVisible (eqSliders[i]);
    }
    
}

void Equalizer::sliderValueChanged(juce::Slider* sliderChanged) {
    for (int i=0; i < nSliders; i++){
        if (((int) sizeof(synthesiserParameters.gains) / (int) sizeof(synthesiserParameters.gains[0])) < i){
            /* The number of sliders is more than the length of the gains array.
                writing passed the value would break the system. Better to return
                before crashing.    */
#ifdef DBG_MSG
            DBG("Too many sliders in the equalizer compared to the number of gains defined in `synthesiserParameters.gains`.\n");
#endif
            return;
        }

        if (sliderChanged == &(eqSliders[i]->slider)){
            /* Updating the slider value in the synthesiserParameters struct to match the new updated value. */
            synthesiserParameters.gains[i] = eqSliders[i]->slider.getValue();
        }
    }
    return;            
}
    

/* ========================================================================================== */
/*                                                                                            */
/*                                   Drop Down Menu                                           */
/*                                                                                            */
/* ========================================================================================== */
 DropDownMenu::DropDownMenu(juce::String labelPhrase){
    addAndMakeVisible (dropDownChoice);
    dropDownChoice.setSelectedId (0);

    /* Adding a label to be beside the shift key dropdown menu. */
    addAndMakeVisible (dropDownLabel);
    dropDownLabel.setText (labelPhrase, juce::dontSendNotification);
    dropDownLabel.setJustificationType (juce::Justification::centred);
    dropDownLabel.attachToComponent(&dropDownChoice, true);
 }

 void DropDownMenu::resized(){
    auto area = getLocalBounds();
    auto labelBounds = area.removeFromLeft (std::min(area.getWidth()*0.6, double(80)));
    dropDownLabel.setBounds (labelBounds);
    dropDownChoice.setBounds (area.withSizeKeepingCentre(area.getWidth()*0.95, area.getHeight()));
}

 void DropDownMenu::AddItem(juce::String itemName, int itemValue){
    dropDownChoice.addItem(itemName, itemValue);
 }

 void DropDownMenu::ChangeLabelText(juce::String text, juce::NotificationType notification){
    dropDownLabel.setText(text, notification);
 }

 juce::String DropDownMenu::getLabelText(void){
    return dropDownLabel.getText();
 }


/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrationOnClick                                       */
/*                                                                                            */
/* ========================================================================================== */

 CalibrationOnClick::CalibrationOnClick(){
    /* Setting the button to only be momentary and not a switch. */
    this->button.setToggleable(false);
    /* Setting the button to only be triggered on the mouse down motion. */
    button.setTriggeredOnMouseDown(true);
    /* Attaching a listener so that this object can perform functions 
    when the button is pressed. */
    button.addListener(this);

 }
 CalibrationOnClick::~CalibrationOnClick(){
    button.removeListener(this);
 }

 void CalibrationOnClick::resized(){
    juce::Rectangle<int> area = getLocalBounds();
    juce::Rectangle<int> buttonBounds = area.withSizeKeepingCentre(area.getWidth()*0.9, area.getHeight()*0.9);
    this->button.setBounds(buttonBounds);

 }


/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrateEmbouchure                                       */
/*                                                                                            */
/* ========================================================================================== */

CalibrateEmbouchure::CalibrateEmbouchure(){
    /* Adding the dropdown menu to select the start */
    addAndMakeVisible (embouchureChoice);
    embouchureChoice.ChangeLabelText("Embouchure\nselection: ");
    embouchureChoice.AddItem ("Low placement", EMBCR_LowPlacement);
    embouchureChoice.AddItem ("Medium placement", EMBCR_MediumPlacement);
    embouchureChoice.AddItem ("High placement", EMBCR_HighPlacement);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    embouchureChoice.OnChange (&trombonelessParameters.embouchureOption);

    /* Adding the button. */
    addAndMakeVisible (this->button);
    this->button.setButtonText("Calibrate");

    /* Adding a tool tip to be attached to the button so that if a user does not
    know what the calibrate button is meant to do, it will explain it. */
    this->button.setTooltip("This button will read the\ncurrent capacitance of the\nmouthpiece and set that as\nthe zero point.");
    tooltipWindow.setMillisecondsBeforeTipAppears(300);

}

void CalibrateEmbouchure::resized(){
    juce::Rectangle<int> area = getLocalBounds();

    /* Making the button take up 40% of the space or 100 pixels.*/
    juce::Rectangle<int> buttonBounds = area.removeFromRight(std::min((float) (area.getWidth()*0.4), (float) 100));
    buttonBounds = buttonBounds.withSizeKeepingCentre(buttonBounds.getWidth(), buttonBounds.getHeight()*0.5);
    this->button.setBounds(buttonBounds);

    /* Allowing the rest of the area to go to the dropdown and label. */
    embouchureChoice.setBounds(area);

}

void CalibrateEmbouchure::buttonClicked(juce::Button* thisButtonClicked)
{
    if (thisButtonClicked == &this->button){
        trombonelessParameters.triggerEmbouchureCalibrate = true;
    }
}
