#ifndef RAY_H
#define RAY_H
#include <memory>
#include <tuple>
#include <vector>

class Object;

//int mix(int a, int b, double c);
//std::tuple<int, double> mix(std::tuple<int, double> a, int b, double c);
//std::tuple<int, int, int> mix(int a, int b, int c, int d, double e);
//int col(int r, int g, int b);
//std::tuple<int, int, int> col(int c);
std::tuple<double, double, double> trans(double x, double y, double z, double a, double b, double c);
std::tuple<double, double, double> ang(double x, double y);
std::tuple<double, double, double, double> rr(double x, double y, double z, double a, double b, double c, double ia, double ib);
std::tuple<double, double, double> norm(double x, double y, double z);
std::tuple<double, double, double> normal(double x, double y, double z, double a, double b, double c);
std::tuple<double, double, double> cross(double x, double y, double z, double a, double b, double c);
double dot(double x, double y, double z, double a, double b, double c);
double dist(double x, double y, double z, double a, double b, double c);
std::tuple<int, bool> ray(std::vector<std::unique_ptr<Object>>& obj, int rgb, double x, double y, double z, double a, double b, double c, int itr, int max, int sub, int mod, int bac, int bc, int dif, double re);

#endif