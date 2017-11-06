#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>

// ########################################################################################################################################

const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_SAMPLE *musica_capa = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *ajustes = NULL;
ALLEGRO_BITMAP *escolha = NULL;
ALLEGRO_BITMAP *botao_off = NULL;
ALLEGRO_BITMAP *correto = NULL;
ALLEGRO_BITMAP *fundo_nivel = NULL;
ALLEGRO_SAMPLE_ID *id_music = NULL;

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

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
bool tela_da_capa = true;
bool saire = false;

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

bool inicializar();

int main(void)
{
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
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
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

// ----------------------------------------------------------------------------------------------------------------------------------------
        // VERIFICA��O DO CARREGAMENTO DO BACKGROUND (FUNDO)
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

// ########################################################################################################################################

    while (!saire)
    {
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOT�O "X" DA TELA
                }

// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOT�O VOLTAR DA TELA AJUSTES
// ########################################################################################################################################

                if(btvoltar_datela_ajustes == true)
                {
                    tela_da_capa = true;
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 73 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 526 &&
                            evento.mouse.y <= 679)
                        {
                            btvoltar_datela_ajustes = true;
                            tela_ajustes = false;
                        }
                    }
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOT�O DE VOLTAR DA TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################

                if(btvoltar_datela_escolha == true)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                           tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 100 &&
                            evento.mouse.x <= 316 &&
                            evento.mouse.y >= 510 &&
                            evento.mouse.y <= 650)
                        {
                            tela_de_escolha = false;
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
                    saire = true; //BOT�O "X" DA TELA
                }
//=======================================================================================================
//      BOT�O PARA A TELA AJUSTES
                if(tela_ajustes == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 190 &&
                            evento.mouse.x <= 419 &&
                            evento.mouse.y >= 505 &&
                            evento.mouse.y <= 644)
                        {
                            tela_ajustes = true;
                            tela_da_capa = false;
                           // tela_de_escolha = false;
                        }
                    }
                }
//=======================================================================================================
//      BOT�O PARA A TELA DE ESCOLHA DE NAVES
                if(tela_de_escolha == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 475 &&
                            evento.mouse.x <= 718 &&
                            evento.mouse.y >= 505 &&
                            evento.mouse.y <= 644)
                        {
                            tela_de_escolha = true;
                            tela_da_capa = false;
                            tela_ajustes = false;
                        }

                    }if(btvoltar_datela_escolha == true)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                           tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 100 &&
                            evento.mouse.x <= 316 &&
                            evento.mouse.y >= 510 &&
                            evento.mouse.y <= 650)
                        {
                            tela_de_escolha = false;
                        }
                    }
                }
                }
//=======================================================================================================
//      BOT�O PARA SAIR DO JOGO
                if(saire == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_da_capa = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
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

            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE AJUSTES
// ########################################################################################################################################

            if(tela_ajustes == true)
            {
                tela_da_capa = false;
                saire = false;

                al_draw_bitmap(ajustes, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOT�O "X" DA TELA
                }
//=======================================================================================================
//      BOT�O DE VOLTAR PARA A TELA DA CAPA
                if(btvoltar_datela_ajustes == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_ajustes = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 73 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 526 &&
                            evento.mouse.y <= 679)
                        {
                            btvoltar_datela_ajustes = true;
                            tela_ajustes = false;
                        }
                    }
                }
