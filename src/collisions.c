// #include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
// #include <raymath.h>
#include "main.h"
#pragma once

float Vector2Length(Vector2 vector)
{
   return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

Vector2 Vector2Perpendicular(Vector2 vector)
{
   return (Vector2){-vector.y, vector.x};
}

Vector2 Vector2Normalize(Vector2 vector)
{
   float length = Vector2Length(vector);
   if (length > 0)
   {
      vector.x /= length;
      vector.y /= length;
   }
   return vector;
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2)
{
   Vector2 result;
   result.x = v1.x + v2.x;
   result.y = v1.y + v2.y;
   return result;
}

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)
{
   Vector2 result;
   result.x = v1.x - v2.x;
   result.y = v1.y - v2.y;
   return result;
}

float Vector2Dot(Vector2 v1, Vector2 v2)
{
   return v1.x * v2.x + v1.y * v2.y;
}

Vector2 Vector2Scale(Vector2 v, float scale)
{
   Vector2 result;
   result.x = v.x * scale;
   result.y = v.y * scale;
   return result;
}

static inline bool CheckCollisionRec(Rectangle rec1, Rectangle rec2)
{
   bool collision = false;
   if ((rec1.x - rec1.width / 2 < rec2.x + rec2.width / 2) &&
       (rec1.x + rec1.width / 2 > rec2.x - rec2.width / 2) &&
       (rec1.y - rec1.height / 2 < rec2.y + rec2.height / 2) &&
       (rec1.y + rec1.height / 2 > rec2.y - rec2.height / 2))
   {
      collision = true;
   }
   return collision;
}

static inline bool CheckColliders(Collider col1, Collider col2)
{
   bool collision = false;
   if ((col1.x - col1.width / 2 < col2.x + col2.width / 2) &&
       (col1.x + col1.width / 2 > col2.x - col2.width / 2) &&
       (col1.y - col1.height / 2 < col2.y + col2.height / 2) &&
       (col1.y + col1.height / 2 > col2.y - col2.height / 2))
   {
      collision = true;
   }
   return collision;
}

typedef struct
{
   float min;
   float max;
   Vector2 axis;
} Projection;

// Function to calculate dot product of two vectors
float Dot(Vector2 a, Vector2 b)
{
   return a.x * b.x + a.y * b.y;
}

// Function to check if two projections overlap
bool Overlap(Projection a, Projection b)
{
   return !(a.max < b.min || b.max < a.min);
}

Projection Project(Collider shape, Vector2 axis)
{
   Projection projection;

   float projectionCenter = Dot((Vector2){shape.x, shape.y}, axis);
   float projectionExtentX = shape.width / 2.0f * fabsf(axis.x);
   float projectionExtentY = shape.height / 2.0f * fabsf(axis.y);

   projection.min = projectionCenter - projectionExtentX - projectionExtentY;
   projection.max = projectionCenter + projectionExtentX + projectionExtentY;
   projection.axis = axis;

   return projection;
}
bool CheckCollisionSAT(Collider a, Collider b)
{
   Vector2 axisAtoB = Vector2Normalize(Vector2Subtract((Vector2){b.x, b.y}, (Vector2){a.x, a.y}));
   Vector2 axisBtoA = Vector2Normalize(Vector2Subtract((Vector2){a.x, a.y}, (Vector2){b.x, b.y}));

   Vector2 axes[2] = {
       axisAtoB,
       axisBtoA};

   for (int i = 0; i < 2; i++)
   {
      Vector2 axis = axes[i];

      float lenSqr = axis.x * axis.x + axis.y * axis.y;
      if (lenSqr < 0.0001)
      {
         continue; // Skip tiny axes
      }

      float invLen = 1.0f / sqrtf(lenSqr);
      axis.x *= invLen;
      axis.y *= invLen;

      Projection projA = Project(a, axis);
      Projection projB = Project(b, axis);

      if (!Overlap(projA, projB))
      {
         return false; // Separating axis found, no collision
      }
   }

   return true; // All axes overlap, collision detected
}

Collider GetCollisionArea(Collider rec1, Collider rec2)
{
   Collider overlap = {0};

   float left = fmaxf(rec1.x - rec1.width / 2, rec2.x - rec2.width / 2);
   float right = fminf(rec1.x + rec1.width / 2, rec2.x + rec2.width / 2);
   float top = fmaxf(rec1.y - rec1.height / 2, rec2.y - rec2.height / 2);
   float bottom = fminf(rec1.y + rec1.height / 2, rec2.y + rec2.height / 2);

   overlap.x = (left + right) / 2;
   overlap.y = (top + bottom) / 2;
   overlap.width = right - left;
   overlap.height = bottom - top;

   return overlap;
}

float intersectionX = 0;
float intersectionY = 0;
// LINE/LINE
PointIntersect lineLine(float x1, float y_1, float x2, float y2, float x3, float y3, float x4, float y4)
{

   // calculate the direction of the lines
   float uA = ((x4 - x3) * (y_1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y_1));
   float uB = ((x2 - x1) * (y_1 - y3) - (y2 - y_1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y_1));

   // if uA and uB are between 0-1, lines are colliding
   if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
   {

      // optionally, draw a circle where the lines meet
      intersectionX = x1 + (uA * (x2 - x1));
      intersectionY = y_1 + (uA * (y2 - y_1));
      return (PointIntersect){(Vector2){intersectionX, intersectionY}, true};
   }
   return (PointIntersect){(Vector2){0, 0}, false};
}

