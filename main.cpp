#include <iostream>
#include "Window_Object.h"
#include "Player_Object.h"
#include "Player2_Object.h"
#include "Map_Object.h"
#include "Pellets_Object.h"
#include <SDL2/SDL_ttf.h>

// Function to render text
void draw_text(std::string msg, int x, int y, TTF_Font *font, SDL_Color color, SDL_Renderer *Renderer) {
    SDL_Surface *text_on_surface = TTF_RenderText_Solid(font, msg.c_str(), color);
    if (!text_on_surface) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer, text_on_surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(text_on_surface);
        return;
    }

    // Get the width and height of the text surface
    int texW = 50, texH = 50;
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstRect = {x, y, texW, texH};

    // Copy the texture onto the renderer
    SDL_RenderCopy(Renderer, texture, nullptr, &dstRect);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(text_on_surface);
}

int main(int argc, char **argv) {
    // Initialize SDL and TTF
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    TTF_Font *font = TTF_OpenFont("/Users/priyankasatish/Documents/Games/arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return -1;
    }

    SDL_Color fontColor = {255, 255, 255, 255};

    // Initialize objects
    Window_Object *win_obj = new Window_Object("CHOMP", 1280, 720);
    if (!win_obj || !win_obj->Renderer) {
        std::cerr << "Failed to create window or renderer" << std::endl;
        return -1;
    }

    Player_Object *player = new Player_Object(19, 19, 19, 19, win_obj->Renderer);
    Player2_Object *player2 = new Player2_Object(19, 19, 19, 19, win_obj->Renderer);
    Map_Object *map = new Map_Object(20, 20, win_obj->Renderer);
    Pellets_Object *pellets = new Pellets_Object(map->get_pellets(), win_obj->Renderer);

    // Load win images with error checking
    SDL_Texture *Win1_Texture = nullptr, *Win2_Texture = nullptr;

    SDL_Surface *dummy = IMG_Load("/Users/priyankasatish/Documents/Games/Player1win.png");
    if (!dummy) {
        std::cerr << "Failed to load Player1win.png: " << IMG_GetError() << std::endl;
    } else {
        Win1_Texture = SDL_CreateTextureFromSurface(win_obj->Renderer, dummy);
        if (!Win1_Texture) {
            std::cerr << "Failed to create Player1win texture: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(dummy);
    }

    dummy = IMG_Load("/Users/priyankasatish/Documents/Games/Player2win.png");
    if (!dummy) {
        std::cerr << "Failed to load Player2win.png: " << IMG_GetError() << std::endl;
    } else {
        Win2_Texture = SDL_CreateTextureFromSurface(win_obj->Renderer, dummy);
        if (!Win2_Texture) {
            std::cerr << "Failed to create Player2win texture: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(dummy);
    }

    SDL_Event event;
    int start_time = SDL_GetTicks();
    int lag = 0;
    int time_per_frame = 16;

    int score_P1 = 0;
    int score_P2 = 0;
    bool p1_has_won = false;
    bool p2_has_won = false;

    // Main game loop
    while (!win_obj->quit) {
        // Poll events
        SDL_PollEvent(&event);

        // Handle input
        win_obj->handle_input(event);
        if (!p1_has_won) {
            player->handle_input(event);
        }
        if (!p2_has_won) {
            player2->handle_input(event);
        }

        // Update game state
        int end_time = SDL_GetTicks();
        int dt = end_time - start_time;
        lag += dt;
        while (lag >= time_per_frame) {
            lag -= time_per_frame;

            player->update(map);
            player2->update(map);
            map->handle_collision(player->rect, player->old_x, player->old_y);
            map->handle_collision(player2->rect, player2->old_x, player2->old_y);
            pellets->update(player->rect, score_P1);
            pellets->update(player2->rect, score_P2);
        }
        start_time = SDL_GetTicks();

        // Drawing phase
        win_obj->clear_background();
        map->draw();
        pellets->draw();
        player->draw();
        player2->draw();

        // Display scores
        draw_text("Player 1 score : " + std::to_string(score_P1), map->cell_width * map->cols + 20, 10, font, fontColor, win_obj->Renderer);
        draw_text("Player 2 score : " + std::to_string(score_P2), map->cell_width * map->cols + 20, 40, font, fontColor, win_obj->Renderer);

        // Handle win condition
        if (pellets->is_empty()) {
            SDL_Rect dstRect = {0, 0, map->cell_width * map->cols, map->cell_height * map->rows};
            if (score_P1 > score_P2 && Win1_Texture) {
                SDL_RenderCopy(win_obj->Renderer, Win1_Texture, nullptr, &dstRect);
            } else if (Win2_Texture) {
                SDL_RenderCopy(win_obj->Renderer, Win2_Texture, nullptr, &dstRect);
            }
            SDL_Delay(5000); // Pause to display winner
        }

        win_obj->swap_buffer();  // Swaps buffers to display the rendered frame
    }

    // Clean up resources
    if (Win1_Texture) SDL_DestroyTexture(Win1_Texture);
    if (Win2_Texture) SDL_DestroyTexture(Win2_Texture);
    delete win_obj;
    delete player;
    delete player2;
    delete pellets;
    delete map;
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
