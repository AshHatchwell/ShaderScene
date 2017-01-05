#pragma once

// MainApplication.h
// Controls initialisation/termination of OpenGL,
// the creation and destruction of a basic window
// and maintains delta time and applications loop


struct GLFWwindow;

class Mesh;
class Camera;

class MainApplication {
public:

	MainApplication() {}
	virtual ~MainApplication() {}

	int run();

	// Creates window, then calls initialize()
	int startup(const char* windowName);
	// Calls terminate(), then destroys window
	int shutdown();


protected:
	virtual int update();
	virtual int draw();
	
	// For initialisation specific to the application
	virtual int initialize();
	// For termination specific to the application
	virtual int terminate();
	
	int createWindow(const char* title, int width, int height);
	void destroyWindow();

	GLFWwindow* m_window;

private:

};