// LINE/RECTANGLE
PointIntersect lineRect(float x1, float y_1, float x2, float y2, float rx, float ry, float rw, float rh)
{
   // check if the line has hit any of the rectangle's sides
   // uses the Line/Line function below
   PointIntersect left = lineLine(x1, y_1, x2, y2, rx - rw / 2, ry + rh / 2, rx - rw / 2, ry - rh / 2);
   PointIntersect right = lineLine(x1, y_1, x2, y2, rx + rw / 2, ry + rh / 2, rx + rw / 2, ry - rh / 2);
   PointIntersect top = lineLine(x1, y_1, x2, y2, rx - rw / 2, ry + rh / 2, rx + rw / 2, ry + rh / 2);
   PointIntersect bottom = lineLine(x1, y_1, x2, y2, rx - rw / 2, ry - rh / 2, rx + rw / 2, ry - rh / 2);

   // if ANY of the above are true, the line
   // has hit the rectangle
   if (left.hit)
      return left;
   if (right.hit)
      return right;
   if (top.hit)
      return top;
   if (bottom.hit)
      return bottom;
   return (PointIntersect){0, 0, false};
}
void ResolveCollision(Entity *ent, int index)
{
   Collider boxCollider = Boxes[index]; // Assuming Boxes is an array of Colliders
   Collider entCollider = ent->col;

   Vector2 axes[2] = {
       (Vector2){1, 0}, // X-axis
       (Vector2){0, 1}  // Y-axis
   };

   Vector2 mtv = {0, 0}; // Minimum Translation Vector
   float minOverlap = INFINITY;

   for (int i = 0; i < 2; i++)
   {
      Vector2 axis = axes[i];

      Projection projA = Project(entCollider, axis);
      Projection projB = Project(boxCollider, axis);

      if (!Overlap(projA, projB))
      {
         // Separating axis found, no collision
         return;
      }

      // Calculate overlap along the axis
      float overlap = fminf(projA.max, projB.max) - fmaxf(projA.min, projB.min);

      if (overlap < minOverlap)
      {
         minOverlap = overlap;
         mtv = axis;
      }
   }

   // Apply the MTV to resolve the collision
   Vector2 normal = Vector2Normalize(mtv);

   // Adjust the entity's position to resolve the collision
   float penetrationDepth = minOverlap;
   Vector2 correction = Vector2Scale(normal, penetrationDepth);
   ent->col.x += correction.x;
   ent->col.y += correction.y;

   // Update velocity based on collision
   float dotProduct = Vector2Dot(ent->velocity, normal);
   Vector2 collisionVelocity = Vector2Scale(normal, dotProduct);
   ent->velocity = Vector2Subtract(ent->velocity, collisionVelocity);
}
bool CheckCollisionAndResolveSAT(Entity *ent, int entIndex, Collider *colliders, int numColliders)
{
    Collider entCollider = ent->col;
    bool collisionDetected = false;

    for (int i = 0; i < numColliders; i++)
    {
        if (i != entIndex)
        {
            Collider boxCollider = colliders[i]; // Assuming colliders is an array of Colliders

            Vector2 axisAtoB = Vector2Normalize(Vector2Subtract((Vector2){boxCollider.x, boxCollider.y}, (Vector2){entCollider.x, entCollider.y}));
            Vector2 axisBtoA = Vector2Perpendicular(axisAtoB);

            Vector2 axes[2] = {
                axisAtoB,
                axisBtoA
            };

            bool axesOverlap = true;
            float minOverlap = INFINITY;
            Vector2 mtv = {0, 0};

            for (int j = 0; j < 2; j++)
            {
                Vector2 axis = axes[j];

                Projection projA = Project(entCollider, axis);
                Projection projB = Project(boxCollider, axis);

                if (!Overlap(projA, projB))
                {
                    axesOverlap = false; // Separating axis found, no collision
                    break;
                }

                // Calculate overlap along the axis
                float overlap = fminf(projA.max, projB.max) - fmaxf(projA.min, projB.min);
                if (overlap < minOverlap)
                {
                    minOverlap = overlap;
                    mtv = axis;
                }
            }

            if (axesOverlap)
            {
                collisionDetected = true;

                Vector2 normal = Vector2Normalize(mtv);

                // Calculate penetration depth
                float penetrationDepth = minOverlap;

                // Apply the MTV to resolve the collision
                Vector2 correction = Vector2Scale(normal, penetrationDepth);
                ent->col.x += correction.x;
                ent->col.y += correction.y;

                // Update velocity based on collision
                float dotProduct = Vector2Dot(ent->velocity, normal);
                Vector2 collisionVelocity = Vector2Scale(normal, dotProduct);
                ent->velocity = Vector2Subtract(ent->velocity, collisionVelocity);
            }
        }
    }

    return collisionDetected;
}

bool CheckCollisionXY(Entity *ent, int index)
{
   Collider entCollider = ent->col;
   Collider boxCollider = Boxes[index]; // Assuming Boxes is an array of Colliders
   if (CheckCollisionSAT(entCollider, boxCollider))
   {
      ResolveCollision(ent, index);
      ent->is_floor = true;
      return true;
   }

   return false;
}
bool CheckFloor(Entity *ent, int index)
{
   if (CheckCollisionRec(ent->floor_check, (Rectangle){Boxes[index].x, Boxes[index].y, Boxes[index].width, Boxes[index].height}))
   {
      ent->is_floor = true;
      return true;
   }
   return false;
}

bool ReactToSolid(Entity *ent)
{
   printf("no\n");
   bool hit = false;
   if (CheckCollisionAndResolveSAT(ent, -1, Boxes, state.cur_colliders))
   {
      hit = true;
   }
   if (hit)
   {
      printf("yes\n");
   }
   return hit;
}