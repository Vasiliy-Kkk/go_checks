#include "TableStone.hpp"
TableStone::TableStone() {
    x_t = 0;
    y_t = 0;
    radius = 0;
    size = 0;
}
TableStone::TableStone(sf::Vector2i&& _mouse, Table& table, bool _stone_color) {
    switch (table.return_tablesize()) {
    case 1:
        radius = 38;
        size = 9;
        break;
    case 2:
        radius = 28;
        size = 13;
        break;
    case 3:
        radius = 20;
        size = 19;
        break;
    default: break;
    }
    stone_color = _stone_color;
    stone.setRadius(radius);
    x_t = std::get<0>(Checkcoordinate(_mouse, radius, size, table));
    y_t = std::get<1>(Checkcoordinate(_mouse, radius, size, table));
    if (x_t && y_t) {
        stone.setPosition(x_t - radius, y_t - radius);
        if (stone_color) {
            Tablestone_texture.loadFromFile("Images/WhiteTexture.png");
        }
        else {
            Tablestone_texture.loadFromFile("Images/BlackTexture.png");
        }
        stone.setTexture(&Tablestone_texture);
    }

};
TableStone::TableStone(int& x, int& y, Table& table, bool&& _stone_color) {
    x_t = x;
    y_t = y;
    stone_color = _stone_color;
    switch (table.return_tablesize()) {
    case 1:
        radius = 38;
        size = 9;
        break;
    case 2:
        radius = 28;
        size = 13;
        break;
    case 3:
        radius = 20;
        size = 19;
        break;
    default: break;
    }
    stone.setRadius(radius);
    stone.setPosition(x_t - radius, y_t - radius);
    if (stone_color) {
        Tablestone_texture.loadFromFile("Images/WhiteTexture.png");
    } else {
        Tablestone_texture.loadFromFile("Images/BlackTexture.png");
    }
    stone.setTexture(&Tablestone_texture);
}

TableStone::TableStone(TableStone& _stone) {
    this->x_t = _stone.x_t;
    this->y_t = _stone.y_t;
    this->radius = _stone.radius;
    this->Tablestone_texture = _stone.Tablestone_texture;
    this->Tablestone_sprite = _stone.Tablestone_sprite;
    this->stone = _stone.stone;
    this->stone_color = _stone.stone_color;
    this->size = _stone.size;

}
TableStone TableStone::operator=(TableStone& _stone) {
    this->x_t = _stone.x_t;
    this->y_t = _stone.y_t;
    this->radius = _stone.radius;
    this->Tablestone_texture = _stone.Tablestone_texture;
    this->Tablestone_sprite = _stone.Tablestone_sprite;
    this->stone = _stone.stone;
    this->stone_color = _stone.stone_color;
    this->size = _stone.size;
    return *this;
}
bool TableStone::operator ==(TableStone&& _stone) {
    if (this->stone_color == _stone.stone_color
        && this->x_t == _stone.x_t
        && this->y_t == _stone.y_t) {
        return true;
    } else {
        return false;
    }
}
void TableStone::change_tablestone(sf::Vector2i&& _mouse, Table& table, bool& _stone_color) {
    this->x_t = std::get<0>(Checkcoordinate(_mouse, radius, size, table));
    this->y_t = std::get<1>(Checkcoordinate(_mouse, radius, size, table));
    if (x_t && y_t) {
        stone.setPosition(x_t - radius, y_t - radius);
        if (_stone_color) {
            Tablestone_texture.loadFromFile("Images/WhiteTexture.png");
        }
        else {
            Tablestone_texture.loadFromFile("Images/BlackTexture.png");
        }
        stone.setTexture(&Tablestone_texture);
    }
};
bool TableStone::check_stone() {
    if (this->x_t == 0 || this->y_t == 0) {
        return false;
    } else {
        return true;
    }
}
bool TableStone::check_color() {
    return stone_color;
}
sf::CircleShape TableStone::displaystone() const {
    return stone;
};
std::pair<int, int> TableStone::stone_coords(Table* _table) { //first - horisontal, second - vertical
    std::vector<int> x_coords = _table->get_doats_coordinate_x();
    std::vector<int> y_coords = _table->get_doats_coordinate_y();
    int horizontal;
    int vertical;
    for (std::vector<int>::iterator it = x_coords.begin(); it < x_coords.end(); ++it) {
        if (this->x_t == *it) {
            horizontal = std::distance(x_coords.begin(), it);
        }
    }
    for (std::vector<int>::iterator it = y_coords.begin(); it < y_coords.end(); ++it) {
        if (this->y_t == *it) {
            vertical = std::distance(y_coords.begin(), it);
        }
    }
    return (std::make_pair(horizontal, vertical));
}
std::pair<int, int> TableStone::stone_coords_pixels() {
    return std::make_pair(x_t, y_t);
}
int TableStone::stone_x_coords(Table* _table) {
    return (this->stone_coords(_table)).first;
}
int TableStone::stone_y_coords(Table* _table) {
    return (this->stone_coords(_table)).second;
}
int TableStone::get_x() {
    return this->x_t;
}
int TableStone::get_y() {
    return this->y_t;
}