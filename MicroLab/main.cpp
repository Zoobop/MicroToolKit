#include "Tests/StringTestRunner.h"
#include "IO/IOHandler.h"

int main()
{
    //TestRunner<String>::Run();
    
    //String str = "|x|o|x| |x|o|x| |x|o|x| |x|o|x|";

    //List<String> splitList = str.Split({ ' ', 'o' });

    //IOHandler::WriteLine(splitList);
    String string = "Zoobop";
    BufferView view = string;
    
    IOHandler::WriteLine(view.Slice(1, 3));
    
    return 0;
}
