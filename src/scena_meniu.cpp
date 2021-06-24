#include "../include/scena_meniu.h"
#include <cstring>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "fstream"
#include "../include/init.h"

void popup(std::string text_popup);
using namespace std;
void beep();

void generare_meniu_nivele()
{
    meniu_nivele.numar_optiuni=0;
    string s;
    string p;
    for (int i=1; i<=nivel_maxim; i++)
    {
        s="";
        int aux=i;
        while (aux)
        {
            s=(char)(aux%10+'0')+s;
            aux/=10;
        }
        s+=" -> ";

        p="";
        aux=clasament[i][1];
        if (aux<=9)
        {
            p=(char)(aux%10+'0')+p;
            p='0'+p;
        }
        else
        {
            while (aux)
            {
                p=(char)(aux%10+'0')+p;
                aux/=10;
            }
        }
        p=':'+p;
        aux=clasament[i][0];
        if (aux<=9)
        {
            p=(char)(aux%10+'0')+p;
            p='0'+p;
        }
        else
        {
            while (aux)
            {
                p=(char)(aux%10+'0')+p;
                aux/=10;
            }
        }
        s+=p;
        meniu_nivele.adaugare_optiune(s.data());
    }
    if (limba==true)
        meniu_nivele.adaugare_optiune("Inapoi");
    else
        meniu_nivele.adaugare_optiune("Back");
}

void oprire_muzica()
{
    for (int i=0; i<50; i++)
        melodii[i].stop();
}

void update_muzica()
{
    if (muzica>numar_melodii||muzica==0)
    {
        muzica=0;
        oprire_muzica();
    }
    else
    {
        oprire_muzica();
        melodii[muzica].play();
    }
}

void update_fonturi()
{
    if (fontul_curent>numar_fonturi)
    {
        fontul_curent=1;
    }
    font=fonturi[fontul_curent];
    meniu_principal.generare_text();
    meniu_setari.generare_text();
}

void update_limba()
{
    meniu_principal.numar_optiuni=0;
    meniu_setari.numar_optiuni=0;
    meniu_ajutor.numar_optiuni=0;
    meniu_pauza.numar_optiuni=0;
    if(limba==true)
    {
        meniu_principal.adaugare_optiune("Joaca");
        meniu_principal.adaugare_optiune("Nivele");
        meniu_principal.adaugare_optiune("Ajutor");
        meniu_principal.adaugare_optiune("Setari");
        meniu_principal.adaugare_optiune("Iesire");
        //meniu_principal.generare_text();
        meniu_setari.adaugare_optiune("Muzica");
        meniu_setari.adaugare_optiune("Font");
        meniu_setari.adaugare_optiune("Limba");
        meniu_setari.adaugare_optiune("Rezolutie");
        meniu_setari.adaugare_optiune("Inapoi");
        //meniu_setari.generare_text();

        meniu_ajutor.adaugare_optiune("Reguli");
        meniu_ajutor.adaugare_optiune("Inapoi");

        //meniu_ajutor.generare_text();

        meniu_pauza.adaugare_optiune("Inapoi");
        meniu_pauza.adaugare_optiune("Setari");
        meniu_pauza.adaugare_optiune("Meniu principal");

        //meniu_pauza.generare_text();

        meniu_nivele.adaugare_optiune("Inapoi");
        //meniu_nivele.generare_text();

    }
    else

    {
        meniu_principal.adaugare_optiune("Play");
        meniu_principal.adaugare_optiune("Levels");
        meniu_principal.adaugare_optiune("Help");
        meniu_principal.adaugare_optiune("Settings");
        meniu_principal.adaugare_optiune("Exit");
        meniu_principal.generare_text();
        meniu_setari.adaugare_optiune("Music");
        meniu_setari.adaugare_optiune("Font");
        meniu_setari.adaugare_optiune("Language");
        meniu_setari.adaugare_optiune("Resolution");
        meniu_setari.adaugare_optiune("Back");
        meniu_setari.generare_text();

        meniu_ajutor.adaugare_optiune("Rules");
        meniu_ajutor.adaugare_optiune("Back");

        // meniu_ajutor.generare_text();

        meniu_pauza.adaugare_optiune("Back");
        meniu_pauza.adaugare_optiune("Settings");
        meniu_pauza.adaugare_optiune("Main menu");

        // meniu_pauza.generare_text();

        meniu_nivele.adaugare_optiune("Back");
        // meniu_nivele.generare_text();

    }
    meniu_principal.generare_text();
    meniu_setari.generare_text();
    meniu_ajutor.generare_text();
    meniu_pauza.generare_text();
    meniu_nivele.generare_text();
    joc_principal.reset_butoane();

}


