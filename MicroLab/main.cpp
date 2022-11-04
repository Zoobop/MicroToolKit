#include "Common/List.h"
#include "Example/Person.h"
#include "IO/FileHandler.h"
#include "IO/IOHandler.h"

int main()
{
    //TestRunner<String>::Run();
    
    //String str = "|x|o|x| |x|o|x| |x|o|x| |x|o|x|";

    //List<String> splitList = str.Split({ ' ', 'o' });

    //IOHandler::WriteLine(splitList);

    const String path = R"(C:\dev\MicroToolKit\MicroToolKit\MicroLab\src\Output\output.out)";
    
    Sequence<String> data = { "aaaa", "bbbb", "cccc", "dddd", "eeee", "ffff" };
    
    FileHandler::WriteLines(path, data);
    IOHandler::WriteLine(data.Data());
    
    return 0;
}
