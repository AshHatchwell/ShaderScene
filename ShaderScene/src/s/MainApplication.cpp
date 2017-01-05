#include "MainApplication.h"
#include "AppGlobals.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <time.h>

#include "Mesh.h"
#include "Camera.h"


using namespace AppGlobals;

int MainApplication::startup(const char* windowName) {
	int err = createWindow(windowName, 1280, 720);
	if (err != APP_NO_ERROR)
		return err;

	err = initialize();
	if (err != APP_NO_ERROR)
		return err;

	return APP_NO_ERROR;
}

int MainApplication::initialize() {
	// To be overridden by children.
	return APP_NO_ERROR;
}

int MainApplication::shutdown() {
	int err = terminate();
	if (err != APP_NO_ERROR) {
		// We might want this to do something later.
		// But for now it's closing anyway.
		// Potentially have a virtual function for handling
		// errors during termination.
		// But then couldn't we just handle them during termination?
	}

	destroyWindow();
	return APP_NO_ERROR;
}

int MainApplication::terminate() {
	// To be overridden by children.
	return APP_NO_ERROR;
}

int MainApplication::update() {

	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return APP_CLOSED;

	return APP_NO_ERROR;
}

int MainApplication::draw() {

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	return APP_NO_ERROR;
}




int MainApplication::run() {
	double prevTime = glfwGetTime();
	double currTime = 0;

	int err = 0;

	while (currTime = glfwGetTime(),
		AppGlobals::deltaTime = (float)(currTime - prevTime),
		(err = update()) == APP_NO_ERROR) {

		glfwPollEvents();
		draw();
		glfwSwapBuffers(m_window);

		prevTime = currTime;
	}

	return err;
}

int MainApplication::createWindow(const char* title, int width, int height) {

	// Initialise OpenGL
	if (glfwInit() == GL_FALSE)
		return APP_GL_INIT_FAIL;

	// Create window
	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr) {
		glfwTerminate();
		return APP_GL_WINDOW_CREATE_FAIL;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return APP_GL_LOAD_FUNCTIONS_FAIL;
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h) {glViewport(0, 0, w, h); });

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	glClearColor(0.25f, 0.25f, 0.25f, 1.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return APP_NO_ERROR;
}

void MainApplication::destroyWindow() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}