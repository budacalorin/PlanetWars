#include "init.h"
#include <iostream>
#include <cstdlib>
void centrare_text (sf::Text &text, sf::Vector2f pozitie)
{
    sf::FloatRect rectMarker =text.getGlobalBounds();
    sf::FloatRect rectBox;
    rectBox.height=1;
    rectBox.width=1;
    rectBox.top=pozitie.y;
    rectBox.left=pozitie.x;
    while ((pozitie.x) - (rectMarker.left + rectMarker.width) > rectMarker.left - pozitie.x)
    {
        text.move(sf::Vector2f(1, 0));
        rectMarker =text.getGlobalBounds();
    }
    while ((pozitie.x ) - (rectMarker.left + rectMarker.width) < rectMarker.left - pozitie.x)
    {
        text.move(sf::Vector2f(-1, 0));
        rectMarker =text.getGlobalBounds();
    }
    while ((pozitie.y) - (rectMarker.top + rectMarker.height) > rectMarker.top - pozitie.y)
    {
        text.move(sf::Vector2f(0, 1));
        rectMarker =text.getGlobalBounds();
    }
    while ((pozitie.y) - (rectMarker.top + rectMarker.height) < rectMarker.top - pozitie.y)
    {
        text.move(sf::Vector2f(0, -1));
        rectMarker =text.getGlobalBounds();
    }
}

void popup(std::string text_popup)
{
    sf::Texture texture;
    texture.create(width,height);
    texture.update(window);
    sf::RectangleShape fundal;
    fundal.setSize(sf::Vector2f(width,height));
    fundal.setTexture(&texture);

    sf::Text text;
    text.setString(text_popup);
    text.setFont(font);
    text.setOutlineThickness(3);
    text.setFillColor(sf::Color::Black);
    text.setOutlineColor(sf::Color::White);
    text.setCharacterSize(80);
    int min_size=50;
    int max_size=200;
    sf::Vector2f mijlocul_ecranului(width/2,height/2);
    int ch_size;
    bool ok=0;
    int separari=1;
    do
    {
        std::string new_string=text_popup;
        if (separari!=1)
        {
            int interval=new_string.size()/separari;
            bool pune=0;
            for (int i=0;i<(int)new_string.size();i++)
            {
                if (i%interval==0&&i<=interval*(separari-1)&&i!=0)
                {
                    pune=1;
                }
                if (new_string[i]==' '&&pune==1)
                {
                    new_string[i]='\n';
                    pune=0;
                }
            }
        }
        text.setString(new_string);

        ch_size=min_size;
        text.setCharacterSize(ch_size);
        while (text.getGlobalBounds().width<width-40&&(int)text.getCharacterSize()<max_size)
        {
            ch_size+=5;
            text.setCharacterSize(ch_size);
            centrare_text(text,mijlocul_ecranului);

        }
        text.setCharacterSize(ch_size-5);
        if (ch_size!=min_size)
            ok=1;
        separari++;
        centrare_text(text,mijlocul_ecranului);
    }while (ok==0);


    sf::Clock ceas;
    int timp_tranzitie=200000; // microsecunde
    int numar_tranzitii=30;
    int interval=timp_tranzitie/numar_tranzitii;
    ceas.restart();
    float s=0;
    float darkening=100.f/numar_tranzitii;
    float suma_opacitate=0;
    float diferenta_opacitate=255.f/numar_tranzitii;
    sf::RectangleShape negru;
    negru.setSize(sf::Vector2f(width,height));
    negru.setFillColor(sf::Color(0,0,0,0));
    text.setFillColor(sf::Color(text.getFillColor().r,text.getFillColor().g,text.getFillColor().b,0));
    text.setOutlineColor(sf::Color(text.getOutlineColor().r,text.getOutlineColor().g,text.getOutlineColor().b,0));

//std::cout<<text.getCharacterSize()<<std::endl;
    for (int i=1;i<=numar_tranzitii;i++)
    {
        if (ceas.getElapsedTime().asMicroseconds()<interval)
            sf::sleep(sf::microseconds(interval-ceas.getElapsedTime().asMicroseconds()));
        ceas.restart();
        s+=darkening;
        negru.setFillColor(sf::Color(0,0,0,s));
        suma_opacitate+=diferenta_opacitate;
        text.setFillColor(sf::Color(text.getFillColor().r,text.getFillColor().g,text.getFillColor().b,suma_opacitate));
        text.setOutlineColor(sf::Color(text.getOutlineColor().r,text.getOutlineColor().g,text.getOutlineColor().b,suma_opacitate));

        window.clear();
        window.draw(fundal);
        window.draw(negru);
        window.draw(text);
        window.display();
    }

    bool open=1;
    while (open&&gameOn)
    {
        window.clear();
        window.draw(fundal);
        window.draw(negru);
        window.draw(text);

        window.display();
        sf::Event event;
        sf::Mouse mouse;
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::Closed)
                    gameOn=0;
            if (event.type==event.KeyReleased)
            {
                if (event.key.code==sf::Keyboard::Enter)
                    open=0;
                if (event.key.code==sf::Keyboard::Escape)
                    open=0;

            }
            if (event.type==sf::Event::MouseButtonReleased)
            {
                if (text.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==0)
                {
                    open=0;
                }
            }
        }
    }

    for (int i=1;i<=numar_tranzitii;i++)
    {
        if (ceas.getElapsedTime().asMicroseconds()<interval)
            sf::sleep(sf::microseconds(interval-ceas.getElapsedTime().asMicroseconds()));
        ceas.restart();
        s-=darkening;
        negru.setFillColor(sf::Color(0,0,0,s));
        suma_opacitate-=diferenta_opacitate;
        text.setFillColor(sf::Color(text.getFillColor().r,text.getFillColor().g,text.getFillColor().b,suma_opacitate));
        text.setOutlineColor(sf::Color(text.getOutlineColor().r,text.getOutlineColor().g,text.getOutlineColor().b,suma_opacitate));

        window.clear();
        window.draw(fundal);
        window.draw(negru);
        window.draw(text);
        window.display();
    }


    window.clear();
    window.draw(fundal);
    window.display();
}
