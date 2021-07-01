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

static juce::Colour colors[6] {juce::Colours::green, juce::Colours::blue, juce::Colours::red, juce::Colours::yellow, juce::Colours::cyan, juce::Colours::magenta};

void GraphComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background

    g.setColour (juce::Colours::white);
    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component
    if (!grid->valid)
    {
      return;
    }
    auto height = getHeight() - 20;
    auto width = getWidth();
    auto step = grid->nSamples / width / xscale;
    auto vstep = height / grid->size();

    for (int i = 0; i < grid->size(); i++)
    {
      g.setColour (juce::Colours::grey);
      auto ypos = (i * vstep) + (vstep / 2) + 10;
      g.drawLine(0, ypos, width, ypos);
    }
    for (int i = 0; i < width; i++)
    {
      if ((((int) (i * step)) % 2048) < step)
      {
      g.setColour (juce::Colours::grey);
      g.drawLine(i, 0, i, height);
      }
    }
    for (int i = 1; i < width; i++)
    {
      int j = 0;
      for (auto c: *grid)
      {
          g.setColour (colors[j]);
          auto ypos = (j * vstep) + (vstep / 2) + 10;
          auto val = (i * step) < c->nPoints ? c->points[i * step] : 0.0f;
          auto pval = (i * step) < c->nPoints ? c->points[(i-1) * step] : 0.0f;
          auto yval = ypos - (val * (vstep / 2.2));
          auto pyval = ypos - (pval * (vstep / 2.2));
          g.drawLine(i-1, pyval, i, yval, 2);
          j++;
      }
    }
    g.drawText("Scale = " + juce::String(xscale), 10, 10, 150, 20, juce::Justification::bottomLeft);
}

void GraphComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GraphComponent::mouseDrag(const juce::MouseEvent& ev)
{
  int dx = ev.getDistanceFromDragStartX();
  int dy = ev.getDistanceFromDragStartY();
  auto fact = dy / 400.0f;
  xscale += fact;
  if (xscale <= 0.1f) xscale = 0.1f;
  repaint();
}
