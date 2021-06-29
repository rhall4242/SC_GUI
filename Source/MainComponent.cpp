#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : graph(&grid)
{
    grid.nodeInit();
    setSize (1600, 800);
    graphButton.onClick = graphClicked;
    addAndMakeVisible(graphButton);
    addAndMakeVisible(graph);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
  graphButton.setBounds(25, 15, 60, 20);
  graph.setBounds(25, 40, 1550, 750);
}

void MainComponent::graphMenu()
{
    juce::PopupMenu m;
    m.addItem (1, "Run");
    m.addItem (2, "Show Graph");
 
    const int result = m.show();
 
    if (result == 0)
    {
        // user dismissed the menu without picking anything
    }
    else if (result == 1)
    {
      grid.run();
    }
    else if (result == 2)
    {
      graph.repaint();
    }      

}


