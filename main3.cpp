//
// Created by Young.L SDU on 2021/4/1.
// �ο� https://frankdura.blog.csdn.net/article/details/79559163
//
// ����㷨��https://blog.csdn.net/Daisy__Ben/article/details/51941608
//
#include <GL/glut.h>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

using namespace std;

struct line{
    int x1,y1,x2,y2;
    line(int x11,int y11,int x22,int y22){
        x1=x11;
        y1=y11;
        x2=x22;
        y2=y22;
    }
};

int flag=1;//�������㣬˫������
int x_1,y_1,x_2,y_2;
vector<line> lines;
int xleft=150,xright=450,ybuttom=150,ytop=350;//�ü����ڳ�ʼֵ

int mouse_down=1;//�Ҽ�����
int mouse_x1,mouse_y1,mouse_x2,mouse_y2;
int distance_x,distance_y;//�ƶ��ľ���

//��ʼ������
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);//����Ϊ��ɫ
    glMatrixMode(GL_PROJECTION);//���ú��ʵľ���
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);//������ɫ
    glPointSize(5.0);
}

void line_paint(int x1,int y1,int x2,int y2){
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2d(x1,600-y1);
    glVertex2d(x2,600-y2);
    glEnd();
    glFlush();
}

void rectangle_paint(){
    //����
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glBegin(GL_LINES);
    glColor3f(1.0,1.0,1.0);
    glVertex2d(xleft,600-ybuttom);
    glVertex2d(xleft,600-ytop);

    glVertex2d(xleft,600-ytop);
    glVertex2d(xright,600-ytop);

    glVertex2d(xright,600-ytop);
    glVertex2d(xright,600-ybuttom);

    glVertex2d(xright,600-ybuttom);
    glVertex2d(xleft,600-ybuttom);
    glEnd();
    glFlush();
}

//��p��q�����ж�,ͬʱ�����ж�ˢ��u1 u2, ע��u1 u2�Ǵ�ֵ����
bool clipT(float p,float q,float &u1,float &u2){
    float u;
    if(p<0.0){
        u=q/p;
        if(u>u2)
            return false;
        if(u>u1)
            u1=u;
    }
    else if(p>0){
        u=q/p;
        if(u<u1)
            return false;
        if(u<u2)
            u2=u;
    }
    else
        return(q>=0);
    return true;
}

//���Ѷ�-Barskey�㷨
void LYD_Barskey(float x1,float y1,float x2,float y2,float xl,float xr,float yb,float yt){
    float u1=0,u2=1;
    float dx=x2-x1,dy=y2-y1;
    if(clipT(-dx,x1-xl,u1,u2))
        if(clipT(dx,xr-x1,u1,u2))
            if(clipT(-dy,y1-yb,u1,u2))
                if(clipT(dy,yt-y1,u1,u2)){
                    if (u2< 1.0) {
                        x2 = x1 + u2*dx;
                        y2 = y1+  u2*dy;
                    }
                    if (u1>0.0) {
                        x1 = x1 + u1*dx;
                        y1 = y1+ u1*dy;
                    }
                    //����չʾ���ڵ���
                    glBegin(GL_LINES);
                    glColor3f(1.0,0.0,0.0);
                    glVertex2f(x1, 600-y1);
                    glVertex2f(x2, 600-y2);
                    glEnd();
                    glFlush();
                }
}

void mydisplay(){
    rectangle_paint();
    for(int i=0;i<lines.size();i++) {
        line_paint(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
        LYD_Barskey(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2, xleft, xright, ybuttom, ytop);
    }
}

//������ߣ��Ҽ��ƶ��ü�����
void mouse_click(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
        if(flag%2==0){//����
            x_2=x;
            y_2=y;
            line_paint(x_1,y_1,x_2,y_2);
            line new_line(x_1,y_1,x_2,y_2);
            lines.emplace_back(new_line);
            flag++;
        }
        else{
            x_1=x;
            y_1=y;
            flag++;
        }
    }
    if(button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN){
        if(mouse_down%2==0){
            mouse_x2=x;
            mouse_y2=y;
            distance_x = (mouse_x2-mouse_x1)*0.5;
            distance_y = (mouse_y2-mouse_y1)*0.5;
            xleft+=distance_x;
            xright+=distance_x;
            ybuttom+=distance_y;
            ytop+=distance_y;
            rectangle_paint();
            mouse_down++;
        }
        else{
            mouse_x1=x;
            mouse_y1=y;
            mouse_down++;
        }
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);

    //ָ����ʾģʽ������
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//RGB��ɫģʽ�������崰��

    glutCreateWindow("experiment_3");

    gluOrtho2D(0, 600, 0, 600);

    init();
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mouse_click);

    glutMainLoop();
    return 0;
}