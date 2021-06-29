/*
  ==============================================================================

    GraphComponent.cpp
    Created: 28 Jun 2021 12:30:02pm
    Author:  rhall

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GraphComponent.h"

//==============================================================================
GraphComponent::GraphComponent(CaptureGrid* g)
{
  grid = g;

}

GraphComponent::~GraphComponent()
{
}

void GraphComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component
    if (!grid->valid)
    {
      return;
    }
    g.setColour (juce::Colours::green);
    auto height = getHeight() - 20;
    auto width = getWidth();
    auto step = grid->nSamples / width / 6;
    auto vstep = height / grid->size();

    for (int i = 1; i < width; i++)
    {
      int j = 0;
      for (auto c: *grid)
      {
          auto ypos = (j * vstep) + (vstep / 2) + 10;
          auto val = c->points[i * step];
          auto pval = c->points[(i-1) * step];
          auto yval = ypos + (val * (vstep / 2.2));
          auto pyval = ypos + (pval * (vstep / 2.2));
          g.drawLine(i-1, pyval, i, yval);
          j++;
      }
    }
}

void GraphComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
