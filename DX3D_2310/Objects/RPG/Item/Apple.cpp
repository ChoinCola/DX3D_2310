#include "Framework.h"
#include "Apple.h"

Apple::Apple()
	: Item("Apple" + Applecount)
{
	Applecount++;
}

Apple::~Apple()
{
}
