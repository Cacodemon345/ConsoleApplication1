#include "CommonInclude.h"
extern sf::RenderWindow window;

void MidiInMessage(const rtmidi::message& message)
{
    auto lobyted = message.bytes.at(0);
    auto lobyted2 = message.bytes.at(1);
    unsigned char hibyted;
    try {
        hibyted = message.bytes.at(2);
    }
    catch (std::out_of_range &outofrange)
    {
        hibyted = 0;
    }
    if (lobyted == 0xFF)
    {
        for (int i = 0; i < 16; i++)
        {
            rects[i]->isNoteOn = false;
        }
        return;
    }
    switch (lobyted & 0xF0)
    {
    case 0xB0:
        if (lobyted2 == 0x7B)
        {
            for (int i = 0; i < 16; i++)
            {
                rects[i]->isNoteOn = false;
            }
        }
        break;
    case 0x80:
        rects[lobyted & 0x0F]->isNoteOn = false;
        break;
    case 0x90:
        double value = hibyted / 127;
        int highbytevalue = lerp(window.getSize().y, 0, value);
        rects[lobyted & 0x0F]->lerpInto = highbytevalue;
        rects[lobyted & 0x0F]->isNoteOn = true;
        if ((lobyted & 0x0F) == 0x9) rects[lobyted & 0x0F]->lerpFactor = 0.25;
        break;
    }
    midiout.send_message(message);
}
