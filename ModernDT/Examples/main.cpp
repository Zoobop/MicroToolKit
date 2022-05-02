#include "mtkpch.h"

#include "Example.h"
#include "File/FileHandler.h"
#include "Structures/Map.h"

int main() 
{
    auto num = 0;
    if (TryParse("10", num))
    {
        LOG(num);
    }
    LOG("Cannot parse!");
}
