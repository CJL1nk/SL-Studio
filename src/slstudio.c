//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

AudioDescriptor* init_standard_file(const char* filename, const float duration) {

    AudioDescriptor* audio = (AudioDescriptor*)malloc(sizeof(AudioDescriptor));
    audio->filename = filename;

    audio->SUBCHUNK1_SIZE = 16;
    audio->AUDIO_FORMAT = 1;
    audio->NUM_CHANNELS = 1;
    audio->SAMPLE_RATE = 41000;
    audio->BITS_PER_SAMPLE = 16;
    audio->BYTE_RATE = audio->SAMPLE_RATE * audio->NUM_CHANNELS * (audio->BITS_PER_SAMPLE / 8);
    audio->BLOCK_ALIGN = audio->NUM_CHANNELS * (audio->BITS_PER_SAMPLE / 8);

    audio->DURATION = duration;
    audio->NUM_SAMPLES = audio->DURATION * audio->SAMPLE_RATE;
    audio->FILE_SIZE = audio->NUM_SAMPLES * sizeof(uint16_t) + HEADER_SIZE;
    audio->CHUNK_SIZE = audio->FILE_SIZE - 8;

    audio->SUBCHUNK2_SIZE = audio->NUM_SAMPLES * audio->BLOCK_ALIGN;

    audio->SUBCHUNK1_SIZE = 16;

    return audio;
}

float calculate_amplitude(const Instrument instrument, const float note, const float time) {

    float sample = 0;

    switch (instrument) {
        case SYNTH_SINE:
            sample = synth_standard(note, time);
            break;
        case SYNTH_SQUARE:
            sample = synth_square(note, time);
            break;
        case SYNTH_HARMONIC:
            sample = synth_harmonic(note, time);
            break;
        default:
            break;
    }

    return sample;
}

float synth_standard(const float note, const float time) {
    return 0.15f * sinf(time * note * 2.0f * M_PI);
}

float synth_square(const float note, const float time) {
    const float period = 1.0f / note;
    const float phase = fmodf(time, period) / period;
    return (phase < 0.5f) ? 0.10f : -0.10f;
}

float synth_harmonic(const float note, const float time) {
    return
        (1.00f * sinf(2.0f * M_PI * note * time) +
        0.50f * sinf(2.0f * M_PI * note * 2 * time) +
        0.30f * sinf(2.0f * M_PI * note * 3 * time) +
        0.15f * sinf(2.0f * M_PI * note * 4 * time)) / 10;
}

void write_note(const AudioDescriptor* audio, const Instrument instrument, const float duration, const float note) {

    fseek(audio->file, 0, SEEK_END); // Go to end so stuff doesn't get overwritten

    // Write note samples to file
    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {
        const float time = (float)i / (float)audio->SAMPLE_RATE; // Quantum of time sample represents
        const float amplitude = calculate_amplitude(instrument, note, time);
        const int16_t sample = (int16_t)(amplitude * 32767);

        write_u16_le(audio->file, sample);
    }
}

void write_notes(const AudioDescriptor* audio, const Instrument instrument, const float duration, const int count, ...) {

    fseek(audio->file, 0, SEEK_END); // Go to end so stuff doesn't get overwritten

    va_list args;
    va_start(args, count);
    float* notes = malloc(count * sizeof(float));

    // Get all args from variable arg list, load into notes array cuz for some reason va_arg() consumes the object
    for (int i = 0; i < count; i++) {
        notes[i] = va_arg(args, double);
    }

    va_end(args);

    // Write note samples to file
    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {

        float amplitude = 0.0f;
        const float time = (float)i / (float)audio->SAMPLE_RATE; // Quantum of time sample represents

        for (int j = 0; j < count; j++) {
            amplitude += calculate_amplitude(instrument, notes[j], time);
        }

        const int16_t sample = (int16_t)(amplitude * 32767);

        write_u16_le(audio->file, sample);
    }

    free(notes);
}

void write_note_ts(const AudioDescriptor* audio, const Instrument instrument, const float timestamp, const float duration, const float note) {

    const int start_offset = (HEADER_SIZE + (sizeof(int16_t) * (timestamp * audio->SAMPLE_RATE)));
    fseek(audio->file, start_offset, SEEK_SET);

    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {

        // Get the current sample at that offset
        int16_t cur_sample = 0;
        fread(&cur_sample, sizeof(cur_sample), 1, audio->file);
        fseek(audio->file, -2, SEEK_CUR); // Reset back to correct offset cuz fread moves file pointer

        float cur_amplitude = (float)cur_sample / 32767.0f;

        const float time = (float)i / (float)audio->SAMPLE_RATE;
        cur_amplitude += calculate_amplitude(instrument, note, time);

        // Convert back to sample and write
        const int16_t sample = (int16_t)(cur_amplitude * 32767);
        write_u16_le(audio->file, sample);
    }
}

void write_notes_ts(const AudioDescriptor* audio, const Instrument instrument, const float timestamp, const float duration, const int count, ...) {

    int start_offset = (HEADER_SIZE + (sizeof(int16_t) * (timestamp * audio->SAMPLE_RATE)));
    fseek(audio->file, start_offset, SEEK_SET);

    va_list args;
    va_start(args, count);
    float* notes = malloc(count * sizeof(float));

    // Get all args from variable arg list, load into notes array cuz for some reason va_arg() consumes the object
    for (int i = 0; i < count; i++) {
        notes[i] = va_arg(args, double);
    }

    va_end(args);

    // Write note samples to file
    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {

        // Get the current sample at that offset
        int16_t cur_sample = 0;
        fread(&cur_sample, sizeof(cur_sample), 1, audio->file);
        fseek(audio->file, -2, SEEK_CUR); // Reset back to correct offset cuz fread moves file pointer

        float cur_amplitude = (float)cur_sample / 32767.0f;

        const float time = (float)i / (float)audio->SAMPLE_RATE; // Quantum of time sample represents
        for (int j = 0; j < count; j++) {
            cur_amplitude += calculate_amplitude(instrument, notes[j], time);
        }

        // Convert back to sample and write
        const int16_t sample = (int16_t)(cur_amplitude * 32767);
        write_u16_le(audio->file, sample);
    }

    free(notes);
}

int audio_init(AudioDescriptor* audio) {

    audio->file = fopen(audio->filename, "w+b");

    write_string(audio->file, "RIFF");
    write_u32_le(audio->file, audio->CHUNK_SIZE);
    write_string(audio->file, "WAVE");
    write_string(audio->file, "fmt ");
    write_u32_le(audio->file, audio->SUBCHUNK1_SIZE);
    write_u16_le(audio->file, audio->AUDIO_FORMAT);
    write_u16_le(audio->file, audio->NUM_CHANNELS);
    write_u32_le(audio->file, audio->SAMPLE_RATE);
    write_u32_le(audio->file, audio->BYTE_RATE);
    write_u16_le(audio->file, audio->BLOCK_ALIGN);
    write_u16_le(audio->file, audio->BITS_PER_SAMPLE);
    write_string(audio->file, "data");
    write_u32_le(audio->file, audio->SUBCHUNK2_SIZE);

    if (!audio->file) {
        return 1;
    }

    return 0;
}

int audio_deinit(const AudioDescriptor* audio) {
    return fclose(audio->file);
}

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