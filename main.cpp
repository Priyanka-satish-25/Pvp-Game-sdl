#include <iostream>
#include "Window_Object.h"
#include "Player_Object.h"
#include "Player2_Object.h"
#include "Map_Object.h"
#include "Pellets_Object.h"
#include <SDL2/SDL_ttf.h>

void draw_text(std::string msg, int x, int y, TTF_Font *font, SDL_Color color, SDL_Renderer *Renderer) {
    SDL_Surface *text_on_surface = TTF_RenderText_Solid(font, msg.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, text_on_surface);

    int texW = 50, texH = 50;
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstRect = {x, y, texW, texH};
    SDL_RenderCopy(Renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(text_on_surface);
}

int main(int argc, char **argv) {
    // initializing stuff
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 15);
    SDL_Color fontColor = {255, 255, 255, 255};

    Window_Object *win_obj = new Window_Object("CHOMP", 975, 775);
    Player_Object *player = new Player_Object(26, 26, 25, 25, win_obj->Renderer);
    Player2_Object *player2 = new Player2_Object(26, 26, 25, 25, win_obj->Renderer);
    Map_Object *map = new Map_Object(25, 25, win_obj->Renderer);
    Pellets_Object *pellets = new Pellets_Object(map->get_pellets(), win_obj->Renderer);
    SDL_Texture *Win1_Texture;
    SDL_Texture *Win2_Texture;

    SDL_Surface *dummy = IMG_Load("Player1win.png");
    Win1_Texture = SDL_CreateTextureFromSurface(player->Renderer, dummy);
    SDL_FreeSurface(dummy);

    dummy = IMG_Load("player2win.png");
    Win2_Texture = SDL_CreateTextureFromSurface(player2->Renderer, dummy);
    SDL_FreeSurface(dummy);

    SDL_Event event;

    int start_time = SDL_GetTicks();
    int lag = 0;
    int time_per_frame = 16;

    int score_P1= 0;
    int score_P2= 0;
    bool restart = false;
    bool p1_has_won = false;
    bool p2_has_won = false;

    while (!win_obj->quit) {
        if (restart && player->State == Player_Object::ALIVE && player2->State == Player2_Object::ALIVE) {
            restart = false;
            pellets->pellets.clear();
            pellets->pellets = map->get_pellets();
        }

        // taking input from user
        SDL_PollEvent(&event);

        // handling input
        win_obj->handle_input(event);
        if (!p1_has_won) {
            player->handle_input(event);
        }
        if (!p2_has_won) {
            player2->handle_input(event);
        }

        // update
        int end_time = SDL_GetTicks();
        int dt = end_time - start_time;
        lag += dt;
        while (lag >= time_per_frame) {
            lag -= time_per_frame;

            // update
            player->update(map);
            player2->update(map);
            map->handle_collision(player->rect, player->old_x, player->old_y);
            map->handle_collision(player2->rect, player2->old_x, player2->old_y);
            pellets->update(player->rect, score_P1);
            pellets->update(player2->rect, score_P2);


            if (!restart && pellets->is_empty()) {
                player->transition(Player_Object::DEAD);
                player2->transition(Player2_Object::DEAD);
                restart = true;
            }
        }
        start_time = SDL_GetTicks();

        // drawing
        //win_obj->clear_background();  // clears background
        map->draw();                  // draws map
        pellets->draw();              // draws pellets
        player->draw();               // draws player
        player2->draw();              // draws player2
        draw_text("Player 1 score : " + std::to_string(score_P1),
                  map->cell_width * map->cols + 10, 0,
                  font, fontColor, win_obj->Renderer);
        draw_text("Player 2 score : " + std::to_string(score_P2),
                  map->cell_width * map->cols + 10, 25,
                  font, fontColor, win_obj->Renderer);
if(pellets->is_empty()){
    if (score_P1 > score_P2) {

        SDL_Rect dstRect = {0, 0, map->cell_width * map->cols, map->cell_height * map->rows};
        SDL_RenderCopy(player->Renderer, Win1_Texture, nullptr, &dstRect);

    } else {

        SDL_Rect dstRect = {0, 0, map->cell_width * map->cols, map->cell_height * map->rows};
        SDL_RenderCopy(player2->Renderer, Win2_Texture, nullptr, &dstRect);

    }
    SDL_Delay(5000);
    player->rect={26,26};
    player2->rect={26,26};
}

        win_obj->swap_buffer();       // swaps buffers
    }

    // destroys everything
    delete win_obj;
    delete player;
    delete player2;
    delete pellets;
    delete map;
    SDL_Quit();
    TTF_Quit();

    return 0;
}