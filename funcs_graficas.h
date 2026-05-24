/*************************************************************
Age of miks 2

funcs_graficas.h: funciones gr�ficas del juego
*************************************************************/
//Carga sprites
SDL_Surface *cs(const char *archivo)
{
    SDL_Surface *tmp = SDL_LoadBMP(archivo);
    if (!tmp) { printf("Fallo al cargar: %s\r\n", archivo); return NULL; }
    Uint32 color_key = SDL_MapRGB(tmp->format, 255, 0, 255);
    SDL_SetColorKey(tmp, SDL_TRUE, color_key);
    return tmp;
}

//Funcion para dibujar texto
void textprintf(SDL_Surface *dest, TTF_Font *fuente, Sint16 x, Sint16 y, SDL_Color color, const char *formato, ...)
{
	SDL_Surface *s;
	SDL_Rect rectDest = { x, y, 0, 0 };
	char bufer[512];
	va_list ap;

	// Revisa que todo esta correcto.
	if ( !formato || !dest || !fuente ) return;
	va_start( ap , formato );
	vsprintf( bufer , formato , ap );
	va_end( ap );
	s = TTF_RenderText_Solid( fuente, bufer, color );
	if (!s) return;
	SDL_BlitSurface( s, NULL, dest, &rectDest );
	SDL_FreeSurface( s );
}
