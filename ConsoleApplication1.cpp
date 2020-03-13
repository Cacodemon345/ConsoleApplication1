
#include "CommonInclude.h"

std::vector<NoteRectangleShape*> rects;
sf::Texture texture;
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")
#endif
extern double lerp(double v0, double v1, double t);

constexpr auto SFMLMINRECTSIZE = 10.0;

sf::RenderWindow window(sf::VideoMode(800, 600), "Test");
sf::RectangleShape mfillrect(sf::Vector2f(window.getSize().x,window.getSize().y));
sf::Texture captureTex = sf::Texture();
sf::Sprite captureSprite = sf::Sprite(captureTex);
std::string shaderString = "";
sf::Shader custShaderObj = sf::Shader();
rtmidi::midi_in midiin = rtmidi::midi_in();
rtmidi::midi_out midiout = rtmidi::midi_out();

[[noreturn]] void rtMidiErrorCallback(rtmidi::midi_error type, std::string_view errorText)
{
    std::cout << "Error: " << errorText << std::endl;
    exit(-1);
}

int main()
{
    shaderString += "void main()";
    shaderString += "{";
    shaderString += "gl_FragColor = vec4(1.0,1.0,1.0,gl_FragColor.a);";
    shaderString += "}";
    custShaderObj.loadFromMemory(shaderString, sf::Shader::Fragment);
    captureTex.create(800, 600);
    window.setVisible(false);
    unsigned int i;
    midiin.set_error_callback(&rtMidiErrorCallback);
    midiout.set_error_callback(&rtMidiErrorCallback);
    auto num = midiin.get_port_count();
       
    for (i = 0; i < num; i++)
    {
        std::cout << i << ". " << midiin.get_port_name(i) << std::endl;
    }
    std::cout << "Select a device: ";
    int selection;
    scanf_s("%d", &selection);
    midiin.set_callback(MidiInMessage);
    midiin.open_port(selection);
    num = midiout.get_port_count();
    i = 0;
    for (i = 0; i < num; i++)
    {
        std::cout << i << ". " << midiout.get_port_name(i) << std::endl;
    }
    std::cout << "Select a device: ";
    scanf_s("%d", &selection);
    midiout.open_port(selection);

    srand(time(NULL));
    mfillrect.setFillColor(sf::Color(0,0,0,64));
    window.setVerticalSyncEnabled(true);
    for (i = 0; i < 16; i++)
    {
        auto midirect = new NoteRectangleShape();
        midirect->setSize(sf::Vector2f(window.getSize().x / 16, SFMLMINRECTSIZE));
        midirect->setPosition(midirect->getSize().x * (i + 1), window.getSize().y);
        midirect->rotate(180);
        midirect->setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        rects.push_back(midirect);
    }
    window.setVisible(true);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                auto oldColor = mfillrect.getFillColor();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
                {
                    oldColor.a -= 1;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
                {
                    oldColor.a += 1;
                }
                mfillrect.setFillColor(oldColor);
                std::string outstring(std::string("Current fade out: ") + std::to_string(mfillrect.getFillColor().a));
                std::cout << outstring;
            }
        }
        for (i = 0; i < rects.size(); i++)
        {
            if (!rects[i]->isNoteOn)
            {
                if (rects[i]->getSize().y > SFMLMINRECTSIZE)
                {
                    rects[i]->setSize(sf::Vector2f(rects[i]->getSize().x, rects[i]->getSize().y * .95));
                }
                else rects[i]->setSize(sf::Vector2f(rects[i]->getSize().x, SFMLMINRECTSIZE));
            }
            else
            {
                auto linearPointY = lerp(rects[i]->getSize().y, rects[i]->lerpInto, rects[i]->lerpFactor);
                rects[i]->setSize(sf::Vector2f(rects[i]->getSize().x, linearPointY));
            }
        }
        window.draw(captureSprite);
        window.draw(mfillrect);

        for (i = 0; i < rects.size(); i++)
        {
            auto oldFillColor = rects[i]->getFillColor();
            auto maxSize = window.getSize().y;
            auto curSize = rects[i]->getSize().y;
            auto newred = lerp(oldFillColor.r, 255, (curSize - SFMLMINRECTSIZE) / maxSize);
            clamp(newred, 0, 255);
            auto newgreen = lerp(oldFillColor.g, 255, (curSize - SFMLMINRECTSIZE) / maxSize);
            clamp(newgreen, 0, 255);
            auto newblue = lerp(oldFillColor.b, 255, (curSize - SFMLMINRECTSIZE) / maxSize);
            clamp(newblue, 0, 255);
            auto newFillColor = sf::Color(newred, newgreen, newblue, mfillrect.getFillColor().a);
            rects[i]->setFillColor(newFillColor);
            window.draw(*rects[i]);
            rects[i]->setFillColor(oldFillColor);
        }
        window.display();
        captureTex.update(window);
        captureSprite.setTexture(captureTex, true);
        captureSprite.setColor(sf::Color(1, 1, 1, 1));
    }

    return 0;
}
