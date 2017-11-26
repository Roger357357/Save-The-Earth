#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>

// ----------------------------------------------------------------------------------------------------------------------------------------
        // VARIÁVEIS GLOBAIS
// ########################################################################################################################################

const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;
const int NUM_BULLETS = 5;
const int NUM_COMETS = 70;
int i, j;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_SAMPLE *musica_capa = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *ajustes = NULL;
ALLEGRO_BITMAP *escolha = NULL;
ALLEGRO_BITMAP *botao_off = NULL;
ALLEGRO_BITMAP *correto = NULL;
ALLEGRO_BITMAP *navea = NULL;
ALLEGRO_BITMAP *naveb = NULL;
ALLEGRO_BITMAP *navec = NULL;
ALLEGRO_BITMAP *IMG_tiro = NULL;
ALLEGRO_BITMAP *fundo_nivel = NULL;
ALLEGRO_BITMAP *coracao = NULL;
ALLEGRO_BITMAP *gameover = NULL;
ALLEGRO_BITMAP *tampa_coracao = NULL;
ALLEGRO_SAMPLE_ID *id_music = NULL;
ALLEGRO_TIMER *timer = NULL;


enum teclas{UP, DOWN, LEFT, RIGHT, SPACE};
enum IDS{PLAYER, BULLET, ENEMY};

typedef struct SpaceShip
{
    int ID;
    int x;
    int y;
    int lives;
    int speed;
    int boundx;
    int boundy;
    int score;
}SpaceShip;

typedef struct Comet
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
    int boundx;
    int boundy;
}Comet;
typedef struct Bullet
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
}Bullet;

bool tela_ajustes = false;
bool melodia = false;
bool efeito_sonoro = false;
bool tela_de_escolha = false;
bool nave1 = false;
bool nave2 = false;
bool nave3 = false;
bool btvoltar_datela_ajustes = false;
bool btvoltar_datela_escolha = false;
bool btcomecar_datela_escolha = false;
bool btsair_datela_donivel = false;
bool tela_da_capa = true;
bool saire = false;
bool jogando = false;
bool funcao_tiro = false;
bool teclas[5] = {false, false, false, false, false};
bool tampar1 = false;
bool tampar2 = false;
bool tampar3 = false;
bool apagar_coracao = false;
bool tela_gameover = false;
bool pausado = false;
bool redraw = true;

void InitShip(SpaceShip ship);
void DrawShip(SpaceShip ship);
void MoveShipUp(SpaceShip ship);
void MoveShipDown(SpaceShip ship);
void MoveShipLeft(SpaceShip ship);
void MoveShipRight(SpaceShip ship);

void InitComet(Comet comets[], int size);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
//void CollideComet(Comet comets[], int cSize, SpaceShip ship);

void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize);

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

bool inicializar();

int main(void)
{

//=======================================================================================================
//      POSIÇÃO INICIAL DA NAVE NA TELA

    int tecla = 0;
    int pos_x = 1202 / 2;
    int pos_y = 1200 / 2;
    int pos_xbala = navea;
    int pos_ybala = 1200 / 2 - 30;

    if (!inicializar())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

    al_set_sample_instance_playing(musica_capa, true);
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();

// ----------------------------------------------------------------------------------------------------------------------------------------
        // CARREGAMENTO DOS ARQUIVOS "IMAGENS"
// ########################################################################################################################################

    fundo = al_load_bitmap("background.png");
    ajustes = al_load_bitmap("setting.png");
    botao_off = al_load_bitmap("off.png");
    escolha = al_load_bitmap("tela_escolha.png");
    correto = al_load_bitmap("correto.png");
    fundo_nivel = al_load_bitmap("fundo_nivel.png");
    navea = al_load_bitmap("navea.png");
    naveb = al_load_bitmap("naveb.png");
    navec = al_load_bitmap("navec.png");
    IMG_tiro = al_load_bitmap("tiro.png");
    coracao = al_load_bitmap("vida.png");
    tampa_coracao = al_load_bitmap("tampa.png");
    gameover = al_load_bitmap("tela_gameover.png");

    srand(time(NULL));

    SpaceShip ship;
    Comet comets[NUM_COMETS];
    Bullet bullets[NUM_BULLETS];

    InitShip(ship);
    InitComet(comets, NUM_COMETS);
    InitBullet(bullets, NUM_BULLETS);

// ----------------------------------------------------------------------------------------------------------------------------------------
        // VERIFICAÇÃO DO CARREGAMENTO DO BACKGROUND (FUNDO)
// ########################################################################################################################################

    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar fundo.png\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
// ----------------------------------------------------------------------------------------------------------------------------------------
                //  FUNÇAO PRINCIPAL DO JOGO
// ########################################################################################################################################

    while (!saire)
    {
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                saire = true; //BOTÃO "X" DA TELA
            }

            /*if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                printf("[%d, %d]\n", evento.mouse.x, evento.mouse.y);
            }*/

// ----------------------------------------------------------------------------------------------------------------------------------------
                // RESET DE VARIÁVEIS DE CONTROLE
// ########################################################################################################################################

            if (!jogando)
            {
                btcomecar_datela_escolha = false;
            }

// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO VOLTAR DA TELA AJUSTES
// ########################################################################################################################################

                if(btvoltar_datela_ajustes == true)
                {
                    tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 73 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 526 &&
                            evento.mouse.y <= 679)
                        {
                            btvoltar_datela_ajustes = false;
                            tela_ajustes = false;
                        }
                    }
                }

// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO VOLTAR DA TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################

                if(btvoltar_datela_escolha == true)
                {
                    tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 130 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 535 &&
                            evento.mouse.y <= 645)
                        {
                            nave1 = false;
                            nave2 = false;
                            nave3 = false;
                            tela_de_escolha = false;
                            btvoltar_datela_escolha = false;
                        }
                    }
                }

// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO SAIR DA TELA DO NIVEL DO JOGO
// ########################################################################################################################################

                if(btsair_datela_donivel == true)
                {
                    //tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 1054 &&
                            evento.mouse.x <= 1150 &&
                            evento.mouse.y >= 590 &&
                            evento.mouse.y <= 678)
                        {
                            tela_da_capa = true;
                            btsair_datela_donivel = false;
                            btcomecar_datela_escolha = false;
                            pos_x = 1202 / 2;
                            pos_y = 1200 / 2;
                            pausado = false;
                            tampar1 = false;
                            tampar2 = false;
                            tampar3 = false;
                            nave1 = false;
                            nave2 = false;
                            nave3 = false;
                        }
                    }
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE GAME OVER
// ########################################################################################################################################

            if(tela_gameover == true)
            {
                al_draw_bitmap(gameover, 0, 0, 0);

                switch(evento.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ENTER:
                        tela_de_escolha = true;
                        btcomecar_datela_escolha = false;
                        nave1 = false;
                        nave2 = false;
                        nave3 = false;
                        tampar1 = false;
                        tampar2 = false;
                        tampar3 = false;
                    break;
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // FUNÇÃO "PAUSE"
// ########################################################################################################################################

            if(pausado == true)
            {
                if(nave1 == true || nave2 == true || nave3 == true)
                {
                    btcomecar_datela_escolha = false;
                    switch(evento.keyboard.keycode)
                    {
                        case ALLEGRO_KEY_SPACE:
                            printf("O jogo esta pausado\n");
                            btcomecar_datela_escolha = true;
                            pausado = false;
                        break;
                    }
                }
            }

// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DA CAPA DE FUNDO
// ########################################################################################################################################

            if(tela_da_capa == true)
            {
                al_draw_bitmap(fundo , 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
                }

//=======================================================================================================
//      BOTÃO PARA A TELA AJUSTES

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (evento.mouse.x >= 190 &&
                        evento.mouse.x <= 419 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        tela_ajustes = true;
                        tela_de_escolha = false;
                        tela_da_capa = false;
                    }
                }
//=======================================================================================================
//      BOTÃO PARA A TELA DE ESCOLHA DE NAVES

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (evento.mouse.x >= 475 &&
                        evento.mouse.x <= 718 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        tela_de_escolha = true;
                        tela_ajustes = false;
                        tela_da_capa = false;
                    }
                }
//=======================================================================================================
//      BOTÃO PARA SAIR DO JOGO
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 772 &&
                        evento.mouse.x <= 1002 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        saire = true;
                    }
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE AJUSTES
// ########################################################################################################################################

            if(tela_ajustes == true)
            {
                tela_da_capa = false;
                tela_de_escolha = false;
                saire = false;
<<<<<<< HEAD
                printf("tela_ajustes = true\n");
                btvoltar_datela_ajustes = true;
=======
>>>>>>> 8aad1e3beeafc6e87f170d1b8ad90ca3c728b3c4

                al_draw_bitmap(ajustes, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
                }
//=======================================================================================================
//      BOTÃO DE VOLTAR PARA A TELA DA CAPA

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 73 &&
                        evento.mouse.x <= 290 &&
                        evento.mouse.y >= 526 &&
                        evento.mouse.y <= 679)
                    {
                        //btvoltar_datela_ajustes = true;
                        tela_ajustes = false;
                    }
                }
