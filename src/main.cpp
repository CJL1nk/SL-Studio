//
// Created by CJ on 2/16/26.
//

#include "slstudio.hpp"

int main() {

    Audio audio = Audio("galaxy_collapse.wav");

    for (int i = 0; i < 8; i++) {
        audio.append_note(HarmonicSynth(D4, 0.1f));
        audio.append_note(HarmonicSynth(G4, 0.1f));
        audio.append_note(HarmonicSynth(A4, 0.1f));
        audio.append_note(HarmonicSynth(D5, 0.1f));

        audio.append_note(HarmonicSynth(Ds4, 0.1f));
        audio.append_note(HarmonicSynth(G4, 0.1f));
        audio.append_note(HarmonicSynth(A4, 0.1f));
        audio.append_note(HarmonicSynth(D5, 0.1f));

        audio.append_note(HarmonicSynth(E4, 0.1f));
        audio.append_note(HarmonicSynth(G4, 0.1f));
        audio.append_note(HarmonicSynth(A4, 0.1f));
        audio.append_note(HarmonicSynth(D5, 0.1f));

        audio.append_note(HarmonicSynth(Ds4, 0.1f));
        audio.append_note(HarmonicSynth(G4, 0.1f));
        audio.append_note(HarmonicSynth(A4, 0.1f));
        audio.append_note(HarmonicSynth(D5, 0.1f));
    }


    audio.add_note(SquareSynth(C3, 0.4f, 0.5), 12.8);

    audio.add_chord(Chord({
        new SawSynth(F5, 0.4f),
        new SawSynth(C4, 0.4f),
    }), 13.2f);

    audio.add_note(SquareSynth(As3, 0.4f, 0.5), 13.6);

    audio.add_chord(Chord({
        new SawSynth(F5, 0.4f),
        new SawSynth(C4, 0.4f),
    }), 14.f);

    audio.add_note(SquareSynth(As3, 0.4f, 0.5), 14.4);

    audio.add_chord(Chord({
        new SawSynth(G5, 0.4f),
        new SawSynth(D4, 0.4f),
    }), 14.8f);


    audio.add_chord(Chord({
        new SawSynth(G5, 0.2f),
        new SawSynth(D4, 0.2f),
    }), 16.f);
    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_note(SawSynth(D5, 0.2f));
    audio.append_note(SawSynth(E5, 0.2f));
    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_note(SawSynth(D5, 0.2f));
    audio.append_note(SawSynth(E5, 0.2f));
    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_note(SawSynth(D5, 0.2f));


    audio.append_chord(Chord({
        new SawSynth(A5, 0.4f),
        new SawSynth(F5, 0.4f),
        new SawSynth(A4, 0.4f),
    }));

    audio.append_chord(Chord({
        new SawSynth(C6, 0.4f),
        new SawSynth(G5, 0.4f),
        new SawSynth(C5, 0.4f),
    }));

    audio.append_chord(Chord({
        new SawSynth(D6, 0.4f, 0.8f),
        new SawSynth(A5, 0.4f),
        new SawSynth(D5, 0.4f),
    }));

    audio.append_chord(Chord({
        new SawSynth(E6, 0.4f, 0.8f),
        new SawSynth(A5, 0.4f),
        new SawSynth(E5, 0.4f),
    }));

    audio.append_chord(Chord({
        new SawSynth(F6, 0.8f, 0.8f),
        new SawSynth(As5, 0.8f),
        new SawSynth(F5, 0.8f),
    }));


    audio.append_note(SawSynth(F6, 0.2f));
    audio.append_note(SawSynth(E6, 0.2f));
    audio.append_note(SawSynth(C6, 0.2f));
    audio.append_note(SawSynth(A5, 0.2f, 0.199f, 1.0f));
    audio.append_note(SawSynth(A5, 0.4f));


    audio.append_note(SawSynth(C6, 0.125f));
    audio.append_note(SawSynth(A5, 0.125f));
    audio.append_note(SawSynth(C6, 0.125f));
    audio.append_note(SawSynth(A5, 0.45f));

    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_note(SawSynth(E5, 0.2f));
    audio.append_chord(Chord({
        new SawSynth(G5, 0.6f),
        new SawSynth(D5, 0.6f),
    }));
    audio.append_chord(Chord({
        new SawSynth(A5, 0.6f),
        new SawSynth(E5, 0.6f),
    }));

    audio.append_note(SawSynth(E5, 0.2f));
    audio.append_note(SawSynth(F5, 0.2f));
    audio.append_chord(Chord({
        new SawSynth(D5, 0.4f),
        new SawSynth(A4, 0.4f),
    }));
    audio.append_chord(Chord({
        new SawSynth(C5, 0.4f),
        new SawSynth(G4, 0.4f),
    }));
    audio.append_chord(Chord({
        new SawSynth(A4, 0.6),
        new SawSynth(E4, 0.6f),
    }));

    audio.save();

    return 0;
}