void update_rezolutie()
{
    if (indice_rezolutie==1)
    {
        width=1920;
        height=1080;
    }
    if (indice_rezolutie==2)
    {
        width=1366;
        height=768;
    }
    window.create(sf::VideoMode(width,height),"",sf::Style::None);
    background.setSize(sf::Vector2f(width,height));
    meniu_setari.generare_text();
    meniu_ajutor.generare_text();
    meniu_principal.generare_text();
    meniu_pauza.generare_text();
    joc_principal.reset_tabla(false);
    sf::Event event;
    while (window.pollEvent(event))
    {

    }
}

void scena_meniu::adaugare_optiune(const char *s)
{
    strcpy(optiuni[++numar_optiuni], s);
}

void scena_meniu::input_tastatura()
{
    sf::Mouse mouse;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                up = 1;
                down = 0;
            }
            if (event.key.code == sf::Keyboard::Down)
            {
                up = 0;
                down = 1;
            }
            if (event.key.code == sf::Keyboard::Enter)
            {
                enter = 1;
            }
            if (event.key.code == sf::Keyboard::Escape)
            {
                beep();
                scena_curenta = scena_inapoi;
                if (scena_curenta==2)
                {
                    background_texture.loadFromFile("img/stars3.1.png");
                    background.setTexture(&background_texture,true);
                }
            }
        }

        if (event.type==sf::Event::MouseButtonReleased)
        {
            for (int i=1; i<=numar_optiuni; i++)
            {
                if (text[i].getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))
                        ||((text[i].getGlobalBounds().top<=mouse.getPosition(window).y&&mouse.getPosition(window).y<=text[i].getGlobalBounds().top+text[i].getGlobalBounds().height)&&
                           (text[i].getGlobalBounds().left-100<=mouse.getPosition(window).x&&mouse.getPosition(window).x<=text[i].getGlobalBounds().left+text[i].getGlobalBounds().width+100)))
                {
                    beep();
                    actiune();
                    break;
                }
            }
        }
        if (event.type==sf::Event::MouseMoved)
        {
            int aux=optiune_curenta;
            for (int i=1; i<=numar_optiuni; i++)
            {
                if (text[i].getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))
                        ||((text[i].getGlobalBounds().top<=mouse.getPosition(window).y&&mouse.getPosition(window).y<=text[i].getGlobalBounds().top+text[i].getGlobalBounds().height)&&
                           (text[i].getGlobalBounds().left-100<=mouse.getPosition(window).x&&mouse.getPosition(window).x<=text[i].getGlobalBounds().left+text[i].getGlobalBounds().width+100)))
                {
                    optiune_curenta=i;
                }
            }
            if (optiune_curenta!=aux)
            {
                beep();
            }
        }
        if (event.type == sf::Event::Closed)
        {
            gameOn = 0;
        }
    }
}

void scena_meniu::deplasare()
{
    if (up == 1)
    {
        beep();
        optiune_curenta--;
        if(optiune_curenta == 0)
        {
            optiune_curenta = 1;
        }
        up = 0;
    }
    if (down == 1)
    {
        beep();
        optiune_curenta++;
        if (optiune_curenta == numar_optiuni + 1)
        {
            optiune_curenta = numar_optiuni;
        }
        down = 0;
    }
    if (enter == 1)
    {
        beep();
        actiune();
        enter = 0;
    }
    while (optiune_curenta>numar_optiuni)
        optiune_curenta--;
    while (optiune_curenta<1)
        optiune_curenta++;

}

void scena_meniu::draw()
{
    window.clear();
    window.draw(background);
    afisare_optiuni();
    window.display();
}

void scena_meniu::afisare_optiuni()
{
    for (int i=1; i<=numar_optiuni; i++)
    {
        if (optiune_curenta == i)
        {
            text[i].setFillColor(sf::Color(25,150,200));
            //text[i].setStyle(sf::Text::Underlined);
        }
        else
        {
            text[i].setFillColor(sf::Color::Black);
            //text[i].setStyle(sf::Text::Regular);
        }
        if (strstr(optiuni[i], "Muzica")||strstr(optiuni[i], "Music"))
        {
            string s;
            if(strstr(optiuni[i], "Music"))
                s="Music: ";
            else
                s="Muzica: ";
            if (muzica!=0)
            {
                s+=nume_melodii[muzica];
            }
            else
            {
                s+="OFF ";
            }
            text[i].setString(s);
        }
        if (strstr(optiuni[i], "Font"))
        {
            string s;
            s="Font: ";
            s+=nume_fonturi[fontul_curent];

            text[i].setString(s);
            if(strstr(optiuni[i], "Limba")||strstr(optiuni[i], "Language"))
            {
                string s;
                if(strstr(optiuni[i], "Language"))
                    s="Language:English";
                else
                    s="Limba:Romana";
                text[i].setString(s);
            }
        }
        if (strstr(optiuni[i], "Rezolutie")||strstr(optiuni[i], "Resolution"))
        {
            string s;
            if(strstr(optiuni[i], "Resolution"))
                s="Resolution: ";
            else
                s="Rezolutie: ";
            if (indice_rezolutie==1)
                s+="1920X1080";
            if (indice_rezolutie==2)
                s+="1366X768";

            text[i].setString(s);
        }
        window.draw(text[i]);
    }
}

