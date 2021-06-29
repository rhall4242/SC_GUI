/*
  ==============================================================================

    Capture.cpp
    Created: 26 Jun 2021 8:57:42pm
    Author:  rhall

  ==============================================================================
*/

#include "Capture.h"
#include "MainComponent.h"

Capture::Capture(Connector *c, std::string nm)
{
  name = nm;
  conn = c;
  points = std::vector<float>(nPoints);
}

void Capture::updatePoint(int index)
{
  SCValue *v = conn->value;
  float val = static_cast<float>(*v);
  points[index] = val;
}

void CaptureGrid::nodeInit()
{
    MidiInputNode* midiInputNode = new MidiInputNode("SysMidiInputNode");
    nodeTree.addNode(midiInputNode);
    MonoOsc1Node* monoOsc1Node = new MonoOsc1Node("SysMonoOsc1Node");
    nodeTree.addNode(monoOsc1Node);
    MonoOsc1Node* monoOsc1Node2 = new MonoOsc1Node("SysMonoOsc1Node2");
    nodeTree.addNode(monoOsc1Node2);
    AudioOutputNode* audioOutputNode = new AudioOutputNode("SysAudioOutputNode");
    nodeTree.addNode(audioOutputNode);
    Value8Node* value8Node = new Value8Node("Value8Node");
    nodeTree.addNode(value8Node);
    MSEGNode* msegNode = new MSEGNode("MSEGNode");
    msegNode->loadDesc("/home/rhall/JUCE/projects/SC_GUI/TestMSEG.json");
    nodeTree.addNode(msegNode);
    MSEGNode* msegNode2 = new MSEGNode("MSEGNode2");
    msegNode2->loadDesc("/home/rhall/JUCE/projects/SC_GUI/TestMSEG2.json");
    nodeTree.addNode(msegNode2);
    MonoControlOutputConnector* trueOut = dynamic_cast<MonoControlOutputConnector*>(value8Node->outputs["TrueOutput"]);
    MidiOutputConnector* midiOut = dynamic_cast<MidiOutputConnector*>(midiInputNode->outputs["MidiOutput"]);
    MidiInputConnector* midiIn =  dynamic_cast<MidiInputConnector*>(monoOsc1Node->inputs["MidiInput"]);
    createConnection(midiOut, midiIn);
    MonoControlOutputConnector* gateOut = dynamic_cast<MonoControlOutputConnector*>(midiInputNode->outputs["GateOutput"]);
    MonoControlInputConnector* gateIn =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node->inputs["GateInput"]);
    createConnection(trueOut, gateIn);
    MonoAudioOutputConnector* audioOut = dynamic_cast<MonoAudioOutputConnector*>(monoOsc1Node->outputs["AudioOutput"]);
    MonoAudioInputConnector* audioIn =  dynamic_cast<MonoAudioInputConnector*>(audioOutputNode->inputs["AudioInput"]);
    createConnection(audioOut, audioIn);
    MidiInputConnector* midiIn2 =  dynamic_cast<MidiInputConnector*>(monoOsc1Node2->inputs["MidiInput"]);
    createConnection(midiOut, midiIn2);
    MonoControlInputConnector* gateIn2 =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node2->inputs["GateInput"]);
    createConnection(trueOut, gateIn2);
    MonoAudioOutputConnector* audioOut2 = dynamic_cast<MonoAudioOutputConnector*>(monoOsc1Node2->outputs["AudioOutput"]);
    MonoAudioInputConnector* fmIn =  dynamic_cast<MonoAudioInputConnector*>(monoOsc1Node->inputs["FMInput"]);
    createConnection(audioOut2, fmIn);
    MonoControlOutputConnector* ratioOut = dynamic_cast<MonoControlOutputConnector*>(value8Node->outputs["V1Output"]);
    MonoControlInputConnector* ratioIn =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node2->inputs["RatioInput"]);
    createConnection(ratioOut, ratioIn);
    MonoControlInputConnector* gateIn3 =  dynamic_cast<MonoControlInputConnector*>(msegNode->inputs["GateInput"]);
    createConnection(gateOut, gateIn3);
    MonoControlInputConnector* levelIn =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node->inputs["LevelInput"]);
    MonoControlOutputConnector* msegOut = dynamic_cast<MonoControlOutputConnector*>(msegNode->outputs["MSEGOutput"]);
    createConnection(msegOut, levelIn);
    MonoControlInputConnector* gateIn4 =  dynamic_cast<MonoControlInputConnector*>(msegNode2->inputs["GateInput"]);
    createConnection(gateOut, gateIn4);
    MonoControlInputConnector* levelIn2 =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node2->inputs["LevelInput"]);
    MonoControlOutputConnector* msegOut2 = dynamic_cast<MonoControlOutputConnector*>(msegNode2->outputs["MSEGOutput"]);
    createConnection(msegOut2, levelIn2);
    MonoControlInputConnector* optionIn =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node->inputs["OptionInput"]);
    MonoControlOutputConnector* optionOut = dynamic_cast<MonoControlOutputConnector*>(value8Node->outputs["V3Output"]);
    createConnection(optionOut, optionIn);
    MonoControlInputConnector* optionIn2 =  dynamic_cast<MonoControlInputConnector*>(monoOsc1Node2->inputs["OptionInput"]);
    MonoControlOutputConnector* optionOut2 = dynamic_cast<MonoControlOutputConnector*>(value8Node->outputs["V4Output"]);
    createConnection(optionOut, optionIn);
    Capture* c1 = new Capture(audioOut, "Audio Out");
    addCapture(c1);
    Capture* c2 = new Capture(audioOut2, "Audio Out 2");
    addCapture(c2);
    Capture* c3 = new Capture(msegOut, "MSEG Out 1");
    addCapture(c3);
    Capture* c4 = new Capture(msegOut2, "MSEG Out 2");
    addCapture(c4);
    Capture* c5 = new Capture(gateOut, "Gate Out");
    addCapture(c5);
}

