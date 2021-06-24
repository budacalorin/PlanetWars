#ifndef SEGMENT_H
#define SEGMENT_H
#include <SFML/Graphics.hpp>

class segment
{
public:
    bool N,E,S,V;
    int tip_bulina;
    bool selectat;

    sf::RectangleShape forma;

    sf::RectangleShape zid_N;
    sf::RectangleShape zid_E;
    sf::RectangleShape zid_S;
    sf::RectangleShape zid_V;


    void setup(int x,int y, int latura);

    void redimensionare(int x,int y,int latura);
};

#endif // SEGMENT_H
