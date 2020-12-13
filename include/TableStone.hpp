#ifndef INCLUDE_TABLESTONE_HPP_
#define INCLUDE_TABLESTONE_HPP_

#pragma once
#include <vector>
#include "Table.hpp"

class TableStone {
private:
    int x_t; //координаты камн€
    int y_t;
    int radius; //радиус камн€
    sf::Texture Tablestone_texture; //текстура камн€
    sf::Sprite Tablestone_sprite;
    sf::CircleShape stone; //окружность камн€
    bool stone_color; //true - white, false - black
    int size; //длина доски
protected:

public:
    TableStone();
    explicit TableStone(TableStone& stone);
    TableStone(sf::Vector2i&& _mouse, Table& table, bool _stone_color);
    TableStone(int& x, int& y, Table& table, bool&& _stone_color);
    TableStone& operator=(TableStone& _stone);
    bool operator ==(TableStone&& _stone);
    void change_tablestone(sf::Vector2i&& _mouse, Table& table, bool& _stone_color);
    bool check_stone();
    bool check_color();
    sf::CircleShape displaystone() const; //вывод камн€ на экран
    std::pair<int, int> stone_coords(Table* _table); //first - horisontal, second - vertical
    std::pair<int, int> stone_coords_pixels();
    int stone_x_coords(Table* _table);
    int stone_y_coords(Table* _table);
    int get_x();// in pixels
    int get_y();// in pixels
};
#endif // INCLUDE_TABLESTONE_HPP_
