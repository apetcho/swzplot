#include "swzplot.hpp"

// -*----------------------------------------------------------------*-
// -*- namesapce::swzplot                                           -*-
// -*----------------------------------------------------------------*-
namespace swzplot{
// -*-
namespace glut{
// -*-
using WindowDict = std::map<int, Figure>;
using WindowQueue = std::list<Figure>;

// -*-
void idlefn();
void displayfn();
void no_displayfn();
void reshapefn(int width, int height);
void mousefn(int button, int state, int x, int y);
void motionfn(int x, int y);
void passivemotionfn(int x, int y);
void keyboardfn(unsigned char key, int x, int y);

int toolWindowNum;
WindowDict windowDict;      // windows
WindowQueue windowQueue;    // 
std::mutex wq_mutex;

// -*-
void set_window_title(int window, std::string name){
    assert(window);
    glutSetWindow(window);
    glutSetWindowTitle(name.c_str());
}

// -*-
void create_window(const Figure fig){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(fig->m_windowBBox.left, fig->m_windowBBox.bottom);
    glutInitWindowSize(fig->m_windowBBox.width, fig->m_windowBBox.height);
    fig->m_window_num = glutCreateWindow(fig->m_window_name.c_str());
    glutIdleFunc(idlefn);
    glutDisplayFunc(displayfn);
    glutReshapeFunc(reshapefn);
    glutMotionFunc(motionfn);
    glutMouseFunc(mousefn);
    glutPassiveMotionFunc(passivemotionfn);
    glutKeyboardFunc(keyboardfn);
    glutShowWindow();
    windowDict.insert(WindowDict::value_type(fig->m_window_num, fig));
}

// -*------------------------*-
}//-*- end::namespace::glut -*-
// -*------------------------*-

// -*----------------------------------------------------------------*-
}//-*- end::namespace::swzplot                                      -*-
// -*----------------------------------------------------------------*-