//
//  maze.cpp
//  FAPBoard
//
//  Created by Alexander Hill on 15/12/2015.
//  Copyright © 2015 Alexander Hill. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

class Maze {
public:
    Maze(int w, int h, int cs) {
        width = w;
        height = h;
        cellSize = cs;
    }
    
    int getCellSize() {
        return cellSize;
    }
private:
    enum Dir {
        NONE = 0,
        N = 1 << 1,
        S = 1 << 2,
        W = 1 << 3,
        E = 1 << 4
    };
    
    typedef struct edge {
        int i; // index
        Dir d; // direction
    } Edge;

    int cellSize = 40;
    int cellWidth;
    int cellHeight;
    Dir *cells;
    sf::Image *maze;
    
    int width, height;
    
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
    
    void reset() {
        cellWidth = (width-cellSize)/(cellSize + cellSize);
        cellHeight = (height-cellSize)/(cellSize + cellSize);
        
        cells = new Dir[cellWidth * cellHeight]();
        
        maze = new sf::Image;
        frontier.clear();
        maze->create(width, height, sf::Color::Black);
    }
    
public:
    // create a simple maze using recursive division method - see wikipedia for details
    sf::Image *generate() {
        reset();
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
    
};
