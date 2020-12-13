#include "MainMenu.hpp"
MainMenu::MainMenu(int& scrX, int& scrY) {
				this->scrX = scrX;
				this->scrY = scrY;
				menu_table_flag = true;
				board_size = 0;
				waiting_answer_flag = false;
}
MainMenu::MainMenu(Table& _table) {
				table = &_table;
}

void MainMenu::add_stone(std::shared_ptr<TableStone> stone) {
				if (VACANT(stone->stone_x_coords(table), stone->stone_y_coords(table))) {
								if (stone->check_color()) {
												list_real_stones.push_back(stone);
												list_coord_white_stones.push_back(stone->stone_coords(table));
												delete_flag = true;
												suicide_flag = false;
												while (delete_flag) {
																if_delete_stones(stone->check_color());
																if (suicide_flag) {
																				//////////////////////////////////
																				list_real_stones.erase(--list_real_stones.end());
																				list_coord_white_stones.erase(--list_coord_white_stones.end());
																				break;
																}	
												}
								}	else {
												list_real_stones.push_back(stone);
												list_coord_black_stones.push_back(stone->stone_coords(table));
												delete_flag = true;
												suicide_flag = false;
												while (delete_flag) {
																if_delete_stones(stone->check_color());
																if (suicide_flag) {
																				//////////////////////////////////
																				list_real_stones.erase(--list_real_stones.end());
																				list_coord_black_stones.erase(--list_coord_black_stones.end());
																				break;
																}	
												}
								}
								if (!suicide_flag) {
												std::thread th(wait_stone, this, stone);
												th.detach();
								}
				}
}
void MainMenu::wait_stone(MainMenu* obj, std::shared_ptr<TableStone> stone) {
				obj->waiting_answer_flag = true;
				sf::Packet packet;
				packet << stone->get_x() << stone->get_y();
				obj->socket.send(packet);
				std::cout << "packet sended\n";
				packet.clear();
			//	if (obj->socket.receive(packet) == sf::Socket::Done) {
				obj->socket.receive(packet);
				std::cout << "packet get\n";
				packet >> obj->disconnect_flag;
				if (obj->disconnect_flag) {
								int x;
								int y;
								packet >> x >> y;
								TableStone* stone__ = new TableStone(x, y, *obj->table, !obj->color);
								std::shared_ptr<TableStone> _stone(stone__);
								obj->list_real_stones.push_back(_stone);
								if (obj->color) {
												obj->list_coord_black_stones.push_back(_stone->stone_coords(obj->table));
								}
								else {
												obj->list_coord_white_stones.push_back(_stone->stone_coords(obj->table));
								}
								obj->delete_flag = true;
								while (obj->delete_flag) {
												obj->if_delete_stones(_stone->check_color());
								}
								obj->waiting_answer_flag = false;
								obj->changed_score = true;
								//		}
								std::cout << "end of waiting\n";
				}	else {
								obj->waiting_answer_flag = false;
								obj->disconnect_flag = false;
				}
}
void MainMenu::print_menu(sf::RenderWindow& window) {
				Button creategame(middlescreenX(scrX), middlescreenY(scrY) / 4, "Create Game", 100); //создание кнопок главного меню
				Button joingame(middlescreenX(scrX), middlescreenY(scrY) / 2, "Join Game", 100);
				Button exitbutton(middlescreenX(scrX), 3 * middlescreenY(scrY) / 4, "Exit", 100);
				Button smallboard(7 * middlescreenX(scrX) / 5, middlescreenY(scrY) / 4, "9x9", 50);
				Button mediumboard(8 * middlescreenX(scrX) / 5, middlescreenY(scrY) / 4, "13x13", 50);
				Button bigboard(9 * middlescreenX(scrX) / 5, middlescreenY(scrY) / 4, "19x19", 50);
				Button WhiteColor(7.5 * middlescreenX(scrX) / 5, 1.5 * middlescreenY(scrY) / 4, "White", 50);
				Button BlackColor(8.5 * middlescreenX(scrX) / 5, 1.5 * middlescreenY(scrY) / 4, "Black", 50);
				Button backbutton(8 * middlescreenX(scrX) / 5, 1.5 * middlescreenY(scrY) / 2, "Back", 50);
				Button writetext(7.5 * middlescreenX(scrX) / 5, middlescreenY(scrY) / 2, "Write lobby name:", 40);
				Button writebutton(8.5 * middlescreenX(scrX) / 5, 0.87 * middlescreenY(scrY) / 2, 40);
				Button create(8 * middlescreenX(scrX) / 5, 1.25 * middlescreenY(scrY) / 2, "Create", 50);
				Button join(8 * middlescreenX(scrX) / 5, 1.25 * middlescreenY(scrY) / 2, "Join", 50);
				std::vector<std::unique_ptr<Button>> buttons; 
				buttons.push_back(std::make_unique<Button>(creategame));  //0
				buttons.push_back(std::make_unique<Button>(joingame));				//1
				buttons.push_back(std::make_unique<Button>(exitbutton));		//2
				buttons.push_back(std::make_unique<Button>(smallboard));		//3
				buttons.push_back(std::make_unique<Button>(mediumboard));	//4
				buttons.push_back(std::make_unique<Button>(bigboard));				//5
				buttons.push_back(std::make_unique<Button>(WhiteColor));		//6
				buttons.push_back(std::make_unique<Button>(BlackColor));		//7
				buttons.push_back(std::make_unique<Button>(backbutton));		//8
				buttons.push_back(std::make_unique<Button>(writetext));			//9
				buttons.push_back(std::make_unique<Button>(writebutton));	//10
				buttons.push_back(std::make_unique<Button>(create));						//11
				buttons.push_back(std::make_unique<Button>(join));								//12
				std::vector <bool> colorflags;
				for (auto i = 0; i < 13; ++i) {
								colorflags.push_back(false);
				}
				std::vector<bool> chooseflags; //choose boardsize&color
				for (auto i = 0; i < 5; ++i) {
								chooseflags.push_back(false);
				}
				bool createflag = false; //флаг дл€ изменени€ цвета кнопок
				bool joinflag = false;
				menu_table_flag = true;
				socket.disconnect();
				while (window.isOpen()) {
								sf::Event event;
								while (window.pollEvent(event)) {
												if (event.type == sf::Event::Closed) {
																window.close();
												}
												if ((*buttons[0]).ifpress(sf::Mouse::getPosition(window))) {
																(*buttons[0]).changeTextColor();
																if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																				createflag = true;
																				joinflag = false;
																				(*buttons[10]).emptytext();
																}
																colorflags[0] = true;
												}
												if_mouse_not_on_button((*buttons[0]), window, colorflags[0]);
												if ((*buttons[1]).ifpress(sf::Mouse::getPosition(window))) {
																(*buttons[1]).changeTextColor();
																if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																				joinflag = true;
																				createflag = false;
																				(*buttons[10]).emptytext();
																}
																colorflags[1] = true;
												}
												if_mouse_not_on_button((*buttons[1]), window, colorflags[1]);
												if ((*buttons[2]).ifpress(sf::Mouse::getPosition(window))) {
																(*buttons[2]).changeTextColor();
																if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
																				socket.disconnect();
																				window.close();
																}
																colorflags[2] = true;
												}
												if_mouse_not_on_button((*buttons[2]), window, colorflags[2]);
												if (createflag) {
																if ((*buttons[3]).ifpress(sf::Mouse::getPosition(window))) {
																				if (!check_press(chooseflags, 0, 2)) {
																								(*buttons[3]).changeTextColor();
																				}
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (chooseflags[1]) {
																												(*buttons[4]).changeTextColorBack();
																												chooseflags[1] = false;
																								}
																								if (chooseflags[2]) {
																												(*buttons[5]).changeTextColorBack();
																												chooseflags[2] = false;
																								}
																								chooseflags[0] = true;
																								(*buttons[3]).changeTextColor();
																								board_size = 1;
																				}
																				colorflags[3] = true;
																}
																if ((*buttons[4]).ifpress(sf::Mouse::getPosition(window))) {
																				if (!check_press(chooseflags, 0, 2)) {
																								(*buttons[4]).changeTextColor();
																				}
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (chooseflags[0]) {
																												(*buttons[3]).changeTextColorBack();
																												chooseflags[0] = false;
																								}
																								if (chooseflags[2]) {
																												(*buttons[5]).changeTextColorBack();
																												chooseflags[2] = false;
																								}
																								chooseflags[1] = true;
																								(*buttons[4]).changeTextColor();
																								board_size = 2;
																				}
																				colorflags[4] = true;
																}
																if ((*buttons[5]).ifpress(sf::Mouse::getPosition(window))) {
																				if (!check_press(chooseflags, 0, 2)) {
																								(*buttons[5]).changeTextColor();
																				}
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (chooseflags[0]) {
																												(*buttons[3]).changeTextColorBack();
																												chooseflags[0] = false;
																								}
																								if (chooseflags[1]) {
																												(*buttons[4]).changeTextColorBack();
																												chooseflags[1] = false;
																								}
																								chooseflags[2] = true;
																								(*buttons[5]).changeTextColor();
																								board_size = 3;
																				}
																				colorflags[5] = true;
																}
																if ((*buttons[6]).ifpress(sf::Mouse::getPosition(window))) {
																				if (!check_press(chooseflags, 3, 4)) {
																								(*buttons[6]).changeTextColor();
																				}
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (chooseflags[4]) {
																												(*buttons[7]).changeTextColorBack();
																												chooseflags[4] = false;
																								}
																								chooseflags[3] = true;
																								(*buttons[6]).changeTextColor();
																								color = true;
																				}
																				colorflags[6] = true;
																}
																if ((*buttons[7]).ifpress(sf::Mouse::getPosition(window))) {
																				if (!check_press(chooseflags, 3, 4)) {
																								(*buttons[7]).changeTextColor();
																				}
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (chooseflags[3]) {
																												(*buttons[6]).changeTextColorBack();
																												chooseflags[3] = false;
																								}
																								chooseflags[4] = true;
																								(*buttons[7]).changeTextColor();
																								color = false;
																				}
																				colorflags[7] = true;
																}
																if ((*buttons[8]).ifpress(sf::Mouse::getPosition(window))) {
																				(*buttons[8]).changeTextColor();
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								createflag = false;
																								(*buttons[10]).emptytext();
																				}
																				colorflags[8] = true;
																}
																if_mouse_not_on_button((*buttons[8]), window, colorflags[8]);
																window.draw((*buttons[8]).displayText());
																window.draw((*buttons[9]).displayText());
																if (event.type == sf::Event::TextEntered && !waiting_answer_flag)
																{
																				// отсекаем не ASCII символы
																				if (event.text.unicode < 128)
																				{
																								(*buttons[10]).add_letter(static_cast<char>(event.text.unicode));
																				}
																}
																if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
																				if (!waiting_answer_flag) {
																								(*buttons[10]).delete_letter();
																				}	else {
 																								waiting_answer_flag = false;
																				} 
																} 
																window.draw((*buttons[10]).displayText());
																if ((*buttons[11]).ifpress(sf::Mouse::getPosition(window)) && !waiting_answer_flag) {
																				(*buttons[11]).changeTextColor();
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !waiting_answer_flag) {
																								if (check_press(chooseflags, 0, 2) && check_press(chooseflags, 3, 4) && !(*buttons[10]).isempty()) {
																												//creategame
																												loby_name = (*buttons[10]).get_text();
																												host_flag = true;
																												creator = false;
																												std::thread th(wait_connect, this);
																												th.detach();
																												std::cout << "jointed";
																								}
																				}
																				colorflags[11] = true;
																}
																if_mouse_not_on_button((*buttons[11]), window, colorflags[11]);
																window.draw((*buttons[11]).displayText());
																if (check_press(chooseflags, 3, 4)) {
																				window.draw((*buttons[7]).displayText());
																				window.draw((*buttons[6]).displayText());
																}
																else {
																				if_mouse_not_on_button((*buttons[7]), window, colorflags[7]);
																				window.draw((*buttons[7]).displayText());
																				if_mouse_not_on_button((*buttons[6]), window, colorflags[6]);
																				window.draw((*buttons[6]).displayText());
																}
																if (check_press(chooseflags, 0, 2)) {
																				window.draw((*buttons[5]).displayText());
																				window.draw((*buttons[4]).displayText());
																				window.draw((*buttons[3]).displayText());
																}
																else {
																				if_mouse_not_on_button((*buttons[5]), window, colorflags[5]);
																				window.draw((*buttons[5]).displayText());
																				if_mouse_not_on_button((*buttons[4]), window, colorflags[4]);
																				window.draw((*buttons[4]).displayText());
																				if_mouse_not_on_button((*buttons[3]), window, colorflags[3]);
																				window.draw((*buttons[3]).displayText());
																}
												}
												if (joinflag) {
																window.draw((*buttons[9]).displayText());
																if (event.type == sf::Event::TextEntered)
																{
																				if (event.text.unicode < 128)
																				{
																								(*buttons[10]).add_letter(static_cast<char>(event.text.unicode));
																				}
																}
																if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
																				(*buttons[10]).delete_letter();
																}
																window.draw((*buttons[10]).displayText());
																if ((*buttons[12]).ifpress(sf::Mouse::getPosition(window))) {
																				(*buttons[12]).changeTextColor();
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
																								if (!(*buttons[10]).isempty()) {
																												//joingame
																												loby_name = (*buttons[10]).get_text();
																												host_flag = false;
																												ip = "192.168.1.21";
																												sf::Packet packet;
																												socket.connect(ip, 5001);
																												packet << host_flag << loby_name;
																												socket.send(packet);
																												packet.clear();
																												socket.receive(packet);
																												packet >> creator;
																												if (creator) {
																																std::cout << "connected!";
																																packet >> color >> board_size;
																																menu_table_flag = false;
																																break;
																												}
																								}
																				}
																				colorflags[12] = true;
																}
																if ((*buttons[8]).ifpress(sf::Mouse::getPosition(window))) {
																				(*buttons[8]).changeTextColor();
																				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
																								if (!waiting_answer_flag) {
																												joinflag = false;
																												(*buttons[10]).emptytext();
																								} else {

																								}
																				}
																				colorflags[8] = true;
																}
																if_mouse_not_on_button((*buttons[8]), window, colorflags[8]);
																window.draw((*buttons[8]).displayText());
																if_mouse_not_on_button((*buttons[12]), window, colorflags[12]);
																window.draw((*buttons[12]).displayText());
												}
												window.draw((*buttons[0]).displayText());
												window.draw((*buttons[1]).displayText());
												window.draw((*buttons[2]).displayText());
												window.display();
												window.clear();
												if (!menu_table_flag) {
																break;
												}
								}
								if (!menu_table_flag) {
												break;
								}
				}
}
void MainMenu::wait_connect(MainMenu* obj) {
				obj->waiting_answer_flag = true;
				std::cout << "before connecting\n";
				obj->ip = "192.168.1.21";
				sf::Packet packet;
				obj->socket.connect(obj->ip, 5001);
				std::cout << "after connecting\n";
				packet << obj->host_flag << obj->loby_name << obj->color << obj->board_size;
				obj->socket.send(packet);
				packet.clear();
				obj->socket.receive(packet);
				packet >> obj->creator;
				if (obj->creator) {
								packet.clear();
								obj->socket.receive(packet);
								packet >> obj->creator;
								obj->menu_table_flag = false;
				} else {
								obj->socket.disconnect();
								obj->loby_name.clear();
				}
				obj->waiting_answer_flag = false;
}
void MainMenu::wait_first_stone(MainMenu* obj) {
				obj->waiting_answer_flag = true;
				sf::Packet packet;
				obj->socket.receive(packet);
				packet >> obj->disconnect_flag;
				if (obj->disconnect_flag) {
								int x;
								int y;
								packet >> x >> y;
								TableStone* stone_ = new TableStone(x, y, *obj->table, !obj->color);
								std::shared_ptr<TableStone> _stone(stone_);
								obj->list_real_stones.push_back(_stone);
								obj->list_coord_white_stones.push_back(_stone->stone_coords(obj->table));
								obj->waiting_answer_flag = false;
				}
}
void MainMenu::print_table(sf::RenderWindow& window) {
				white_score = 0;
				black_score = 0;
				Button Push_Stone(1.2 * scrX / 2, 0.9 * scrY / 2, "Add Stone", 75);
				Button exitbutton(1.2 * scrX / 2, 18 * scrY / 20, "Exit", 50);
				Button whitebutton(scrX / 2, scrY / 20, "White:", 50);
				Button whitescorebut(1.1 * scrX / 2, scrY / 20, white_score, 50);
				Button blackbutton(1.3 * scrX / 2, scrY / 20, "Black:", 50);
				Button blackscorebut(1.4 * scrX / 2, scrY / 20, black_score, 50);
				bool push_flag = false;
				bool begin_flag = false;
				bool exit_flag = false;
				bool dontpush_stone_flag = false;
				disconnect_flag = true;
				TableStone _helperstone(sf::Mouse::getPosition(window), *table, this->color);
				TableStone* helperstone = new TableStone();
				TableStone _pushed_stone(sf::Mouse::getPosition(window), *table, this->color);
				TableStone* p_stone = new TableStone();
				std::shared_ptr<TableStone> pushed_stone(p_stone);
				if (!color) {
								std::thread th(wait_first_stone, this);
								th.detach();
				}
				while (window.isOpen())
				{
								sf::Event event;
								while (window.pollEvent(event))
								{
												if (event.type == sf::Event::Closed) {
																window.close();
												}
												window.draw((*table).displaytablesprite());
												if ((*table).checkStoneCursor(sf::Mouse::getPosition(window))) {
																if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && !waiting_answer_flag) {

																				if (*pushed_stone == TableStone(sf::Mouse::getPosition(window), *table, color)) {
																								dontpush_stone_flag = false;																				}
																				else {
																								pushed_stone.reset(new TableStone(sf::Mouse::getPosition(window), *table, color));
																								begin_flag = true;
																								dontpush_stone_flag = true;
																				}
																}
															 if (!dontpush_stone_flag && !waiting_answer_flag) {
																								helperstone = new TableStone(sf::Mouse::getPosition(window), *table, color);
																								window.draw(helperstone->displaystone());
																}
												}
												if (VACANT(pushed_stone->stone_x_coords(table), pushed_stone->stone_y_coords(table)) && begin_flag && dontpush_stone_flag) {
																window.draw(pushed_stone->displaystone());
												}
												if_mouse_not_on_button(exitbutton, window, exit_flag);
												if (exitbutton.ifpress(sf::Mouse::getPosition(window))) {
																exitbutton.changeTextColor();
																if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
																				menu_table_flag = true;
																				break;
																}
																exit_flag = true;
												}
												if_mouse_not_on_button(Push_Stone, window, push_flag);
												if (Push_Stone.ifpress(sf::Mouse::getPosition(window))) {
																Push_Stone.changeTextColor();
																if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && begin_flag && dontpush_stone_flag && !waiting_answer_flag) {
																				add_stone(pushed_stone);
																				whitescorebut.change_text(white_score);
																				blackscorebut.change_text(black_score);
																				dontpush_stone_flag = false;
																}
																push_flag = true;
												}
												if (changed_score) {
																whitescorebut.change_text(white_score);
																blackscorebut.change_text(black_score);
																changed_score = false;
												}
												if (!disconnect_flag) {
																menu_table_flag = true;
																break;
												}
												for (auto it : list_real_stones) {
																window.draw((*it).displaystone());
												}
												window.draw(Push_Stone.displayText());
												window.draw(exitbutton.displayText());
												window.draw(whitebutton.displayText());
												window.draw(whitescorebut.displayText());
												window.draw(blackbutton.displayText());
												window.draw(blackscorebut.displayText());
												window.display();
												window.clear(sf::Color(210, 155, 52));
								}
								if (menu_table_flag) {
												list_real_stones.clear();
												list_coord_black_stones.clear();
												list_coord_black_stones.clear();
												loby_name.clear();
												creator = false;
												waiting_answer_flag = false;
												break;
								}
				}
}

