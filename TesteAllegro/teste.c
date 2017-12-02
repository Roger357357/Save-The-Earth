#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "Estruturas.h"

// ----------------------------------------------------------------------------------------------------------------------------------------
        // VARIÁVEIS GLOBAIS
// ########################################################################################################################################

const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;
const int NUM_Balas = 10000;
const int NUM_Inimigo = 40;
const int NUM_objetos = 10;
int pos_x = 1202 / 2;
int pos_y = 1200 / 2;
int i, j;
char pause[100];
int pontos = 0;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_SAMPLE *musica_capa = NULL;
ALLEGRO_SAMPLE *semaudio = NULL;
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
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *cifrao = NULL;
ALLEGRO_FONT *missao = NULL;
ALLEGRO_TIMER *timer = NULL;


enum teclas{UP, DOWN, LEFT, RIGHT, SPACE};


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
bool vida_da_terra = false;
bool comecar_nivel = false;

void IniciaInimigo(Inimigo inimigo[], int size);
void DesenhaInimigo(Inimigo inimigo[], int size);
void ComecaInimigo(Inimigo inimigo[], int size);
void CarregarInimigo(Inimigo inimigo[], int size);

void IniciaObjeto(Objeto objetos[], int size);
void DesenhaObjeto(Objeto objetos[], int size);
void ComecaObjeto(Objeto objetos[], int size);
void CarregarObjeto(Objeto objetos[], int size);

void IniciaBala(Bala balas[], int size);
void DesenharBala(Bala balas[], int size);
void DesenharBala1(Bala balas[], int size);
void DesenharBala2(Bala balas[], int size);
void AtirarBala(Bala balas[], int size);
void CarregarBala(Bala balas[], int size);
void ColisaoBala(Bala balas[], int bSize, Inimigo inimigo[], int cSize, Objeto objetos[], int dSize);

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

bool inicializar();

int main(void)
{

//=======================================================================================================
//      VALORES INICAIS DE VARIAVEIS

    //int tecla = 0;
    //int pos_xbala = navea;
    int pos_ybala = 1200 / 2 - 30;
    int tempx = pos_x;
    int tempy = pos_y;

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
    al_play_sample(musica_capa, 0.6, 0.3, 1, ALLEGRO_PLAYMODE_LOOP, id_music);


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
    fonte = al_load_font("Square.ttf", 35, 0);
    cifrao = al_load_font("Square.ttf", 9, 0);
    missao = al_load_font("Square.ttf", 24, 0);

    srand(time(NULL));

    Inimigo inimigo[NUM_Inimigo];
    Objeto objetos[NUM_objetos];
    Bala balas[NUM_Balas];

    IniciaInimigo(inimigo, NUM_Inimigo);
    IniciaObjeto(objetos, NUM_objetos);
    IniciaBala(balas, NUM_Balas);

// ----------------------------------------------------------------------------------------------------------------------------------------
        // VERIFICAÇÃO DO CARREGAMENTO
// ########################################################################################################################################

    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar fundo.png\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }

    if (!fonte)
    {
        fprintf(stderr,"Falha ao carregar fonte\n");
        al_destroy_display(janela);
        return false;
    }

    if (!missao)
    {
        fprintf(stderr,"Falha ao carregar missao\n");
        al_destroy_display(janela);
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
                    //tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && tela_de_escolha != true)
                    {
                        if (evento.mouse.x >= 73 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 526 &&
                            evento.mouse.y <= 679)
                        {
                            btvoltar_datela_ajustes = false;
                            tela_da_capa = true;
                            tela_ajustes = false;
                        }
                    }
                }

// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO VOLTAR DA TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################

                if(btvoltar_datela_escolha == true)
                {
                    //tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && tela_da_capa != true && tela_ajustes != true)
                    {
                        if (evento.mouse.x >= 130 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 535 &&
                            evento.mouse.y <= 645)
                        {
                            tela_da_capa = true;
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
                            pontos = 0;
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
                        btsair_datela_donivel = false;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                        pausado = false;
                        nave1 = false;
                        nave2 = false;
                        nave3 = false;
                        tampar1 = false;
                        tampar2 = false;
                        tampar3 = false;
                        pontos = 0;
                    break;
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // FUNÇÃO "PAUSE"
// ########################################################################################################################################

            if(pausado == true)
            {
                al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 - 50, ALLEGRO_ALIGN_CENTRE, "PAUSADO", pause);
                al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "PRESSIONE  ENTER", pause);
                al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 + 50, ALLEGRO_ALIGN_CENTRE, "PARA CONTINUAR", pause);

                if(nave1 == true || nave2 == true || nave3 == true)
                {
                    btcomecar_datela_escolha = false;
                    switch(evento.keyboard.keycode)
                    {
                        case ALLEGRO_KEY_ENTER:
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

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && tela_de_escolha != true && tela_da_capa == true)
                {
                    if (evento.mouse.x >= 190 &&
                        evento.mouse.x <= 419 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        tela_ajustes = true;
                        tela_de_escolha = false;
                        tela_da_capa = false;
                        btvoltar_datela_escolha = false;
                        btvoltar_datela_ajustes = true;
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
                btvoltar_datela_ajustes = true;


                al_draw_bitmap(ajustes, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
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
                            al_play_sample(musica_capa, 0.6, 0.3, 1, ALLEGRO_PLAYMODE_LOOP, id_music);
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
                            al_stop_sample(id_music);

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
                btvoltar_datela_ajustes = false;

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
                        //btvoltar_datela_escolha = true;
                        tela_da_capa = true;
                        tela_de_escolha = false;
                        nave1 = false;
                        nave2 = false;
                        nave3 = false;
                    }
                }
//=======================================================================================================
//     BOTÃO DE COMEÇAR PARA JOGAR
                if(btcomecar_datela_escolha == false) // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 554 &&
                            evento.mouse.x <= 747 &&
                            evento.mouse.y >= 534 &&
                            evento.mouse.y <= 676)
                        {
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
            if((btcomecar_datela_escolha == true) && (nave1 == true || nave2 == true || nave3 == true)) // VERIFICAÇÃO DO ESTADO DO BOTÃO
            {
                tela_ajustes = false;
                tela_de_escolha = false;
                saire = false;
                tela_gameover = false;

                al_start_timer(timer);

                al_draw_bitmap(fundo_nivel, 0, 0, 0);
                al_draw_bitmap(coracao, 50, 0, 0);
                al_draw_bitmap(coracao, 120, 0, 0);
                al_draw_bitmap(coracao, 190, 0, 0);
                al_draw_textf(fonte, al_map_rgb(255, 255, 255), 1100, 100, ALLEGRO_ALIGN_CENTRE, "%d", pontos);
                al_draw_textf(missao, al_map_rgb(0, 128, 128), 127, 273, ALLEGRO_ALIGN_LEFT, "|....| ATINGIR");
                al_draw_textf(missao, al_map_rgb(0, 128, 128), 120, 310, ALLEGRO_ALIGN_LEFT, "200 PONTOS");
                al_draw_textf(missao, al_map_rgb(0, 128, 128), 125, 373, ALLEGRO_ALIGN_LEFT, "|....| ELIMINAR");
                al_draw_textf(missao, al_map_rgb(0, 128, 128), 125, 410, ALLEGRO_ALIGN_LEFT, "50 INIMIGOS");

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
                        pausado = true;
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
//      FUNÇÃO TAMPAR CORAÇÃO "VIDA"

                if(vida_da_terra == true)
                {
                    if((inimigo[i].y > 720) && (tampar1 == false))
                    {
                        tampar1 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }
                    else if((inimigo[i].y = 720) && (tampar1 == true) && (tampar2 == false))
                    {
                        tampar2 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }
                    else if((inimigo[i].y = 720) && (tampar1 == true) && (tampar2 == true) && (tampar3 == false))
                    {
                        tampar3 = true;
                        pos_x = 1202 / 2;
                        pos_y = 1200 / 2;
                    }

                }

                if(comecar_nivel == true)
                {
                    al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 - 50, ALLEGRO_ALIGN_CENTRE, "LEVEL 2", pause);
                    al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "PRSSIONE ENTER", pause);
                    al_draw_textf(fonte, al_map_rgb(255, 0, 0), LARGURA_TELA/2, ALTURA_TELA/2 + 50, ALLEGRO_ALIGN_CENTRE, "PARA CONTINUAR", pause);

                    nave1 = false;
                    nave2 = false;
                    nave3 = false;
                    pausado = false;

                    if(nave1 == true)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_SPACE:
                                nave1 = true;
                                comecar_nivel = false;
                                tampar1 = false;
                                tampar2 = false;
                                tampar3 = false;
                                pontos = 0;
                            break;
                        }
                    }
                    if(nave2 == true)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_SPACE:
                                nave2 = true;
                                comecar_nivel = false;
                                tampar1 = false;
                                tampar2 = false;
                                tampar3 = false;
                                pontos = 0;
                            break;
                        }
                    }
                    if(nave3 == true)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_SPACE:
                                nave3 = true;
                                comecar_nivel = false;
                                tampar1 = false;
                                tampar2 = false;
                                tampar3 = false;
                                pontos = 0;
                            break;
                        }
                    }
                }

