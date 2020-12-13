#ifndef INCLUDE_MAINMENU_HPP_
#define INCLUDE_MAINMENU_HPP_

#pragma once
#include<SFML/Network.hpp>
#include<iterator>
#include <list>
#include <utility>
#include <iostream>
#include <memory>
#include <thread>
#include "Button.hpp"
#include "TableStone.hpp"
#include "for_mainmenu.hpp"

class MainMenu {
private:
    Table* table;
    std::list<std::shared_ptr<TableStone>> list_real_stones;
    std::list<std::pair<int, int>> list_coord_white_stones;
    std::list<std::pair<int, int>> list_coord_black_stones;
    int board_size;
    bool menu_table_flag;
    int scrX;
    int scrY;
    bool color;
    bool delete_flag;   //флаг, что удалились камни
    bool suicide_flag;   //флаг попытки суицида камня
    sf::IpAddress ip;
    sf::TcpSocket socket;
    std::string loby_name;
    bool host_flag;
    bool waiting_answer_flag; //for threads & block interface
    bool creator; //true - loby is created
    bool changed_score;
    bool disconnect_flag; // false if player disconnected   
    int white_score;
    int black_score;
public:
    MainMenu(int& scrX, int& scrY);
    MainMenu(Table& _table);
    void print_table(sf::RenderWindow& window); //печатаем стол+камни
    void print_menu(sf::RenderWindow& window); //печатаем главное меню
    void add_stone(std::shared_ptr<TableStone> stone);
    void print_window(sf::RenderWindow& window); //печатаем либо стол, либо меню
    void if_delete_stones(bool&& color);
    bool check_neighbours(int& x, int& y, bool& color, bool& last_color);
    bool check_neighbours(int& x, int& y, std::list<std::pair<int, int>>& eat, std::list<std::pair<int, int>>& eated, bool& color);
    void delete_stones(const std::pair<int, int>& eated, bool& color);
    bool check_eat_stone(int& x, int& y, bool& color,
        std::list<std::pair<int, int>>& eat, std::list<std::pair<int, int>>& eated);
    bool NOT_VACANT(int& x, int& y, bool& color);
    bool NOT_VACANT(int& x, int& y, bool&& color);
    bool VACANT(int&& x, int&& y);
    bool NOT_VACANT_ANY_LIST(int& x, int& y, std::list<std::pair<int, int>> _list);
    static void wait_connect(MainMenu* obj);
    static void wait_stone(MainMenu* obj, std::shared_ptr<TableStone> stone);
    static void wait_first_stone(MainMenu* obj);
};
#endif // INCLUDE_MAINMENU_HPP_
