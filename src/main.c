//
// Created by CJ on 2/16/26.
//

#include "slstudio.h"

int main() {

    const AudioDescriptor* audio = init_standard_file("rard.wav", 1.f);

    write_notes(audio, 1.0f, 4, C5, E5, G5, B5);

    fclose(audio->file);

    return 0;
}
