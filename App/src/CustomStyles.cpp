/** @file       CustomStyles.cpp
 *  @author     Ryan McBride
 *  @brief      This file holds the defintions of all functions and variables
 *              needed by the Tromboneless project to change the looks and feel
 *              of various different widgets which are being displayed to the
 *              end user.
 */

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
        int numSteps = 6;

        juce::Array <juce::String> numeralLabels = {"-9dB", "-6dB", "-3dB", "0dB", "+3dB", "+6dB", "+9dB"};
        g.setFont((float) 14);	
        int textHeight = 18;
        int textWidth = (int) (bounds.getWidth()/3);
        
        for (int i = 1; i <= numSteps-1; ++i){
            float proportion = (float)i / (float)numSteps;

            float yPos = bounds.getBottom() - proportion * bounds.getHeight();

            // Draw horizontal tick line (perpendicular to vertical slider)
            g.setColour(tickColour);
            g.drawLine(bounds.getCentreX() - tickWidth, yPos,
                        bounds.getCentreX() + tickWidth, yPos, tickThickness);

            /* Adding numerals to the side of the slider. */
            g.setColour(textColour);
            g.drawText(numeralLabels[i],
                       (int) (bounds.getCentreX() + tickWidth + 5), (int) ((yPos)-(textHeight/2)),
                       (int) textWidth, (int) textHeight,
                       juce::Justification::left );
        }
        
        /* Drawing the maxmimum and minimum ticks as wider than the rest of the ticks.*/
        g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom(),
                        bounds.getCentreX() + tickWidth*1.5, bounds.getBottom(), tickThickness);
        g.drawText(numeralLabels[0],
                       (int) (bounds.getCentreX() + tickWidth + 10), (int) (bounds.getBottom()-(textHeight/2)),
                       (int) textWidth, (int) textHeight,
                       juce::Justification::left );

        g.drawLine(bounds.getCentreX() - tickWidth*1.5, bounds.getBottom() - bounds.getHeight(),
                        bounds.getCentreX() + tickWidth*1.5, bounds.getBottom() - bounds.getHeight(), tickThickness);
        g.drawText(numeralLabels[6],
                       (int) (bounds.getCentreX() + tickWidth + 10), (int) ((bounds.getBottom() - bounds.getHeight())-(textHeight/2)),
                       (int) textWidth, (int) textHeight,
                       juce::Justification::left );


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

void VerticalSliderLookAndFeel::setTrackWidth(float width){
    trackWidth = width;
}

float VerticalSliderLookAndFeel::getTrackWidth(void){
    return trackWidth;
}

void VerticalSliderLookAndFeel::setTickThickness(float thickness){
    tickThickness = thickness;
}

float VerticalSliderLookAndFeel::getTickThickness(void){
    return tickThickness;
}

void VerticalSliderLookAndFeel::setTickWidth(float width){
    tickWidth = width;
}

float VerticalSliderLookAndFeel::getTickWidth(void){
    return tickWidth;
}

#ifdef USE_CIRCLE_FINGER 
void VerticalSliderLookAndFeel::setFingerRadius(float radius){
    fingerRadius = radius;
}

float VerticalSliderLookAndFeel::getFingerRadius(void){
    return fingerRadius;
}

#else
void VerticalSliderLookAndFeel::setFingerWidth(float width){
    fingerWidth = width;
}

float VerticalSliderLookAndFeel::getFingerWidth(void){
    return fingerWidth;
}

void VerticalSliderLookAndFeel::setFingerHeight(float height){
    fingerHeight = height;
}

float VerticalSliderLookAndFeel::getFingerHeight(void){
    return fingerHeight;
}
#endif
/* ========================================================================================== */
/*                                                                                            */
/*                                       PaintTrombone                                        */
/*                                                                                            */
/* ========================================================================================== */

