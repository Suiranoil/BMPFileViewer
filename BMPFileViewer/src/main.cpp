#include "Core/Application.h"

#include <Windows.h>

int main(int argc, char* argv[])
{
	auto app = new Application();
	app->Run();
	delete app;

	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
#ifdef DEBUG
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	return main(__argc, __argv);
}