//=======================================================================================================
//      BOTÃO DE MELODIA (LIGAR E DESLIGAR)

                if(melodia == true)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    al_draw_bitmap(botao_off, 420, 175, 0);

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = false;
                        }
                    }
                }
                else if(melodia == false)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = true;
                        }
                    }
                }
//=======================================================================================================
//      BOTÃO DE EFEITOS SONOROS (LIGAR E DESLIGAR)

                if(efeito_sonoro == true) // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                     al_draw_bitmap(botao_off, 855, 410, 0);

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 875 &&
                            evento.mouse.x <= 1118 &&
                            evento.mouse.y >= 424 &&
                            evento.mouse.y <= 595)
                        {
                            efeito_sonoro = false;

                        }
                    }
                }
                else if(efeito_sonoro == false)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 875 &&
                            evento.mouse.x <= 1118 &&
                            evento.mouse.y >= 424 &&
                            evento.mouse.y <= 595)
                        {
                            efeito_sonoro = true;
                        }
                    }
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################

          if(tela_de_escolha == true)
            {
                tela_da_capa = false;
                tela_ajustes = false;
                saire = false;

                al_draw_bitmap(escolha, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; // BOTÃO "X" DA TELA
                }
//=======================================================================================================
//      BOTÃO DE VOLTAR PARA A TELA DE CAPA

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 100 &&
                        evento.mouse.x <= 316 &&
                        evento.mouse.y >= 510 &&
                        evento.mouse.y <= 650)
                    {
                        btvoltar_datela_escolha = true;
                        tela_de_escolha = false;
                        nave1 = false;
                        nave2 = false;
                        nave3 = false;
                    }
                }
//=======================================================================================================
//     BOTÃO DE COMEÇAR PARA JOGAR

                printf("antes do if: %d\n", btcomecar_datela_escolha);
                if(btcomecar_datela_escolha == false) // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    printf("dentro do if da tela de escolha\n");
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        printf("dentro do if da tela de escolha - AND - button down\n");
                        if (evento.mouse.x >= 554 &&
                            evento.mouse.x <= 747 &&
                            evento.mouse.y >= 534 &&
                            evento.mouse.y <= 676)
                        {
                            printf("dentro do if da tela de escolha - AND - button down - AND - dentro da área\n");
                            btcomecar_datela_escolha = true;
                            tela_de_escolha = false;
                        }
                    }
                }
//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 1

                if(nave1 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 163 &&
                            evento.mouse.x <= 425 &&
                            evento.mouse.y >= 192 &&
                            evento.mouse.y <= 466)
                        {
                            nave1 = true;
                            nave2 = false;
                            nave3 = false;
                        }
                    }
                }

                else (nave1 == true);

//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 2
                if(nave2 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 512 &&
                            evento.mouse.x <= 765 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave1 = false;
                            nave2 = true;
                            nave3 = false;
                        }
                    }
                }

                else if(nave2 == true);

//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 3
                if(nave3 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 806 &&
                            evento.mouse.x <= 1115 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave1 = false;
                            nave2 = false;
                            nave3 = true;
                        }
                    }
                }

                else if(nave3 == true);

//=======================================================================================================
//      CÓDIGO DE DESENHAR O "CORRETO" NA NAVE SELECIONADA

                if(nave1 == true)
                {
                    al_draw_bitmap(correto, 200, 241, 0);
                }
                else if(nave2 == true)
                {
                    al_draw_bitmap(correto, 539, 241, 0);
                }
                else if(nave3)
                {
                    al_draw_bitmap(correto, 888, 241, 0);
                }
            }

