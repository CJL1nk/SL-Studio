//
// Created by CJ on 2/16/26.
//

#ifndef SL_STUDIO_SLSTUDIO_H
#define SL_STUDIO_SLSTUDIO_H

#include <stdio.h>
#include <stdint.h>

#define HEADER_SIZE 44

#define A4_FREQUENCY 440.0f

typedef struct {
    FILE* file;
    const char* filename;
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

typedef enum {
    SYNTH_SINE,
    SYNTH_SQUARE,
    SYNTH_HARMONIC,
    GRAND_PIANO
} Instrument;

/**
 * Initializes a standard, 44100hz, single channel WAV file header
 * @param filename Desired filename of song (abc.wav)
 * @param duration Duration of desired song
 * @return AudioDescriptor object to create file
 */
AudioDescriptor* init_standard_file(const char* filename, float duration);

float calculate_sample(float note, float time, Instrument instrument);

/**
 * Returns a sample for a standard sine wave synth
 * @param note Frequency of note (macros defined for all notes octaves 0-8)
 * @param time
 * @return Sample for a standard sine wave synth
 */
float synth_standard(float note, float time);

/**
 * Returns a sample for a standard square wave synth
 * @param note Frequency of note (macros defined for all notes octaves 0-8)
 * @param time tihng
 * @return Sample for a standard square wave synth
 */
float synth_square(float note, float time);

float synth_harmonic(float note, float time);

/**
 * Places a note at the end of a song
 * @param audio AudioDescriptor object to write into
 * @param instrument
 * @param duration Duration, in seconds, of note
 * @param note Frequency of note (macros defined for all notes octaves 0-8)
 */
void write_note(const AudioDescriptor* audio, Instrument instrument, float duration, float note);

/**
 * Places multiple notes on top of each other, at the end of a song
 * @param audio AudioDescriptor object to write into
 * @param instrument
 * @param duration Duration of *all* notes
 * @param count Number of notes to write
 * @param ... Frequencies of notes (macros defined for all notes octaves 0-8)
 */
void write_notes(const AudioDescriptor* audio, Instrument instrument, float duration, int count, ...);

/**
 * Places a note at a specific timestamp of your song
 * @param audio AudioDescriptor object to write into
 * @param instrument
 * @param timestamp Timestamp (in seconds) to place note at
 * @param duration Duration, in seconds, of note
 * @param note Frequency of note (macros defined for all notes octaves 0-8)
 */
void write_note_ts(const AudioDescriptor* audio, Instrument instrument, float timestamp, float duration, float note);

/**
 * Places multiple notes on top of each other, at a specific timestamp of your song
 * @param audio AudioDescriptor object to write into
 * @param instrument
 * @param timestamp Timestamp (in seconds) to places notes at
 * @param duration Duration of *all* notes
 * @param count Number of notes to write
 * @param ... Frequencies of notes (macros defined for all notes octaves 0-8)
 */
void write_notes_ts(const AudioDescriptor* audio, Instrument instrument, float timestamp, float duration, int count, ...);

/**
 * Initializes AudioDescriptor object for writing
 * @param audio AudioDescriptor object to initialize for writing
 * @return 0 on success
 */
int audio_init(AudioDescriptor* audio);

/**
 * De-initializes audio files for safe closing
 * @param audio AudioDescriptor object to de-initialize
 * @return 0 on success
 */
int audio_deinit(const AudioDescriptor* audio);

/**
 * Writes a 16-bit unsigned integer to a file, in little endian byte order
 * @param file File to write into
 * @param value Integer to write
 */
inline void write_u16_le(FILE *file, uint16_t value);

/**
 * Writes a 32-bit unsigned integer to a file, in little endian byte order
 * @param file File to write into
 * @param value Integer to write
 */
void write_u32_le(FILE *file, uint32_t value);

/**
 * Writes string binary to file
 * @param file File to write into
 * @param str String to write
 */
void write_string(FILE *file, const char* str);

// This is the best code ever written. There is no question.
// --- NOTES DECLARATION ---
float note_semitone(int semitones);
#define SILENCE 0.0f

#define C0 note_semitone(-57)
#define Cs0 note_semitone(-56)
#define D0 note_semitone(-55)
#define Ds0 note_semitone(-54)
#define E0 note_semitone(-53)
#define F0 note_semitone(-52)
#define Fs0 note_semitone(-51)
#define G0 note_semitone(-50)
#define Gs0 note_semitone(-49)
#define A0 note_semitone(-48)
#define As0 note_semitone(-47)
#define B0 note_semitone(-46)

#define C1 note_semitone(-45)
#define Cs1 note_semitone(-44)
#define D1 note_semitone(-43)
#define Ds1 note_semitone(-42)
#define E1 note_semitone(-41)
#define F1 note_semitone(-40)
#define Fs1 note_semitone(-39)
#define G1 note_semitone(-38)
#define Gs1 note_semitone(-37)
#define A1 note_semitone(-36)
#define As1 note_semitone(-35)
#define B1 note_semitone(-34)

#define C2 note_semitone(-33)
#define Cs2 note_semitone(-32)
#define D2 note_semitone(-31)
#define Ds2 note_semitone(-30)
#define E2 note_semitone(-29)
#define F2 note_semitone(-28)
#define Fs2 note_semitone(-27)
#define G2 note_semitone(-26)
#define Gs2 note_semitone(-25)
#define A2 note_semitone(-24)
#define As2 note_semitone(-23)
#define B2 note_semitone(-22)

#define C3 note_semitone(-21)
#define Cs3 note_semitone(-20)
#define D3 note_semitone(-19)
#define Ds3 note_semitone(-18)
#define E3 note_semitone(-17)
#define F3 note_semitone(-16)
#define Fs3 note_semitone(-15)
#define G3 note_semitone(-14)
#define Gs3 note_semitone(-13)
#define A3 note_semitone(-12)
#define As3 note_semitone(-11)
#define B3 note_semitone(-10)

#define C4 note_semitone(-9)
#define Cs4 note_semitone(-8)
#define D4 note_semitone(-7)
#define Ds4 note_semitone(-6)
#define E4 note_semitone(-5)
#define F4 note_semitone(-4)
#define Fs4 note_semitone(-3)
#define G4 note_semitone(-2)
#define Gs4 note_semitone(-1)
#define A4 A4_FREQUENCY
#define As4 note_semitone(1)
#define B4 note_semitone(2)

#define C5 note_semitone(3)
#define Cs5 note_semitone(4)
#define D5 note_semitone(5)
#define Ds5 note_semitone(6)
#define E5 note_semitone(7)
#define F5 note_semitone(8)
#define Fs5 note_semitone(9)
#define G5 note_semitone(10)
#define Gs5 note_semitone(11)
#define A5 note_semitone(12)
#define As5 note_semitone(13)
#define B5 note_semitone(14)

#define C6 note_semitone(15)
#define Cs6 note_semitone(16)
#define D6 note_semitone(17)
#define Ds6 note_semitone(18)
#define E6 note_semitone(19)
#define F6 note_semitone(20)
#define Fs6 note_semitone(21)
#define G6 note_semitone(22)
#define Gs6 note_semitone(23)
#define A6 note_semitone(24)
#define As6 note_semitone(25)
#define B6 note_semitone(26)

#define C7 note_semitone(27)
#define Cs7 note_semitone(28)
#define D7 note_semitone(29)
#define Ds7 note_semitone(30)
#define E7 note_semitone(31)
#define F7 note_semitone(32)
#define Fs7 note_semitone(33)
#define G7 note_semitone(34)
#define Gs7 note_semitone(35)
#define A7 note_semitone(36)
#define As7 note_semitone(37)
#define B7 note_semitone(38)

#define C8 note_semitone(39)
#define Cs8 note_semitone(40)
#define D8 note_semitone(41)
#define Ds8 note_semitone(42)
#define E8 note_semitone(43)
#define F8 note_semitone(44)
#define Fs8 note_semitone(45)
#define G8 note_semitone(46)
#define Gs8 note_semitone(47)
#define A8 note_semitone(48)
#define As8 note_semitone(49)
#define B8 note_semitone(50)
// -------------------------

#endif //SL_STUDIO_SLSTUDIO_H