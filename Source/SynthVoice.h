/*
  ==============================================================================

    SynthVoice.h
    Created: 4 Sep 2020 9:49:46pm
    Author:  Isaac

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

using namespace juce;

class SynthVoice : public SynthesiserVoice
{
public:
    bool canPlaySound(SynthesiserSound* sound) {
        return dynamic_cast<SynthSound*>(sound) != nullptr;
    }
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currPitchWheelPosition) {
        level = velocity;
        frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }

    void stopNote(float velocity, bool allowTailOff) {
        level = velocity;
        frequency = 0;
        clearCurrentNote();
    }

    void pitchWheelMoved(int newPitch) {

    }

    void controllerMoved(int collerNumber, int newControllerValue) {

    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
        angleDelta = frequency / 24000;
        for (int sample = 0; sample < numSamples; sample++) {
            double currAmp = std::sin(currentAngle);
            currentAngle += angleDelta;
            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, startSample + sample, currAmp);
            }
        }
    }

private:
    double level;
    double frequency;
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
};
