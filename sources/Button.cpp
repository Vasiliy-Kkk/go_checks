#include "Button.hpp"
Button::Button(double&& _x_t, double&& _y_t, std::string&& _text, int&& _text_size) {
    x_t = _x_t;
    y_t = _y_t;
    
    text_size = _text_size;
    font.loadFromFile("fonts/RodchenkoBTT.ttf"); //��������� �����
    text.setString(_text);						// ��������� �����
    text.setFont(font);							// ������������� �����
    text.setCharacterSize(_text_size); 					// ������������� ������ ������

    width = text.getLocalBounds().width;
    height = text.getLocalBounds().height;

    text.setColor(sf::Color::Red);					// ������������� ���� ������
    text.setPosition(x_t - width / 2, y_t - height);						// ������������� ���������� ������

    box.setSize(sf::Vector2f(width, height));		// ������������� ������ ������
    box.setPosition(x_t - width / 2, y_t - 3 * height / 5);						// ������������� ���������� ������
    box.setColor(sf::Color::White);	// ������������� ���� ������
    box.setOutlineThickness(2);					// ������������� ������
    box.setOutlineColor(sf::Color(66, 66, 66));		// ������������� ���� �������
}
Button::Button(double&& _x_t, double&& _y_t, int&& _text_size) {
    x_t = _x_t;
    y_t = _y_t;

    font.loadFromFile("fonts/RodchenkoBTT.ttf"); //��������� �����
    text.setString("");
    text.setFont(font);							// ������������� �����
    text.setCharacterSize(_text_size); 					// ������������� ������ ������

    width = text.getLocalBounds().width;
    height = text.getLocalBounds().height;

    text.setColor(sf::Color::Red);					// ������������� ���� ������
    text.setPosition(x_t - width / 2, y_t - height);						// ������������� ���������� ������

    box.setSize(sf::Vector2f(width, height));		// ������������� ������ ������
    box.setPosition(x_t - width / 2, y_t - 3 * height / 5);						// ������������� ���������� ������
    box.setColor(sf::Color(240, 100, 100));	// ������������� ���� ������
    box.setOutlineThickness(2);					// ������������� ������
    box.setOutlineColor(sf::Color(66, 66, 66));		// ������������� ���� �������
}
Button::Button(double&& _x_t, double&& _y_t, int& score, int&& _text_size) {
    x_t = _x_t;
    y_t = _y_t;

    font.loadFromFile("fonts/RodchenkoBTT.ttf"); //��������� �����
    str_text = std::to_string(score);
    text.setString(str_text);
    text.setFont(font);							// ������������� �����
    text.setCharacterSize(_text_size); 					// ������������� ������ ������

    width = text.getLocalBounds().width;
    height = text.getLocalBounds().height;

    text.setColor(sf::Color::Red);					// ������������� ���� ������
    text.setPosition(x_t - width / 2, y_t - height);						// ������������� ���������� ������

    box.setSize(sf::Vector2f(width, height));		// ������������� ������ ������
    box.setPosition(x_t - width / 2, y_t - 3 * height / 5);						// ������������� ���������� ������
    box.setColor(sf::Color(240, 100, 100));	// ������������� ���� ������
    box.setOutlineThickness(2);					// ������������� ������
    box.setOutlineColor(sf::Color(66, 66, 66));		// ������������� ���� �������
}

//�������� ������� �� ������
bool Button::ifpress(sf::Vector2i&& _mouse) const{
    if ((_mouse.x > (x_t - width / 2) && _mouse.x < (x_t + width / 2)) && (_mouse.y > (y_t - 3 * height / 5) && _mouse.y < (y_t + 2 * height / 5))) {
        return  true;
    }
    else {
        return false;
    }
}
//����� ������
sf::RectangleShape Button::displayButton() const{
    return box;
}
sf::Text Button::displayText() const{
    return text;
}
void Button::add_letter(char&& a) {
    if (str_text.length() < 7) {
        str_text += a;
        text.setString(str_text);
    }
}
void Button::delete_letter() {
    if (!str_text.empty()) {
        str_text.pop_back();
        text.setString(str_text);
    }
}
void Button::changeTextColor() {
    text.setColor(sf::Color(240, 100, 100));
}
void Button::changeTextColorBack() {
    text.setColor(sf::Color::Red);
}
void Button::emptytext() {
    str_text.clear();
    text.setString("");
}
bool Button::isempty() {
    return str_text.empty();
}
void Button::change_text(int& score) {
    str_text = std::to_string(score);
    text.setString(str_text);
}
std::string Button::get_text() {
    return str_text;
}