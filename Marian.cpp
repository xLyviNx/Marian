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
#define MARIAN_FILE "Picture/Marian.bmp"
#define BULLET_TEXTURE "Picture/bullet.bmp"

using namespace std;
double deltaTime = 0;

float clamp(float val, float min, float max)
{
    if (val > max && max>min)
    {
        return max;
    }
    else if (val < min && min < max)
    {
        return min;
    }
    else
    {
        return val;
    }
}


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

class Bullet
{
private:
    set<Bullet*>* mySet;
    public:
        float x;
        float y;
        float Speed;
        int spritewidth;
        int spriteheight;
        float spriteScale;
        int scaled_spritewidth;
        int scaled_spriteheight;
        int direction;
        ALLEGRO_BITMAP* sprite = NULL;

        Bullet(float nX, float nY, int dir, float speed, set<Bullet*>* mset)
        {
            mySet = mset;
            this->spritewidth = 48;
            this->spriteheight = 17;
            this->spriteScale = 0.4;
            this->direction = dir;
            this->Speed = speed;
            this->x = nX;
            this->y = nY;
            scaled_spritewidth = ceil(spritewidth * spriteScale);
            scaled_spriteheight = ceil(spriteheight * spriteScale);
            sprite = al_load_bitmap(BULLET_TEXTURE);
            if (mySet != NULL && sprite)
            {
                mySet->insert(this);
            }
            else {
                delete(this);
            }
        }
        ~Bullet()
        {
            if (mySet != NULL)
            {
                mySet->erase(this);
            }
            //delete(this);
        }
        void renderMe()
        {
            if (this->sprite) {
                if (this->direction == 0) { delete(this); return; }
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - (this->direction * this->scaled_spritewidth / 2), this->y - this->scaled_spriteheight / 2, this->direction * this->scaled_spritewidth, this->scaled_spriteheight, 0);
                al_draw_filled_circle(this->x, this->y, 2, al_map_rgb(255, 100, 100));
                //al_draw_bitmap(this->marian, width / 2, height / 2, 0);
            }
        }
        void UpdateBullet()
        {
            this->x = this->x + ((this->direction * Speed * 100) * deltaTime);
            this->y = this->y + (9.81 * deltaTime);
            renderMe();
        }
};

class Player : public Behaviour
{
private:
    float moveForceX = 0;
    float velocityY=0;
    float shootCd = 0;
    void renderMe()
    {
        if (this->marian) {
            if (lastMoveForceX == 0) { lastMoveForceX = 1; }
            al_draw_scaled_bitmap(this->marian, 0, 0, spritewidth, spriteheight, x-(lastMoveForceX * scaled_spritewidth/2), y-scaled_spriteheight/2, lastMoveForceX * scaled_spritewidth, scaled_spriteheight, 0);
            al_draw_filled_circle(this->x, this->y, 10, al_map_rgb(255, 100, 100));
            //al_draw_bitmap(this->marian, width / 2, height / 2, 0);
        }
    }
    set<int>* pressedKeys = NULL;
    void Jump()
    {
        if (isGrounded)
        {
            cout << "Jump\n";
            this->velocityY -= jumpForce*15;
        }
    }
    void Shoot()
    {
        if (this->shootCd <= 0)
        {
            if (this->lastMoveForceX == 0) { this->lastMoveForceX = 1; }
            Bullet* bul = new Bullet(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y+5, lastMoveForceX, 15, &Bullets);
            if (bul != NULL)
            {
                this->shootCd = 0.15;
            }
        }
    }
    void loopBullets()
    {
        for (set<Bullet*>::iterator it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->UpdateBullet();
        }
    }
public:
    float x;
    float y;
    float Speed;
    int spritewidth;
    int spriteheight;
    float spriteScale;
    int scaled_spritewidth;
    int scaled_spriteheight;
    bool isGrounded = false;
    float jumpForce = 60;
    float lastMoveForceX = 0;

