#define ENABLE_LOGGING

#include "Collections/List.hpp"
#include "IO/IOHandler.h"
#include "Common/String.h"

using namespace mtk;

template <typename T>
void LogSize()
{
	IOHandler::WriteLine("{}: {}", typeid(T).name(), sizeof(T));
}

int main()
{
	// TODO: Fix up List class
	List<String> list;

	IO::WriteLine(list);


	IO::WriteLine(list);
	return 0;
}
