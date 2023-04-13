#include <iostream>
#include <allegro5/allegro.h>
#define height 720
#define width 1280
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#define BACKGROUND_FILE     "Picture/Background.bmp"


int main(int argc, char* argv[])
{
    if (!al_init_primitives_addon()) { return -1; }
    ALLEGRO_BITMAP* background = NULL;
    ALLEGRO_DISPLAY* display = NULL;

    /* Tworzymy zmienn� w kt�rej przechowamy adres kolejki */
    ALLEGRO_EVENT_QUEUE* kolejka = NULL;

    /* Tworzymy zmienn� w kt�rej przetrzymamy event do obs�u�enia */
    ALLEGRO_EVENT ev1;

    if (!al_init())
    {
        fprintf(stderr, "Failed to initialize allegro!\n");
        return -1;
    }
    al_init_image_addon();
    /* Allokujemy pami�� dla kolejki i sprawdzamy czy funkcja nie zwr�ci�a b��du */
    kolejka = al_create_event_queue();
    if (kolejka == NULL)
    {
        fprintf(stderr, "Failed to create event queue!\n");
        return -1;
    }

    display = al_create_display(width, height);
    if (display == NULL)
    {
        fprintf(stderr, "Failed to create display!\n");
        return -1;
    }


    /* Skoro mamy kolejk� i display, rejestrujemy display jako �r�d�o event�w */
    al_register_event_source(kolejka, al_get_display_event_source(display));
    al_clear_to_color(al_map_rgb(38, 95, 55));
    al_flip_display();

    background = al_load_bitmap(BACKGROUND_FILE);
    if (!background)
    {
        fprintf(stderr, "failed to load background bitmap!\n");
        return -1;
    }


    bool open = true;
    while (true)
    {
        

        /* Je�eli wyst�pi event, wysy�amy go do zmiennej ev1 */
        al_wait_for_event(kolejka, &ev1);

        /* Je�eli wyst�pi� event, b�dzie posiada� okre�lony typ; Sprawdzamy czy typ jest r�wny warto�ci,
                 * kt�ra m�wi nam �e zosta� wci�ni�ty przycisk exit */




        if (ev1.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            /* Je�eli prawda, przerwij p�tle while */
            break;
        }
        al_draw_bitmap(background, width / 2, height / 2, 0);
        al_draw_rectangle(2, 2, 62, 35, al_map_rgb(10, 10, 10), 50);
        al_flip_display();
        printf("dupa");
    }

    al_destroy_bitmap(background);
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    /* Zwalniamy pami�� po kolejce */
    al_destroy_event_queue(kolejka);
    return 0;
}