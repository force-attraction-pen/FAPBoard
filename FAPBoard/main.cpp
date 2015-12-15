
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
#include <random>
#include <algorithm>
#include <string>
#include <boost/scoped_ptr.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "arduino-serial-lib.c"
#include "arduino-serial-lib.h"

#include "ResourcePath.hpp"
#include "maze.cpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

int clamp(int n, int min, int max) {
    if (n < min) n = min;
    if (n > max) n = max;
    return n;
}

int getCellAverage(sf::Image *maze, int x, int y, int cellSize) {
    cellSize -= 5;
    sf::Vector2u s = maze->getSize();
    int startX = clamp(x - (cellSize/2), 0, s.x);
    int startY = clamp(y - (cellSize/2), 0, s.y);
    int sum = 0;
    std::cout <<"x:"<<x<<" y:"<<y<<" startX:"<<startX<<" startY:"<<startY<<" cellSize:"<<cellSize<<std::endl;
    for (int i = startX; i < startX + cellSize; i++) {
        for (int j = startY; j < startY + cellSize; j++) {
            sum += maze->getPixel(i, j).r;
        }
    }
    std::cout << "sum: " << sum << std::endl;
    return sum  / (cellSize * cellSize);
}

int main(int, char const**)
{
    char* buf[1000];
    const char* de = "/dev/cu.usbmodem1421";
    auto sp = serialport_init(de, 9600);

    // Create the main window
    auto screen = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "FAPBoard");
    Maze m(WINDOW_WIDTH, WINDOW_HEIGHT, 40);
    sf::Image *image = m.generate();

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
    texture.loadFromImage(*image);
    //texture.loadFromFile(resourcePath() + "maze.bmp");

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
            
            if (event.type == sf::Event::MouseButtonReleased) {
                image = m.generate();
                texture.loadFromImage(*image);
            }
        }
        auto mousePos = sf::Mouse::getPosition();
        if (mousePos.x < screen.width || mousePos.y < screen.height) {
            sf::Vector2i windowPos(WINDOW_WIDTH * mousePos.x/screen.width,WINDOW_HEIGHT * mousePos.y/screen.height);
            auto force = getCellAverage(image, windowPos.x, windowPos.y, m.getCellSize());

            overlay.create(WINDOW_WIDTH+1, WINDOW_HEIGHT+1, sf::Color::Transparent);
            
            overlay.setPixel(windowPos.x,   windowPos.y, sf::Color::Red);
            overlay.setPixel(windowPos.x+1, windowPos.y, sf::Color::Red);
            overlay.setPixel(windowPos.x,   windowPos.y+1, sf::Color::Red);
            overlay.setPixel(windowPos.x+1, windowPos.y+1, sf::Color::Red);
            
            // set text to show the brightness (r, g and b are the same so doesn't matter which)
            sf::String s(std::to_string(force));
            text.setString(s);
            int wr = serialport_writebyte(sp, force);
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
