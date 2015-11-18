
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
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

int main(int, char const**)
{
    // Create the main window
    auto screen = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");
    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            int r = 127*(1+std::sin(2*M_PI*(float)x/WINDOW_WIDTH));
            sf::Color randCol(r, r, r);
            image.setPixel(x, y, randCol);
        }
    }
    
    sf::Image overlay;
    overlay.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Transparent);
    
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
//        std::cout << mousePos.x << " " << mousePos.y << std::endl;
        if (mousePos.x <= screen.width || mousePos.y <= screen.height) {
            sf::Vector2i windowPos(WINDOW_WIDTH * mousePos.x/screen.width,WINDOW_HEIGHT * mousePos.y/screen.height);
            auto pixel = image.getPixel(windowPos.x, windowPos.y);
            overlay.setPixel(windowPos.x, windowPos.y, sf::Color::Red);
            ovTex.loadFromImage(overlay);
//            std::cout << (int) pixel.r << std::endl;
        }
        
        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);
        window.draw(ovSpr);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
