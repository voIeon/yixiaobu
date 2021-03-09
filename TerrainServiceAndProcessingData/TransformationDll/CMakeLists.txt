
# Set the project name
project (TransformationDll)


file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
source_group("Header Files" FILES ${CURRENT_HEADERS}) 

# Add a library with the above sources
add_library(${PROJECT_NAME} Transformation.cpp ${CURRENT_HEADERS})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR} 
)
