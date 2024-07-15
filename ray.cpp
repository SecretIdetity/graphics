#include "ray.h"
#include "object.h"
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <tuple>

#define PI 3.14159265359

/*int col(int r, int g, int b){
    return r * 0x10000 + g * 0x100 + b;
}

std::tuple<int, int, int>col(int c){
    return {(c & 0xff0000) / 0x10000, (c & 0xff00) / 0x100, c & 0xff};
}*/

std::tuple<double, double, double> trans(double x, double y, double z, double a, double b, double c){
    //note that this has an arbitrary rotation, which for the purposes here (random and deterministic for the same normal) is fine (1 0 0 = x y z)
    if(std::abs(x) < 0.000000001 && std::abs(y) < 0.000000001 && std::abs(z) < 0.000000001)
        return {0, 0, 0};
    double ax, ay, az, bx, by, bz;
    if(std::abs(x) < 0.000000001){
        ax = 1; ay = 0; az = 0;
    }else if(std::abs(y) < 0.000000001){
        ax = 0; ay = 1; az = 0;
    }else if(std::abs(z) < 0.000000001){
        ax = 0; ay = 0; az = 1;
    }else{
        ax = x;
        ay = -y;
        az = (y * y - x * x) / z;
    }
    std::tie(ax, ay, az) = norm(ax, ay, az);
    std::tie(bx, by, bz) = cross(x, y, z, ax, ay, az);
    std::tie(bx, by, bz) = norm(bx, by, bz); //no idea id cross is norm
    //printf("%f %f %f %f %f %f", ax, ay, az, bx, by, bz);
    return {x * a + ax * b + bx * c, y * a + ay * b + by * c, z * a + az * b + bz * c};
}

std::tuple<double, double, double> ang(double x, double y){
    x = x * PI / 180;
    y = y * PI / 180;
    return {cos(x) * cos(y), sin(x) * cos(y), sin(y)};
}

std::tuple<double, double, double, double> rr(double x, double y, double z, double a, double b, double c, double ia, double ib){
    //cheers to https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
    std::tie(x, y, z) = norm(x, y, z); //v2 v2 0
    std::tie(a, b, c) = norm(a, b, c); //-1 0 0 //-1 -.5 0
    double d = -dot(a, b, c, x, y, z); //v2
    //temp bc doing properly would be more painful, its just gonna be wierd when a refr obj is in a refr obj
    if(d < 0 || ia == ib)
        ib = 1;
    double n = ia / ib; //0.5
    double sst = n * n * (1 - d * d); //-.25
    //printf("%f %f %f %f %f %f %f %f %f %f %f %f\n", x, y, z, a, b, c, ia, ib, n, d, d, sst);
    if(sst <= 1)
        return {n * x + (n * d - std::sqrt(1 - sst)) * a, n * y + (n * d - std::sqrt(1 - sst)) * b, n * z + (n * d - std::sqrt(1 - sst)) * c, ib};
    std::tie(x, y, z) = normal(x, y, z, a, b, c);
    return {x, y, z, ia};
}

/*int mix(int a, int b, double c){
    double d = 1 - c;
    int aa, ab, ac, ba, bb, bc;
    std::tie(aa, ab, ac) = col(a);
    std::tie(ba, bb, bc) = col(b);
    return col(aa * d + ba * c, ab * d + bb * c, ac * d + bc * c);
}

std::tuple<int, double> mix(std::tuple<int, double> a, int b, double c){
    int d;
    double e;
    std::tie(d, e) = a;
    return {mix(d, b, c), e};
}

std::tuple<int, int, int> mix(int a, int b, int c, int d, double e){
    double f = 1 - e;
    int aa, ab, ac;
    std::tie(aa, ab, ac) = col(d);
    return {a * f + aa * c, b * f + ab * c, c * f + ac * c};
}*/

double dot(double x, double y, double z, double a, double b, double c){
    return(x * a + y * b + z * c);
}

