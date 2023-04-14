#include <iostream>
#include <allegro5/allegro.h>
#define height 720 
#define width 1280
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <set>
#include <vector>
#define BACKGROUND_FILE "Picture/Background.bmp"
#define FLOOR_FILE "Picture/floor.bmp"

using namespace std;
class Behaviour
{   
protected:
    vector<Behaviour*>* vec = NULL;
public:
    virtual void Update() {};
    virtual void Start() {};
    bool dontDestroy;
    Behaviour(vector<Behaviour*>* bhvs, bool nodestroying)
    {
        cout<< "Constructed NEW Behaviour.\n";
        this->dontDestroy = nodestroying;
        this->vec = bhvs;
        if (this->vec != NULL)
        {
            (*(this->vec)).push_back(this);
        }
    }
};
vector<Behaviour*> Behaviours;

class Player : public Behaviour
{
private:
    void renderMe()
    {
        al_draw_filled_rectangle(this->x - this->pixelSize / 2, this->y - this->pixelSize / 2, this->x + this->pixelSize / 2, this->y + this->pixelSize / 2, al_map_rgb(160, 160, 160));
    }
public:
    float x;
    float y;
    int pixelSize;
    void Update()
    {
        renderMe();
    }
    void Start()
    {
        cout << "Stworzono mnie\n";
    }
    Player(vector<Behaviour*>* bhvs, bool nodestroying) :Behaviour(bhvs, nodestroying)
    { 
        this->pixelSize = 64;
        this->x = 20; 
        this->y = height / 2;
        this->Start();
    };
};

void loopBehaviours()
{
    for (vector<Behaviour*>::iterator it = Behaviours.begin(); it != Behaviours.end(); it++)
    {
        (*it)->Update();
    }
}
int main(int argc, char* argv[])
{
    if (!al_init_primitives_addon()) { return -1; }
    ALLEGRO_BITMAP* background = NULL;
    ALLEGRO_BITMAP* floor = NULL;
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
    set<int> buttons;
    background = al_load_bitmap(BACKGROUND_FILE);
    if (!background)
    {
        cout<<("failed to load background bitmap!\n");
        return -1;
    }
    floor = al_load_bitmap(FLOOR_FILE);
    if (!floor)
    {
        cout << ("failed to load floor bitmap!\n");
        return -2;
    }

    bool open = true;
    Player* plr = NULL;
    while (open)
    {
        
        if (plr == NULL)
        {
            plr = new Player(&Behaviours, false);
        }

        loopBehaviours();

        /* Je�eli wyst�pi event, wysy�amy go do zmiennej ev1 */
        al_wait_for_event_timed(kolejka, &ev1, 0.01);

        /* Je�eli wyst�pi� event, b�dzie posiada� okre�lony typ; Sprawdzamy czy typ jest r�wny warto�ci,
                 * kt�ra m�wi nam �e zosta� wci�ni�ty przycisk exit */

        switch (ev1.type)
        {
            case (ALLEGRO_EVENT_DISPLAY_CLOSE):
            {
                open = false;
                break;
            }
            case ALLEGRO_EVENT_KEY_DOWN:
            {
                buttons.insert(ev1.keyboard.keycode);
                break;
            }
            case ALLEGRO_EVENT_KEY_UP:
            {
                buttons.erase(ev1.keyboard.keycode);
                break;
            }
        }

        al_draw_bitmap(background, 0, 0, 0);
        al_draw_bitmap(floor, 0, 0, 0);
    


        al_flip_display();
    
    }

    al_destroy_bitmap(background);
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    /* Zwalniamy pami�� po kolejce */
    al_destroy_event_queue(kolejka);
    delete plr;
    return 0;
}