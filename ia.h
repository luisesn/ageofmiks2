/*************************************************************
Age of miks 2

ia.h: clase de la ia del juego
*************************************************************/

int crear_aldeano(int id_p);
int crear_soldado(int id_p);
int construir_objeto(int x, int y, int tipo, int id_jugador);


typedef class def_ia
{
public:
    //
    int id_jugador;
    int j;
    int seleccion;
    
    //
    int unidades_minimas_recogiendo;
    int unidades_maximas_recogiendo;
    
    int unidades_minimas_defensa;
    int unidades_maximas_defensa;
    int unidades_patrullando_edificio;
    
    int unidades_minimas_ataque;
    int unidades_maximas_ataque;

    
    int recursos_minimos_defensas;
    int recursos_minimos_ataque;

    int probabilidad_atacar;
    int estrategia_modo;

#define IA_MODO_ECONOMIA 0
#define IA_MODO_DEFENSA  1
#define IA_MODO_ATAQUE   2

int contar_enemigos_memoria()
{
    int enemigos=0;
    for (int j=0; j<MAX_MEM; j++)
    {
        if (jugador[id_jugador].mem.m[j].activa!=0 && jugador[id_jugador].mem.m[j].tipo==MEMORIA_UNIDAD_ENEMIGA)
        {
            enemigos++;
        }
    }
    return enemigos;
}

void recalcular_estrategia(int recursos, int soldados)
{
    int enemigos = contar_enemigos_memoria();
    if (enemigos>0 && soldados>=unidades_minimas_ataque && recursos>recursos_minimos_ataque)
    {
        estrategia_modo = IA_MODO_ATAQUE;
        return;
    }
    if (soldados<unidades_minimas_defensa || recursos<recursos_minimos_ataque)
    {
        estrategia_modo = IA_MODO_DEFENSA;
        return;
    }
    estrategia_modo = IA_MODO_ECONOMIA;
}



void configurar(int id)
{
    id_jugador=id;

    recursos_minimos_defensas=0;
    recursos_minimos_ataque=cfg_ia_recursos_min_ataque;
    unidades_minimas_recogiendo=4+rand()%4;
    unidades_maximas_recogiendo=20+rand()%20;
    
    unidades_minimas_defensa=8+rand()%8;
    unidades_maximas_defensa=100+rand()%100;
    
    unidades_minimas_ataque=unidades_minimas_defensa+2;
    unidades_maximas_ataque=unidades_maximas_defensa+10;
    
    unidades_patrullando_edificio=3;
    probabilidad_atacar=rand()%100;
    estrategia_modo=IA_MODO_ECONOMIA;
}



int buscar_aldeano_libre() //Busca un aldeano libre
{
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa()==1 && ud[temp].tipo==UD_TIPO_ALDEANO && (ud[temp].orden.realizada==0 || ud[temp].orden.tipo==0)) return temp;
    }
    return -1;
}

int buscar_aldeano() //Busca un aldeano
{
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa() && ud[temp].tipo==UD_TIPO_ALDEANO) return temp;
    }
    return -1;
}

int numero_aldeanos()
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_ALDEANO) ald++;
        }
    }
    return ald;
}

int numero_soldados()
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO) ald++;
        }
    }
    return ald;
}

int buscar_soldados(int orden, int objeto)
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO && ud[temp].orden.tipo==orden && ud[temp].orden.realizada==1 && ud[temp].orden.o==objeto)
            ald++;
        }
    }
    return ald;
}

int buscar_soldados_patrullando()
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO && ud[temp].orden.tipo==ORDEN_PATRULLAR)
            if (ud[temp].orden.realizada==1)
            ald++;
        }
    }
    return ald;
}
int buscar_soldados_explorando()
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO && ud[temp].orden.tipo==ORDEN_EXPLORAR)
            if (ud[temp].orden.realizada==1)
            ald++;
        }
    }
    return ald;
}

int buscar_soldados_atacando()
{
    int ald=0;
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO && ud[temp].orden.tipo==ORDEN_ATACAR)
            if (ud[temp].orden.realizada==1)
            ald++;
        }
    }
    return ald;
}

int buscar_soldado_patrullando_objetodestruido()
{
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO)
            if (ud[temp].orden.realizada==1 && ud[temp].orden.tipo==ORDEN_PATRULLAR)
            if (obj[ud[temp].orden.o].construido!=1)
            return temp;
        }
    }
    return -1;
}

