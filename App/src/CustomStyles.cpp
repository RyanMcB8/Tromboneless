
#include "CustomStyles.hpp"

/* ========================================================================================== */
/*                                                                                            */
/*                                     VerticalSlider                                         */
/*                                                                                            */
/* ========================================================================================== */

VerticalSliderLookAndFeel::VerticalSliderLookAndFeel(){
    
}

void VerticalSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, float minSliderPos, 
    float maxSliderPos, const juce::Slider::SliderStyle style, 
    juce::Slider& slider) {

    /* Ensureing that only the vertical sliders are being changed */
    if (style == juce::Slider::LinearVertical)
    {
        auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);
        
        /* Adding a background colour to the slider. */
        g.setColour(backgroundColour);
        auto background = (bounds.withHeight(bounds.getHeight()));
        background = background.withSizeKeepingCentre(width, height*1.5);
        g.fillRect(background);

        /* Adding a track decal running the length of the slider. */
        g.setColour(trackColour);
        g.fillRect(bounds.withWidth(trackWidth).withCentre(bounds.getCentre()));

        /* Adding ticks to the slider */
        int numSteps = 10;
        g.setColour(tickColour);

        for (int i = 1; i <= numSteps-1; ++i){
            float proportion = (float)i / (float)numSteps;

            float yPos = bounds.getBottom() - proportion * bounds.getHeight();

            // Draw horizontal tick line (perpendicular to vertical slider)
            g.drawLine(bounds.getCentreX() - tickWidth, yPos,
                        bounds.getCentreX() + tickWidth, yPos, tickThickness);
        }
        
        /* Drawing the maxmimum and minimum ticks as wider than the rest of the ticks.*/
        g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom(),
                        bounds.getCentreX() + tickWidth*1.5, bounds.getBottom(), tickThickness);

        g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom() - bounds.getHeight(),
                        bounds.getCentreX() + tickWidth*1.5, bounds.getBottom() - bounds.getHeight(), tickThickness);

        /* Drawing the custom finger. */
        g.setColour(dialColour);
#ifdef USE_CIRCLE_FINGER            
        g.fillEllipse(bounds.getCentreX() - fingerRadius, sliderPos - fingerRadius,
                        2*fingerRadius, 2*fingerRadius);

        g.setColour(dialEdgeColour);
        g.drawEllipse(bounds.getCentreX() - (0.9)*fingerRadius, sliderPos - (0.9 *fingerRadius),
                        (1.8*fingerRadius), (1.8*fingerRadius), 2);
#else
        /* Creating a slider finger that is shaped like a rectangle as opposed to a circle. */
        g.setColour(dialColour);
        g.fillRect(bounds.getCentreX() - (fingerWidth/2), sliderPos - (fingerHeight/2),
                        fingerWidth, fingerHeight);
        
        /* Adding a small horizontal line to the slider to give it more depth. */              
        g.setColour(dialEdgeColour);                          
        g.drawLine(bounds.getCentreX() - (fingerWidth*0.4), sliderPos ,
                    bounds.getCentreX() + (fingerWidth*0.4), sliderPos, 0.2 * fingerHeight) ;

        /* Adding a small horizontal line to the slider to give it more depth. */              
        g.setColour(dialEdgeColour);                          
        g.drawLine(bounds.getCentreX() - (fingerWidth*0.3), sliderPos + (fingerHeight*0.3),
                    bounds.getCentreX() + (fingerWidth*0.3), sliderPos + (fingerHeight*0.3), 0.2 * fingerHeight) ;
        g.drawLine(bounds.getCentreX() - (fingerWidth*0.3), sliderPos - (fingerHeight*0.3),
                    bounds.getCentreX() + (fingerWidth*0.3), sliderPos - (fingerHeight*0.3), 0.2 * fingerHeight) ;
#endif
    }
    else
    {
        // fallback to default for other styles
        juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, 
            maxSliderPos, style, slider);
    }
}        

/* ========================================================================================== */
/*                                                                                            */
/*                                       PaintTrombone                                        */
/*                                                                                            */
/* ========================================================================================== */

