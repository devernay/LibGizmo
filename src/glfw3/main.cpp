/* port of win32example in GLFW 3.2 to test */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "IGizmo.h"
// ArcBall from http://nehe.gamedev.net/tutorial/arcball_rotation/19003/
#include "ArcBall.h"
#include "glut_teapot.h"

namespace {

int g_x = 0, g_y = 0, g_width = 640, g_height = 480;

float g_objectMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

IGizmo *g_moveGizmo = 0, *g_rotateGizmo = 0, *g_scaleGizmo = 0, *g_currentGizmo = 0;

static void drawScene()
{
    //printf("draw\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
    GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    /* light_position is NOT default value */
    GLfloat light_position[] = {1.0, 0.0, 0.0, 0.0};
    GLfloat global_ambient[] = {0.75, 0.75, 0.75, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)g_width / (GLfloat)g_height, 0.1f, 10000.0f);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(5., 5., 5., 0., 0., 0., 0., 1., 0.);


    float viewMat[16];
    float projMat[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, viewMat );
    glGetFloatv (GL_PROJECTION_MATRIX, projMat );

    glPushMatrix();
    glMultMatrixf( g_objectMatrix );
    // Now draw a teapot

    //glFrontFace(GL_CW); // the GLUT teapot is CW (default is CCW)
    //glCullFace(GL_BACK); // GL_BACK is default value
    //glEnable(GL_CULL_FACE); // disabled by default. the GLUT teapot does not work well with backface culling
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    /*  material has small ambient reflection */
    GLfloat low_ambient[] = {0.1, 0.1, 0.1, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, low_ambient);
    glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
    //glEnable(srcTarget); // it deserves testure
    glutSolidTeapot(1.);
    //glDisable(srcTarget);
    glPopMatrix();

    if (g_currentGizmo) {
        g_currentGizmo->SetCameraMatrix(viewMat, projMat);
        g_currentGizmo->Draw();
    }
}

static void reshape(GLFWwindow *w, int width, int height)
{
    printf("reshape %d %d\n", width, height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    g_width = width; g_height = height;

    if (g_currentGizmo) {
        g_currentGizmo->SetScreenDimension(width, height);
    }
}

static void mouseButton(GLFWwindow *w, int button, int action, int mods)
{
    switch (action) {
    case GLFW_PRESS:
        if (g_currentGizmo) {
            if (g_currentGizmo->OnMouseDown(g_x, g_y)) {
                printf("OnMouseDown: capture!\n");
            }
        }
        break;
    case GLFW_RELEASE:
        if (g_currentGizmo) {
            g_currentGizmo->OnMouseUp(g_x, g_y);
        }
        break;
    default:
        break;
    }
}

static void mousePos(GLFWwindow *w, double x, double y)
{
    g_x = x;
    g_y = y;
    if (g_currentGizmo) {
        g_currentGizmo->OnMouseMove(x, y);
    }
}

static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_W:
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            }
            break;
        case GLFW_KEY_L:
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_LOCAL);
            }
            break;
        case GLFW_KEY_V:
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_VIEW);
            }
            break;
        case GLFW_KEY_M:
            g_currentGizmo = g_moveGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            break;
        case GLFW_KEY_R:
            g_currentGizmo = g_rotateGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_LOCAL);
            break;
        case GLFW_KEY_S:
            g_currentGizmo = g_scaleGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            break;
        case GLFW_KEY_ESCAPE:
             glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
        g_currentGizmo->SetEditMatrix(g_objectMatrix);
        g_currentGizmo->SetScreenDimension(g_width, g_height);
    }
}

}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char *argv[])
{
    if (glfwInit() == GL_FALSE) {
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);

    // Specify minimum OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Only if the requested OpenGL version is 3.0 or above
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // Only if the requested OpenGL version is 3.2 or above
    glfwWindowHint(GLFW_SAMPLES, 4); // default is 0
    //glfwWindowHint(GLFW_DEPTH_BITS, 24); // default is 24
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(g_width, g_height, "LibGizmo", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(window, reshape);
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, mousePos);
    glfwSetKeyCallback(window, keyboard);

    g_moveGizmo = CreateMoveGizmo();
    g_moveGizmo->SetDisplayScale(2.0f);
    g_rotateGizmo = CreateRotateGizmo();
    g_rotateGizmo->SetDisplayScale(2.0f);
    g_scaleGizmo = CreateScaleGizmo();
    g_scaleGizmo->SetDisplayScale(2.0f);
    g_currentGizmo = g_rotateGizmo;
    g_currentGizmo->SetScreenDimension(g_width, g_height);
    g_currentGizmo->SetEditMatrix(g_objectMatrix);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glDepthMask(1);

    while (!glfwWindowShouldClose(window)) {
        drawScene();
        glfwSwapBuffers(window);
        //glfwPollEvents(); // use this if something is going on, such as an animation in a game
        glfwWaitEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
