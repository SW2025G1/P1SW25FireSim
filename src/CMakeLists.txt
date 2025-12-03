set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})  # Place programs in root folder when compiled.

# 1. Definer 'functions-c' biblioteket (indeholder kun functions.c)
add_library(functions-c functions.c)
target_include_directories(functions-c PUBLIC .)
target_link_libraries(functions-c m)  # Linker matematikbiblioteket

# 2. Definer 'simulation-c' biblioteket
add_library(simulation-c simulation.c)
# Sørg for at den kan finde functions.h (i '.') og visualizer.h (i '../visualizer')
target_include_directories(simulation-c PUBLIC . ../visualizer)
target_link_libraries(simulation-c PUBLIC m functions-c)

# 3. Definer 'visualizer' biblioteket
# Vi skal definere visualizer-målet i sin egen mappe (hvis det er der den er),
# men da det ikke er specificeret, definerer vi det her, og bruger den korrekte sti til kildekoden.
add_library(visualizer ../visualizer/visualizer.c)
# Visualizer skal kunne inkludere sine egne headers (i '../visualizer') og headers fra src (i '.')
target_include_directories(visualizer PUBLIC ../visualizer .)


# 4. Definer det eksekverbare program
add_executable(main main.c)

# 5. Link de nødvendige biblioteker til main.
# Main har brug for simulation-c, som igen bruger functions-c og visualizer.
target_link_libraries(main
        PUBLIC
        simulation-c
        functions-c
        visualizer
        m
)