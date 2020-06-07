#include "engine/Engine.h"

int main(int argc, char *argv[]) {
	Engine engine("test", 1080, 720, 60);

	engine.start();

	return 0;
}