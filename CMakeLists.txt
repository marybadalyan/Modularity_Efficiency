cmake_minimum_required(VERSION 3.10)
project(Optimization_levels CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We only need ONE executable target. The configuration will change its properties.
add_executable(main_unoptimized main.cpp)

# Define the output directory for the analysis files
set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}/analysis")
file(MAKE_DIRECTORY ${ANALYSIS_DIR})

if(MSVC)
    add_custom_command(
        TARGET main_unoptimized POST_BUILD
        # This command runs after the build for the given configuration
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:main_unoptimized>\" > \"${ANALYSIS_DIR}/assembly_unoptimized.txt\""
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly_unoptimized.txt"
    )
else() # For GCC/Clang
     add_custom_command(
        TARGET main_unoptimized POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:main_unoptimized>" > "${ANALYSIS_DIR}/assembly_unoptimized.txt"
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly_unoptimized.txt"
    )
endif()

# We only need ONE executable target. The configuration will change its properties.
add_executable(main_optimized main.cpp)

if(MSVC)
    add_custom_command(
        TARGET main_optimized POST_BUILD
        # This command runs after the build for the given configuration
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:main_optimized>\" > \"${ANALYSIS_DIR}/assembly_optimized.txt\""
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly_optimized.txt"
    )
else() # For GCC/Clang
     add_custom_command(
        TARGET main_optimized POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:main_optimized>" > "${ANALYSIS_DIR}/assembly_optimized.txt"
        COMMENT "Dumping assembly for $<CONFIG> build to analysis/assembly_optimized.txt"
    )
endif()


target_compile_options(main_optimized PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:
        -O3
        -march=native
        -ffast-math
        -funroll-loops
        -fopt-info-vec
        -fopt-info-vec-missed
    >
    $<$<CXX_COMPILER_ID:Clang>:
        -O3
        -march=native
        -ffast-math
        -fvectorize
        -fslp-vectorize
        -Rpass=loop-vectorize
        -Rpass=slp-vectorize
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /O2
        /arch:AVX2
        /fp:fast
        /GL
    >
)
