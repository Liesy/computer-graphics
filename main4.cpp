//实验四
//理论参考https://blog.csdn.net/DUGUjing/article/details/83049407
#include <GL/glut.h>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <cmath>

using namespace std;

//初始化设置
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);//背景为黑色
    glMatrixMode(GL_PROJECTION);//设置合适的矩阵
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);//设置颜色
    glPointSize(5.0);
}

//画点
void point_paint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();

    glFlush();//不经过缓冲区，上面的代码立即执行
}

//边的信息
struct edge {
    float x, y, delta_x, ymax;

    //输入边顶点坐标
    edge(float x1, float y1, float x2, float y2) {
        if (y1 == y2) {//单独处理横线
            y = y1;
            ymax = y1;
            if (x1 < x2) {
                delta_x = x1;
                x = x2;
            } else {
                delta_x = x2;
                x = x1;
            }
        } else if (y2 < y1) {
            x = x2;
            y = y2;
            ymax = y1;
            delta_x = (x2 - x1) / (y2 - y1);
        } else {
            x = x1;
            y = y1;
            ymax = y2;
            delta_x = (x2 - x1) / (y2 - y1);
        }
    }
};

int hmax, hmin;//扫描线的开始和结束位置
list<edge> AET;//活性边表
vector<vector<edge>> NET;//新边表
map<int, int> mapper;//对y进行处理
int x_1, y_1, x_2, y_2;//构成边的两个端点
int x_0, y_0;//图形开始的位置
float lmax, lmin;//画线开始和结束的位置
int flag = 1;//记录点击次数，单次画点，双次画线
bool can_draw = true;//再次点击鼠标时不再更改信息

int level = 1;
vector<vector<edge>> net2;//新边表的copy

//将活性边表中的line按照x值进行排序
bool AET_lefter(const edge &l1, const edge &l2) {
    return l1.x < l2.x;
}

//将新边表中的直线按照y值进行排序；
bool higher(const vector<edge> &l1, const vector<edge> &l2) {
    //注意我们的窗口是从上到下不断递增从左到右不断递增
    return l1[0].y < l2[0].y;
}

//将新边表中的直线按照x值或dx值进行排序
bool lefter(const edge &l1, const edge &l2) {
    if (l1.x < l2.x) {
        return 1;
    } else if (l1.x == l2.x) {
        if (l1.delta_x < 0 && l2.delta_x > 0)
            return 1;
        else
            return 0;
    } else
        return 0;
}

//对新边表进行排序处理，按y从小到大，x从左到右
void sort_net() {
    for (int i = 0; i < NET.size(); i++)
        if (NET[i].size() >= 2)
            sort(NET[i].begin(), NET[i].end(), lefter);//按x从左到右排
    for (int i = 0; i < NET.size(); i++) {
        vector<edge> a;
        for (int j = 0; j < NET[i].size(); j++)
            a.push_back(NET[i][j]);
        net2.push_back(a); //复制一份新边表
    }
    sort(NET.begin(), NET.end(), higher);//按y从小到大排
}

//填充
void draw_lines(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, 600 - y1);
    glVertex2f(x2, 600 - y2);
    glEnd();

    glFlush();
}

