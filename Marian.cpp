#include <iostream>
#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <set>
#include <vector>
#include "Base.h"
#include "MenuClass.h"
#include "saveSystem.h"
#include "floorO.h"
#include "coins.h"
/**
 * @brief Lokalizacja bitmapy Background
 */
#define BACKGROUND_FILE "Picture/Background.bmp"
 /**
  * @brief Szerokosc mapy
  */
#define MAP1_width 3840
  /**
   * @brief Lokalizacja bitmapy podlogi
   */
#define FLOOR_FILE "Picture/floor.bmp"
/**
 * @brief Lokalizacja bitmapy gracza
 */
#define MARIAN_FILE "Picture/Marian.bmp"
/**
 * @brief Lokalizacja bitmapy pocisku gracza
 */
#define BULLET_TEXTURE "Picture/bullet.bmp"
/**
 * @brief Lokalizacja bitmapy pocisku przeciwnikow
 */
#define BULLET_TEXTURE2 "Picture/Buletv2.bmp"
 /**
  * @brief Lokalizacja bitmapy platformy
  */
#define PLATFORM_FILE "Picture/platform1x1.bmp"
/**
 * @brief Lokalizacja bitmapy przeciwnika
 */
#define ENEMY_TEXTURE "Picture/Enemy.bmp"
/**
 * @brief Lokalizacja bitmapy drugiego przeciwnika
 */
#define ENEMY2_TEXTURE "Picture/enemy2.bmp"
/**
 * @brief Lokalizacja bitmapy bossa
 */
#define BOSS_TEXTURE "Picture/boss.bmp"
 /**
  * @brief Lokalizacja bitmapy platformy
  */
#define PLATFORM2_FILE "Picture/platform1x1_2.bmp"
/**
 * @brief Lokalizacja bitmapy platformy
*/
#define PLATFORM3_FILE "Picture/platform1x1_3.bmp"
/**
 * @brief Lokalizacja bitmapy mety
*/
#define FINISH_FILE "Picture/Finish.bmp"
/**
 * @brief Lokalizacja bitmapy punktow
*/
#define COIN_FILE "Picture/MarioCoin.bmp"
/**
 * @brief Maksymalna wysokosc na ktora moze skoczyc gracz (-maxY)
*/
#define maxY 100

