#include "Collections/Array.hpp"
#include "Collections/LinkedList.hpp"
#include "Collections/List.hpp"
#include "Collections/Queue.hpp"
#include "Collections/Stack.hpp"
#include "Collections/Set.hpp"
#include "Experimental/Enumerable.hpp"
#include "IO/IOHandler.hpp"
#include "Common/String.h"
#include "Utility/ContainerUtils.hpp"
#include "Utility/Tuple.hpp"

using namespace Micro;


int main()
{
	// TODO: Overhaul Set implementation

	Set<String> set1 = {"Zoobop", "is", "a", "god"};
	Set<String> set2 = {"not", "okay", "and", "Boobop"};

	IO::WriteLine(set1);
	IO::WriteLine(set2);

	set1.UnionWith(set2);

	IO::WriteLine(set1);
	return 0;
}