//=======================================================================================================
//      BOTÃO DE COMECAR DA TELA DE ESCOLHA

            printf("antes da tela do if da tela do jogo: %d / n1: %d / n2: %d / n3: %d\n", btcomecar_datela_escolha, nave1, nave2, nave3);
            if((btcomecar_datela_escolha == true) && (nave1 == true || nave2 == true || nave3 == true)) // VERIFICAÇÃO DO ESTADO DO BOTÃO
            {
                printf("dentro do if da tela do jogo\n");
                tela_ajustes = false;
                tela_de_escolha = false;
                saire = false;
                tela_gameover = false;

                al_start_timer(timer);

                al_draw_bitmap(fundo_nivel, 0, 0, 0);
                al_draw_bitmap(coracao, 50, 0, 0);
                al_draw_bitmap(coracao, 120, 0, 0);
                al_draw_bitmap(coracao, 190, 0, 0);

//=======================================================================================================
//      BOTÃO DE SAIR DO NIVEL DO JOGO PARA A CAPA

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 1054 &&
                        evento.mouse.x <= 1150 &&
                        evento.mouse.y >= 590 &&
                        evento.mouse.y <= 678)
                    {
                        btsair_datela_donivel = true;
                    }
                }

//=======================================================================================================
//      CONDIÇÃO DA TELA GAME OVER

                if(tampar1 == true && tampar2 == true && tampar3 == true)
                {
                    btcomecar_datela_escolha = false;
                    tela_gameover = true;
                }

//=======================================================================================================
//      BOTÃO DE PAUSE DO NÍVEL DO JOGO

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 110 &&
                        evento.mouse.x <= 240 &&
                        evento.mouse.y >= 600 &&
                        evento.mouse.y <= 690)
                    {
                        printf("o jogo esta pausado\n");
                        pausado = true;
                    }
                }

//=======================================================================================================
//      FUNÇÃO TAMPAR CORAÇÃO "VIDA"
                if(apagar_coracao == true)
                {
                    if((pos_y == 0 || pos_y == 630 || pos_x == 371 || pos_x == 830) && (tampar1 == false))
                    {
                        tampar1 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }
                    else if((pos_y == 0 || pos_y == 630 || pos_x == 371 || pos_x == 830) && (tampar1 == true) && (tampar2 == false))
                    {
                        tampar2 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }
                    else if((pos_y == 0 || pos_y == 630 || pos_x == 371 || pos_x == 830) && (tampar1 == true) && (tampar2 == true) && (tampar3 == false))
                    {
                        tampar3 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }

                }

                if(tampar1 == true)
                {
                    al_draw_bitmap(tampa_coracao, 210, 10, 0);
                }

                if(tampar2 == true)
                {
                    al_draw_bitmap(tampa_coracao, 140, 10, 0);
                }

                if(tampar3 == true)
                {
                    al_draw_bitmap(tampa_coracao, 70, 10, 0);
                }

//=======================================================================================================
//      JOGANDO COM A NAVE 1

                if(nave1 == true) // MOVER A NAVE COM O TECLADO PRESSIONADO
                {
                    if(evento.type == ALLEGRO_EVENT_TIMER)
                    {
                        printf("Depois do Timer\n");
                        if(teclas[UP])
                            MoveShipUp(ship);
                        if(teclas[DOWN])
                            MoveShipDown(ship);
                        if(teclas[LEFT])
                            MoveShipLeft(ship);
                        if(teclas[RIGHT])
                            MoveShipRight(ship);

                      redraw = true;

                      UpdateBullet(bullets, NUM_BULLETS);
                      StartComet(comets, NUM_COMETS);
                      UpdateComet(comets, NUM_COMETS);
                      CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS);
                      CollideComet(comets, NUM_COMETS, ship);
                    }

                    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    {
                         saire = true;
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                            case ALLEGRO_KEY_SPACE:
                            teclas[SPACE] = true;
                            FireBullet(bullets, NUM_BULLETS, ship);
                            break;
                        }
                    }
                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;
                            case ALLEGRO_KEY_SPACE:
                            teclas[SPACE] = false;
                            break;
                        }
                    }

                    al_draw_bitmap(navea, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=630)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }

                    if(apagar_coracao == false)
                    {
                        apagar_coracao = true;
                    }

                    if(redraw && al_is_event_queue_empty(fila_eventos))
                    {
                        redraw = false;
                        printf("Desenhando nave\n");
                        DrawShip(ship);
                        DrawBullet(bullets, NUM_BULLETS);
                        DrawComet(comets, NUM_COMETS);
                    }
                }

