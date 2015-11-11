
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
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

int main(int, char const**)
{
    // Create the main window
    auto f = sf::VideoMode::getFullscreenModes()[0];
    f.width = 1920*2;
    f.height = 1200*2;
    sf::RenderWindow window(sf::VideoMode(f.width, f.height), "SFML window");

    sf::Image image;
    image.create(f.width, f.height);
    
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("Hello SFML", font, 50);
    text.setColor(sf::Color::Black);

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
        sf::Uint8 *pixels = new sf::Uint8[4]();
        image.setPixel(mousePos.x, mousePos.y, sf::Color::White);
        for (int i = 0; i <= 2; i++) {
            image.setPixel(mousePos.x + i, mousePos.y, sf::Color::White);
            image.setPixel(mousePos.x, mousePos.y + i, sf::Color::White);
            image.setPixel(mousePos.x + i, mousePos.y + i, sf::Color::White);
        }
        texture.loadFromImage(image);

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
