#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namesapce::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
namespace glut{
// -*-
using GlutDict = std::map<int, Figure>;
using WindowQueue = std::list<Figure>;

// -*-
void idle();
void display();
void no_display();
void reshape(int width, int height);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passivemotion(int x, int y);
void keyboard(unsigned char key, int x, int y);

int toolWindowNum;
GlutDict windowDict;        // windows
WindowQueue windowQueue;    // 
std::mutex wq_mutex;

// -*-
void set_window_title(int window, std::string name){
    assert(window);
    glutSetWindow(window);
    glutSetWindowTitle(name.c_str());
}

// -*------------------------*-
}//-*- end::namespace::glut -*-
// -*------------------------*-

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-