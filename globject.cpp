#include "globject.h"

gLObject::gLObject()
{

}

gLObject::~gLObject()
{

}
 gLObject* gLObject::DrawPoint(float x, float  y, float z){
    gLObject *_obj = new gLObject();
    _obj->object_type = EM_point;
    EM_Point *point = new EM_Point;
    point->x = x;
    point->y = y;
    point->z = z;
    _obj->Key_Point.push_back(point);
    return _obj;
}
