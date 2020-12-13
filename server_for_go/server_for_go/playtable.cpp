#include "playtable.hpp"
playtable::playtable(sf::TcpSocket* _socket, std::string& _name, bool& _color, int& size) {
    players.push_back(std::shared_ptr<sf::TcpSocket>(_socket));
    selector.add(*_socket);
    lobby_name = _name;
    color_1 = _color;
    color_2 = !_color;
    table_size = size;
    game_begin = false;
    time(&begin_time);
}
void playtable::add_socket(std::shared_ptr<sf::TcpSocket>&& _socket) {
    selector.add(*_socket);
    players.push_back(std::shared_ptr<sf::TcpSocket>(_socket));
    game_begin = true;
}
std::string playtable::return_lob_name() {
    return lobby_name;
}   
bool playtable::get_other_color() {
    return color_2;
}
int playtable::get_tablesize() {
    return table_size;
}
std::shared_ptr<sf::TcpSocket> playtable::get_first_socket() {
    return players[0];
}
std::shared_ptr<sf::TcpSocket> playtable::get_second_socket() {
    return players[1];
}
bool playtable::get_game_status() {
    return game_begin;
}
int playtable::get_tokens_size() {
    return players.size();
}
sf::SocketSelector playtable::get_selector() {
    return selector;
}
bool playtable::send_data() {
    if (selector.wait(sf::seconds(10.f))) {
        sf::Packet packet;
        sf::Packet packet_client;
        int x;
        int y;
        if (selector.isReady(*players[0])) {
            if (players[0]->receive(packet) == sf::Socket::Done) {
                packet >> x >> y;
                packet_client << true << x << y;
                if (players[1]->send(packet_client) == sf::Socket::Disconnected) {
                    sf::Packet disconnect_packet;
                    disconnect_packet << false;
                    players[0]->send(disconnect_packet);
                    players[0]->disconnect();
                    std::cout << "player disconnected, game deleted\n";
                    return false;
                } else {
                    std::cout << "sended for jointer\n";
                    return true;
                }
            } else if (players[0]->receive(packet) == sf::Socket::Disconnected) {
                sf::Packet disconnect_packet;
                disconnect_packet << false;
                players[1]->send(disconnect_packet);
                players[1]->disconnect();
                std::cout << "player disconnected, game deleted\n";
                return false;
            }
        } else {
            if (players[1]->receive(packet) == sf::Socket::Done) {
                packet >> x >> y;
                packet_client << true << x << y;
                if (players[0]->send(packet_client) == sf::Socket::Disconnected) {
                    sf::Packet disconnect_packet;
                    disconnect_packet << false;
                    players[1]->send(disconnect_packet);
                    players[1]->disconnect();
                    std::cout << "player disconnected, game deleted\n";
                    return false;
                } else {
                    std::cout << "sended for creator\n";
                    return true;
                }
            } else if (players[1]->receive(packet) == sf::Socket::Disconnected) {
                sf::Packet disconnect_packet;
                disconnect_packet << false;
                players[0]->send(disconnect_packet);
                players[0]->disconnect();
                std::cout << "player disconnected, game deleted\n";
                return false;
            }
        }
    }
}
bool playtable::check_time() {
    time_t this_time;
    time(&this_time);
    if ((this_time - begin_time) > 600) {
        return false;
    } else {
        return true;
    }
}


