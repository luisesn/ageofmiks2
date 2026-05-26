/*************************************************************
Age of miks 2

func_programa.h: Funciones externas al juego
*************************************************************/

void cargar_balance()
{
    FILE *f = fopen("balance.cfg", "r");
    if (!f) return;

    char linea[256];
    char clave[128];
    int valor = 0;

    while (fgets(linea, sizeof(linea), f))
    {
        if (linea[0] == '#' || linea[0] == '\n' || linea[0] == '\r') continue;
        if (sscanf(linea, " %127[^=]=%d", clave, &valor) != 2) continue;

        if (strcmp(clave, "precio_aldeano") == 0) cfg_precio_aldeano = valor;
        else if (strcmp(clave, "precio_soldado") == 0) cfg_precio_soldado = valor;
        else if (strcmp(clave, "alcance_aldeano") == 0) cfg_alcance_aldeano = valor;
        else if (strcmp(clave, "alcance_soldado") == 0) cfg_alcance_soldado = valor;
        else if (strcmp(clave, "resistencia_aldeano") == 0) cfg_resistencia_aldeano = valor;
        else if (strcmp(clave, "resistencia_soldado") == 0) cfg_resistencia_soldado = valor;
        else if (strcmp(clave, "ataque_aldeano") == 0) cfg_ataque_aldeano = valor;
        else if (strcmp(clave, "ataque_soldado") == 0) cfg_ataque_soldado = valor;
        else if (strcmp(clave, "defensa_aldeano") == 0) cfg_defensa_aldeano = valor;
        else if (strcmp(clave, "defensa_soldado") == 0) cfg_defensa_soldado = valor;
    }

    fclose(f);
}

void quitar(void)
{
    if (fuente)   { TTF_CloseFont(fuente); fuente = NULL; }
    if (payum)    { Mix_FreeChunk(payum);  payum  = NULL; }
    if (musica)   { Mix_FreeMusic(musica); musica = NULL; }
    if (spr_mapa) { SDL_FreeSurface(spr_mapa); spr_mapa = NULL; }
    if (spr_gui)  { SDL_FreeSurface(spr_gui);  spr_gui  = NULL; }
    Mix_CloseAudio();
    TTF_Quit();
    if (ventana)  { SDL_DestroyWindow(ventana); ventana = NULL; }
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
    ventana = SDL_CreateWindow("Age of Miks II",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               RESX, RESY, 0);
    pantalla = SDL_GetWindowSurface(ventana);

Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
//musica=Mix_LoadMUS("bsotelecosa.ogg");
payum=Mix_LoadWAV("phaser.wav");
//explosion=Mix_LoadWAV("explotanave.wav");
Mix_VolumeMusic(volumen);
//Mix_PlayMusic(musica,-1);
       //SDL_ShowCursor (SDL_DISABLE);
    cargar_balance();
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
