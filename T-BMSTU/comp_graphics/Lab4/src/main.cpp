#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "point.hpp"

int width = 800, height = 600;
int blur_rate = 5, blur_rate_sq = 2 << (blur_rate - 1);

Shape shape;

bool wait_vertex = true;
bool wait_fill = false;

void framebuffer_resize_callback(GLFWwindow *window, int w, int h) {
    width = w, height = h;
    glViewport(0, 0, w, h);
    shape.resize();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
        int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
            // translate
        case GLFW_KEY_P:
            wait_vertex = true;
            break;
        case GLFW_KEY_F:
            wait_fill = true;
            wait_vertex = false;
            break;
            //case GLFW_KEY_O: break;
        }
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    if (action == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);
        if (wait_fill) {
            shape.fillShape((int) x, (int) (height - y));
        }
        if (wait_vertex) {
            shape.addPoint((int) x, (int) (height - y));
        }
    }
}

int main() {

    GLFWwindow* window;

    glewExperimental = GL_TRUE;
    
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glewInit();

    glDisable (GL_DEPTH_TEST);
   // glEnable (GL_ACCUM);
    glReadBuffer(GL_BACK);
    glDrawBuffer(GL_BACK);
    
    glShadeModel(GL_SMOOTH);
    
    glDepthFunc (GL_LESS);
    glViewport(0, 0, width, height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, mtrx);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
