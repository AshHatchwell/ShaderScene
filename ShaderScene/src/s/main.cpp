#include "../h/ShaderApplication.h"

int main() {

	MainApplication* shaderApp = new ShaderApplication();

	int err = shaderApp->startup("ShaderScene");

	if (err != 0) {
		shaderApp->shutdown();
		return err;
	}
	err = shaderApp->run();
	if (err != 0) {
		shaderApp->shutdown();
		return err;
	}

	err = shaderApp->shutdown();

	return err;
}