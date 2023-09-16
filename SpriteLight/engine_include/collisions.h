#include "main.h"
bool CheckCollisionRec(Rectangle rec1, Rectangle rec2);
bool CheckColliders(Collider col1, Collider col2);
Collider GetCollisionArea(Collider rec1, Collider rec2);
bool CheckRotatingColliders(Collider col1, Collider col2);

// LINE/LINE
PointIntersect lineLine(float x1, float y_1, float x2, float y2, float x3, float y3, float x4, float y4);
// LINE/RECTANGLE
PointIntersect lineRect(float x1, float y_1, float x2, float y2, float rx, float ry, float rw, float rh);
int CheckCollisionX(Entity *ent, int index);
int CheckCollisionY(Entity *ent, int index);
bool CheckFloor(Entity *ent, int index);
bool ReactToSolid(Entity *ent);