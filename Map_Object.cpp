#include "Map_Object.h"

Map_Object::Map_Object(int cell_w, int cell_h, SDL_Renderer *Renderer)
        : Renderer(Renderer), cell_height(cell_h), cell_width(cell_w) {

    SDL_Surface *dummy = IMG_Load("Maze.png");
    Map_Texture = SDL_CreateTextureFromSurface(Renderer, dummy);
    SDL_FreeSurface(dummy);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (tile_map[row][col] == 1) {
                SDL_Rect rect = {
                        col * cell_w,
                        row * cell_h,
                        cell_w,
                        cell_h
                };
                walls.push_back(rect);
            }
        }
    }
}

Map_Object::~Map_Object() {
    SDL_DestroyTexture(Map_Texture);
}

void Map_Object::draw() {
   SDL_SetRenderDrawColor(Renderer, 128, 0, 128, 255);
    for (auto rect: walls) {
        SDL_RenderFillRect(Renderer, &rect);
   }
   //SDL_Rect dstRect = {0, 0, cell_width * cols, cell_height * rows};
    //SDL_RenderCopy(Renderer, Map_Texture, nullptr, &dstRect);
}

void Map_Object::handle_collision(SDL_Rect &rect, int old_x, int old_y) {
    /*for (auto wall: walls) {
        if (SDL_HasIntersection(&wall, &rect)) {
            if (rect.x > old_x) {
                rect.x = wall.x - rect.w;
            } else if (rect.x < old_x) {
                rect.x = wall.x + wall.w;
            }
            if (rect.y > old_y) {
                rect.y = wall.y - rect.h;
            } else if (rect.y < old_y) {
                rect.y = wall.y + wall.h;
            }
            break;
        }
    }*/
    if (is_colliding(rect)) {
        int x_dist = rect.x - old_x;
        int y_dist = rect.y - old_y;

        rect.x = old_x;
        rect.y = old_y;

        while (!is_colliding(rect)) {
            old_y = rect.y;

            if (y_dist > 0) {
                rect.y += 1;
                y_dist -= 1;
            } else if (y_dist < 0) {
                rect.y -= 1;
                y_dist += 1;
            } else if (y_dist == 0) {
                break;
            }
        }
        rect.y = old_y;

        while (!is_colliding(rect)) {
            old_x = rect.x;

            if (x_dist > 0) {
                rect.x += 1;
                x_dist -= 1;
            } else if (x_dist < 0) {
                rect.x -= 1;
                x_dist += 1;
            } else if (x_dist == 0) {
                break;
            }
        }
        rect.x = old_x;
    }
}

bool Map_Object::is_wall(int x_index, int y_index) {
    if (!(0 <= x_index && x_index <= cols))
        return false;
    if (!(0 <= y_index && y_index <= rows))
        return false;
    return (tile_map[y_index][x_index] == 1);
}

bool Map_Object::is_colliding(SDL_Rect &rect) {
    int x1 = (rect.x + 1) / cell_width;
    int x2 = (rect.x + rect.w - 1) / cell_width;
    int y1 = (rect.y + 1) / cell_height;
    int y2 = (rect.y + rect.h - 1) / cell_height;

    return is_wall(x1, y1) ||
           is_wall(x2, y1) ||
           is_wall(x1, y2) ||
           is_wall(x2, y2);

}

bool Map_Object::is_on_floor(SDL_Rect &rect) {
    int x1 = (rect.x + 1) / cell_width;
    int x2 = (rect.x + rect.w - 1) / cell_width;
    int y1 = rect.y / cell_height;

    if (is_wall(x1, y1 + 1) || is_wall(x2, y1 + 1)) {
        if (rect.y + rect.h == (y1 + 1) * cell_height) {
            return true;
        }
    }
    return false;
}

bool Map_Object::is_below_ceil(SDL_Rect &rect) {
    int x1 = (rect.x + 1) / cell_width;
    int x2 = (rect.x + rect.w - 1) / cell_width;
    int y2 = (rect.y + rect.h - 1) / cell_height;

    if (is_wall(x1, y2 - 1) || is_wall(x2, y2 - 1)) {
        if (rect.y == y2 * cell_height) {
            return true;
        }
    }
    return false;
}

std::vector<SDL_Rect> Map_Object::get_pellets() {
    std::vector<SDL_Rect> pellets;
    int size = cell_width / 3;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (tile_map[row][col] == 2) {
                SDL_Rect rect = {
                        col * cell_width + cell_width / 2 - size / 2,
                        row * cell_height + cell_height / 2 - size / 2,
                        size, size
                };
                pellets.push_back(rect);
            }
        }
    }
    return pellets;
}