#include <GL/glut.h>
#include <cmath>

#define PI 3.1415926

//标志鼠标左右键按下与否
bool mouse_left_down;
bool mouse_right_down;

//标志鼠标滑动时，前一刻的鼠标所在位置。
float mouse_x;
float mouse_y;

//根据鼠标右键按下并滑动操作来控制模型物体的平移
float distance_x;
float distance_y;

//根据鼠标左键按下并滑动来控制场景中物体的旋转
float angle_x;
float angle_y;

//初始化设置
void init(){
    glClearColor(0.0,0.0,0.0,0.0);//背景为黑色
//    glColor3f(1.0,1.0,1.0);//绘图用白色
    glMatrixMode(GL_PROJECTION);//设置合适的矩阵
    glLoadIdentity();
}

//鼠标点击
void mouse_click(int button, int state, int x, int y){
    //保存鼠标位置
    mouse_x=x;
    mouse_y=y;

    //鼠标左键
   if(button==GLUT_LEFT_BUTTON){
        if(state==GLUT_DOWN)
            mouse_left_down= true;
        else if(state==GLUT_UP)
            mouse_left_down= false;
    }
    //鼠标右键
    else if(button==GLUT_RIGHT_BUTTON){
        if(state==GLUT_DOWN)
            mouse_right_down= true;
        else if(state==GLUT_UP)
            mouse_right_down= false;
    }
}

//鼠标移动
void mouse_motion(int x,int y){
    //初始化
    distance_x=distance_y=0;
    angle_x=angle_y=0;

    //左键控制平移
    if (mouse_left_down){
        //计算移动距离
        distance_x=(x-mouse_x)*0.002f;
        distance_y=-(y-mouse_y)*0.002f;
        //鼠标位置信息更新
        mouse_x=x;
        mouse_y=y;
    }
    //右键控制旋转
    if (mouse_right_down){
        //计算偏移角度
        angle_x+=(y-mouse_y)*0.5f;
        angle_y+=(x-mouse_x)*0.5f;
        //鼠标位置信息更新
        mouse_x=x;
        mouse_y=y;
    }

    glutPostRedisplay();
}

//绘制
void paint() {
    glClear(GL_COLOR_BUFFER_BIT);//清除屏幕和深度缓存

    //移动
    glTranslatef(distance_x,distance_y,0);
    //旋转
    glRotatef(angle_x,1,0,0);
    glRotatef(angle_y,0,1,0);

    glColor3f(0.5,0.5,1.0);

    //绘制三角形
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.4f,0.2f,0.0f);//上顶点
    glVertex3f(-0.6f,-0.2f,0.0f);//左下顶点
    glVertex3f(-0.2f,-0.2f,0.0f);//右下顶点
    glEnd();

    //绘制圆形
    glBegin(GL_TRIANGLE_FAN);//连续圆形
    //用五十个连续的三角形面积来逼近圆的面积实现圆周的画法
    for (int i = 1; i <= 50; i++) {
        double angle=2*PI*i/50;
        glVertex3d(0.2*cos(angle),0.2*sin(angle),0);
    }
    glEnd();

    glFlush();//输出全部图形
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("experiment_1");

    init();
    glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_motion);
    glutDisplayFunc(paint);

    glutMainLoop();
    return 0;
}
