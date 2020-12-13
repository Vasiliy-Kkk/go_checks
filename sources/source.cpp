#include <SFML/Graphics.hpp>
//if you use windows
//#include <Windows.h>
#include "MainMenu.hpp"

int main()
{
				// in windows:
				//int scrX = GetSystemMetrics(SM_CXSCREEN); //îïòèìàëüíûå ïàðàìåòðû îêíà 
				//int scrY = GetSystemMetrics(SM_CYSCREEN);
				int scrX = 1600;
				int scrY = 900;
				sf::RenderWindow window(sf::VideoMode(scrX, scrY), "Go", sf::Style::Close);
				window.setVerticalSyncEnabled(true); //âåðòèêàëüíàÿ ñèíõðîíèçàöèÿ îêíà
				MainMenu menu(scrX, scrY);
				menu.print_window(window);
				return 0;
}
