#include "Common/List.h"
#include "IO/IOHandler.h"
#include "Tests/ListTests/ListTestRunner.h"
#include "Tests/StringTests/StringTestRunner.h"

template<typename T>
void Swap(T* data, size_t index1, size_t index2)
{
    const T temp = data[index1];
    data[index1] = data[index2];
    data[index2] = temp;
}

int main()
{
    //TestRunner<String>::Run();
    //TestRunner<List<ComplexType>>::Run();

    List<String> list = { "Brandon", "Nathan", "Ruby", "Montrell", "Austin", "Gale" };

    IOHandler::WriteLine(list);
    
    uint32_t removedCount = list.RemoveAll([](const String& string) { return string.Contains('a'); });
    
    IOHandler::WriteLine(list);
    IOHandler::WriteLine(removedCount);
    
    return 0;
}
