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


//Variables de gráficos
SDL_Surface *pantalla = NULL;
//SDL_Joystick *joy = NULL;
SDL_Surface *spr_mapa=NULL;
SDL_Surface *spr_gui=NULL;
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
int mousex=RESY/2, mousey=RESX/2; // Posición del ratón dentro de la pantalla 
int curx=ANCHOX/2, cury=ANCHOY/2; //Posición del cursor en el mapa
int scrollx=0, scrolly=0; // Posicion del scroll
def_seleccion seleccion;
int seleccionn=0;
Uint32 tiempo; //Variable para el cálculo de FPSs
Uint32 fps; //Variable para el cálculo de FPSs
Uint32 tm_mouseover; //Variable para el scroll
int camara=1; // Tipo de cámara
#define DEBUG 1
//Tabla trionométrica
float se[361],co[361];
int menu=0;
    
//Variables del usuario
char usuario[254]="luisyo2001@gmail.com\0";
char password[254]="luisyo2001\0";

#define jugadores 4
#define JUGADOR_LOCAL 0


//int tecla=0, pulsada=0, teclap=0,temp=0, tempx=0, tempy=0; //Variables de control de la rutina de teclado.


