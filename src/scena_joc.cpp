#include "../include/scena_joc.h"
#include "../include/scena_meniu.h"
#include "../include/init.h"
#include "../include/time.h"
#include <fstream>
#include <cmath>
#include <iostream>
using namespace std;

void centrare_text (sf::Text &text, sf::Vector2f pozitie);
void popup(std::string text_popup);
void beep()
{
    //if (beep_sound.getStatus()==sf::Sound::Status::Playing)
    //    return;
    beep_sound.play();
}
void alert()
{
    //if (alert_sound.getStatus()==sf::Sound::Status::Playing)
    //return;
    alert_sound.play();
}
void update_nivel_maxim()
{
    if (joc_principal.nivel_curent>nivel_maxim)
        nivel_maxim=joc_principal.nivel_curent;
}


void scena_joc::update()
{
    verificare_final();
    update_ceas();
    update_events();

    for (int i=1; i<=latura_tablei; i++)
        matrice[i][1].V=matrice[1][i].N=matrice[latura_tablei][i].S=matrice[i][latura_tablei].E=1;

    draw();
}

void scena_joc::draw()
{
    window.clear();

    window.draw(background);
    afisare_tabla();
    afisare_numar_selectate();
    window.draw(text_ceas);
    window.draw(undo);
    window.draw(stergere);
    window.draw(iesire);
    if (ceas_afisat.minute>=10)
        window.draw(raspuns);
    window.draw(nivel);
    if (terminat==1)
        window.draw(nivel_urmator);

    window.display();
}


bool scena_joc::verificare_final()
{
    if (terminat==1)
    {
        return 1;
    }

    for (int i=1; i<=latura_tablei; i++)
        for (int j=1; j<=latura_tablei; j++)
            if (matrice[i][j].selectat==true)
                return 0;
    // verificare final nivel

    sf::Vector2i coada[5000];
    int viz[10][10];
    for (int i=0; i<=9; i++)
        for (int j=0; j<=9; j++)
            viz[i][j]=0;
    int numar_segmente=0;
    int planete[4];

    int tip_bulina_maxim=1;
    for (int i=1; i<=latura_tablei; i++)
        for (int j=1; j<=latura_tablei; j++)
            if (matrice[i][j].tip_bulina>tip_bulina_maxim)
                tip_bulina_maxim=matrice[i][j].tip_bulina;

    coada[0].x=coada[0].y=1;

    int u,p;
    u=p=0;

    bool nu_am_terminat=true;

    do
    {
        numar_segmente=1;
        planete[1]=planete[2]=planete[3]=0;

        while (u<=p)
        {
            int x,y;
            x=coada[u].x;
            y=coada[u].y;
            viz[x][y]=1;

            if (matrice[x][y].tip_bulina!=0)
                planete[matrice[x][y].tip_bulina]++;

            if (matrice[x][y].N==0&&viz[x-1][y]==0)
            {
                p++;
                coada[p].x=x-1;
                coada[p].y=y;
                viz[x-1][y]=1;
                numar_segmente++;
            }
            if (matrice[x][y].S==0&&viz[x+1][y]==0)
            {
                p++;
                coada[p].x=x+1;
                coada[p].y=y;
                viz[x+1][y]=1;
                numar_segmente++;
            }
            if (matrice[x][y].E==0&&viz[x][y+1]==0)
            {
                p++;
                coada[p].x=x;
                coada[p].y=y+1;
                viz[x][y+1]=1;
                numar_segmente++;
            }
            if (matrice[x][y].V==0&&viz[x][y-1]==0)
            {
                p++;
                coada[p].x=x;
                coada[p].y=y-1;
                viz[x][y-1]=1;
                numar_segmente++;
            }
            u++;
        }
        if (nivel_curent<33&&numar_segmente!=8)
        {
            //cout << "!=8\n";
            return 0;
        }
        if(nivel_curent>=33&&numar_segmente!=16)
        {
            //cout << "!=16\n";
            return 0;
        }

        for (int i=1; i<=tip_bulina_maxim; i++)
        {
            if (planete[i]!=1)
            {
                //cout << "planete[i]!=1\n";
                return 0;
            }
        }

        bool gasit=false;
        for (int i=1; i<=latura_tablei&&!gasit; i++)
        {
            for (int j=1; j<=latura_tablei&&!gasit; j++)
            {
                if (viz[i][j]==0)
                {
                    p++;
                    coada[p].x=i;
                    coada[p].y=j;
                    gasit=true;
                }
            }
        }
        if (gasit==false)
            nu_am_terminat=false;

    }
    while (nu_am_terminat==true);


    if (terminat==0 )// tabla e corecta)
    {
        draw();
        terminat=1;
        if (limba==true)
            popup("Ai reusit!!");
        else
            popup("It's a win!!");
        if (clasament[nivel_curent][0]==0&&clasament[nivel_curent][1]==0)
        {
            clasament[nivel_curent][0]=ceas_afisat.minute;
            clasament[nivel_curent][1]=ceas_afisat.secunde;
        }
        else
        {
            if (ceas_afisat.minute<=clasament[nivel_curent][0])
            {
                if (ceas_afisat.secunde<=clasament[nivel_curent][1])
                {
                    clasament[nivel_curent][0]=ceas_afisat.minute;
                    clasament[nivel_curent][1]=ceas_afisat.secunde;
                }
            }
        }
    }

    return 1;
}

