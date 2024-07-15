#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <thread>
#include <cstring>
#include "load.h"
#include "render.h"
#include "object.h"

#define PI 3.14159265359
/*
README

currently this is not very optimized, working or well done
yes it very well could use a type of vec3 to not have all the doubles separated all the time

*/
int WIDTH = 1000;
int HEIGHT = 1000;
int MAX_RAY = 20;
int BACK = 0x000000;
int COL = 0;
int AA = 0;
int DIF = 5; //ray
int AMT = 1; //path
std::tuple<double, double, double, double, double, double, double, double> cam = {0, 0, 0, 0, 0, 1, 0, 90}; //xyz, abc, rot, ang
GLubyte* pixels = new GLubyte[WIDTH * HEIGHT * 3];
//GLubyte* pixel = new GLubyte[WIDTH * HEIGHT * 3];
std::vector<std::unique_ptr<Object>> obj;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //std::memcpy(pixel, pixels, WIDTH * HEIGHT * 3);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers(); 
}

int main(int argc, char *argv[])
{
    //return 0;
    std::srand(2);
    obj.clear();
    std::string i = "a";
    std::cin >> i;
    i = "./" + i + ".txt";
    obj = load(i, &WIDTH, &HEIGHT, &MAX_RAY, &BACK, &COL, &AA, &DIF, &AMT);
    //printf("%zuo\n", obj.size());
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WIDTH, HEIGHT); 
    glutInitWindowPosition(0, 0); 

    /*int MainWindow = */glutCreateWindow("graphics"); 
    glClearColor(0.0, 0.0, 0.0, 0);
    render(pixels, obj, 4, WIDTH, HEIGHT, cam, MAX_RAY, BACK, COL, AA, DIF, AMT);
    glutDisplayFunc(display); 
    glutMainLoop();
    return 0;
}
//cheers to https://stackoverflow.com/a/52695129