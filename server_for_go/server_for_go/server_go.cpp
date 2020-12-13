#include "server_go.hpp"
server_go::server_go() {
    ip_local = sf::IpAddress::getLocalAddress();
    ip_public = sf::IpAddress::getPublicAddress();
    std::cout << "Server is running on local ip: " << ip_local
        << "\n public ip: " << ip_public << "\n port: 5001\n";
    new_games_flag = false;
    std::thread waiting_th(wait_new, this);
    std::thread handler_th(clients_handler, this);
  //  std::thread disconnect_th(check_disconnect, this);
    waiting_th.join();
    handler_th.join();
   // disconnect_th.join();
}
void server_go::wait_new(server_go* obj) {
    obj->listener.getLocalPort();
    obj->listener.listen(5001);
    obj->selector.add(obj->listener);
    while (true) {
     //   std::cout << "q";
      //  if (obj->selector.wait()) {
      //      if (obj->selector.isReady(obj->listener)) {
        sf::TcpSocket* socket = new sf::TcpSocket;
        if (obj->listener.accept(*socket) == sf::Socket::Done) {
                std::shared_ptr<sf::TcpSocket> sh_sock;
                //obj->listener.accept(*socket);
                sf::Packet packet;
                if (socket->receive(packet) == sf::Socket::Done) {
                    std::cout << "new socket\n";
                    sf::IpAddress ip_client;
                    bool type; //true - create, false - join
                    std::string loby_name;
                    packet >> type >> loby_name;
                    sf::Packet packet_client;
                    if (type) {
                        if (std::find(obj->lob_names.begin(), obj->lob_names.end(), loby_name) == obj->lob_names.end()) {
                            //create lob
                            bool color;
                            int size;
                            packet >> color >> size;
                            std::cout << "created lobby with name: " << loby_name << std::endl;
                            obj->lob_names.push_back(loby_name);
                            obj->lobbys.push_back(std::make_shared<playtable>(playtable(socket, loby_name, color, size)));
                            packet_client << true;
                            auto _it = obj->lobbys.end();
                            --_it;
                            obj->clients.push_back(std::shared_ptr<sf::TcpSocket>((*_it)->get_first_socket()));
                            socket->send(packet_client);
                            obj->selector.add(*socket);
                        } else {
                            //can't create lobby
                            std::cout << "cant connected\n";
                            packet_client << false;
                            socket->send(packet_client);
                        }
                    } else {
                        auto it = std::find_if(obj->lobbys.begin(), obj->lobbys.end(), [&](const std::shared_ptr<playtable>& pt) {
                            return pt->return_lob_name() == loby_name;
                            });
                        std::cout << loby_name << std::endl;
                        if (it != obj->lobbys.end() && !(*it)->get_game_status()) {
                            std::cout << "connected to lobby with name: " << loby_name << std::endl;
                            //connect to lobby
                            (*it)->add_socket(std::shared_ptr<sf::TcpSocket>(socket));
                            sf::Packet packet_host;
                            packet_host << true;
                            //message for host - true
                            (*it)->get_first_socket()->send(packet_host);
                            obj->clients.push_back(std::shared_ptr<sf::TcpSocket>((*it)->get_second_socket()));
                            packet_client << true << (*it)->get_other_color() << (*it)->get_tablesize();
                            obj->games.push_back(std::shared_ptr<playtable>(*it));
                            obj->new_games_flag = true;
                            obj->lobbys.erase(it);
                            //message for client true-color-size
                            socket->send(packet_client);
                            obj->selector.add(*socket);
                        }
                        else {
                            //can't connect to lobby
                            packet_client << false;
                            socket->send(packet_client);
                        }
                    }
                }
            }
        //}
    }
}
void server_go::clients_handler(server_go* obj) {
    time_t _time;
    time(&_time);
    time_t this_time;
    while (true) {
        while (true) {
            if (obj->selector.wait(sf::seconds(10.f))) {
                for (std::vector<std::shared_ptr<playtable>>::iterator it = obj->games.begin();
                    it != obj->games.end(); ++it) {
                    if (!(*it)->send_data()) {
                        auto it_1 = std::find_if(obj->clients.begin(), obj->clients.end(), [&](const std::shared_ptr<sf::TcpSocket>& pt) {
                            return pt == (*it)->get_second_socket();
                            });
                        auto it_2 = std::find_if(obj->clients.begin(), obj->clients.end(), [&](const std::shared_ptr<sf::TcpSocket>& pt) {
                            return pt == (*it)->get_first_socket();
                            });
                        sf::Packet packet;
                        packet << false;
                        (*it_2)->send(packet);
                        obj->selector.remove(**it_1);
                        obj->selector.remove(**it_2);
                        obj->clients.erase(it_1);
                        obj->clients.erase(it_2);
                        obj->games.erase(it);
                        std::cout << "disconnected";
                        obj->listener.listen(5001);
                        break;
                    }
                }
            }
            //delete lobbys whitout games (10 min)
            time(&this_time);
            if ((this_time - _time) > 600) {
                for (std::vector<std::shared_ptr<playtable>>::iterator it = obj->lobbys.begin();
                    it != obj->lobbys.end(); ++it) {
                    if (!(*it)->check_time()) {
                        auto it_1 = std::find_if(obj->clients.begin(), obj->clients.end(), [&](const std::shared_ptr<sf::TcpSocket>& pt) {
                            return pt == (*it)->get_second_socket();
                            });
                        obj->selector.remove(**it_1);
                        (*it_1)->disconnect();
                        obj->lobbys.erase(it);
                        obj->clients.erase(it_1);
                        break;
                    }
                }
                time(&_time);
            }
        }
    }
}
void server_go::check_disconnect(server_go* obj) {
    time_t _time;
    time(&_time);
    time_t this_time;
    while (true) {
        std::cout << "sss\n";
        if (obj->selector.wait(sf::seconds(10.f))) {
            for (std::vector<std::shared_ptr<sf::TcpSocket>>::iterator it = obj->clients.begin();
                it != obj->clients.end(); ++it) {
                if ((**it).Disconnected) {
                    auto it_1 = std::find_if(obj->games.begin(), obj->games.end(), [&](const std::shared_ptr<playtable>& pt) {
                        return pt->get_first_socket() == *it;
                        });
                    if (it_1 != obj->games.end()) {
                        sf::Packet  packet;
                        packet << false;
                        (*it_1)->get_second_socket()->disconnect();
                        obj->games.erase(it_1);
                        auto it_2 = std::find(obj->clients.begin(), obj->clients.end(), (*it_1)->get_second_socket());
                        (*it_2)->send(packet);
                        obj->selector.remove(**it_2);
                        (*it_2)->disconnect();
                        obj->clients.erase(it_2);
                    }
                    else {
                        it_1 = std::find_if(obj->games.begin(), obj->games.end(), [&](const std::shared_ptr<playtable>& pt) {
                            return pt->get_second_socket() == *it;
                            });
                        if (it_1 != obj->games.end()) {
                            sf::Packet  packet;
                            packet << false;
                            (*it_1)->get_first_socket()->disconnect();
                            obj->games.erase(it_1);
                            auto it_2 = std::find(obj->clients.begin(), obj->clients.end(), (*it_1)->get_first_socket());
                            (*it_2)->send(packet);
                            obj->selector.remove(**it_2);
                            (*it_2)->disconnect();
                            obj->clients.erase(it_2);
                        } 
                        else {
                            auto it_3 = std::find_if(obj->lobbys.begin(), obj->lobbys.end(), [&](const std::shared_ptr<playtable>& pt) {
                                return pt->get_first_socket() == *it;
                                });
                            obj->lobbys.erase(it_3);
                        }
                    }
                    obj->selector.remove(**it);
                    it = obj->clients.erase(it);
                    break;
                    std::cout << "disconnected\n";
                }
            }
        } 
        //delete lobbys whitout games (10 min)
        time(&this_time);
        if ((this_time - _time) > 600) {
            for (std::vector<std::shared_ptr<playtable>>::iterator it = obj->lobbys.begin();
                it != obj->lobbys.end(); ++it) {
                if (!(*it)->check_time()) {
                    auto it_1 = std::find_if(obj->clients.begin(), obj->clients.end(), [&](const std::shared_ptr<sf::TcpSocket>& pt) {
                        return pt == (*it)->get_second_socket();
                        });
                    obj->selector.remove(**it_1);
                    (*it_1)->disconnect();
                    obj->lobbys.erase(it);
                    obj->clients.erase(it_1);
                    break;
                }
            }
            time(&_time);
        }
    }
}

