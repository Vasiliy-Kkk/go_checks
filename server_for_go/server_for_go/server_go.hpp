#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <thread>
#include <mutex>
#include "playtable.hpp"
class server_go {
private:
    std::vector<std::shared_ptr<sf::TcpSocket>> clients;
    std::vector<std::shared_ptr<playtable>> games; // vector for ready games
    std::vector<std::shared_ptr<playtable>> lobbys; //vector for waiting games
    sf::IpAddress ip_local;
    sf::IpAddress ip_public;
    sf::TcpListener listener;
    sf::SocketSelector selector;  
    std::list<std::string> lob_names;
    bool new_games_flag;
    static void wait_new(server_go* obj);
    static void clients_handler(server_go* obj);
    static void check_disconnect(server_go* obj);
public:
    server_go();
};