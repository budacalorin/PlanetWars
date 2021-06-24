#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../include/scena_meniu.h"
#include "../include/scena_joc.h"
#include <string>

extern sf::RenderWindow window;
extern sf::RectangleShape background;
extern sf::Texture background_texture;
extern bool gameOn;
extern int scena_curenta;
extern int height;
extern int width;
extern sf::Font fonturi[50];
extern sf::Font font;
extern int numar_fonturi;
extern int fontul_curent;
extern std::string nume_fonturi[50];
extern sf::Texture textura_segment;
extern int muzica;
extern int numar_melodii;
extern sf::Music melodii[50];
extern std::string nume_melodii[50];
extern scena_joc joc_principal;
extern scena_meniu meniu_principal;
extern scena_meniu meniu_ajutor;
extern scena_meniu meniu_setari;
extern scena_meniu meniu_nivele;
extern scena_meniu meniu_pauza;
extern int indice_rezolutie;
extern sf::Texture textura_bulina_1;
extern sf::Texture textura_bulina_2;
extern sf::Texture textura_bulina_3;
extern sf::Texture texture;
extern sf::SoundBuffer beep_buffer;
extern sf::Sound beep_sound;
extern sf::SoundBuffer alert_buffer;
extern sf::Sound alert_sound;
extern int nivel_maxim;
extern int clasament[100][2];
extern bool limba;




#endif // INIT_H_INCLUDED