void scena_joc::reset(int x)
{
    background_texture.loadFromFile("img/stars3.1.png");

    ceas_afisat.secunde=ceas_afisat.minute=0;
    ceas.restart();

    latura_tablei=8;

    reset_tabla(true);

    initiere_nivel();

    reset_butoane();

    terminat=0;

}

void scena_joc::update_events()
{
    sf::Event event;
    sf::Mouse mouse;
    while (window.pollEvent(event))
    {
        if (event.type==sf::Event::Closed)
            gameOn=0;
        if (event.type==sf::Event::KeyReleased)
        {
            if (event.key.code==sf::Keyboard::Escape)
            {
                beep();
                trecere_la_pauza();
                continue;
            }
            if (event.key.code==sf::Keyboard::S)
            {
                save();
            }
            if (event.key.code==sf::Keyboard::F)
            {
                terminat=1;
            }
        }
        if (mouse.isButtonPressed(sf::Mouse::Left))
        {
            procesare_mouse_left();
        }
        if (mouse.isButtonPressed(sf::Mouse::Right))
        {
            procesare_mouse_right();
        }
        if (event.type==sf::Event::MouseButtonReleased)
        {
            procesare_mouse();
        }

    }
    update_butoane();
}

void scena_joc::afisare_tabla()
{
    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {
            if (matrice[i][j].selectat==true)
                matrice[i][j].forma.setFillColor(sf::Color(0,0,255));
            else
                matrice[i][j].forma.setFillColor(sf::Color::Cyan);
            window.draw(matrice[i][j].forma);
            if (matrice[i][j].N==1)
                window.draw(matrice[i][j].zid_N);
            if (matrice[i][j].E==1)
                window.draw(matrice[i][j].zid_E);
            if (matrice[i][j].S==1)
                window.draw(matrice[i][j].zid_S);
            if (matrice[i][j].V==1)
                window.draw(matrice[i][j].zid_V);
        }
    }
    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {

            if (matrice[i][j].tip_bulina!=0)
            {
                sf::CircleShape bulina;
                bulina.setRadius(matrice[i][j].forma.getSize().x/2.5);
                bulina.setOrigin(sf::Vector2f(matrice[i][j].forma.getSize().x/2.5,matrice[i][j].forma.getSize().x/2.5));
                bulina.setPosition(sf::Vector2f(matrice[i][j].forma.getPosition().x+matrice[i][j].forma.getSize().x/2,matrice[i][j].forma.getPosition().y+matrice[i][j].forma.getSize().y/2));
                if (matrice[i][j].tip_bulina==1)
                    bulina.setTexture(&textura_bulina_1);
                if (matrice[i][j].tip_bulina==2)
                    bulina.setTexture(&textura_bulina_2);
                if (matrice[i][j].tip_bulina==3)
                    bulina.setTexture(&textura_bulina_3);
                window.draw(bulina);
            }
        }
    }
}

