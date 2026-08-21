//
// Created by CJ on 8/19/26.
//

#include "slstudio.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <random>

#define HEADER_SIZE 44

// ----- EFFECTS -------------------------------------------------------------------------------------
float Distortion::frequencyModulation(const float note_freq, const float time) const {
    return note_freq;
}

float Distortion::apply(const float amplitude, const float time) const {
    return amplitude;
}

float Vibrato::frequencyModulation(const float note_freq, const float time) const {
    return note_freq + this->depth * sinf(2.0f * M_PI * this->speed * time);
}

float Vibrato::apply(const float amplitude, const float time) const {
    return amplitude;
}


// ----- NOTES -------------------------------------------------------------------------------------
float SineSynth::calculate_amplitude(const float time, const float dt) const {

    // Frequency modulation for effects that use it
    float frequency = this->note_freq;
    for (int i = 0; i < this->effects.size(); i++) {
        frequency = this->effects[i]->frequencyModulation(this->note_freq, time);
        std::cout << "Frequency: " << frequency << std::endl;
    }

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    else if (time > this->hold_time) { attack_multiplier = 0.0f; }
    else if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    this->phase += frequency * 2.0f * M_PI * dt;
    if (this->phase >= 2.0f * M_PI) this->phase -= 2.0f * M_PI;

    float amplitude = 0.15f * this->volume * attack_multiplier;
    amplitude *= sinf(this->phase);

    // Post-processing
    for (int i = 0; i < this->effects.size(); i++) {
        amplitude = this->effects[i]->apply(amplitude, time);
    }

    return amplitude;
}

float SquareSynth::calculate_amplitude(const float time, const float dt) const {

    // Frequency modulation for effects that use it
    float frequency = this->note_freq;
    for (int i = 0; i < this->effects.size(); i++) {
        frequency = this->effects[i]->frequencyModulation(this->note_freq, time);
    }

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    else if (time > this->hold_time) { attack_multiplier = 0.0f; }
    else if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    const float volume = this->volume * 0.065f * attack_multiplier;
    this->phase += frequency * dt;
    this->phase -= floorf(this->phase);

    // Post-processing
    float amplitude = (this->phase < 0.5f) ? volume : -volume;
    for (int i = 0; i < this->effects.size(); i++) {
        amplitude = this->effects[i]->apply(amplitude, time);
    }

    return amplitude;
}

float SawSynth::calculate_amplitude(const float time, const float dt) const {

    // Frequency modulation for effects that use it
    float frequency = this->note_freq;
    for (int i = 0; i < this->effects.size(); i++) {
        frequency = this->effects[i]->frequencyModulation(this->note_freq, time);
    }

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    else if (time > this->hold_time) { attack_multiplier = 0.0f; }
    else if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    const float volume = (0.08f * this->volume * attack_multiplier);
    this->phase += frequency * dt;
    this->phase -= floorf(this->phase);

    // Post-processing
    float amplitude = volume * (2.0f * this->phase - 1.0f);
    for (int i = 0; i < this->effects.size(); i++) {
        amplitude = this->effects[i]->apply(amplitude, time);
    }
    return amplitude;
}

float HarmonicSynth::calculate_amplitude(const float time, const float dt) const {

    // Frequency modulation for effects that use it
    float frequency = this->note_freq;
    for (int i = 0; i < this->effects.size(); i++) {
        frequency = this->effects[i]->frequencyModulation(this->note_freq, time);
    }

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    else if (time > this->hold_time) { attack_multiplier = 0.0f; }
    else if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    this->phase1 += 2.0f * M_PI * frequency * 1.0f * dt;
    this->phase2 += 2.0f * M_PI * frequency * 2.0f * dt;
    this->phase3 += 2.0f * M_PI * frequency * 3.0f * dt;
    this->phase4 += 2.0f * M_PI * frequency * 4.0f * dt;

    float amplitude =
            ((1.00f * this->volume * attack_multiplier) * sinf(this->phase1) +
            (0.5f  * this->volume * attack_multiplier) * sinf(this->phase2) +
            (0.3f  * this->volume * attack_multiplier) * sinf(this->phase3) +
            (0.15f * this->volume * attack_multiplier) * sinf(this->phase4)) / 10;

    // Post-processing
    for (int i = 0; i < this->effects.size(); i++) {
        amplitude = this->effects[i]->apply(amplitude, time);
    }
    return amplitude;
}


// ----- AUDIO STUFF --------------------------------------------------------------------------------
Audio::Audio(const char* filename) {
    this->filename = filename;
    this->DURATION = 0;
    this->AUDIO_FORMAT = 1;
    this->NUM_CHANNELS = 1;
    this->SAMPLE_RATE = 41000;
    this->BITS_PER_SAMPLE = 16;
    this->BYTE_RATE = this->SAMPLE_RATE * this->NUM_CHANNELS * (this->BITS_PER_SAMPLE / 8);
    this->BLOCK_ALIGN = this->NUM_CHANNELS * (this->BITS_PER_SAMPLE / 8);
    this->SUBCHUNK1_SIZE = 16;
}

void Audio::append_note(const Note& note) {

    // Index to start in within amplitude buffer
    const size_t start_index = this->amplitudes.size();
    const size_t num_samples = (size_t)(note.duration * (float)this->SAMPLE_RATE);

    this->amplitudes.resize(start_index + num_samples);

    for (int i = 0; i < num_samples; i++) {
        const float time = (float)i / (float)this->SAMPLE_RATE; // Quantum of time sample represents
        const float dt = 1.0f / (float)this->SAMPLE_RATE;
        const float amplitude = note.calculate_amplitude(time, dt);

        this->amplitudes[start_index + i] = amplitude;
    }
}

