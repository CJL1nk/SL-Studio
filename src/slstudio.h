//
// Created by CJ on 2/16/26.
//

#ifndef SL_STUDIO_SLSTUDIO_H
#define SL_STUDIO_SLSTUDIO_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct {
    FILE* file;
    float DURATION; // Might make this get auto-set

    uint32_t NUM_SAMPLES;
    uint32_t FILE_SIZE;
    uint32_t CHUNK_SIZE;

    uint32_t SUBCHUNK1_SIZE;
    uint16_t AUDIO_FORMAT;
    uint16_t NUM_CHANNELS;
    uint32_t SAMPLE_RATE;
    uint16_t BITS_PER_SAMPLE;
    uint32_t BYTE_RATE;
    uint16_t BLOCK_ALIGN;

    uint32_t SUBCHUNK2_SIZE;
} AudioDescriptor;

typedef struct {
    float note;
    float duration;
} Note;

typedef struct {
    Note* notes;
} NoteStack;

AudioDescriptor* init_standard_file(const char* filename, float duration);
void write_note(const AudioDescriptor* audio, float note, float duration);
void write_notes(const AudioDescriptor* audio, float duration, int count, ...);
void write_u16_le(FILE *file, uint16_t value);
void write_u32_le(FILE *file, uint32_t value);
void write_string(FILE *file, const char* str);

// --- NOTES DECLARATION ---
#define SILENCE 0.0f

#define A0 27.50f
#define A1 55.0f
#define A2 110.0f
#define A3 220.0f
#define A4 440.0f
#define A5 880.0f
#define A6 1760.0f
#define A7 3520.0f
#define A8 7040.0f

#define As0 29.14f
#define As1 58.27f
#define As2 116.54f
#define As3 233.08f
#define As4 466.16f
#define As5 932.33f
#define As6 1864.66f
#define As7 3729.31f
#define As8 7458.62f

#define B0 30.87f
#define B1 61.74f
#define B2 123.47f
#define B3 246.94f
#define B4 493.88f
#define B5 987.77f
#define B6 1975.53f
#define B7 3951.07f
#define B8 7902.13f

#define C0 16.35f
#define C1 32.70f
#define C2 65.41f
#define C3 130.81f
#define C4 261.63f
#define C5 523.25f
#define C6 1046.50f
#define C7 2093.0f
#define C8 4186.01f

#define Cs0 17.32f
#define Cs1 34.65f
#define Cs2 69.30f
#define Cs3 138.59f
#define Cs4 277.18f
#define Cs5 554.37f
#define Cs6 1108.73f
#define Cs7 2217.46f
#define Cs8 4434.92f

#define D0 18.35f
#define D1 36.71f
#define D2 73.42f
#define D3 146.83f
#define D4 293.66f
#define D5 587.33f
#define D6 1174.66f
#define D7 2349.32f
#define D8 4698.63f

#define Ds0 19.45f
#define Ds1 38.89f
#define Ds2 77.78f
#define Ds3 155.56f
#define Ds4 311.13f
#define Ds5 622.25f
#define Ds6 1244.51f
#define Ds7 2489.02f
#define Ds8 4978.03f

#define E0 20.60f
#define E1 41.20f
#define E2 82.41f
#define E3 164.81f
#define E4 329.63f
#define E5 659.25f
#define E6 1318.51f
#define E7 2637.02f
#define E8 5274.04f

#define F0 21.83f
#define F1 43.65f
#define F2 87.31f
#define F3 174.61f
#define F4 349.23f
#define F5 698.46f
#define F6 1396.91f
#define F7 2793.83f
#define F8 5587.65f

#define Fs0 23.12f
#define Fs1 46.25f
#define Fs2 92.50f
#define Fs3 185f
#define Fs4 369.99f
#define Fs5 739.99f
#define Fs6 1479.98f
#define Fs7 2959.96f
#define Fs8 5919.91f

#define G0 24.50f
#define G1 49.0f
#define G2 98.0f
#define G3 196.0f
#define G4 392.0f
#define G5 783.99f
#define G6 1567.98f
#define G7 3135.96f
#define G8 6271.93f

#define Gs0 25.96f
#define Gs1 51.91f
#define Gs2 103.83f
#define Gs3 207.65f
#define Gs4 415.30f
#define Gs5 830.61f
#define Gs6 1661.22f
#define Gs7 3322.44f
#define Gs8 6644.88f
// -------------------------

#endif //SL_STUDIO_SLSTUDIO_H