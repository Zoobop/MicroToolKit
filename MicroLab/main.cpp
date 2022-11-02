#include "IO/IOHandler.hpp"
#include "Tests/StringTestRunner.h"

int main()
{
    //TestRunner<String>::Run();

    IOHandler::Write("Enter your name: ");
    const String input = IOHandler::ReadLine();
    
    IOHandler::Write(input);
    
    return 0;
}
