#define UD_TIPO_ALDEANO 1
#define UD_TIPO_SOLDADO 2

#define OBJ_TIPO_CENTRO 0 

#define UD_PRECIO_ALDEANO 50
#define UD_PRECIO_SOLDADO 100

#define ORDEN_IR    1
#define ORDEN_OBTENER_RECURSOS 2
#define ORDEN_EXPLORAR 3
#define ORDEN_BUSCAR_RECURSOS 4
#define ORDEN_PATRULLAR 5
#define ORDEN_ATACAR 6

#define ALCANCE_ALDEANO 3
#define ALCANCE_SOLDADO 6

#define ACTITUD_PASIVA 0
#define ACTITUD_AGRESIVA 1

#define RESISTENCIA_ALDEANO 100
#define RESISTENCIA_SOLDADO 200

#define ATAQUE_ALDEANO 10
#define ATAQUE_SOLDADO 50

#define DEFENSA_ALDEANO 2
#define DEFENSA_SOLDADO 4


#define MEMORIA_AVISO 0
#define MEMORIA_OBJETO_RECURSOS 1
#define MEMORIA_UNIDAD_ENEMIGA 2

#define MAX_MEM 20

typedef struct def_mem
{
    int tipo; // 0 aviso, 1 objeto, 2 unidad
    int x;  //Coordenadas
    int y;
    int o;
    int t;
    int activa;
};

typedef class def_memoria
{
public:
    def_mem m[MAX_MEM];
    

void comprobar();
int buscar (int tipo)
{
    for (int j=0; j<MAX_MEM; j++)
    {
        if (m[j].tipo==tipo && m[j].activa!=0) 
        {
            return m[j].o;
        }
    }
    return -1;
}
void anadir(int x, int y, int o, int tipo)
{
    for (int j=0; j<MAX_MEM; j++)
    {
        if (m[j].activa!=0) 
        {
            if (m[j].o==o)
            if (m[j].tipo==tipo) { m[j].x=x; m[j].y=y; m[j].t=SDL_GetTicks(); return;}
        }
    }   
    for (int j=0; j<MAX_MEM; j++)
    {
        if (m[j].activa==0) 
        {
            m[j].activa=1;
            m[j].x=x;
            m[j].y=y;
            m[j].o=o;
            m[j].tipo=tipo;
            m[j].t=SDL_GetTicks();
            return;
        }
    }
}

def_memoria()
{
    for (int j=0; j<MAX_MEM; j++)
    {
        m[j].activa=0;
    }
}
};


typedef struct tipo_orden
{
    int tipo; //Tipo de orden
    int s; //Estado de la orden, 0 yendo 1 cargando 2 volviendo 3 descargando
    int o; // Objeto destino
    int b; // Objeto origen
    int realizada;
int dx(){return destx;}
int dy(){return desty;}
void set(int x,int y) {destx=x; desty=y;}
private:    
    int destx,desty; //Destino de la orden    
};

typedef class tipo_objeto
{
public:
    int x,y; //Coordenadas
    int cx;
    int cy; // ¿Coordenadas dentro de la casilla?
    int direccion; //Para el sprite
    int ap;
    int tipo; //Tipo de objeto
    int sprite; //Numero de sprite (para posibles animaciones)
    int actualizar; //Indica si la unidad ha de actualizarse o no.
    
    tipo_orden orden; //Para guardar la cola de ordenes
    int id_jugador; //A quien pertenece este objeto
    int construido;

    int ptos_resistencia; //Daño recibido por el objeto.
    
    int propiedad_1;
    int propiedad_2;
    int propiedad_3;

void procesar_orden()
{
}
void dibujar()
{
    //Posición en la pantalla
    area2.x=(x - y)*(32) - scrollx; //-32;
    area2.y=(x + y)*(16) - scrolly; //-16;
           
     if (area2.x<RESX && area2.x>(-65) && area2.y>(-65) && area2.y<RESY)
     {
         area1.x=sprite*64+64*2*direccion; //256;
         area1.y=6*64+64*tipo; //128;
         area1.w=64;
         area1.h=64;
         area2.w=64;
         area2.h=64;
         SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
    }
    textprintf(pantalla, fuente,area2.x,area2.y,clr_blanco,"%d",propiedad_1);
}
private:
};
#define uds 200
tipo_objeto obj[uds];

