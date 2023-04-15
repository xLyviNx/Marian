#include <iostream>
#include <allegro5/allegro.h>
#define height 720 
#define width 1280
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <set>
#include <vector>
#define BACKGROUND_FILE "Picture/Background.bmp"
#define MAP1_width 3840
#define FLOOR_FILE "Picture/floor.bmp"
#define MARIAN_FILE "Picture/Marian.bmp"
#define BULLET_TEXTURE "Picture/bullet.bmp"
#define PLATFORM_FILE "Picture/platform1x1.bmp"

using namespace std;
class Behaviour
{   
protected:
    vector<Behaviour*>* vec = NULL;
public:
    virtual void Update() {};
    virtual void Start() {};
    //bool dontDestroy;
    Behaviour(vector<Behaviour*>* bhvs)
    {
        cout<< "Constructed NEW Behaviour.\n";
        //this->dontDestroy = nodestroying;
        this->vec = bhvs;
        if (this->vec != NULL)
        {
            (*(this->vec)).push_back(this);
        }
    }
    ~Behaviour()
    {
        cout << "DESTRUCTION OF Behaviour.\n";
        if (this->vec != NULL)
        {
            auto it = find(vec->begin(), vec->end(), this);
            if (it != vec->end())
            {
                vec->erase(it);
            }
        }
    }
};
vector<Behaviour*> Behaviours;

double deltaTime = 0;
float cam_x_offset;
float clamp(float val, float min, float max)
{
    if (val > max && max > min)
    {
        return max;
    }
    else if (val < min && min < max)
    {
        return min;
    }
    else if (val < min && min == max)
    {
        return min;
    }
    else
    {
        return val;
    }
}

