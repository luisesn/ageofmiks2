/*************************************************************
Age of miks 2

main.cpp: archivo principal
*************************************************************/

#include "main.h"

static const Uint32 TARGET_FPS = 25;
static const Uint32 TARGET_FRAME_MS = 1000 / TARGET_FPS;
static const Uint32 MAX_FRAME_DELTA_MS = 250;
static const int MAX_UPDATES_PER_FRAME = 5;

static void actualizar_camara()
{
    switch (game_state.camera_mode)
    {
        case 1: // Camara libre
            if ((mousex>RESX-RESX/20) || (mousey>RESY-RESY/20) || (mousey>32 && mousex<RESX/20 ) || (mousey>32 && mousey<32+(RESY/20)))
            {
                if (tm_mouseover!=99999)
                {
                    if (SDL_GetTicks()-tm_mouseover>250)
                    {
                        if (mousex>RESX-RESX/20 && mousey>32) scrollx+=PASO_SCROLL;
                        if (mousey>RESY-RESY/20) scrolly+=PASO_SCROLL;
                        if (mousey>32 && mousex<RESX/20 ) scrollx-=PASO_SCROLL;
                        if (mousey>32 && mousey<32+(RESY/20)) scrolly-=PASO_SCROLL;
                    }
                } else {
                    tm_mouseover=SDL_GetTicks();
                }
            } else {
                tm_mouseover=99999;
            }
            // Limitar scroll a los bordes del mapa isometrico
            {
                const int SX_MIN = -(ANCHOY-1)*32;
                const int SX_MAX =  (ANCHOX-1)*32 - RESX;
                const int SY_MIN = -32;
                const int SY_MAX = ((ANCHOX-1)+(ANCHOY-1))*16 - RESY;
                if (scrollx < SX_MIN) scrollx = SX_MIN;
                if (scrollx > SX_MAX) scrollx = SX_MAX;
                if (scrolly < SY_MIN) scrolly = SY_MIN;
                if (scrolly > SY_MAX) scrolly = SY_MAX;
            }
            break;
        case 0: // Camara sigue a cursor
            scrollx =(curx - cury)*( 64 /2)-RESX/2;
            scrolly = (curx + cury)*( 32 /2)-RESY/2;
            break;
        case 2: // Camara sigue a objeto
            scrollx =(posx - posy)*( 64 /2)-RESX/2;
            scrolly = (posx + posy)*( 32 /2)-RESY/2;
            break;
        case 3: // Camara sigue a objeto a seleccionar
            scrollx =(ud[seleccionn].x - ud[seleccionn].y)*( 64 /2)-RESX/2;
            scrolly = (ud[seleccionn].x + ud[seleccionn].y)*( 32 /2)-RESX/2;
            break;
    }
}

static void actualizar_simulacion(double &ang)
{
    posx=(int) (5*cos(ang)+49);
    posy=(int) (5*sin(ang)+49);
    ang=ang+3.1415/90;

    actualizar_camara();
    actualizar_ocupacion_uds();
    command_process_queue();
    actualizar_uds();
    actualizar_economia_objetos();
    actualizar_niebla_local();
    for (int j=0; j<jugadores-1; j++) ia[j].main();
}

static void renderizar_frame()
{
    dibujarmapa();
    dibujarobjetos();
    d_spr_cursor();
    gui();
    minimapa_render();
    actualizar();
}




int main (int argc, char *argv[])
{
    
    srand(time(0));
    double ang = 0;
    game_state.running=1;
    game_state.camera_mode=1;
    game_state.build_mode=OBJ_TIPO_CENTRO;
    game_state.selected_id=-1;
    game_state.selected_type=0;
    game_state.mouse_x=RESX/2;
    game_state.mouse_y=RESY/2;
    game_state.cursor_x=ANCHOX/2;
    game_state.cursor_y=ANCHOY/2;
    gs_apply_to_globals();
    seleccionn=2;
    atexit (SDL_Quit);
    inicializar();
    cargar_mapa();
    cargar_sprites();
    crear_unidades();
    crear_objetos();
    
    game_state.running=1;
    gs_apply_to_globals();
    seleccion.n=-1;
    seleccion.tipo=0;
    gs_sync_from_globals();

    jugadores_configurar();
    for (int j=0; j<jugadores-1; j++) ia[j].configurar(j+1);

       textprintf(pantalla, fuente,0,posy,clr_blanco,"Iniciando...");SDL_UpdateWindowSurface(ventana);
       posy+=13;
    //while(!fin) vteclas();
    //SDL_Delay(500);
    vteclas();
    game_state.running=1;
    gs_apply_to_globals();

    Uint32 previous_ticks = SDL_GetTicks();
    Uint32 accumulator_ms = 0;

    while(game_state.running)
    {
        Uint32 frame_start = SDL_GetTicks();
        Uint32 frame_delta = frame_start - previous_ticks;
        previous_ticks = frame_start;

        if (frame_delta > MAX_FRAME_DELTA_MS)
        {
            frame_delta = MAX_FRAME_DELTA_MS;
        }

        accumulator_ms += frame_delta;

        vteclas();

        int updates_this_frame = 0;
        while (accumulator_ms >= TARGET_FRAME_MS && updates_this_frame < MAX_UPDATES_PER_FRAME)
        {
            actualizar_simulacion(ang);
            accumulator_ms -= TARGET_FRAME_MS;
            updates_this_frame++;
        }

        // Si hubo demasiado retraso, descartamos el sobrante para evitar espiral de muerte.
        if (updates_this_frame == MAX_UPDATES_PER_FRAME && accumulator_ms >= TARGET_FRAME_MS)
        {
            accumulator_ms = 0;
        }

        renderizar_frame();

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        game_state.frame_ms = frame_time;
        game_state.fps = (frame_time > 0) ? 1000 / frame_time : 999;
        gs_apply_to_globals();

        if (frame_time < TARGET_FRAME_MS)
        {
            SDL_Delay(TARGET_FRAME_MS - frame_time);
        }
    }
    quitar();
    return 0;
}
