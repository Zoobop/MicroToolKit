#include "IO/IOHandler.hpp"
#include "Tests/StringTestRunner.h"
#include "Utility/BufferView.h"

int main()
{
    //TestRunner<String>::Run();

    BufferView bufferView = "Zoobop";
    BufferView slicedView = bufferView.Slice(1, 4);

    IOHandler::WriteLine(bufferView.Data());
    
    for (size_t i = 0; i < slicedView.Size(); i++)
    {
        IOHandler::Write(slicedView[i]);
    }
    
    return 0;
}
