/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: bulat
 *
 * Created on 29 октября 2021 г., 1:19
 */
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>

int health = 100;
float level = 1;
float count_of_wood = 0;
float count_of_stone = 0;

int speed = 100;

const int map_size_x = 32;
const int map_size_y = 24;
float tile_size = 1024 / map_size_x;
int map[map_size_x][map_size_y] = {
    {}
};
float map_health[map_size_x][map_size_y] = {
    {}
};

sf::Texture player_texture;
sf::Texture player_hit_texture;

sf::Sprite player(player_texture, sf::IntRect(0, 0, 43, 64));
float player_x_pos = 0;
float player_y_pos = 0;
float player_x_pos_map = 0;
float player_y_pos_map = 0;

int mouse_x_pos = 0;
int mouse_y_pos = 0;

bool is_hit = false;

int changed_block_to_build = -1;

void map_init() {
    std::srand(static_cast<unsigned int> (time(0)));
    int water_x_pos = map_size_x, water_y_pos = 0;
    map[water_x_pos][water_y_pos] = 3;
    int step = 0;
    int n_w_y_p = water_y_pos, n_w_x_p = water_x_pos;
    for (int i = water_x_pos; i > 0; i--) {
        n_w_x_p -= 1;
        if (step % 2 == 0) {
            n_w_y_p += 1;
        }
        if (n_w_y_p < map_size_y && n_w_y_p > 0)
            map[n_w_x_p][n_w_y_p - 1] = 2;

        if (n_w_y_p < map_size_y && n_w_y_p > 1)
            map[n_w_x_p][n_w_y_p - 2] = 2;

        if (n_w_y_p < map_size_y - 4 && n_w_y_p>-5)
            map[n_w_x_p][n_w_y_p + 4] = 2;

        if (n_w_y_p < map_size_y - 5 && n_w_y_p>-6)
            map[n_w_x_p][n_w_y_p + 5] = 2;

        if (n_w_y_p < map_size_y && n_w_y_p>-1)
            map[n_w_x_p][n_w_y_p] = 3;

        if (n_w_y_p < map_size_y - 1 && n_w_y_p>-2)
            map[n_w_x_p][n_w_y_p + 1] = 3;

        if (n_w_y_p < map_size_y - 2 && n_w_y_p>-3)
            map[n_w_x_p][n_w_y_p + 2] = 3;

        if (n_w_y_p < map_size_y - 3 && n_w_y_p>-4)
            map[n_w_x_p][n_w_y_p + 3] = 3;

        step++;
    }
    for (int i = 0; i < 15; i++) {
        int x = std::rand() / ((RAND_MAX + 1u) / map_size_x);
        int y = std::rand() / ((RAND_MAX + 1u) / map_size_y);
        if (map[x][y] < 3) {
            map[x][y] = map[x][y] + 5;
        }
    }
    for (int i = 0; i < 15; i++) {
        int x = std::rand() / ((RAND_MAX + 1u) / map_size_x);
        int y = std::rand() / ((RAND_MAX + 1u) / map_size_y);

        if (map[x][y] < 3) {
            map[x][y] = map[x][y] + 8;
        }
    }
}