juce::Path PaintTrombone(float x, float y, float width, float height, bool incPipes)
{
    juce::Path tromboneShape;
    
    /* Determining the proportions of the components based upon the input.*/
    float pipeThickness =   height*0.25;                 /* Pipe thickness will always be 25% of the height. */
    float coneLength    =   width*0.3;                  /* Cone length with always be 30% of the overall width. */
    float gap           =   height*0.1;                 /* The gap between each pipe will always be 10% of the height. */
    float end           =   std::min((float) 10, (float) (width* 0.1));   /* The extra length of the pipes compared to the main pipe. */
    float pipeLength    =   width - coneLength - end;   /* The length of the pipe is whatever is left of the width. */

    float X             = x+pipeLength;
    float Y             = y+height;

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

    }
    /* Returning the path which has been drawn. */
    return tromboneShape;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                          PaintArc                                          */
/*                                                                                            */
/* ========================================================================================== */

juce::Path PaintArc(float x, float y, float thickness, float gap, bool direction)
{
    juce::Path arcShape;
    // thickness = thickness/2;
    float maxRadius    =   (gap+(2*thickness))/2;
    float minRadius    =   gap/2;

    float startRad = 0;
    float endRad =   M_PI;
    float rotDir;
    arcShape.startNewSubPath(x, y+(maxRadius));
    /* If the direction of the arc is on the right. */
    if (0 == direction){
        rotDir = 0;
        arcShape.addCentredArc (x, y+(maxRadius), maxRadius, maxRadius, rotDir, startRad, endRad);
        arcShape.addCentredArc (x, y+maxRadius, minRadius, minRadius, rotDir, startRad, endRad);
    }

    /* If the direction of the arc is on the left.*/
    else{
        rotDir = M_PI;
        arcShape.addCentredArc (x, y+maxRadius, minRadius, minRadius, rotDir, endRad, startRad);
        arcShape.addCentredArc (x, y+(maxRadius), maxRadius, maxRadius, rotDir, endRad, startRad);
    }

    /* Adding the outer arc from 0 to pi radians. */
    /* Adding the inner arc from 0 to pi radians. */

    arcShape.closeSubPath();


    return arcShape;
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
        /* This block is simply to use the unused parameters within the function so
           that no extra warnings appear in the debug output. */
        maxSliderPos = sliderPos;
        sliderPos = minSliderPos;
        maxSliderPos = maxSliderPos;
    /* Ensuring that it is a 2 valued horizontal slider. */
    if (style == juce::Slider::TwoValueHorizontal)
    {
        /* Adjusting the sizes of different components depending upon the screen size.*/
        float fingerThickness = std::min((float) 4, (float) (width*0.05));
        float extraPipe = std::min((float) 30, (float) (width*0.2));
        float pipeGap = std::min((float) 8, (float) (height*0.7));
        float mouthPieceDelta = std::min((float) 20, (float) (width*0.1));
        auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);

        /* Adding a track decal running the length of the slider. */
        g.setColour(emptyTrackColour);
        juce::Rectangle<float> sliderBounds  = bounds.withHeight(trackWidth).withCentre(bounds.getCentre());
        g.fillRect(sliderBounds.withY(y + (height*0.8)));

        /* Finding the 2 thumb positions. */
        float thumbOneX = slider.getPositionOfValue(slider.getMinValue());
        float thumbTwoX = slider.getPositionOfValue(slider.getMaxValue());

        /* Adding the start of the pipe to start near the first finger. */
        g.setColour(fullTrackColour);
        juce::Rectangle fullSliderBounds = juce::Rectangle<float>((float) thumbOneX-extraPipe, (float) y+(height/2), 
            (float) (thumbTwoX - thumbOneX + extraPipe), (float) trackWidth);
        g.fillRect(fullSliderBounds);

        /* Adding the bottom part of the pipe which is slightly longer than the middle part of the pipe. */
        fullSliderBounds = fullSliderBounds.withY(fullSliderBounds.getY() + pipeGap);
        fullSliderBounds = fullSliderBounds.withWidth(fullSliderBounds.getWidth() + mouthPieceDelta);
        fullSliderBounds = fullSliderBounds.withX(fullSliderBounds.getX() - mouthPieceDelta);
        g.fillRect(fullSliderBounds);
        
        /* Adding the trombone horn to be output to the screen. */
        g.setColour (juce::Colours::gold);
        juce::Path tromboneShape = PaintTrombone(thumbOneX - std::min((float) 80, (float) (width*0.2))*0.65 , y - 1.2*pipeGap, std::min((float) 80, (float) (width*0.2)), 20, false);
        g.fillPath(tromboneShape);

        /* Adding the arc between the middle part of the trombone to meet the bottom part. */
        tromboneShape = PaintArc(thumbTwoX, fullSliderBounds.getY() - pipeGap, trackWidth, pipeGap/2, 0);
        g.fillPath(tromboneShape);

        /* Adding the arc between the bottom part of the trombone to meet the top part. */
        tromboneShape = PaintArc(fullSliderBounds.getX()+0.1, y+((height +2)/2)- pipeGap, trackWidth, (pipeGap/2)+trackWidth, 1);
        g.fillPath(tromboneShape);

        /* Adding lines to indicate where on the slider the values are. */
        g.setColour (juce::Colours::black);
        juce::Rectangle Thumbs = juce::Rectangle<float>((float) thumbOneX, (float) (y + (height*0.8)), (float) fingerThickness, (float) bounds.getHeight()*0.8);
        g.fillRect(Thumbs);
        Thumbs = Thumbs.withX(thumbTwoX);
        g.fillRect(Thumbs);
    }

}