void scena_joc::afisare_numar_selectate()
{
    int numar=0;
    for (int i=1; i<=latura_tablei; i++)
        for (int j=1; j<=latura_tablei; j++)
            if (matrice[i][j].selectat==1)
                numar++;
    if (numar==0)
        return;
    int x,y;
    x=width-width/7;
    y=height/20;
    sf::Text text;
    std::string str;
    int aux=numar;
    while(aux)
    {
        str=((char)(aux%10+'0'))+str;
        aux/=10;
    }
    text.setString(str);
    text.setFont(font);
    text.setPosition(sf::Vector2f(x,y));
    text.setCharacterSize(height/12);
    if (nivel_curent<33)
    {
        if (numar==8)
        {
            text.setFillColor(sf::Color::Green);
            text.setCharacterSize(height/8);
        }
        if (numar>8)
        {
            text.setFillColor(sf::Color::Red);
            text.setCharacterSize(height/8);
        }
    }
    else
    {
        if (numar==16)
        {
            text.setFillColor(sf::Color::Green);
            text.setCharacterSize(height/8);
        }
        if (numar>16)
        {
            text.setFillColor(sf::Color::Red);
            text.setCharacterSize(height/8);
        }
    }
    centrare_text(text,sf::Vector2f(x,y));
    window.draw(text);
}

void scena_joc::initiere_nivel()
{
    if (nivel_curent>=1&&nivel_curent<=56)
    {
        ifstream fin ("nivele/nivele.txt");
        int n;
        fin >> n;
        fin.get();
        char s[30];
        while (n!=nivel_curent)
        {
            //cout << n <<endl;
            for (int i=1; i<=8; i++)
            {
                fin.getline(s,30);
                //cout << s <<endl;
            }

            fin.get();
            fin >> n;
            fin.get();
        }
        for (int i=1; i<=latura_tablei; i++)
            for (int j=1; j<=latura_tablei; j++)
                fin >> matrice[i][j].tip_bulina;
    }
}

void scena_joc::reset_tabla(bool full_reset)
{
    int latura=std::min(width,height)/10;
    int dx,dy;
    dx=width/2-latura*(latura_tablei/2);
    dy=height/2-latura*(latura_tablei/2);

    if (full_reset==true)
    {
        for (int i=1; i<=latura_tablei; i++)
        {
            for (int j=1; j<=latura_tablei; j++)
            {
                matrice[i][j].setup((j-1)*(latura+0)+dx,(i-1)*(latura+0)+dy,latura);
            }
        }
        for (int i=1; i<=latura_tablei; i++)
            matrice[i][1].V=matrice[1][i].N=matrice[latura_tablei][i].S=matrice[i][latura_tablei].E=1;

        for (int i=1; i<=latura_tablei; i++)
            for (int j=1; j<=latura_tablei; j++)
                matrice_undo[i][j].N=matrice_undo[i][j].S=matrice_undo[i][j].E=matrice_undo[i][j].V=0;
    }
    else
    {
        for (int i=1; i<=latura_tablei; i++)
        {
            for (int j=1; j<=latura_tablei; j++)
            {
                matrice[i][j].redimensionare((j-1)*(latura+0)+dx,(i-1)*(latura+0)+dy,latura);
            }
        }
        reset_butoane();
    }
}

