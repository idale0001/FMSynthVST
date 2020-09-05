/*
  ==============================================================================

    SynthSound.h
    Created: 4 Sep 2020 9:49:29pm
    Author:  Isaac

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class SynthSound : public SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNumber*/) {
        return true;
    }

    bool appliesToChannel(int /*midiChannel*/) {
        return true;
    }
};

