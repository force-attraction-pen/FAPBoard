
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//
#include <iostream>
#include <cmath>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "arduino-serial-lib.c"
#include "arduino-serial-lib.h"

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

int main(int, char const**)
{
    char* buf[1000];
    const char* de = "/dev/cu.usbmodem1421";
    auto sp = serialport_init(de, 9600);


    // Create the main window
    auto screen = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "FAPBoard");
    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            int r = 127*(1-std::cos(2*M_PI*(float)x/WINDOW_WIDTH));
            sf::Color randCol(r, r, r);
            image.setPixel(x, y, randCol);
        }
    }
    
    // initialise overlay and font
    sf::Image overlay;
    overlay.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Transparent);
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("?", font, 50);
    text.setColor(sf::Color::Blue);
    
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);
    
    sf::Texture ovTex;
    ovTex.loadFromImage(overlay);
    sf::Sprite ovSpr(ovTex);
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        auto mousePos = sf::Mouse::getPosition();
        if (mousePos.x < screen.width || mousePos.y < screen.height) {
            sf::Vector2i windowPos(WINDOW_WIDTH * mousePos.x/screen.width,WINDOW_HEIGHT * mousePos.y/screen.height);
            auto pixel = image.getPixel(windowPos.x, windowPos.y);

            overlay.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Transparent);
            
            overlay.setPixel(windowPos.x, windowPos.y, sf::Color::Red);
            overlay.setPixel(windowPos.x+1, windowPos.y, sf::Color::Red);
            overlay.setPixel(windowPos.x, windowPos.y+1, sf::Color::Red);
            overlay.setPixel(windowPos.x+1, windowPos.y+1, sf::Color::Red);
            
            // set text to show the brightness (r, g and b are the same so doesn't matter which)
            sf::String s(std::to_string(pixel.r));
            text.setString(s);
            int wr = serialport_writebyte(sp, pixel.r);
            //std::cout << wr;
            auto read = serialport_read_until(sp, *buf, 'a', 10, 10);
            ovTex.loadFromImage(overlay);
        }
        
        window.clear();

        window.draw(sprite);
        window.draw(ovSpr);
        window.draw(text);

        window.display();
    }

    return EXIT_SUCCESS;
}
