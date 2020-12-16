/*
  ==============================================================================

    SynthVoice.cpp
    Created: 8 Sep 2020 8:28:40pm
    Author:  Isaac

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(SynthesiserSound* sound) {
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}
    
void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currPitchWheelPosition) {
    currentAngle = 0.0;
    attackVolume = velocity * .15;
    samplesSincePressed = 0;
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    double cyclesPerSample = frequency / getSampleRate();

    angleDelta = cyclesPerSample * 2 * MathConstants<double>::pi;
}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    attackVolume = velocity;
    frequency = 0;
    currentAngle = 0;
    angleDelta = 0;
    clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitch) {
    frequency = MidiMessage::getMidiNoteInHertz(newPitch);
}

void SynthVoice::controllerMoved(int collerNumber, int newControllerValue) {

}


void SynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (angleDelta != 0) {
        for (int sample = 0; sample < numSamples; sample++){ 
            double currSample = waveCalc(currentAngle, samplesSincePressed);
            samplesSincePressed++;
            currentAngle += angleDelta;
            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, startSample + sample, currSample);
            }
            
        }
    }
}

double SynthVoice::waveCalc(double currentAngle, int samplesSincePressed) {
    if (samplesSincePressed < attack) {
        return sin(currentAngle) * attackVolume * (samplesSincePressed * 1.0 / attack);
    }
    else if (samplesSincePressed <= decay) {
        return sin(currentAngle) * ((attackVolume - sustainVolume) * (1 - (samplesSincePressed - attack) * 1.0 / (decay - attack)) + sustainVolume);
    }
    else {
        return sin(currentAngle) * sustainVolume;
    }
}

void SynthVoice::setADSR(int att, int dec, double susVol, int rel) {
    attack = att;
    decay = dec + attack;
    sustainVolume = susVol;
    release = rel + decay;
}