//
// Created by root on 3/5/17.
//

#include "glViewer.h"
#include <GL/glut.h>
#include "../Threads/Threads.h"

namespace viewer {

    vector<vector<Point3f>> boxPoints;
    int w = 0;
    int h = 0;
    float scalar = 50;//scalar of converting pixel color to float coordinates

    bool mouseisdown = false;
    bool loopr = false;
    int mx, my;
    int ry = 10;
    int rx = 10;

    void timer(int p) {
        ry -= 5;
        //marks the current window as needing to be redisplayed.
        glutPostRedisplay();

        cout <<"Timer"<<endl;
        if (loopr)
            glutTimerFunc(200, timer, 0);
    }

    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                mouseisdown = true;
                loopr = false;
            } else {
                mouseisdown = false;
            }
        }

        if (button == GLUT_RIGHT_BUTTON)
            if (state == GLUT_DOWN) {
                loopr = true;
                glutTimerFunc(200, timer, 0);
            }
    }

    void motion(int x, int y) {
        if (mouseisdown == true) {
            ry += x - mx;
            rx += y - my;
            mx = x;
            my = y;
            glutPostRedisplay();
        }
    }

    void special(int key, int x, int y) {
        switch (key) {
            case GLUT_KEY_LEFT:
                ry -= 5;
                glutPostRedisplay();
                break;
            case GLUT_KEY_RIGHT:
                ry += 5;
                glutPostRedisplay();
                break;
            case GLUT_KEY_UP:
                rx += 5;
                glutPostRedisplay();
                break;
            case GLUT_KEY_DOWN:
                rx -= 5;
                glutPostRedisplay();
                break;
        }
    }

    void renderScene(void) {

            //pthread_mutex_lock(&mutex);
            boxPoints = processer.boxPoints3d;
            cout << "test" << endl;
            //pthread_mutex_unlock(&mutex);

            glClear(GL_COLOR_BUFFER_BIT);
            glLoadIdentity();// Reset the coordinate system before modifying
            gluLookAt(0.0, 0.0, 7.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
            //to invert the image
            glRotatef(ry, 0, 1, 0);
            glRotatef(rx - 180, 1, 0, 0);

            float x, y, z;
            glColor3b(255,255,255);
            glPointSize(10.0);
            glBegin(GL_POINTS);//GL_POINTS
            //cout << "test2" << endl;
//            glVertex3f(0, 0, 1);
            //cout << "test3" << endl;
        for (size_t i = 0; i < boxPoints.size(); i++) {
            for (size_t j = 0; j < boxPoints.at(i).size(); j++) {
                // color interpolation
                glColor3f(1, 1, 1);//red,green,blue
                glVertex3f(boxPoints.at(i).at(j).x,
                           boxPoints.at(i).at(j).y,
                           boxPoints.at(i).at(j).z);
            }
        }


            glEnd();
            //cout << "test4" << endl;
            glFlush();
            //cout << "test5" << endl;

        }

    void reshape(int w, int h) {
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);//set viewpoint
        glMatrixMode(GL_PROJECTION);//specify which matrix is the current matrix
        glLoadIdentity();//replace the current matrix with the identity matrix
        //gluPerspective(60, (GLfloat) w / (GLfloat) h, 1.0, 100.0);
        glOrtho(-w,w,-h,h,-100000,10000);

        glMatrixMode(GL_MODELVIEW);
    }

    void init(int argc, char *argv[]) {
        //pthread_mutex_lock(&mutex);
        glutInit (&argc, argv);;//initialize the GLUT library
        glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);//sets the initial display mode
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(640, 480);
        glutCreateWindow("3D disparity image");
        glutDisplayFunc(renderScene);

        glutReshapeFunc(reshape);
//        glutMouseFunc(mouse);
//        glutMotionFunc(motion);
//        glutSpecialFunc(special);
//        pthread_mutex_unlock(&mutex);

    }

    void start() {
//        pthread_mutex_lock(&mutex);


        cout<<"enter thread"<<endl;
        glutMainLoop();
//        pthread_mutex_unlock(&mutex);

    }
}
