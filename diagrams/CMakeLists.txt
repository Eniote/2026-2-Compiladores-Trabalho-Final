# Locate the Graphviz compiler locally
find_program(DOT_EXECUTABLE dot)

if(DOT_EXECUTABLE)
    # Scan the current directory for all files with the .dot extension
    file(GLOB DOT_FILES CONFIGURE_DEPENDS "*.dot")

    # Define the absolute path for the documentation image directory
    set(IMG_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/diagrams")

    # Instruct CMake to ensure the target directory exists prior to build execution
    file(MAKE_DIRECTORY "${IMG_OUTPUT_DIR}")

    # Initialize an empty list to track all expected output files
    set(ALL_PNG_FILES "")

    # Iterate over each discovered .dot file
    foreach(DOT_FILE ${DOT_FILES})
        get_filename_component(BASE_NAME ${DOT_FILE} NAME_WE)

        # Route the PNG output directly to the new image directory
        set(PNG_FILE "${IMG_OUTPUT_DIR}/${BASE_NAME}.png")

        # Define the command to generate this specific PNG
        add_custom_command(
                OUTPUT ${PNG_FILE}
                COMMAND ${DOT_EXECUTABLE} -Tpng -Gdpi=300 ${DOT_FILE} -o ${PNG_FILE}
                DEPENDS ${DOT_FILE}
                COMMENT "Compiling automata diagram: ${BASE_NAME}.png to diagrams"
                VERBATIM
        )

        list(APPEND ALL_PNG_FILES ${PNG_FILE})
    endforeach()

    # Create a single master target that triggers compilation for all tracked PNGs
    if(ALL_PNG_FILES)
        add_custom_target(generate_diagrams ALL DEPENDS ${ALL_PNG_FILES})
    endif()
else()
    message(WARNING "System alert: Graphviz 'dot' compiler not located. Diagram generation bypassed.")
endif()