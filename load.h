#ifndef LOAD_H
#define LOAD_H
#include "object.h"
#include <memory>
#include <string>
#include <vector>

std::vector<std::unique_ptr<Object>> load(std::string f, int* w, int* h, int* m, int* b, int* c, int* a, int* d, int* t);

#endif