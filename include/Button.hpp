#ifndef INCLUDE_BUTTON_HPP_
#define INCLUDE_BUTTON_HPP_


#pragma once
#include <SFML/Graphics.hpp>

class Button {
private:
    double width;		// Ширина Кнопки
    double height;		// Высота Кнопки
    double x_t;
    double y_t;
    unsigned int text_size;
    std::string str_text;
    sf::Text text;
    sf::Font font;
    sf::RectangleShape box;	// Создаем прямоугольник
public:
    Button(double&& _x_t, double&& _y_t, std::string&& _text, int&& _text_size);
    Button(double&& _x_t, double&& _y_t, int&& _text_size);
    Button(double&& _x_t, double&& _y_t, int& score, int&& _text_size);
    bool ifpress(sf::Vector2i&& _mouse) const;
    sf::RectangleShape displayButton() const;
    sf::Text displayText() const;
    void add_letter(char&& a);
    void delete_letter();
    void changeTextColor();
    void changeTextColorBack();
    void emptytext();
    bool isempty();
    void change_text(int& score);
    std::string get_text();
};
#endif // INCLUDE_BUTTON_HPP_
