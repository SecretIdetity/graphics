#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "load.h"
#include "objects.h"

std::vector<std::unique_ptr<Object>> load(std::string f, int* w, int* h, int* m, int* b, int* c, int* a, int* d, int* t){
    std::vector<std::unique_ptr<Object>> obj;
    std::string l;
    std::ifstream n (f);
    int i = 0;
    double j[16];
    if(n.is_open()){
        while( getline(n, l, ' ')){
            j[i] = stod(l);
            i++;
            if(i == 16){
                switch (int(j[0])){
                    case -1:
                        *w = int(j[1]); *h = j[2]; *m = j[3]; *b = j[4]; *c = j[5]; *a = j[6]; *d = j[7]; *t = j[8];
                        break;
                    case 0:
                        obj.push_back(std::unique_ptr<Object>(new Ball(j[1], j[2], j[3], j[4], j[5], j[6], j[7], j[8], j[9], j[10])));
                        break;
                    case 1:
                        obj.push_back(std::unique_ptr<Object>(new Triangle(j[1], j[2], j[3], j[4], j[5], j[6], j[7], j[8], j[9], j[10], j[11], j[12], j[13], j[14], j[15])));
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        break;
                }
                i = 0;
            }
        }
    }
    return obj;
}