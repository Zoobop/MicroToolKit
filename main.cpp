

#include <iostream>

#include "Common/String.h"
#include "Utility/Log.h"

using namespace mtk;

int main()
{
    String string = "1011001101";

    String newString = string.Trim("10");
    
    std::cout << newString << " " << newString.Size();
    
    return 0;
}
