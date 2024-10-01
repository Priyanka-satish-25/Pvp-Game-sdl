#ifndef DRAMA_CLUB_PLAYER2_OBJECT_H
#define DRAMA_CLUB_PLAYER2_OBJECT_H
#include <SDL2/SDL.h>
#include "Map_Object.h"

class Player2_Object {
public:

    SDL_Rect rect;
    SDL_Renderer *Renderer;

    int old_x;
    int old_y;
    int speed = 5;

    bool move_left{};
    bool move_up{};
    bool move_down{};
    bool move_right{};

    struct frame {
        int current;
        int limit;
    } Animation_Frame;

    struct frame_index {
        int row;
        int col;
    } Cur_Frame;

    SDL_Texture *Alive_Texture;
    SDL_Texture *Dead_Texture;
    SDL_Texture *Current_Texture;

    enum States {
        ALIVE, DEAD
    } State = ALIVE;

    Player2_Object(int x, int y, int w, int h, SDL_Renderer *Renderer);

    void handle_input(SDL_Event event);

    void update(Map_Object *map);

    void transition(States s);

    void draw();

    ~Player2_Object();

};


#endif //DRAMA_CLUB_PLAYER2_OBJECT_H