//=======================================================================================================
//      JOGANDO COM A NAVE 1 OU NAVE 2 OU NAVE 3

                if(nave1 == true || nave2 == true || nave3 == true) // MOVER A NAVE COM O TECLADO PRESSIONADO
                {
                    if(evento.type == ALLEGRO_EVENT_TIMER)
                    {
                        redraw = true;

                        CarregarBala(balas, NUM_Balas);
                        ComecaInimigo(inimigo, NUM_Inimigo);
                        CarregarInimigo(inimigo, NUM_Inimigo);
                        ComecaObjeto(objetos, NUM_objetos);
                        CarregarObjeto(objetos, NUM_objetos);
                        ColisaoBala(balas, NUM_Balas, inimigo, NUM_Inimigo, objetos, NUM_objetos);
                        ColisaoInimigo(inimigo, NUM_Inimigo);
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
                            AtirarBala(balas, NUM_Balas);
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

                    if(nave1 == true)
                    {
                        al_draw_bitmap(navea, pos_x, pos_y, 0);
                    }
                    if(nave2 == true)
                    {
                        al_draw_bitmap(naveb, pos_x, pos_y, 0);
                    }
                    if(nave3 == true)
                    {
                        al_draw_bitmap(navec, pos_x, pos_y, 0);
                    }

                    pos_y -= teclas[UP] * 7;
                    pos_y += teclas[DOWN] * 7;
                    pos_x -= teclas[LEFT] * 7;
                    pos_x += teclas[RIGHT] * 7;

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
                        DesenharBala(balas, NUM_Balas);
                        DesenharBala1(balas, NUM_Balas);
                        DesenharBala2(balas, NUM_Balas);
                        DesenhaInimigo(inimigo, NUM_Inimigo);
                        DesenhaObjeto(objetos, NUM_objetos);
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
    al_destroy_font(fonte);
    al_destroy_display(cifrao);
    al_destroy_display(missao);
    al_destroy_timer(timer);

    return 0;
}

void IniciaObjeto(Objeto objetos[], int size)
{
    for( i = 0; i < size; i++)
    {
        objetos[i].ID = Coisa;
        objetos[i].vida = false;
        objetos[i].velocidade = 2;
        objetos[i].limite_x = 18;
        objetos[i].limite_y = 18;
    }
}
void DesenhaObjeto(Objeto objetos[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(objetos[i].vida)
        {

            al_draw_filled_rectangle(objetos[i].x, objetos[i].y, objetos[i].x + 25, objetos[i].y + 17, al_map_rgb(50, 205, 50));
            al_draw_filled_rectangle(objetos[i].x + 2.5, objetos[i].y + 2.5, objetos[i].x + 22.5, objetos[i].y + 14.5, al_map_rgb(0, 160, 0));
            al_draw_filled_rectangle(objetos[i].x + 3, objetos[i].y + 3, objetos[i].x + 21.5, objetos[i].y + 13.5, al_map_rgb(50, 205, 50));
            al_draw_filled_circle(objetos[i].x + 12, objetos[i].y + 8, 5, al_map_rgb(0, 160, 0));
            al_draw_filled_circle(objetos[i].x + 2.5, objetos[i].y + 2.5, 2, al_map_rgb(0, 160, 0));
            al_draw_filled_circle(objetos[i].x + 22.5, objetos[i].y + 14.5, 2, al_map_rgb(0, 160, 0));
            al_draw_filled_circle(objetos[i].x + 22.5, objetos[i].y + 2.5, 2, al_map_rgb(0, 160, 0));
            al_draw_filled_circle(objetos[i].x + 2.5, objetos[i].y + 14.5, 2, al_map_rgb(0, 160, 0));
            al_draw_textf(cifrao, al_map_rgb(240, 230, 140), objetos[i].x + 11, objetos[i].y + 3, ALLEGRO_ALIGN_CENTRE, "$", pause);

;        }
    }
}
void ComecaObjeto(Objeto objetos[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(!objetos[i].vida)
        {
            if(rand() % 1000 == 0)
            {
                objetos[i].vida = true;
                objetos[i].x = 400 + rand() % (LARGURA_TELA - 800);
                objetos[i].y = 0;

                break;
            }
        }
    }
}
void CarregarObjeto(Objeto objetos[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(objetos[i].vida)
        {
            objetos[i].y += objetos[i].velocidade;

            if(objetos[i].x < 0)
                objetos[i].vida = false;
        }
    }
}

void IniciaInimigo(Inimigo inimigo[], int size)
{
    for( i = 0; i < size; i++)
    {
        inimigo[i].ID = Adversario;
        inimigo[i].vida = false;
        inimigo[i].velocidade = 4;
        inimigo[i].limite_x = 18;
        inimigo[i].limite_y = 18;
    }
}
void DesenhaInimigo(Inimigo inimigo[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(inimigo[i].vida)
        {
            al_draw_filled_rectangle(inimigo[i].x - 9, inimigo[i].y, inimigo[i].x - 7, inimigo[i].y + 10, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(inimigo[i].x +9 , inimigo[i].y, inimigo[i].x + 7, inimigo[i].y + 10, al_map_rgb(255, 0, 0));

            al_draw_filled_triangle(inimigo[i].x - 17, inimigo[i].y - 12, inimigo[i].x, inimigo[i].y + 12, inimigo[i].x + 17, inimigo[i].y - 12, al_map_rgb(255, 255, 255));
            al_draw_filled_rectangle(inimigo[i].x - 2, inimigo[i].y - 12, inimigo[i].x +2, inimigo[i].y + 15, al_map_rgb(0, 0, 255));
        }
    }
}
void ComecaInimigo(Inimigo inimigo[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(!inimigo[i].vida)
        {
            if(rand() % 1000 == 0)
            {
                inimigo[i].vida = true;
                inimigo[i].x = 400 + rand() % (LARGURA_TELA - 800);
                inimigo[i].y = 0;

                break;
            }
        }
    }
}
void CarregarInimigo(Inimigo inimigo[], int size)
{
    for( i = 0; i < size; i++)
    {
        if(inimigo[i].vida)
        {
            inimigo[i].y += inimigo[i].velocidade;

            if(inimigo[i].x < 0)
            {
                inimigo[i].vida = false;
            }
        }
    }
}

void ColisaoInimigo(Inimigo inimigo[], int cSize)
{
    for(i = 0; i < cSize; i++)
    {
        if(inimigo[i].vida)
        {
            if(inimigo[i].x - inimigo[i].limite_x < pos_x + 80 &&
                inimigo[i].x + inimigo[i].limite_x > pos_x &&
                inimigo[i].y - inimigo[i].limite_y < pos_y + 180 &&
                inimigo[i].y + inimigo[i].limite_y > pos_y)
            {
                inimigo[i].vida = false;
                apagar_coracao = true;
            }
            if(apagar_coracao == true)
            {
                if((tampar1 == false) && (inimigo[i].x - inimigo[i].limite_x < pos_x + 80 &&
                    inimigo[i].x + inimigo[i].limite_x > pos_x &&
                    inimigo[i].y - inimigo[i].limite_y < pos_y + 180 &&
                    inimigo[i].y + inimigo[i].limite_y > pos_y))
                {
                    tampar1 = true;
                }
                else if((tampar1 == true) && (tampar2 == false) && (inimigo[i].x - inimigo[i].limite_x < pos_x + 80 &&
                        inimigo[i].x + inimigo[i].limite_x > pos_x &&
                        inimigo[i].y - inimigo[i].limite_y < pos_y + 180 &&
                        inimigo[i].y + inimigo[i].limite_y > pos_y))
                {
                    tampar2 = true;
                }
                else if((tampar1 == true) && (tampar2 == true) && (tampar3 == false) && (inimigo[i].x - inimigo[i].limite_x < pos_x + 80 &&
                        inimigo[i].x + inimigo[i].limite_x > pos_x &&
                        inimigo[i].y - inimigo[i].limite_y < pos_y + 180 &&
                        inimigo[i].y + inimigo[i].limite_y > pos_y))
                {
                    tampar3 = true;
                }
            }
        }
    }
}

void IniciaBala(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        balas[i].ID = Tiro;
        balas[i].velocidade = 10;
        balas[i].vida = false;
    }
}
void DesenharBala(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(balas[i].vida)
            al_draw_filled_circle(balas[i].x, balas[i].y, 7, al_map_rgb(0, 191, 255));
    }
}

