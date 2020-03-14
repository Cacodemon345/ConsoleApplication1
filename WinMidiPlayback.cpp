#include "CommonInclude.h"
#include <iomanip>
extern sf::RenderWindow window;

void MidiInMessage(const rtmidi::message& message)
{
    unsigned char  lobyted = message.bytes.at(0);
    unsigned char  lobyted2 = message.bytes.at(1);
    unsigned char hibyted;
    try {
        hibyted = message.bytes.at(2);
    }
    catch (std::out_of_range &outofrange)
    {
        hibyted = 0;
    }
    printf("Bytes found: ");
    for (auto bytes : message.bytes)
    {
        printf("0x%x,",bytes);
    }
    //printf("Bytes found: 0x%x, 0x%x, 0x%x",lobyted,lobyted2,hibyted);
    std::cout << std::endl;
    switch (message.get_message_type())
    {
    case rtmidi::message_type::CONTROL_CHANGE:
        if (lobyted2 == 0x7B)
        {
            for (int i = 0; i < 16; i++)
            {
                rects[i]->isNoteOn = false;
            }
        }
        break;
    case rtmidi::message_type::NOTE_OFF:
        std::cout << "Note Off" << std::endl;
        rects[message.get_channel()]->isNoteOn = false;
        break;
    case rtmidi::message_type::NOTE_ON:
        std::cout << "Note On" << std::endl;
        double value = hibyted / 127;
        int highbytevalue = lerp(window.getSize().y, 0, value);
        rects[message.get_channel()]->lerpInto = highbytevalue;
        rects[message.get_channel()]->isNoteOn = true;
        if ((message.get_channel()) == 0x9) rects[message.get_channel()]->lerpFactor = 0.25;
        break;
    }
    midiout.send_message(message);
}