void scena_joc::save()
{
    sf::Vector2i coada[5000];
    int viz[10][10];
    for (int i=1; i<=9; i++)
        for (int j=1; j<=9; j++)
            viz[i][j]=0;
    int p,u;
    p=u=0;
    coada[0].x=coada[0].y=1;
    int indice=1;
    viz[1][1]=1;
    bool ok;
    do
    {
        while (u<=p)
        {

            if (!matrice[coada[u].x][coada[u].y].N&&viz[coada[u].x-1][coada[u].y]==0)
            {
                p++;
                coada[p].x=coada[u].x-1;
                coada[p].y=coada[u].y;
                viz[coada[p].x][coada[p].y]=viz[coada[u].x][coada[u].y];
            }
            if (!matrice[coada[u].x][coada[u].y].S&&viz[coada[u].x+1][coada[u].y]==0)
            {
                p++;
                coada[p].x=coada[u].x+1;
                coada[p].y=coada[u].y;
                viz[coada[p].x][coada[p].y]=viz[coada[u].x][coada[u].y];
            }
            if (!matrice[coada[u].x][coada[u].y].E&&viz[coada[u].x][coada[u].y+1]==0)
            {
                p++;
                coada[p].x=coada[u].x;
                coada[p].y=coada[u].y+1;
                viz[coada[p].x][coada[p].y]=viz[coada[u].x][coada[u].y];
            }
            if (!matrice[coada[u].x][coada[u].y].V&&viz[coada[u].x][coada[u].y-1]==0)
            {
                p++;
                coada[p].x=coada[u].x;
                coada[p].y=coada[u].y-1;
                viz[coada[p].x][coada[p].y]=viz[coada[u].x][coada[u].y];
            }
            u++;
        }

        ok=0;
        for (int i=1; i<=8; i++)
        {
            for (int j=1; j<=8; j++)
            {
                if (viz[i][j]==0)
                {

                    p++;
                    coada[p].x=i;
                    coada[p].y=j;
                    indice++;
                    viz[i][j]=indice;
                    i=8;
                    ok=1;
                    break;
                }
            }
        }
    }
    while (ok==1);

    string s;
    s=".txt";
    int aux=nivel_curent;
    while (aux)
    {
        s=((char)(aux%10+'0'))+s;
        aux/=10;
    }
    s="rezolvari/"+s;
    ofstream fout(s);

    for (int i=1; i<=8; i++)
    {
        for (int j=1; j<=8; j++)
        {
            fout << viz[i][j]<< " ";
        }
        fout << endl;
    }
}

void scena_joc::RASPUNS()
{
    if (ceas_afisat.minute<0)
        return;

    int viz[13][13];
    int init[13][13];
    for (int i=0; i<=9; i++)
        for (int j=0; j<=9; j++)
            viz[i][j]=init[i][j]=0;
    string s;
    s=".txt";
    int aux=nivel_curent;
    while (aux)
    {
        s=((char)(aux%10+'0'))+s;
        aux/=10;
    }
    s="rezolvari/"+s;
    ifstream fin(s);
    if (fin.fail())
        return;
    STERGERE();
    for (int i=1; i<=8; i++)
    {
        for (int j=1; j<=8; j++)
        {
            fin >> viz[i][j];
            init[i][j]=viz[i][j];
        }
    }
    sf::Vector2i coada[5000];
    int p,u;
    p=u=0;
    coada[0].x=coada[0].y=1;
    bool ok;
    do
    {
        while (u<=p)
        {
            if (viz[coada[u].x][coada[u].y]==0)
            {
                u++;
                continue;
            }
            if (init[coada[u].x-1][coada[u].y]==init[coada[u].x][coada[u].y])
            {
                p++;
                int x,y;
                x=coada[u].x-1;
                y=coada[u].y;
                coada[p].x=x;
                coada[p].y=y;
            }
            else
            {
                matrice[coada[u].x-1][coada[u].y].S=matrice[coada[u].x][coada[u].y].N=1;
            }
            if (init[coada[u].x+1][coada[u].y]==init[coada[u].x][coada[u].y])
            {
                p++;
                int x,y;
                x=coada[u].x+1;
                y=coada[u].y;
                coada[p].x=x;
                coada[p].y=y;
            }
            else
            {
                matrice[coada[u].x+1][coada[u].y].N=matrice[coada[u].x][coada[u].y].S=1;
            }
            if (init[coada[u].x][coada[u].y+1]==init[coada[u].x][coada[u].y])
            {
                p++;
                int x,y;
                x=coada[u].x;
                y=coada[u].y+1;
                coada[p].x=x;
                coada[p].y=y;

            }
            else
            {
                matrice[coada[u].x][coada[u].y+1].V=matrice[coada[u].x][coada[u].y].E=1;
            }
            if (init[coada[u].x][coada[u].y-1]==init[coada[u].x][coada[u].y])
            {
                p++;
                int x,y;
                x=coada[u].x;
                y=coada[u].y-1;
                coada[p].x=x;
                coada[p].y=y;
            }
            else
            {
                matrice[coada[u].x][coada[u].y-1].E=matrice[coada[u].x][coada[u].y].V=1;
            }
            viz[coada[u].x][coada[u].y]=0;
            u++;
        }

        ok=0;
        for (int i=1; i<=8; i++)
        {
            for (int j=1; j<=8; j++)
            {
                if (viz[i][j]!=0)
                {
                    p++;
                    coada[p].x=i;
                    coada[p].y=j;
                    i=8;
                    ok=1;
                    break;
                }
            }
        }
    }
    while (ok==1);
}