//=======================================================================================================
//      JOGANDO COM A NAVE 2

                if(nave2 == true)
                {
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;
                        }
                    }

                    al_draw_bitmap(naveb, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=644)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }

                    if(apagar_coracao == false)
                    {
                        apagar_coracao = true;
                    }

                }

//=======================================================================================================
//      JOGANDO COM A NAVE 3

                if(nave3 == true)
                {
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;
                        }
                    }

                    al_draw_bitmap(navec, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=626)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }

                    if(apagar_coracao == false)
                    {
                        apagar_coracao = true;
                    }
                }

                jogando = true;

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; // BOTÃO "X" DA TELA
                }

            }

        }
        // ATUALIZA A TELA
        al_flip_display();
    }
//=======================================================================================================
//      DESTRUIÇÃO DOS ARQUIVOS - LIMPA MEMÓRIA

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(ajustes);
    al_destroy_bitmap(botao_off);
    al_destroy_bitmap(escolha);
    al_destroy_bitmap(navea);
    al_destroy_bitmap(naveb);
    al_destroy_bitmap(navec);
    al_destroy_bitmap(correto);
    al_destroy_bitmap(IMG_tiro);
    al_destroy_bitmap(coracao);
    al_destroy_bitmap(gameover);
    al_destroy_sample(musica_capa);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    al_destroy_timer(timer);

    return 0;
}

