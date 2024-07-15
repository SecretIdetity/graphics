#ifndef OBJECTS_H
#define OBJECTS_H
#pragma once
#include "object.h"

/*class Template : public Object {
public:
    Template(double l, double x, double y, double z, double a, double b, double c, double d, double e);
    ~Template() override = default;
    std::tuple<bool, double, double, double> i(double x, double y, double z, double a, double b, double c, int mod) override;
    std::tuple<int, double, double, double, int, double> r(double x, double y, double z, double a, double b, double c, int mod) override;
};*/

class Ball: public Object {
public:
    Ball(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a);
    ~Ball() override = default;
    std::tuple<bool, double> i(double x, double y, double z, double a, double b, double c, int mod) override;
    std::tuple<int, double, double, double, int, double, double> r(int rgb, double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re) override;
};

class Triangle: public Object {
public:
    Triangle(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a, double b, double c, double d, double e, double f);
    ~Triangle() override = default;
    std::tuple<bool, double> i(double x, double y, double z, double a, double b, double c, int mod) override;
    std::tuple<int, double, double, double, int, double, double> r(int rgb, double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re) override;
    std::tuple<double, double, double> n = {0, 0, 0};
};

/*class Rectangle: public Object {
public:
    Rectangle(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a);
    ~Rectangle() override = default;s
    std::tuple<bool, double> i(double x, double y, double z, double a, double b, double c, int mod) override;
    std::tuple<int, double, double, double, int, double, double> r(double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re) override;
    std::tuple<double, double, double> n;
};*/

#endif