/* ========================================================================================== */
/*                                                                                            */
/*                                          RotaryArc                                         */
/*                                                                                            */
/* ========================================================================================== */

juce::Path RotaryArc(float x, float y, float thickness, float width, float height, float angularRange, float rotation)
{
    juce::Path arcShape;
    float xRadius   =   width;
    float yRadius   =   height;

    float endRad = angularRange/2;
    float startRad = -angularRange/2;

    arcShape.startNewSubPath(x , y);

    arcShape.addCentredArc (x, y, 
        xRadius + thickness, yRadius + thickness, 
        rotation, startRad, endRad);

    arcShape.closeSubPath();
    return arcShape;

}

/* ========================================================================================== */
/*                                                                                            */
/*                                        drawCircle                                          */
/*                                                                                            */
/* ========================================================================================== */

juce::Path drawCircle(float x, float y, float xRadius, float yRadius){

    juce::Path circleShape;
    circleShape.startNewSubPath(x , y);

    circleShape.addCentredArc (x, y, xRadius, yRadius, 0, 0, 2*M_PI);

    circleShape.closeSubPath();
    return circleShape;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                   NeedleLookAndFeel                                        */
/*                                                                                            */
/* ========================================================================================== */

void NeedleLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, 
            int height, float sliderPos, const float rotaryStartAngle,
             const float rotaryEndAngle, juce::Slider& slider) {

    /* Finding the centere of the slider. */
    float centreX = x + (width/2);
    float centreY = y + (height/2);

    float thickness = 5;
    /* How much extra length relative to the radius is added to the other side of the slider. */
    float extra = 0.1;

    /* Determining the angle of the knob with respect to the centre point.*/
    float angle = (sliderPos*rotaryEndAngle) + (1-sliderPos)*(rotaryStartAngle);

    /* Creating the main line of the arrow.*/
    juce::Rectangle<float> mainLine = juce::Rectangle<float>{-(thickness/2), (float)(-(width*0.4)), thickness, (float)((height*0.4)*(1+extra))};
    juce::Path p;
    p.addRectangle(mainLine);
    /* Adding the point at the end of the triangle. */
    p.addTriangle(-thickness, -(height)*(0.4), 0, -((height)*(0.5)), thickness, -(height*0.4));
    p.applyTransform (juce::AffineTransform::translation (x, y));
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour(needleColour);
    g.fillPath(p);
}
