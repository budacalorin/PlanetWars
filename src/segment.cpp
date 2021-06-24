#include "segment.h"
#include "init.h"
void segment::setup(int x, int y, int latura)
{
    float aux=(float)(1)/20*latura;
    forma.setPosition(sf::Vector2f(x,y));
    zid_N.setPosition(sf::Vector2f(x,y));
    zid_E.setPosition(sf::Vector2f(x+latura-aux,y));
    zid_S.setPosition(sf::Vector2f(x,y+latura-aux));
    zid_V.setPosition(sf::Vector2f(x,y));

    forma.setSize(sf::Vector2f(latura,latura));
    zid_N.setSize(sf::Vector2f(latura,aux));
    zid_E.setSize(sf::Vector2f(aux,latura));
    zid_S.setSize(sf::Vector2f(latura,aux));
    zid_V.setSize(sf::Vector2f(aux,latura));

    zid_N.setFillColor(sf::Color(200,200,200));
    zid_E.setFillColor(sf::Color(200,200,200));
    zid_S.setFillColor(sf::Color(200,200,200));
    zid_V.setFillColor(sf::Color(200,200,200));

    forma.setTexture(&textura_segment);

    tip_bulina=0;

    N=V=S=E=false;

    selectat=false;
}

void segment::redimensionare(int x, int y, int latura)
{
    float aux=(float)(1)/20*latura;

    forma.setPosition(sf::Vector2f(x,y));
    zid_N.setPosition(sf::Vector2f(x,y));
    zid_E.setPosition(sf::Vector2f(x+latura-aux,y));
    zid_S.setPosition(sf::Vector2f(x,y+latura-aux));
    zid_V.setPosition(sf::Vector2f(x,y));

    forma.setSize(sf::Vector2f(latura,latura));
    zid_N.setSize(sf::Vector2f(latura,aux));
    zid_E.setSize(sf::Vector2f(aux,latura));
    zid_S.setSize(sf::Vector2f(latura,aux));
    zid_V.setSize(sf::Vector2f(aux,latura));
}
