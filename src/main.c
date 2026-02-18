//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

int main() {

    AudioDescriptor* audio = init_standard_file("layertest.wav", 4.f);
    audio_init(audio);

    write_note(audio, 4.f, A5);

    write_note_ts(audio, 1.f, 1.f, E5);
    write_notes_ts(audio, 2.f, 1.f, 2, B5, G5);

    audio_deinit(audio);

    return 0;
}

/*write_notes(audio, 0.5f, 2, A5, E5);
    write_note(audio, 0.5f, A4);
    write_note(audio, 0.5f, E5);
    write_note(audio, 0.5f, A4);

    write_notes(audio, 0.5f, 2, A5, E5);
    write_note(audio, 0.5f, A4);
    write_note(audio, 0.5f, E5);
    write_note(audio, 0.5f, D5);*/