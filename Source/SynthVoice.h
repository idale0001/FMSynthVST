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
   
    bool canPlaySound(SynthesiserSound* sound);

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currPitchWheelPosition);

    void stopNote(float velocity, bool allowTailOff);

    void pitchWheelMoved(int newPitch);

    void controllerMoved(int collerNumber, int newControllerValue);

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples);

    void setADSRinSamples(double att, double dec, double susVol, double rel);

    double getAttackVolume() { return attackVolume; }

private:
    double attackVolume;
    double frequency;
    double currentSampleRate, currentAngle, angleDelta;
    bool isReleasing;
    int attack, decay, release, samplesSincePressed;
    double sustainVolume;
};