int buscar_soldado_libre ()
{
    int t;
    t=buscar_soldado_patrullando_objetodestruido();
    if (t>0) return t;
    /*
    //Buscar un soldado que este explorando y lejos de la base
    for (int temp=0; temp<uds; temp++)
    {
        if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
        {
            if (ud[temp].tipo==UD_TIPO_SOLDADO &&  ud[temp].orden.realizada==1)
            if (ud[temp].orden.tipo==ORDEN_EXPLORAR);
            return temp;
        }
    }*/
    
    return -1;
}

int numero_almacenes()
{
    int total=0;
    for (int j=0; j<uds; j++)
    {
        if (obj[j].construido && obj[j].id_jugador==id_jugador && obj[j].tipo==OBJ_TIPO_ALMACEN)
        {
            total++;
        }
    }
    return total;
}

void intentar_expandir_economia(int recursos)
{
    if (recursos < (cfg_precio_almacen + cfg_precio_aldeano)) return;

    int almacenes = numero_almacenes();
    if (almacenes >= cfg_ia_objetivo_almacenes) return;

    int cx = obj[jugador[id_jugador].objeto_centro].x;
    int cy = obj[jugador[id_jugador].objeto_centro].y;

    for (int intento=0; intento<10; intento++)
    {
        int nx = cx + (rand()%9) - 4;
        int ny = cy + (rand()%9) - 4;
        if (construir_objeto(nx, ny, OBJ_TIPO_ALMACEN, id_jugador) != -1)
        {
            break;
        }
    }
}


