/*
  ==============================================================================

    GraphComponent.h
    Created: 28 Jun 2021 12:30:02pm
    Author:  rhall

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Capture.h"

//==============================================================================
/*
*/
class GraphComponent  : public juce::Component
{
public:
    GraphComponent(CaptureGrid* g);
    ~GraphComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDrag(const juce::MouseEvent& ev) override;
    
    CaptureGrid* grid; 

    float xscale {1.0f};

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphComponent)
};