using namespace std;
/**
 * @brief Wektor (lista) zawierajaca wszystkie utworzone obiekty Behaviour Behaviour.
*/
vector<Behaviour*> Behaviours;
/**
 * @brief Czas miedzy klatkami.
*/
double deltaTime = 0;
/**
 * @brief Przesuniecie (offset) kamery w osi X.
*/
float cam_x_offset;
/**
 * @brief Funkcja przycinajaca float do podanej minimalnej i maksymalnej wartosci.
 * @param val Wartosc wejsciowa.
 * @param min Wartosc minimalna.
 * @param max Wartosc maksymalna
 * @return Wartosc pomiedzy minimalna a maksymalna dopuszczalna wartoscia.
*/
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
/**
 * @brief Klasa Platformy.
*/
class Platform : public Behaviour
{
private:
    int blocks = 0; /**< Ilosc blokow platformy. */
    ALLEGRO_BITMAP* sprite = NULL; /**< Sprite (wskaznik na bitmape) jednego bloku platformy. */
    set<Platform*>* SetPointer = NULL; /**< Wskaznik na set platform. */
public:
    float x = 0; /**< Pozycja w osi X. */
    float y = 0;/**< Pozycja w osi Y. */
    int spritewidth = 65; /**< Szerokosc sprite'u. */
    int spriteheight = 65; /**< Wysokosc sprite'u. */
    float spriteScale = 1; /**< Skala sprite'u. */
    int scaled_spritewidth; /**< Przeskalowana szerokosc sprite'u. */
    int scaled_spriteheight; /**< Przeskalowana wysokosc sprite'u. */
    bool Vertical = false; /**< Wartosc logiczna ktora mowi czy platforma jest pionowa lub pozioma. */
    /**
     * @brief Destruktor Platformy.
    */
    ~Platform()
    {
        if (SetPointer)
        {
            SetPointer->erase(this);
        }
    }
    /**
     * @brief Konstruktor Platformy.
     * @param bhvs Wskaznik na wektor behaviours.
     * @param nX Wspolrzedna X srodka platformy.
     * @param nY Wspolrzedna Y srodka platformy.
     * @param blocksAroundMiddle Ilosc blokow w kazda ze stron.
     * @param vertical Okreslenie czy platforma jest pionowa czy pozioma.
     * @param mySprite Wskaznik na bitmape (grafike) platformy.
     * @param pointer Wskaznik na set platform.
    */
    Platform(vector<Behaviour*>* bhvs, float nX, float nY, int blocksAroundMiddle, bool vertical, ALLEGRO_BITMAP* mySprite, set<Platform*>* pointer) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->sprite = mySprite;
        this->blocks = blocksAroundMiddle;
        this->Vertical = vertical;
        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
        this->SetPointer = pointer;
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
    /**
     * @brief Funkcja sprawdzajaca czy dane koordynaty sa w platformie.
     * @param cX Wskaznik na wspolrzedna X.
     * @param cY Wskaznik na wspolrzedna Y.
     * @param cW Szerokosc kolidujacego obiektu.
     * @param cH Wysokosc kolidujacego obiektu.
     * @return Wartosc logiczna - czy cos jest w platformie.
    */
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
/**
 * @brief Klasa kuli.
*/
class Bullet
{
private:
    set<Bullet*>* mySet;
    public:
        float x; /**< Pozycja w osi X. */
        float y;/**< Pozycja w osi Y. */
        float Speed;/**< Predkosc kuli. */
        int spritewidth; /**< Szerokosc sprite'u. */
        int spriteheight;/**< Wysokosc sprite'u. */
        float spriteScale;/**< Skala sprite'u. */
        int scaled_spritewidth; /**< Przeskalowana szerokosc sprite'u. */
        int scaled_spriteheight; /**< Przeskalowana wysokosc sprite'u. */
        int direction;/**< Kierunek w ktorym leci kula. */
        float lifetime = 2;/**< Czas do automatycznego zniszczenia kuli. */
        bool hurtPlayer = false; /**< Czy kula ma ranic gracza. */
        ALLEGRO_BITMAP* sprite = NULL; /**< Wskaznik na bitmape (grafike) kuli. */
        /**
         * @brief Konstruktor kuli.
         * @param nX Wspolrzedna X kuli.
         * @param nY Wspolrzedna Y kuli.
         * @param dir kierunek kuli.
         * @param speed Predkosc kuli.
         * @param spriteFile Wskaznik na bitmape (grafike) kuli.
         * @param mset Wskaznik na set kul.
         * @param hPlayer Czy kula ma ranic gracza.
         * @param sizeX Szerokosc.
         * @param sizeY Wysokosc.
         * @param scale Skala.
        */
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
        /**
         * @brief Destruktor kuli.
        */
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
        /**
         * @brief Funkcja renderujaca (wyswietlajaca) kule.
        */
        void renderMe()
        {
            if (this->sprite) {
                if (this->direction == 0) { delete(this); return; }
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x - cam_x_offset - (this->direction * this->scaled_spritewidth / 2), this->y - this->scaled_spriteheight / 2, this->direction * this->scaled_spritewidth, this->scaled_spriteheight, 0);
            }
        }
        /**
         * @brief Funkcja aktualizujaca wartosci kuli.
        */
        void UpdateBullet()
        {
            this->x = this->x + ((this->direction * Speed * 100) * deltaTime);
            this->y = this->y + (9.81 * deltaTime);
            lifetime -= deltaTime;
            renderMe();
        }
};
/**
 * @brief Klasa efektu strzelania (czasteczek)
*/
class BulletParticle
{
private:
    set<BulletParticle*>* mySet;  /**< Wskaznik na set czasteczek. */
    float th = 0.5;  /**< Czas po ktorym czasteczka zaczyna malec. */
    bool reachedth = false;  /**< Czy osiagnieto th. */
public:
    float x; /**< Pozycja w osi X. */
    float y; /**< Pozycja w osi Y. */
    float timeLeft = 0.0; /**< Czas czasteczki. */
    /**
     * @brief Konstruktor czasteczki.
     * @param nX Pozycja w osi X.
     * @param nY Pozycja w osi Y.
     * @param mset Wskaznik na set czasteczek.
    */
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
    /**
     * @brief Destruktor czasteczki.
    */
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
    }
    /**
     * @brief Funkcja wyswietlajaca czasteczke.
    */
    void renderMe()
    {
        if (this->timeLeft>0) {
            al_draw_filled_circle(this->x - cam_x_offset, this->y, timeLeft * 30, al_map_rgba(255, 230, 10, (int)(timeLeft * 300)));
            this->x -= 30 * deltaTime;
        }
    }
    /**
     * @brief Funkcja aktualizujaca obiekt.
    */
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
/**
 * @brief Klasa gracza.
*/
class Player : public Behaviour
{
private:
    float moveForceX = 0; /**< Sila poruszania sie w osi X. */
    float shootCd = 0; /**< Cooldown strzelania. */
    float shot = 0; /**< Wartosc zwiekszana przy strzelaniu (Odrzut). */
    float jumpCd = 0; /**< Cooldown skoku. */
    ALLEGRO_BITMAP* bulletsprite = NULL; /**< Wskaznik na bitmape (grafike) kul gracza. */
    ALLEGRO_FONT* debugFont = al_load_ttf_font("Fonts/oldtimer.regular.ttf", 30, 0); /**< Czcionka. */
    ALLEGRO_FONT* upFont = al_load_ttf_font("Fonts/Qaz-Regular.ttf", 22, 0);/**< Czcionka. */
    /**
     * @brief Funkcja wyswietlajaca gracza.
    */
    void renderMe()
    {
        if (this->marian) {
            if (lastMoveForceX == 0) { lastMoveForceX = 1; }
            al_draw_scaled_bitmap(this->marian, 0, 0, spritewidth, spriteheight, x - cam_x_offset -(lastMoveForceX * scaled_spritewidth/2), y-scaled_spriteheight/2, lastMoveForceX * scaled_spritewidth, scaled_spriteheight, 0);
            //al_draw_filled_circle(this->x, this->y, 10, al_map_rgb(255, 100, 100));
            //al_draw_bitmap(this->marian, WIDTH / 2, HEIGHT / 2, 0);
        }
    }
    set<int>* pressedKeys = NULL; /**< Wskaznik na set wcisnietych przyciskow. */
    set<Platform*>* platforms = NULL;  /**< Wskaznik na set platform. */
    /**
     * @brief Metoda skakania gracza.
    */
    void Jump()
    {
        if (isGrounded && jumpCd<=0)
        {
            this->velocityY -= jumpForce*15;
            jumpCd += 0.2;
        }
    }
    /**
     * @brief Metoda strzelania.
    */
    void Shoot()
    {
        if (this->shootCd <= 0)
        {
            if (this->lastMoveForceX == 0) { this->lastMoveForceX = 1; }
            Bullet* bul = new Bullet(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y + 10, lastMoveForceX, 15, bulletsprite, Bullets,false, 48, 17, 0.4);
            BulletParticle* bulP = new BulletParticle(this->x + ((this->scaled_spritewidth * 0.47) * this->lastMoveForceX), this->y + 10, &BulletParticles);
            if (bul != NULL)
            {
                this->shootCd = clamp(0.13 - cdUpgrade* 0.01, 0.03, 0.13);
                shot += 5;
            }
        }
    }
    /**
     * @brief Metoda petli (co klatke) kul wystrzelonych przez gracza.
    */
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
    /**
     * @brief Metoda sprawdzajaca czy na danych koordynatach jest platforma.
     * @param cx Pozycja w osi X.
     * @param cy Pozycja w osi Y.
     * @return Wartosc logiczna - true/false.
    */
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
    /**
     * @brief Metoda sprawdzajaca czy gracz stoi na podlozu.
     * @param cx Wskaznik na pozycje w osi X.
     * @param cy Wskaznik na pozycje w osi Y.
     * @return Wartosc logiczna - true/false.
    */
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
    bool* died; /**< Czy gracz umarl. */
    float upcd = 0;  /**< Cooldown ulepszen. */
public:
    float x; /**< Pozycja na osi X. */
    float y;/**< Pozycja na osi Y. */
    float Speed = 6.0;/**< Predkosc poruszania sie gracza. */
    int spritewidth; /**< Szerokosc sprite'u. */
    int spriteheight;/**< Wysokosc sprite'u. */
    float spriteScale;/**< Skala sprite'u. */
    int scaled_spritewidth; /**< Przeskalowana szerokosc sprite'u. */
    int scaled_spriteheight; /**< Przeskalowana wysokosc sprite'u. */
    bool isGrounded = false; /**< Czy gracz stoi na podlozu. */
    float jumpForce = 60; /**< Sila skoku. */
    float lastMoveForceX = 0;/**< Ostatni kierunek poruszania sie gracza. */
    int Health = 100; /**< Zdrowie gracza. */
    ALLEGRO_BITMAP* marian = NULL;/**< Wskaznik na bitmape (grafike) gracza. */
    set<Bullet*>* Bullets; /**< Wskaznik na set wystrzelonych kul. */
    set<BulletParticle*> BulletParticles; /**< Wskaznik na czasteczki (efekt strzelania). */
    set<coin*>* coinsR; /**< Wskaznik na set coinow (punktow). */
    float velocityY = 0; /**< Przyspieszenie gracza w osi Y. */
    int* coinsTaken;/**< Wskaznik na zmienna zebranych coinow (punktow) gracza. */
    int cdUpgrade;/**< Ulepszenie szybkostrzelnosci gracza. */
    /**
     * @brief Metoda wywolywana co klatke gry.
     * 
     * Przeprowadza ona wszelkiego rodzaju operacje, jest to glowna petla gry dla gracza. Obsluguje ona np. poruszanie sie oraz strzelanie.
    */
    void Update()
    {
        if (Health <= 0)
        {
            *died = true;
            //GlobalAction = 3;
            removing = true;
            return;
        }
        al_draw_textf(debugFont, al_map_rgb(80, 0, 0), 20, HEIGHT - 50, 0, "hp: %d", Health);
        al_draw_textf(debugFont, al_map_rgb(60, 200, 60), WIDTH-20, HEIGHT - 50, ALLEGRO_ALIGN_RIGHT, "Marian-Coins: %d", *coinsTaken);
        bool lastGrounded = isGrounded;
        bool lowground = !(this->y + this->scaled_spriteheight / 2 < 580);
        isGrounded = checkGrounded(&(this->x), &(this->y)) || lowground;
        renderMe();
        moveForceX = 0;
        float gravForce = 0;
        if (upcd > 0)
        {
            upcd -= deltaTime;
        }
        if (*coinsTaken >= 3 && upcd <=0)
        {
            al_draw_filled_rectangle(WIDTH / 2 - 225, HEIGHT - 130, WIDTH / 2 + 225, HEIGHT - 30, al_map_rgba_f(0, 0, 0, 0.6));
            al_draw_text(debugFont, al_map_rgb_f(1, 1, 1), WIDTH / 2, HEIGHT - 125, ALLEGRO_ALIGN_CENTER, "Ulepsz");
            al_draw_text(upFont, al_map_rgb_f(1, 1, 1), WIDTH / 2 - 200, HEIGHT - 70, ALLEGRO_ALIGN_LEFT, "(Z) SPEED");
            al_draw_text(upFont, al_map_rgb_f(1, 1, 1), WIDTH / 2 + 200, HEIGHT - 70, ALLEGRO_ALIGN_RIGHT, "GUN SPEED (X)");
            for (set<int>::iterator it = (*pressedKeys).begin(); it != (*pressedKeys).end(); it++)
            {
                if ((*it) == ALLEGRO_KEY_X)
                {
                    upcd = 1;
                    cdUpgrade++;
                    *coinsTaken -= 3;
                    break;
                }
                else if ((*it) == ALLEGRO_KEY_Z)
                {
                    upcd = 1;
                    Speed+=0.5;
                    *coinsTaken -= 3;
                    break;
                }
            }
        }
        if (jumpCd > 0) {
            jumpCd -= deltaTime;
        }
        if (y + scaled_spriteheight/2 < 580)
        {
            gravForce = 9.81;
        }
        bool fastJump = false;
        bool extendJump = false;
        for (set<coin*>::iterator it = (*coinsR).begin(); it != (*coinsR).end(); it++)
        {
            (*it) -> renderMe(cam_x_offset);
            bool collided = (*it)->Collides((float)x, (float)y);
            if (collided)
            {
                coin* c = (*it);
                coinsR->erase(c);
                delete(c);
                (*coinsTaken)++;
                break;
            }
        }
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
        if (this->shot) {
            newX = this->x - (this->lastMoveForceX * 10 * this->shot) * deltaTime;
            if (!platformBlock(newX, this->y-1))
            {
                this->x = clamp(newX, this->scaled_spritewidth / 2, MAP1_width - this->scaled_spritewidth / 2);
            }

        }
        if (!isGrounded)
        {
            float change = fastJump ? 3 : extendJump ? 0.6 : 1;
            velocityY += (gravForce * 250 * change) * deltaTime;

        }
        if (isGrounded && !lastGrounded)
        {
            velocityY = 0;
        }
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
    }
    /**
     * @brief Metoda Start.
     *
     * Przeprowadza (domyslnie jednorazowo) pierwsze operacje po konstruktorze.
    */
    void Start()
    {
        this->marian = al_load_bitmap(MARIAN_FILE);
        if (!this->marian)
        {
            cout << ("failed to load Marian bitmap!\n");
        }
        scaled_spritewidth = ceil(spritewidth * spriteScale);
        scaled_spriteheight = ceil(spriteheight * spriteScale);
        cout << "Stworzono mnie: rozmiary? " << scaled_spriteheight << " " << scaled_spritewidth << endl;
    }
    /**
     * @brief Konstruktor Gracza.
     *
     * @param bhvs Wskaznik na vector zawierajacy wszystkie Behaviour (aby dodac do niego gracza).
     * @param keys Wskaznik na set wcisnietych klawiszy.
     * @param pts Wskaznik na set zawierajacy wszystkie platformy.
     * @param bulletSpr Wskaznik na bitmape (grafike) kul gracza.
     * @param blts Wskaznik na set zawierajacy kule wystrzelone przez gracza.
     * @param dead Wskaznik na wartosc logiczna mowiaca czy gracz zyje czy nie.
     * @param coinsad Wskaznik na set zawierajacy wszystkie punkty (coinsy).
     * @param tCoins Wskaznik (int*) na zebrane coiny(punkty) gracza.
    */
    Player(vector<Behaviour*>* bhvs, set<int>* keys, set<Platform*>* pts, ALLEGRO_BITMAP* bulletSpr, set<Bullet*>* blts, bool* dead, set<coin*>* coinsad, int* tCoins) :Behaviour(bhvs)
    { 
        platforms = pts;
        this->spritewidth = 356;
        this->spriteheight = 472; 
        this->x = 120; 
        this-> spriteScale = 0.28;
        this->y = 500;
        this->pressedKeys = keys;
        this->bulletsprite = bulletSpr;
        this->Bullets = blts;
        this->died = dead;
        this->coinsR = coinsad;
        this->coinsTaken = tCoins;
        if (SaveSystem::Instance && *SaveSystem::Instance)
        {
            cdUpgrade = (*SaveSystem::Instance)->LoadShootSpeed();
            Speed = (*SaveSystem::Instance)->LoadSpeed();
        }
        this->Start();
    };
    /**
     * @brief Destruktor Gracza.
    */
    ~Player() {
        if (marian != NULL) {
            al_destroy_bitmap(marian);
            cout << "Destroying MARIAN Bitmap" << endl;
            marian = NULL;
        }
        if (debugFont) {

            al_destroy_font(debugFont);
        }
        if (upFont) {
            al_destroy_font(upFont);
        }
        Behaviour::~Behaviour();
        
    }
};
/**
 * @brief Klasa ekranu koncowego.
*/
class FinishScreen : public Behaviour
{
private:
    short int* level = 0; /**< Wskaznik na poziom. */
    ALLEGRO_FONT* font = NULL; /**< Wskaznik na czcionke */
    bool wasEnabled = false; /**< Czy byl juz aktywowany. */
    Player* myPlayer; /**< Wskaznik na gracza. */
public:
    bool enabled;/**< Czy aktywny. */
    /**
     * @brief Metoda Update, wywolywana co klatke gry.
    */
    void Update()
    {

        if ((*level) > 0 && font && enabled)
        {
            if (!wasEnabled)
            {
                wasEnabled = true;
                if (SaveSystem::Instance && (*SaveSystem::Instance))
                {
                    char str[32] = "";
                    int nextlevel = clamp((*level + 1), 1, 4);
                    _itoa_s(nextlevel, str, LINE_LENGTH, 10);
                    (*SaveSystem::Instance)->printAtLine(1, str);
                    _itoa_s(*(myPlayer->coinsTaken), str, LINE_LENGTH, 10);
                    (*SaveSystem::Instance)->printAtLine(2, str);

                    _itoa_s(myPlayer->cdUpgrade, str, LINE_LENGTH, 10);
                    (*SaveSystem::Instance)->printAtLine(3, str);

                    _gcvt_s(str, LINE_LENGTH, myPlayer->Speed, 2);
                    (*SaveSystem::Instance)->printAtLine(4, str);
                    GlobalAction = 3;
                }
                else
                {
                    SaveSystem::Create();
                    wasEnabled = false;
                }
            }
            al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba(0, 0, 0, 230));
            al_draw_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgba(0, 255, 0, 240), 10);
            if (*level < 4) {
                al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Level %d ukonczony!", *level);
            }
            else {
                al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Brawo! Gra ukonczona!");
            }
        }
    }
    /**
     * @brief Konstruktor.
     * @param bhvs Wskaznik na wektor zawierajacy wszystkie Behaviour(s). 
     * @param lv Wskaznik na poziom.
     * @param myfont Wskaznik na czcionke.
     * @param plr Wskaznik na gracza.
    */
    FinishScreen(vector<Behaviour*>* bhvs, short int* lv, ALLEGRO_FONT* myfont, Player* plr) :Behaviour(bhvs)
    {
        this->level = lv;
        this->font = myfont;
        this->enabled = false;
        this->myPlayer = plr;
    }
};
/**
 * @brief Klasa Mety.
*/
class FinishLine : public Behaviour
{
private:
    int spritewidth = 401;/**< Szerokosc sprite'u. */
    int spriteheight = 251;/**< Wysokosc sprite'u. */
    float spriteScale = 1;/**< Skala sprite'u. */
    int scaled_spritewidth;/**< Przeskalowana szerokosc sprite'u. */
    int scaled_spriteheight;/**< Przeskalowana wysokosc sprite'u. */
    short int level;/**< Poziom. */
    Player* mPlayer = NULL;/**< Wskaznik na gracza. */
    ALLEGRO_FONT* finishFont = NULL; /**< Wskaznik na czcionke. */
    ALLEGRO_BITMAP* sprite = NULL;/**< Wskaznik na bitmape (grafike) mety. */
    FinishScreen** fScreen = NULL; /**< Podwojny wskaznik na obiekt ekranu koncowego.*/
public:
    bool hidden = false; /**< Czy meta jest ukryta. */
    float x;/**< Pozycja na osi X. */
    float y;/**< Pozycja na osi Y. */
    /**
     * @brief Metoda Update, wywolywana co klatke gry.
    */
    void Update()
    {
        if (!hidden) {
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
    }
    /**
     * @brief Konstruktor mety.
     * @param bhvs Wskaznik na set zawierajacy wszystkie klasy typu Behaviour.
     * @param nX Pozycja na osi X.
     * @param nY Pozycja na osi Y.
     * @param lvl Poziom.
     * @param plr Wskaznik na gracza.
     * @param mySprite Wskaznik na bitmape (grafike) mety.
     * @param fS Podwojny wskaznik na ekran koncowy.
     * @param finishfont Wskaznik na czcionke ekranu koncowego.
     * @param hide Czy meta jest ukryta.
    */
    FinishLine(vector<Behaviour*>* bhvs, float nX, float nY, short int lvl, Player* plr, ALLEGRO_BITMAP* mySprite, FinishScreen** fS, ALLEGRO_FONT* finishfont, bool hide) :Behaviour(bhvs)
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
        this->hidden = hide;
    }
};
/**
 * @brief Klasa przeciwnika.
*/
class Enemy : public Behaviour
{
private:
    ALLEGRO_BITMAP* sprite = NULL;/**< Wskaznik na bitmape. */
    int spritewidth = 356;/**< Szerokosc sprite'u. */
    int spriteheight = 472;/**< Wysokosc sprite'u. */
    float spriteScale = 0.21;/**< Skala sprite'u. */
    int scaled_spritewidth;/**< Przeskalowana szerokosc sprite'u. */
    int scaled_spriteheight;/**< Przeskalowana wysokosc sprite'u. */
    bool walking = false;/**< Czy porusza przeciwnik sie porusza. */
    float shootCd = 0;/**< Cooldown strzelania przeciwnika. */
    ALLEGRO_BITMAP* bulletsprite = NULL; /**< Wskaznik na bitmape kuli przeciwnika. */
    set<Bullet*> Bullets; /**< Set zawierajacy kule przeciwnika. */
    set<Platform*>* platforms;/**< Wskaznik na set zawierajacy platformy. */
    Player** mPlayer = NULL; /**< Podwojny wskaznik na gracza. */
    int Health = 50;/**< Zdrowie przeciwnika. */
    bool isBOSS = false; /**< Czy przeciwnik jest bossem. */
    bool* didPlayerDie; /**< Wskaznik na wartosc logiczna czy gracz umarl. */
    float walkcd = 0; /**< Cooldown na chodzenie przeciwnika. */
    bool playerWasFar = false;/**< Czy gracz byl z dala od przeciwnika. */
    bool stupid = false; /**< Czy przeciwnik chodzi od lewej do prawej, czy chodzi w kierunku gracza. */
    /**
     * @brief Metoda wywolywana co klatke gry dzialajaca na kazdej wystrzelonej kuli.
    */
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
                        if (isBOSS) {
                            (*mPlayer)->Health -= 10;

                        }
                        else {
                            (*mPlayer)->Health -= 25;
                        }
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
    /**
     * @brief Strzelanie przeciwnika.
    */
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
                    if (isBOSS)
                    {
                        this->shootCd = 0.3;
                    }
                    else
                    {
                        this->shootCd = 0.75;

                    }
                }
            }
        }
    }
