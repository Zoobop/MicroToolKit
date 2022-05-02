#include "mtkpch.h"

#include "Example.h"
#include "File/FileHandler.h"
#include "Structures/Map.h"

int main() 
{
    auto result = mtk::Parse<float>("1");

    LOG(result);
}
