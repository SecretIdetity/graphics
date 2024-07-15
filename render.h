#ifndef RENDER_H
#define RENDER_H
#include "object.h"
#include <GL/gl.h>
#include <memory>
#include <mutex>
#include <tuple>
#include <vector>

void set(int x, int y, int r, int g, int b, int w, int h, GLubyte* pixels);
std::tuple<double, double, double> atox(double x, double y, std::tuple<double, double, double, double, double, double, double, double> cam, int h, int w);
void draw(GLubyte* pix, std::vector<std::unique_ptr<Object>>& obj, std::mutex& mu, int* it, std::tuple<double, double, double, double, double, double, double, double> cam, int max, int h, int w, int bac, int bc, int aa, int dif, int amt);
void render(GLubyte* pix, std::vector<std::unique_ptr<Object>>& obj, int t, int w, int h, std::tuple<double, double, double, double, double, double, double, double> cam, int m, int b, int bc, int aa, int dif, int amt);

#endif