public:
    float Speed;/**< Predkosc przeciwnika. */
    float range = 100; /**< Zasieg przeciwnika. */
    float x = 0; /**< Pozycja na osi X. */
    float x_offset = 0; /**< Offset przeciwnika na osi X. */
    float y = 0;/**< Pozycja na osi Y. */
    int dir = 1;/**< Kierunek poruszania sie przeciwnika. */
    FinishLine** finishl; /**< Podwojny wskaznik na obiekt mety. */
    /**
     * @brief Metoda wywolywana co klatke gry przeprowadzajace operacje na przeciwniku.
    */
    void Update()
    {
        if (Health <= 0)
        {
            if (isBOSS && finishl && *finishl)
            {
                (*finishl)->hidden = false;
            }
            removing = true;
            return;
        }
        if (walkcd > 0) { walkcd -= deltaTime; }
        if (walking && stupid)
        {
            if (abs(x_offset) < range)
            {
                float change = (dir * Speed * 4) * deltaTime;
                x_offset = clamp(x_offset + change, -range, range);
            }
            else if (abs(x_offset) >= range) {
                x_offset = (x_offset / (abs(x_offset))) * (abs(x_offset - (x_offset - range)) - 1);
                dir *= -1;
            }
        }
        else if (walking && *mPlayer && !stupid)
        {
            if (abs(x - (*mPlayer)->x) >= 70 && walkcd<=0)
            {
                playerWasFar = true;
                if (x > (*mPlayer)->x) {
                    dir = -1;
                }
                else {
                    dir = 1;

                }
                float change = (dir * Speed * 4) * deltaTime;
                x += change;
            }
            else
            {

                if (walkcd <= 0 && playerWasFar) {
                    walkcd = 4;
                }
            }
        }
        if (x + x_offset - cam_x_offset >= -scaled_spritewidth/2 && x + x_offset - cam_x_offset <= WIDTH + scaled_spritewidth/2)
        {
            if (isBOSS)
            {
                float x1 = x + x_offset - cam_x_offset - scaled_spritewidth;
                float x2 = x - cam_x_offset + x_offset + scaled_spritewidth;
                float percentage = (float)Health / 5000;
                float dx = x2 - x1;
                dx *= percentage;
                al_draw_filled_rectangle(x1, y - scaled_spriteheight + scaled_spriteheight / 5, x1 + dx, y - scaled_spriteheight + scaled_spriteheight / 3, al_map_rgba_f(1, 0, 0, 0.9));
                al_draw_rounded_rectangle(x1, y - scaled_spriteheight + scaled_spriteheight / 5, x2, y - scaled_spriteheight + scaled_spriteheight / 3, 2, 2, al_map_rgba_f(0, 0, 0, 1), 4);
            }
            if (sprite)
            {
                al_draw_scaled_bitmap(this->sprite, 0, 0, this->spritewidth, this->spriteheight, this->x + x_offset - cam_x_offset - (dir * (this->scaled_spritewidth / 2)), y - this->scaled_spriteheight / 2, dir * this->scaled_spritewidth, this->scaled_spriteheight, 0);
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
            if (x+x_offset >= a && x+x_offset <= b)
            {
                if (y >= c && y<=d)
                {
                    if (bulletsprite)
                    {
                        *didPlayerDie = true;
                        (*mPlayer)->removing = true;
                    }
                    else 
                    {
                        if ((*mPlayer)->velocityY > 800)
                        {
                            (*mPlayer)->velocityY -= 2000;
                            removing = true;
                        }
                        else
                        {
                            *didPlayerDie = true;
                            (*mPlayer)->removing = true;
                        }
                    }
                }
            }
        }
    }
    /**
     * @brief Konstruktor przeciwnika.
     * @param bhvs Wskaznik na set zawierajacy wszystkie obiekty typu Behaviour.
     * @param nX Pozycja na osi X.
     * @param nY Pozycja na osi Y.
     * @param walk Czy przeciwnik sie porusza.
     * @param speed Jesli przeciwnik sie porusza, z jaka predkoscia.
     * @param direction W ktora strone skierowany jest przeciwnik.
     * @param mySprite Wskaznik na bitmape (grafike) przeciwnika.
     * @param pts Wskaznik set zawierajacy wszystkie platformy.
     * @param bulletsp Wskaznik na bitmape (grafike) kuli przeciwnika.
     * @param plr Podwojny wskaznik na gracza.
     * @param isBoss Czy przeciwnik jest bossem.
     * @param killedPlayer Wskaznik na wartosc logiczna - czy gracz zostal zabity.
     * @param Stupid Czy przeciwnik chodzi tylko od lewej do prawej, czy podaza za graczem.
     * @param fL Podwojny wskaznik na mete.
    */
    Enemy(vector<Behaviour*>* bhvs, float nX, float nY, bool walk, float speed, int direction, ALLEGRO_BITMAP* mySprite, set<Platform*>* pts, ALLEGRO_BITMAP* bulletsp, Player** plr, bool isBoss, bool* killedPlayer, bool Stupid, FinishLine** fL) :Behaviour(bhvs)
    {
        this->x = nX;
        this->y = nY;
        this->sprite = mySprite;
        this->isBOSS = isBoss;
        this->finishl = fL;
        if (isBOSS)
        {
            this->spriteScale = this->spriteScale * 2;
            Health = 5000;
        }
        this->scaled_spritewidth = this->spritewidth * this->spriteScale;
        this->scaled_spriteheight = this->spriteheight * this->spriteScale;
        this->walking = walk;
        this->Speed = speed;
        this->dir = direction;
        this->platforms = pts;
        this->bulletsprite = bulletsp;
        this->mPlayer = plr;
        this->didPlayerDie = killedPlayer;
        this->stupid = Stupid;
        //cout << "BULLET ADDRESS: " << bulletsprite << endl;
        shootCd = 1;
    };
};

