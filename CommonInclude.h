#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <exception>
#include <string>
#include "RtMidi17.h"


extern rtmidi::midi_in midiin;
extern rtmidi::midi_out midiout;
void MidiInMessage(const rtmidi::message& message);

#define clamp(v,lo,hi) (v < lo) ? lo : (hi < v) ? hi : v;
class NoteRectangleShape : public sf::RectangleShape
{
public:
    bool isNoteOn = false;
    float lerpInto = 0.0;
    float lerpFactor = 0.05;
};
extern std::vector<NoteRectangleShape*> rects;
inline double lerp(double v0, double v1, double t) {
    return (1 - t) * v0 + t * v1;
}