    ALLEGRO_BITMAP* marian = NULL;
    set<Bullet*> Bullets;
    void Update()
    {
        renderMe();
        moveForceX = 0;
        float gravForce = 0;
        isGrounded = false;
        if (y + scaled_spriteheight/2 < 580)
        {
            gravForce = 9.81;
        }
        else
        {
            if (!isGrounded)
            {
                velocityY = 0;
            }
            isGrounded = true;
        }

        bool extendJump = false;
        for (set<int>::iterator it = (*pressedKeys).begin(); it != (*pressedKeys).end(); it++)
        {
            if ((*it) == ALLEGRO_KEY_D)
            {
                moveForceX += 1;
            }        
            if ((*it) == ALLEGRO_KEY_A)
            {
                moveForceX -= 1;
            }
            if ((*it) == ALLEGRO_KEY_W)
            {
                this->Jump();
                extendJump = true;
            }
            if ((*it) == ALLEGRO_KEY_SPACE)
            {
                this->Shoot();
            }
        }
        if (moveForceX != 0) { lastMoveForceX = moveForceX; }
        //cout << "Force: " << moveForceX << endl;


        x += (moveForceX * Speed * 50) * deltaTime;
        if (!isGrounded) 
        {
            float change = extendJump ? 0.6 : 1;
            velocityY += (gravForce * 250 * change) * deltaTime;
        }
        cout << "Velocity: " << velocityY << endl;
        y = clamp(y + (velocityY * deltaTime), 0, 580-scaled_spriteheight/2);
        this->loopBullets();
        if (this->shootCd > 0) {
            this->shootCd -= deltaTime;
        }
    }
    void Start()
    {
        this->marian = al_load_bitmap(MARIAN_FILE);
        if (!this->marian)
        {
            cout << ("failed to load Marian bitmap!\n");
        }
        scaled_spritewidth = ceil(spritewidth * spriteScale);
        scaled_spriteheight = ceil(spriteheight * spriteScale);
        this->Speed = 6;
        cout << "Stworzono mnie: rozmiary? " << scaled_spriteheight << " " << scaled_spritewidth << endl;
    }
    Player(vector<Behaviour*>* bhvs, bool nodestroying, set<int>* keys) :Behaviour(bhvs, nodestroying)
    { 

        this->spritewidth = 356;
        this->spriteheight = 472; 
        this->x = 120; 
        this-> spriteScale = 0.28;
        this->y = height / 2;
        this->pressedKeys = keys;
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
        return -2;
    }

    display = al_create_display(width, height);
    if (display == NULL)
    {
        fprintf(stderr, "Failed to create display!\n");
        return -3;
    }


    /* Skoro mamy kolejkê i display, rejestrujemy display jako Ÿród³o eventów */
    al_register_event_source(kolejka, al_get_display_event_source(display));
    if (!al_install_keyboard())
    {
        return -4;
    }
    al_register_event_source(kolejka, al_get_keyboard_event_source());
    al_clear_to_color(al_map_rgb(38, 95, 55));
    al_flip_display();
    set<int> buttons;
    background = al_load_bitmap(BACKGROUND_FILE);
    if (!background)
    {
        cout<<("failed to load background bitmap!\n");
        return -5;
    }
    floor = al_load_bitmap(FLOOR_FILE);
    if (!floor)
    {
        cout << ("failed to load floor bitmap!\n");
        return -6;
    }
    double time = al_get_time();
    double oldTime = time;

    bool open = true;
    Player* plr = NULL;

    while (open)
    {
        al_wait_for_vsync();
        if (plr == NULL)
        {
            plr = new Player(&Behaviours, false, &buttons);
        }
        /* Je¿eli wyst¹pi event, wysy³amy go do zmiennej ev1 */
        al_wait_for_event_timed(kolejka, &ev1, 0);

        time = al_get_time();
        deltaTime = time - oldTime;
        oldTime = time;

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
        //al_draw_bitmap(marian, 0, 0, 0);

    
        loopBehaviours();

        al_flip_display();
    }

    al_destroy_bitmap(background);
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    /* Zwalniamy pamiêæ po kolejce */
    al_destroy_event_queue(kolejka);
    delete plr;
    return 0;
}