void scena_joc::update_ceas()
{
    if (terminat==1&&!(ceas_afisat.minute==0&&ceas_afisat.secunde==0))
    {
        ceas.restart();
        return;
    }
    if (ceas.getElapsedTime().asSeconds()>=1||(ceas_afisat.minute==0&&ceas_afisat.secunde==0))
    {
        if (!(ceas_afisat.minute==0&&ceas_afisat.secunde==0)||ceas.getElapsedTime().asSeconds()>=1)
        {
            ceas_afisat.secunde++;
            ceas.restart();
        }
        if (ceas_afisat.secunde>=60)
        {
            ceas_afisat.minute++;
            ceas_afisat.secunde=0;
        }
        if (ceas_afisat.minute>=60)
        {
            ceas_afisat.minute=0;
        }
        std::string s;
        s=((char)ceas_afisat.minute/10+'0');
        s+=((char)ceas_afisat.minute%10+'0');
        s+=':';
        s+=((char)ceas_afisat.secunde/10+'0');
        s+=((char)ceas_afisat.secunde%10+'0');

        int x,y;
        x=width-width/7;
        y=3*height/20;
        text_ceas.setString(s);
        text_ceas.setFont(font);
        text_ceas.setCharacterSize(height/12);
        text_ceas.setPosition(sf::Vector2f(x,y));
        centrare_text(text_ceas,sf::Vector2f(x,y));
    }
}

void scena_joc::UNDO()
{
    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {
            matrice[i][j].N=matrice_undo[i][j].N;
            matrice[i][j].E=matrice_undo[i][j].E;
            matrice[i][j].S=matrice_undo[i][j].S;
            matrice[i][j].V=matrice_undo[i][j].V;
        }
    }
}
void scena_joc::STERGERE()
{
    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {
            matrice[i][j].N=0;
            matrice[i][j].E=0;
            matrice[i][j].S=0;
            matrice[i][j].V=0;
        }
    }
}

void scena_joc::reset_butoane()
{
    undo.setFont(font);
    undo.setCharacterSize(height/12);
    undo.setString("UNDO");
    undo.setOutlineThickness(3);
    undo.setFillColor(sf::Color::Black);
    undo.setOutlineColor(sf::Color::White);
    int dx,dy;
    dx=width-width/7;
    dy=height-height/20;
    undo.setPosition(sf::Vector2f(dx,dy));
    centrare_text(undo,sf::Vector2f(dx,dy));

    stergere.setFont(font);
    stergere.setCharacterSize(height/12);
    if(limba==true)
        stergere.setString("STERGERE");
    else
        stergere.setString("ERASE");
    stergere.setOutlineThickness(3);
    stergere.setFillColor(sf::Color::Black);
    stergere.setOutlineColor(sf::Color::White);
    dx=width-width/7;
    dy=height-3*height/20;
    stergere.setPosition(sf::Vector2f(dx,dy));
    centrare_text(stergere,sf::Vector2f(dx,dy));

    raspuns.setFont(font);
    raspuns.setCharacterSize(height/12);
    if(limba==true)
        raspuns.setString("RASPUNS");
    else
        raspuns.setString("SOLUTION");
    raspuns.setOutlineThickness(3);
    raspuns.setFillColor(sf::Color::Black);
    raspuns.setOutlineColor(sf::Color::White);
    dx=width-width/7;
    dy=5*height/20;
    raspuns.setPosition(sf::Vector2f(dx,dy));
    centrare_text(raspuns,sf::Vector2f(dx,dy));


    nivel.setFont(font);
    nivel.setCharacterSize(height/12);
    string s;
    if(limba==true)
        s="NIVEL: ";
    else
        s="LEVEL:";
    if (nivel_curent>9)
        s+=((char)nivel_curent/10+'0');
    s+=((char)nivel_curent%10+'0');
    nivel.setString(s);
    nivel.setOutlineThickness(3);
    nivel.setFillColor(sf::Color::Black);
    nivel.setOutlineColor(sf::Color::White);
    dx=width/7;
    dy=height/20;
    nivel.setPosition(sf::Vector2f(dx,dy));
    centrare_text(nivel,sf::Vector2f(dx,dy));

    nivel_urmator.setFont(font);
    nivel_urmator.setCharacterSize(height/12);
    if(limba==true)
        nivel_urmator.setString("NIVEL URMATOR");
    else
        nivel_urmator.setString("NEXT LEVEL");
    nivel_urmator.setOutlineThickness(3);
    nivel_urmator.setFillColor(sf::Color::Black);
    nivel_urmator.setOutlineColor(sf::Color::White);
    dx=width/2;
    dy=height-height/20;
    nivel_urmator.setPosition(sf::Vector2f(dx,dy));
    centrare_text(nivel_urmator,sf::Vector2f(dx,dy));

    iesire.setFont(font);
    iesire.setCharacterSize(height/12);
    if(limba==true)
        iesire.setString("IESIRE");
    else
        iesire.setString("EXIT");
    iesire.setOutlineThickness(3);
    iesire.setFillColor(sf::Color::Black);
    iesire.setOutlineColor(sf::Color::White);
    dx=width/7;
    dy=height-height/20;
    iesire.setPosition(sf::Vector2f(dx,dy));
    centrare_text(iesire,sf::Vector2f(dx,dy));
}