void InitShip(SpaceShip ship)
{
    ship.x = 1202 / 2;
    ship.y = 1200 / 2;
    ship.ID = PLAYER;
    ship.lives = 3;
    ship.speed = 7;
    ship.boundx = 6;
    ship.boundy = 7;
    ship.score = 0;
}
void DrawShip(SpaceShip ship)
{
    al_draw_filled_rectangle(ship.x + 9, ship.y, ship.x + 7, ship.y - 10, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(ship.x - 9 , ship.y, ship.x - 7, ship.y - 10, al_map_rgb(255, 0, 0));
    al_draw_filled_triangle(ship.x + 17, ship.y + 12, ship.x, ship.y - 12, ship.x - 17, ship.y + 12, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(ship.x + 2, ship.y + 12, ship.x - 2, ship.y - 15, al_map_rgb(0, 0, 255));
}
void MoveShipUp(SpaceShip ship)
{
    ship.y -= ship.speed;
    if(ship.y < 0)
        ship.y = 0;
}
void MoveShipDown(SpaceShip ship)
{
    ship.y += ship.speed;
    if(ship.y > ALTURA_TELA)
        ship.y = ALTURA_TELA;
}
void MoveShipLeft(SpaceShip ship)
{
    ship.x -= ship.speed;
    if(ship.x < 0)
        ship.x = 0;
}
void MoveShipRight(SpaceShip ship)
{
    ship.x += ship.speed;
    if(ship.x > 300)
        ship.x = 300;
}
void InitComet(Comet comets[], int size)
{
    for( i = 0; i < size; i++)
    {
        comets[i].ID = ENEMY;
        comets[i].live = false;
        comets[i].speed = 5;
        comets[i].boundx = 18;
        comets[i].boundy = 18;
    }
}
void DrawComet(Comet comets[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(comets[i].live)
        {
            al_draw_filled_rectangle(comets[i].x - 9, comets[i].y, comets[i].x - 7, comets[i].y + 10, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(comets[i].x +9 , comets[i].y, comets[i].x + 7, comets[i].y + 10, al_map_rgb(255, 0, 0));

            al_draw_filled_triangle(comets[i].x - 17, comets[i].y - 12, comets[i].x, comets[i].y + 12, comets[i].x + 17, comets[i].y - 12, al_map_rgb(255, 255, 255));
            al_draw_filled_rectangle(comets[i].x - 2, comets[i].y - 12, comets[i].x +2, comets[i].y + 15, al_map_rgb(0, 0, 255));
        }
    }
}
void StartComet(Comet comets[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(!comets[i].live)
        {
            if(rand() % 1000 == 0)
            {
                comets[i].live = true;
                comets[i].x = 375 + rand() % (LARGURA_TELA - 800);
                comets[i].y = 0;

                break;
            }
        }
    }
}
void UpdateComet(Comet comets[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(comets[i].live)
        {
            comets[i].y += comets[i].speed;

            if(comets[i].x < 0)
                comets[i].live = false;
        }
    }
}
void CollideComet(Comet comets[], int cSize, SpaceShip ship)
{
    for(i = 0; i < cSize; i++)
    {
        if(comets[i].live)
        {
            if(comets[i].x - comets[i].boundx < ship.x + ship.boundx &&
                comets[i].x + comets[i].boundx > ship.x - ship.boundx &&
                comets[i].y - comets[i].boundy < ship.y + ship.boundy &&
                comets[i].y + comets[i].boundy > ship.y - ship.boundy)
            {
                ship.lives--;
                comets[i].live = false;
            }
            else if(comets[i].x < 0)
            {
                comets[i].live = false;
                ship.lives--;
            }
        }
    }
}
void InitBullet(Bullet bullet[], int size)
{
    for(i = 0; i < size; i++)
    {
        bullet[i].ID = BULLET;
        bullet[i].speed = 10;
        bullet[i].live = false;
    }
}
void DrawBullet(Bullet bullet[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(bullet[i].live)
            al_draw_filled_circle(bullet[i].x, bullet[i].y, 10, al_map_rgb(0, 0, 0));
    }
}
void FireBullet(Bullet bullet[], int size, SpaceShip ship)
{
    for(i = 0; i < size; i++)
    {
        if(!bullet[i].live)
        {
            bullet[i].x = ship.x;
            bullet[i].y = ship.y + 30;
            bullet[i].live = true;
            break;
        }
    }
}
void UpdateBullet(Bullet bullet[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(bullet[i].live)
        {
            bullet[i].y -= bullet[i].speed;
            if(bullet[i].y > ALTURA_TELA)
                bullet[i].live = false;
        }
    }
}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize)
{
    for(i = 0; i < bSize; i++)
    {
        if(bullet[i].live)
        {
            for(j =0; j < cSize; j++)
            {
                if(comets[j].live)
                {
                    if(bullet[i].x > (comets[j].x - comets[j].boundx) &&
                        bullet[i].x < (comets[j].x + comets[j].boundx) &&
                        bullet[i].y > (comets[j].y - comets[j].boundy) &&
                        bullet[i].y < (comets[j].y + comets[j].boundy))
                    {
                        bullet[i].live = false;
                        comets[j].live = false;
                    }
                }
            }
        }
    }
}


// ----------------------------------------------------------------------------------------------------------------------------------------
        //INICIALIZAÇÕES E FUNÇÕES
// ########################################################################################################################################

bool inicializar()
{

    if (!al_init())
    {
        fprintf(stderr, "Não inicializou Allegro.\n");
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return false;
    }
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }

    id_music = malloc(sizeof(ALLEGRO_SAMPLE_ID));
    musica_capa = al_load_sample("04.wav");
    al_play_sample(musica_capa, 0.6, 0.3, 1, ALLEGRO_PLAYMODE_LOOP, id_music);
    if (!musica_capa)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
            // CONFIGURAÇÃO DO TÍTULO DA JANELA
    al_set_window_title(janela, "SAVE THE EARTH - The Social Problems");

            // TORNA APTO O USO DO MOUSE
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

            // ATRIBUI O CURSOR PADRÃO DO SISTEMA A SER USADO
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!id_music)
    {
        fprintf(stderr, "Falha ao carregar id_music.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
    timer = al_create_timer(1.0/35);
    if (!timer)
    {
        fprintf(stderr, "Falha ao criar timer.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        return false;
    }


//=======================================================================================================
//      REGISTROS DE EVENTOS
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    return true;
}
