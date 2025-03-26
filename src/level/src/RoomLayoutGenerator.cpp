#include "RoomLayoutGenerator.hpp"

#include <cmath>

#include "Level.hpp"
#include "TileBatch.hpp"

namespace {
    enum class Direction : std::uint16_t {
        LEFT = 0,
        RIGHT,
        DOWN
    };

    void obtain_new_direction(int current_x, int width_x_rooms, Direction &direction) {
        if (current_x == 0) {
            // We're on the left side of the map, so go right:
            direction = Direction::RIGHT;
        } else if (current_x == width_x_rooms - 1) {
            // We're on the right side of the map, so go left:
            direction = Direction::LEFT;
        } else {
            // We're in the middle, so make a guess where should we go now (only left/right allowed):
            direction = static_cast<Direction>(std::rand() % 2);
        }
    }
}


void RoomLayoutGenerator::generate(const RoomLayoutGeneratorParams &params) {

    _width_x_rooms = params.width_x_rooms;
    _height_y_rooms = params.height_y_rooms;
    _room_width_tiles = params.room_width_tiles;
    _room_height_tiles = params.room_height_tiles;
    
    // Clean the previous layout:
    for (int x = 0; x < _width_x_rooms; x++) {
        for (int y = 0; y < Consts::ROOMS_COUNT_HEIGHT; y++) {
            _layout[x][y] = RoomType::CLOSED;
        }
    }

    // Set starting position to the random room in the upper-most row:
    int curr_x = std::rand() % _width_x_rooms;
    int curr_y = Consts::ROOMS_COUNT_HEIGHT - 1;

    // Direction represents where the generator will go in the next loop iteration.
    Direction direction;
    obtain_new_direction(curr_x, _width_x_rooms, direction);

    bool exit_placed = false;

    // Set the starting room as an entrance room
    _layout[curr_x][curr_y] = RoomType::ENTRANCE;

    // While we're on the very bottom floor or higher, do:
    while (curr_y >= 0) {
        if (direction == Direction::LEFT || direction == Direction::RIGHT) {
            if ((direction == Direction::LEFT && curr_x == 0) || (
                    direction == Direction::RIGHT && curr_x == _width_x_rooms - 1)) {
                // Our direction is to go left, but we're already on the left side of the map, so go down:
                direction = Direction::DOWN;
            } else {
                if (direction == Direction::LEFT) {
                    // Our direction is to go left, and we're not on the left side of the map yet:
                    curr_x--;
                } else {
                    // Same, if right side:
                    curr_x++;
                }

                if (curr_y == 0 && !exit_placed && std::rand() % 2 == 0) {
                    // We're on the most bottom floor, we didn't plant an exit yet and we've guessed that's the place:
                    exit_placed = true;
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                } else {
                    _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT;
                }

                if (std::rand() % 3 == 2) {
                    // Random chance that we change our direction to go down in the next iteration:
                    direction = Direction::DOWN;
                }
            }
        } else if (direction == Direction::DOWN) {
            if (curr_y > 0) {
                _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_DOWN;
                curr_y--;
                _layout[curr_x][curr_y] = RoomType::LEFT_RIGHT_UP;

                if (curr_y == 0 && !exit_placed && std::rand() % 2 == 0) {
                    // If we're on the very bottom floor, no exit planted yet and a guess tells us so, place an exit:
                    exit_placed = true;
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                }

                obtain_new_direction(curr_x, _width_x_rooms, direction);
            } else {
                if (!exit_placed) {
                    // We're on the very bottom floor, didn't plant an exit yet and we're
                    // done with iterating through map, so plant an exit:
                    _layout[curr_x][curr_y] = RoomType::EXIT;
                }
                break;
            }
        }
    }

    // Post-generation effects:
    place_an_altar();
    place_a_shop(params.shopkeeper_robbed);
}

RoomType RoomLayoutGenerator::get_room_type_at_room(int x_room, int y_room) const {
    return _layout[x_room][y_room];
}

RoomType RoomLayoutGenerator::get_room_type_at_tile(int x_tile, int y_tile) const {
    int room_x = std::floor(static_cast<float>(x_tile) / _room_width_tiles);
    int room_y = std::floor(static_cast<float>(y_tile) / _room_height_tiles);

    return _layout[room_x][room_y];
}

int RoomLayoutGenerator::get_width_tiles() const {
    return _width_x_rooms * Consts::ROOM_WIDTH_TILES;
}

int RoomLayoutGenerator::get_height_tiles() const {
    return _height_y_rooms * Consts::ROOM_HEIGHT_TILES;
}

void RoomLayoutGenerator::place_an_altar() {
    auto &level = Level::instance().get_tile_batch();
    for (int x = 0; x < _width_x_rooms; x++) {
        for (int y = 0; y < Consts::ROOMS_COUNT_HEIGHT; y++) {
            if (_layout[x][y] == RoomType::CLOSED) {
                _layout[x][y] = RoomType::ALTAR;
                return;
            }
        }
    }
}

void RoomLayoutGenerator::place_a_shop(bool shopkeeper_robbed) {
    for (int x = 0; x < _width_x_rooms; x++) {
        for (int y = 0; y < Consts::ROOMS_COUNT_HEIGHT; y++) {
            if (_layout[x][y] == RoomType::CLOSED) {
                if (x == 0) {
                    if (_layout[x + 1][y] != RoomType::CLOSED) {
                        if (shopkeeper_robbed) {
                            _layout[x][y] = RoomType::SHOP_RIGHT_MUGSHOT;
                        } else {
                            _layout[x][y] = RoomType::SHOP_RIGHT;
                        }
                        return;
                    }
                } else if (x == 2) {
                    if (_layout[x - 1][y] != RoomType::CLOSED) {
                        if (shopkeeper_robbed) {
                            _layout[x][y] = RoomType::SHOP_LEFT_MUGSHOT;
                        } else {
                            _layout[x][y] = RoomType::SHOP_LEFT;
                        }
                        return;
                    }
                } else if (x == 1) {
                    if (_layout[x - 1][y] != RoomType::CLOSED &&
                        _layout[x + 1][y] != RoomType::CLOSED) {
                        if (std::rand() % 2 == 0) {
                            _layout[x][y] = RoomType::SHOP_LEFT;
                        } else {
                            _layout[x][y] = RoomType::SHOP_RIGHT;
                        }

                        // Placed a shop, can return.
                        return;
                    }
                }
            }
        }
    }
}
