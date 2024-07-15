#ifndef OBJECT_H
#define OBJECT_H
#include <tuple>

class Object {
    public:
        Object(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a, double b, double c, double d, double e, double f);
        virtual ~Object() = default;
        virtual std::tuple<bool, double> i(double xx, double yy, double zz, double a, double b, double c, int mod) = 0;
        virtual std::tuple<int, double, double, double, int, double, double> r(int rgb, double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re) = 0;
    //protected:
        int col;
        double lig;
        double dif;
        double ref;
        double bre;
        double ind;
        double x;
        double y;
        double z;
        double a;
        double b;
        double c;
        double d;
        double e;
        double f;
};

#endif 
