/*************************************************************
Age of miks 2

main.h: funciones generales e includes
*************************************************************/


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>
#include <algorithm>
using namespace std;
//#include <SDL/SDL_gfxPrimitives.h>

void d_escaner(int x, int y);
#include "defs.h"
#include "funcs_graficas.h"
#include "clases.h"
#include "funcs_programa.h"
#include "ia.h"

void jugadores_configurar()
{
    for (int j=0; j<jugadores; j++)
    {
        crear_aldeano(j);        crear_aldeano(j);
    }

}

void cargar_mapa()
{
     #ifdef DEBUG
            printf ("Cargando mapa...");
     #endif
     int c;
     f_mapa = fopen("mapa.txt", "r");
     if (!f_mapa) { printf("Error: no se pudo abrir mapa.txt\r\n"); return; }
     for (int tempy=0; tempy<ANCHOY; tempy++)
     {
              for (int tempx=0; tempx<ANCHOX; tempx++)
              {
                      c = fgetc(f_mapa);
                      mapa[tempx][tempy] = (c != EOF) ? c - '0' : 0;
                  obstaculos[tempx][tempy] = 0;
              }
              // Consumir fin de linea (soporta \r\n y \n)
              while ((c = fgetc(f_mapa)) != EOF && c != '\n') {}
     }
     fclose(f_mapa);
     #ifdef DEBUG
            printf ("Cargado.\r\n");
     #endif
}

int unidad_vacia()
{
    for (int j=0; j<uds; j++)
    {
        if (!ud[j].activa()) return j;
    }
    return -1;
}

int crear_aldeano(int id_p)
{
    int id=-1;
    if (jugador[id_p].quitar_recursos(cfg_precio_aldeano))
    {
        id=unidad_vacia();
        if (id!=-1)
        {
            ud[id].ptos_resistencia=cfg_resistencia_aldeano;
            ud[id].x=obj[jugador[id_p].objeto_centro].x+1;
            ud[id].y=obj[jugador[id_p].objeto_centro].y+1;
            ud[id].cx=5;
            ud[id].cy=5;
            ud[id].tipo=UD_TIPO_ALDEANO;
            ud[id].alcance_vision=cfg_alcance_aldeano;
            ud[id].actitud=ACTITUD_PASIVA;
            ud[id].id_jugador=id_p;
            ud[id].ptos_ataque=cfg_ataque_aldeano;
            ud[id].ptos_defensa=cfg_defensa_aldeano;
            return id;
        }
    }
    return -1;
}

int crear_soldado(int id_p)
{
    int id=-1;
    if (jugador[id_p].quitar_recursos(cfg_precio_soldado))
    {
        id=unidad_vacia();
        if (id!=-1)
        {
            ud[id].ptos_resistencia=cfg_resistencia_soldado;
            ud[id].x=obj[jugador[id_p].objeto_centro].x+1;
            ud[id].y=obj[jugador[id_p].objeto_centro].y+1;
            ud[id].tipo=UD_TIPO_SOLDADO;
            ud[id].alcance_vision=cfg_alcance_soldado;
            ud[id].actitud=ACTITUD_AGRESIVA;
            ud[id].id_jugador=id_p;
            ud[id].ptos_ataque=cfg_ataque_soldado;
            ud[id].ptos_defensa=cfg_defensa_soldado;
            return id;
        }
    }
    return -1;
}

void crear_unidades()
{
    /*
    for (int temp=0; temp<2; temp++)
      {
          ud[temp].x=rand()%10;
          ud[temp].y=rand()%10;
          ud[temp].cx=5;
          ud[temp].cy=5;
          ud[temp].tipo=UD_TIPO_ALDEANO;
          ud[temp].resistencia=100;
          if (temp>0) ud[temp].id_jugador=1;
      }
*/
}

int ver_memoria=0;

void gs_apply_to_globals()
{
    fin = game_state.running ? 0 : 1;
    camara = game_state.camera_mode;
    fps = game_state.fps;
    tiempo = game_state.frame_ms;
    modo_construccion_tipo = game_state.build_mode;
    seleccion.n = game_state.selected_id;
    seleccion.tipo = game_state.selected_type;
    mousex = game_state.mouse_x;
    mousey = game_state.mouse_y;
    curx = game_state.cursor_x;
    cury = game_state.cursor_y;
}