void scena_joc::trecere_la_pauza()
{
    scena_curenta=scena_inapoi;

    for (int i=1; i<=latura_tablei; i++)
        for (int j=1; j<=latura_tablei; j++)
            matrice[i][j].selectat=0;

    window.clear();

    afisare_tabla();

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(width/7);
    text.setPosition(sf::Vector2f(width/2,height/2));
    window.clear();
    window.draw(background);

    // text.setFillColor(sf::Color::White);
    text.setFillColor(sf::Color(25,150,200));
    text.setOutlineColor(sf::Color::White);
    text.setOutlineThickness(3);
    if(limba==true)
        text.setString("Pauza");
    else
        text.setString("Pause");
    centrare_text(text,sf::Vector2f(width/2,height/2));
    window.draw(text);

    background_texture.create(width,height);
    background_texture.update(window);

    background.setTexture(&background_texture,true);
}

void scena_joc::procesare_mouse_left()
{

    sf::Mouse mouse;
    bool ok=0,ok2=0;
    for (int i=1; i<=latura_tablei&&ok==0; i++)
        for (int j=1; j<=latura_tablei&&ok==0; j++)
        {
            if (matrice[i][j].selectat==1)
                ok=1;
            if (matrice[i][j].forma.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y)))
                ok2=1;
        }
    if (ok==0&&ok2==1)
    {
        for (int i=1; i<=latura_tablei; i++)
            for (int j=1; j<=latura_tablei; j++)
                matrice_undo[i][j]=matrice[i][j];
    }
    int x=mouse.getPosition(window).x;
    int y=mouse.getPosition(window).y;
    int numar_selectate=0;
    bool nou=false;
    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {
            if (matrice[i][j].forma.getGlobalBounds().contains(sf::Vector2f(x,y)))
            {
                if (matrice[i][j].selectat==false)
                {
                    nou=true;
                    beep();
                }
                matrice[i][j].selectat=true;

                if (matrice[i-1][j].selectat==false)
                {
                    matrice[i][j].N=true;
                    matrice[i-1][j].S=true;
                }
                else
                {
                    matrice[i][j].N=false;
                    matrice[i-1][j].S=false;
                }
                if (matrice[i+1][j].selectat==false)
                {
                    matrice[i][j].S=true;
                    matrice[i+1][j].N=true;
                }
                else
                {
                    matrice[i][j].S=false;
                    matrice[i+1][j].N=false;
                }
                if (matrice[i][j+1].selectat==false)
                {
                    matrice[i][j].E=true;
                    matrice[i][j+1].V=true;
                }
                else
                {
                    matrice[i][j].E=false;
                    matrice[i][j+1].V=false;
                }
                if (matrice[i][j-1].selectat==false)
                {
                    matrice[i][j].V=true;
                    matrice[i][j-1].E=true;
                }
                else
                {
                    matrice[i][j].V=false;
                    matrice[i][j-1].E=false;
                }

            }
            if (matrice[i][j].selectat==true)
            {
                numar_selectate++;
            }
        }
    }
    if (nou==true&&((numar_selectate==9&&nivel_curent<33)||(numar_selectate==17&&nivel_curent>=33)))
        alert();
}

