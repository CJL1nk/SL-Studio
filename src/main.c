//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

int main() {

    AudioDescriptor* audio = init_standard_file("rard.wav", 4.f);
    audio_init(audio);

    write_notes(audio, 0.5f, 2, A5, E5);
    write_note(audio, A4, 0.5f);
    write_note(audio, E5, 0.5f);
    write_note(audio, A4, 0.5f);

    write_notes(audio, 0.5f, 2, A5, E5);
    write_note(audio, A4, 0.5f);
    write_note(audio, E5, 0.5f);
    write_note(audio, D5, 0.5f);

    audio_deinit(audio);

    return 0;
}
