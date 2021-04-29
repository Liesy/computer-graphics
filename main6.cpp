//
// Created by sdu-cs-ai ly on 2021/4/28.
// 实现bezier和b样条
// 参考glfw实现 https://blog.csdn.net/HANZY72/article/details/109555198
//

#include <GL/glut.h>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

using namespace std;

struct info{
    int x,y;
    info(){}
    info(int xx,int yy){
        x=xx;
        y=yy;
    }
};
vector<info> points;//存点
vector<vector<info>> per;
int cnt=0,k=0;//控制点个数，按下键盘的次数
bool can_draw=true;
int x11,y11,x22,y22;

//初始化设置
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);//背景为黑色
    glMatrixMode(GL_PROJECTION);//设置合适的矩阵
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);//设置颜色
    glPointSize(2);
}

//清屏
void clear_screen() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

//画点
void point_paint(int x,int y){
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2i(x,600-y);
    glEnd();
    glFlush();
}

//画线
void line_paint(int x1,int y1,int x2,int y2){
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2d(x1,600-y1);
    glVertex2d(x2,600-y2);
    glEnd();
    glFlush();
}

void bezier(){
    clear_screen();
    if(cnt<=0)
        return;
    x11=points[0].x;
    y11=points[0].y;
    point_paint(x11,y11);
    for(int i=1;i<cnt;i++){
        x22=points[i].x;
        y22=points[i].y;
        point_paint(x22,y22);
        line_paint(x11,y11,x22,y22);
        x11=x22;
        y11=y22;
    }
    for(double t=0.0;t<=1.0;t+=0.001) {
        per.clear();
        for(int i=0;i<=cnt;i++){
            vector<info> temp;
            per.emplace_back(temp);
        }
        //n-2
        for(int i=0;i<cnt-1;i++){
            double newx=(1-t)*points[i].x+t*points[i+1].x;
            double newy=(1-t)*points[i].y+t*points[i+1].y;
            per[cnt-2].emplace_back(info(newx,newy));
        }
        for(int i=cnt-3;i>=0;i--){
            for(int j=0;j<per[i+1].size()-1;j++){
                double newx=(1-t)*per[i+1][j].x+t*per[i+1][j+1].x;
                double newy=(1-t)*per[i+1][j].y+t*per[i+1][j+1].y;
                if(i==0)
                    point_paint(newx,newy);
                per[i].emplace_back(info(newx,newy));
            }
        }
    }
}

void B(){

}

void mydisplay(){}

//鼠标动作, 滚轮控制左键是加点还是拖动， 右键删点, 滚轮按下切换加点和拖动
void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(can_draw){//加点
            point_paint(x,y);
            info new_point(x,y);
            cnt++;
            points.emplace_back(new_point);
            bezier();
//            B();
        } else{//拖动

        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if(cnt!=0) {
            points.erase(points.end());
            cnt--;
        }
        bezier();
//        B();
    }
    if(button==GLUT_MIDDLE_BUTTON&&state==GLUT_DOWN)
        can_draw=!can_draw;
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);

    //指定显示模式的类型
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//RGB颜色模式，单缓冲窗口

    glutCreateWindow("experiment_6");

    gluOrtho2D(0, 600, 0, 600);

    init();
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mouse_click);

    glutMainLoop();
    return 0;
}