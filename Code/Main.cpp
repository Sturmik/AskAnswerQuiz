#include "Interface.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif

// Если у вас возникли вопросы касательно работы программы, загляните в "Interface.h"

int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));

	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);

	system("mode con cols=180 lines=32");
	{
		Interface check;

		check.MainMenu();
	}
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtMemDumpAllObjectsSince(&_ms);
}