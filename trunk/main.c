#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <windows.h>

/*
 PLEASE NOTE: the program will require SDL.dll which is located in
              dev-c++'s dll directory. You have to copy it to you
			  program's home directory or the path.
 */

/* The screen surface */
SDL_Surface *screen = NULL;


/* This function draws to the screen; replace this with your own code! */
static void
draw ()
{
    static int direction = 0;
    static int value = 0;
    static int which = 0;
    SDL_Rect rect;
    Uint32 color;

    /* Create a black background */
    color = SDL_MapRGB (screen->format, 0, 0, 0);
    SDL_FillRect (screen, NULL, color);

    /* Determine which color the layer should have */
    if (direction == 0)
    {
        value += 2;
        if (value >= 256)
        {
            value = 255;
            direction = 1;
        }
    }
    else
    {
        value -= 2;
        if (value <= 5)
        {
            value = 0;
            direction = 0;
            which++;
            if (which == 5)
                which = 0;
        }
    }

    /* Draw a layer with variable color */
    switch (which)
    {
      case 0:
          color = SDL_MapRGB (screen->format, value, 0, 0);
          break;
      case 1:
          color = SDL_MapRGB (screen->format, 0, value, 0);
          break;
      case 2:
          color = SDL_MapRGB (screen->format, 0, 0, value);
          break;
      case 3:
          color = SDL_MapRGB (screen->format, value, value, value);
          break;
      case 4:
          color = SDL_MapRGB (screen->format, value, 0, value);
          break;
    }

    rect.w = screen->w / 2;
    rect.h = screen->h / 2;
    rect.x = (screen->w / 2) - (rect.w / 2);
    rect.y = (screen->h / 2) - (rect.h / 2);
    SDL_FillRect (screen, &rect, color);


    /* Make sure everything is displayed on screen */
    SDL_Flip (screen);
    /* Don't run too fast */
    SDL_Delay (1);
}


int
main (int argc, char *argv[])
{
    char *msg;
    int done;

    /* Initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        sprintf (msg, "Couldn't initialize SDL: %s\n", SDL_GetError ());
        MessageBox (0, msg, "Error", MB_ICONHAND); 
        free (msg);
        exit (1);
    }
    atexit (SDL_Quit);

    /* Set 640x480 16-bits video mode */
    screen = SDL_SetVideoMode (640, 480, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL)
    {
        sprintf (msg, "Couldn't set 640x480x16 video mode: %s\n",
          SDL_GetError ());
        MessageBox (0, msg, "Error", MB_ICONHAND); 
        free (msg);
        exit (2);
    }
    SDL_WM_SetCaption ("SDL MultiMedia Application", NULL);

    done = 0;
    while (!done)
    {
        SDL_Event event;

        /* Check for events */
        while (SDL_PollEvent (&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                break;
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }

        /* Draw to screen */
        draw ();
    }

    return 0;
}
