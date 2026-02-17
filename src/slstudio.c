//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

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
    audio->FILE_SIZE = audio->NUM_SAMPLES * sizeof(uint16_t) + 44;
    audio->CHUNK_SIZE = audio->FILE_SIZE - 8;

    audio->SUBCHUNK2_SIZE = audio->NUM_SAMPLES * audio->BLOCK_ALIGN;

    audio->SUBCHUNK1_SIZE = 16;

    return audio;
}

void write_note(const AudioDescriptor* audio, const float duration, const float note) {

    // Write note samples to file
    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {
        const float time = (float)i / (float)audio->SAMPLE_RATE; // Quantum of time sample represents
        const float amplitude = 0.15f * sinf(time * note * 2.0f * M_PI);
        const int16_t sample = (int16_t)(amplitude * 32767);

        write_u16_le(audio->file, sample);
    }
}

void write_notes(const AudioDescriptor* audio, const float duration, const int count, ...) {

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
            amplitude += 0.15f * sinf(time * notes[j] * 2.0f * M_PI);
        }

        const int16_t sample = (int16_t)(amplitude * 32767);

        write_u16_le(audio->file, sample);
    }

    free(notes);
}

int audio_init(AudioDescriptor* audio) {

    audio->file = fopen(audio->filename, "wb");

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