void scena_meniu::update()
{
    input_tastatura();
    deplasare();
    draw();
}

void scena_meniu::actiune()
{
    if (strstr(optiuni[optiune_curenta], "Joaca")||strstr(optiuni[optiune_curenta], "Play"))
    {
        scena_curenta = -1;
    }
    if (strstr(optiuni[optiune_curenta], "Iesire")||strstr(optiuni[optiune_curenta], "Exit"))
    {
        scena_curenta = scena_inapoi;
    }
    if (strstr(optiuni[optiune_curenta], "Inapoi")||strstr(optiuni[optiune_curenta], "Back"))
    {
        scena_curenta = scena_inapoi;
        if (scena_curenta==2)
        {
            background_texture.loadFromFile("img/stars3.1.png");
            background.setTexture(&background_texture,true);
        }
    }
    if (strstr(optiuni[optiune_curenta], "Setari")||strstr(optiuni[optiune_curenta], "Settings"))
    {
        meniu_setari.scena_inapoi=scena_curenta;
        scena_curenta = 3;

    }
    if (strstr(optiuni[optiune_curenta], "Muzica")||strstr(optiuni[optiune_curenta], "Music"))
    {
        muzica++;
        update_muzica();
    }
    if (strstr(optiuni[optiune_curenta], "Ajutor")||strstr(optiuni[optiune_curenta], "Help"))
    {
        scena_curenta=4;
    }
    if (strstr(optiuni[optiune_curenta], "Reguli")||strstr(optiuni[optiune_curenta], "Rules"))
    {
        //popup("Pam-pam");
        // popup("Regulile");
        if(limba==true)
        {
            popup("Imparte tabla in parti continui si egale ca arie, care sa aiba fiecare cate o planeta de fiecare fel!");
            popup("Tine apasat pe click-stanga pentru a creea ziduri si pe click-dreapta pentru a le sterge");
        }
        else
        {
            popup("Split the table in continous sectors, with ecual surface. Each one of them has to have one planet of each kind!");
            popup("Hold left-click for creating walls and right-click for erase them");
        }
    }
    if (strstr(optiuni[optiune_curenta], "Rezolutie")||strstr(optiuni[optiune_curenta], "Resolution"))
    {
        indice_rezolutie=3-indice_rezolutie;
        update_rezolutie();
    }
    if (strstr(optiuni[optiune_curenta], "Font"))
    {
        fontul_curent++;
        update_fonturi();
    }
    if(strstr(optiuni[optiune_curenta], "Limba")||strstr(optiuni[optiune_curenta], "Language"))
    {
        if(limba==true)
            limba=false;
        else
            limba=true;
        update_limba();
    }
    if (strstr(optiuni[optiune_curenta], "Meniu principal")||strstr(optiuni[optiune_curenta], "Main menu"))
    {
        scena_curenta=-2;
    }
    if (strstr(optiuni[optiune_curenta], "Nivele")||strstr(optiuni[optiune_curenta], "Levels"))
    {
        scena_curenta=-7;
    }
    if (optiuni[optiune_curenta][0]-'0'>=0&&optiuni[optiune_curenta][0]-'0'<=9)
    {
        if (optiuni[optiune_curenta][1]!=(char)NULL&&optiuni[optiune_curenta][1]!=' ')
        {
            if (optiuni[optiune_curenta][1]-'0'>=0&&optiuni[optiune_curenta][1]-'0'<=9)
            {
                joc_principal.nivel_curent=(optiuni[optiune_curenta][0]-'0')*10+(optiuni[optiune_curenta][1]-'0');
                scena_curenta=-1;
            }
        }
        else
        {
            joc_principal.nivel_curent=(optiuni[optiune_curenta][0]-'0');
            scena_curenta=-1;
        }

    }

}

void scena_meniu::generare_text()
{
    if (numar_optiuni==0)
        return;
    int x_optiuni = width/10; // coordonata x a textului
    int size=(height-20)/numar_optiuni; // dimensiunea textului
    if (size>height/9)
        size=height/9;
    int y_optiuni = height - numar_optiuni * (size+0)-size*(3.f/5.f); // coordonata y a textului
    for (int i = 1; i <= numar_optiuni; i++)
    {
        string s;
        s+=optiuni[i];

        text[i].setString(s.data());
        text[i].setCharacterSize (size);
        text[i].setPosition(sf::Vector2f(x_optiuni, y_optiuni));
        y_optiuni += size;
        text[i].setFillColor(sf::Color::Black);
        text[i].setFont (font);
        text[i].setOutlineColor(sf::Color::White);
        text[i].setOutlineThickness(3);
    }
}
