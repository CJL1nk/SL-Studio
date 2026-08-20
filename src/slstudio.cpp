//
// Created by CJ on 8/19/26.
//

#include "slstudio.hpp"
#include <cmath>
#include <cstring>
#include <iostream>

#define HEADER_SIZE 44

// ----- EFFECTS -------------------------------------------------------------------------------------
float Distortion::apply(const float amplitude) const {
    return fmaxf(-1.0f, fminf(1.0f, amplitude * this->amount));
}


// ----- NOTES -------------------------------------------------------------------------------------
float SineSynth::calculate_amplitude(const float time) const {
    if (time >= this->hold_time) { return 0.0f; } // Will fix this later

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    float amplitude = 0.15f * this->volume * attack_multiplier;
    return amplitude * sinf(time * this->note_freq * 2.0f * M_PI);
}

float SquareSynth::calculate_amplitude(const float time) const {
    if (time >= this->hold_time) { return 0.0f; } // Will fix this later

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    const float volume = this->volume * 0.065f * attack_multiplier;
    const float period = 1.0f / this->note_freq;
    const float phase = fmodf(time, period) / period;
    return (phase < 0.5f) ? volume : -volume;
}

float SawSynth::calculate_amplitude(const float time) const {
    if (time >= this->hold_time) { return 0.0f; } // Will fix this later

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }

    const float period = 1.0f / this->note_freq;
    const float phase = fmodf(time, period) / period;  // 0 to 1 over one period
    return (0.08f * this->volume * attack_multiplier) * (2.0f * phase - 1.0f);  // -1 to +1
}

float HarmonicSynth::calculate_amplitude(const float time) const {
    if (time >= this->hold_time) { return 0.0f; } // Will fix this later

    float attack_multiplier = 1.0f;
    if (time < this->attack_time) { attack_multiplier = time / this->attack_time; }
    if (time > this->hold_time - this->release_time) { attack_multiplier = (this->hold_time - time) / this->release_time; }
    return
        ((1.00f * this->volume * attack_multiplier) * sinf(2.0f * M_PI * this->note_freq * time) +
        (0.5f * this->volume * attack_multiplier) * sinf(2.0f * M_PI * this->note_freq * 2 * time) +
        (0.3f * this->volume * attack_multiplier) * sinf(2.0f * M_PI * this->note_freq * 3 * time) +
        (0.15f * this->volume * attack_multiplier) * sinf(2.0f * M_PI * this->note_freq * 4 * time)) / 10;
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
        const float amplitude = note.calculate_amplitude(time);

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
        float cur_amplitude = this->amplitudes[start_index + i];
        cur_amplitude += note.calculate_amplitude(time);

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
            const float amplitude = note.calculate_amplitude(time);

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
            const float amplitude = note.calculate_amplitude(time);

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