//
// Created by root on 3/5/17.
//

#ifndef JDROBOTKINECT_GLSHOWER_H
#define JDROBOTKINECT_GLSHOWER_H
#include "../opencvProc/cvProcesser.h"
#include "../Communicate/Communication.h"
#include "GL/gl.h"

namespace viewer {

    void init(int argc, char *argv[]);

    static void timer(int p);

    void mouse(int button, int state, int x, int y);

    void motion(int x, int y);

    void special(int key, int x, int y);

    void renderScene(void);

    void reshape(int w, int h);

    void start();

}
#endif //JDROBOTKINECT_GLSHOWER_H
