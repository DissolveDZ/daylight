#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "rlgl.h" 
#include <stdlib.h> // Required for: abs()

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
typedef struct Point2D
{
    float x;
    float y;
} Point2D;
const float EquityTolerance = 0.0000001f;
typedef struct ConvexPolygon2D
{
    int corner_size;
    Point2D *Corners;
} ConvexPolygon2D;

bool IsEqual(float d1, float d2)
{
    return fabs(d1 - d2) <= EquityTolerance;
}
Point2D GetIntersectionPoint(Point2D l1p1, Point2D l1p2, Point2D l2p1, Point2D l2p2, bool *success)
{
    float A1 = l1p2.y - l1p1.y;
    float B1 = l1p1.x - l1p2.x;
    float C1 = A1 * l1p1.x + B1 * l1p1.y;
    float A2 = l2p2.y - l2p1.y;
    float B2 = l2p1.x - l2p2.x;
    float C2 = A2 * l2p1.x + B2 * l2p1.y;
    // lines are parallel
    float det = A1 * B2 - A2 * B1;
    if (IsEqual(det, 0.f))
    {
        *success = false;
        return (Point2D){0, 0}; // parallel lines
    }
    else
    {
        float x = (B2 * C1 - B1 * C2) / det;
        float y = (A1 * C2 - A2 * C1) / det;
        bool online1 = ((fminf(l1p1.x, l1p2.x) < x || IsEqual(fminf(l1p1.x, l1p2.x), x)) && (fmaxf(l1p1.x, l1p2.x) > x || IsEqual(fmaxf(l1p1.x, l1p2.x), x)) && (fminf(l1p1.y, l1p2.y) < y || IsEqual(fminf(l1p1.y, l1p2.y), y)) && (fmaxf(l1p1.y, l1p2.y) > y || IsEqual(fmaxf(l1p1.y, l1p2.y), y)));
        bool online2 = ((fminf(l2p1.x, l2p2.x) < x || IsEqual(fminf(l2p1.x, l2p2.x), x)) && (fmaxf(l2p1.x, l2p2.x) > x || IsEqual(fmaxf(l2p1.x, l2p2.x), x)) && (fminf(l2p1.y, l2p2.y) < y || IsEqual(fminf(l2p1.y, l2p2.y), y)) && (fmaxf(l2p1.y, l2p2.y) > y || IsEqual(fmaxf(l2p1.y, l2p2.y), y)));
        if (online1 && online2)
        {
            *success = true;
            return (Point2D){x, y};
        }
    }
    *success = false;
    return (Point2D){0, 0}; // intersection is at out of at least one segment.
}

bool IsPointInsidePoly(Point2D test, ConvexPolygon2D poly)
{
    int i;
    int j;
    bool result = false;
    for (i = 0, j = poly.corner_size - 1; i < poly.corner_size; j = i++)
    {
        if ((poly.Corners[i].y > test.y) != (poly.Corners[j].y > test.y) &&
            (test.x < (poly.Corners[j].x - poly.Corners[i].x) * (test.y - poly.Corners[i].y) / (poly.Corners[j].y - poly.Corners[i].y) + poly.Corners[i].x))
        {
            result = !result;
        }
    }
    return result;
}

int max_intersectionPoints = 100;
int cur_intersectionPoints = 0;

Point2D* GetIntersectionPoints(Point2D l1p1, Point2D l1p2, ConvexPolygon2D poly)
{
    Point2D* intersectionPoints = malloc(sizeof(Point2D) * max_intersectionPoints);
    for (int i = 0; i < poly.corner_size-1; i++)
    {
        if (cur_intersectionPoints >= max_intersectionPoints - 1)
        {
            max_intersectionPoints += 100;
            intersectionPoints = realloc(intersectionPoints, sizeof(Point2D) * max_intersectionPoints);
        }
        int next = (i + 1 == poly.corner_size) ? 0 : i + 1;
        bool success = false;
        Point2D ip = GetIntersectionPoint(l1p1, l1p2, poly.Corners[i], poly.Corners[next], &success);
        if (success) 
        {
            printf("ppu");
            intersectionPoints[cur_intersectionPoints] = ip;
            cur_intersectionPoints++;
        }

    }
    return intersectionPoints;
}
int cur_newpoints = 0;
int cur_pool = 0;
int clippedcount = 0;

void AddPoints(Point2D *pool, Point2D *newpoints, int new)
{
    for (int i = 0; i < cur_newpoints; i++)
    {
        Point2D np = newpoints[i];
        bool found = false;
        for (int p = 0; p < clippedcount; p++)
        {
            Point2D point = pool[p];
            if (IsEqual(point.x, np.x) && IsEqual(point.y, np.y))
            {
                //printf("hat ihn gefunden");
                found = true;
                break;
            }
        }
        if (!found) 
        {
            //printf("hat ihn NICHt gefunden");
            pool[clippedcount] = np;
            clippedcount++;
        }
    }
}

