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

    audio.append_note(HarmonicSynth(D4, 8.f));

    audio.add_chord(Chord({
        new SquareSynth(F3, 1.f),
        new SquareSynth(A4, 2.f),
        new SquareSynth(C4, 4.f)
    }), 14.8f);

    audio.add_note(HarmonicSynth(E5, 3.f), 1.f);

    audio.save();

    return 0;
}
