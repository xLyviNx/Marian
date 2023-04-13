#include <iostream>
#include <allegro5/allegro.h>
#define height 720
#define width 1280



int main(int argc, char* argv[])
{
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

    al_clear_to_color(al_map_rgb(20, 75, 80));
    al_flip_display();

    /* Pêtla w której bêdzie pracowaæ program */
    bool open = true;
    while (open)
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
    }

    al_destroy_display(display);

    /* Zwalniamy pamiêæ po kolejce */
    al_destroy_event_queue(kolejka);
    return 0;
}