//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

int main() {

    AudioDescriptor* audio = init_standard_file("galaxy_collapse.wav", 12.8f);
    audio_init(audio);

    for (int i = 0; i < 8; i++) {
        write_note(audio, SYNTH_HARMONIC, 0.1f, D4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, G4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, A4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, D5);

        write_note(audio, SYNTH_HARMONIC, 0.1f, Ds4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, G4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, A4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, D5);

        write_note(audio, SYNTH_HARMONIC, 0.1f, E4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, G4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, A4);
        write_note(audio, SYNTH_HARMONIC, 0.1f, D5);

        write_note(audio, SYNTH_HARMONIC,0.1f, Ds4);
        write_note(audio, SYNTH_HARMONIC,0.1f, G4);
        write_note(audio, SYNTH_HARMONIC,0.1f, A4);
        write_note(audio, SYNTH_HARMONIC,0.1f, D5);
    }

    write_note_ts(audio, SYNTH_HARMONIC, 4.8f, 8.f, D3);

    audio_deinit(audio);

    return 0;
}