void gs_sync_from_globals()
{
    game_state.running = (fin==0);
    game_state.camera_mode = camara;
    game_state.fps = fps;
    game_state.frame_ms = tiempo;
    game_state.build_mode = modo_construccion_tipo;
    game_state.selected_id = seleccion.n;
    game_state.selected_type = seleccion.tipo;
    game_state.mouse_x = mousex;
    game_state.mouse_y = mousey;
    game_state.cursor_x = curx;
    game_state.cursor_y = cury;
}

int construir_objeto(int x, int y, int tipo, int id_jugador);

int command_enqueue(int tipo, int unidad, int jugador, int x, int y, int extra, int prioridad=0)
{
    // Deduplicacion: actualiza orden de movimiento existente para la misma unidad.
    if (tipo==CMD_TIPO_MOVER_UNIDAD && unidad>=0)
    {
        for (int i=0; i<command_queue_size; i++)
        {
            if (command_queue[i].tipo==CMD_TIPO_MOVER_UNIDAD && command_queue[i].unidad==unidad)
            {
                command_queue[i].x = x;
                command_queue[i].y = y;
                if (prioridad > command_queue[i].prioridad) command_queue[i].prioridad = prioridad;
                return 1;
            }
        }
    }

    // Deduplicacion de ordenes macro repetidas del mismo jugador.
    if (tipo==CMD_TIPO_EXPLORAR_TODOS || tipo==CMD_TIPO_EXPLORAR_SOLDADOS || tipo==CMD_TIPO_RECOGER_ALDEANOS)
    {
        for (int i=0; i<command_queue_size; i++)
        {
            if (command_queue[i].tipo==tipo && command_queue[i].jugador==jugador)
            {
                if (prioridad > command_queue[i].prioridad) command_queue[i].prioridad = prioridad;
                return 1;
            }
        }
    }

    if (command_queue_size >= MAX_COMMAND_QUEUE) return 0;
    command_queue[command_queue_size].tipo = tipo;
    command_queue[command_queue_size].unidad = unidad;
    command_queue[command_queue_size].jugador = jugador;
    command_queue[command_queue_size].x = x;
    command_queue[command_queue_size].y = y;
    command_queue[command_queue_size].extra = extra;
    command_queue[command_queue_size].prioridad = prioridad;
    command_queue[command_queue_size].secuencia = command_queue_sequence++;
    command_queue_size++;
    return 1;
}

bool command_compare(const def_command &a, const def_command &b)
{
    if (a.prioridad != b.prioridad) return a.prioridad > b.prioridad;
    return a.secuencia < b.secuencia;
}

void command_process_queue()
{
    if (command_queue_size>1)
    {
        sort(command_queue, command_queue + command_queue_size, command_compare);
    }

    for (int i=0; i<command_queue_size; i++)
    {
        def_command &cmd = command_queue[i];
        switch (cmd.tipo)
        {
            case CMD_TIPO_MOVER_UNIDAD:
                if (cmd.unidad>=0 && cmd.unidad<uds && ud[cmd.unidad].activa())
                {
                    ud[cmd.unidad].ira(cmd.x, cmd.y);
                }
                break;
            case CMD_TIPO_CONSTRUIR:
                construir_objeto(cmd.x, cmd.y, cmd.extra, cmd.jugador);
                break;
            case CMD_TIPO_EXPLORAR_TODOS:
                for (int t=0; t<uds; t++) if (ud[t].activa()) ud[t].ira(rand()%ANCHOX, rand()%ANCHOY);
                break;
            case CMD_TIPO_EXPLORAR_SOLDADOS:
                for (int t=0; t<uds; t++) if (ud[t].tipo==UD_TIPO_SOLDADO && ud[t].activa()) ud[t].explorar();
                break;
            case CMD_TIPO_RECOGER_ALDEANOS:
                for (int t=0; t<uds; t++)
                {
                    if (ud[t].id_jugador==cmd.jugador && ud[t].tipo==UD_TIPO_ALDEANO && ud[t].activa())
                    {
                        ud[t].explorar_recursos();
                    }
                }
                break;
            case CMD_TIPO_CANCELAR_UNIDAD:
                if (cmd.unidad>=0 && cmd.unidad<uds && ud[cmd.unidad].activa())
                {
                    ud[cmd.unidad].orden.realizada=0;
                    ud[cmd.unidad].orden.tipo=0;
                    ud[cmd.unidad].sprite=0;
                }
                break;
            default:
                break;
        }
    }
    command_queue_size=0;
}