Connection* CaptureGrid::createConnection(OutputConnector *in, InputConnector *out, juce::String nm)
{
  Connection *conn = new Connection(nm);
  connectionTree.addConnection(conn);
  conn->connect(in, out);
  return conn;
}

int CaptureGrid::addCapture(Capture* c, std::string nm)
{
  if (size() >= maxCaptures)
  {
    return -1;
  }
  push_back(c);
  return 0;
}

void CaptureGrid::run(int startSample, int numSamples)
{
  AudioOutputNode* out = dynamic_cast<AudioOutputNode*>(nodeTree["SysAudioOutputNode"]);
  for (int s = startSample; s < numSamples; s++)
  {
    if (s == onSample)
    {
      startNote(36);
    }
    if (s == offSample)
    {
      stopNote();
    }
    nodeTree.clearAllReadyFlags();
    out->process(0, s);
    for (auto c: *this)
    {
      c->updatePoint(s);
    }
  }
  valid = true;
}

void CaptureGrid::startNote(int midiNoteNumber)
{
  MidiInputNode* node = dynamic_cast<MidiInputNode*>(nodeTree.getByName("SysMidiInputNode"));
  MidiNoteValue* val = node->getValue();
  val->start_time = std::chrono::system_clock::now().time_since_epoch().count();
  val->sample = 0;
  val->note = midiNoteNumber;
  val->vel = 127;
  node->setValue(val);
  SwitchValue* gate = new SwitchValue(true);
  gate->switchval = true;
  node->setGate(gate);
}

void CaptureGrid::stopNote()
{
  MidiInputNode* node = dynamic_cast<MidiInputNode*>(nodeTree.getByName("SysMidiInputNode"));
  MidiNoteValue* val = node->getValue();
  val->start_time = std::chrono::system_clock::now().time_since_epoch().count();
  val->sample = 0;
  val->note = 0;
  val->vel = 0;
  node->setValue(val);
  SwitchValue* gate = new SwitchValue{false};
  gate->switchval = false;
  node->setGate(gate);
}

