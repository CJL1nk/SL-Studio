//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

int main() {

    AudioDescriptor* audio = init_standard_file("rard.wav", 2.5f);

    write_note(audio, G5, 1.f);
    write_note(audio, E5, 0.5f);
    write_note(audio, D5, 0.5f);
    write_note(audio, As4, 0.5f);

    fclose(audio->file);

    return 0;
}
