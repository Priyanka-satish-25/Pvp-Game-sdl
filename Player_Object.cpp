#include "Player_Object.h"

Player_Object::Player_Object(int x, int y, int w, int h, SDL_Renderer *Renderer)
        : Renderer(Renderer) {
    rect = {x, y, w, h};

    SDL_Surface *dummy = IMG_Load("pacman_alive.png");
    Alive_Texture = SDL_CreateTextureFromSurface(Renderer, dummy);
    SDL_FreeSurface(dummy);

    dummy = IMG_Load("pacman_dead.png");
    Dead_Texture = SDL_CreateTextureFromSurface(Renderer, dummy);
    SDL_FreeSurface(dummy);

    transition(ALIVE);
}

void Player_Object::handle_input(SDL_Event event) {
    old_x = rect.x;
    old_y = rect.y;
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
            move_right = true;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
            move_left = true;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            move_up = true;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            move_down = true;
        }
    } else if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
            move_right = false;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
            move_left = false;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            move_up = false;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            move_down = false;
        }
    }
}

void Player_Object::update(Map_Object *map) {
    // code for movement
    switch (State) {
        case ALIVE:
            // move
            if (move_right) {
                rect.x += speed;
            }
            if (move_left) {
                rect.x -= speed;
            }
            if (move_up) {
                rect.y -= speed;
            }
            if (move_down) {
                rect.y += speed;
            }
            break;
        case DEAD:
            // do nothing
            break;
    }

    // code for animation
    switch (State) {
        case ALIVE:
            if (!move_left && !move_right && !move_up && !move_down) {
                Cur_Frame.row = 2;
                Animation_Frame.current = Animation_Frame.limit - 1;
            } else {
                Animation_Frame.current++;
                if (Animation_Frame.current == Animation_Frame.limit) {
                    Animation_Frame.current = 0;
                    Cur_Frame.row++;
                    if (Cur_Frame.row == 3) Cur_Frame.row = 0;
                }
                if (move_left) Cur_Frame.col = 0;
                if (move_right) Cur_Frame.col = 1;
                if (move_up) Cur_Frame.col = 2;
                if (move_down) Cur_Frame.col = 3;
            }
            break;
        case DEAD:
            Animation_Frame.current++;
            if (Animation_Frame.current == Animation_Frame.limit) {
                Animation_Frame.current = 0;
                Cur_Frame.col++;
                if (Cur_Frame.col == 3) {
                    Cur_Frame.col = 0;
                    Cur_Frame.row++;
                    if (Cur_Frame.row == 4) {
                        transition(ALIVE);
                    }
                }
            }

            break;
    }
}

void Player_Object::draw() {
    SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(Renderer, &rect);
    SDL_Rect srcRect = {Cur_Frame.col * 48, Cur_Frame.row * 48, 48, 48};
    SDL_Rect dstRect = {rect.x - rect.w / 2, rect.y - rect.h / 2, rect.w * 2, rect.h * 2};
    SDL_RenderCopy(Renderer, Current_Texture, &srcRect, &dstRect);
}

Player_Object::~Player_Object() {
    SDL_DestroyTexture(Alive_Texture);
    SDL_DestroyTexture(Dead_Texture);
}

void Player_Object::transition(Player_Object::States s) {
    State = s;
    switch (State) {
        case ALIVE:
            Current_Texture = Alive_Texture;
            Animation_Frame = {0, 5};
            Cur_Frame = {2, 0};
            rect.x = 25;
            rect.y = 25;
            break;
        case DEAD:
            Current_Texture = Dead_Texture;
            Animation_Frame = {0, 10};
            Cur_Frame = {0, 0};
            break;
    }
}

