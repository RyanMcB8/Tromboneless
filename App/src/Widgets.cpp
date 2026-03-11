/* ======================================== Creation of functions for the widgets that will be displayed in the window. ======================================== */

/** @brief function that can change the appearance of the sliders to look like a rotary slider used on amplifiers.
 * @param g A reference to the current window which is in use.
 * @param x The centre width location of the widget with respect to the window frame.
 * @param y The centre height location of the widget with respect to the window frame.
 * @param width The total width of the slider being created.
 * @param height The total height of the slider being created.
 * @param sliderPosProportional Sets the position of the slider proportional to...
 * @param rotaryStartAngle Sets the minimum angle that the rotary slider can be rotated to.
 * @param rotaryEndAngle Sets the maximum angle that the rotary slider can be rotated to.
 * @note This function was created by matkatmusic and was modified to work better for the Tromboneless project.
 */
// void LookAndFeel::drawRotarySlider(juce::Graphics & g,
//                                    int x,
//                                    int y,
//                                    int width,
//                                    int height,
//                                    float sliderPosProportional,
//                                    float rotaryStartAngle,
//                                    float rotaryEndAngle,
//                                    juce::Slider & slider)
// {
//     using namespace juce;
    
//     auto bounds = Rectangle<float>(x, y, width, height);
    
//     auto enabled = slider.isEnabled();
    
//     g.setColour(enabled ? Colour(97u, 18u, 167u) : Colours::darkgrey );
//     g.fillEllipse(bounds);
    
//     g.setColour(enabled ? Colour(255u, 154u, 1u) : Colours::grey);
//     g.drawEllipse(bounds, 1.f);
    
//     if( auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
//     {
//         auto center = bounds.getCentre();
//         Path p;
        
//         Rectangle<float> r;
//         r.setLeft(center.getX() - 2);
//         r.setRight(center.getX() + 2);
//         r.setTop(bounds.getY());
//         r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);
        
//         p.addRoundedRectangle(r, 2.f);
        
//         jassert(rotaryStartAngle < rotaryEndAngle);
        
//         auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
        
//         p.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));
        
//         g.fillPath(p);
        
//         g.setFont(rswl->getTextHeight());
//         auto text = rswl->getDisplayString();
//         auto strWidth = g.getCurrentFont().getStringWidth(text);
        
//         r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
//         r.setCentre(bounds.getCentre());
        
//         g.setColour(enabled ? Colours::black : Colours::darkgrey);
//         g.fillRect(r);
        
//         g.setColour(enabled ? Colours::white : Colours::lightgrey);
//         g.drawFittedText(text, r.toNearestInt(), juce::Justification::centred, 1);
//     }
// }

/** @brief A function that can be used to add a slider to the window.
 * @param g A reference to the window being painted to.
 * @note This function was created by matkatmusic and was modified to work better for the Tromboneless project.
 */
// void RotarySliderWithLabels::paint(juce::Graphics &g)
// {
//     using namespace juce;
    
//     auto startAng = degreesToRadians(180.f + 45.f);
//     auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;
    
//     auto range = getRange();
    
//     auto sliderBounds = getSliderBounds();
    
// //    g.setColour(Colours::red);
// //    g.drawRect(getLocalBounds());
// //    g.setColour(Colours::yellow);
// //    g.drawRect(sliderBounds);
    
//     getLookAndFeel().drawRotarySlider(g,
//                                       sliderBounds.getX(),
//                                       sliderBounds.getY(),
//                                       sliderBounds.getWidth(),
//                                       sliderBounds.getHeight(),
//                                       jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
//                                       startAng,
//                                       endAng,
//                                       *this);
    
//     auto center = sliderBounds.toFloat().getCentre();
//     auto radius = sliderBounds.getWidth() * 0.5f;
    
//     g.setColour(Colour(0u, 172u, 1u));
//     g.setFont(getTextHeight());
    
//     auto numChoices = labels.size();
//     for( int i = 0; i < numChoices; ++i )
//     {
//         auto pos = labels[i].pos;
//         jassert(0.f <= pos);
//         jassert(pos <= 1.f);
        
//         auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);
        
//         auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1, ang);
        
//         Rectangle<float> r;
//         auto str = labels[i].label;
//         r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
//         r.setCentre(c);
//         r.setY(r.getY() + getTextHeight());
        
//         g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
//     }
    
// }