typedef class tipo_jugador
{
public:
    
    int objeto_centro;
    char nombre[254];
    int seleccion;
    int tipo;    
    def_memoria mem;

int numero_recursos()
{
    return obj[objeto_centro].propiedad_1;
}

int quitar_recursos(int cantidad)
{
    if (numero_recursos()>cantidad)
    {
        obj[objeto_centro].propiedad_1-=cantidad;
        return 1;
    }
    return -1;
}

};


tipo_jugador jugador[jugadores];

#define MAX_ORDENES 25
typedef class tipo_unidad
{
public:
    int x,y; //Coordenadas
    int cx;
    int cy; // ¿Coordenadas dentro de la casilla?
    int direccion; //Para el sprite
    int ap;
    int tipo; //Tipo de objeto
    int sprite; //Numero de sprite (para posibles animaciones)
    int actualizar; //Indica si la unidad ha de actualizarse o no.
    
    tipo_orden orden; //Para guardar la cola de ordenes
    int id_jugador; //A quien pertenece este objeto
    int carga;

    int alcance_vision;
    int actitud;
    
    int ptos_ataque;
    int ptos_defensa;
    int ptos_resistencia; //Daño recibido por el objeto.
    
tipo_unidad()
{
    orden.realizada=0;
}
int activa()
{
    if (ptos_resistencia>0) { return 1;} else {return 0;}
}

int hallegado()
{
    //if (orden.dx()==x && orden.dy()==y && cx==5 && cy==5) { return 1; } else { return 0;}
    if (orden.dx()==x && orden.dy()==y) { return 1; } else { return 0;}
}
void escaner();

void procesar_orden()
{
    int dx, dy;
    if (orden.realizada==1)
    {
        //Procesamos la orden
        switch (orden.tipo)
        {
            case 0:
                orden.realizada=0;
                break;
            case ORDEN_IR: //Orden de movimiento
                //Movemos
                mover();
                //Comprobamos si hemos llegado
                if (hallegado()==1)
                {
                    // destx=x y desty=y <- orden cumplida
                    orden.realizada=0;
                    sprite=0;
                }
                break;
            case ORDEN_OBTENER_RECURSOS: //Orden de ir a buscar recursos
                switch (orden.s)
                {
                    case 0: //Yendo a por recursos
                        //Asignamos las coordenadas
                        orden.set(obj[orden.o].x,obj[orden.o].y);
                        //Movemos la unidad
                        mover();
                        //Comprobamos si hemos llegado
                        if (hallegado()==1)
                        {
                            orden.s++; //Estado a carga
                        }
                        break;
                    case 1: //Cargando recurso
                        if (carga<100)
                        {
                            //Cargamos recursos
                            if (obj[orden.o].propiedad_1>1)
                            {
                                obj[orden.o].propiedad_1--;
                                carga++;
                            } else {
                                obj[orden.o].construido=0;
                                explorar_recursos();
                            }
                        } else {
                            orden.s++; //Volvemos a la base
                        }
                        break;
                    case 2:// Buscando base
                        //Asignamos las coordenadas
                        //Buscamos casa
                        /*
                        //Habría q buscar el centro de recursos mas cercano.
                        if (obj[temp].jugador==jugador && obj[temp].tipo==0)
                        {
                            orden.destx=obj[temp].x;
                            orden.desty=obj[temp].y;
                            orden.o=temp;
                            orden.s++;
                        }*/
                        orden.set(obj[jugador[id_jugador].objeto_centro].x,obj[jugador[id_jugador].objeto_centro].y);
                        orden.b=jugador[id_jugador].objeto_centro;
                        orden.s++;
                        break;
                    case 3: //Volviendo a base
                        //Movemos la unidad
                        mover();
                        //Comprobamos si hemos llegado
                        if (hallegado()==1)
                        {
                            orden.s++; //Estado a descarga
                        }
                        break;
                    case 4:
                        if (carga>0)
                        {
                            //Descargamos recursos
                            carga--;
                            obj[orden.b].propiedad_1++;
                        } else {
                            orden.s=0; //Reseteamos a yendo
                        }
                        break;
                    default:
                        orden.s=0;
                        break;
                }
                break;
            case ORDEN_EXPLORAR: // Orden de explorar
                switch (orden.s)
                {
                    case 0: //Sin coordenadas establecidas
                        dx=(int) (co[rand()%360]*5+x);
                        if (dx>ANCHOX || dx<0) dx=x;
                        dy=int (se[rand()%360]*5+y);
                        if (dy>ANCHOY || dy<0) dy=y;
                        orden.set(dx,dy);
                        orden.s=1;
                        break;
                    case 1: //Yendo
                        //Movemos la unidad
                        mover();
                        //Comprobamos si hemos llegado
                        if (hallegado()==1)
                        {
                            if (actitud==ACTITUD_PASIVA) {orden.s=0;} else {orden.s=2;}
                        }
                        break;
                    case 2: // Escaneamos
                        escaner();
                        break;
                }
                break;
             case ORDEN_BUSCAR_RECURSOS: //Orden de explorar por recursos
                switch (orden.s)
                {
                    case 0: //Sin coordenadas establecidas
                        //orden.destx=obj[jugador[id_jugador].objeto_centro].x;
                        //orden.desty=obj[jugador[id_jugador].objeto_centro].y;
                        dx=jugador[id_jugador].mem.buscar(MEMORIA_OBJETO_RECURSOS);
                        if (dx!=-1)
                        {
                            orden.realizada=1;
                            orden.tipo=ORDEN_OBTENER_RECURSOS;
                            orden.o=dx;
                            orden.s=0;
                        } else {
                            orden.b+=30;
                            if (orden.b>=360) {orden.b=0; orden.o+=1;}
                            dx=(int) (orden.o*co[orden.b]);
                            dx+=obj[jugador[id_jugador].objeto_centro].x;
                            dy=(int) (orden.o*se[orden.b]);
                            dy+=obj[jugador[id_jugador].objeto_centro].y;
                            if (dx>ANCHOX || dx<0) dx=obj[jugador[id_jugador].objeto_centro].x;
                            if (dy>ANCHOY || dy<0) dy=obj[jugador[id_jugador].objeto_centro].y;
                            orden.set(dx,dy);
                            orden.s++;
                        }
                        break;
                    case 1: //Yendo
                        //Movemos la unidad
                        //printf("%d,%d->%d,%d\r\n",obj[jugador[id_jugador].objeto_centro].x,(int) (orden.o*co[orden.b]),(int) (orden.o*se[orden.b]));
                        //printf ("[%d]M1:%d.%d,%d.%d -> %d,%d\r\n",id_jugador, x, cx, y ,cy, orden.dx(), orden.dy());
                        mover();
                        //printf ("[%d]M2:%d.%d,%d.%d -> %d,%d\r\n",id_jugador, x, cx, y ,cy, orden.dx(), orden.dy());
                        //Comprobamos si hemos llegado
                        if (hallegado()==1) {orden.s++; }
                        buscar_recursos();
                        break;
                    case 2:
                        orden.s=0;
                        break;
                }
                break;
            case ORDEN_PATRULLAR: //Orden de patrullar objetoo
                switch (orden.s)
                {
                    case 0: //Sin coordenadas establecidas
                        orden.b+=72;
                        if (orden.b>=360) orden.b=0;
                        dx=(int) (obj[orden.o].x+2*co[orden.b]);
                        dy=(int) (obj[orden.o].y+2*se[orden.b]);
                        orden.set (dx,dy);
                        orden.s++;
                        break;
                    case 1: //Yendo
                        //Movemos la unidad
                        mover();
                        //Comprobamos si hemos llegado
                        if (hallegado()==1)
                        {
                            if (actitud==ACTITUD_PASIVA) {orden.s=0;} else {orden.s=2;}
                        }
                        break;
                    case 2: // Escaneamos
                        escaner();
                        orden.s=0;
                        break;
                }
                break;
            case ORDEN_ATACAR: //Orden de atacar
                switch (orden.s)
                {
                    case 0: //Yendo
                        //Movemos la unidad
                        mover();
                        //Comprobamos si hemos llegado
                        if (hallegado()==1)
                        {
                            orden.s++;
                        }
                        break;
                    case 1: //Atacando
                        atacar_();
                        break;
                }
                break;   
        }
    }
}
void ira(int curx, int cury)
{
    orden.set(curx,cury);
    orden.tipo=ORDEN_IR;
    orden.realizada=1;
}

void atacar_();
void dibujar()
{
    //Posición en la pantalla
    area2.x=int ((x - y)*(32) - scrollx+(cx-cy)*32/10); //-32;
    area2.y=int ((x + y)*(16) - scrolly-16+(cx+cy)*16/10); //-16;
    
    
    if (area2.x<RESX && area2.x>(-65) && area2.y>(-65) && area2.y<RESY)
    {
        area1.x=sprite*64+64*2*direccion; //256;
        area1.y=64+64*tipo; //128;
        area1.w=64;
        area1.h=64;
        area2.w=64;
        area2.h=64;
        SDL_BlitSurface(spr_mapa, &area1, pantalla, &area2);
        textprintf(pantalla, fuente,area2.x-32,area2.y,clr_blanco,"(%d) %d.%d.%d.%d (%d.%d,%d.%d)-(%d,%d)",id_jugador,orden.tipo, orden.s,orden.b,orden.o, x,cx, y,  cy,orden.dx(), orden.dy());
    }
}
void explorar_recursos()
{
    orden.realizada=1;
    orden.tipo=ORDEN_BUSCAR_RECURSOS;
    orden.s=0;
    orden.b=0;
    orden.o=3;
}
void buscar_recursos()
{
    int dst, dstmin=9999;
    for (int j=0; j<uds; j++)
    {
        if (obj[j].tipo==1 && obj[j].construido==1 && obj[j].propiedad_1>0)
        {
            dst=int (abs(x-obj[j].x)+abs(y-obj[j].y)); //sqrt(pow((x-obj[j].x),2)+ pow((y-obj[j].y),2))
            if (dst<dstmin && dst<alcance_vision)
            {
                dstmin=dst;
                orden.realizada=1;
                orden.tipo=ORDEN_OBTENER_RECURSOS;
                orden.o=j;
                orden.s=0;
                jugador[id_jugador].mem.anadir(x,y, j, MEMORIA_OBJETO_RECURSOS);
            }
        }
    }
}

void atacar(int x, int y)
{
    orden.realizada=1;
    orden.set(x,y);
    orden.tipo=ORDEN_ATACAR;
    orden.s=0;
}

void explorar()
{
    orden.realizada=1;
    orden.tipo=ORDEN_EXPLORAR;
    orden.s=0;
}

void patrullar(int objeto)
{
    orden.realizada=1;
    orden.tipo=ORDEN_PATRULLAR;
    orden.s=0;
    orden.b=0;
    orden.o=objeto;
}
private:
void mover()
{
    int dx, dy;
    dy=orden.dy();
    dx=orden.dx();
    if (dy<y) // El objetivo esta por encima.
    {
        arriba();
    } else {
        if (dy>y) // El objetivo esta por debajo.
        {
            abajo();
        } else {
            if (5<cy)
            {
                arriba();
            } else {
                if (5>cy) abajo();
            }
        }
    }

    
    if (dx<x) // El objetivo esta a la izda.
    {
        izda();
    } else {
        if (dx>x) // El objetivo esta a la dcha.
        {
            dcha();
        } else {
            if (5<cx)
            {
                izda();
            } else {
                if (5>cx) dcha();
            }
        }
    }
    if (sprite==1) {sprite=0;} else {sprite=1;}
}
void arriba()
{
   if (cy>0)
   { 
        cy=cy+1;
   } else {
        if (y>0) 
        { 
          //if (x>0) x--;
          y--;
          cy=9;
        }
   }
    direccion=0;

}
      
void abajo()
{
   if (cy<10)
   { 
        cy=cy+1;
   } else {
        if (y<ANCHOY) 
        { 
          //if (x<ANCHOX) x++;
          y++;
          cy=0;
        }
   }
    direccion=1;
}

void izda()
{
   if (cx>0)
   { 
        cx--;
   } else {
        if (x>0) 
        { 
          //if (y<ANCHOY) y++;
          x--;
          cx=9;
        }
   }
   direccion=1;
}

void dcha()
{
    if (cx<10)
    { 
        cx++;
    }
    else 
    {
        if (x<ANCHOX) 
        { 
          //if (y>0) y--;
          x++;
          cx=0;
        }
    }
    direccion=0;
}
};


