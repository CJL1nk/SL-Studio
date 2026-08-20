# SL Studio

Time signatures? Bars? Eighth notes? Must have been a crazy dream bro, come on let's go place a note at 0:45:590

## Usage

### Notes

*Move `slstudio.cpp` and `slstudio.h` into your project, then `#include "slstudio.h"` in your `main.cpp`.*

The very easiest thing we can do is make a note. To do this, first, you'll need to initialize your audio.
```c++
Audio mysong = Audio("epicbeats123.wav");
```

This creates our audio object that we'll work around from here out. Now that this is instantiated, we can add a note to our audio.
```c++
mysong.append_note(SineSynth(D5, 4.f));
```
This will add a very bland, basic sine synth note into our song. The first argument is the note, all of which have macros defined from C0 to B8. The second argument is the duration of the note in seconds. This, quite obviously, makes the note hold for some *n* specified seconds.

<br>
Now that we have this, we can save and play our audio.

```c++
mysong.save();
```

After compiling, our song will be saved into the directory we ran the program from. If you take a listen, you should hear your note. This note will probably sound pretty bland, which is expected because it's a basic sine synth, which is nothing but a sine wave. Luckily, there's a few more instruments we can check out here. Currently, there are:
- SineSynth
- SquareSynth
- SawSynth
- HarmonicSynth

To use them, simply replace 'SineSynth' with the name of another instrument.
```c++
mysong.append_note(SquareSynth(D5, 4.f));
mysong.append_note(HarmonicSynth(D5, 4.f));
```

You may have already wondered "what happens if I want to place a note at a specific timestamp?" It just so happens that there's a function for that as well.
```c++
mysong.add_note(SawSynth(Gs4, 2.5f), 10.f);
```

The add_note() function takes two arguments: the note, and the timestamp. The note will be placed at the specified timestamp in your song, stacking the note on top of any other note(s) there. This is one method to create a chord, however, not the preferred method.

You also can control the volume of any single note by passing a 3rd (or 4th) argument into its constructor.
```c++
mysong.append_note(SawSynth(Gs4, 2.5f, 0.5f)); // 0.5 volume, default value is 1.0
```

Lastly, all notes can have a controlled "hold time" which can be useful for adding a small stop between 2 notes or letting a note have some form of decay (coming later). To add hold time, pass another argument into its constructor,
```c++
mysong.add_note(SawSynth(A5, 0.2f, 0.19f, 1.0f));
// Note lasts for 0.2 seconds and has a 0.19 second hold time, leaving a 0.01 span of silence perfect for appending another note with the same frequency.
```
When this is done, ensure you add the 4th volume argument, otherwise the hold time will be interpreted as the volume.

### Chords

Creating chords is relatively simple. To append a chord to the end of our song, you can do;
```c++
mysong.append_chord(Chord({
        new SquareSynth(F3, 4.f),
        new SquareSynth(A4, 4.f),
        new SquareSynth(C4, 4.f)
    }));
```
This makes a 4 second long F major chord with the F, A, and C notes. Chords can contain as many notes as you'd like, but it might not sound that great if you stack too many. The length of each note can be set as desired, so some notes can outlast others within the chord.

Similarly to notes, chords can also be placed at a specific timestamp.
```c++
mysong.add_chord(Chord({
        new HarmonicSynth(F3, 1.f),
        new HarmonicSynth(A4, 2.f),
        new HarmonicSynth(C4, 4.f)
    }), 3.5f);
```
This will place a chord 3.5 seconds into the song.

## Todo:

- Support stereo audio
- MIDI import support
- Support more file format exports
- Add effects
- Add more instruments