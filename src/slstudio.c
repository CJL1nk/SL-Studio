//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

AudioDescriptor* init_standard_file(const char* filename, const float duration) {

    const float DURATION = duration;

    AudioDescriptor* audio = (AudioDescriptor*)malloc(sizeof(AudioDescriptor));
    audio->file = fopen(filename, "wb");

    audio->SUBCHUNK1_SIZE = 16;
    audio->AUDIO_FORMAT = 1;
    audio->NUM_CHANNELS = 1;
    audio->SAMPLE_RATE = 44100;
    audio->BITS_PER_SAMPLE = 16;
    audio->BYTE_RATE = audio->SAMPLE_RATE * audio->NUM_CHANNELS * (audio->BITS_PER_SAMPLE / 8);
    audio->BLOCK_ALIGN = audio->NUM_CHANNELS * (audio->BITS_PER_SAMPLE / 8);

    audio->NUM_SAMPLES = audio->DURATION * audio->SAMPLE_RATE;
    audio->FILE_SIZE = audio->NUM_SAMPLES * sizeof(uint16_t) + 44;
    audio->CHUNK_SIZE = audio->FILE_SIZE - 8;

    audio->SUBCHUNK2_SIZE = audio->NUM_SAMPLES * audio->BLOCK_ALIGN;

    const uint32_t SUBCHUNK1_SIZE = 16;

    write_string(audio->file, "RIFF");
    write_u32_le(audio->file, audio->CHUNK_SIZE);
    write_string(audio->file, "WAVE");
    write_string(audio->file, "fmt ");
    write_u32_le(audio->file, SUBCHUNK1_SIZE);
    write_u16_le(audio->file, audio->AUDIO_FORMAT);
    write_u16_le(audio->file, audio->NUM_CHANNELS);
    write_u32_le(audio->file, audio->SAMPLE_RATE);
    write_u32_le(audio->file, audio->BYTE_RATE);
    write_u16_le(audio->file, audio->BLOCK_ALIGN);
    write_u16_le(audio->file, audio->BITS_PER_SAMPLE);
    write_string(audio->file, "data");
    write_u32_le(audio->file, audio->SUBCHUNK2_SIZE);

    return audio;
}

void write_note(AudioDescriptor* audio, const float note, const float duration) {

    // Write not samples to file
    for (int i = 0; i < duration * (float)audio->SAMPLE_RATE; i++) {
        const float time = (float)i / (float)audio->SAMPLE_RATE; // Quantum of time sample represents
        const float amplitude = 0.15f * sinf(time * note * 2.0f * M_PI);
        const int16_t sample = (int16_t)(amplitude * 32767);

        write_u16_le(audio->file, sample);
    }
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