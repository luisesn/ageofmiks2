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
    if (jugador[id_p].quitar_recursos(UD_PRECIO_ALDEANO))
    {
        id=unidad_vacia();
        if (id!=-1)
        {
            ud[id].ptos_resistencia=RESISTENCIA_ALDEANO;
            ud[id].x=obj[jugador[id_p].objeto_centro].x+1;
            ud[id].y=obj[jugador[id_p].objeto_centro].y+1;
            ud[id].cx=5;
            ud[id].cy=5;
            ud[id].tipo=UD_TIPO_ALDEANO;
            ud[id].alcance_vision=ALCANCE_ALDEANO;
            ud[id].actitud=ACTITUD_PASIVA;
            ud[id].id_jugador=id_p;
            ud[id].ptos_ataque=ATAQUE_ALDEANO;
            ud[id].ptos_defensa=DEFENSA_ALDEANO;
            return id;
        }
    }
    return -1;
}

int crear_soldado(int id_p)
{
    int id=-1;
    if (jugador[id_p].quitar_recursos(UD_PRECIO_SOLDADO))
    {
        id=unidad_vacia();
        if (id!=-1)
        {
            ud[id].ptos_resistencia=RESISTENCIA_SOLDADO;
            ud[id].x=obj[jugador[id_p].objeto_centro].x+1;
            ud[id].y=obj[jugador[id_p].objeto_centro].y+1;
            ud[id].tipo=UD_TIPO_SOLDADO;
            ud[id].alcance_vision=ALCANCE_SOLDADO;
            ud[id].actitud=ACTITUD_AGRESIVA;
            ud[id].id_jugador=id_p;
            ud[id].ptos_ataque=ATAQUE_SOLDADO;
            ud[id].ptos_defensa=DEFENSA_SOLDADO;
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

void actualizar ()
{
    int u[]={0,0,0,0,0,0,0,0};
    int c=0;
    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa()) u[ud[j].id_jugador]++;
    }
    textprintf(pantalla, fuente,0,0,clr_blanco,"CAM:%d FPS:%d  Recursos:%d  Sel:%d  [1=mem 2=debug]",
        camara, fps, jugador[JUGADOR_LOCAL].numero_recursos(), seleccion.n);
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

}

void cambiar_camara()
{
     camara++;
     if (camara>3) {camara=0;}
 }

void actualizar_uds()
{
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].activa()) ud[temp].procesar_orden();
    }
}

int construir_objeto(int x, int y, int tipo, int id_jugador)
{
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
    obj[objn].propiedad_1=10000;
    return objn;
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

    for (int temp=0; temp<40; temp++)
      {
            construir_objeto(rand()%ANCHOX, rand()%ANCHOY, 1,-1);
      }
}



void vteclas()
{
SDL_Event event;
int temp;

while (SDL_PollEvent (&event))
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
        	break;
        case SDL_KEYUP:
        	break;
        /*	case SDL_JOYBUTTONDOWN:
        	break;
        case SDL_JOYBUTTONUP:
             //if (tecla==event.jbutton.button) { teclap--;  if (teclap<=0) tecla=0; teclap=0;}
        	break;*/
        case SDL_QUIT:
             fin=1;
        	break;
        default:
        	break;
        case SDL_MOUSEMOTION:
             mousex = event.motion.x;
             mousey = event.motion.y;
        	//Calculamos las coordenadas de pantalla a partir de las de la casilla
        	//y el desplazamiento de la camara (scrollX, scrollY)
        	// Ecs. originales de c�lculo de posici�n de sprites del mapa
            //px = (x - y)*(32) - scrollx;
            //py = (x + y)*(16) - scrolly;
            //Sacamos (x,y)
        	//px= 32*x - 32*y - scrollx;
        	//py = 16*x + 16*y - scrolly;
        	//px+2*py = 64*x  - scrolly - scrollx;
        	curx= (mousex+2*mousey+scrollx+2*scrolly-96)/64;
        	//px-2*py= -64*y -scrollx + scrolly;
            cury=(2*mousey-mousex-scrollx+2*scrolly-32)/64;
        	//Dibuja el tile en la pantalla
        	//d_spr_mapa(px, py, mapa[x][y]);
             break;
        case SDL_MOUSEBUTTONDOWN:
            //If the left mouse button was pressed
            if (event.button.button == SDL_BUTTON_LEFT )
            {
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
                                    ud[t].explorar_recursos();
                                }
                            }
                            break;
                        case 10: //Bot�n 10
                            fin=1;
                            break;
                    }
                }         
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
                                    ud[seleccion.n].ira(curx, cury);
                                    break;
                            }
                        } else {
                            construir_objeto(curx, cury,0 ,1);
                        }
                   }
            }
            break;
    }
}


const Uint8 *teclas = SDL_GetKeyboardState(NULL);
if (teclas[SDL_SCANCODE_ESCAPE]) fin=1;
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
      cury--;
   }
   if (teclas[SDL_SCANCODE_S])
   {
      cury++;
   }
   if (teclas[SDL_SCANCODE_A])
   {
      curx--;
   }
   if (teclas[SDL_SCANCODE_D])
   {
      curx++;
   }

   if (teclas[SDL_SCANCODE_Q])
   {
    fin=1;
   }
   if (teclas[SDL_SCANCODE_C])
   {
    cambiar_camara();
   }
   if (teclas[SDL_SCANCODE_E])
   {
        for (int temp=0; temp<uds; temp++)
        {
            ud[temp].ira(rand()%ANCHOX, rand()%ANCHOY);
        }
   }
   if (teclas[SDL_SCANCODE_R])
   {
        for (int t=0; t<uds; t++)
        {
            if (ud[t].tipo==1 && ud[t].activa())
            {
                ud[t].explorar_recursos();
            }
        }
   }
   if (teclas[SDL_SCANCODE_T])
   {
        for (int t=0; t<uds; t++)
        {
            if (ud[t].tipo==2 && ud[t].activa())
            {
                ud[t].explorar();
            }
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
			//Calculamos las coordenadas de pantalla a partir de las de la casilla
			//y el desplazamiento de la camara (scrollX, scrollY)
			px = (x - y)*(32) - scrollx;
			py = (x + y)*(16) - scrolly;

			//Dibuja el tile en la pantalla
			d_spr_mapa(px, py, mapa[x][y]);
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
