#ifndef SCENA_MENIU
#define SCENA_MENIU
#include <SFML/Graphics.hpp>

class scena_meniu
{
    public:
        int numar_optiuni = 0;
        int optiune_curenta = 1;
        bool up = 0, down = 0, left = 0, right = 0, enter = 0;
        char optiuni[100][50];
        sf::Text text[100];
        int modJoc=0;
        int scena_inapoi;

        void adaugare_optiune(const char *s);

        void update();

        void generare_text();

    private:
        void afisare_optiuni();

        void actiune();

        void input_tastatura();

        void deplasare();

        void draw();



};

#endif // SCENA_MENIU
