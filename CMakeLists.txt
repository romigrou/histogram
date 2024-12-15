cmake_minimum_required(VERSION 3.15...3.31)
project(histogram CXX)

add_executable(histogram histogram.cpp)
target_compile_features(histogram PRIVATE cxx_std_17)
set_property(DIRECTORY "." PROPERTY VS_STARTUP_PROJECT histogram)

# Enable NASM if possible
include(CheckCXXSymbolExists)
check_cxx_symbol_exists("_M_AMD64"   "" _M_AMD64)
check_cxx_symbol_exists("__amd64__"  "" __amd64__)
if (_M_AMD64 OR __amd64__)
    include(CheckLanguage)
    check_language(ASM_NASM)
    if (CMAKE_ASM_NASM_COMPILER)
        enable_language(ASM_NASM)
    endif()
    if (MSVC OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(RMGR_HISTO_NASM_OPTIONS -Xvc)
    else()
        set(RMGR_HISTO_NASM_OPTIONS -Xgnu)
    endif()
    list(APPEND RMGR_HISTO_OPTIONS $<$<NOT:$<CONFIG:Release>>:-g>)

    set(RMGR_HISTO_NASM_SRC
        histogram_x86.nasm
    )
    #set_property(FILE ${RMGR_HISTO_NASM_SRC} PROPERT LANGUAGE ASM_NASM)
    target_sources(histogram PRIVATE ${RMGR_HISTO_NASM_SRC})
    target_compile_definitions(histogram PRIVATE USE_ASM)
endif()
