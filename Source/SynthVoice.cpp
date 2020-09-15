/*
  ==============================================================================

    SynthVoice.cpp
    Created: 8 Sep 2020 8:28:40pm
    Author:  Isaac

  ==============================================================================
*/

#include "SynthVoice.h"
#include "FMCalculations.h"


bool SynthVoice::canPlaySound(SynthesiserSound* sound) {
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}
    
void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currPitchWheelPosition) {
    currentAngle = 0.0;
    attackVolume = velocity * .2 ;
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


//sets duration for attack sustain and release as well as the volume for the sustain
void SynthVoice::setADSRinSamples(double att, double dec, double susVol, double rel) {
    attack = att;
    decay = dec;
    sustainVolume = susVol;
    release = rel;
}

void SynthVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (angleDelta != 0) {
        
        setADSRinSamples(getSampleRate() * .01, getSampleRate() * .1, attackVolume * .1, getSampleRate() * .020);
        
        for (int sample = 0; sample < numSamples; sample++){ 
            double currSample = 0;
            if (samplesSincePressed < attack) {
                currSample = std::sin(currentAngle) * (attackVolume + sustainVolume) * (samplesSincePressed / (attack));
            }
            else if (samplesSincePressed <= decay) {
                currSample = std::sin(currentAngle) * (attackVolume * ((decay + attack  - samplesSincePressed) / (attack)) + sustainVolume);
            }
            else {
                currSample = std::sin(currentAngle) * sustainVolume;
            }
            samplesSincePressed++;
            currentAngle += angleDelta;
            for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++) {
                outputBuffer.addSample(channel, startSample + sample, currSample);
            }
            
        }
    }
}