void scena_joc::procesare_mouse_right()
{
    sf::Mouse mouse;
    bool ok=0,ok2=0;
    for (int i=1; i<=latura_tablei&&ok==0; i++)
        for (int j=1; j<=latura_tablei&&ok==0; j++)
        {
            if (matrice[i][j].selectat==1)
                ok=1;
            if (matrice[i][j].forma.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y)))
                ok2=1;
        }
    if (ok==0&&ok2==1)
    {
        for (int i=1; i<=latura_tablei; i++)
            for (int j=1; j<=latura_tablei; j++)
                matrice_undo[i][j]=matrice[i][j];
    }


    int x=mouse.getPosition(window).x;
    int y=mouse.getPosition(window).y;

    for (int i=1; i<=latura_tablei; i++)
    {
        for (int j=1; j<=latura_tablei; j++)
        {
            if (matrice[i][j].forma.getGlobalBounds().contains(sf::Vector2f(x,y)))
            {
                if (matrice[i][j].selectat==false)
                    beep();
                matrice[i][j].selectat=true;
                matrice[i][j].N=matrice[i][j].E=matrice[i][j].S=matrice[i][j].V=false;
                matrice[i-1][j].S=matrice[i+1][j].N=matrice[i][j-1].E=matrice[i][j+1].V=false;
            }
        }
    }
}

void scena_joc::procesare_mouse()
{
    sf::Mouse mouse;
    for (int i=1; i<=latura_tablei; i++)
        for (int j=1; j<=latura_tablei; j++)
            matrice[i][j].selectat=false;
    if (undo.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        beep();
        UNDO();
    }
    if (stergere.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {

        for (int i=1; i<=latura_tablei; i++)
            for (int j=1; j<=latura_tablei; j++)
                matrice_undo[i][j]=matrice[i][j];
        beep();
        STERGERE();
    }
    if (raspuns.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        beep();
        RASPUNS();
    }
    if (nivel_urmator.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        nivel_curent++;
        reset(8);
        update_nivel_maxim();
        if (nivel_curent==57)
        {
            nivel_curent=56;
            nivel_maxim=56;
            if(limba==true)
                popup("Ai terminat jocul!!");
            else
                popup("You finished the game!!");
            scena_curenta=-2;
        }
    }
    if (iesire.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        beep();
        trecere_la_pauza();
    }
}

void scena_joc::update_butoane()
{
    sf::Mouse mouse;
    if (undo.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        if (undo.getFillColor()==sf::Color::Black)
            beep();
        undo.setFillColor(sf::Color(25,150,200));
    }
    else
    {
        undo.setFillColor(sf::Color::Black);
    }
    if (stergere.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        if (stergere.getFillColor()==sf::Color::Black)
            beep();
        stergere.setFillColor(sf::Color(25,150,200));
    }
    else
    {
        stergere.setFillColor(sf::Color::Black);
    }
    if (raspuns.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        if (raspuns.getFillColor()==sf::Color::Black&& ceas_afisat.minute>=10)
            beep();
        raspuns.setFillColor(sf::Color(25,150,200));
    }
    else
    {
        raspuns.setFillColor(sf::Color::Black);
    }
    if (nivel_urmator.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        if (nivel_urmator.getFillColor()==sf::Color::Black&&terminat==true)
            beep();
        nivel_urmator.setFillColor(sf::Color(25,150,200));
    }
    else
    {
        nivel_urmator.setFillColor(sf::Color::Black);
    }
    if (iesire.getGlobalBounds().contains(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y))==1)
    {
        if (iesire.getFillColor()==sf::Color::Black)
            beep();
        iesire.setFillColor(sf::Color(25,150,200));
    }
    else
    {
        iesire.setFillColor(sf::Color::Black);
    }
}
