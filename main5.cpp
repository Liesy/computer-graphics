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

using namespace std;

float times = 1;//放大、缩小倍数
float mousex, mousey;//鼠标位置

float zb[601][601];
float zc[601][601];
//三角形a的初始点
float a1x = 100, a1y = 100, a1z = 1;
float a2x = 200, a2y = 100, a2z = -0.5;
float a3x = 150, a3y = 250, a3z = -0.8;
//三角形b的初始点
float b1x = 150, b1y = 150, b1z = -0.5;
float b2x = 250, b2y = 150, b2z = 1;
float b3x = 200, b3y = 300, b3z = 0;

float alpha, beta, gamma;

//判断是否在三角形 a 中
bool in_a(int x, int y) {
    if (y >= 100 && (3 * x - y - 200 >= 0) && (3 * x + y - 700 <= 0))
        return true;
    else
        return false;
}

//判断是否在三角形 b 中
bool in_b(int x, int y) {
    if (y >= 150 && (3 * x - y - 300 >= 0) && (3 * x + y - 900 <= 0))
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
    glColor3f(1.0, 1.0, 1.0);//设置颜色
    glPointSize(5.0);
}

//清屏
void clear_screen() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

//反走样画点
void point_paint(int x, int y, int z) {
    glEnable(GL_POINT_SMOOTH);//开启抗锯齿
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//点的反走样, GL_NICEST为给出最高质量的选择

//    glPointSize(2);
    glScalef(times, times, times);//缩放
    glBegin(GL_POINTS);
    glVertex3i(x, y, z);
    glEnd();

    glFlush();//不经过缓冲区，上面的代码立即执行
}

void mydisplay() {
    clear_screen();
    for(int i=0;i<=600;i++)
        for(int j=0;j<=600;j++){
            zb[i][j]=-1;
            zc[i][j]=1;
            if(in_a(i,j)){
                float adeepth= deepth_a(i,j);
                if(adeepth>zb[i][j]){
                    zb[i][j]=adeepth;
                    zc[i][j]=2;
                }
            }
            if(in_b(i,j)){
                float bdeepth= deepth_b(i,j);
                if(bdeepth>zb[i][j]){
                    zb[i][j]=bdeepth;
                    zc[i][j]=3;
                }
            }
        }
    for(int i=0;i<=600;i++)
        for(int j=0;j<=600;j++){
            if(zc[i][j]==1)
                continue;
            else if(zc[i][j]==2)
                glColor3f(0.1f,0.7f,0.3f);
            else if(zc[i][j]==3)
                glColor3f(0.7f,0.4f,0.9f);

            point_paint(i,j,zb[i][j]);
        }
}

//鼠标滚轮缩放操作
void mouse_click(int button, int state, int x, int y) {
    //左键变大
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        times = 0.05f + 1;
        mydisplay();
    }
        //右键变小
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        times = -0.05f + 1;
        mydisplay();
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);

    //指定显示模式的类型
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//RGB颜色模式，单缓冲窗口

    glutCreateWindow("experiment_5");

    gluOrtho2D(0, 600, 0, 600);

    init();
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mouse_click);

    glutMainLoop();
    return 0;
}