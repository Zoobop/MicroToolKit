#include "Tests/StringTestRunner.h"
#include "IO/IOHandler.h"

int main()
{
    //TestRunner<String>::Run();
    
    //String str = "|x|o|x| |x|o|x| |x|o|x| |x|o|x|";

    //List<String> splitList = str.Split({ ' ', 'o' });

    //IOHandler::WriteLine(splitList);
    String strings[10];
    strings[0] = "Zoobop";
    
    Sequence sequence { strings, 6 };
    for (const auto& item : sequence)
    {
        IOHandler::Write(item);
    }
    
    return 0;
}
