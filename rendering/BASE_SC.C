#include <tos.h> /* Contains Physbase, Logbase, Getrez, Setscreen, Vsync */
#include <ext.h> /* Contains standard memory allocation functions like malloc */

#include <stdio.h>
#include <stdlib.h>

/* Screen memory constants */
#define SCREEN_SIZE 32000
#define ALIGNMENT 256

/* Global variables to save original desktop state */
void *old_physical_screen;
void *old_logical_screen;
int old_resolution;
short old_palette[16];

/* Double buffering screen pointers */
unsigned char *orig_buffer = NULL;
unsigned char *back_screen = NULL;
unsigned char *front_screen = NULL;

/* Save current GEM/TOS video state */
void save_state()
{
    old_physical_screen = (void *)Physbase();
    old_logical_screen = (void *)Logbase();
    old_resolution = Getrez();

    /* Save the 16 original palette colors */
    for (int i = 0; i < 16; i++)
    {
        old_palette[i] = Setcolor(i, -1);
    }
}

/* Restore GEM/TOS desktop video state */
void restore_state()
{
    /* Restore original palette */
    for (int i = 0; i < 16; i++)
    {
        Setcolor(i, old_palette[i]);
    }

    /* Restore original screens and resolution */
    Setscreen(old_logical_screen, old_physical_screen, old_resolution);

    /* Free allocated memory */
    if (orig_buffer)
    {
        free(orig_buffer);
    }
}

/* Initialize low resolution and double buffers */
void init_graphics()
{
    save_state();

    /* Allocate memory for the background buffer + alignment padding */
    orig_buffer = (unsigned char *)malloc(SCREEN_SIZE + ALIGNMENT);
    if (!orig_buffer)
    {
        printf("Error: Out of memory!\n");
        exit(1);
    }

    /* Align pointer to 256-byte boundary (required by SHIFTER hardware) */
    back_screen = (unsigned char *)(((unsigned long)orig_buffer + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
    front_screen = (unsigned char *)old_physical_screen; /* Use current screen as front */

    /* Switch to Low Resolution (0) and clear the screens */
    Setscreen(back_screen, front_screen, 0);
}

/* Wait for the monitor's vertical blank interrupt */
void vsync()
{
    Vsync();
}

/* Standard clear screen routine using long words */
void clear_screen(unsigned char *screen_ptr)
{
    unsigned long *long_ptr = (unsigned long *)screen_ptr;
    int i = SCREEN_SIZE / 4;
    while (i--)
    {
        *long_ptr++ = 0;
    }
}

/* Main application execution loop */
void run_app()
{
    /* Set a custom game palette (Format: 0x0RGB, values 0-7) */
    Setcolor(0, 0x000); /* Color 0: Black Background */
    Setcolor(1, 0x700); /* Color 1: Bright Red */
    Setcolor(2, 0x070); /* Color 2: Bright Green */

    int running = 1;
    unsigned short frame_counter = 0;

    while (running)
    {
        /* 1. Clear the hidden back buffer */
        clear_screen(back_screen);

        /* 2. Draw directly to back_screen (Example: Interleaved bitplane injection) */
        unsigned short *draw_ptr = (unsigned short *)back_screen;

        // Fill the top rows of screen with Color 1 (Red) and Color 2 (Green)
        for (int i = 0; i < 2000; i += 4)
        {
            draw_ptr[i] = 0xFFFF;                     // Plane 0
            draw_ptr[i + 1] = 0xAAAA + frame_counter; // Plane 1
            draw_ptr[i + 2] = 0x0000;                 // Plane 2
            draw_ptr[i + 3] = 0x0000;                 // Plane 3
        }

        /* 3. Wait for electron beam to finish drawing to avoid tearing */
        vsync();

        /* 4. Flip buffers: Make back visible, make front hidden */
        unsigned char *temp = front_screen;
        front_screen = back_screen;
        back_screen = temp;

        Setscreen(back_screen, front_screen, -1);

        frame_counter++;

        /* 5. Check if user pressed a key to exit */
        if (Cconis())
        {
            /* Key is waiting in buffer */
            Bconin(2); /* Read the key to clear the buffer before exiting */
            running = 0;
        }
    }
}

int main()
{
    /* Hide mouse cursor via line-A or VDI if needed, or simply let full screen override */
    Cconws("\033f"); /* TOS escape sequence to hide text cursor */

    init_graphics();
    run_app();
    restore_state();

    Cconws("\033e"); /* TOS escape sequence to show text cursor */
    return 0;
}
