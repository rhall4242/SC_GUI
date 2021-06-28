/*
  ==============================================================================

    Capture.h
    Created: 26 Jun 2021 8:57:42pm
    Author:  rhall

  ==============================================================================
*/

#pragma once

#include "MainComponent.h"

class Capture
{
public:
  Connector *conn;
  int nPoints = 96000;
  std::vector<float> points;
  std::string name;

  Capture(Connector *c, std::string nm);
  void updatePoint(int index);
};

class CaptureGrid : public std::vector<Capture*>
{
public:
  const int maxCaptures {6};
  int onSample {0};
  int offSample {48000};
  NodeTree nodeTree;
  ConnectionTree connectionTree;

  void nodeInit();
  Connection* createConnection(OutputConnector* from, InputConnector* to, juce::String nm = "");
  int addCapture(Capture* c, std::string nm = "");
  void run(int startSample = 0, int nSamples = 96000);
  void startNote(int noteNum);
  void stopNote();

};