void main()
{
    int aldeanos=0, soldados=0, recursos=0,seleccion=0, t=0;
    jugador[id_jugador].mem.comprobar();

    //Recuento de aldeanos
    aldeanos=numero_aldeanos();
    soldados=numero_soldados();
    
    //Recursos
    recursos=jugador[id_jugador].numero_recursos();
    recalcular_estrategia(recursos, soldados);
    intentar_expandir_economia(recursos);

    //
    if (aldeanos>0 || recursos>cfg_precio_aldeano)
    {
        //Conseguir recursos "minimo"
        if (aldeanos<unidades_minimas_recogiendo && recursos>cfg_precio_aldeano)
        {
            //Hay que mejorar la busqueda, no todos los aldeanos tienen que estar recogiendo.
            //Crear un aldeano.
            //printf("Crear aldeano.\r\n");
            crear_aldeano(id_jugador);
        }
        
        //Buscamos un aldeano libre
        jugador[id_jugador].seleccion=buscar_aldeano_libre();
        //
        if (jugador[id_jugador].seleccion!=-1)
        {
            //Hay un aldeano libre, pong�moslo a buscar recursos.
            //printf("Aldeano libre.\r\n");
            ud[jugador[id_jugador].seleccion].explorar_recursos();
            printf ("[%d] Encontrado aldeano libre %d, asignado.\r\n", id_jugador,jugador[id_jugador].seleccion);
        } else {
            //No hay ning�n aldeano libre
            //printf("No hay ning�n aldeano libre.\r\n");
        }

        //Estrategia de defensa
        if (recursos>recursos_minimos_defensas && estrategia_modo!=IA_MODO_ATAQUE)
        {
            if ((buscar_soldados_patrullando()+buscar_soldados_atacando())<=unidades_maximas_defensa)
            {
                for (int j=0; j<uds; j++)
                {
                    if (obj[j].id_jugador==id_jugador)
                    {
                        if (obj[j].construido==1)
                        {
                            if (buscar_soldados(ORDEN_PATRULLAR, j)<unidades_patrullando_edificio)
                            {   
                                t=crear_soldado(id_jugador);
                                if (t>0) 
                                {
                                    ud[t].patrullar(j); //jugador[id_jugador].objeto_centro
                                }
                                break;
                            }
                        }
                    }
                }
                //Patrullamos minas encontradas
                for (int j=0; j<MAX_MEM; j++)
                {
                    if (jugador[id_jugador].mem.m[j].activa!=0 && jugador[id_jugador].mem.m[j].tipo==MEMORIA_OBJETO_RECURSOS)
                    {
                        if (buscar_soldados(ORDEN_PATRULLAR, jugador[id_jugador].mem.m[j].o)<unidades_patrullando_edificio)
                        {   
                            t=buscar_soldado_libre();
                            if (t==-1) t=crear_soldado(id_jugador);
                            if (t!=-1) 
                            {
                                ud[t].patrullar(jugador[id_jugador].mem.m[j].o); //jugador[id_jugador].objeto_centro
                            }
                            //break;
                        }
                    }
                }
            }
        }

        //Estrategia de ataque
        if (recursos>recursos_minimos_ataque && estrategia_modo!=IA_MODO_DEFENSA)
        {
            //Construcci�n de uds. de ataque
            if (soldados<unidades_minimas_ataque && recursos>cfg_precio_soldado)
            {
                    t=crear_soldado(id_jugador);
                    if (t!=-1) 
                    {
                        ud[t].explorar();
                    }
            }

            int tpatrullando,enemigos=0,tasig=0;
            tpatrullando=buscar_soldados_explorando();
            enemigos = contar_enemigos_memoria();
            if (enemigos>0)
            {
                tpatrullando=tpatrullando/enemigos;
                if (tpatrullando<1) tpatrullando=1;
                
                for (int j=0; j<MAX_MEM; j++)
                {
                    if (jugador[id_jugador].mem.m[j].activa!=0 && jugador[id_jugador].mem.m[j].tipo==MEMORIA_UNIDAD_ENEMIGA)
                    {
                        tasig=0;
                        for (int temp=0; temp<uds; temp++)
                        {
                            if (tasig>=tpatrullando) break;
                            if (ud[temp].id_jugador==id_jugador && ud[temp].activa())
                            {
                                if (ud[temp].tipo==UD_TIPO_SOLDADO && ud[temp].orden.tipo==ORDEN_EXPLORAR && ud[temp].orden.realizada==1)
                                {
                                    ud[temp].atacar(jugador[id_jugador].mem.m[j].x, jugador[id_jugador].mem.m[j].y);
                                    tasig++;
                                    jugador[id_jugador].mem.m[j].activa=0;
                                }
                                //if (abs(ud[temp].x-jugador[id_jugador].mem.m[j].x) + abs(ud[temp].y-jugador[id_jugador].mem.m[j].y)<ANCHOX/2)
                            }
                        }
                    }
                }
            }
            
        }
     
        //Estrategia de recursos sobrantes
        if (recursos>(recursos_minimos_defensas+recursos_minimos_ataque))
        {
            if (aldeanos<unidades_maximas_recogiendo && recursos>cfg_precio_aldeano)
            {
                //Hay que mejorar la busqueda, no todos los aldeanos tienen que estar recogiendo.
                //Crear un aldeano.
                //printf("Crear aldeano.\r\n");
                crear_aldeano(id_jugador);
            }
            /*
            if (soldados<(unidades_maximas_defensa+unidades_maximas_ataque) && recursos>UD_PRECIO_SOLDADO)
            {
                //Hay que mejorar la busqueda, no todos los aldeanos tienen que estar recogiendo.
                //Crear un aldeano.
                //printf("Crear aldeano.\r\n");
                t=crear_soldado(id_jugador);
                if (t!=-1) 
                {
                    ud[t].explorar();
                }
            }
            */
            //unidades_patrullando_edificio++;
            //unidades_maximas_defensa+=5;
        }

    } else {
        //Si no hay aldeanos... �ltima estrategia...
    }
}


};

/*

void tipo_unidad::escaner()
{
    int dst=0, dstmin=9999, umin=-1;
    for (int j=0; j<uds; j++)
    {
        if (ud[j].activa())
        if (ud[j].id_jugador!=id_jugador)
        {
            dst=sqrt(pow((ud[j].x-x),2)+pow((ud[j].y-y),2));
            if (dst<dstmin)
            {
                dstmin=dst;
                umin=j;
            }
        }
    }
    
    if (dstmin<alcance_vision)
    {
        if (dstmin>2)
        {
            orden.destx=ud[umin].x;
            orden.desty=ud[umin].y;
            orden.tipo=ORDEN_EXPLORAR;
            orden.s=1;
            return;
        } else {
            atacar(umin);
            return;
        }
    }
    orden.s=0;
}
*/

def_ia ia[10];