void actualizar_niebla_local()
{
    for (int y=0; y<ANCHOY; y++)
    {
        for (int x=0; x<ANCHOX; x++)
        {
            vis_actual[x][y]=0;
        }
    }

    for (int j=0; j<uds; j++)
    {
        if (!ud[j].activa() || ud[j].id_jugador!=JUGADOR_LOCAL) continue;
        int r = (ud[j].alcance_vision > 1) ? ud[j].alcance_vision : 3;

        for (int dy=-r; dy<=r; dy++)
        {
            for (int dx=-r; dx<=r; dx++)
            {
                int xx = ud[j].x + dx;
                int yy = ud[j].y + dy;
                if (xx<0 || xx>=ANCHOX || yy<0 || yy>=ANCHOY) continue;
                if (abs(dx)+abs(dy) > r) continue;
                vis_actual[xx][yy]=1;
                vis_explorado[xx][yy]=1;
            }
        }
    }
}

void actualizar ()
{
    int u[]={0,0,0,0,0,0,0,0};
    int c=0;
    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa()) u[ud[j].id_jugador]++;
    }
    textprintf(pantalla, fuente,0,0,clr_blanco,"CAM:%d FPS:%d  Recursos:%d  Sel:%d  Build:%s [B centro/N almacen] [1=mem 2=debug]",
        game_state.camera_mode, game_state.fps, jugador[JUGADOR_LOCAL].numero_recursos(), game_state.selected_id,
        (game_state.build_mode==OBJ_TIPO_CENTRO)?"centro":"almacen");
    textprintf(pantalla, fuente,0,14,clr_blanco,"Uds  P0:%d P1:%d P2:%d P3:%d",u[0], u[1], u[2], u[3]);
    if (ver_memoria==1)
    for (int i=1; i<jugadores; i++)
    {
        c=0;
        for (int j=0; j<MAX_MEM; j++)
        {
            if (jugador[i].mem.m[j].activa==1)
            {
                switch (jugador[i].mem.m[j].tipo)
                {
                    case MEMORIA_AVISO:
                        c++; 
                        textprintf (pantalla, fuente, 150*(i-1), 14*(c+2), clr_azul, "M(%d,[%d]>%d)",j,jugador[i].mem.m[j].tipo, jugador[i].mem.m[j].o);
                        break;
                    case MEMORIA_OBJETO_RECURSOS:
                        c++; 
                        textprintf (pantalla, fuente, 150*(i-1), 14*(c+2), clr_verde, "M(%d,[%d]>%d)",j,jugador[i].mem.m[j].tipo, jugador[i].mem.m[j].o);
                        break;
                    case MEMORIA_UNIDAD_ENEMIGA:
                        c++; 
                        textprintf (pantalla, fuente, 150*(i-1), 14*(c+2), clr_rojo, "M(%d,%d (%d))",j, jugador[i].mem.m[j].o, ud[jugador[i].mem.m[j].o].ptos_resistencia);
                        break;
                }
            }
        }
    }
    SDL_UpdateWindowSurface(ventana);
    /* SDL_Rect rect;
     rect = (SDL_Rect) {0,0,RESX,RESY};
     SDL_FillRect(pantalla, &rect, SDL_MapRGB(pantalla->format, 255, 0, 0));*/
    SDL_FillRect (pantalla, NULL, 0);

    gs_sync_from_globals();

}

void cambiar_camara()
{
    game_state.camera_mode++;
    if (game_state.camera_mode>3) {game_state.camera_mode=0;}
    gs_apply_to_globals();
 }

void actualizar_uds()
{
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].activa()) ud[temp].procesar_orden();
    }
}

void actualizar_ocupacion_uds()
{
    for (int y=0; y<ANCHOY; y++)
    {
        for (int x=0; x<ANCHOX; x++)
        {
            ocupacion_uds[x][y]=0;
        }
    }

    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa() && ud[j].x>=0 && ud[j].x<ANCHOX && ud[j].y>=0 && ud[j].y<ANCHOY)
        {
            ocupacion_uds[ud[j].x][ud[j].y]++;
        }
    }
}