//扫描多边形
void polygon() {
    if (NET.empty())
        return;
    int scan_line;
//    map<int, int>::iterator it_map;
    for (scan_line = hmin; scan_line <= hmax; scan_line++) {
        int posi = mapper[scan_line];//确定扫描线是否到某条新边的开始
        if (!posi) {
            //没到线的尽头，只需要画当前线，更新当前线的信息
            float x11 = 0.0;
            flag = 1;//单数画点双数画线
            for (auto it_aet = AET.begin(); it_aet != AET.end();) {
                if (flag % 2 == 0) {
                    //画线
                    draw_lines(x11, scan_line, it_aet->x, scan_line);
                    //检查是否需要删除AET中的边
                    for (auto it_exam = AET.begin(); it_exam != AET.end(); )
                        if (it_exam->ymax == scan_line)
                            AET.erase(it_exam++);
                        else
                            it_exam++;
                    it_aet->x += it_aet->delta_x;
                } else {
                    //画点
                    if (it_aet->y == it_aet->ymax)
                        x11 = x_1;
                    else {
                        x11 = it_aet->x;
                        it_aet->x += it_aet->delta_x;
                    }
                }
                flag++;
                it_aet++;
            }
        } else {
            //某条线到尽头了，要加线删线
            list<edge>::iterator it_aet;
            float x11 = 0.0;
            int counter = 1;//和flag一个作用
            for (it_aet = AET.begin(); it_aet != AET.end(); it_aet++) {
                edge temp = *it_aet;//存储当前边
                if (counter % 2 == 0)
                    draw_lines(x11, scan_line, temp.x, scan_line);
                else
                    x11 = temp.x;
                counter++;
            }
            //活性边表中删线
            for (auto it = AET.begin(); it != AET.end(); )
                if (it->ymax == scan_line)
                    AET.erase(it++);
                else
                    it++;
            //活性边表中加线
            for (int i = 0; i < net2[posi - 1].size(); i++)
                if (net2[posi - 1][i].y == net2[posi-1][i].ymax)
                    continue;//横线不加进去
                else
                    AET.emplace_back(net2[posi - 1][i]);
            AET.sort(AET_lefter);//保证活性边表有序
        }
    }
}

//鼠标动作
void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && can_draw) {
        if (flag == 1 && can_draw) {//画点
            point_paint(x, 600 - y);
            x_0 = x_1 = x;//图形起始位置和第一个点
            y_0 = y_1 = y;
            hmax = hmin = y_0;
            flag++;
        } else {//画线
            point_paint(x, 600 - y);
            x_2 = x;
            y_2 = y;
            draw_lines(x_1, y_1, x_2, y_2);
            if (y_1 != y_2) {
                edge new_edge(x_1, y_1, x_2, y_2);
                //处理扫描线范围
                if (y_1 < hmin)
                    hmin = y_1;
                if (y_2 < hmin)
                    hmin = y_2;
                if (y_1 > hmax)
                    hmax = y_1;
                if (y_2 > hmax)
                    hmax = y_2;
                int pos = mapper[min(y_1, y_2)];
                if (pos == 0) {
                    mapper[min(y_1, y_2)] = level++;
                    vector<edge> lines;
                    lines.emplace_back(new_edge);
                    NET.emplace_back(lines);
                } else
                    NET[pos - 1].emplace_back(new_edge);
            }
            //画下一个点时把前一个点的末尾当作第一个点
            x_1 = x_2;
            y_1 = y_2;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && can_draw) {
        point_paint(x, 600 - y);
        x_2 = x;
        y_2 = y;
        if (y_2 < hmin)
            hmin = y_2;
        if (y_2 > hmax)
            hmax = y_2;
        draw_lines(x_1, y_1, x_2, y_2);
        edge new_edge(x_1, y_1, x_2, y_2);
        int pos = mapper[min(y_1, y_2)];
        if (pos == 0) {
            mapper[min(y_1, y_2)] = level++;
            vector<edge> lines;
            lines.emplace_back(new_edge);
            NET.emplace_back(lines);
        } else
            NET[pos - 1].emplace_back(new_edge);
        //首尾相连，结束画图
        draw_lines(x_0, y_0, x_2, y_2);
        edge last_edge(x_0, y_0, x_2, y_2);
        int pos_last = mapper[min(y_0, y_2)];
        if (pos_last == 0) {
            mapper[min(y_0, y_2)] = level++;
            vector<edge> lines;
            lines.emplace_back(last_edge);
            NET.emplace_back(lines);
        } else
            NET[pos_last - 1].emplace_back(last_edge);
        sort_net();
        polygon();
        can_draw = false;
    }
}

void mydisplay() { glFlush(); }

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);

    //指定显示模式的类型
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//RGB颜色模式，单缓冲窗口

    glutCreateWindow("experiment_4");

    gluOrtho2D(0, 600, 0, 600);

    init();
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mouse_click);

    glutMainLoop();
    return 0;
}
