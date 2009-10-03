/*************************************************************
Age of miks 2

func_programa.h: Funciones externas al juego
*************************************************************/

void quitar(void)
{
     //fflush(NULL);
     TTF_CloseFont( fuente );
     TTF_Quit();
     fin=0;
}

void cargar_sprites()
{
     #ifdef DEBUG
            printf ("Cargando sprites...\r\n");
     #endif
     
     #ifdef DEBUG
            printf ("Cargando sprites mapa...");
     #endif
     spr_mapa=cs("graficos/spr_mapa.bmp");
    spr_gui=cs("graficos/spr_gui.bmp");
  
     #ifdef DEBUG
            printf ("Cargados\r\n");
     #endif

 }

void tabla_senoscosenos()
{
     	 /* Genera tabla de senos y cosenos */
    float a2;
    for (int a=0; a<361; a++)
    {
        a2=a;
        se[a]=(sin(a2*0.017453292519943295769236907684886));
        co[a]=(cos(a2*0.017453292519943295769236907684886));
        //printf ("%u,%f,%f\r\n",a,co[a],se[a]);
    }
} 

void inicializar ()
{

     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1)
     {
        fprintf (stderr, "Error: No se pudieron inicializar subsistemas: %s\r\n",SDL_GetError());
        SDL_Quit();
     }
    TTF_Init();
    fuente = TTF_OpenFont( "./tahoma.ttf", 10);
    pantalla=SDL_SetVideoMode(RESX, RESY, 16,  SDL_HWSURFACE); //SDL_HWSURFACESDL_FULLSCREEN |


Mix_OpenAudio(44100,AUDIO_S16,2,4096);
//musica=Mix_LoadMUS("bsotelecosa.ogg");
payum=Mix_LoadWAV("phaser.wav");
//explosion=Mix_LoadWAV("explotanave.wav");
Mix_VolumeMusic (volumen);
//Mix_PlayMusic (musica,-1);

#ifdef WIN32
       SDL_WM_SetCaption("Age of Miks II", NULL);
#endif
       //SDL_ShowCursor (SDL_DISABLE);
       tabla_senoscosenos();
}

void gui()
{
    area2.y=0;
    area2.x=RESX-320;
         
         
     if (area2.x<RESX && area2.x>(-65) && area2.y>(-65) && area2.y<RESY)
     {
         area1.x=0;
         area1.y=menu*32;
         area1.w=320;
         area1.h=32;
         area2.w=320;
         area2.h=32;
         SDL_BlitSurface(spr_gui, &area1, pantalla, &area2);
        //textprintf(pantalla, fuente,area2.x,area2.y,clr_blanco,"%d->%d",orden.tipo, orden.s);
    }
}
