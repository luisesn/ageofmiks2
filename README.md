# Age of Miks 2

Juego de estrategia en tiempo real (RTS) con vista isométrica, desarrollado en C++ con SDL2.
Proyecto retomado y migrado a un toolchain moderno (SDL2 + CMake + MSYS2).

## Estado actual

- Mapa isométrico 100×100 con scroll y 4 modos de cámara
- Unidades con órdenes: moverse, recoger recursos, explorar, patrullar, atacar
- IA para 3 jugadores rivales con estrategias de recursos, defensa y ataque
- Sistema de memoria de la IA (recuerda posición de enemigos y recursos)
- Sonido (efecto de combate)
- Demo jugable: compila y corre

## Controles

| Tecla / Ratón | Acción |
|---|---|
| Flechas | Desplazar cámara |
| Click izquierdo | Seleccionar unidad u objeto |
| Click derecho | Ordenar movimiento (unidad seleccionada) / construir |
| Click izquierdo en barra (botones 2/3) | Cambiar modo de construccion (centro/almacen) |
| `W A S D` | Mover cursor isométrico |
| `C` | Cambiar modo de cámara (libre / cursor / objeto / unidad) |
| `R` | Poner aldeanos a recoger recursos |
| `E` | Poner todas las unidades a explorar |
| `T` | Poner soldados a explorar |
| `B` | Seleccionar modo de construccion: centro |
| `N` | Seleccionar modo de construccion: almacen |
| `1` | Mostrar/ocultar memoria de la IA |
| `2` | Mostrar/ocultar debug de unidades |
| `Q` / `Escape` | Salir |

---

## Instalación del toolchain (Windows)

El proyecto usa **MSYS2**, que es una aplicación de Windows que instala un entorno de
compilación Unix-like dentro de Windows. Al instalarlo obtienes:

- Un compilador C++ moderno (MinGW-w64 / GCC)
- `pacman`, un gestor de paquetes que corre **dentro de la terminal de MSYS2** (no en CMD ni PowerShell)
- Las librerías SDL2 listas para usar

> Todos los comandos de `pacman` y `cmake` del siguiente apartado se ejecutan dentro
> de la terminal **MSYS2 MINGW64**, no en CMD ni en PowerShell de Windows.

### 1. Instalar MSYS2

Descarga el instalador `.exe` desde **https://www.msys2.org** y ejecútalo como cualquier
programa de Windows. Deja la ruta por defecto (`C:\msys64`).

Al terminar, abre la aplicación **MSYS2 MINGW64** desde el menú Inicio
(es importante usar **MINGW64**, no la genérica "MSYS2").

### 2. Actualizar el sistema base

Dentro de la terminal MSYS2 MINGW64:

```bash
pacman -Syu
```

Si la terminal se cierra sola al terminar, vuelve a abrirla y ejecuta:

```bash
pacman -Su
```

### 3. Instalar compilador, CMake y librerías SDL2

Dentro de la misma terminal MSYS2 MINGW64:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer
```

Confirma con `Y` cuando lo pida. Esto instala el compilador y las tres librerías SDL2 necesarias.

### 4. Añadir MSYS2 al PATH de Windows (recomendado para VSCode)

Para poder usar `cmake` y `g++` desde el terminal integrado de VSCode (sin necesidad
de abrir MSYS2 cada vez):

1. Abre **Configuración avanzada del sistema** → **Variables de entorno**
2. En "Variables del sistema", edita `Path` y añade: `C:\msys64\mingw64\bin`
3. Reinicia VSCode

---

## Compilar el proyecto

Desde la terminal **MSYS2 MINGW64** (o cualquier terminal con el PATH configurado):

```bash
# Clonar el repositorio
git clone https://github.com/luisesn/ageofmiks2.git
cd ageofmiks2

# Configurar y compilar
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

El ejecutable se genera en `build/AgeOfMiks2.exe`.

### Ejecutar

**Importante:** hay que lanzar el juego desde la raíz del proyecto, no desde `build/`,
para que encuentre los assets (`graficos/`, `mapa.txt`, `tahoma.ttf`, `phaser.wav`):

```bash
# Desde la raíz del proyecto
./build/AgeOfMiks2.exe
```

O en PowerShell/CMD de Windows:

```bat
build\AgeOfMiks2.exe
```

---

## Configurar VSCode

### Extensiones necesarias

Instala estas dos extensiones desde el Marketplace de VSCode:

| Extensión | ID |
|---|---|
| C/C++ | `ms-vscode.cpptools` |
| CMake Tools | `ms-vscode.cmake-tools` |

### Configuración

1. Abre la carpeta del proyecto en VSCode (`File → Open Folder`)
2. CMake Tools detecta `CMakeLists.txt` automáticamente
3. Selecciona el kit **GCC (MinGW64)** cuando lo pida
4. Usa el botón **Build** de la barra inferior o `Ctrl+Shift+P → CMake: Build`

Para depurar, crea un `.vscode/launch.json` apuntando a `build/AgeOfMiks2.exe`
y establece `cwd` a `${workspaceFolder}` para que los assets se encuentren.

---

## Changelog

### 2026-05-26

