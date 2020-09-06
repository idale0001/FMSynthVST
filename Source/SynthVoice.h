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
        currentAngle = 0.0;
        level = velocity * 0.15;

        frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        double cyclesPerSample = frequency / getSampleRate();

        angleDelta = cyclesPerSample * 2 * juce::MathConstants<double>::pi;
    }

    void stopNote(float velocity, bool allowTailOff) {
        level = velocity;
        frequency = 0;
        currentAngle = 0;
        angleDelta = 0;
        clearCurrentNote();
    }

    void pitchWheelMoved(int newPitch) {
        frequency = MidiMessage::getMidiNoteInHertz(newPitch);
    }

    void controllerMoved(int collerNumber, int newControllerValue) {

    }

    void setSampleRate(int sampleRate) {
        currentSampleRate = sampleRate;
    }

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
        if (angleDelta != 0) {
            for (int sample = 0; sample < numSamples; sample++) {
                double currAmp = std::sin(currentAngle) * level;
                currentAngle += angleDelta;
                for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                    outputBuffer.addSample(0, startSample + sample, currAmp);
                }
            }
        }
    }

private:
    double level;
    double frequency;
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
};