//=======================================================================================================
//      BOT�O DE MELODIA (LIGAR E DESLIGAR)
                if(melodia == true)  // VERIFICA��O DO ESTADO DO BOT�O
                {
                    al_draw_bitmap(botao_off, 420, 175, 0);

                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            melodia = true;
                        }
                    }

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
                else if(melodia == false)  // VERIFICA��O DO ESTADO DO BOT�O
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_ajustes = true;
                        }
                    }

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
//      BOT�O DE EFEITOS SONOROS (LIGAR E DESLIGAR)

                if(efeito_sonoro == false) // VERIFICA��O DO ESTADO DO BOT�O
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_ajustes = true;
                        }
                    }

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
                else if(efeito_sonoro == true)  // VERIFICA��O DO ESTADO DO BOT�O
                {
                    al_draw_bitmap(botao_off, 855, 410, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            efeito_sonoro = true;

                        }
                    }

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
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################
          if(tela_de_escolha == true)
            {
                tela_da_capa = false;
                saire = false;

                al_draw_bitmap(escolha, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; // BOT�O "X" DA TELA
                }

//=======================================================================================================
//      BOT�O DE VOLTAR PARA A TELA DE CAPA
                if(btvoltar_datela_escolha == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_de_escolha = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 100 &&
                            evento.mouse.x <= 316 &&
                            evento.mouse.y >= 510 &&
                            evento.mouse.y <= 650)
                        {
                            btvoltar_datela_escolha = true;
                            tela_de_escolha = false;
                        }
                    }
                }
//=======================================================================================================
//     BOT�O DE COME�AR PARA JOGAR
               if(btcomecar_datela_escolha == false) // VERIFICA��O DO ESTADO DO BOT�O
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_de_escolha = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 554 &&
                            evento.mouse.x <= 747 &&
                            evento.mouse.y >= 534 &&
                            evento.mouse.y <= 676)
                        {
                            btcomecar_datela_escolha = true;
                        }
                    }
                }

//=======================================================================================================
//      BOT�O DE ESCOLHA DA NAVE 1
                if(nave1 == false)
                {

                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_de_escolha = true;
                        }
                    }
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 163 &&
                            evento.mouse.x <= 425 &&
                            evento.mouse.y >= 192 &&
                            evento.mouse.y <= 466)
                        {
                            //tela_de_escolha = true;
                            nave1 = true;
                            nave2 = false;
                            nave3 = false;
                        }
                    }
                }

                else if(nave1 == true)
                {
                    al_draw_bitmap(correto, 200, 241, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            nave1 = true;
                        }

                    }
                }

//=======================================================================================================
//      BOT�O DE ESCOLHA DA NAVE 2
                if(nave2 == false)
                {

                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_de_escolha = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 512 &&
                            evento.mouse.x <= 765 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave2 = true;
                            nave1 = false;
                            nave3 = false;
                        }
                    }
                }

                else if(nave2 == true)
                {
                    al_draw_bitmap(correto, 539, 241, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            nave2 = true;
                        }
                    }
                }

//=======================================================================================================
//      BOT�O DE ESCOLHA DA NAVE 3
                if(nave3 == false)
                {

                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_de_escolha = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 806 &&
                            evento.mouse.x <= 1115 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave3 = true;
                            nave1 = false;
                            nave2 = false;
                        }
                    }
                }

                else if(nave3 == true)
                {
                    al_draw_bitmap(correto, 888, 241, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            nave3 = true;
                        }
                    }
                }
            }
//=======================================================================================================
//      BOT�O DE COMECAR DA TELA DE ESCOLHA

            if(btcomecar_datela_escolha == true) // VERIFICA��O DO ESTADO DO BOT�O
                {
                    al_draw_bitmap(fundo_nivel, 0, 0, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            btcomecar_datela_escolha = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 554 &&
                            evento.mouse.x <= 747 &&
                            evento.mouse.y >= 534 &&
                            evento.mouse.y <= 676)
                        {
                            btcomecar_datela_escolha = false;
                        }
                    }
                }
        }

        // ATUALIZA A TELA
        al_flip_display();
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(ajustes);
    al_destroy_bitmap(botao_off);
    al_destroy_bitmap(escolha);
    al_destroy_sample(musica_capa);
    al_destroy_bitmap(correto);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

bool inicializar()
{

    if (!al_init())
    {
        fprintf(stderr, "N�o inicializou Allegro.\n");
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar �udio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de �udio.\n");
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de �udio.\n");
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
    // Configura o t�tulo da janela
    al_set_window_title(janela, "SAVE THE EARTH - The Social Problems");

    // Torna apto o uso de mouse na aplica��o
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    // Atribui o cursor padr�o do sistema para ser usado
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


    // Dizemos que vamos tratar os eventos vindos do mouse
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################