int main(int argc, char** argv) {
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
    }
    sf::RenderWindow window(sf::VideoMode(1124, 768), "Game", sf::Style::Fullscreen);
    if (!player_hit_texture.loadFromFile("player_hit.png"));
    if (!player_texture.loadFromFile("player.png"));
    sf::Texture tile_texture;
    if (!tile_texture.loadFromFile("tiles.png")) {
    }
    map_init();



    player.setOrigin(16, 32);
    player.setPosition(player_x_pos, player_y_pos);
    player.setScale(0.5f, 0.5f);

    sf::Clock clock;
    sf::Clock cl;
    sf::Time millis;
    while (window.isOpen()) {

        sf::Time elaps = clock.restart();
        sf::Event event;
        window.clear(sf::Color(0, 0, 0));
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
                mouse_x_pos = sf::Mouse::getPosition().x;
                mouse_y_pos = sf::Mouse::getPosition().y;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&!is_hit) {
                if (mouse_x_pos <= 1024 && changed_block_to_build == -1) {
                    float near_block_x_pos = -1;
                    float near_block_y_pos = -1;
                    is_hit = true;
                    millis = cl.restart();
                    for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                            if (player_x_pos / tile_size - 1 > -1 && player_x_pos / tile_size + 1 < map_size_x && player_y_pos / tile_size - 1 > -1 && player_y_pos / tile_size + 1 < map_size_y) {
                                if (map[(int) floor(i + player_x_pos / tile_size)][(int) floor(j + player_y_pos / tile_size)] > 3) {
                                    near_block_x_pos = i * tile_size + player_x_pos;
                                    near_block_y_pos = j * tile_size + player_y_pos;
                                }
                            }
                        }
                    }

                    if (near_block_x_pos > -1 && near_block_y_pos > -1) {
                        if (mouse_x_pos >= near_block_x_pos - tile_size / 2 && mouse_x_pos <= near_block_x_pos + tile_size / 2 && mouse_y_pos >= near_block_y_pos - tile_size / 2 && mouse_y_pos <= near_block_y_pos + tile_size / 2) {
                            switch (map[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)]) {
                                case 5:
                                    count_of_wood += level;
                                    break;
                                case 6:
                                    count_of_wood += level;
                                    break;
                                case 7:
                                    count_of_wood += level;
                                    break;
                                case 8:
                                    count_of_stone += level;
                                    break;
                                case 9:
                                    count_of_stone += level;
                                    break;
                                case 10:
                                    count_of_stone += level;
                                    break;
                                case 4:
                                    map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] -= level;
                                    if (map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] <= 0) {
                                        map[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] = 0;
                                    }
                                    break;
                                case 11:
                                    map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] -= level;
                                    if (map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] <= 0) {
                                        map[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] = 0;
                                    }
                                    break;
                                case 12:
                                    map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] -= level;
                                    if (map_health[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] <= 0) {
                                        map[(int) (near_block_x_pos / tile_size)][(int) (near_block_y_pos / tile_size)] = 0;
                                    }
                                    break;

                            }
                            level += 0.02f;
                        }
                    }
                } else if (changed_block_to_build != -1 && map[(int) (mouse_x_pos / tile_size)][(int) (mouse_y_pos / tile_size)] < 4) {
                    map[(int) (mouse_x_pos / tile_size)][(int) (mouse_y_pos / tile_size)] = changed_block_to_build;
                    switch (changed_block_to_build) {
                        case 4:
                            map_health[(int) (mouse_x_pos / tile_size)][(int) (mouse_y_pos / tile_size)] = 50;
                            break;
                        case 11:
                            map_health[(int) (mouse_x_pos / tile_size)][(int) (mouse_y_pos / tile_size)] = 15;
                            break;
                        case 12:
                            map_health[(int) (mouse_x_pos / tile_size)][(int) (mouse_y_pos / tile_size)] = 35;
                            break;
                    }
                    changed_block_to_build = -1;

                } else {
                    if (mouse_y_pos >= 128) {
                        if (mouse_y_pos < 228) {
                            if (count_of_wood >= 25 && count_of_stone >= 50) {
                                changed_block_to_build = 4;
                                count_of_wood -= 25;
                                count_of_stone -= 50;
                            }
                        } else if (mouse_y_pos < 328) {
                            if (count_of_wood >= 25) {
                                changed_block_to_build = 11;
                                count_of_wood -= 25;
                            }
                        } else if (mouse_y_pos < 428) {
                            if (count_of_stone >= 50) {
                                changed_block_to_build = 12;
                                count_of_stone -= 50;
                            }
                        }
                    }
                }
            }
        }


        for (int i = 0; i < map_size_x; i++) {
            for (int j = 0; j < map_size_y; j++) {
                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tile_size, tile_size));
                tile.setPosition(i * tile_size, j * tile_size);
                tile.setTexture(&tile_texture);
                tile.setTextureRect(sf::IntRect(map[i][j]*512, 0, 512, 512));
                if (map[i][j] > 6) {
                    tile.setTextureRect(sf::IntRect((map[i][j] - 7)*512, 512, 512, 512));
                }
                window.draw(tile);
            }
        }

        window.draw(player);
        sf::Texture resources_text_texture;
        if (!resources_text_texture.loadFromFile("texts.png"));

        sf::RectangleShape count_of_wood_text_i;
        count_of_wood_text_i.setPosition(1030.0f, 6.0f);
        count_of_wood_text_i.setSize(sf::Vector2f(64.0f, 64.0f));
        count_of_wood_text_i.setTexture(&resources_text_texture);
        count_of_wood_text_i.setTextureRect(sf::IntRect(0, 0, 64, 64));

        sf::Text count_of_wood_text;
        count_of_wood_text.setCharacterSize(40);
        count_of_wood_text.setColor(sf::Color(255, 255, 255));
        count_of_wood_text.setFont(font);
        count_of_wood_text.setPosition(1100.0f, 6.0f);
        count_of_wood_text.setString(std::to_string(count_of_wood));
        window.draw(count_of_wood_text_i);
        window.draw(count_of_wood_text);

        sf::RectangleShape count_of_stone_text_i;
        count_of_stone_text_i.setPosition(1030.0f, 40.0f);
        count_of_stone_text_i.setSize(sf::Vector2f(64.0f, 64.0f));
        count_of_stone_text_i.setTexture(&resources_text_texture);
        count_of_stone_text_i.setTextureRect(sf::IntRect(64, 0, 64, 64));

        sf::Text count_of_stone_text;
        count_of_stone_text.setCharacterSize(40);
        count_of_stone_text.setColor(sf::Color(255, 255, 255));
        count_of_stone_text.setFont(font);
        count_of_stone_text.setPosition(1100.0f, 40.0f);
        count_of_stone_text.setString(std::to_string(count_of_stone));
        window.draw(count_of_stone_text_i);
        window.draw(count_of_stone_text);


        sf::RectangleShape strong_bricks_button;
        strong_bricks_button.setPosition(1024, 128);
        strong_bricks_button.setSize(sf::Vector2f(100.0f, 100.0f));
        strong_bricks_button.setTexture(&tile_texture);
        strong_bricks_button.setTextureRect(sf::IntRect(2048, 0, 512, 512));
        window.draw(strong_bricks_button);

        sf::RectangleShape wooden_block_button;
        wooden_block_button.setPosition(1024, 228);
        wooden_block_button.setSize(sf::Vector2f(100.0f, 100.0f));
        wooden_block_button.setTexture(&tile_texture);
        wooden_block_button.setTextureRect(sf::IntRect(2048, 512, 512, 512));
        window.draw(wooden_block_button);

        sf::RectangleShape stone_block_button;
        stone_block_button.setPosition(1024, 328);
        stone_block_button.setSize(sf::Vector2f(100.0f, 100.0f));
        stone_block_button.setTexture(&tile_texture);
        stone_block_button.setTextureRect(sf::IntRect(2560, 512, 512, 512));
        window.draw(stone_block_button);

        if (changed_block_to_build != -1) {
            sf::RectangleShape block_change_rect;
            block_change_rect.setFillColor(sf::Color(255, 0, 0, 100));
            block_change_rect.setSize(sf::Vector2f(tile_size, tile_size));
            block_change_rect.setPosition(floor(mouse_x_pos / tile_size) * tile_size, floor(mouse_y_pos / tile_size) * tile_size);
            window.draw(block_change_rect);
        }

        window.display();

        if (cl.getElapsedTime().asMilliseconds() > 500) {
            is_hit = false;
        }
        if (is_hit) {
            player.setTexture(player_hit_texture);
        } else {

            player.setTexture(player_texture);
            player.setPosition(player_x_pos, player_y_pos);
            //player.setRotation(atan2(near_block_y_pos - player.getPosition().y, near_block_x_pos - player.getPosition().x)*(180 / M_PI));
            player.setRotation(atan2(mouse_y_pos - player.getPosition().y, mouse_x_pos - player.getPosition().x)*(180 / M_PI));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (map[(int) floor((player_x_pos) / tile_size)][((int) floor((player_y_pos) / tile_size)) - 1] < 4)
                    player_y_pos -= speed * elaps.asSeconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if (map[((int) floor((player_x_pos) / tile_size)) - 1][(int) floor((player_y_pos) / tile_size)] < 4)
                    player_x_pos -= speed * elaps.asSeconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (map[(int) floor((player_x_pos) / tile_size)][((int) floor((player_y_pos) / tile_size)) + 1] < 4)
                    player_y_pos += speed * elaps.asSeconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                if (map[((int) floor((player_x_pos) / tile_size)) + 1][(int) floor((player_y_pos) / tile_size)] < 4)
                    player_x_pos += speed * elaps.asSeconds();
            }

        }
        switch (map[(int) floor((player_x_pos) / tile_size)][(int) floor((player_y_pos) / tile_size)]) {
            case 0:
                speed = 100;
                break;
            case 1:
                speed = 75;
                break;
            case 2:
                speed = 60;
                break;
            case 3:
                speed = 30;
                break;
        }

    }
    return 0;
}