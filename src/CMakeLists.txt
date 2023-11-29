find_package(PackageHandleStandardArgs REQUIRED)
find_path(gl2ps_INCLUDE_DIR gl2ps.h)
find_library(gl2ps_LIBRARY gl2ps)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_path(freeglut_INCLUDE_DIR GL/freeglut.h)
#find_library(freeglut_LIBRARY glut)

#include_directories(/opt/homebrew/opt/freeglut/include)
include_directories(/opt/homebrew/opt/gl2ps/include)

add_library(
    swzplot swzplot.cpp swzplot.hpp
    swzaxes.cpp swzglut.cpp swzline.cpp
    swzpatch.cpp swzsurface.cpp
)
target_include_directories(
    swzplot PRIVATE
    ${gl2ps_INCLUDE_DIR}
    #${freeglut_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIRS}
)
target_link_libraries(
    swzplot PRIVATE
    ${gl2ps_LIBRARY}
    #${freeglut_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)
# target_compile_definitions(swzplot PRIVATE GL_SILENCE_DEPRECATION)
add_compile_options(-fno-common)
add_executable(demo demo.cpp swzplot.hpp)
target_link_libraries(demo PUBLIC swzplot)
target_include_directories(demo PUBLIC ./)