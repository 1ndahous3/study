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

int sectors = 100, slices = 5;
int keyframes = 20, currentframe = 0;

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

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_bot.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_bot.getNel());

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_top.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_top.getNel());

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_left.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_left.getNel());

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_right.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_right.getNel());

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_back.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_back.getNel());

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertexPointer(3, GL_FLOAT, 0, cube.squ_front.getVertsArray());
	glDrawArrays(GL_QUADS, 0, cube.squ_front.getNel());

	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCylinder(Cylinder &cyl) {
	glColor3f(0.0f, 1.0f, 0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cyl.getVertsArray());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, cyl.getNormsArray());

	glDrawArrays(GL_QUADS, 0, cyl.getNel());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cyl.cir_top.getVertsArray());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, cyl.cir_top.getNormsArray());

	glDrawArrays(GL_QUADS, 0, cyl.cir_top.getNel());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cyl.cir_bot.getVertsArray());

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, cyl.cir_bot.getNormsArray());

	glDrawArrays(GL_QUADS, 0, cyl.cir_bot.getNel());
}



void modelview() {


	// glMatrixMode(GL_MODELVIEW);
	// glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// glTranslatef(x_t, y_t, 0.0f);
	// glRotatef(r_xy, 0.0f, 0.0f, 1.0f);
	// glRotatef(r_yz, 1.0f, 0.0f, 0.0f);
	// glRotatef(r_xz, 0.0f, 1.0f, 0.0f);
	// glScalef(k, k, k);



	glm::mat4 model_mtrx;
	glm::mat4 view_mtrx;
	glm::mat4 proj_mtrx;
	glm::mat4 mvp_mtrx;

	model_mtrx = glm::translate(model_mtrx, glm::vec3(x_t, y_t, 0.0f));
	model_mtrx = glm::rotate(model_mtrx, glm::radians(r_xy), glm::vec3(0.0f, 0.0f, 1.0f));
	model_mtrx = glm::rotate(model_mtrx, glm::radians(r_yz), glm::vec3(1.0f, 0.0f, 0.0f));
	model_mtrx = glm::rotate(model_mtrx, glm::radians(r_xz), glm::vec3(0.0f, 1.0f, 0.0f));
	model_mtrx = glm::scale(model_mtrx, glm::vec3(k));

	// view_mtrx = glm::lookAt(
	// 	glm::vec3(0, 0, 2),
	// 	glm::vec3(0, 0, 0),
	// 	glm::vec3(0, 1, 0));

	// proj_mtrx = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	mvp_mtrx = proj_mtrx * view_mtrx * model_mtrx;
	// mvp_mtrx = view_mtrx * model_mtrx;
	// mvp_mtrx = model_mtrx;

	glMultMatrixf(glm::value_ptr(mvp_mtrx));

	// light_mtrx = glm::inverse(model_mtrx) * light_mtrx;
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
		case GLFW_KEY_T:
			currentframe++;
			// fragmentation
		case GLFW_KEY_KP_ADD: sectors++; break;
		case GLFW_KEY_KP_SUBTRACT: sectors--; break;
		case GLFW_KEY_KP_MULTIPLY: slices++; break;
		case GLFW_KEY_KP_DIVIDE: slices--; break;

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

	 //glEnable(GL_AUTO_NORMAL);
	 glEnable(GL_LIGHTING);
	 glShadeModel(GL_FLAT); // "Эт что?" - Роман
	 glEnable(GL_LIGHT0);

GLfloat ambientLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat specularLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat light_position[] = {0.0f, 0.0f, 2.0f, 0.0f};

glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);
// glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

// glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
// glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, light_position);



	// Bezier bezier;	
	Cylinder cylinder;
	Cube cube;

	cylinder.calculateVert(keyframes, currentframe);
	cylinder.calculateNorms();
	cube.calculateVert(keyframes, currentframe);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//projection();
		// glLoadIdentity();
		// glTranslated(-1.0f, -0.45f, 0.0f);
		// glScalef(0.1f, 0.1f, 0.1f);
		
		// drawCube(cube);


		

	glColor3f(1.0f, 0.0f, 0.0f);

		glPushMatrix();
		modelview();

		//glScalef(0.5f, 0.5f, 0.5f);
		// drawCube(cube);
		drawCylinder(cylinder);
		
		glPopMatrix();

		//glLightfv(GL_LIGHT0, GL_POSITION, light1_position);


		if (sectors != cylinder.sectors || slices != cylinder.slices || currentframe != cylinder.currentframe) {
			cylinder.changeFragm(sectors, slices, currentframe);
			cylinder.calculateVert(keyframes, currentframe);
			cylinder.calculateNorms();
		}
		// drawCylinder(cylinder);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
