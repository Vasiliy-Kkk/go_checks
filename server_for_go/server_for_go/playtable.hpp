#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include <chrono>
class playtable {
private:
    std::vector<std::shared_ptr<sf::TcpSocket>> players;
    bool color_1;
    bool color_2;
    int table_size;
    std::string lobby_name;
    sf::SocketSelector selector;
    bool game_begin;
    time_t begin_time;
public:
    playtable(sf::TcpSocket* _socket, std::string& _name, bool& _color, int& size);
    void add_socket(std::shared_ptr<sf::TcpSocket>&& _socket);
    std::string return_lob_name();
    bool get_other_color();
    int get_tablesize();
    std::shared_ptr<sf::TcpSocket> get_first_socket();
    std::shared_ptr<sf::TcpSocket> get_second_socket();
    bool get_game_status();
    int get_tokens_size();
    sf::SocketSelector get_selector();
    bool send_data();
    bool check_time(); //false if time > 15m
};