int casilla_construible(int x, int y)
{
    if (x<0 || x>=ANCHOX || y<0 || y>=ANCHOY) return 0;
    if (obstaculos[x][y] != 0) return 0;
    if (ocupacion_uds[x][y] > 0) return 0;

    for (int j=0; j<uds; j++)
    {
        if (obj[j].construido && obj[j].x==x && obj[j].y==y) return 0;
    }

    return 1;
}

int construir_objeto(int x, int y, int tipo, int id_jugador)
{
    if (!casilla_construible(x, y)) return -1;

    if (id_jugador>=0)
    {
        if (tipo==OBJ_TIPO_CENTRO)
        {
            if (jugador[id_jugador].quitar_recursos(cfg_precio_centro)!=1) return -1;
        }
        if (tipo==OBJ_TIPO_ALMACEN)
        {
            if (jugador[id_jugador].quitar_recursos(cfg_precio_almacen)!=1) return -1;
        }
    }

    int objn=-1;
    for (int temp=0; temp<uds; temp++)
    {
        if (!obj[temp].construido) {objn=temp; break;}
    }
    if (objn==uds || objn==-1) return -1;
    obj[objn].id_jugador=id_jugador;
    obj[objn].construido=1;
    obj[objn].tipo=tipo;
    obj[objn].x=x;
    obj[objn].y=y;
    if (tipo==OBJ_TIPO_CENTRO) obj[objn].propiedad_1=10000;
    else obj[objn].propiedad_1=0;
    return objn;
}

void actualizar_economia_objetos()
{
    static Uint32 ultimo_tick = 0;
    Uint32 ahora = SDL_GetTicks();
    if (ahora - ultimo_tick < 1000) return;
    ultimo_tick = ahora;

    for (int j=0; j<uds; j++)
    {
        if (!obj[j].construido) continue;
        if (obj[j].tipo!=OBJ_TIPO_ALMACEN) continue;
        if (obj[j].id_jugador<0 || obj[j].id_jugador>=jugadores) continue;

        int c = jugador[obj[j].id_jugador].objeto_centro;
        if (c>=0 && c<uds && obj[c].construido)
        {
            obj[c].propiedad_1 += cfg_ingreso_almacen;
        }
    }
}

void crear_objetos()
{
    for (int j=0; j<jugadores; j++)
    {
        obj[j].construido=1;
        obj[j].x=rand()%(ANCHOX-10)+5;
        obj[j].y=rand()%(ANCHOY-10)+5;
        obj[j].tipo=0;
        obj[j].id_jugador=0;
        jugador[j].objeto_centro=j;
    }

    for (int y=0; y<ANCHOY; y++)
    {
        for (int x=0; x<ANCHOX; x++)
        {
            vis_actual[x][y]=0;
            vis_explorado[x][y]=0;
        }
    }

    for (int temp=0; temp<40; temp++)
      {
            construir_objeto(rand()%ANCHOX, rand()%ANCHOY, 1,-1);
      }
}


int minimapa_contiene(int mx, int my);
void minimapa_mover_camara(int mx, int my);



