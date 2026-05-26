# Script para copiar todos los DLLs necesarios a la carpeta de build
# Este script se ejecuta como POST_BUILD desde CMakeLists.txt

# Obtener la ruta del ejecutable
get_filename_component(EXE_DIR "${CMAKE_BINARY_DIR}" ABSOLUTE)

# Ruta base de MSYS2
set(MSYS2_BIN "C:/msys64/mingw64/bin")

# Función auxiliar para copiar un DLL (intenta múltiples nombres)
function(copy_dll_variants base_names)
    foreach(dll_name ${base_names})
        if(EXISTS "${MSYS2_BIN}/${dll_name}")
            file(COPY "${MSYS2_BIN}/${dll_name}" DESTINATION "${EXE_DIR}")
            message(STATUS "Copied: ${dll_name}")
            return()
        endif()
    endforeach()
    message(STATUS "Warning: DLL not found for: ${base_names}")
endfunction()

# Copiar SDL2 y variantes
copy_dll_variants("libSDL2.dll;SDL2.dll")
copy_dll_variants("libSDL2_ttf.dll;SDL2_ttf.dll")
copy_dll_variants("libSDL2_mixer.dll;SDL2_mixer.dll")

# Copiar dependencias
set(DEPENDENCY_DLLS
    zlib1.dll
    libfreetype-6.dll
    libpng16-16.dll
    libharfbuzz.dll
    libharfbuzz-0.dll
    libglib-2.0-0.dll
    libbrotlidec.dll
    libbrotlicommon.dll
    libgraphite2.dll
    libintl-8.dll
    libbz2-1.dll
    libpcre2-8-0.dll
    libFLAC.dll
    libmpg123-0.dll
    libogg.dll
    libogg-0.dll
    libopus.dll
    libopus-0.dll
    libopusfile.dll
    libopusfile-0.dll
    libvorbis.dll
    libvorbis-0.dll
    libvorbisfile.dll
    libvorbisfile-3.dll
    libwavpack.dll
    libwavpack-1.dll
    libiconv.dll
    libiconv-2.dll
    libxmp.dll
    libstdc++-6.dll
    libgcc_s_seh-1.dll
    libwinpthread-1.dll
)

foreach(dll_name ${DEPENDENCY_DLLS})
    if(EXISTS "${MSYS2_BIN}/${dll_name}")
        file(COPY "${MSYS2_BIN}/${dll_name}" DESTINATION "${EXE_DIR}")
        message(STATUS "Copied: ${dll_name}")
    endif()
endforeach()