void DesenharBala1(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(balas[i].vida)
            al_draw_filled_circle(balas[i].x, balas[i].y, 5, al_map_rgb(0, 0, 205));
    }
}
void DesenharBala2(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(balas[i].vida)
            al_draw_filled_circle(balas[i].x, balas[i].y, 3, al_map_rgb(255, 255, 255));
    }
}
void AtirarBala(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(!balas[i].vida)
        {
            //printf("%d\n",balas[i].x);
            balas[i].x = pos_x + 40;
            balas[i].y = pos_y;
            balas[i].vida = true;
            break;
        }
    }
}
void CarregarBala(Bala balas[], int size)
{
    for(i = 0; i < size; i++)
    {
        if(balas[i].vida)
        {
            balas[i].y -= balas[i].velocidade;
            if(balas[i].y > ALTURA_TELA)
                balas[i].vida = false;
        }
    }
}

void ColisaoBala(Bala balas[], int bSize, Inimigo inimigo[], int cSize, Objeto objetos[], int dSize)
{
    for(i = 0; i < bSize; i++)
    {
        if(balas[i].vida)
        {
            for(j =0; j < cSize; j++)
            {
                if(inimigo[j].vida)
                {
                    if(balas[i].x > (inimigo[j].x - inimigo[j].limite_x) &&
                        balas[i].x < (inimigo[j].x + inimigo[j].limite_x) &&
                        balas[i].y > (inimigo[j].y - inimigo[j].limite_y) &&
                        balas[i].y < (inimigo[j].y + inimigo[j].limite_y))
                    {
                        balas[i].vida = false;
                        inimigo[j].vida = false;
                        pontos++;
                    }
                }
            }
            for(j =0; j < dSize; j++)
            {
                if(objetos[j].vida)
                {
                    if(balas[i].x > (objetos[j].x - objetos[j].limite_x) &&
                        balas[i].x < (objetos[j].x + objetos[j].limite_x) &&
                        balas[i].y > (objetos[j].y - objetos[j].limite_y) &&
                        balas[i].y < (objetos[j].y + objetos[j].limite_y))
                    {
                        balas[i].vida = false;
                        objetos[j].vida = false;
                        pontos += 10;
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

    if (!al_init_font_addon())
    {
        fprintf(stderr,"Falha ao inicializar fontes addon.\n");
        return false;
    }
    if (!al_init_ttf_addon()){
        fprintf(stderr,"Falha ao inicializar add-on allegro_ttf");
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
    semaudio = al_load_sample("semaudio.wav");
    if (!musica_capa)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
            // CONFIGURAÇÃO DO TÍTULO DA JANELA
    al_set_window_title(janela, "SAVE THE EARTH");

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

    void error_msg(char *text)
    {
    al_show_native_message_box(NULL,"ERRO", "Ocorreu o seguinte erro e o programa sera finalizado:", text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
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
