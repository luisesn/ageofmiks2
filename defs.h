/*************************************************************
Age of miks 2

defs.h: Definiciones y variables principales del programa
*************************************************************/


//Dimensiones del mapa
#define      ANCHOX         100
#define      ANCHOY         100


//Dimensiones de la ventana
#define              RESX         800
#define              RESY         600




//Paso de scroll
#define   PASO_SCROLL 20

FILE *f_mapa;
int mapa[ANCHOX][ANCHOY];
int obstaculos[ANCHOX][ANCHOY];
int ocupacion_uds[ANCHOX][ANCHOY];
unsigned char vis_actual[ANCHOX][ANCHOY];
unsigned char vis_explorado[ANCHOX][ANCHOY];


//Variables de gr�ficos
SDL_Window  *ventana  = NULL;
SDL_Surface *pantalla = NULL;
//SDL_Joystick *joy = NULL;
SDL_Surface *spr_mapa=NULL;
SDL_Surface *spr_gui=NULL;
SDL_Surface *spr_fog=NULL;
//Para el dibujo
SDL_Rect area1;
SDL_Rect area2;
int tempy, tempx;
TTF_Font *fuente;
SDL_Color clr_blanco = { 255, 255, 255, 0 };
SDL_Color clr_rojo = { 255,0, 0, 0 };
SDL_Color clr_azul = { 0,0, 255, 0 };
SDL_Color clr_verde = { 0,255, 0, 0 };

//Variables de sonidos
Mix_Music *musica=NULL;
Mix_Chunk *payum=NULL;
Mix_Chunk *explosion=NULL;
int volumen=64;

typedef struct def_seleccion
{
    int n; //id de objeto seleccionado
    int tipo; //tipo de objeto 0=unidad 1=objeto
};


//Variables de programa

int fin=1; // Si esta variable==0 se cierra el bucle y se sale del programa
//int curx=1, cury=1;
int posx=0, posy=0; // Variables del cursor de texto
int mousex=RESY/2, mousey=RESX/2; // Posici�n del rat�n dentro de la pantalla 
int curx=ANCHOX/2, cury=ANCHOY/2; //Posici�n del cursor en el mapa
int scrollx=0, scrolly=0; // Posicion del scroll
def_seleccion seleccion;
int seleccionn=0;
Uint32 tiempo; //Variable para el c�lculo de FPSs
Uint32 fps; //Variable para el c�lculo de FPSs
Uint32 tm_mouseover; //Variable para el scroll
int camara=1; // Tipo de c�mara
int ver_debug_uds=0; // Toggle con tecla 2
#define DEBUG 1
//Tabla trionom�trica
float se[361],co[361];
int menu=0;

// Configuracion de balance (cargable desde balance.cfg)
int cfg_precio_aldeano=50;
int cfg_precio_soldado=100;
int cfg_alcance_aldeano=3;
int cfg_alcance_soldado=6;
int cfg_resistencia_aldeano=100;
int cfg_resistencia_soldado=200;
int cfg_ataque_aldeano=10;
int cfg_ataque_soldado=50;
int cfg_defensa_aldeano=2;
int cfg_defensa_soldado=4;
int cfg_precio_centro=400;
int cfg_precio_almacen=250;
int cfg_ingreso_almacen=2;
int cfg_ia_objetivo_almacenes=3;
int cfg_ia_recursos_min_ataque=200;
int cfg_fog_habilitada=0;

// Tipo de edificio a construir al hacer click derecho sin seleccion
int modo_construccion_tipo=0;

typedef struct GameState
{
    int running;
    int camera_mode;
    Uint32 fps;
    Uint32 frame_ms;
    int build_mode;
    int selected_id;
    int selected_type;
    int mouse_x;
    int mouse_y;
    int cursor_x;
    int cursor_y;
} GameState;

GameState game_state = {1, 1, 0, 0, 0, -1, 0, RESX/2, RESY/2, ANCHOX/2, ANCHOY/2};

#define CMD_TIPO_NONE 0
#define CMD_TIPO_MOVER_UNIDAD 1
#define CMD_TIPO_CONSTRUIR 2
#define CMD_TIPO_EXPLORAR_TODOS 3
#define CMD_TIPO_EXPLORAR_SOLDADOS 4
#define CMD_TIPO_RECOGER_ALDEANOS 5
#define CMD_TIPO_CANCELAR_UNIDAD 6

typedef struct def_command
{
    int tipo;
    int unidad;
    int jugador;
    int x;
    int y;
    int extra;
    int prioridad;
    int secuencia;
} def_command;

#define MAX_COMMAND_QUEUE 1024
def_command command_queue[MAX_COMMAND_QUEUE];
int command_queue_size=0;
int command_queue_sequence=0;
    
//Variables del usuario
char usuario[254]="\0";
char password[254]="\0";

#define jugadores 4
#define JUGADOR_LOCAL 0


//int tecla=0, pulsada=0, teclap=0,temp=0, tempx=0, tempy=0; //Variables de control de la rutina de teclado.


