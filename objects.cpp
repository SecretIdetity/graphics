#include "objects.h"
#include "ray.h"
//#include "float.h"
#include <cstdio>
#include <cstdlib>
#include <math.h>

#define PI 3.14159265359

Ball::Ball(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a): Object(col, lig, dif, ref, bre, ind, x, y, z, a, 0, 0, 0, 0, 0) {}

std::tuple<bool, double> Ball::i(double xx, double yy, double zz, double aa, double bb, double cc, int mod){
    double xxx = xx - x;
    double yyy = yy - y;
    double zzz = zz - z;
    double ka = (aa*aa + bb*bb + cc*cc);
    double kb = (2*xxx*aa + 2*yyy*bb + 2*zzz*cc);
    double k = (kb * kb - 4 * (xxx*xxx + yyy*yyy + zzz*zzz - 4 * a * a) * ka);
    if(k >= 0){
        k = sqrtf(k);
        double k1 = (-kb + k) / (2 * ka);
        double k2 = (-kb - k) / (2 * ka);
        //printf("%f %f %f %f %f %f %f %f %f %f %f %db\n", aa, bb, cc, xxx, yyy, zzz, ka, kb, k, k1, k2, k1 <= 0.000001);
        //int Digs = DECIMAL_DIG;
        //printf("%.*ekunt\n", Digs, k1);
        if(k1 < k2 && k1 > 0.0000001)
            return {1, k1};
        if (k2 > 0.0000001)
            return {1, k2};
    }
    return {0, 0};
}

std::tuple<int, double, double, double, int, double, double> Ball::r(int rgb, double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re){
    int coll = rgb == 0 ? (col & 0xff0000) / 0x10000 : rgb == 1 ? (col & 0x00ff00) / 0x100 : col & 0x0000ff;
    coll *= lig;
    if(ref == 0 && dif == 0 && bre == 0)
        return {0, 0, 0, 0, coll, 1, re};
    double ax = xxx - x;
    double ay = yyy - y;
    double az = zzz - z;
    double tot = ref + dif + bre;
    //col = az * 255 + ay * 255 * 255 + ax * 255 * 255 * 255;
    double r = (rand() * (tot)) / RAND_MAX;
    if(r < ref){ //reflect
        std::tie(ax, ay, az) = normal(a, b, c, ax, ay, az);
        //printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %fd\n", xxx, yyy, zzz, x, y, z, xx, yy, zz, ax, ay, az, ref, col, n);
        //temp bc not intelligent, just brute force :/
        return {dif > 0 || (ref != tot && bre != tot) ? 2 : 1, ax, ay, az, coll, (1 - ref - dif - bre), re};
    }else if(r < ref + dif){ //diffuse
        std::tie(ax, ay, az) = norm(ax, ay, az);
        double bx, by, bz;
        std::tie(bx, by, bz) = ang(std::rand() / (float(RAND_MAX) / 180) - 90, acos(std::rand() / (float(RAND_MAX) / 2) - 1) / (PI / 180) - 90);
        //printf("%f, %f, %f", bx, by, bz);
        std::tie(ax, ay, az) = trans(ax, ay, az, bx, by, bz);
        return {2, ax, ay, az, coll, (1 - ref - dif - bre), re};
    }else{ //refract
        std::tie(ax, ay, az, re) = rr(a, b, c, ax, ay, az, re, ind);
        return {dif > 0 || (ref != tot && bre != tot) ? 2 : 1, ax, ay, az, coll, (1 - ref - dif - bre), re};
    }
}
//xx*xx + yy*yy + zz*zz - rad * rad + (2*xx*a + 2*yy*b + 2*zz*c) * f + (a*a + b*b + c*c) * ff == 0
//x = (-b +- Vb*b-4ac)/2a
//r=d−2(d⋅n)n


Triangle::Triangle(int col, double lig, double dif, double ref, double bre, double ind, double x, double y, double z, double a, double b, double c, double d, double e, double f): Object(col, lig, dif, ref, bre, ind, x, y, z, a, b, c, d, e, f) {
    double x1 = a - x;
    double y1 = b - y;
    double z1 = c - z;
    double x2 = d - x;
    double y2 = e - y;
    double z2 = f - z;
    double x3, y3, z3;
    std::tie(x3, y3, z3) = cross(x1, y1, z1, x2, y2, z2);
    n = {x3, y3, z3}; //copied from Triangle::i()
}


std::tuple<bool, double> Triangle::i(double xx, double yy, double zz, double aa, double bb, double cc, int mod){
    //https://stackoverflow.com/a/42752998
    double x1 = a - x;
    double y1 = b - y;
    double z1 = c - z;
    double x2 = d - x;
    double y2 = e - y;
    double z2 = f - z;
    double x3, y3, z3, x4, y4, z4, x5, y5, z5, u, v, k; //very bad but meh
    std::tie(x3, y3, z3) = cross(x1, y1, z1, x2, y2, z2);
    double id = 1 / (-dot(aa, bb, cc, x3, y3, z3));
    if(id > 1000000)
        return {0, 0};
    x4 = xx - x;
    y4 = yy - y;
    z4 = zz - z;
    std::tie(x5, y5, z5) = cross(x4, y4, z4, aa, bb, cc);
    u = dot(x2, y2, z2, x5, y5, z5) * id;
    v = -dot(x1, y1, z1, x5, y5, z5) * id;
    k = dot(x4, y4, z4, x3, y3, z3) * id;
    if(id < 1000000 && k > 0.0000001 && u >= 0 && v >= 0 && (u + v) <= 1)
        return {1, k};
    return {0, 0};
}


std::tuple<int, double, double, double, int, double, double> Triangle::r(int rgb, double xx, double yy, double zz, double a, double b, double c, double xxx, double yyy, double zzz, int mod, double re){
    int coll = rgb == 0 ? (col & 0xff0000) / 0x10000 : rgb == 1 ? (col & 0x00ff00) / 0x100 : col & 0x0000ff;
    coll *= lig;
    if(ref == 0 && dif == 0 && bre == 0)
        return {0, 0, 0, 0, coll, 1, re};
    double nx, ny, nz;
    std::tie(nx, ny, nz) = n;
    double tot = ref + dif + bre;
    double r = (rand() * (tot)) / RAND_MAX;
    if(r < ref){ //reflect
        std::tie(nx, ny, nz) = normal(a, b, c, nx, ny, nz);
        //printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %fd\n", xxx, yyy, zzz, x, y, z, xx, yy, zz, nx, ny, nz, ref, col, n);
        //temp bc not intelligent, just brute force :/
        return {dif > 0 || (ref != tot && bre != tot) ? 2 : 1, nx, ny, nz, coll, (1 - ref - dif - bre), re};
    }else if(r < ref + dif){ //diffuse
        double bx, by, bz;
        std::tie(bx, by, bz) = ang(std::rand() / (float(RAND_MAX) / 180) - 90, acos(std::rand() / (float(RAND_MAX) / 2) - 1) / (PI / 180) - 90);
        //printf("%f, %f, %f", bx, by, bz);
        std::tie(nx, ny, nz) = trans(nx, ny, nz, bx, by, bz);
        return {2, nx, ny, nz, coll, (1 - ref - dif - bre), re};
    }else{ //refract
        std::tie(nx, ny, nz, re) = rr(a, b, c, nx, ny, nz, re, ind);
        return {dif > 0 || (ref != tot && bre != tot) ? 2 : 1, nx, ny, nz, coll, (1 - ref - dif - bre), re};
    }
}