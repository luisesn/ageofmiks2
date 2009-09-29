//Carga sprites
SDL_Surface *cs(const char *archivo) 
{
    SDL_Surface *tmp, *bmp;
    tmp=SDL_LoadBMP(archivo);
    Uint32 color_key=SDL_MapRGB(tmp->format, 255, 0, 255);
    if(!tmp) return 0;
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY|SDL_RLEACCEL, color_key);
    bmp=SDL_DisplayFormat(tmp);
    SDL_FreeSurface(tmp);
    if(!bmp) printf("Fallo al cargar: %c\r\n", archivo);
    return bmp;
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
	SDL_BlitSurface( s, NULL, dest, &rectDest );
	SDL_FreeSurface( s );
}