tipo_unidad ud[uds];

void tipo_unidad::atacar_()
{
    int dst;
    //Buscamos una unidad en la zona
    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa()==1)
        if (ud[j].id_jugador!=id_jugador)
        {
            dst=abs(ud[j].y-y)+abs(ud[j].x-x); // Uso distancia manhattan sqrt(pow((ud[j].x-x),2)+pow((ud[j].y-y),2))
            if (dst<alcance_vision)
            {
                jugador[id_jugador].mem.anadir(x,y, j, MEMORIA_UNIDAD_ENEMIGA);
                if (dst<2)
                {
                    //Esta forma de quitar puntos no es correcta si la defensa>ataque ...
                    ud[j].ptos_resistencia-=abs(ptos_ataque-ud[j].ptos_defensa);
                    ptos_resistencia-=abs(ud[j].ptos_ataque-ptos_defensa);
                    ud[j].sprite=2;
                    sprite=2;
                    Mix_PlayChannel(-1,payum,0);
                    break;
                }
            }
        }
    }
    //No hemos atacado a ninguna así q a explorar
    explorar();
}


void tipo_unidad::escaner()
{
    int dst;
    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa())
        if (ud[j].id_jugador!=id_jugador)
        {
            dst=abs(ud[j].y-y)+abs(ud[j].x-x); // Uso distancia manhattan sqrt(pow((ud[j].x-x),2)+pow((ud[j].y-y),2))
            d_escaner(ud[j].x, ud[j].y);
            if (dst<alcance_vision)
            {
                jugador[id_jugador].mem.anadir(ud[j].x,ud[j].y, j, MEMORIA_UNIDAD_ENEMIGA);
                if (dst<2)
                atacar(x ,y);
            }
        }
        if (obj[j].construido!=0)
        {
            //dst=abs(ud[j].y-y)+abs(ud[j].x-x); // Uso distancia manhattan sqrt(pow((ud[j].x-x),2)+pow((ud[j].y-y),2))
            if (abs(obj[j].y-y)+abs(obj[j].x-x)<alcance_vision)
            {
                jugador[id_jugador].mem.anadir(x,y, j, MEMORIA_OBJETO_RECURSOS);
            }
        }
    }
    orden.s=0;
}

void def_memoria::comprobar()
{
    for (int j=0; j<100; j++)
    {
        if (m[j].activa!=0) 
        {
            switch (m[j].tipo)
            {
                case MEMORIA_OBJETO_RECURSOS:
                    if (obj[m[j].o].construido!=1) m[j].activa=0;
                    break;
                case MEMORIA_UNIDAD_ENEMIGA:
                    if (SDL_GetTicks()-m[j].t>1000) m[j].activa=0;
                    if (ud[m[j].o].activa()!=1) m[j].activa=0;
                    break;
            }
        }
    }       
}