void MainMenu::print_window(sf::RenderWindow& window) {
				while (window.isOpen()) {
								if (menu_table_flag) {
												print_menu(window);
								}
								else {
												Table t(board_size);
												table = &t;
												print_table(window);
								}
				}
}
void MainMenu::if_delete_stones(bool&& color) {
				bool _color;
				if (color) {
								_color = false;
								for (std::list < std::pair<int, int>>::iterator it = list_coord_black_stones.begin();
												it != list_coord_black_stones.end(); ++it) {
												int x = it->first;
												int y = it->second;
												if (check_neighbours(x, y, _color, color)) {
																delete_flag = true;
																return;
												}	else {
																delete_flag = false;
												}
								}
								_color = true;
								for (std::list < std::pair<int, int>>::iterator it = list_coord_white_stones.begin();
												it != list_coord_white_stones.end(); ++it) {
												int x = it->first;
												int y = it->second;
												if (check_neighbours(x, y, _color, color)) {
																delete_flag = true;
																suicide_flag = true;
																return;
												}	else {
																delete_flag = false;
												}
								}
				}	else {
								_color = true;
								for (std::list < std::pair<int, int>>::iterator it = list_coord_white_stones.begin();
												it != list_coord_white_stones.end(); ++it) {
												int x = it->first;
												int y = it->second;
												if (check_neighbours(x, y, _color, color)) {
																delete_flag = true;
																return;
												}
												else {
																delete_flag = false;
												}
								}
								_color = false;
								for (std::list < std::pair<int, int>>::iterator it = list_coord_black_stones.begin();
												it != list_coord_black_stones.end(); ++it) {
												int x = it->first;
												int y = it->second;
												if (check_neighbours(x, y, _color, color)) {
																suicide_flag = true;
																delete_flag = true;
																return;
												}
												else {
																delete_flag = false;
												}
								}
				}
}

