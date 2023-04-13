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

    /* Tworzymy zmienn¹ w której przechowamy adres kolejki */
    ALLEGRO_EVENT_QUEUE* kolejka = NULL;

    /* Tworzymy zmienn¹ w której przetrzymamy event do obs³u¿enia */
    ALLEGRO_EVENT ev1;

    if (!al_init())
    {
        fprintf(stderr, "Failed to initialize allegro!\n");
        return -1;
    }
    al_init_image_addon();
    /* Allokujemy pamiêæ dla kolejki i sprawdzamy czy funkcja nie zwróci³a b³êdu */
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


    /* Skoro mamy kolejkê i display, rejestrujemy display jako Ÿród³o eventów */
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
        

        /* Je¿eli wyst¹pi event, wysy³amy go do zmiennej ev1 */
        al_wait_for_event(kolejka, &ev1);

        /* Je¿eli wyst¹pi³ event, bêdzie posiadaæ okreœlony typ; Sprawdzamy czy typ jest równy wartoœci,
                 * która mówi nam ¿e zosta³ wciœniêty przycisk exit */




        if (ev1.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            /* Je¿eli prawda, przerwij pêtle while */
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
    /* Zwalniamy pamiêæ po kolejce */
    al_destroy_event_queue(kolejka);
    return 0;
}