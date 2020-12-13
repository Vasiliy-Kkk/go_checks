#include "for_mainmenu.hpp"
std::pair<int, int> return_stone_coordinate(const std::pair<int, int>& _pair, int& tablesize) {
				int x;
				int y;
				int i;
				int j;
				i = _pair.first;
				j = _pair.second;
				switch (tablesize) {
				case 1:
								x = round(115 + 77.625 * i);
								y = round(114 + 77.625 * j);
								return std::make_pair(x, y);
								break;
				case 2:
								x = round(83 + 56.33 * i);
								y = round(82 + 56.33 * j);
								return std::make_pair(x, y);
								break;
				case 3:
								x = 59 + 40 * i;
								y = 60 + 40 * j;
								return std::make_pair(x, y);
								break;
				default: break;
				}
}
std::list<std::pair<int, int>>::iterator& operator+=(std::list < std::pair<int, int>>::iterator& it, int x) {
				for (auto i = 0; i < x; ++i) {
								++it;
				}
				return it;
}
int middlescreenX(int& x) {
				return x / 2;
}

int middlescreenY(int& y) {
				return y / 2;
}
void if_mouse_not_on_button(Button& button, sf::RenderWindow& window, bool flag) {
				if (flag && !button.ifpress(sf::Mouse::getPosition(window))) {
								button.changeTextColorBack();
								flag = false;
				}
}
bool check_press(const std::vector<bool>& _vec, int&& a, int&& b) {
				if (a > b) {
								std::swap(a, b);
				}
				for (; a <= b; ++a) {
								if (_vec[a]) {
												return true;
								}
				}
				return false;
}