void Audio::add_note(const Note& note, const float timestamp) {
    const size_t start_index = timestamp * (float)this->SAMPLE_RATE;
    const size_t num_samples = (size_t)(note.duration * (float)this->SAMPLE_RATE);

    if (start_index + num_samples > this->amplitudes.size()) {
        this->amplitudes.resize(start_index + num_samples);
    }

    for (int i = 0; i < num_samples; i++) {
        const float time = (float)i / (float)this->SAMPLE_RATE; // Quantum of time sample represents
        const float dt = 1.0f / (float)this->SAMPLE_RATE;
        float cur_amplitude = this->amplitudes[start_index + i];
        cur_amplitude += note.calculate_amplitude(time, dt);

        this->amplitudes[start_index + i] = cur_amplitude;
    }
}

void Audio::append_chord(const Chord& chord) {

    float longest_note = 0;
    for (int i = 0; i < chord.notes.size(); i++) {
        if (chord.notes[i]->duration > longest_note) {
            longest_note = chord.notes[i]->duration;
        }
    }

    // Index to start in within amplitude buffer
    const size_t start_index = this->amplitudes.size();
    const size_t num_samples = (size_t)(longest_note * (float)this->SAMPLE_RATE);

    this->amplitudes.resize(start_index + num_samples);

    for (int i = 0; i < chord.notes.size(); i++) {

        const Note& note = *chord.notes[i];
        const size_t curr_duration = (size_t)(note.duration * (float)this->SAMPLE_RATE);

        for (int j = 0; j < curr_duration; j++) {
            const float time = (float)j / (float)this->SAMPLE_RATE; // Quantum of time sample represents
            const float dt = 1.0f / (float)this->SAMPLE_RATE;
            const float amplitude = note.calculate_amplitude(time, dt);

            float curr_amplitude = this->amplitudes[start_index + j];
            this->amplitudes[start_index + j] = curr_amplitude + amplitude;
        }
    }
}

void Audio::add_chord(const Chord& chord, const float timestamp) {

    float longest_note = 0;
    for (int i = 0; i < chord.notes.size(); i++) {
        if (chord.notes[i]->duration > longest_note) {
            longest_note = chord.notes[i]->duration;
        }
    }

    // Index to start in within amplitude buffer
    const size_t start_index = timestamp * (float)this->SAMPLE_RATE;
    const size_t num_samples = (size_t)(longest_note * (float)this->SAMPLE_RATE);

    if (start_index + num_samples > this->amplitudes.size()) {
        this->amplitudes.resize(start_index + num_samples);
    }

    for (int i = 0; i < chord.notes.size(); i++) {

        const Note& note = *chord.notes[i];
        const size_t curr_duration = (size_t)(note.duration * (float)this->SAMPLE_RATE);

        for (int j = 0; j < curr_duration; j++) {
            const float time = (float)j / (float)this->SAMPLE_RATE; // Quantum of time sample represents
            const float dt = 1.0f / (float)this->SAMPLE_RATE;
            const float amplitude = note.calculate_amplitude(time, dt);

            float curr_amplitude = this->amplitudes[start_index + j];
            this->amplitudes[start_index + j] = curr_amplitude + amplitude;
        }
    }
}

bool Audio::save() {

    this->NUM_SAMPLES = this->amplitudes.size();
    this->DURATION = this->NUM_SAMPLES / (float)this->SAMPLE_RATE;
    this->SUBCHUNK2_SIZE = this->NUM_SAMPLES * this->BLOCK_ALIGN;
    this->FILE_SIZE = this->NUM_SAMPLES * sizeof(uint16_t) + HEADER_SIZE;
    this->CHUNK_SIZE = this->FILE_SIZE - 8;

    FILE* file = fopen(this->filename, "w+b");

    write_string(file, "RIFF");
    write_u32_le(file, CHUNK_SIZE);
    write_string(file, "WAVE");
    write_string(file, "fmt ");
    write_u32_le(file, SUBCHUNK1_SIZE);
    write_u16_le(file, AUDIO_FORMAT);
    write_u16_le(file, NUM_CHANNELS);
    write_u32_le(file, SAMPLE_RATE);
    write_u32_le(file, BYTE_RATE);
    write_u16_le(file, BLOCK_ALIGN);
    write_u16_le(file, BITS_PER_SAMPLE);
    write_string(file, "data");
    write_u32_le(file, SUBCHUNK2_SIZE);

    for (int i = 0; i < this->amplitudes.size(); i++) {
        const float amplitude = this->amplitudes[i];
        const int16_t sample = (int16_t)(amplitude * 32767);
        write_u16_le(file, (uint16_t)sample);
    }

    fclose(file);

    return true;
}


// ----- MACROS AND OTHER --------------------------------------------------------------------------------
void write_u16_le(FILE* file, const uint16_t value) {
    fputc(value & 0xFF, file);
    fputc(value >> 8, file);
}

void write_u32_le(FILE* file, const uint32_t value) {
    fputc(value & 0xFF, file);
    fputc((value >> 8) & 0xFF, file);
    fputc((value >> 16) & 0xFF, file);
    fputc(value >> 24, file);
}

void write_string(FILE *file, const char* str) {
    fwrite(str, 1, strlen(str), file);
}

float note_semitone(const int semitones) {
    return A4_FREQUENCY * powf(2.0f, semitones / 12.0f);
}