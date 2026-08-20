#define _USE_MATH_DEFINES
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

float k = -0.25, r = 200, l = 0.2, th = 0;
float x, y, xp, yp;
char buffer[100];

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void Hardware_Init(void);
void Hardware_DeInit(void);

void Test_Loop(void);
void JagPad_PacketDisplay(const U8 aPacket);

void Screen_DrawBars(U16 *apScreen);

void draw()
{
    for (int i = 0; i < 800; i++)
    {
        x = r * ((1 - k) * cos(th) + l * k * cos((1 - k) / k * th));
        y = r * ((1 - k) * sin(th) - l * k * sin((1 - k) / k * th));
        if (i > 0)
        {
            al_draw_line(x + 400, y + 300, xp + 400, yp + 300, al_map_rgb(255, 120, 255), 0);
        }
        xp = x;
        yp = y;
        th += M_PI / 30;
    }
}

int main()
{

    bool redraw = true;

    GemDos_Super(0);

    Hardware_Init();

    while (!IKBD_GetKeyStatus(eIKBDSCAN_SPACE))
    {
        Screen_Swap();

        if (redraw)
        {
            draw();

            th = 0;
            // r2+=1;

            k += 0.02;
            if (k > 1.0)
            {
                k = -0.25;
                l += 0.1;
            }

            if (l > 1.0)
            {
                l = -1.0;
            }

            redraw = false;
        }
    }

    Hardware_DeInit();

    return (0);
}
