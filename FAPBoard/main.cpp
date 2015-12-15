
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

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

enum Dir {
    NONE = 0,
    N = 1 << 1,
    S = 1 << 2,
    W = 1 << 3,
    E = 1 << 4
};

int cellSize = 40;
int cellWidth;
int cellHeight;
Dir *cells;
sf::Image *maze;

typedef struct edge {
    int i; // index
    Dir d; // direction
} Edge;

std::vector<Edge*> frontier;

void fillCell(int index, sf::Color color, Dir direction = NONE) {
    int i = index % cellWidth,
        j = index / cellWidth;

    sf::Image block;
    block.create(cellSize, cellSize, color);
    
    if (direction & (S | E)) {
        if (direction == E) {
            maze->copy(block, (i+1) * (cellSize + cellSize), j * cellSize + (j+1) * cellSize);
        } else if (direction == S) {
            maze->copy(block, i*cellSize + (i+1) * cellSize, (j+1)*(cellSize + cellSize));
        }
    } else {
        maze->copy(block, i*cellSize + (i+1) * cellSize, j*cellSize + (j+1)*cellSize);
    }
}

// create a simple maze using recursive division method - see wikipedia for details
sf::Image *createMaze(unsigned int width, unsigned int height) {

    cellWidth = (width-cellSize)/(cellSize + cellSize);
    cellHeight = (height-cellSize)/(cellSize + cellSize);
    
    cells = new Dir[cellWidth * cellHeight]();
    
    maze = new sf::Image;
    frontier.clear();
    maze->create(width, height, sf::Color::Black);
    
    frontier.push_back(new Edge{0, S});
    frontier.push_back(new Edge{0, E});
    fillCell(0, sf::Color::White);
    
    while (frontier.size() > 0) {
        Edge *e = frontier.back();
        frontier.pop_back();
        
        int i = e->i;
        Dir d = e->d;
        
        int newI = i + (d == N ? -cellWidth : d == S ? cellWidth : d == W ? -1 : 1);
        int x = e->i % cellWidth;
        int y = e->i / cellWidth;
        int newX, newY, newDir;
        bool open = cells[newI] == NONE;
        sf::Color cellColour = open ? sf::Color::White : sf::Color::Black;
        
        switch (d) {
            case N:
                fillCell(newI, cellColour, S);
                newX = x;
                newY = y - 1;
                newDir = S;
                break;
            case S:
                fillCell(i, cellColour, S);
                newX = x;
                newY = y + 1;
                newDir = N;
                break;
            case W:
                fillCell(newI, cellColour, E);
                newX = x - 1;
                newY = y;
                newDir = E;
                break;
            case E:
                fillCell(i, cellColour, E);
                newX = x + 1;
                newY = y;
                newDir = W;
                break;
            default:
                std::cout << "WTF????\n";
                break;
        }
        
        if (open) {
            fillCell(newI, cellColour);
            cells[i] = (Dir)(d | cells[i]);
            cells[newI] = (Dir)(d | cells[newI]);
            
            if (newY > 0 && cells[newI - cellWidth] == NONE) {
                frontier.push_back(new Edge{newI, N});
            }
            if (newY < cellHeight - 1 && cells[newI + cellWidth] == NONE) {
                frontier.push_back(new Edge{newI, S});
            }
            if (newX > 0 && cells[newI - 1] == NONE) {
                frontier.push_back(new Edge{newI, W});
            }
            if (newX < cellWidth - 1 && cells[newI + 1] == NONE) {
                frontier.push_back(new Edge{newI, E});
            }
            std::random_shuffle(frontier.begin(), frontier.end());
        }
    }
    
    return maze;
}

int main(int, char const**)
{
    char* buf[1000];
    const char* de = "/dev/cu.usbmodem1421";
    auto sp = serialport_init(de, 9600);


    // Create the main window
    auto screen = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "FAPBoard");
    sf::Image *image = createMaze(WINDOW_WIDTH, WINDOW_HEIGHT);
//    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
//    for (int x = 0; x < WINDOW_WIDTH; x++) {
//        for (int y = 0; y < WINDOW_HEIGHT; y++) {
//            int r = 127*(1-std::cos(2*M_PI*(float)x/WINDOW_WIDTH));
//            sf::Color randCol(r, r, r);
//            image.setPixel(x, y, randCol);
//        }
//    }
//    image->loadFromFile(resourcePath() + "maze.bmp");
    
    // initialise overlay and font
    sf::Image overlay;
    overlay.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Transparent);
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("?", font, 50);
    text.setColor(sf::Color::Blue);
    sf::Vector2f targetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Texture texture;
    texture.loadFromImage(*image);
    //texture.loadFromFile(resourcePath() + "maze.bmp");

    sf::Sprite sprite(texture);
    sprite.setScale(
                    targetSize.x / sprite.getLocalBounds().width,
                    targetSize.y / sprite.getLocalBounds().height);
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
            auto pixel = image->getPixel(windowPos.x, windowPos.y);

            overlay.create(WINDOW_WIDTH+1, WINDOW_HEIGHT+1, sf::Color::Transparent);
            
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
