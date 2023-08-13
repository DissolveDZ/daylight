#include <stdio.h>
#include <stdlib.h>
// #include <raymath.h>
#include "main.h"
#pragma once

static inline bool CheckCollisionRec(Rectangle rec1, Rectangle rec2)
{
   bool collision = false;
   if ((rec1.x - rec1.width / 2 < (rec2.x + rec2.width / 2) && (rec1.x + rec1.width / 2) > rec2.x - rec2.width / 2) &&
       (rec1.y - rec1.height / 2 < (rec2.y + rec2.height / 2) && (rec1.y + rec1.height / 2) > rec2.y - rec2.height / 2))
      collision = true;
   return collision;
}
static inline bool CheckColliders(Collider col1, Collider col2)
{
   bool collision = false;
   if ((col1.x - col1.width / 2 < (col2.x + col2.width / 2) && (col1.x + col1.width / 2) > col2.x - col2.width / 2) &&
       (col1.y - col1.height / 2 < (col2.y + col2.height / 2) && (col1.y + col1.height / 2) > col2.y - col2.height / 2))
      collision = true;
   return collision;
}

Collider GetCollisionArea(Collider rec1, Collider rec2)
{
   Collider overlap = {0};

   float left = (rec1.x - rec1.width / 2 > rec2.x - rec2.width / 2) ? rec1.x - rec1.width / 2 : rec2.x - rec2.width / 2;
   float right1 = rec1.x + rec1.width / 2;
   float right2 = rec2.x + rec2.width / 2;
   float right = (right1 < right2) ? right1 : right2;
   float top = (rec1.y + rec1.height / 2 < rec2.y + rec2.height / 2) ? rec1.y + rec1.height / 2 : rec2.y + rec2.height / 2;
   float bottom1 = rec1.y - rec1.height / 2;
   float bottom2 = rec2.y - rec2.height / 2;
   float bottom = (bottom1 > bottom2) ? bottom1 : bottom2;

   if ((left < right) && (top > bottom))
   {
      overlap.width = right - left;
      overlap.height = top - bottom;
      overlap.x = left + overlap.width / 2;
      overlap.y = top - overlap.height / 2;
   }
   return overlap;
}

static inline bool CheckRotatingColliders(Collider col1, Collider col2)
{
   bool collision = false;
   if ((col1.x < (col2.x + col2.width / 2) && (col1.x + col1.width / 2) > col1.x) &&
       (col1.y < (col2.y + col2.height / 2) && (col1.y + col1.height / 2) > col1.y))
      collision = true;
   return collision;
}

float x1 = 0; // points for line (controlled by mouse)
float y_1 = 0;
float x2 = 0; // static point
float y2 = 0;

float sx = 200; // square position
float sy = 100;
float sw = 200; // and size
float sh = 200;

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
int min()
{
   return 0;
}
int CheckCollisionX(Entity *ent, int index)
{
   if (CheckColliders((Collider){ent->col.x + (ent->velocity.x * frame_time), ent->col.y, ent->col.width, ent->col.height, false, false, NULL}, Boxes[index]))
   {
      PointIntersect point;
      point.hit = false;
      // printf("pelocity x: %f", ent->velocity.x);
      Vector2 new = (Vector2){ent->col.x, ent->col.y};
      if (ent->velocity.x < 0)
      {
         point = lineRect(new.x, new.y, new.x - ent->col.width / 2 + (ent->velocity.x * frame_time), new.y, Boxes[index].x, Boxes[index].y, Boxes[index].width, Boxes[index].height);
      }
      else if (ent->velocity.x > 0)
      {
         point = lineRect(new.x, new.y, new.x + ent->col.width / 2 + (ent->velocity.x * frame_time), new.y, Boxes[index].x, Boxes[index].y, Boxes[index].width, Boxes[index].height);
      }
      if (point.hit)
      {
         if (ent->velocity.x < 0)
         {
            // printf("left");
            ent->col.x = point.dist.x + ent->col.width / 2;
            ent->col.x -= GetCollisionArea(ent->col, Boxes[index]).width;
         }
         else
         {
            // printf("right");
            ent->col.x = point.dist.x - ent->col.width / 2;
            ent->col.x -= GetCollisionArea(ent->col, Boxes[index]).width;
         }
      }
      ent->velocity.x = 0;
      return 1;
   }
   else
      return 0;
}
int CheckCollisionY(Entity *ent, int index)
{
   if (CheckColliders((Collider){ent->col.x, ent->col.y + (ent->velocity.y * frame_time), ent->col.width, ent->col.height, false, false, NULL}, Boxes[index]))
   {
      PointIntersect point;
      if (ent->velocity.y < 0)
      {
         Vector2 new = (Vector2){ent->col.x, ent->col.y};
         point = lineRect(new.x, new.y, new.x, new.y - ent->col.height / 2 + (ent->velocity.y * frame_time), Boxes[index].x, Boxes[index].y, Boxes[index].width, Boxes[index].height);
      }
      else if (ent->velocity.y > 0)
      {
         Vector2 new = (Vector2){ent->col.x, ent->col.y};
         point = lineRect(new.x, new.y, new.x, new.y + ent->col.height / 2 + (ent->velocity.y * frame_time), Boxes[index].x, Boxes[index].y, Boxes[index].width, Boxes[index].height);
      }
      if (point.hit)
      {
         if (ent->velocity.y < 0)
         {
            // printf("down");
            ent->col.y = point.dist.y + ent->col.height / 2;
            ent->col.y += GetCollisionArea(ent->col, Boxes[index]).height;
         }
         else
         {
            // printf("up");
            ent->col.y = point.dist.y - ent->col.height / 2;
            ent->col.y -= GetCollisionArea(ent->col, Boxes[index]).height;
         }
      }
      ent->velocity.y = 0;
      return 1;
   }
   else
      return 0;
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
   bool hit = false;
   for (int i = 0; i < state.cur_colliders; i++)
   {
      if (CheckCollisionX(ent, i))
         hit = true;
      if (CheckCollisionY(ent, i))
         hit = true;
   }
   return hit;
}