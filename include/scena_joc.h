#ifndef SCENA_JOC
#define SCENA_JOC
#include "segment.h"

struct ceas_salvat
{
    int minute=0;
    int secunde=0;
};
class scena_joc{
public:

    long long timp_precedent;
    segment matrice[100][100];
    segment matrice_undo[100][100];
    ceas_salvat ceas_afisat;
    sf::Text text_ceas;
    sf::Clock ceas;
    sf::Text undo;
    sf::Text raspuns;
    sf::Text stergere;
    sf::Text iesire;
    sf::Text nivel;
    sf::Text nivel_urmator;
    int latura_tablei=0;
    int nivel_curent;
    int scena_inapoi;
    bool terminat=0;


    void draw();
    void afisare_numar_selectate();
    void update_events();
    bool verificare_final();

    void update();

    void reset(int x);

    void afisare_tabla();

    void initiere_nivel();

    void reset_tabla(bool full_reset);

    void save();

    void update_ceas();

    void UNDO();

    void reset_butoane();

    void STERGERE();

    void RASPUNS();

    void trecere_la_pauza();

    void procesare_mouse_left();

    void procesare_mouse_right();

    void procesare_mouse();

    void update_butoane();

};


#endif