std::tuple<double, double, double> norm(double x, double y, double z){
    double n = dist(x, y, z, 0, 0, 0);
    return {x / n, y / n, z / n};
}

std::tuple<double, double, double> normal(double x, double y, double z, double a, double b, double c){
    std::tie(a, b, c) = norm(a, b, c);
    double aa = dot(x, y, z, a, b, c);
    if(aa < 0){
        aa = -aa;
        a = -a;
        b = -b;
        c = -c;
    }
    return {x - 2 * aa * a, y - 2 * aa * b, z - 2 * aa * c};
}

std::tuple<double, double, double> cross(double x, double y, double z, double a, double b, double c){
    return {y * c - z * b, z * a - x * c, x * b - y * a};
}

double dist(double x, double y, double z, double a, double b, double c){
    return std::sqrt((x - a) * (x - a) + (y - b) * (y - b) + (z - c) * (z - c));
}

std::tuple<int, bool> ray(std::vector<std::unique_ptr<Object>>& obj, int rgb, double x, double y, double z, double a, double b, double c, int itr, int max, int sub, int mod, int bac, int bc, int dif, double re){
    bool in, di = 0;
    double k, aa, bb, cc, per, kk, ree;
    int j, ref, sr = 1, cl = bac; //background
    kk = INFINITY;
    for(std::size_t i = 0; i < obj.size(); i++){
        std::tie(in,k) = obj[i].get()->i(x, y, z, a, b, c, mod);
        if(!in)
            continue;
        if(k == 0){
            std::tie(in,k) = obj[i].get()->i(x + a * 0.001, y + b * 0.001, z + c * 0.001, a, b, c, mod);
            if(!in)
                continue;
            if(k < kk){
                j = i;
                kk = k;
            }
        }
        if(k < kk){
            j = i;
            kk = k;
        }
    }
    if(kk != INFINITY){
        double xx = a * kk + x;
        double yy = b * kk + y;
        double zz = c * kk + z;
        int cr = 0;
        std::tie(ref, aa, bb, cc, cl, per, ree) = obj[j].get()->r(rgb, x, y, z, a, b, c, xx, yy, zz, mod, re);
        //printf("x:%f y:%f z:%f a:%f b:%f c:%f kk:%f xx:%f yy:%f zz:%f aa:%f bb:%f cc:%f ref:%d cl:%d per:%f itr:%dc\n", x, y, z, a, b, c, kk, xx, yy, zz, aa, bb, cc, ref, cl, per, itr);
        if(itr == max || ref == 0)
            return {cl, 0};
        if(ref == 2)
            sr = sub; //NO SUPPORT FOR REFLECTION AND REFRACTION AT THE SAME TIME (intelligently), WOULD HAVE TO DO SAME AS DIFFUSION AKA BRUTE FORCE
        int ra;
        std::tie(ra, di) = ray(obj, rgb, xx, yy, zz, aa, bb, cc, itr + 1, max, sub, mod, bac, bc, dif, ree);
        cr += ra;
        sr--;
        while(sr > 0){
            std::tie(ref, aa, bb, cc, cl, per, ree) = obj[j].get()->r(rgb, x, y, z, a, b, c, xx, yy, zz, mod, re);
            //printf("x:%f y:%f z:%f a:%f b:%f c:%f kk:%f xx:%f yy:%f zz:%f aa:%f bb:%f cc:%f ref:%d cl:%d per:%f itr:%dc\n", x, y, z, a, b, c, kk, xx, yy, zz, aa, bb, cc, ref, cl, per, itr);
            std::tie(ra, di) = ray(obj, rgb, xx, yy, zz, aa, bb, cc, itr + 1, max, sub, mod, bac, bc, dif, ree);
            cr += ra;
            sr--;
        }
        di = ref == 2 ? 1 : di;
        return {cr * cl / 0xff, di};
    }
    return {rgb == 0 ? (bac & 0xff0000) / 0x10000 : rgb == 1 ? (bac & 0x00ff00) / 0x100 : bac & 0x0000ff, 0};
}