- Build de CMake corregido en Windows para resolver el error de entrada `WinMain` con SDL2.
- Enlace actualizado para incluir `SDL2main` junto con `SDL2`, `SDL2_ttf`, `SDL2_mixer` y `ZLIB`.
- Añadido post-build en CMake para copiar automáticamente los DLL de SDL2 al directorio del ejecutable.
- Añadido script `copy_dlls.cmake` para copiar dependencias transitivas de MSYS2 (audio, fuentes, compresion y runtime de MinGW).
- Corregida la copia de variantes de nombre de DLL (por ejemplo `SDL2.dll`/`libSDL2.dll`, `libopus-0.dll`, `libopusfile-0.dll`, `libiconv-2.dll`, `libvorbis-0.dll`, `libwavpack-1.dll`, `libharfbuzz-0.dll`).
- Verificado que el ejecutable `build/AgeOfMiks2.exe` se genera junto con los DLL requeridos en `build/`.
- Loop principal refactorizado para eliminar espera activa (`busy wait`) y usar `SDL_Delay` para limitar FPS.
- Separacion inicial entre simulacion y renderizado en `main.cpp` (`actualizar_simulacion` y `renderizar_frame`).
- Credenciales hardcodeadas eliminadas de `defs.h` (usuario/password quedan vacios por defecto).
- Inicializacion explicita de `obstaculos[][]` durante la carga de mapa.
- Movimiento de unidades con verificacion de casillas transitables frente a obstaculos y objetos construidos.
- Balance basico configurable en runtime mediante `balance.cfg` (costes y stats principales de aldeanos/soldados).
- Integracion de pathfinding A* basico (4 direcciones) para calcular el siguiente paso de movimiento.
- Integrada ocupacion dinamica de unidades por frame para evitar pisado de casillas en movimiento.
- Construccion basica de centros jugable con click derecho (sin seleccion), validacion de casilla y coste configurable (`precio_centro`).
- Fallback de desbloqueo local cuando A* queda temporalmente bloqueado por ocupacion dinamica.
- Nuevo edificio jugable `almacen` con ingreso periodico de recursos y coste configurable (`precio_almacen`, `ingreso_almacen`).
- Loop principal con timestep fijo por acumulador y limite de catch-up para evitar picos/espiral de actualizaciones.
- Migracion inicial de estado a `GameState` (sin eliminar aun todos los globales).
- Sistema de comandos desacoplado por cola (`command_queue`) para entradas de movimiento/construccion/ordenes globales.
- IA reorganizada con modos estrategicos basicos (economia/defensa/ataque) y helpers de decision.
- Niebla de guerra visual basica: solo se renderizan tiles explorados del jugador local.
- Niebla de guerra extendida a entidades: unidades enemigas solo visibles en vision actual y objetos por tiles explorados.
- IA con decision de expansion economica: construccion automatica limitada de `almacen` cuando hay recursos suficientes.
- Cola de comandos con prioridad y deduplicacion (movimiento por unidad y ordenes macro) para reducir spam de input.
- IA parametrizable desde `balance.cfg` para expansion/economia (`ia_objetivo_almacenes`, `ia_recursos_min_ataque`).

### Progreso de implementacion (items 1-10)

- [x] 1. Frame pacing sin busy wait
- [~] 2. Pathfinding A* (implementacion base activa, faltan optimizaciones/caching y resolucion de bloqueos dinamicos)
- [x] 3. Separar simulacion y render (incluye timestep fijo con acumulador)
- [~] 4. Migrar globales a `GameState` (fase inicial, convive con globales legacy)
- [~] 5. Sistema de comandos desacoplado (cola con prioridades y deduplicacion; faltan colas por unidad y cancelaciones explicitas)
- [~] 6. Reestructura de IA (introducidos modos estrategicos y helpers, falta BT/utility completo)
- [~] 7. Niebla de guerra visual (tiles + entidades, faltan suavizado, niebla parcial y rendimiento)
- [~] 8. Sistema de obstaculos/ocupacion (integrado en movimiento con ocupacion dinamica y fallback local; faltan politicas avanzadas anti-atasco)
- [~] 9. Balance data-driven (incluye unidades, edificios y parametros IA base; falta cubrir mas sistemas)
- [~] 10. Construccion de edificios jugable (centro + almacen con coste/validacion y efecto economico, faltan GUI/colas/tipos)

---

## Estructura del proyecto

```
ageofmiks2/
├── main.cpp            # Punto de entrada y bucle principal
├── main.h              # Funciones del juego (mapa, input, cámara, HUD)
├── defs.h              # Variables globales y constantes
├── clases.h            # Clases: tipo_unidad, tipo_objeto, tipo_jugador, def_memoria
├── ia.h                # Clase def_ia con la inteligencia artificial
├── funcs_graficas.h    # Carga de sprites y renderizado de texto
├── funcs_programa.h    # Inicialización SDL, carga de assets, GUI
├── CMakeLists.txt      # Build system (reemplaza el Makefile.win de Dev-C++)
├── balance.cfg         # Balance configurable en runtime (costes y stats base)
├── mapa.txt            # Mapa del juego (100×100 dígitos, 0=hierba, 1-5=otros)
├── tahoma.ttf          # Fuente para el texto en pantalla
├── phaser.wav          # Efecto de sonido de combate
└── graficos/
    ├── spr_mapa.bmp    # Sprite sheet: tiles, unidades, objetos, cursores
    └── spr_gui.bmp     # Sprite sheet de la barra de interfaz
```

---

## Pendiente

- Pathfinding (actualmente las unidades van en línea recta sin evitar obstáculos)
- Sistema de obstáculos (array `obstaculos[]` declarado pero sin uso)
- Botones de la GUI funcionales
- Niebla de guerra visual (la lógica existe, falta el renderizado)
- Construcción de edificios
- Menú principal