int cur_points = 0;

Point2D *Sort(Point2D *points)
{
    float mX = 0;
    float my = 0;
    for(int i = 0; i < 4; i++)
    {
        Point2D point = points[i];
        mX += point.x;
        my += point.y;
    }
    mX /= 4;
    my /= 4;
    Point2D temp = (Point2D){0, 0};
    float last = 0;
    for(int i = 0; i < 4; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            float cur;
            if (i != 0) cur = atan2(points[i-1].y - my, points[i-1].x - mX);
            else cur = atan2(points[i].y - my, points[i].x - mX);

            if (cur > last) 
            {
                temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
            last = cur;
        }
        //printf("x: %f  y: %f\n", points[i].x, points[i].y);
    }
    return points;
}

Point2D *OrderClockwise(Point2D *points)
{
    float mX = 0;
    float my = 0;
    for(int i = 0; i < cur_points; i++)
    {
        Point2D point = points[i];
        mX += point.x;
        my += point.y;
    }
    mX /= cur_points;
    my /= cur_points;
    Point2D temp = (Point2D){0, 0};
    float last = 0;
    for(int i = 0; i < cur_points; i++)
    {
        for (int j = i + 1; j < cur_points; j++)
        {
            float cur;
            if (i != 0) cur = atan2(points[i-1].y - my, points[i-1].x - mX);
            else cur = atan2(points[i].y - my, points[i].x - mX);

            if (cur > last) 
            {
                temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
            last = cur;
        }
        //printf("x: %f  y: %f\n", points[i].x, points[i].y);
    }
    return points;
}
ConvexPolygon2D GetIntersectionOfPolygons(ConvexPolygon2D poly1, ConvexPolygon2D poly2)
{
    cur_intersectionPoints = 0;
    clippedcount = 0;
    cur_points = 0;
    cur_newpoints = 0;
    cur_pool = 0;
    Point2D *clippedCorners;
    clippedCorners = malloc(100 * sizeof(Point2D));
    //Add  the corners of poly1 which are inside poly2       
    for (int i = 0; i < poly1.corner_size; i++)
    {
        if (IsPointInsidePoly(poly1.Corners[i], poly2))
        {
            cur_newpoints = poly1.corner_size;
            AddPoints(clippedCorners, &poly1.Corners[i], cur_newpoints);
            cur_points++;
        }

    }
    //Add the corners of poly2 which are inside poly1
    for (int i = 0; i < poly2.corner_size; i++)
    {
        if (IsPointInsidePoly(poly2.Corners[i],poly1))     
        {
            cur_newpoints = poly2.corner_size;
            AddPoints(clippedCorners, &poly2.Corners[i], cur_newpoints);
            cur_points++;
        }          
    }
    //Add  the intersection points
    for (int i = 0, next = 1; i < poly1.corner_size; i++, next = (i + 1 == poly1.corner_size) ? 0 : i + 1)
    {
        cur_newpoints = cur_intersectionPoints;
        AddPoints(clippedCorners, GetIntersectionPoints(poly1.Corners[i], poly1.Corners[next], poly2), cur_newpoints);
        cur_points++;
    }
    //cur_newpoints + cur_intersectionPoints
    return (ConvexPolygon2D){cur_points, OrderClockwise( clippedCorners)};
}

int main(void)
{
    // Initialization
    //---------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - collision area");

    Point2D *polers = malloc(4 * sizeof(Point2D));
    polers[0] = (Point2D){0, 0};
    polers[1] = (Point2D){100, 0};
    polers[2] = (Point2D){100, 100};
    polers[3] = (Point2D){0, 100};

    Point2D *polers2 = malloc(4 * sizeof(Point2D));
    polers2[0] = (Point2D){50, 50};
    polers2[1] = (Point2D){150, 0};
    polers2[2] = (Point2D){150, 150};
    polers2[3] = (Point2D){0, 150};

    ConvexPolygon2D poler1 = {4, polers};
    ConvexPolygon2D poler2 = {4, polers2};
    // Box A: Moving box
    Rectangle boxA = {10, GetScreenHeight() / 2.0f - 50, 200, 100};
    int boxASpeedX = 4;

    // Box B: Mouse moved box
    Rectangle boxB = {GetScreenWidth() / 2.0f - 30, GetScreenHeight() / 2.0f - 30, 60, 60};

    Rectangle boxCollision = {0};

    int screenUpperLimit = 40;

    bool pause = false;
    bool collision = false;
    Texture2D tex = LoadTexture("resources/vedl.png");

    SetTargetFPS(60);
    float sinewav = 0;
    Point2D *polers5 = malloc(4 * sizeof(Point2D));
    polers5[0] = (Point2D){50, 50};
    polers5[1] = (Point2D){150, 0};
    polers5[2] = (Point2D){150, 150};
    polers5[3] = (Point2D){0, 150};
    for(int i = 0; i < 4; i++)
    {
        printf("old x: %f y: %f\n", polers5[i].x, polers5[i].y);
    }
    polers2 = Sort(polers5);
    for(int i = 0; i < 4; i++)
    {
        printf("new x: %f y: %f\n", polers5[i].x, polers5[i].y);
    }
    while (!WindowShouldClose())
    {
        //sinewav += GetFrameTime();
        //ConvexPolygon2D poler4 = poler2;
        //poler4.Corners[0].x += sinewav;
        //poler4.Corners[1].x += sinewav;
        //poler4.Corners[2].x += sinewav;
        //poler4.Corners[3].x += sinewav;
        //ConvexPolygon2D poler3 = GetIntersectionOfPolygons(poler1, poler2);
        //printf("%f\n", sin(sinewav));
        //ConvexPolygon2D poler3 = GetIntersectionOfPolygons(poler1, poler2);
        //printf("clipped: %i\n", poler3.corner_size);
        //for(int i = 0; i < poler3.corner_size; i++)
            //printf("x: %f y: %f\n", poler3.Corners[i].x, poler3.Corners[i].y);
        if (!pause)
            boxA.x += boxASpeedX;

        // Bounce box on x screen limits
        if (((boxA.x + boxA.width) >= GetScreenWidth()) || (boxA.x <= 0))
            boxASpeedX *= -1;

        // Update player-controlled-box (box02)
        boxB.x = GetMouseX() - boxB.width / 2;
        boxB.y = GetMouseY() - boxB.height / 2;
        //Vector2 offset = (Vector2){GetMouseX() - 25, GetMouseY() - 25};
        //polers[0] = (Point2D){50 + offset.x, 50 + offset.y};
        //polers[1] = (Point2D){150 + offset.x, 50 + offset.y};
        //polers[2] = (Point2D){150 + offset.x, 150 + offset.y};
        //polers[3] = (Point2D){50 + offset.x, 150 + offset.y};

        // Make sure Box B does not go out of move area limits
        if ((boxB.x + boxB.width) >= GetScreenWidth())
            boxB.x = GetScreenWidth() - boxB.width;
        else if (boxB.x <= 0)
            boxB.x = 0;

        if ((boxB.y + boxB.height) >= GetScreenHeight())
            boxB.y = GetScreenHeight() - boxB.height;
        else if (boxB.y <= screenUpperLimit)
            boxB.y = (float)screenUpperLimit;

        // Check boxes collision
        collision = CheckCollisionRecs(boxA, boxB);

        // Get collision rectangle (only on collision)
        if (collision)
            boxCollision = GetCollisionRec(boxA, boxB);

        // Pause Box A movement
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;
        //printf("count: %i\n", poler3.corner_size);
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, screenWidth, screenUpperLimit, collision ? RED : BLACK);

        DrawRectangleRec(boxA, GOLD);
        DrawRectangleRec(boxB, BLUE);

        if (collision)
        {
            // Draw collision area
            DrawRectangleRec(boxCollision, LIME);

            // Draw collision message
            DrawText("COLLISION!", GetScreenWidth() / 2 - MeasureText("COLLISION!", 20) / 2, screenUpperLimit / 2 - 10, 20, BLACK);

            // Draw collision area
            DrawText(TextFormat("Collision Area: %i", (int)boxCollision.width * (int)boxCollision.height), GetScreenWidth() / 2 - 100, screenUpperLimit + 10, 20, BLACK);
        }
        //printf("count: %i\n", poler1.corner_size);
        //for(int i = 0; i < poler3.corner_size; i++)
            //printf("x: %f y: %f\n", poler3.Corners[i].x, poler3.Corners[i].y);
        DrawFPS(10, 10);
        //Vector2 *tex_pol = malloc(poler3.corner_size * sizeof(Vector2));
       // Vector2 *vec2_pol = malloc(poler3.corner_size * sizeof(Vector2)); 
        //for (int i = 0; i < poler3.corner_size; i++)
        //{
        //    tex_pol[i] = Vector2Normalize((Vector2){poler3.Corners[i].x, poler3.Corners[i].y});
        //    vec2_pol[i] = (Vector2){poler3.Corners[i].x, poler3.Corners[i].y};
        //}
        //DrawTexturePoly(tex, (Vector2){0, 0}, vec2_pol, tex_pol, poler3.corner_size, GRAY);
        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}