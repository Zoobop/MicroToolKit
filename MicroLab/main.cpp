#define ENABLE_LOGGING

#include "Collections/Array.hpp"
#include "Collections/List.hpp"
#include "IO/IOHandler.h"
#include "Common/String.h"
#include "Core/Timer.hpp"
#include "Test/Object.hpp"
#include "Utility/CollectionUtils.hpp"

using namespace mtk;

template <typename T>
void LogSize()
{
	IOHandler::WriteLine("{}: {}", typeid(T).name(), sizeof(T));
}


int main()
{
	//// Allocate
	//constexpr size_t size = 4;
	//auto data = Alloc<String>(size);

	//// Set data
	//new(&data[0]) String{"Is"};
	//new(&data[1]) String{"Zoobop"};
	//new(&data[2]) String{"a"};
	//new(&data[3]) String{"god"};

	//IO::WriteLine(ArrayToString(data, size));

	//data[1].~String();
	//ShiftLeft(data, size, 2);

	//IO::WriteLine(ArrayToString(data, size - 1));

	//// Invalidate data
	//for (size_t i = 0; i < size - 1; i++)
	//	data[i].~String();

	//// Dispose
	//Delete(data, size - 1);
	//return 0;

	Container<String> container("Zoobop", "is", "a", "god");

	List<String> list;

	IO::WriteLine(list);

	list.AddRange(container);

	IO::WriteLine(list);

	list.Insert(3, "great");

	IO::WriteLine(list);

	IO::WriteLine();
	IO::WriteLine("Size: {}", list.Size());
	IO::WriteLine("Capacity: {}", list.Capacity());
	return 0;
}
