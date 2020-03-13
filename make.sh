#!/bin/sh

g++ ConsoleApplication1.cpp WinMidiPlayback.cpp ./RtMidi17/rtmidi17/reader.cpp ./RtMidi17/rtmidi17/writer.cpp ./RtMidi17/rtmidi17/rtmidi17.cpp -D RTMIDI17_ALSA -I ./RtMidi17/ -include "sstream" -fpermissive -std=c++17 -lsfml-graphics -lsfml-window -lsfml-system -lgcc -lpthread -lasound

