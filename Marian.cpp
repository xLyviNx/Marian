#include <iostream>
#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <set>
#include <vector>
#include "Base.h"
#include "MenuClass.h"
#define BACKGROUND_FILE "Picture/Background.bmp"
#define MAP1_width 3840
#define FLOOR_FILE "Picture/floor.bmp"
#define MARIAN_FILE "Picture/Marian.bmp"
#define BULLET_TEXTURE "Picture/bullet.bmp"
#define BULLET_TEXTURE2 "Picture/Buletv2.bmp"
#define PLATFORM_FILE "Picture/platform1x1.bmp"
#define ENEMY_TEXTURE "Picture/Enemy.bmp"
#define PLATFORM2_FILE "Picture/platform1x1_2.bmp"
#define PLATFORM3_FILE "Picture/platform1x1_3.bmp"
#define FINISH_FILE "Picture/Finish.bmp"
#define maxY 100
#include "saveSystem.h"
#include "floorO.h"
using namespace std;

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
    bool Vertical = false;
    Platform(vector<Behaviour*>* bhvs, float nX, float nY, int blocksAroundMiddle, bool vertical, ALLEGRO_BITMAP* mySprite) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->sprite = mySprite;
        this->blocks = blocksAroundMiddle;
        this->Vertical = vertical;
        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
    };
    void Update()
    {
        if (sprite)
        {
            float x0 = this->x;
            float y0 = this->y;
            if (x0 >= cam_x_offset - this->scaled_spritewidth / 2 && x0 <= WIDTH + cam_x_offset + this->scaled_spritewidth/2) {
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset - this->scaled_spritewidth / 2, y0 - this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
            }
            for (int i = 1; i <= this->blocks; i++)
            {

                float x1 = this->x - this->scaled_spritewidth/2;
                float x2 = this->x - this->scaled_spritewidth/2;
                float y1 = this->y;
                float y2 = this->y;
                if (Vertical)
                {
                    y1 = this->y + (i * this->scaled_spriteheight);
                    y2 = this->y - (i * this->scaled_spriteheight);
                }
                else
                {
                    x1 = this->x - this->scaled_spritewidth / 2 + (i * this->scaled_spritewidth);
                    x2 = this->x - this->scaled_spritewidth / 2 - (i * this->scaled_spritewidth);
                }
                if (x1>=cam_x_offset-this->scaled_spritewidth/2 && x1<= WIDTH + cam_x_offset + this->scaled_spritewidth / 2) {
                    al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, x1 - cam_x_offset, y1 - this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
                }
                if (x2 >= cam_x_offset - this->scaled_spritewidth / 2 && x2<= WIDTH + cam_x_offset + this->scaled_spritewidth / 2) {
                    al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, x2 - cam_x_offset, y2 - this->scaled_spriteheight / 2, this->scaled_spritewidth, this->scaled_spriteheight, 0);
                }
            }

        }
    }
    bool isMyBlock(float *cX, float *cY, float cW, float cH)
    {
        float left = this->x - (this->blocks * this->scaled_spritewidth) - (this->scaled_spritewidth / 2);
        float right = this->x + (this->blocks * this->scaled_spritewidth) + (this->scaled_spritewidth / 2);
        float top = this->y - ((float)this->scaled_spriteheight / 2);
        float bottom = this->y + ((float)this->scaled_spriteheight / 2);
        if (Vertical)
        {
            left = this->x - (this->scaled_spritewidth / 2);
            right = this->x + (this->scaled_spritewidth / 2);
            top = this->y - (this->blocks * this->scaled_spriteheight) - ((float)this->scaled_spriteheight / 2);
            bottom = this->y + (this->blocks * this->scaled_spriteheight) + ((float)this->scaled_spriteheight / 2);
        }
        //al_draw_filled_rectangle(left-cam_x_offset, top, right-cam_x_offset, bottom, al_map_rgb(255, 0, 0));
        //al_draw_filled_rectangle(x - scaled_spritewidth / 2, y - scaled_spriteheight / 2, x + scaled_spritewidth / 2, y + scaled_spriteheight / 2, al_map_rgb(0, 255, 0));

        bool isInsideHorizontal = *cX - cW / 2 <= right && *cX + cW / 2 >= left;
        bool isInsideVertical = *cY - cH / 2 <= bottom && *cY + cH / 2 >= top;
        bool col = (isInsideHorizontal ) && ( isInsideVertical);
        if (col)
        {
            if (*cY - cH / 2 < top)
            {
                *cY = top - cH / 2;
            }
            else if (*cY + cH / 2 > bottom)
            {
                *cY = bottom +1  + cH / 2;
            }
        }
        //cout << "Col: " << col << endl;
        return col;
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
        bool hurtPlayer = false;
        ALLEGRO_BITMAP* sprite = NULL;
        Bullet(float nX, float nY, int dir, float speed, ALLEGRO_BITMAP* spriteFile, set<Bullet*>* mset, bool hPlayer, int sizeX, int sizeY, float scale)
        {
            mySet = mset;
            this->hurtPlayer = hPlayer;
            this->spritewidth = sizeX;
            this->spriteheight = sizeY;
            this->spriteScale = scale;
            this->direction = dir;
            this->Speed = speed;
            this->x = nX;
            this->y = nY;
            this->sprite = spriteFile;
            scaled_spritewidth = ceil(spritewidth * spriteScale);
            scaled_spriteheight = ceil(spriteheight * spriteScale);
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
        }
        void renderMe()
        {
            //cout << "My bullet sprite is " << sprite << endl;
            if (this->sprite) {
                //cout << "Rendering" << endl;
                if (this->direction == 0) { delete(this); return; }
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset - (this->direction * this->scaled_spritewidth / 2), this->y - this->scaled_spriteheight / 2, this->direction * this->scaled_spritewidth, this->scaled_spriteheight, 0);
                //al_draw_filled_circle(this->x-cam_x_offset, this->y, 2, al_map_rgb(255, 100, 100));
                //al_draw_bitmap(this->marian, WIDTH / 2, HEIGHT / 2, 0);
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
            //al_draw_bitmap(this->marian, WIDTH / 2, HEIGHT / 2, 0);
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
    float shootCd = 0;
    float shot = 0;
    float jumpCd = 0;
    ALLEGRO_BITMAP* bulletsprite = NULL;
    ALLEGRO_FONT* debugFont = al_load_ttf_font("Fonts/data-unifon.ttf", 30, 0);
    void renderMe()
    {
        if (this->marian) {
            if (lastMoveForceX == 0) { lastMoveForceX = 1; }
            al_draw_scaled_bitmap(this->marian, 0, 0, spritewidth, spriteheight, x - cam_x_offset -(lastMoveForceX * scaled_spritewidth/2), y-scaled_spriteheight/2, lastMoveForceX * scaled_spritewidth, scaled_spriteheight, 0);
            //al_draw_filled_circle(this->x, this->y, 10, al_map_rgb(255, 100, 100));
            //al_draw_bitmap(this->marian, WIDTH / 2, HEIGHT / 2, 0);
        }
    }
    set<int>* pressedKeys = NULL;
    set<Platform*>* platforms = NULL;
    void Jump()
    {
        if (isGrounded && jumpCd<=0)
        {
            cout << "Jump\n";
            this->velocityY -= jumpForce*15;
            jumpCd += 0.2;
        }
    }
    void Shoot()
    {
        if (this->shootCd <= 0)
        {
            if (this->lastMoveForceX == 0) { this->lastMoveForceX = 1; }
            Bullet* bul = new Bullet(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y + 10, lastMoveForceX, 15, bulletsprite, Bullets,false, 48, 17, 0.4);
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
        for (set<Bullet*>::iterator it = (*Bullets).begin(); it != (*Bullets).end(); it++)
        {
            (*it)->UpdateBullet();
            if ((*it)->lifetime <= 0)
            {
                BtoDelete.insert((*it));
            }
            for (set<Platform*>::iterator it2 = (*platforms).begin(); it2 != (*platforms).end(); it2++)
            {
                if ((*it2)->isMyBlock(&(*it)->x, &(*it)->y, (*it)->scaled_spritewidth, (*it)->scaled_spriteheight))
                {
                    BtoDelete.insert((*it));
                }
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
            (*Bullets).erase((*it));
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
            if ((*it)->isMyBlock(&cx, &cy, this->scaled_spritewidth, this->scaled_spriteheight))
            {
                return true;
            }
        }
        return false;
    }
    bool checkGrounded(float *cx, float *cy)
    {
        for (set<Platform*>::iterator it = (*platforms).begin(); it != (*platforms).end(); it++)
        {
            if ((*it)->isMyBlock(cx, cy, this->scaled_spritewidth, this->scaled_spriteheight))
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
    int Health = 100;
    ALLEGRO_BITMAP* marian = NULL;
    set<Bullet*>* Bullets;
    set<BulletParticle*> BulletParticles;
    float velocityY = 0;
    void Update()
    {
        if (Health <= 0)
        {
            GlobalAction = 3;
            removing = true;
            return;
        }
        al_draw_textf(debugFont, al_map_rgb(80, 0, 0), 20, HEIGHT - 50, 0, "hp: %d, vel: %lf", Health, velocityY);
        bool lastGrounded = isGrounded;
        bool lowground = !(this->y + this->scaled_spriteheight / 2 < 580);
        isGrounded = checkGrounded(&(this->x), &(this->y)) || lowground;
        renderMe();
        moveForceX = 0;
        float gravForce = 0;
        //isGrounded = false;
        if (jumpCd > 0) {
            jumpCd -= deltaTime;
        }
        if (y + scaled_spriteheight/2 < 580)
        {
            gravForce = 9.81;
        }
        bool fastJump = false;
        bool extendJump = false;
        for (set<int>::iterator it = (*pressedKeys).begin(); it != (*pressedKeys).end(); it++)
        {
            if ((*it) == ALLEGRO_KEY_D)
            {
                moveForceX += 1;
            }        
            else if ((*it) == ALLEGRO_KEY_A)
            {
                moveForceX -= 1;
            }

            else if ((*it) == ALLEGRO_KEY_W || (*it) == ALLEGRO_KEY_SPACE)
            {
                this->Jump();
                extendJump = true;
            }
            else if ((*it) == ALLEGRO_KEY_S)
            {
                fastJump = true;
            }
            else if ((*it) == ALLEGRO_KEY_LCTRL)
            {
                this->Shoot();
            }
        }
        if (moveForceX != 0) { lastMoveForceX = moveForceX; }
        //cout << "Force: " << moveForceX << endl;

      
        if (this->shot > 0) {
            this->shot = clamp(shot-deltaTime * 40, 0, 40);
        }

        this->loopBullets();
        if (this->shootCd > 0) {
            this->shootCd -= deltaTime;
        }
        float newX = this->x;
        if (moveForceX) {
            newX = this->x + (this->moveForceX * this->Speed * 50) * deltaTime;
            if (!platformBlock(newX, this->y-1))
            {
                this->x = clamp(newX, this->scaled_spritewidth / 2, MAP1_width - this->scaled_spritewidth / 2);
            }
        }
        //al_draw_circle(newX, this->y, 2, al_map_rgb(100, 30, 30), 5);
        if (this->shot) {
            newX = this->x - (this->lastMoveForceX * 10 * this->shot) * deltaTime;
            if (!platformBlock(newX, this->y-1))
            {
                this->x = clamp(newX, this->scaled_spritewidth / 2, MAP1_width - this->scaled_spritewidth / 2);
            }

        }


        //cout << isGrounded << endl;
        if (!isGrounded)
        {
            float change = fastJump ? 3 : extendJump ? 0.6 : 1;
            velocityY += (gravForce * 250 * change) * deltaTime;

        }
        if (isGrounded && !lastGrounded)
        {
            velocityY = 0;
        }
        //cout << "Velocity: " << velocityY << endl;
        float newY = clamp(y + (velocityY * deltaTime), -maxY, 580 - scaled_spriteheight / 2);
        if (!isGrounded || velocityY < 0) {
            this->y = newY;
        }
        
        if (this->x > cam_x_offset + WIDTH)
        {
            cam_x_offset += WIDTH;
        }
        else if (this->x < cam_x_offset)
        {
            cam_x_offset -= WIDTH;
        }
        if (debugFont)
        {
            //al_draw_textf(debugFont, al_map_rgb(0, 50, 0), 15, 15, 0, " velocity: %lf, grounded: %d (%d), XY: %lf, %lf", velocityY, isGrounded, lowground, this->x, this->y);
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
    Player(vector<Behaviour*>* bhvs, set<int>* keys, set<Platform*>* pts, ALLEGRO_BITMAP* bulletSpr, set<Bullet*>* blts) :Behaviour(bhvs)
    { 
        platforms = pts;
        this->spritewidth = 356;
        this->spriteheight = 472; 
        this->x = 120; 
        this-> spriteScale = 0.28;
        this->y = HEIGHT / 2;
        this->pressedKeys = keys;
        this->bulletsprite = bulletSpr;
        this->Bullets = blts;
        this->Start();
    };
    ~Player() {
        if (marian != NULL) {
            al_destroy_bitmap(marian);
            cout << "Destroying MARIAN Bitmap" << endl;
            marian = NULL;
        }
        Behaviour::~Behaviour();
        
    }
};
class FinishScreen : public Behaviour
{
private:
    short int* level = 0;
    ALLEGRO_FONT* font = NULL;
    bool wasEnabled = false;
public:
    bool enabled;
    void Update()
    {

        //cout << "F1\n";
        if ((*level) > 0 && font && enabled)
        {
            if (!wasEnabled)
            {
                wasEnabled = true;
                if (SaveSystem::Instance && (*SaveSystem::Instance))
                {
                    char str[32] = "";
                    int nextlevel = (*level + 1);
                    _itoa_s(nextlevel, str, LINE_LENGTH, 10);
                    (*SaveSystem::Instance)->printAtLine(1, str);
                    GlobalAction = 3;
                }
                else
                {
                    SaveSystem::Create();
                    wasEnabled = false;
                }
            }
            //cout << "F2\n";
            al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba(0, 0, 0, 230));
            al_draw_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba(0, 255, 0, 240), 10);
            al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Level %d ukonczony!", *level);
        }
        else
        {
            //cout << "LV: " << *level << ", FONT: " << (font != NULL) << ", ENABLED: " << enabled << endl;
        }
    }
    void Start()
    {

    }
    FinishScreen(vector<Behaviour*>* bhvs, short int* lv, ALLEGRO_FONT* myfont) :Behaviour(bhvs)
    {
        this->level = lv;
        this->font = myfont;
        this->enabled = false;
    }
};
class Enemy : public Behaviour
{
private:
    ALLEGRO_BITMAP* sprite = NULL;
    int spritewidth = 356;
    int spriteheight = 472;
    float spriteScale = 0.21;
    int scaled_spritewidth;
    int scaled_spriteheight;
    bool walking = false;
    float shootCd = 0;
    ALLEGRO_BITMAP* bulletsprite = NULL;
    set<Bullet*> Bullets;
    set<Platform*>* platforms;
    Player** mPlayer = NULL;
    int Health = 50;
    void loopBullets()
    {
        if (mPlayer != NULL && (*mPlayer) != NULL)
        {
            for (set<Bullet*>::iterator pit = (*mPlayer)->Bullets->begin(); pit != (*mPlayer)->Bullets->end(); pit++)
            {
                if (this->x + this->scaled_spritewidth / 2 > (*pit)->x && this->x - this->scaled_spritewidth / 2 < (*pit)->x)
                {
                    if (this->y + this->scaled_spriteheight / 2 > (*pit)->y && this->y - this->scaled_spriteheight / 2 < (*pit)->y)
                    {
                        if (bulletsprite) {
                            this->Health -= 25;
                        }
                        (*pit)->lifetime = 0;
                    }
                }

            }
            set<Bullet*> BtoDelete;
            for (set<Bullet*>::iterator it = Bullets.begin(); it != Bullets.end(); it++)
            {
                (*it)->UpdateBullet();

                Player* plr = *mPlayer;
                if (plr->x + plr->scaled_spritewidth / 2 > (*it)->x && plr->x - plr->scaled_spritewidth / 2 < (*it)->x)
                {
                    if (plr->y + plr->scaled_spriteheight / 2 > (*it)->y && plr->y - plr->scaled_spriteheight / 2 < (*it)->y)
                    {
                        (*mPlayer)->Health -= 25;
                        BtoDelete.insert((*it));
                    }
                }

                if ((*it)->lifetime <= 0)
                {
                    BtoDelete.insert((*it));
                }
                for (set<Platform*>::iterator it2 = (*platforms).begin(); it2 != (*platforms).end(); it2++)
                {
                    if ((*it2)->isMyBlock(&(*it)->x, &(*it)->y, (*it)->scaled_spritewidth, (*it)->scaled_spriteheight))
                    {
                        BtoDelete.insert((*it));
                    }
                }
            }
            for (set<Bullet*>::iterator it = BtoDelete.begin(); it != BtoDelete.end(); it++) {
                Bullets.erase((*it));
            }
        }
    }
    void shoot()
    {
        if (bulletsprite) {
            if (this->shootCd <= 0)
            {
                if (this->dir == 0) { this->dir = 1; }
                Bullet* bul = new Bullet(this->x + ((this->scaled_spritewidth * 0.47) * this->dir), this->y + 10, dir, 15, bulletsprite, &Bullets, true, 62, 39, 0.5);
                //BulletParticle* bulP = new BulletParticle(this->x + ((this->scaled_spritewidth * 0.47) * this->dir), this->y + 10, &BulletParticles);
                if (bul != NULL)
                {
                    this->shootCd = 0.75;
                    //cout << "Shoot! " << bul <<endl;
                }
            }
        }
    }
public:
    float Speed;
    float range = 100;
    float x = 0;
    float x_offset = 0;
    float y = 0;
    int dir = 1;
    void Update()
    {
        if (Health <= 0)
        {
            removing = true;
            return;
        }

        if (x + x_offset - cam_x_offset > -scaled_spritewidth/2 && x + x_offset - cam_x_offset <= WIDTH + scaled_spritewidth/2)
        {
            if (sprite)
            {
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x + x_offset - cam_x_offset - (dir * (this->scaled_spritewidth / 2)), y - this->scaled_spriteheight / 2, dir * this->scaled_spritewidth, this->scaled_spriteheight, 0);
                //al_draw_circle(x, y, 5, al_map_rgb(255, 255, 255), 2);

            }
            if (walking)
            {
                if (abs(x_offset) < range)
                {
                    float change = (dir * Speed * 4) * deltaTime;

                    x_offset = clamp(x_offset + change, -range, range);
                    //cout << "offset: " << x_offset << "change: " << change << endl;

                }
                else if (abs(x_offset) >= range) {
                    //cout << "-======\n";

                    x_offset = (x_offset / (abs(x_offset))) * (abs(x_offset - (x_offset - range)) - 1);
                    dir *= -1;
                }
            }
            if (shootCd > 0) {
                shootCd -= deltaTime;
            }
            else {
                shoot();
            }
            loopBullets();
            //cout << "Shooting: " << shootCd<<endl;
        }
        if (mPlayer && *mPlayer)
        {
            float a = (*mPlayer)->x - (*mPlayer)->scaled_spritewidth / 2;
            float b = (*mPlayer)->x + (*mPlayer)->scaled_spritewidth / 2;
            float c = (*mPlayer)->y - (*mPlayer)->scaled_spriteheight / 2;
            float d = (*mPlayer)->y + (*mPlayer)->scaled_spriteheight / 2;
            //al_draw_rectangle(a, c, b, d, al_map_rgb(100, 100, 100), 3);
           // al_draw_circle(x - cam_x_offset + x_offset, y, 10, al_map_rgb(255, 0, 0), 3);
            //al_draw_circle(a - cam_x_offset, c, 10, al_map_rgb(255, 0, 0), 2);
            //al_draw_circle(b - cam_x_offset, c, 10, al_map_rgb(0, 255, 0), 2);
            //al_draw_circle(a - cam_x_offset, d, 10, al_map_rgb(0, 0, 255), 2);
            //al_draw_circle(b - cam_x_offset, d, 10, al_map_rgb(255, 255, 0), 2);
            if (x+x_offset >= a && x+x_offset <= b)
            {
                if (y >= c && y<=d)
                {
                    if (bulletsprite)
                    {
                        (*mPlayer)->removing = true;
                        GlobalAction = 3;
                    }
                    else 
                    {
                        if ((*mPlayer)->velocityY > 800)
                        {
                            removing = true;
                        }
                        else
                        {
                            (*mPlayer)->removing = true;
                            GlobalAction = 3;
                        }
                    }
                }
            }
        }
    }
    Enemy(vector<Behaviour*>* bhvs, float nX, float nY, bool walk, float speed, int direction, ALLEGRO_BITMAP* mySprite, set<Platform*>* pts, ALLEGRO_BITMAP* bulletsp, Player** plr) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->sprite = mySprite;
        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
        this->walking = walk;
        this->Speed = speed;
        this->dir = direction;
        this->platforms = pts;
        this->bulletsprite = bulletsp;
        this->mPlayer = plr;
        //cout << "BULLET ADDRESS: " << bulletsprite << endl;
        shootCd = 1;
    };
};
class FinishLine : public Behaviour
{
private:
    int spritewidth = 401;
    int spriteheight = 251;
    float spriteScale = 1;
    int scaled_spritewidth;
    int scaled_spriteheight;
    short int level;
    Player* mPlayer = NULL;
    ALLEGRO_FONT* finishFont = NULL;
    ALLEGRO_BITMAP* sprite = NULL;
    FinishScreen** fScreen = NULL;
public:
    
    float x;
    float y;
    void Update()
    {
        if (this->sprite)
        {
            al_draw_scaled_bitmap(this->sprite, 0, 0, spritewidth, spriteheight, x - cam_x_offset - scaled_spritewidth / 2, y - scaled_spriteheight / 2, scaled_spritewidth, scaled_spriteheight, 0);
        }
        if (this->mPlayer)
        {
            if (this->mPlayer->x > this->x - scaled_spritewidth / 2 && this->mPlayer->x < this->x + this->scaled_spritewidth / 2)
            {
                if (this->mPlayer->y < this->y + this->scaled_spriteheight / 2 && this->mPlayer->y > this->y - this->scaled_spriteheight / 2)
                {
                    mPlayer->removing = true;
                    if (fScreen && (*fScreen))
                    {
                        (*fScreen)->enabled = true;
                    }
                }
            }
        }
    }
    FinishLine(vector<Behaviour*>* bhvs, float nX, float nY, short int lvl, Player* plr, ALLEGRO_BITMAP* mySprite, FinishScreen** fS, ALLEGRO_FONT* finishfont) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
        this->mPlayer = plr;
        this->sprite = mySprite;
        this->finishFont = finishfont;
        this->level = lvl;
        this->fScreen = fS;
    }
};

void loopBehaviours(Player** plr, Pause* gPause)
{
    vector<Behaviour*> toDelete;
    for (vector<Behaviour*>::iterator it = Behaviours.begin(); it != Behaviours.end(); it++)
    {
        if (!gPause || !gPause->enabled || (*it) == gPause)
        {
            (*it)->Update();
        }
        if ((*it)->removing)
        {
            toDelete.push_back((*it));
        }
    }
    for (vector<Behaviour*>::iterator it = toDelete.begin(); it != toDelete.end(); it++)
    {
        if ((*it) == (*plr))
        {
            cout << "PLAYER!!\n";
            *plr = NULL;
        }
        delete(*it);
        (*it) = NULL;
    }
    toDelete.clear();
}
void ChangeScene(short int* currLevel, int scene, Menu** MenuObj, set<Platform*>* platforms, set<Enemy*>* enemies, vector<ALLEGRO_BITMAP*>* platformSprites, Player** plrAddress, vector<ALLEGRO_BITMAP*>* enemySprites, ALLEGRO_BITMAP* enemyBulletSprite, set<int>* buttons, ALLEGRO_BITMAP* floorT, float* cxo)
{
    while (Behaviours.size() > 0)
    {
        for (vector<Behaviour*>::iterator it = Behaviours.begin(); it != Behaviours.end(); it++)
        {
            delete((*it));
            break;
        }
    }
    *currLevel = scene;
    if (*currLevel > 0 && *currLevel<=4)
    {
        switch (*currLevel)
        {
        case 1:
        {
            platforms->insert(new Platform(&Behaviours, 100, 390, 2, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 900, 580-32.5, 3, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 900, 580-32.5-300, 3, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 1100, 580-32.5-400-30.5, 2, true, (*platformSprites)[1]));
            platforms->insert(new Platform(&Behaviours, 1800, 300, 2, false, (*platformSprites)[0]));

            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 2000, 530, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 2300, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 2800, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 3000, 530, true, 25, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));

            break;
        }
        case 2:
        {
            platforms->insert(new Platform(&Behaviours, 525, 390, 2, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 1150, 280, 4, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2126, 520, 1, true, (*platformSprites)[1]));
            platforms->insert(new Platform(&Behaviours, 2126, 390, 0, false, (*platformSprites)[2]));
            platforms->insert(new Platform(&Behaviours, 1931, 390, 2, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2740, 390, 2, false, (*platformSprites)[0]));
            //platforms->insert(new Platform(&Behaviours, 3200, 390, 2, false, platformSprite));

            platforms->insert(new Platform(&Behaviours, 3200, 580 - 32.5, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 3265, 580 - 32.5 - 65, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 3330, 580 - 32.5 - 65 - 65, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 3330 + 65, 580 - 32.5 - 65 - 65 - 65, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 3655, 287, 3, false, (*platformSprites)[0]));
            //platforms->insert(new Platform(&Behaviours, 3755, 580 - 65 - 65 - 65 - 65-65, 0, false, platformSprite));
            //platforms->insert(new Platform(&Behaviours, 2093.5, 485, 2, true, platformSprite2));
            
            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 1150, 200, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1520, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 2740, 310, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 3200, 580 - 32.5 - 80, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 3200 + 65 + 65, 580 - 32.5 - 80 - 65 - 65, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            
            break;
        }
        case 3:
        {
            platforms->insert(new Platform(&Behaviours, -28, 550, 0, false, (*platformSprites)[2]));
            platforms->insert(new Platform(&Behaviours, 400, 280, 2, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 530, 355, 1, true, (*platformSprites)[1]));
            platforms->insert(new Platform(&Behaviours, 1300, 390, 1, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 1930, 390, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2000, 100, 2, true, (*platformSprites)[1]));
            //platforms->insert(new Platform(&Behaviours, 2740, 390, 2, false, (*platformSprites)[0]));
            //platforms->insert(new Platform(&Behaviours, 3200, 390, 2, false, platformSprite));

            platforms->insert(new Platform(&Behaviours, 2500, 580, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2700, 300, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2695, 80, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2200, 80, 2, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2900, 200, 0, false, (*platformSprites)[0]));
            platforms->insert(new Platform(&Behaviours, 2901, 120, 2, true, (*platformSprites)[1]));

            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            /*enemies->insert(new Enemy(&Behaviours, 1150, 530, true, 25, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 700, 530, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 500, 200, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1350, 310, false, 0, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1930, 310, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1860, 530, false, 0, -1, (*enemySprites)[0], platforms, NULL, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1660, 530, true, 5, -1, (*enemySprites)[0], platforms, NULL, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1460, 530, false, 0, 1, (*enemySprites)[0], platforms, NULL, plrAddress));
            enemies->insert(new Enemy(&Behaviours, 1300, 530, false, 0, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress));
            */
            break;
        }
        }
    }
    else {
        
    }
}
int main(int argc, char* argv[])
{
    GlobalAction = 0;
    if (!al_init_primitives_addon()) { return -1; }
    ALLEGRO_BITMAP* background = NULL;
    ALLEGRO_BITMAP* floorT = NULL;
    ALLEGRO_BITMAP* platformSprite = NULL;
    ALLEGRO_BITMAP* platformSprite2 = NULL;
    ALLEGRO_BITMAP* platformSprite3 = NULL;
    ALLEGRO_BITMAP* finishSprite = NULL;
    ALLEGRO_BITMAP* playerBulletSprite = NULL;
    ALLEGRO_BITMAP* enemyBulletSprite = NULL;
    ALLEGRO_BITMAP* enemysprite = NULL;
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

    display = al_create_display(WIDTH, HEIGHT);
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
    al_install_mouse();
    al_register_event_source(kolejka, al_get_keyboard_event_source());
    al_flip_display();
    set<int> buttons;
    background = al_load_bitmap(BACKGROUND_FILE);
    if (!background)
    {
        cout<<("failed to load background bitmap!\n");
        return -5;
    }
    floorT = al_load_bitmap(FLOOR_FILE);
    if (!floorT)
    {
        cout << ("failed to load floor bitmap!\n");
        return -6;
    }
    platformSprite = al_load_bitmap(PLATFORM_FILE);
    platformSprite2 = al_load_bitmap(PLATFORM2_FILE);
    platformSprite3 = al_load_bitmap(PLATFORM3_FILE);
    if (!platformSprite || !platformSprite2 || !platformSprite3)
    {
        cout << ("failed to load PLATFORM bitmap!\n");
        return -7;
    }
    finishSprite = al_load_bitmap(FINISH_FILE);
    if (!finishSprite)
    {
        cout << ("failed to load FINISHLINE bitmap!\n");
        return -8;
    }
    playerBulletSprite = al_load_bitmap(BULLET_TEXTURE);
    enemyBulletSprite = al_load_bitmap(BULLET_TEXTURE2);
    if (!playerBulletSprite || !enemyBulletSprite)
    {
        cout << ("failed to load BULLET bitmaps!\n");
        return -8;
    }
    if (!al_init_font_addon())
    {
        return -9;
    }
    if (!al_init_ttf_addon()) { return -10; }
    ALLEGRO_FONT* uiFont = al_load_ttf_font("Fonts/PlatNomor-WyVnn.ttf", 74, 0);
    if (!uiFont) { return -11; }
    enemysprite = al_load_bitmap(ENEMY_TEXTURE);
    if (!enemysprite)
    {
        cout << ("failed to load ENEMY bitmap!\n");
        return -12;
    }
    double time = al_get_time();
    double oldTime = time;

    bool open = true;
    Player* plr = NULL;
    short int Level = -1;
    set<Platform*> platforms;
    set<Enemy*> enemies;
    FinishLine* finish = NULL;
    FinishScreen* finishScreen = NULL;
    set<Bullet*> playerBullets;
    Menu* MenuObject = NULL;
    al_clear_to_color(al_map_rgb(0, 0, 0));
    SaveSystem::Create();
    float toMenuCount = -1;
    Pause* gamePause = NULL;
    while (open)
    {

        if (Level == -1) { ChangeScene(&Level, 0, &MenuObject, NULL, NULL, NULL, NULL, NULL, NULL, &buttons, NULL, NULL);}
        switch (GlobalAction)
        {
            case -1:
            {
                open = false;
                GlobalAction = 0;
                continue;
            }
            case 1:
            {
                if (Level == 0)
                {
                    if (SaveSystem::Instance)
                    {
                        (*SaveSystem::Instance)->setStartData();
                    }
                    vector<ALLEGRO_BITMAP*> ptS = { platformSprite, platformSprite2, platformSprite3 };
                    vector<ALLEGRO_BITMAP*> eS = { enemysprite };
                    
                    ChangeScene(&Level, 1, &MenuObject, &platforms, &enemies, &ptS, &plr, &eS, enemyBulletSprite, &buttons, floorT, &cam_x_offset);
                }
            }
            case 2:
            {
                if (Level == 0)
                {
                    if (MenuObject)
                    {
                        vector<ALLEGRO_BITMAP*> ptS;
                        vector<ALLEGRO_BITMAP*> eS;
                        switch (MenuObject->LoadedLevel)
                        {
                            case 1:
                            {

                            }
                            case 2:
                            {
                                ptS.push_back(platformSprite);
                                ptS.push_back(platformSprite2);
                                ptS.push_back(platformSprite3);
                                eS.push_back(enemysprite);
                                break;
                            }
                            case 3:
                            {

                            }
                            case 4:
                            {
                                ptS.push_back(platformSprite);
                                ptS.push_back(platformSprite2);
                                ptS.push_back(platformSprite3);
                                eS.push_back(enemysprite);
                                eS.push_back(enemysprite);
                                break;
                            }
                        }
                        Level = MenuObject->LoadedLevel;
                        ChangeScene(&Level, Level, &MenuObject, &platforms, &enemies, &ptS, &plr, &eS, enemyBulletSprite, &buttons, floorT, &cam_x_offset);
                    }
                }
                break;
            }
            case 3:
            {

                if (Level) {
                    toMenuCount = 3;
                    GlobalAction = 0;
                    break;
                }
            }
            case 4:
            {

                if (Level) {
                    toMenuCount = 0.01;
                    GlobalAction = 0;
                    break;
                }
            }
            default:
            {
                break;
            }
        }

        al_wait_for_vsync();
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

        if (Level > 0 && Level <= 4)
        {
            if (!gamePause)
            {
                gamePause = new Pause(&Behaviours, &buttons, &gamePause, &deltaTime);
            }
            if (toMenuCount > 0)
            {
                toMenuCount -= deltaTime;
                if (toMenuCount <= 0 && toMenuCount != -1)
                {
                    toMenuCount = -1;
                    ChangeScene(&Level, 0, &MenuObject, NULL, NULL, NULL, NULL, NULL, NULL, &buttons, NULL, NULL);
                    finish = NULL;
                    plr = NULL;
                    continue;
                }
            }
            al_clear_to_color(al_map_rgb(38, 95, 55));

            if (plr == NULL && finish == NULL)
            {
                plr = new Player(&Behaviours, &buttons, &platforms, playerBulletSprite, &playerBullets);
                float fX = 0;
                float fY = 0;
                switch (Level)
                {
                case 1:
                {
                    fX = 3655;
                    fY = 480;
                    break;
                }
                case 2:
                {   
                    fX = 3655;
                    fY = 157.5;
                    break;
                }
                case 3:
                {
                    fX = 2230;
                    fY = -45;
                    break;
                }
                }

                finish = new FinishLine(&Behaviours, fX, fY, Level, plr, finishSprite, &finishScreen, uiFont);
                finishScreen = new FinishScreen(&Behaviours, &Level, uiFont);

            }


            al_draw_bitmap(background, 0, 0, 0);
        }
        else if (Level == 0)
        {
            if (!MenuObject)
            {
                MenuObject = new Menu(&Behaviours, &buttons, &MenuObject);
            }
        }
        //al_draw_bitmap(marian, 0, 0, 0);

    
        loopBehaviours(&plr, gamePause);
        al_flip_display();
    }
    if (SaveSystem::Instance != NULL && (*SaveSystem::Instance)) { (*SaveSystem::Instance)->Destroy(); }

    while (Behaviours.size() > 0)
    {
        for (vector<Behaviour*>::iterator it = Behaviours.begin(); it != Behaviours.end(); it++)
        {
            delete((*it));
            break;
        }
    }
    al_destroy_bitmap(background);
    al_destroy_bitmap(floorT);
    al_destroy_bitmap(platformSprite);
    al_destroy_bitmap(platformSprite2);
    al_destroy_bitmap(platformSprite3);
    al_destroy_bitmap(finishSprite);
    al_destroy_bitmap(playerBulletSprite);
    al_destroy_bitmap(enemysprite);
    al_destroy_bitmap(enemyBulletSprite);
    al_destroy_display(display);
    al_shutdown_primitives_addon();


    /* Zwalniamy pamiêæ po kolejce */
    al_destroy_event_queue(kolejka);
    return 0;
}