//
// Created by CJ on 8/19/26.
//

#ifndef SL_STUDIO_SLSTUDIO_H
#define SL_STUDIO_SLSTUDIO_H

#define A4_FREQUENCY 440.0f

#include <cstdio>
#include <vector>
#include <bits/stdint-uintn.h>

// ----- EFFECTS -------------------------------------------------------------------------------------
class Effect {

};

class Distortion : public Effect {
    public:
        float amount;
};



// ----- NOTES -------------------------------------------------------------------------------------
class Note {
    public:
        virtual ~Note() = default;

        float note_freq;
        float duration;
        float volume;
        std::vector<Effect> effects;

        Note(const float note_freq, const float duration) : note_freq(note_freq), duration(duration), volume(1.f){};
        Note(const float note_freq, const float duration, const float volume) : note_freq(note_freq), duration(duration), volume(volume){};

        [[nodiscard]] virtual float calculate_amplitude(float note_freq, float time) const = 0;
};

class SineSynth final : public Note {
    public:
        SineSynth(const float note_freq, const float duration, const float volume = 1.f) : Note(note_freq, duration, volume){};
        [[nodiscard]] float calculate_amplitude(float note_freq, float time) const override;
};

class SquareSynth final : public Note {
    public:
        SquareSynth(const float note_freq, const float duration, const float volume = 1.f) : Note(note_freq, duration, volume){};
        [[nodiscard]] float calculate_amplitude(float note_freq, float time) const override;
};

class HarmonicSynth final : public Note {
    public:
        HarmonicSynth(const float note_freq, const float duration, const float volume = 1.f) : Note(note_freq, duration, volume){};
        [[nodiscard]] float calculate_amplitude(float note_freq, float time) const override;
};


class Chord {
    public:
        std::vector<Note*> notes;
        explicit Chord(const std::vector<Note*> &notes) : notes(notes){};
};

// ----- AUDIO STUFF --------------------------------------------------------------------------------
class Audio {

    public:
        explicit Audio(const char* filename);

        void append_note(const Note& note);
        void add_note(const Note& note, float timestamp);
        void append_chord(const Chord& chord);
        void add_chord(const Chord& chord, float timestamp);

        bool save();

    private:
        std::vector<float> amplitudes;

        // Header stuff
        const char* filename;

        float DURATION;
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
};



// ----- MACROS AND OTHER --------------------------------------------------------------------------------
void write_u16_le(FILE* file, uint16_t value);
void write_u32_le(FILE* file, uint32_t value);
void write_string(FILE *file, const char* str);

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


#endif //SL_STUDIO_SLSTUDIO_H