#ifndef GLOBJECT_H
#define GLOBJECT_H
//#include "oglwidget.h"
#include "vector"
using namespace std;
class gLObject
{
public:

    typedef enum{
        EM_point,
        EM_line
    }Object_Type;

    typedef struct{
        float x;
        float y;
        float z;
    }EM_Point;
    gLObject();
    ~gLObject();
    //OGLWidget DrawPlane;
    Object_Type object_type;
    vector<EM_Point*> Key_Point;
    static gLObject* DrawLine(float x1,float y1,float z1,float x2,float y2,float z2);
    static gLObject* DrawPoint(float x,float y,float z);



};

#endif // GLOBJECT_H
