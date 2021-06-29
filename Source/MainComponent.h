#pragma once

#include <JuceHeader.h>
#include "Connector.h"
#include "Node.h"
#include "MidiInputNode.h"
#include "MonoOsc1Node.h"
#include "Value8Node.h"
#include "MSEGNode.h"
#include "AudioOutputNode.h"
#include "Capture.h"
#include "GraphComponent.h"

class SC_GUIApplication;
static SC_GUIApplication* theApp;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::TextButton graphButton {"Graph"};
    void graphMenu();
    std::function<void()> graphClicked {[this] (){ graphMenu();}};

    CaptureGrid grid;
    GraphComponent graph;


private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
