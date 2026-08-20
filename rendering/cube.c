
#include <GODLIB\GEMDOS\GEMDOS.H>
#include <GODLIB\IKBD\IKBD.H>
#include <GODLIB\MEMORY\MEMORY.H>
#include <GODLIB\SYSTEM\SYSTEM.H>
#include <GODLIB\VBL\VBL.H>
#include <GODLIB\VIDEO\VIDEO.H>
#include <GODLIB\GRAPHIC\GRAPHIC.H>

#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int nvert = 4;
const int nfaces = 6;
unsigned char idx;
int screen, row, col;
float th = 0;

int CUBE[] = {

    -1, -1, -1, // X=-1
    -1, 1, -1,
    -1, 1, 1,
    -1, -1, 1,

    -1, -1, -1, // Y=-1
    -1, -1, 1,
    1, -1, 1,
    1, -1, -1,

    -1, -1, -1, // Z=-1
    1, -1, -1,
    1, 1, -1,
    -1, 1, -1,

    1, -1, 1, // Z=1
    -1, -1, 1,
    -1, 1, 1,
    1, 1, 1,

    1, -1, -1, // X=1
    1, 1, -1,
    1, 1, 1,
    1, -1, 1,

    1, 1, 1, // Y=1
    1, 1, -1,
    -1, 1, -1,
    1, 1, -1};

void put_pixel(int x, int y)
{
    // TODO: USE GODLIB to se pixels on screen
}

void line(int x, int y, int x1, int y1)
{
    int x0 = x;
    int y0 = y;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = -1;
    int sy = -1;
    int rx = x1;
    int ry = y1;
    int e2, error;

    if (x0 < x1)
    {
        sx = 1;
        rx = x0;
    }
    if (y0 < y1)
    {
        sy = 1;
        ry = y0;
    }

    error = dx - dy;

    while (x0 != x1 || y0 != y1)
    {
        put_pixel(x0, y0);
        e2 = 2 * error;
        if (e2 > -dy)
        {
            error -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            error += dx;
            y0 += sy;
        }
    }
}

void bline(int x, int y, int u, int v)
{
    int x0 = x;
    int y0 = y;
    int x1 = u;
    int y1 = v;
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = -1;
    int sy = -1;
    int e2, error;

    if (x0 < x1)
    {
        sx = 1;
    }
    if (y0 < y1)
    {
        sy = 1;
    }

    error = dx + dy;

    while (1 == 1)
    {
        put_pixel(x0, y0);
        put_pixel(x1, y1);

        if (abs(x0 - x1) <= 1 && abs(y0 - y1) <= 1)
            return;

        e2 = 2 * error;
        if (e2 >= dy)
        {
            error += dy;
            x0 += sx;
            x1 -= sx;
        }
        if (e2 <= dx)
        {
            error += dx;
            y0 += sy;
            y1 -= sy;
        }
    }
}

int cube(void)
{
    int x, y, z, xp, yp, yr, zr;
    int i, j, xs, ys, x1, y1, x0, y0;
    int sqrt2 = 1414;
    int sqrt6 = 2449;
    int r = 80;
    idx = 0;
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for (i = 0; i < nfaces - 1; i++)
    {
        for (j = 0; j < nvert; j++)
        {
            x = CUBE[idx++] * 100;
            y = CUBE[idx++] * 100;
            z = CUBE[idx++] * 100;

            // rotation
            yr = ((long)y * cos(th) - (long)z * sin(th));
            zr = ((long)y * sin(th) + (long)z * cos(th));

            xp = (long)1000 * (x - zr) / sqrt2;
            yp = (long)1000 * (x + 2 * yr + zr) / sqrt6;

            xs = xp + 800 / 2;
            ys = yp + 600 / 2;

            if (j == 0)
            {
                x0 = xs;
                y0 = ys;
            }
            else
            {
                // USE GODLIB to draw lines on screen
                line(x0, y0, xs, ys);
            }
            x1 = xs;
            y1 = ys;
        }
    }
    return EXIT_SUCCESS;
}

int main()
{

    bool redraw = true;

    while (1)
    {
        th = th + M_PI / 20;
        if (th >= 2 * M_PI)
            th = 0;

        cube();
        redraw = false;
    }

    return 0;
}