void vteclas()
{
SDL_Event event;
int temp;

while (SDL_PollEvent (&event))
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode==SDL_SCANCODE_B) game_state.build_mode=OBJ_TIPO_CENTRO;
            if (event.key.keysym.scancode==SDL_SCANCODE_N) game_state.build_mode=OBJ_TIPO_ALMACEN;
            gs_apply_to_globals();
        	break;
        case SDL_KEYUP:
        	break;
        /*	case SDL_JOYBUTTONDOWN:
        	break;
        case SDL_JOYBUTTONUP:
             //if (tecla==event.jbutton.button) { teclap--;  if (teclap<=0) tecla=0; teclap=0;}
        	break;*/
        case SDL_QUIT:
               game_state.running=0;
               gs_apply_to_globals();
        	break;
        default:
        	break;
        case SDL_MOUSEMOTION:
               game_state.mouse_x = event.motion.x;
               game_state.mouse_y = event.motion.y;
               gs_apply_to_globals();
        	//Calculamos las coordenadas de pantalla a partir de las de la casilla
        	//y el desplazamiento de la camara (scrollX, scrollY)
        	// Ecs. originales de c�lculo de posici�n de sprites del mapa
            //px = (x - y)*(32) - scrollx;
            //py = (x + y)*(16) - scrolly;
            //Sacamos (x,y)
        	//px= 32*x - 32*y - scrollx;
        	//py = 16*x + 16*y - scrolly;
        	//px+2*py = 64*x  - scrolly - scrollx;
    		game_state.cursor_x= (mousex+2*mousey+scrollx+2*scrolly-96)/64;
        	//px-2*py= -64*y -scrollx + scrolly;
            game_state.cursor_y=(2*mousey-mousex-scrollx+2*scrolly-32)/64;
            gs_apply_to_globals();
        	//Dibuja el tile en la pantalla
        	//d_spr_mapa(px, py, mapa[x][y]);
             break;
        case SDL_MOUSEBUTTONDOWN:
            //If the left mouse button was pressed
            if (event.button.button == SDL_BUTTON_LEFT )
            {
                    if (minimapa_contiene(mousex, mousey))
                    {
                        minimapa_mover_camara(mousex, mousey);
                        break;
                    }
                    if (mousey>32) //No estamos pulsando sobre la barra
                    {
                        seleccion.n=-1;
                        //Buscamos una unidad en curx,cury
                        for (temp=0; temp<uds; temp++)
                        {
                            if (ud[temp].id_jugador==0)
                            if (ud[temp].x==curx)
                                if (ud[temp].y==cury)
                                {
                                   seleccion.n=temp;
                                   seleccion.tipo=0;
                                   break;
                                }
                        }
                    if (seleccion.n==-1) //Si no se ha seleccionado unidad, entonces buscamos un objeto.
                        for (temp=0; temp<uds; temp++)
                        {
                            if (obj[temp].x==curx)
                            {
                                if (obj[temp].y==cury)
                                {
                                   seleccion.n=temp;
                                   seleccion.tipo=1;
                                   break;
                                }
                            }
                        }
                } else {
                    switch (10-(RESX-mousex)/32)//Boton pulsado
                    {
                        case 1: //Bot�n 1
                            for (int t=0; t<uds; t++)
                            {
                                if (ud[t].id_jugador==JUGADOR_LOCAL && ud[t].tipo==UD_TIPO_ALDEANO && ud[t].activa())
                                {
                                    command_enqueue(CMD_TIPO_RECOGER_ALDEANOS, -1, JUGADOR_LOCAL, 0, 0, 0, 5);
                                    break;
                                }
                            }
                            break;
                        case 2: // Boton 2: modo construccion centro
                            game_state.build_mode=OBJ_TIPO_CENTRO;
                            gs_apply_to_globals();
                            break;
                        case 3: // Boton 3: modo construccion almacen
                            game_state.build_mode=OBJ_TIPO_ALMACEN;
                            gs_apply_to_globals();
                            break;
                        case 10: //Bot�n 10
                            game_state.running=0;
                            gs_apply_to_globals();
                            break;
                    }
                }         
				game_state.selected_id = seleccion.n;
				game_state.selected_type = seleccion.tipo;
                gs_apply_to_globals();
            } else {
                   if ( event.button.button == SDL_BUTTON_RIGHT)
                   {
                        //Si selecion!=-1 ordenamos algo a la unidad
                        if (seleccion.n!=-1)
                        {
                            switch (seleccion.tipo)
                            {
                                case 0:
                                    //ud[seleccion].x=curx;
                                    //ud[seleccion].y=cury;
                                    command_enqueue(CMD_TIPO_MOVER_UNIDAD, seleccion.n, JUGADOR_LOCAL, curx, cury, 0, 10);
                                    break;
                            }
                        } else {
                            command_enqueue(CMD_TIPO_CONSTRUIR, -1, JUGADOR_LOCAL, curx, cury, game_state.build_mode, 20);
                        }
                   }
            }
            break;
    }
}


