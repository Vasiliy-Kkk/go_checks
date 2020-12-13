#ifndef INCLUDE_FOR_MAINMENU_HPP_
#define INCLUDE_FOR_MAINMENU_HPP_

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <list>
#include "Button.hpp"
std::pair<int, int> return_stone_coordinate(const std::pair<int, int>& _pair, int& tablesize);
std::list<std::pair<int, int>>::iterator& operator+=(std::list < std::pair<int, int>>::iterator& it, int x);
int middlescreenX(int& x);
int middlescreenY(int& y);
void if_mouse_not_on_button(Button& button, sf::RenderWindow& window, bool flag);
bool check_press(const std::vector<bool>& _vec, int&& a, int&& b);
#endif // INCLUDE_FOR_MAINMENU_HPP_
