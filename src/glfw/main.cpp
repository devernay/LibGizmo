/* port of win32example in GLFW 2.7 to test */

#include <stdlib.h>
#include <GL/glfw.h>
#include <OpenGL/glu.h>

#include "IGizmo.h"

namespace {

bool g_running = true;
int g_x = 0, g_y = 0, g_width = 640, g_height = 480;

float g_objectMatrix[16] = {
    -0.3210,
    0.0000,
    0.9471,
    0.0000,
    0.0000,
    1.0000,
    0.0000,
    0.0000,
    -0.9471,
    0.0000,
    -0.3210,
    0.0000,
    -137.1790,
    16.4949,
    375.4003,
    1.0000
};

IGizmo *g_moveGizmo = 0, *g_rotateGizmo = 0, *g_scaleGizmo = 0, *g_currentGizmo = 0;

static void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer

    float viewMat[16]={-0.4747,-0.2647,0.8394,0.0000,0.0000,0.9537,0.3007,0.0000,-0.8802,0.1427,-0.4527,0.0000,180.6443,-110.9036,-91.6591,1.0000};
    float projMat[16]={0.5625,0.0000,0.0000,0.0000,0.0000,1.0000,0.0000,0.0000,0.0000,0.0000,-1.0002,-1.0000,0.0000,0.0000,-0.2000,0.0000};
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glLoadMatrixf( projMat );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glLoadMatrixf( viewMat );

    glPushMatrix();
    glMultMatrixf( g_objectMatrix );
    glScalef(50,50,50);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f);
    glCullFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glBegin(GL_QUADS);
    // Front Face
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glColor4f(0.6f,0.6f,0.6f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glColor4f(0.5f,0.5f,0.5f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glColor4f(0.4f,0.4f,0.4f,1.f);glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    glPopMatrix();

    if (g_currentGizmo) {
        g_currentGizmo->SetCameraMatrix(viewMat, projMat);
        g_currentGizmo->Draw();
    }
}

static void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

    if (g_currentGizmo) {
        g_currentGizmo->SetScreenDimension(width, height);
    }
}

static void mouseButton(int button, int action)
{
    switch (action) {
    case GLFW_PRESS:
        if (g_currentGizmo) {
            g_currentGizmo->OnMouseDown(g_x, g_y);
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

static void mousePos(int x, int y)
{
    g_x = x;
    g_y = y;
    if (g_currentGizmo) {
        g_currentGizmo->OnMouseMove(x, y);
    }
}

static void keyboard(int key, int action)
{
    if (action == GLFW_PRESS) {
        switch (key) {
        case 'W':
        case 'w':
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            }
            break;
        case 'L':
        case 'l':
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_LOCAL);
            }
            break;
        case 'V':
        case 'v':
            if (g_currentGizmo) {
                g_currentGizmo->SetLocation(IGizmo::LOCATE_VIEW);
            }
            break;
        case 'M':
        case 'm':
            g_currentGizmo = g_moveGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            break;
        case 'R':
        case 'r':
            g_currentGizmo = g_rotateGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_LOCAL);
            break;
        case 'S':
        case 's':
            g_currentGizmo = g_scaleGizmo;
            g_currentGizmo->SetLocation(IGizmo::LOCATE_WORLD);
            break;
        case GLFW_KEY_ESC:
            g_running = false;
            break;
        }
        g_currentGizmo->SetEditMatrix(g_objectMatrix);
        g_currentGizmo->SetScreenDimension(g_width, g_height);
    }
}

}

int main(int argc, char *argv[])
{
    if (glfwInit() == GL_FALSE) {
        return EXIT_FAILURE;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    if (glfwOpenWindow(g_width, g_height, 0, 0, 0, 0, 24, 0, GLFW_WINDOW) == GL_FALSE) {
        return EXIT_FAILURE;
    }

    glfwEnable(GLFW_MOUSE_CURSOR);
    glfwSetWindowTitle("LibGizmo");
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(reshape);
    glfwSetMouseButtonCallback(mouseButton);
    glfwSetMousePosCallback(mousePos);
    glfwSetKeyCallback(keyboard);

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

    while (g_running) {
        drawScene();
        glfwSwapBuffers();
        if (glfwGetWindowParam(GLFW_OPENED) == GL_FALSE) {
            g_running = false;
        }
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