const Uint8 *teclas = SDL_GetKeyboardState(NULL);
if (teclas[SDL_SCANCODE_ESCAPE]) { game_state.running=0; gs_apply_to_globals(); }
   if (teclas[SDL_SCANCODE_UP])
   {
        scrolly-=20;
   }
   if (teclas[SDL_SCANCODE_DOWN])
   {
        scrolly+=20;
   }
   if (teclas[SDL_SCANCODE_RIGHT])
   {
        scrollx+=20;
   }
   if (teclas[SDL_SCANCODE_LEFT])
   {
        scrollx-=20;
   }

   if (teclas[SDL_SCANCODE_W])
   {
        game_state.cursor_y--;
        gs_apply_to_globals();
   }
   if (teclas[SDL_SCANCODE_S])
   {
        game_state.cursor_y++;
        gs_apply_to_globals();
   }
   if (teclas[SDL_SCANCODE_A])
   {
        game_state.cursor_x--;
        gs_apply_to_globals();
   }
   if (teclas[SDL_SCANCODE_D])
   {
        game_state.cursor_x++;
        gs_apply_to_globals();
   }

   if (teclas[SDL_SCANCODE_Q])
   {
    game_state.running=0;
    gs_apply_to_globals();
   }
   if (teclas[SDL_SCANCODE_C])
   {
    cambiar_camara();
   }
   if (teclas[SDL_SCANCODE_E])
   {
       command_enqueue(CMD_TIPO_EXPLORAR_TODOS, -1, JUGADOR_LOCAL, 0, 0, 0);
   }
   if (teclas[SDL_SCANCODE_R])
   {
        command_enqueue(CMD_TIPO_RECOGER_ALDEANOS, -1, JUGADOR_LOCAL, 0, 0, 0);
   }
   if (teclas[SDL_SCANCODE_T])
   {
        command_enqueue(CMD_TIPO_EXPLORAR_SOLDADOS, -1, JUGADOR_LOCAL, 0, 0, 0);
   }
   if (teclas[SDL_SCANCODE_X])
   {
       if (game_state.selected_id>=0 && game_state.selected_type==0)
       {
          command_enqueue(CMD_TIPO_CANCELAR_UNIDAD, game_state.selected_id, JUGADOR_LOCAL, 0, 0, 0, 30);
       }
   }
   if (teclas[SDL_SCANCODE_1])
   {
        if (ver_memoria==1) {ver_memoria=0;} else {ver_memoria=1;}
   }
   if (teclas[SDL_SCANCODE_2])
   {
        ver_debug_uds = !ver_debug_uds;
   }

}


void d_spr_mapa (int x, int y, int tipo) //Dibujo un sprite de mapa en las coordenadas x,y
{
     if (x<RESX && x>(-65) && y>(-65) && y<RESY)
     {
         area1.x=tipo*64;
         area1.y=0;
         area1.w=64;
         area1.h=64;
         area2.x=x;
         area2.y=y;
         area2.w=64;
         area2.h=64;
         SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
     }
 }

void d_spr_neco (int x, int y) //Dibujo un sprite de �eco en (x,y)
{
     if (x<RESX && x>(-65) && y>(-65) && y<RESY)
     {
         area1.x=0;
         area1.y=64;
         area1.w=64;
         area1.h=64;
         area2.x=x;
         area2.y=y;
         area2.w=64;
         area2.h=64;
         SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
     }
 }

void d_escaner(int x, int y)
{
     if (x<ANCHOX && x>(-1) && y>(-1) && y<ANCHOY)
     {
         area1.x=384;
         area1.y=0;
         area1.w=64;
         area1.h=64;
         area2.x=(x - y)*(32) - scrollx;
         area2.y=(x + y)*(16) - scrolly;
         area2.w=64;
         area2.h=64;
         SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
     }
 }

void d_spr_cursor()
{
     if (curx<ANCHOX && curx>(-1) && cury>(-1) && cury<ANCHOY)
     {
         area1.x=384;
         area1.y=0;
         area1.w=64;
         area1.h=64;
         area2.x=(curx - cury)*(32) - scrollx;
         area2.y=(curx + cury)*(16) - scrolly;
         area2.w=64;
         area2.h=64;
         SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
     }
 }
 
 void d_spr_sel()
{
    if (seleccion.n!=-1) 
    {
        switch (seleccion.tipo)
        {
            case 0:
                if (ud[seleccion.n].x<ANCHOX && ud[seleccion.n].x>(-1) && ud[seleccion.n].y>(-1) && ud[seleccion.n].y<ANCHOY)
                {
                    area1.x=448;
                    area1.y=0;
                    area1.w=64;
                    area1.h=64;
                    area2.x=(ud[seleccion.n].x - ud[seleccion.n].y)*(32) - scrollx;
                    area2.y=(ud[seleccion.n].x + ud[seleccion.n].y)*(16) - scrolly;
                    area2.w=64;
                    area2.h=64;
                    SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
                }
                break;
            case 1:
                if (obj[seleccion.n].x<ANCHOX && obj[seleccion.n].x>(-1) && obj[seleccion.n].y>(-1) && obj[seleccion.n].y<ANCHOY)
                {
                    area1.x=448;
                    area1.y=0;
                    area1.w=64;
                    area1.h=64;
                    area2.x=(obj[seleccion.n].x - obj[seleccion.n].y)*(32) - scrollx;
                    area2.y=(obj[seleccion.n].x + obj[seleccion.n].y)*(16) - scrolly;
                    area2.w=64;
                    area2.h=64;
                    SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
                }
                break;
                break;
        }
    }
 }

