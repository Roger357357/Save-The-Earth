#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>

const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_SAMPLE *musica_capa = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *ajustes = NULL;
ALLEGRO_BITMAP *botao_off = NULL;
ALLEGRO_SAMPLE_ID *id_music = NULL;

bool tela_ajustes = false;
bool melodia = false;
bool efeito_sonoro = false;
bool iniciar = false;
bool nave1 = false;
bool nave2 = false;
bool nave3 = false;
bool voltar_tela_inicial = false;
bool tela_inicial = true;
bool saire = false;

bool inicializar();

int main(void)
{
    if (!inicializar())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }


    al_set_sample_instance_playing(musica_capa, true);
    al_init_image_addon();
    al_init_primitives_addon();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_install_keyboard();

    fundo = al_load_bitmap("fundo da capa.png");
    ajustes = al_load_bitmap("setting.png");
    botao_off = al_load_bitmap("off (2).png");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar fundo.png\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }


    while (!saire)
    {
        // Verificamos se h� eventos na fila
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true;
                }

            if(tela_inicial == true)
            {
                al_draw_bitmap(fundo , 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true;
                }

                if(tela_ajustes == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_inicial = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 190 &&
                            evento.mouse.x <= 419 &&
                            evento.mouse.y >= 505 &&
                            evento.mouse.y <= 644)
                        {
                            tela_ajustes = true;
                            tela_inicial = false;
                        }
                    }
                }

                if(iniciar == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_inicial = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 475 &&
                            evento.mouse.x <= 718 &&
                            evento.mouse.y >= 505 &&
                            evento.mouse.y <= 644)
                        {
                            iniciar = true;
                            tela_inicial = false;
                        }

                    }
                }

                if(saire == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_inicial = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
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

            if(tela_ajustes == true)
            {
                tela_inicial = false;
                al_draw_bitmap(ajustes, 0, 0, 0);

                if(voltar_tela_inicial == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
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
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 58 &&
                            evento.mouse.x <= 280 &&
                            evento.mouse.y >= 547 &&
                            evento.mouse.y <= 675)
                        {
                            voltar_tela_inicial = true;
                            tela_ajustes = false;
                        }
                    }
                }


                    if(voltar_tela_inicial == true)
                {
                    tela_inicial = true;
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            tela_inicial = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 58 &&
                            evento.mouse.x <= 280 &&
                            evento.mouse.y >= 547 &&
                            evento.mouse.y <= 675)
                        {
                            voltar_tela_inicial = true;
                            tela_ajustes = false;
                        }
                    }
                }


                if(melodia == true)
                {
                    al_draw_bitmap(botao_off, 435, 196, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            melodia = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = false;
                        }
                    }
                }
                else if(melodia == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
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
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = true;
                        }
                    }
                }
                if(efeito_sonoro == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
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
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 875 &&
                            evento.mouse.x <= 1118 &&
                            evento.mouse.y >= 424 &&
                            evento.mouse.y <= 595)
                        {
                            efeito_sonoro = true;

                        }
                    }
                }
                else if(efeito_sonoro == true)
                {
                    al_draw_bitmap(botao_off, 865, 424, 0);
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            efeito_sonoro = true;

                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
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



            if(iniciar == true)
            {
                fundo = al_load_bitmap("tela escolha voltar.png");
                al_draw_bitmap(fundo, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true;
                }
                if(nave1 == true)
                {
                    fundo = al_load_bitmap("nave1.png");
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            iniciar = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 163 &&
                            evento.mouse.x <= 425 &&
                            evento.mouse.y >= 192 &&
                            evento.mouse.y <= 466)
                        {
                            nave1 = true;
                            iniciar = false;
                        }
                    }
                }
                else if(nave1 == false)
                {
                    fundo = al_load_bitmap("tela escolha voltar.png");
                    if (evento.type == ALLEGRO_EVENT_MOUSE_AXES || ALLEGRO_EVENT_MOUSE_WARPED)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 0 &&
                            evento.mouse.x <= 1280 &&
                            evento.mouse.y >= 0 &&
                            evento.mouse.y <= 720)
                        {
                            iniciar = true;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        // Verificamos se ele est� sobre a regi�o da imagem habilitado
                        if (evento.mouse.x >= 163 &&
                            evento.mouse.x <= 425 &&
                            evento.mouse.y >= 192 &&
                            evento.mouse.y <= 466)
                        {
                            nave1 = false;
                            iniciar = true;
                        }
                    }
                }
        }



        }
        // Atualiza a tela
        al_flip_display();
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(ajustes);
    al_destroy_sample(musica_capa);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}

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
