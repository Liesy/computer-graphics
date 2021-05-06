/* 2021-4-15 created by sdu-cs-ai ly
计算机图形学实验五 实现反走样、深度缓存算法
*/

#include <GL/glut.h>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

#define WIDTH 600.0f
#define HEIGHT 600.0f

using namespace std;

int zb;
//三角形a的初始点
float a1x = 100, a1y =200, a1z = 0;
float a2x = 400, a2y = 200, a2z = 100;
float a3x = 200, a3y = 400, a3z = 100;
//三角形b的初始点
float b1x = 100, b1y = 200, b1z = 0;
float b2x = 300, b2y = 200, b2z = 0;
float b3x = 400, b3y = 400, b3z = 300;

float alpha, beta, gamma;

bool same_direction(float a, float b) {
    if ((a >= 0 && b >= 0) || (a <= 0 && b <= 0))
        return true;
    else
        return false;
}

bool in_same_side(pair<float, float> a, pair<float, float> b, pair<float, float> c) {
    int z1 = a.first * c.second - a.second * c.first;
    int z2 = b.first * c.second - b.second * c.first;
    if (same_direction(z1, z2))
        return true;
    else
        return false;
}

//判断是否在三角形 a 中
bool in_a(int x, int y) {
    pair<float, float> AP = make_pair(x - a1x, y - a1y),
            BP = make_pair(x - a2x, y - a2y),
            AB = make_pair(a2x - a1x, a2y - a1y),
            AC = make_pair(a3x - a1x, a3y - a1y),
            BA = make_pair(a1x - a2x, a1y - a2y),
            BC = make_pair(a3x - a2x, a3y - a2y);
    if (in_same_side(AP, AB, AC) && in_same_side(AP, AC, AB) && in_same_side(BP, BA, BC))
        return true;
    else
        return false;
}

//判断是否在三角形 b 中
bool in_b(int x, int y) {
    pair<float, float> AP = make_pair(x - b1x, y - b1y),
            BP = make_pair(x - b2x, y - b2y),
            AB = make_pair(b2x - b1x, b2y - b1y),
            AC = make_pair(b3x - b1x, b3y - b1y),
            BA = make_pair(b1x - b2x, b1y - b2y),
            BC = make_pair(b3x - b2x, b3y - b2y);
    if (in_same_side(AP, AB, AC) && in_same_side(AP, AC, AB) && in_same_side(BP, BA, BC))
        return true;
    else
        return false;
}

//计算三角形 a 中的深度
float deepth_a(int x, int y) {
    float xtemp = x, ytemp = y;
    gamma = ((a2x - a1x) * (ytemp - a1y) - (xtemp - a1x) * (a2y - a1y)) /
            ((a2x - a1x) * (a3y - a1y) - (a3x - a1x) * (a2y - a1y));
    beta = (xtemp - a1x - gamma * (a3x - a1x)) / (a2x - a1x);
    alpha = 1.0f - beta - gamma;
    float ztemp = alpha * a1z + beta * a2z + gamma * a3z;
    return ztemp;
}

//计算三角形 b 中的深度
float deepth_b(int x, int y) {
    float xtemp = x, ytemp = y;
    gamma = ((b2x - b1x) * (ytemp - b1y) - (xtemp - b1x) * (b2y - b1y)) /
            ((b2x - b1x) * (b3y - b1y) - (b3x - b1x) * (b2y - b1y));
    beta = (xtemp - b1x - gamma * (b3x - b1x)) / (b2x - b1x);
    alpha = 1.0f - beta - gamma;
    float ztemp = alpha * b1z + beta * b2z + gamma * b3z;
    return ztemp;
}

//初始化设置
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);//背景为黑色
    glMatrixMode(GL_PROJECTION);//设置合适的矩阵
    glLoadIdentity();
//    glPointSize(5.0);
}

//清屏
void clear_screen() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

//反走样画点
void point_paint(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();//不经过缓冲区，上面的代码立即执行
}

void Zbuffering() {
    clear_screen();
    float r, g, b;
    for (int i = 0; i <= 600; i++)
        for (int j = 0; j <= 600; j++) {
            zb = -500;
            if (in_a(i, j)) {
                float deepth = deepth_a(i, j);
                if (deepth > float(zb)) {
                    r = g = b = 0;
                    if (in_a(float(i) + 0.25, float(j) + 0.25))
                        g += 0.25;
                    if (in_a(float(i) + 0.25, float(j) - 0.25))
                        g += 0.25;
                    if (in_a(float(i) - 0.25, float(j) + 0.25))
                        g += 0.25;
                    if (in_a(float(i) - 0.25, float(j) - 0.25))
                        g += 0.25;
                    zb = int(deepth);
                }
            }
            if (in_b(i, j)) {
                float deepth = deepth_b(i, j);
                if (deepth > float(zb)) {
                    r = g = b = 0;
                    if (in_b(float(i) + 0.25, float(j) + 0.25))
                        b += 0.25;
                    if (in_b(float(i) + 0.25, float(j) - 0.25))
                        b += 0.25;
                    if (in_b(float(i) - 0.25, float(j) + 0.25))
                        b += 0.25;
                    if (in_b(float(i) - 0.25, float(j) - 0.25))
                        b += 0.25;
                    zb = int(deepth);
                }
            }
            if (zb != -500)
                point_paint(i, j, r, g, b);
        }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);

    //指定显示模式的类型
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//RGB颜色模式，单缓冲窗口

    glutCreateWindow("experiment_5");

    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    init();
    glutDisplayFunc(Zbuffering);

    glutMainLoop();
    return 0;
}