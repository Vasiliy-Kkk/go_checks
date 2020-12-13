#include "math_functions.hpp"
bool _in_circle(int& x, int& y, int& radius, int x_m, int y_m) {
 //x_m, y_m - координаты центра, x, y - проверяемой точки
 if (radius > sqrt(pow(x - x_m, 2) + pow(y - y_m, 2))) {
     return true;
 }
 else {
     return false;
 }
}