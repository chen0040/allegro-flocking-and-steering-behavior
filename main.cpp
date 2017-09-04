#include "XSEngine.h"

int main(int argc, char* argv[])
{
	XSEngine engine;
	if(engine.Init(640, 480, true) == false)
	{
		return 0;
	}
	engine.Start();

	return 0;
}
END_OF_MAIN()