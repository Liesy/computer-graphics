#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <map>
#include "ArcBall.h"

using namespace std;

/*
关于半边数据结构
 https://blog.csdn.net/outtt/article/details/78544053?utm_medium=distribute.pc_relevant.none-task-blog-
 2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%
 7Edefault%7EBlogCommendFromBaidu%7Edefault-5.control
*/

struct HE_vert;
struct HE_face;
struct HE_edge;

struct HE_vert {
    float x;
    float y;
    float z;
    HE_edge *edge;  // one of the half-edges emantating from the vertex
};

struct HE_face {
    HE_edge *edge;  // one of the half-edges bordering the face
};

struct HE_edge {
    int id;
    HE_vert *vert;   // 半边末尾的点
    HE_edge *pair;   // 反向的相邻半边
    HE_face *face;   // 半边包围的面
    HE_edge *next;   // 下一个半边
};

class HE_off {
private:
    int vert_cnts, face_cnts, edge_cnts;
    map<pair<int, int>, HE_edge *> edge_map;
    HE_vert *HE_verts;
    HE_face *HE_faces;
    HE_edge *HE_edges;
public:
    HE_off() {}

    ~HE_off() {
        delete HE_verts;
        delete HE_faces;
        delete HE_edges;
    }


    void read_off() {
        char theOff[233] = "D:/Clion/graphics/models/bumpy.off";
        ifstream fin(theOff);
        if (!fin.is_open()) {
            puts("Error opening file.");
            exit(1);
        }
        string isOff;
        do {
            fin >> isOff;
        } while (isOff[0] == '#');
        if (isOff != "OFF") {
            puts("This is not a .off file.");
            exit(1);
        }
        fin >> this->vert_cnts >> this->face_cnts >> this->edge_cnts;
        this->HE_verts = new HE_vert[this->vert_cnts];
        for (int i = 0; i < vert_cnts; i++) {
            float a, b, c;
            fin >> a >> b >> c;
            (this->HE_verts + i)->x = a, (this->HE_verts + i)->y = b, (this->HE_verts + i)->z = c;
        }
        this->HE_faces = new HE_face[this->face_cnts];
        this->HE_edges = new HE_edge[this->face_cnts * 5];
        for (int i = 0; i < face_cnts; i++) {
            int n;
            int k[10];
            fin >> n;
            for (int j = 0; j < n; j++) {
                fin >> k[j];
            }
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < n; j++) {
                int t = (j + 1) % n;
                (this->HE_edges + i * n + j)->face = (this->HE_faces + i);
                (this->HE_edges + i * n + j)->vert = (this->HE_verts + k[t]);
                (this->HE_edges + i * n + j)->next = (this->HE_edges + i * n + t);
                (this->HE_edges + i * n + j)->id = i * n + j;
                this->edge_map[make_pair(k[j], k[t])] = (this->HE_edges + i * n + j);
                auto iter = edge_map.find(make_pair(k[t], k[j]));
                if (iter != edge_map.end()) {
                    (this->HE_edges + i * n + j)->pair = iter->second;
                    iter->second->pair = (this->HE_edges + i * n + j);
                }
                (this->HE_faces + i)->edge = (this->HE_edges + i * n + j);
                (this->HE_verts + k[j])->edge = (this->HE_edges + i * n + j);
                float a, b, c;
                a = (this->HE_verts + k[j])->x;
                b = (this->HE_verts + k[j])->y;
                c = (this->HE_verts + k[j])->z;
                glVertex3f(a, b, c);
            }
            glEnd();
        }
    }

    void bianli() {
        float a, b, c;
        a = (this->HE_verts)->x;
        b = (this->HE_verts)->y;
        c = (this->HE_verts)->z;

        //设置点的大小
        glPointSize(7);
        //进行平滑处理　
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH, GL_NICEST);

        glBegin(GL_POINTS);
        glColor3f(0, 255, 0);
        glVertex3f(a, b, c);
        glEnd();

        HE_edge *edge = (this->HE_verts)->edge;

        do {
            // do something with edge, edge->pair or edge->face
            glBegin(GL_POINTS);
            glColor3f(255, 0, 0);
            glVertex3f(edge->vert->x, edge->vert->y, edge->vert->z);
            glEnd();

            glBegin(GL_LINE_LOOP);
            glColor4ub(0, 0, 255, 255);
            glColor3f(0, 0, 255);
            glVertex3f(a, b, c);
            glVertex3f(edge->vert->x, edge->vert->y, edge->vert->z);
            glEnd();

            edge = edge->pair->next;
        } while (edge != (this->HE_verts)->edge);

    }
};


ArcBallT arcBall(600.0f, 400.0f);
ArcBallT *ArcBall = &arcBall;// new ArcBallT(600.0f,400.0f);//&arcBall;

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
    ArcBall->setBounds((GLfloat) w, (GLfloat) h); //1. 设置窗口边界
}

void init() {
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_FLAT);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 1.0);

    //glPushMatrix();
    glTranslatef(ArcBall->rightPan, ArcBall->upPan, 0);
    glScalef(ArcBall->zoomRate, ArcBall->zoomRate, ArcBall->zoomRate);//2. 缩放
    glMultMatrixf(ArcBall->Transform.M);//3. 旋转

    HE_off myoff;
    myoff.read_off();
    myoff.bianli();

    glFlush();
}

//移动
void move(int x, int y) {
    ArcBall->MousePt.s.X = x;
    ArcBall->MousePt.s.Y = y;
    ArcBall->upstate();
    glutPostRedisplay();
}

//点击
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ArcBall->isClicked = true;
        move(x, y);
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        ArcBall->isClicked = false;
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        ArcBall->isRClicked = true;
        move(x, y);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
        ArcBall->isRClicked = false;
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
        ArcBall->isRClicked = false;
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        ArcBall->isMClicked = true;
        move(x, y);
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
        ArcBall->isMClicked = false;
    ArcBall->upstate();
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("experiment_7");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutMotionFunc(move);

    glutMainLoop();

    return 0;
}