/**
 * @brief Petla (funkcja) wywolywana co klatke ktora odpowiada za iterowanie przez wszystkie obiekty typu Behaviour.
 * @param plr Podwojny wskaznik na gracza.
 * @param gPause Wskaznik na pauze.
*/
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
/**
 * @brief Set zawierajacy wszystie istniejace coiny (punkty) na mapie.
*/
set<coin*> coins;
/**
 * @brief Funkcja czyszczaca i usuwajaca wszystkie istniejace coiny (punkty).
*/
void ClearCoins()
{
    for (set<coin*>::iterator it = coins.begin(); it != coins.end(); it++)
    {
        coin* c = (*it);
        delete c;
    }
    coins.clear();
}
/**
 * @brief Funkcja czyszczaca i usuwajaca wszystkie istniejace platformy.
*/
void ClearPlatforms(set<Platform*>* platforms)
{
    for (set<Platform*>::iterator it = platforms->begin(); it != platforms->end(); it++)
    {
        Platform* c = (*it);
        delete c;
    }
    platforms->clear();
}
/**
 * @brief Funkcja zmieniajaca aktualna scene (poziom/level).
 * @param currLevel Wskaznik na aktualny poziom.
 * @param scene Scena/Poziom docelowy.
 * @param MenuObj Podwojny wskaznik na obiekt klasy Menu.
 * @param platforms Wskaznik na set zawierajacy wszystkie platformy.
 * @param enemies Wskaznik na set zawierajacy wszystkich przeciwnikow.
 * @param platformSprites Wskaznik na wektor zawierajacy bitmapy (grafiki) platform.
 * @param plrAddress Podwojny wskaznik na gracza
 * @param enemySprites Wskaznik na wektor zawierajacy bitmapy (grafiki) przeciwnikow.
 * @param enemyBulletSprite Wskaznik na bitmape kul przeciwnikow.
 * @param buttons Wskaznik na set zawierajacy aktualnie wcisniete przyciski.
 * @param floorT Wskaznik na bitmape (grafike) podlogi.
 * @param cxo Wskaznik na wartosc offsetu (przesuniecia) kamery na osi X.
 * @param deadplayer Wskaznik na wartosc logiczna - czy gracz umarl.
 * @param fL Podwojny wskaznik na obiekt mety.
 * @param coinBitmap Wskaznik na bitmape (grafike) coina (punktu)
*/
void ChangeScene(short int* currLevel, int scene, Menu** MenuObj, set<Platform*>* platforms, set<Enemy*>* enemies, vector<ALLEGRO_BITMAP*>* platformSprites, Player** plrAddress, vector<ALLEGRO_BITMAP*>* enemySprites, ALLEGRO_BITMAP* enemyBulletSprite, set<int>* buttons, ALLEGRO_BITMAP* floorT, float* cxo, bool* deadplayer, FinishLine** fL, ALLEGRO_BITMAP* coinBitmap)
{
    *deadplayer = false;
    while (Behaviours.size() > 0)
    {
        for (vector<Behaviour*>::iterator it = Behaviours.begin(); it != Behaviours.end(); it++)
        {
            delete((*it));
            break;
        }
    }

    ClearCoins();
    *currLevel = scene;
    if (*currLevel > 0 && *currLevel<=4)
    {
        switch (*currLevel)
        {
        case 1:
        {
            platforms->insert(new Platform(&Behaviours, 100, 390, 2, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 900, 580-32.5, 3, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 900, 580-32.5-300, 3, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 1100, 580-32.5-400-30.5, 2, true, (*platformSprites)[1], platforms));
            platforms->insert(new Platform(&Behaviours, 1800, 330, 2, false, (*platformSprites)[0], platforms));

            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 2000, 530, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress,false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 2300, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 2800, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 3000, 530, true, 25, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));

            coins.insert(new coin(1000, 170, coinBitmap));
            coins.insert(new coin(1800, 210, coinBitmap));

            break;
        }
        case 2:
        {
            platforms->insert(new Platform(&Behaviours, 525, 390, 2, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 1150, 280, 4, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2126, 520, 1, true, (*platformSprites)[1], platforms));
            platforms->insert(new Platform(&Behaviours, 2126, 390, 0, false, (*platformSprites)[2], platforms));
            platforms->insert(new Platform(&Behaviours, 1931, 390, 2, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2740, 390, 2, false, (*platformSprites)[0], platforms));
            //platforms->insert(new Platform(&Behaviours, 3200, 390, 2, false, platformSprite));

            platforms->insert(new Platform(&Behaviours, 3200, 580 - 32.5, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 3265, 580 - 32.5 - 65, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 3330, 580 - 32.5 - 65 - 65, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 3330 + 65, 580 - 32.5 - 65 - 65 - 65, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 3655, 287, 3, false, (*platformSprites)[0], platforms));
            //platforms->insert(new Platform(&Behaviours, 3755, 580 - 65 - 65 - 65 - 65-65, 0, false, platformSprite));
            //platforms->insert(new Platform(&Behaviours, 2093.5, 485, 2, true, platformSprite2));
            
            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 1150, 200, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1520, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 2740, 310, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 3200, 580 - 32.5 - 80, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 3200 + 65 + 65, 580 - 32.5 - 80 - 65 - 65, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            
            coins.insert(new coin(525, 170, coinBitmap));
            coins.insert(new coin(2126, 300, coinBitmap));
            coins.insert(new coin(1260, 200, coinBitmap));
            coins.insert(new coin(3000, 410, coinBitmap));



            break;
        }
        case 3:
        {
            platforms->insert(new Platform(&Behaviours, -28, 550, 0, false, (*platformSprites)[2], platforms));
            platforms->insert(new Platform(&Behaviours, 400, 280, 2, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 530, 355, 1, true, (*platformSprites)[1], platforms));
            platforms->insert(new Platform(&Behaviours, 1300, 390, 1, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 1930, 390, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2000, 100, 2, true, (*platformSprites)[1], platforms));
            //platforms->insert(new Platform(&Behaviours, 2740, 390, 2, false, (*platformSprites)[0]));
            //platforms->insert(new Platform(&Behaviours, 3200, 390, 2, false, platformSprite));

            platforms->insert(new Platform(&Behaviours, 2500, 580, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2700, 300, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2695, 80, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2200, 80, 2, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2900, 200, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 2901, 120, 2, true, (*platformSprites)[1], platforms));

            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 1150, 530, true, 25, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 700, 530, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 500, 200, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1350, 310, false, 0, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1930, 310, false, 0, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1860, 530, false, 0, -1, (*enemySprites)[1], platforms, NULL, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1660, 530, true, 5, -1, (*enemySprites)[1], platforms, NULL, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1460, 530, false, 0, 1, (*enemySprites)[1], platforms, NULL, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 1300, 530, false, 0, 1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            
            
            coins.insert(new coin(390, 170, coinBitmap));
            coins.insert(new coin(1000, 450, coinBitmap));
            coins.insert(new coin(1450, 300, coinBitmap));
            coins.insert(new coin(1500, 480, coinBitmap));
            coins.insert(new coin(3500, 480, coinBitmap));

            break;
        }
        case 4:
        {
           
            platforms->insert(new Platform(&Behaviours, 1700, 360, 0, false, (*platformSprites)[0], platforms));
            platforms->insert(new Platform(&Behaviours, 3000, 360, 0, false, (*platformSprites)[0], platforms));

            FloorObject* floor = new FloorObject(&Behaviours, floorT, cxo);

            enemies->insert(new Enemy(&Behaviours, 3400, 490, true, 70, -1, (*enemySprites)[2], platforms, enemyBulletSprite, plrAddress, true, deadplayer, false, fL));
            enemies->insert(new Enemy(&Behaviours, 600, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, false, fL));
            enemies->insert(new Enemy(&Behaviours, 2300, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, false, fL));
            enemies->insert(new Enemy(&Behaviours, 1500, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, false, fL));
            enemies->insert(new Enemy(&Behaviours, 1500, 530, true, 25, -1, (*enemySprites)[0], platforms, enemyBulletSprite, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 3000, 530, true, 25, -1, (*enemySprites)[1], platforms, NULL, plrAddress, false, deadplayer, true, fL));
            enemies->insert(new Enemy(&Behaviours, 3200, 530, true, 25, -1, (*enemySprites)[1], platforms, NULL, plrAddress, false, deadplayer, false, fL));

            coins.insert(new coin(200, 480, coinBitmap));
            coins.insert(new coin(1700, 200, coinBitmap));
            coins.insert(new coin(1900, 300, coinBitmap));
            coins.insert(new coin(3300, 480, coinBitmap));

            break;
        }
        }
    }
    else {
        
    }
}
/**
 * @brief Funkcja glowna (MAIN).
*/
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
    ALLEGRO_BITMAP* enemysprite2 = NULL;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_BITMAP* coinBitmap = NULL;

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
        return -2;
    }

    display = al_create_display(WIDTH, HEIGHT);
    if (display == NULL)
    {
        fprintf(stderr, "Failed to create display!\n");
        return -3;
    }


    /* Skoro mamy kolejk� i display, rejestrujemy display jako �r�d�o event�w */
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
    coinBitmap = al_load_bitmap(COIN_FILE);
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
    enemysprite2 = al_load_bitmap(ENEMY2_TEXTURE);
    ALLEGRO_BITMAP* bosssprite = al_load_bitmap(BOSS_TEXTURE);
    if (!enemysprite || !enemysprite2 || !bosssprite)
    {
        cout << ("failed to load ENEMY bitmaps!\n");
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
    bool playerDied = false;
    int LoadedCoins = 0;
    while (open)
    {

        if (Level == -1) { ChangeScene(&Level, 0, &MenuObject, NULL, NULL, NULL, NULL, NULL, NULL, &buttons, NULL, NULL, &playerDied, &finish, coinBitmap);}
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
                    LoadedCoins = 0;
                    ChangeScene(&Level, 1, &MenuObject, &platforms, &enemies, &ptS, &plr, &eS, enemyBulletSprite, &buttons, floorT, &cam_x_offset, &playerDied, &finish, coinBitmap);
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
                                ptS.push_back(platformSprite);
                                ptS.push_back(platformSprite2);
                                ptS.push_back(platformSprite3);
                                eS.push_back(enemysprite);
                                eS.push_back(enemysprite2);
                                break;
                            }
                            case 4:
                            {
                                ptS.push_back(platformSprite);
                                ptS.push_back(platformSprite2);
                                ptS.push_back(platformSprite3);
                                eS.push_back(enemysprite);
                                eS.push_back(enemysprite2);
                                eS.push_back(bosssprite);
                                break;
                            }
                        }
                        LoadedCoins = MenuObject->LoadedCoins;
                        Level = MenuObject->LoadedLevel;
                        ChangeScene(&Level, Level, &MenuObject, &platforms, &enemies, &ptS, &plr, &eS, enemyBulletSprite, &buttons, floorT, &cam_x_offset, &playerDied, &finish, coinBitmap);
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
        /* Je�eli wyst�pi event, wysy�amy go do zmiennej ev1 */
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
                gamePause = new Pause(&Behaviours, &buttons, &gamePause, &deltaTime, &playerDied);
            }
            if (toMenuCount > 0)
            {
                toMenuCount -= deltaTime;
                if (toMenuCount <= 0 && toMenuCount != -1)
                {
                    toMenuCount = -1;
                    ChangeScene(&Level, 0, &MenuObject, NULL, NULL, NULL, NULL, NULL, NULL, &buttons, NULL, NULL, &playerDied, &finish, coinBitmap);
                    finish = NULL;
                    plr = NULL;
                    continue;
                }
            }
            al_clear_to_color(al_map_rgb(38, 95, 55));

            if (plr == NULL && finish == NULL)
            {
                plr = new Player(&Behaviours, &buttons, &platforms, playerBulletSprite, &playerBullets, &playerDied, &coins, &LoadedCoins);
                float fX = 0;
                float fY = 0;
                bool hide = false;
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
                case 4:
                {
                    fX = 3655;
                    fY = 480;
                    hide = true;
                    break;
                }
                }

                finish = new FinishLine(&Behaviours, fX, fY, Level, plr, finishSprite, &finishScreen, uiFont, hide);
                finishScreen = new FinishScreen(&Behaviours, &Level, uiFont, plr);

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
        loopBehaviours(&plr, gamePause);
        al_flip_display();
    }
    if (SaveSystem::Instance != NULL && (*SaveSystem::Instance)) { (*SaveSystem::Instance)->Destroy(); delete(SaveSystem::Instance); SaveSystem::Instance = NULL; }
    ClearCoins();
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


    /* Zwalniamy pami�� po kolejce */
    al_destroy_event_queue(kolejka);
    return 0;
}