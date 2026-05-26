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
| `W A S D` | Mover cursor isométrico |
| `C` | Cambiar modo de cámara (libre / cursor / objeto / unidad) |
| `R` | Poner aldeanos a recoger recursos |
| `E` | Poner todas las unidades a explorar |
| `T` | Poner soldados a explorar |
| `1` | Mostrar/ocultar memoria de la IA |
| `2` | Mostrar/ocultar debug de unidades |
| `Q` / `Escape` | Salir |

---

## Instalación del toolchain (Windows)

El proyecto usa **MSYS2** como entorno de compilación. Proporciona un compilador MinGW-w64
moderno y un gestor de paquetes (`pacman`) para instalar SDL2 y sus extensiones con un solo comando.

### 1. Instalar MSYS2

Descarga e instala desde **https://www.msys2.org**

Durante la instalación deja la ruta por defecto (`C:\msys64`).
Al terminar, abre la terminal **MSYS2 MINGW64** (no la MSYS2 genérica).

### 2. Actualizar el sistema base

```bash
pacman -Syu
```

Si la terminal se cierra sola, vuelve a abrirla y ejecuta:

```bash
pacman -Su
```

### 3. Instalar compilador, CMake y librerías SDL2

```bash
pacman -S \
  mingw-w64-x86_64-gcc \
  mingw-w64-x86_64-cmake \
  mingw-w64-x86_64-SDL2 \
  mingw-w64-x86_64-SDL2_ttf \
  mingw-w64-x86_64-SDL2_mixer
```

### 4. Añadir MSYS2 al PATH de Windows (opcional pero recomendado)

Añade `C:\msys64\mingw64\bin` a la variable de entorno `PATH` de Windows.
Esto permite usar `cmake` y `g++` desde el terminal de VSCode sin abrir MSYS2.

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
