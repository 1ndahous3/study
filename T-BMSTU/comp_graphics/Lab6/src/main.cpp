/*
 ============================================================================
 Name        : main.cpp
 Author      : Roman Dudin
 Version     :
 Copyright   : All rights reserved
 Description : Lab3
 ============================================================================
 */


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shape.hpp"

int sectors = 10, sections = 5;
float width = 800, height = 600;

GLfloat r_xy = 0.0f, r_yz = 0.0f, r_xz = 0.0f; // rotate radius
GLfloat k = 1.0f; // scale
GLfloat x_t = 0.0f, y_t = 0.0f; // translate

GLfloat f = 0.5;

GLfloat fi = asin(f / sqrt(2)) * 180 / M_PI;
GLfloat ro = asin(f / sqrt(2 - f * f)) * 180 / M_PI;

void randomColor() {
	glColor3ub(rand() % 256, rand() % 256, rand() % 256);
}

void uniqueColor(float a, float b, float c) {
	glColor3ub(int(a * 1473) % 256, int(b * 1734) % 256, int(c * 1913) % 256);
}

void drawCube(Cube &cube) {

	glColor3f(0.0f, 1.0f, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_bot.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_bot.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_top.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_top.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_left.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_left.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_right.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_right.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_back.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_back.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cube.squ_front.getArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_front.getNel());

	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCylinder(Cylinder &cyl) {
	glColor3f(0.0f, 1.0f, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cyl.getArray());
	glDrawArrays(GL_QUADS, 0, cyl.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cyl.cir_top.getArray());
	glDrawArrays(GL_QUADS, 0, cyl.cir_top.getNel());

	glVertexPointer(3, GL_FLOAT, 0, cyl.cir_bot.getArray());
	glDrawArrays(GL_QUADS, 0, cyl.cir_bot.getNel());

	glDisableClientState(GL_VERTEX_ARRAY);
}

void modelview() {


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x_t, y_t, 0.0f);
	glRotatef(r_xy, 0.0f, 0.0f, 1.0f);
	glRotatef(r_yz, 1.0f, 0.0f, 0.0f);
	glRotatef(r_xz, 0.0f, 1.0f, 0.0f);
	glScalef(k, k, k);

	GLfloat light_position[4];
			
	light_position[0] = cos((r_xz / 180) * M_PI) * cos((r_xy / 180) * M_PI);
	light_position[1] = -sin((r_xy / 180) * M_PI) * cos((r_yz / 180) * M_PI);
	light_position[2] = cos((r_yz / 180) * M_PI) * sin((r_xz / 180) * M_PI);
	light_position[3] = 0.0f;

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
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
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
			// translate
		case GLFW_KEY_RIGHT: x_t += 0.2f; break;
		case GLFW_KEY_LEFT: x_t -= 0.2f; break;
		case GLFW_KEY_UP: y_t += 0.2f; break;
		case GLFW_KEY_DOWN: y_t -= 0.2f; break;
			// scale
		case GLFW_KEY_LEFT_BRACKET:
			k = (k -= 0.2f) < 0 ? 0 : k; break;
		case GLFW_KEY_RIGHT_BRACKET:
			k = (k += 0.2f) < 0 ? 0 : k; break;
			// rotate
		case GLFW_KEY_A: r_xy += 10.0f; break;
		case GLFW_KEY_D: r_xy -= 10.0f; break;
		case GLFW_KEY_S: r_yz += 10.f; break;
		case GLFW_KEY_W: r_yz -= 10.f; break;
		case GLFW_KEY_Q: r_xz += 10.f; break;
		case GLFW_KEY_E: r_xz -= 10.f; break;
		case GLFW_KEY_O:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case GLFW_KEY_P:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			// fragmentation
		case GLFW_KEY_KP_ADD: sectors++; break;
		case GLFW_KEY_KP_SUBTRACT: sectors--; break;
		case GLFW_KEY_KP_MULTIPLY: sections++; break;
		case GLFW_KEY_KP_DIVIDE: sections--; break;

		}
	}

}

int main(void) {
	GLFWwindow* window;
	if (!glfwInit())
		return -1;


	window = glfwCreateWindow(width, height, "Lab3", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	//glewExperimental=GL_TRUE;

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glViewport(0, 0, width, height);

	 GLfloat material_diffuse[] = {0.5, 0.5, 0.5, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

GLfloat light1_position[] = {0.0, 0.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	Cylinder cylinder;
	Cube cube;
	cylinder.calculateVert();
	cube.calculateVert();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//projection();
		// glMatrixMode(GL_MODELVIEW);
		// glLoadIdentity();
		// glTranslated(-1.0f, -0.45f, 0.0f);
		// glScalef(0.1f, 0.1f, 0.1f);
		
		// drawCube(cube);


		

glColor3f(1.0f, 0.0f, 0.0f);


modelview();

		//glScalef(0.5f, 0.5f, 0.5f);
		drawCube(cube);
		
		// if (sectors != cylinder.sectors || sections != cylinder.sections) {
		// 	cylinder.changeFragm(sectors, sections);
		// 	cylinder.calculateVert();
		// }
		// drawCylinder(cylinder);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