void dibujarmapa () {

	int x, y;	//Casilla del mapa
	int px, py; //Coordenadas en pantalla
	//Por cada casilla del mapa
		for (x = 0; x < ANCHOX; x++) {
	for (y = 0; y < ANCHOY; y++) {
            if (!vis_explorado[x][y]) continue;
			//Calculamos las coordenadas de pantalla a partir de las de la casilla
			//y el desplazamiento de la camara (scrollX, scrollY)
			px = (x - y)*(32) - scrollx;
			py = (x + y)*(16) - scrolly;

			//Dibuja el tile en la pantalla
			d_spr_mapa(px, py, mapa[x][y]);

            if (!vis_actual[x][y])
            {
                SDL_Rect fog_rect;
                fog_rect.x = px + 8;
                fog_rect.y = py + 8;
                fog_rect.w = 48;
                fog_rect.h = 48;
                if (fog_rect.x < RESX && fog_rect.y < RESY && fog_rect.x + fog_rect.w > 0 && fog_rect.y + fog_rect.h > 0)
                {
                    if (spr_fog)
                    {
                        SDL_BlitSurface(spr_fog, NULL, pantalla, &fog_rect);
                    }
                    else
                    {
                        Uint32 fog_color = SDL_MapRGB(pantalla->format, 16, 16, 16);
                        SDL_FillRect(pantalla, &fog_rect, fog_color);
                    }
                }
            }
		}
	}

}


void dibujarobjetos ()
{
     	//int px, py,temp2; //Coordenadas en pantalla
		//	px = (posx - posy)*( 64 /2) - scrollx;
		//	py = (posx + posy)*( 32 /2) - scrolly;
        /*for (temp=0; temp<uds; temp++)
        {
             temp2=ud[temp].x+rand()%2-rand()%2;
             if (temp2>0 && temp2<ANCHOX) ud[temp].x=temp2;
             temp2=ud[temp].y+rand()%2-rand()%2;
             if (temp2>0 && temp2<ANCHOY) ud[temp].y=temp2;
         }*/
         //Dibujar el sprite de selecci�n.
         d_spr_sel();
         //Hay que cambiar el 1 por uds


         for (int temp=0; temp<uds; temp++)
         {
            if (obj[temp].construido)
            {
                obj[temp].dibujar();
            }
            if (ud[temp].activa())
            {
                ud[temp].dibujar();
            }
         }


         //Dibuja el tile en la pantalla
         //d_spr_neco(px, py);
}

int minimapa_scale() { return 2; }
int minimapa_w() { return ANCHOX * minimapa_scale(); }
int minimapa_h() { return ANCHOY * minimapa_scale(); }
int minimapa_x() { return RESX - minimapa_w() - 8; }
int minimapa_y() { return RESY - minimapa_h() - 8; }

int minimapa_contiene(int mx, int my)
{
    return (mx>=minimapa_x() && mx<minimapa_x()+minimapa_w() && my>=minimapa_y() && my<minimapa_y()+minimapa_h());
}

void limitar_scroll_mapa()
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

void minimapa_mover_camara(int mx, int my)
{
    int s = minimapa_scale();
    int mapx = (mx - minimapa_x()) / s;
    int mapy = (my - minimapa_y()) / s;
    if (mapx<0) mapx=0; if (mapx>=ANCHOX) mapx=ANCHOX-1;
    if (mapy<0) mapy=0; if (mapy>=ANCHOY) mapy=ANCHOY-1;

    scrollx = (mapx - mapy)*32 - RESX/2;
    scrolly = (mapx + mapy)*16 - RESY/2;
    limitar_scroll_mapa();
}

