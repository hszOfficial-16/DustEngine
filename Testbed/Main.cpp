#include "GameDirector.h"

int main()
{
	GameDirector::GetInstance().ShiftScene("Test");
	GameDirector::GetInstance().Run();
	return 0;
}