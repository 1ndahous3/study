#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shape.hpp"

int sectors = 10, sections = 5;
float width = 800, height = 600;
bool need_clip = false;

GLfloat r_xy = 0.0f, r_yz = 0.0f, r_xz = 0.0f; // rotate radius
GLfloat k = 1.0f;                              // scale
GLfloat x_t = 0.0f, y_t = 0.0f;                // translate

GLfloat f = 0.5;

GLfloat fi = asin(f / sqrt(2)) * 180 / M_PI;
GLfloat ro = asin(f / sqrt(2 - f * f)) * 180 / M_PI;

void modelview() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x_t, y_t, 0.0f);
    glRotatef(r_xy, 0.0f, 0.0f, 1.0f);
    glRotatef(r_yz, 1.0f, 0.0f, 0.0f);
    glRotatef(r_xz, 0.0f, 1.0f, 0.0f);
    glScalef(k, k, k);
}

void projection() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width / height, width / height, -1.0f, 1.0f, -1.0f, 1.0f);
    glRotatef(ro, 0.0f, 0.0f, 1.0f);
    glRotatef(fi, 1.0f, 1.0f, 0.0f);
}

void framebuffer_resize_callback(GLFWwindow *window, int w, int h) {

    width = (float) w, height = (float) h;
    glViewport(0, 0, w, h);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
        int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
//            translate
        case GLFW_KEY_RIGHT:
            x_t += 0.2f;
            break;
        case GLFW_KEY_LEFT:
            x_t -= 0.2f;
            break;
        case GLFW_KEY_UP:
            y_t += 0.2f;
            break;
        case GLFW_KEY_DOWN:
            y_t -= 0.2f;
            break;
//            scale
        case GLFW_KEY_LEFT_BRACKET:
            k = (k -= 0.2f) < 0 ? 0 : k;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            k = (k += 0.2f) < 0 ? 0 : k;
            break;
//            rotate
        case GLFW_KEY_A:
            r_xy += 10.0f;
            break;
        case GLFW_KEY_D:
            r_xy -= 10.0f;
            break;
        case GLFW_KEY_S:
            r_yz += 10.f;
            break;
        case GLFW_KEY_W:
            r_yz -= 10.f;
            break;
        case GLFW_KEY_Q:
            r_xz += 10.f;
            break;
        case GLFW_KEY_E:
            r_xz -= 10.f;
            break;
        case GLFW_KEY_O:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case GLFW_KEY_P:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case GLFW_KEY_C:
            need_clip = !need_clip;
            break;
//            fragmentation
        case GLFW_KEY_KP_ADD:
            sectors++;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            sectors--;
            break;
        case GLFW_KEY_KP_MULTIPLY:
            sections++;
            break;
        case GLFW_KEY_KP_DIVIDE:
            sections--;
            break;

        }
    }

}

int main(void) {

    srand(time(NULL));

    GLFWwindow* window;

    glewExperimental = GL_TRUE;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Lab5", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glViewport(0, 0, width, height);

    std::vector<Line3D> lines, lines_clipped;
    Cube cube(2);
    cube.calculateVert();

    for (int i = 0; i < 30; i++) {
        Line3D line;
        lines.push_back(line);
        lines_clipped.push_back(line.clip(cube));
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        projection();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslated(-1.0f, -0.45f, 0.0f);

        modelview();
        glColor3f(0.0f, 1.0f, 0.0f);
        cube.render();

        glColor3f(1.0f, 0.0f, 0.0f);

        for (const auto &line : (need_clip ? lines_clipped : lines)) {
            line.render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
