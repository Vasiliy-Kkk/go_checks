#include "Table.hpp"
Table::Table() {
    x_t = 0;
    y_t = 0;
    tablesize = 0;
}
Table::Table(int& _tablesize) {
    x_t = 0;
    y_t = 0;
    tablesize = _tablesize;
    switch (tablesize) {
    case 1: 
        tableimage.loadFromFile("Images/smalltable.png");
        for (auto i = 0; i < 9; ++i) {
            doats_coordinate_x.push_back(round(115 + 77.625 * i));
            doats_coordinate_y.push_back(round(114 + 77.625 * i));
        }
        break;
    case 2: 
        tableimage.loadFromFile("Images/mediumtable.png");
        for (auto i = 0; i < 13; ++i) {
            doats_coordinate_x.push_back(round(83 + 56.33 * i));
            doats_coordinate_y.push_back(round(82 + 56.33 * i));
        }
        break;
    case 3: 
        tableimage.loadFromFile("Images/bigtable.png");
        for (auto i = 0; i < 19; ++i) {
            doats_coordinate_x.push_back(59 + 40 * i);
            doats_coordinate_y.push_back(60 + 40 * i);
        }
        break;
    default: break; 
    }
    tablesprite.setTexture(tableimage);
    tablesprite.setPosition(x_t, y_t);
}
Table::Table(int&& _tablesize) {
    x_t = 0;
    y_t = 0;
    tablesize = _tablesize;
    switch (tablesize) {
    case 1:
        tableimage.loadFromFile("Images/smalltable.png");
        for (auto i = 0; i < 9; ++i) {
            doats_coordinate_x.push_back(round(115 + 77.625 * i));
            doats_coordinate_y.push_back(round(114 + 77.625 * i));
        }
        break;
    case 2:
        tableimage.loadFromFile("Images/mediumtable.png");
        for (auto i = 0; i < 13; ++i) {
            doats_coordinate_x.push_back(round(83 + 56.33 * i));
            doats_coordinate_y.push_back(round(82 + 56.33 * i));
        }
        break;
    case 3:
        tableimage.loadFromFile("Images/bigtable.png");
        for (auto i = 0; i < 19; ++i) {
            doats_coordinate_x.push_back(59 + 40 * i);
            doats_coordinate_y.push_back(60 + 40 * i);
        }
        break;
    default: break;
    }
    tablesprite.setTexture(tableimage);
    tablesprite.setPosition(x_t, y_t);
}
int Table::return_tablesize() const{
    return tablesize;
}
sf::Sprite Table::displaytablesprite() const{
    return tablesprite;
}
std::vector<int>::iterator Table::return_iter_x() {
    std::vector<int>::iterator it_x = doats_coordinate_x.begin();
    return it_x;
}
std::vector<int>::iterator Table::return_iter_y() {
    std::vector<int>::iterator it_y = doats_coordinate_y.begin();
    return it_y;
}
bool Table::checkStoneCursor(sf::Vector2i _mouse) {
    int radius;
    switch ((*this).return_tablesize()) {
    case 1:
        radius = 34;
        break;
    case 2:
        radius = 25;
        break;
    case 3:
        radius = 18;
        break;
    default: break;
    }
    if (std::get<0>(Checkcoordinate(_mouse, radius, *this)) && std::get<1>(Checkcoordinate(_mouse, radius, *this))) {
        return true;
    }
    else {
        return false;
    }
    return false;
}
std::pair<int, int> Table::Checkcoordinate(sf::Vector2i _mouse, int radius, Table&& table) const {
    for (auto it_x : table.doats_coordinate_x) {
        for (auto it_y : table.doats_coordinate_y) {
            if (_in_circle(_mouse.x, _mouse.y, radius, it_x, it_y)) {
                return (std::make_pair(it_x, it_y));
            }
        }
    }
    return std::make_pair(0, 0);
}
std::pair<int, int> Table::Checkcoordinate(sf::Vector2i _mouse, int radius, Table& table) const {
    for (auto it_x : table.doats_coordinate_x) {
        for (auto it_y : table.doats_coordinate_y) {
            if (_in_circle(_mouse.x, _mouse.y, radius, it_x, it_y)) {
                return (std::make_pair(it_x, it_y));
            }
        }
    }
    return std::make_pair(0, 0);
}
std::vector<int> Table::get_doats_coordinate_x() {
    return doats_coordinate_x;
}
std::vector<int> Table::get_doats_coordinate_y() {
    return doats_coordinate_y;
}