class Platform : public Behaviour
{
private:
    int blocks = 0;
    ALLEGRO_BITMAP* sprite = NULL;
public:
    float x = 0;
    float y = 0;
    int spritewidth = 65;
    int spriteheight = 65;
    float spriteScale = 1;
    int scaled_spritewidth;
    int scaled_spriteheight;
    Platform(vector<Behaviour*>* bhvs, float nX, float nY, int blocksAroundMiddle, ALLEGRO_BITMAP* mySprite) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->sprite = mySprite;
        this->blocks = blocksAroundMiddle;

        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
    };
    void Update()
    {
        if (sprite)
        {
            if (this->x - cam_x_offset > 0 && this->x - cam_x_offset < width) {
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset + this->scaled_spritewidth / 2, this->y + this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
                for (int i = 1; i <= this->blocks; i++)
                {
                    al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset + this->scaled_spritewidth / 2 + (i * this->scaled_spritewidth), this->y + this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
                    al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset + this->scaled_spritewidth / 2 - (i * this->scaled_spritewidth), this->y + this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
                }
            }
        }
    }
    bool isMyBlock(float cx, float cy)
    {
        float leftside = this->x - this->scaled_spritewidth / 2 - (this->blocks * this->scaled_spritewidth);
        float rightside = this->x + this->scaled_spritewidth / 2 + (this->blocks * this->scaled_spritewidth);
        float top = this->y - this->scaled_spriteheight / 2;
        float bottom = this->y + this->scaled_spriteheight / 2;
        if (cx >= leftside && cx <= rightside)
        {
            if (cy <= top && cy >= bottom)
            {
                return true;
            }
        }
        return false;
    }
};


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
        float lifetime = 2;
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
                set<Bullet*>::iterator it = mySet->find(this);
                if (it != mySet->end())
                {
                    mySet->erase(it);
                }
            }
            al_destroy_bitmap(sprite);
        }
        void renderMe()
        {
            if (this->sprite) {
                if (this->direction == 0) { delete(this); return; }
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset - (this->direction * this->scaled_spritewidth / 2), this->y - this->scaled_spriteheight / 2, this->direction * this->scaled_spritewidth, this->scaled_spriteheight, 0);
                al_draw_filled_circle(this->x-cam_x_offset, this->y, 2, al_map_rgb(255, 100, 100));
                //al_draw_bitmap(this->marian, width / 2, height / 2, 0);
            }
        }
        void UpdateBullet()
        {
            this->x = this->x + ((this->direction * Speed * 100) * deltaTime);
            this->y = this->y + (9.81 * deltaTime);
            lifetime -= deltaTime;
            renderMe();
        }
};
class BulletParticle
{
private:
    set<BulletParticle*>* mySet;
    float th = 0.5;
    bool reachedth = false;
public:
    float x;
    float y;
    float timeLeft = 0.0;
    BulletParticle(float nX, float nY, set<BulletParticle*>* mset)
    {
        mySet = mset;
        this->x = nX;
        this->y = nY;
        this->timeLeft = 0;
        if (mySet) {
            mySet->insert(this);
        }
        else
        {
            delete(this);
        }
    }
    ~BulletParticle()
    {
        if (mySet != NULL)
        {
            set<BulletParticle*>::iterator it = mySet->find(this);
            if (it != mySet->end())
            {
                mySet->erase(it);
            }
        }
        //delete(this);
    }
    void renderMe()
    {
        if (this->timeLeft>0) {
            al_draw_filled_circle(this->x - cam_x_offset, this->y, timeLeft * 30, al_map_rgba(255, 230, 10, (int)(timeLeft * 300)));
            this->x -= 30 * deltaTime;
            //al_draw_bitmap(this->marian, width / 2, height / 2, 0);
        }
    }
    void UpdateBulletParticle()
    {
        if (!reachedth)
        {
            if (timeLeft <= th)
            {
                timeLeft += deltaTime * 10;
            }
            else {
                reachedth = true;
            }
        }
        else if (timeLeft > 0) {
            timeLeft -= deltaTime * 3;
            renderMe();
        }
        
    }
};
class Player : public Behaviour
{
private:
    float moveForceX = 0;
    float velocityY=0;
    float shootCd = 0;
    float shot = 0;
    void renderMe()
    {
        if (this->marian) {
            if (lastMoveForceX == 0) { lastMoveForceX = 1; }
            al_draw_scaled_bitmap(this->marian, 0, 0, spritewidth, spriteheight, x - cam_x_offset -(lastMoveForceX * scaled_spritewidth/2), y-scaled_spriteheight/2, lastMoveForceX * scaled_spritewidth, scaled_spriteheight, 0);
            //al_draw_filled_circle(this->x, this->y, 10, al_map_rgb(255, 100, 100));
            //al_draw_bitmap(this->marian, width / 2, height / 2, 0);
        }
    }
    set<int>* pressedKeys = NULL;
    set<Platform*>* platforms = NULL;
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
            Bullet* bul = new Bullet(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y + 10, lastMoveForceX, 15, &Bullets);
            BulletParticle* bulP = new BulletParticle(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y + 10, &BulletParticles);
            if (bul != NULL)
            {
                this->shootCd = 0.13;
                shot += 5;
            }
        }
    }
    void loopBullets()
    {
        set<Bullet*> BtoDelete;
        for (set<Bullet*>::iterator it = Bullets.begin(); it != Bullets.end(); it++)
        {
            (*it)->UpdateBullet();
            if ((*it)->lifetime <= 0)
            {
                BtoDelete.insert((*it));
            }
        }
        set<BulletParticle*> BPtoDelete;
        for (set<BulletParticle*>::iterator it = BulletParticles.begin(); it != BulletParticles.end(); it++)
        {
            if ((*it) != NULL) {
                (*it)->UpdateBulletParticle();
                if ((*it)->timeLeft <= 0)
                {
                    BPtoDelete.insert((*it));
                }
            }
        }
        for (set<Bullet*>::iterator it = BtoDelete.begin(); it != BtoDelete.end(); it++) {
            Bullets.erase((*it));
        }
        BtoDelete.clear();
        for (set<BulletParticle*>::iterator it = BPtoDelete.begin(); it != BPtoDelete.end();it++) {
            BulletParticles.erase(*it);
        }
        BPtoDelete.clear();
    }
    bool platformBlock(float cx, float cy)
    {
        for (set<Platform*>::iterator it = (*platforms).begin(); it != (*platforms).end(); it++)
        {
            if ((*it)->isMyBlock(cx, cy))
            {
                return true;
            }
        }
        return false;
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
    set<BulletParticle*> BulletParticles;
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
        bool fastJump = false;
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

            if ((*it) == ALLEGRO_KEY_W || (*it) == ALLEGRO_KEY_SPACE)
            {
                this->Jump();
                extendJump = true;
            }
            if ((*it) == ALLEGRO_KEY_S)
            {
                fastJump = true;
            }
            if ((*it) == ALLEGRO_KEY_LCTRL)
            {
                this->Shoot();
            }
        }
        if (moveForceX != 0) { lastMoveForceX = moveForceX; }
        //cout << "Force: " << moveForceX << endl;

        float newX = this->x + (this->moveForceX * this->Speed * 50) * deltaTime;
        this->x = clamp(newX, this->scaled_spritewidth/2, MAP1_width-this->scaled_spritewidth/2);
        this->x = clamp(this->x - (this->lastMoveForceX * 10 * this->shot) * deltaTime, this->scaled_spritewidth / 2, MAP1_width - this->scaled_spritewidth / 2);
        if (this->shot > 0) {
            this->shot = clamp(shot-deltaTime * 40, 0, 40);
        }
        if (!isGrounded) 
        {
            float change = fastJump ? 3 : extendJump ? 0.6 : 1;
            velocityY += (gravForce * 250 * change) * deltaTime;
        }
        //cout << "Velocity: " << velocityY << endl;
        y = clamp(y + (velocityY * deltaTime), 0, 580-scaled_spriteheight/2);
        this->loopBullets();
        if (this->shootCd > 0) {
            this->shootCd -= deltaTime;
        }

        if (this->x > cam_x_offset + width)
        {
            cam_x_offset += width;
        }
        else if (this->x < cam_x_offset)
        {
            cam_x_offset -= width;
        }
        cout << "Cam X Offset: " << cam_x_offset << endl;
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
    Player(vector<Behaviour*>* bhvs, set<int>* keys, set<Platform*>* pts) :Behaviour(bhvs)
    { 
        platforms = pts;
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
    ALLEGRO_BITMAP* platformSprite = NULL;
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
    platformSprite = al_load_bitmap(PLATFORM_FILE);
    if (!platformSprite)
    {
        cout << ("failed to load PLATFORM bitmap!\n");
        return -7;
    }

    double time = al_get_time();
    double oldTime = time;

    bool open = true;
    Player* plr = NULL;
    short int Level = 1;
    set<Platform*> platforms;
    if (Level == 1)
    {
        platforms.insert(new Platform(&Behaviours, 525, 340, 2, platformSprite));
    }
    while (open)
    {
        al_wait_for_vsync();
        if (plr == NULL)
        {
            plr = new Player(&Behaviours, &buttons, &platforms);
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
        al_draw_bitmap(floor, 0-cam_x_offset, 0, 0);
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