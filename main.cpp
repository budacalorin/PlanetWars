#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include "include/init.h"
#include "include/scena_joc.h"
#include "include/scena_meniu.h"
#include <fstream>
using namespace std;
ifstream fin;
ofstream fout;

bool gameOn=1;
int height=768;
int width=1366;
int scena_curenta=1;
int muzica=0;
int numar_melodii;
int fontul_curent;
int numar_fonturi;
int indice_rezolutie;
int nivel_maxim;
int clasament[100][2];
sf::Font font;
sf::Font fonturi[50];
string nume_fonturi[50];
sf::Texture textura_bulina_1;
sf::Texture textura_bulina_2;
sf::Texture textura_bulina_3;
sf::SoundBuffer beep_buffer;
sf::Sound beep_sound;
sf::SoundBuffer alert_buffer;
sf::Sound alert_sound;
bool limba;

scena_meniu meniu_principal;
scena_meniu meniu_setari;
scena_meniu meniu_ajutor;
scena_meniu meniu_pauza;
scena_meniu meniu_nivele;
scena_joc joc_principal;
sf::Texture background_texture;
sf::Texture textura_segment;
sf::RectangleShape background;
sf::Music melodii[50];
string nume_melodii[50];
sf::Texture texture;
bool prima_deschidere=true;

sf::RenderWindow window(sf::VideoMode(width,height),"",sf::Style::None);
sf::Clock ceas_general;

void update_muzica();
void update_limba();
void update_fonturi();
void update_rezolutie();
void popup(std::string text_popup);
void update_nivel_maxim();
void generare_meniu_nivele();

void adaugare_melodie(string fisier)
{
    melodii[++numar_melodii].openFromFile(fisier);
    melodii[numar_melodii].setLoop(true);
    fisier.erase(0,6);
    fisier.erase(fisier.end()-4,fisier.end());
    nume_melodii[numar_melodii]=fisier;
}
void adaugare_font(string fisier)
{
    fonturi[++numar_fonturi].loadFromFile(fisier);
    fisier.erase(0,5);
    fisier.erase(fisier.end()-4,fisier.end());
    nume_fonturi[numar_fonturi]=fisier;
}

int main()
{
    fin.open("setari.txt");
    muzica=0;
    fontul_curent=6;
    indice_rezolutie=2;
    joc_principal.nivel_curent=1;
    fin >>muzica;
    fin >> fontul_curent;
    fin >> indice_rezolutie;
    fin >> joc_principal.nivel_curent;
    fin >> prima_deschidere;
    fin >> limba;
    if (prima_deschidere)
        scena_curenta=6;
    update_nivel_maxim();
    int minute,secunde;
    int indice=0;
    while (fin >> minute>>secunde)
    {
        indice++;
        clasament[indice][0]=minute;
        clasament[indice][1]=secunde;
    }


    fin.close();

    textura_segment.loadFromFile("img/sky2.png");
    //textura_segment.loadFromFile("img/stars2.1.png");
    //textura_segment.loadFromFile("img/stars2.png");
    background_texture.loadFromFile("img/rocket2.png");
    // background_texture.loadFromFile("img/black2.png");
    background.setTexture(&background_texture);
    background.setSize(sf::Vector2f(width,height));

    textura_bulina_1.loadFromFile("img/terra2.1.png");
    textura_bulina_2.loadFromFile("img/moon2.png");
    textura_bulina_3.loadFromFile("img/sun2.1.png");

    adaugare_melodie("music/Soliloquy.ogg");
    adaugare_melodie("music/the_field_of_dreams.ogg");
    beep_buffer.loadFromFile("music/beep.wav");
    beep_sound.setBuffer(beep_buffer);
    alert_buffer.loadFromFile("music/alert.wav");
    alert_sound.setBuffer(alert_buffer);
    adaugare_font("font/arial.ttf");

    adaugare_font("font/DejaVuSans.ttf");
    adaugare_font("font/LEELAWAD.ttf");
    adaugare_font("font/QuentellCFSemiBold-Regular.ttf");
    adaugare_font("font/Merveille.ttf");
    adaugare_font("font/data-latin.ttf");

    update_muzica();
    update_fonturi();
    update_rezolutie();



update_limba();
    meniu_principal.scena_inapoi=0;
    meniu_ajutor.scena_inapoi=1;
    meniu_setari.scena_inapoi=1;
    meniu_pauza.scena_inapoi=2;
    meniu_nivele.scena_inapoi=1;
    joc_principal.scena_inapoi=5;


    sf::Clock ceas;
    sf::Clock c;
    int fps=0;
    int timp_per_frame=1000000/120;
    while (gameOn==1)
    {
        int timp=ceas.getElapsedTime().asMicroseconds();

        fps++;
        if (c.getElapsedTime().asSeconds()>=1)
        {
            c.restart();
            //cout << fps << endl;
            fps=0;
        }

        if (scena_curenta==-2)
        {
            background_texture.loadFromFile("img/rocket2.png");
            background.setTexture(&background_texture,true);
            scena_curenta=1;
        }
        if (scena_curenta==0)
        {
            gameOn=0;
        }
        if (scena_curenta==1)
        {
            meniu_principal.update(); // meniul de start
        }
        if (scena_curenta==-1)
        {
            joc_principal.reset(1); // trecerea de la meniu la joc
            scena_curenta=2;
        }
        if (scena_curenta==2)
        {

            joc_principal.update();
            // jocul propriu-zis
        }
        if (scena_curenta==3)
        {
            meniu_setari.update();
        }
        if(scena_curenta==4)
        {
            meniu_ajutor.update();
        }
        if (scena_curenta==5)
        {
            meniu_pauza.update();
        }
        if (scena_curenta==6)
        {
            window.draw(background);
            popup("STAR WARS");
            popup("Regulile jocului sunt: Imparte tabla in 8 parti continui si egale ca arie, care sa aiba fiecare cate o bulina de fiecare fel!");
            popup("Foloseste click-stanga pentru a creea ziduri si click-dreapta pentru a le sterge");
            prima_deschidere=false;
            scena_curenta=1;
        }
        if (scena_curenta==7)
        {
            meniu_nivele.update();
        }
        if (scena_curenta==-7)
        {
            generare_meniu_nivele();
            meniu_nivele.generare_text();
            scena_curenta=7;
        }

        if (ceas.getElapsedTime().asMicroseconds()-timp<timp_per_frame)
            sf::sleep(sf::microseconds(timp_per_frame-(ceas.getElapsedTime().asMicroseconds()-timp)));
    }

    fout.open("setari.txt");
    fout<< muzica<<"\n";
    fout << fontul_curent<< "\n";
    fout << indice_rezolutie << "\n";
    fout << nivel_maxim<< "\n";
    fout << prima_deschidere <<"\n";
    fout << limba << "\n";
    for (int i=1; i<=nivel_maxim; i++)
    {
        fout << clasament[i][0] <<" "<< clasament[i][1] <<"\n";
    }
    fout.close();

    return 0;
}
