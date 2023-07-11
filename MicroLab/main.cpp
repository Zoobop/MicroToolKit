#include "Collections/Array.hpp"
#include "Collections/LinkedList.hpp"
#include "Collections/List.hpp"
#include "Collections/Map.hpp"
#include "Collections/Queue.hpp"
#include "Collections/Stack.hpp"
#include "Collections/Set.hpp"
#include "IO/IOHandler.hpp"
#include "Common/String.hpp"
#include "Common/StringBuffer.hpp"
#include "Common/StringBuilder.hpp"

using namespace Micro;


int main()
{
	// TODO: Overhaul String/StringBuffer/StringBuilder classes
	// TODO: Create Unit Tests for remaining Collections: Array, List, Map, Queue, Stack

	String fullName = "Brandon ";
	std::string lastName = "Cunningham";

	IO::WriteLine(fullName);
	IO::WriteLine(lastName);

	fullName.Append(lastName);

	IO::WriteLine(fullName);
	return 0;
}