juce::Path PaintTrombone(float x, float y, float width, bool incPipes)
{
    juce::Path tromboneShape;
    
    float pipeLength    =   60;
    float pipeThickness =   14;
    float height        =   30;
    float coneLength    =   30;
    float gap           =   10; /* The gap between each pipe*/
    float end           =   10; /* The extra length of the pipes compared to the main pipe.*/

    float X             = x+pipeLength;
    float Y             = y+height;
    float totalHeight;

    /* ========== Adding the main pipe. ========== */
    tromboneShape.startNewSubPath(X, Y);
    tromboneShape.lineTo(x, Y);
    tromboneShape.lineTo(x, Y + pipeThickness);
    tromboneShape.lineTo(X, Y + pipeThickness);
    /* Moving from the main pipe to the cone output bottom.*/

    tromboneShape.quadraticTo(X+coneLength, Y+pipeThickness, X+coneLength, Y+pipeThickness+height);
    /* Moving from the bottom of the cone to the top. */
    tromboneShape.lineTo(X+coneLength, y);

    /*Moving from the top of the cone back to the start. */
    tromboneShape.quadraticTo(X+coneLength, Y, X, Y);
    tromboneShape.closeSubPath();

    /* Drawing the pipes if they are incuded in the drawing.*/
    if (true == incPipes){
        /* ========== Creating the first of the 2 minipipes. ========== */
        float yPipe = Y+pipeThickness + gap;
        pipeThickness = pipeThickness/2;
        tromboneShape.startNewSubPath(X+coneLength+end, yPipe);
        tromboneShape.lineTo(x, yPipe);
        tromboneShape.lineTo(x, yPipe+pipeThickness);
        tromboneShape.lineTo(X+coneLength+end, yPipe+pipeThickness);
        tromboneShape.closeSubPath();

        /* ========== Creating the second of the 2 minipipes. ========== */
        yPipe = yPipe + gap+pipeThickness;
        tromboneShape.startNewSubPath(X+coneLength+end, yPipe);
        tromboneShape.lineTo(x, yPipe);
        tromboneShape.lineTo(x, yPipe+pipeThickness);
        tromboneShape.lineTo(X+coneLength+end, yPipe+pipeThickness);
        tromboneShape.closeSubPath();

        /* Determining the total height if the pipes are included. */
        totalHeight =  yPipe + pipeThickness - y;
    }
    else{
        totalHeight =  (2*+height) + pipeThickness;
    }

    /* Determining the scaling factor for the image. */
    float totalWidth = pipeLength + coneLength + end;
    float outputHeight = totalHeight * (width / totalWidth);

    /* Scaling the path to fit into the prescribed width. */
    tromboneShape.scaleToFit(x, y, width, outputHeight, 1);

    /* Returning the path which has been drawn. */
    return tromboneShape;
}

juce::Path PaintTromboneEnd(float x, float y, float width)
{
    juce::Path tromboneShape;
    float gap = 10;
    float pipeThickness = 14;
    float maxDiameter    =   gap+(2*pipeThickness);
    float minDiameter    =   gap;

    /* Adding the main pipe. */
    tromboneShape.startNewSubPath(x, y);
    tromboneShape.addCentredArc (x+(maxDiameter/2), y+(maxDiameter/2), maxDiameter, maxDiameter, 0, 0, M_PI);
    tromboneShape.lineTo(x, y+(2*pipeThickness)+gap);
    tromboneShape.lineTo(x, y + pipeThickness+pipeThickness);
    tromboneShape.addCentredArc (x+maxDiameter/2, y+maxDiameter/2, minDiameter, minDiameter, 0, M_PI, 0);
    tromboneShape.lineTo(x, y+pipeThickness);
    tromboneShape.lineTo(x, y);
    tromboneShape.closeSubPath();

    float thisWidth = maxDiameter/2;
    float height = (width)*(maxDiameter)/thisWidth;
    tromboneShape.scaleToFit(x, y, width, height, 1);


    return tromboneShape;
}



/* ========================================================================================== */
/*                                                                                            */
/*                                   CalibrationSlider                                        */
/*                                                                                            */
/* ========================================================================================== */

CalibrationSliderLookAndFeel::CalibrationSliderLookAndFeel(){
    /* Currently the constructor does not add any extra functionality, only creates an object. */
}


void CalibrationSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, float minSliderPos, 
    float maxSliderPos, const juce::Slider::SliderStyle style, 
    juce::Slider& slider) 
    {
    /* Ensuring that it is a 2 valued horizontal slider. */
    if (style == juce::Slider::TwoValueHorizontal)
    {
        
        float tromboneEndWidth = 12;
        float fingerThickness = 4;
        auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);

        /* Adding a track decal running the length of the slider. */
        g.setColour(emptyTrackColour);
        juce::Rectangle<float> sliderBounds  = bounds.withHeight(trackWidth).withCentre(bounds.getCentre());
        g.fillRect(sliderBounds.withY(y + (height*0.8)));
    
        /* The ratio of the trombones pipe thickness to overall height is 0.0897436*/

        /* Finding the 2 thumb positions. */
        float thumbOneX = slider.getPositionOfValue(slider.getMinValue());
        float thumbTwoX = slider.getPositionOfValue(slider.getMaxValue());

        /* Adding a track decal between the 2 fingers. */
        g.setColour(fullTrackColour);
        juce::Rectangle fullSliderBounds = juce::Rectangle<float>((float) thumbOneX, (float) y+(height/2), 
            (float) (thumbTwoX - thumbOneX-fingerThickness-tromboneEndWidth), (float) trackWidth);
        g.fillRect(fullSliderBounds);
        fullSliderBounds = fullSliderBounds.withY(fullSliderBounds.getY() + 8);
        g.fillRect(fullSliderBounds);
        
        /* Changing the look of the first thumb: */
        // g.setColour(thumbOneColour);
        g.setColour (juce::Colours::gold);
        juce::Path tromboneShape = PaintTrombone(thumbOneX+fingerThickness, y, 50);
        g.fillPath(tromboneShape);

        /* Changing the look of the second thumb: */
        tromboneShape = PaintTromboneEnd(thumbTwoX - fingerThickness - 12, y+20 , 12);
        g.fillPath(tromboneShape);

        g.setColour (juce::Colours::black);
        juce::Rectangle Thumbs = juce::Rectangle<float>((float) thumbOneX, (float) y+(height/2), (float) fingerThickness, (float) bounds.getHeight()*0.8);
        g.fillRect(Thumbs);
        Thumbs = Thumbs.withX(thumbTwoX);
        g.fillRect(Thumbs);
    }
    
    else{
        std::cout << "Error\n";
    }


}

// void CalibrationSliderLookAndFeel::drawSlider(juce::Graphics& g, juce::Slider& slider){

// }