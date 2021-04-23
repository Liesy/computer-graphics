/* 2021-4-15 created by sdu-cs-ai ly
计算机图形学实验五 实现反走样、改进的深度缓存算法(仅用一个深度缓存变量)
参考1 关于反走样
https://blog.csdn.net/weixin_34128411/article/details/92572899?ops_request_misc=&request_id=&biz_id=102&utm_term=opengl%E5%AE%9E%E7%8E%B0%E5%A4%9A%E9%9D%A2%E4%BD%93%E5%8F%8D%E8%B5%B0%E6%A0%B7&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-4-92572899.first_rank_v2_pc_rank_v29

*/

#include <GL/glut.h>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

using namespace std;

float times=1;//放大、缩小倍数
float mousex,mousey;//鼠标位置

//初始化设置
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);//背景为黑色
    glMatrixMode(GL_PROJECTION);//设置合适的矩阵
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);//设置颜色
    glPointSize(5.0);
}

//清屏
void clear_screen(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

//反走样画点
void point_paint(int x, int y) {
    glEnable(GL_POINT_SMOOTH);//开启抗锯齿
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//点的反走样, GL_NICEST为给出最高质量的选择

    glPointSize(2);
    glScalef(times, times, times);//缩放
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();

    glFlush();//不经过缓冲区，上面的代码立即执行
}

//反走样画线
void line_paint(int x1, int y1, int x2, int y2) {
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLineWidth(1.5);
    glScalef(times, times, times);//缩放
    glBegin(GL_LINES);
    glVertex2f(x1, 600 - y1);
    glVertex2f(x2, 600 - y2);
    glEnd();

    glFlush();
}

void mydisplay(){
    clear_screen();
    line_paint(100,100,400,400);
    glFlush();
}

//鼠标滚轮缩放操作
void mouse_click(int button, int state, int x, int y) {
    //左键变大
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        times = 0.05f+1;
        mydisplay();
    }
    //右键变小
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        times = -0.05f+1;
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