/*************************************************************
Age of miks 2

main.cpp: archivo principal
*************************************************************/

#include "main.h"




int main (int argc, char *argv[])
{
    
    srand(time(0));
    double ang = 0;
    camara=1;
    seleccionn=2;
    atexit (SDL_Quit);
    inicializar();
    cargar_mapa();
    cargar_sprites();
    crear_unidades();
    crear_objetos();
    
    fin=0;
    seleccion.n=-1;

    jugadores_configurar();
    for (int j=0; j<jugadores-1; j++) ia[j].configurar(j+1);

       textprintf(pantalla, fuente,0,posy,clr_blanco,"Iniciando...");SDL_UpdateWindowSurface(ventana);
       posy+=13;
    //while(!fin) vteclas();
    //SDL_Delay(500);
    vteclas();
    fin=0;

    while(!fin)
    {

                tiempo=SDL_GetTicks();
                posx=(int) (5*cos(ang)+49);
                posy=(int) (5*sin(ang)+49);
                ang=ang+3.1415/90;
                
                switch (camara)
                {
                       case 1: //Camara libre
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
                            break;
                       case 0: //Camara sigue a cursor
                            scrollx =(curx - cury)*( 64 /2)-RESX/2;
                            scrolly = (curx + cury)*( 32 /2)-RESY/2;
                            break;
                       case 2: //Camara sigue a objeto
                            scrollx =(posx - posy)*( 64 /2)-RESX/2;
                            scrolly = (posx + posy)*( 32 /2)-RESY/2;
                            break;
                      case 3: //Camara sigue a objeto a seleccionar
                            //seleccion.n=0;
                            scrollx =(ud[seleccionn].x - ud[seleccionn].y)*( 64 /2)-RESX/2;
                            scrolly = (ud[seleccionn].x + ud[seleccionn].y)*( 32 /2)-RESX/2;
                            break;
                }
                dibujarmapa();
                dibujarobjetos();
                actualizar_uds();
                d_spr_cursor();


                vteclas();
                gui();
                for (int j=0; j<jugadores-1; j++) ia[j].main();      
                          
                actualizar();
                Uint32 dt = SDL_GetTicks() - tiempo;
                fps = (dt > 0) ? 1000 / dt : 999;
                while ((SDL_GetTicks()-tiempo) < (1000/25)) { }
    }
    quitar();
}
