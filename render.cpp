#include <GL/gl.h>
#include <cstdio>
#include <memory>
#include <tuple>
#include <vector>
#include <thread>
#include <mutex>
#include "object.h"
#include "ray.h"
#include "render.h"

void set(int x, int y, int r, int g, int b, int w, int h, GLubyte* pixels){
    if (0 <= x && x < w && 0 <= y && y < h) {
        int position = (x + y * w) * 3;
        pixels[position] = r;
        pixels[position + 1] = g;
        pixels[position + 2] = b;
    }
}

std::tuple<double, double, double> atox(double x, double y, std::tuple<double, double, double, double, double, double, double, double> cam, int h, int w){
    //do some converting
    return {2 * x / w - 1, 2 * y / h - 1, 1};
}

void draw(GLubyte* pix, std::vector<std::unique_ptr<Object>>& obj, std::mutex& mu, int* it, std::tuple<double, double, double, double, double, double, double, double> cam, int max, int h, int w, int bac, int bc, int aa, int dif, int amt){
    while(true){
        int ax = 1;
        mu.lock();
        int y = it[0];
        if(y >= h * w){
            mu.unlock();
            return;
        }
        it[0]++;
        mu.unlock();
        if(y % 100 == 0)
            printf("%.1f\n", 100 * float(y) / (h * w)); //\r
            //printf("%d/%d %.1f\n", y, h * w, 100 * float(y) / (h * w)); //\r
        int x = y % h;
        y = (y - x) / w;
        double p[32];
        for(int pp = 0; pp < 32; pp++)
            p[pp] = 0;
        switch(aa){ //aa is broken a bit due to wrong scaling :/
            case 0: default:
                ax = 1;
                break;
            case 1:
                ax = 2;
                p[1] = -0.25; p[3] = 0.25;
                break;
            case 2:
                ax = 2;
                p[0] = -0.25; p[2] = 0.25;
                break;
            case 3:
                ax = 4;
                p[0] = -0.25; p[2] = 0.25; p[5] = -0.25; p[7] = 0.25;
                break;
            case 4:
                ax = 4;
                p[0] = -0.25; p[1] = -0.25; p[2] = -0.25; p[3] = 0.25; p[4] = 0.25; p[5] = -0.25; p[6] = 0.25; p[7] = 0.25;
                break;
            case 5:
                ax = 16;
                p[0] = 0.125; p[1] = 0.125; p[2] = 0.125; p[3] = 0.375; p[4] = 0.375; p[5] = 0.125; p[6] = 0.375; p[7] = 0.375;
                p[8] = -0.125; p[9] = 0.125; p[10] = -0.125; p[11] = 0.375; p[12] = -0.375; p[13] = 0.125; p[14] = -0.375; p[15] = 0.375;
                p[16] = 0.125; p[17] = -0.125; p[18] = 0.125; p[19] = -0.375; p[20] = 0.375; p[21] = -0.125; p[22] = 0.375; p[23] = -0.375;
                p[24] = -0.125; p[25] = -0.125; p[26] = -0.125; p[27] = -0.375; p[28] = -0.375; p[29] = -0.125; p[30] = -0.375; p[31] = -0.375;
                break;
        }
        double a, b, c;
        long long r = 0, g = 0, l = 0;
        for(int i = 0; i < ax; i++){
            for(int j = 0; j < amt; j++){
                std::tie(a, b, c) = atox(x + p[(i) * 2], y + p[(i) * 2 + 1], cam, h, w);
                int m, n, o;
                bool di;
                std::tie(m, di) = ray(obj, 0, 0, 0, 0, a, b, c, 0, max, dif, 0, bac, bc, dif, 1);
                std::tie(n, di) = ray(obj, 1, 0, 0, 0, a, b, c, 0, max, dif, 0, bac, bc, dif, 1);
                std::tie(o, di) = ray(obj, 2, 0, 0, 0, a, b, c, 0, max, dif, 0, bac, bc, dif, 1);
                //printf("%d", di);
                if(di){
                    r += m;
                    g += n;
                    l += o;
                }
                else{
                    r += m * amt;
                    g += n * amt;
                    l += o * amt;
                    break;
                }
            }
        }
        r = r / (ax * amt);
        g = g / (ax * amt);
        l = l / (ax * amt);
        //if(r || g || l)
        //    printf("%lld %lld %lld\n", r, g, l);
        double s = 0xff;
        if(r > 0xff && r > s)
            s = r;
        if(g > 0xff && g > s)
            s = g;
        if(l > 0xff && l > s)
            s = l;
        s = 0xff / s;
        r = r * s;
        g = g * s;
        l = l * s;
        set(x, y, r, g, l, w, h, pix);
    }
} 

void render(GLubyte* pix, std::vector<std::unique_ptr<Object>>& obj, int t, int w, int h, std::tuple<double, double, double, double, double, double, double, double> cam, int m, int b, int bc, int aa, int dif, int amt){
    std::mutex mu;
    int it = 0;
    int hw = std::thread::hardware_concurrency();
    std::vector<std::thread>threads(hw);
    for(int i = 0; i < hw; i++)
        threads[i] = std::thread(draw, pix, std::ref(obj), std::ref(mu), &it, cam, m, h, w, b, bc, aa, dif, amt);
    for(int i = hw - 1; i >= 0; i--)
        threads[i].join();
    //    threads[i].detach();
    printf("done :)\n");
}