bool MainMenu::check_neighbours(int& x, int& y,	bool& color, bool& last_color) {    //помен€ть потом пор€док if -  в начале наиболее веро€тные событи€
				std::list < std::pair<int, int>> eat;
				std::list < std::pair<int, int>> eated;
				int tablesize;
				switch (table->return_tablesize()) {
				case 1: tablesize = 8;
								break;
				case 2: tablesize = 12;
								break;
				case 3: tablesize = 18;
								break;
				default: 
								;
				}
				if (NOT_VACANT(x, y, color)) {
								if (x == 0 && y == 0) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if(check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
							 				--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								} else if (x == 0 && y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								} else if (x == tablesize && y == 0) {
												eated.push_back(std::make_pair(x, y));
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}	else if (x == tablesize && y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}	else if (x == 0) {
												eated.push_back(std::make_pair(x, y));
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}	else if (x == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}	else if (y == 0) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								} else if (y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}	else {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
				  			eated.sort();
				  			eated.erase(std::unique(eated.begin(), eated.end()), eated.end());
								if (color != last_color) {
												for (std::list<std::pair<int, int>>::iterator it = eated.begin(); it != eated.end(); ++it) {
																delete_stones(*it, color);
												}
								}	else {
												suicide_flag = true;
								}
								return true;
				} 
				return false;
}
void MainMenu::delete_stones(const std::pair<int, int>& eated, bool& color) {
				int table_size = table->return_tablesize();
				std::pair<int, int> stone_coords;
				stone_coords = return_stone_coordinate(eated, table_size);
				list_real_stones.erase(std::find_if(list_real_stones.begin(), list_real_stones.end(),
								[&stone_coords](std::shared_ptr<TableStone> arg) {
												return	arg->stone_coords_pixels() == stone_coords;
				}));
				if (color) {
								list_coord_white_stones.erase(std::find(list_coord_white_stones.begin(),
												list_coord_white_stones.end(), eated));
								++black_score;
				}	else {
								list_coord_black_stones.erase(std::find(list_coord_black_stones.begin(),
												list_coord_black_stones.end(), eated));
								++white_score;
				}
}
bool MainMenu::check_eat_stone(int& x, int& y, bool& color,
				std::list<std::pair<int, int>>& eat, std::list<std::pair<int, int>>& eated) {
				if (NOT_VACANT(x, y, color)) {
								if (NOT_VACANT_ANY_LIST(x, y, eated)) {
												if (!check_neighbours(x, y, eat, eated, color)) {
																return true;
												}
								}
								return false;
				}	else if (NOT_VACANT(x, y, !color)) {
								if (NOT_VACANT_ANY_LIST(x, y, eat)) {
												eat.push_back(std::make_pair(x, y));
								}
								return false;
				}	else {
								return true;
				}
}
bool MainMenu::NOT_VACANT(int& x, int& y, bool& color) {
				if (color) {
								if (std::find(list_coord_white_stones.begin(), list_coord_white_stones.end(), std::make_pair(x, y))
												== list_coord_white_stones.end()) {
												return false;
								}	else {
												return true;
								}
				}	else {
								if (std::find(list_coord_black_stones.begin(), list_coord_black_stones.end(), std::make_pair(x, y))
												== list_coord_black_stones.end()) {
												return false;
								}	else {
												return true;
								}
				}
}
bool MainMenu::NOT_VACANT(int& x, int& y, bool&& color) {
				if (color) {
								if (std::find(list_coord_white_stones.begin(), list_coord_white_stones.end(), std::make_pair(x, y))
												== list_coord_white_stones.end()) {
												return false;
								}
								else {
												return true;
								}
				}	else {
								if (std::find(list_coord_black_stones.begin(), list_coord_black_stones.end(), std::make_pair(x, y))
												== list_coord_black_stones.end()) {
												return false;
								}
								else {
												return true;
								}
				}
}
bool MainMenu::VACANT(int&& x, int&& y) {
	 		if ((std::find(list_coord_white_stones.begin(), list_coord_white_stones.end(), std::make_pair(x, y))
	 						== list_coord_white_stones.end())
	 						&& (std::find(list_coord_black_stones.begin(), list_coord_black_stones.end(), std::make_pair(x, y))
	 										== list_coord_black_stones.end())) {
	 						return true;
	 		}
	 		else {
	 						return false;
	 		}
}
bool MainMenu::NOT_VACANT_ANY_LIST(int& x, int& y, std::list<std::pair<int, int>> _list) {
				if (std::find(_list.begin(), _list.end(), std::make_pair(x, y))	== _list.end()) {
								return true;
				}	else {
								return false;
				}
}
bool MainMenu::check_neighbours(int& _x, int& _y, std::list<std::pair<int, int>>& eat, std::list<std::pair<int, int>>& eated, bool& color) {
				int tablesize;
				int x;
				int y;
				x = _x;
				y = _y;
				switch (table->return_tablesize()) {
				case 1: tablesize = 8;
								break;
				case 2: tablesize = 12;
								break;
				case 3: tablesize = 18;
								break;
				default: break;
				}
				if (NOT_VACANT(x, y, color)) {
								if (x == 0 && y == 0) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (x == 0 && y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (x == tablesize && y == 0) {
												eated.push_back(std::make_pair(x, y));
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (x == tablesize && y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (x == 0) {
												eated.push_back(std::make_pair(x, y));
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (x == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (y == 0) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												--y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else if (y == tablesize) {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								else {
												eated.push_back(std::make_pair(x, y));
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--y;
												--x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												--x;
												++y;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
												++y;
												++x;
												if (check_eat_stone(x, y, color, eat, eated)) {
																return false;
												}
								}
								return true;
				}
}