void minimapa_render()
{
    int mmx = minimapa_x();
    int mmy = minimapa_y();
    int s = minimapa_scale();

    SDL_Rect bg;
    bg.x = mmx-2;
    bg.y = mmy-2;
    bg.w = minimapa_w()+4;
    bg.h = minimapa_h()+4;
    SDL_FillRect(pantalla, &bg, SDL_MapRGB(pantalla->format, 8, 8, 8));

    for (int y=0; y<ANCHOY; y++)
    {
        for (int x=0; x<ANCHOX; x++)
        {
            SDL_Rect p;
            p.x = mmx + x*s;
            p.y = mmy + y*s;
            p.w = s;
            p.h = s;

            Uint32 color = SDL_MapRGB(pantalla->format, 20, 80, 20);
            switch (mapa[x][y])
            {
                case 0: color = SDL_MapRGB(pantalla->format, 26, 96, 26); break;
                case 1: color = SDL_MapRGB(pantalla->format, 80, 80, 80); break;
                case 2: color = SDL_MapRGB(pantalla->format, 160, 130, 90); break;
                case 3: color = SDL_MapRGB(pantalla->format, 36, 120, 36); break;
                case 4: color = SDL_MapRGB(pantalla->format, 24, 90, 140); break;
                case 5: color = SDL_MapRGB(pantalla->format, 120, 120, 40); break;
                default: break;
            }
            SDL_FillRect(pantalla, &p, color);
        }
    }

    for (int j=0; j<uds; j++)
    {
        if (!obj[j].construido) continue;
        SDL_Rect p;
        p.x = mmx + obj[j].x*s;
        p.y = mmy + obj[j].y*s;
        p.w = s;
        p.h = s;
        Uint32 cobj = (obj[j].id_jugador==JUGADOR_LOCAL)
            ? SDL_MapRGB(pantalla->format, 255, 255, 0)
            : SDL_MapRGB(pantalla->format, 180, 120, 0);
        SDL_FillRect(pantalla, &p, cobj);
    }

    for (int j=0; j<uds; j++)
    {
        if (!ud[j].activa()) continue;
        SDL_Rect p;
        p.x = mmx + ud[j].x*s;
        p.y = mmy + ud[j].y*s;
        p.w = s;
        p.h = s;
        Uint32 cud = SDL_MapRGB(pantalla->format, 255, 0, 0);
        if (ud[j].id_jugador==0) cud = SDL_MapRGB(pantalla->format, 0, 255, 255);
        if (ud[j].id_jugador==1) cud = SDL_MapRGB(pantalla->format, 255, 80, 80);
        if (ud[j].id_jugador==2) cud = SDL_MapRGB(pantalla->format, 80, 255, 80);
        if (ud[j].id_jugador==3) cud = SDL_MapRGB(pantalla->format, 255, 180, 80);
        SDL_FillRect(pantalla, &p, cud);
    }

    // Rectangulo de camara aproximado usando las 4 esquinas de pantalla.
    const int sx[4] = {0, RESX-1, 0, RESX-1};
    const int sy[4] = {32, 32, RESY-1, RESY-1};
    float minx=99999.0f, miny=99999.0f, maxx=-99999.0f, maxy=-99999.0f;
    for (int i=0; i<4; i++)
    {
        float px = (float)(sx[i] + scrollx);
        float py = (float)(sy[i] + scrolly);
        float mx = px/64.0f + py/32.0f;
        float my = py/32.0f - px/64.0f;
        if (mx < minx) minx = mx;
        if (mx > maxx) maxx = mx;
        if (my < miny) miny = my;
        if (my > maxy) maxy = my;
    }

    int rx = mmx + (int)(minx * s);
    int ry = mmy + (int)(miny * s);
    int rw = (int)((maxx-minx) * s);
    int rh = (int)((maxy-miny) * s);
    if (rw<2) rw=2;
    if (rh<2) rh=2;
    SDL_Rect cam;
    cam.x = rx;
    cam.y = ry;
    cam.w = rw;
    cam.h = rh;
    SDL_FillRect(pantalla, &cam, SDL_MapRGB(pantalla->format, 255, 255, 255));
}
