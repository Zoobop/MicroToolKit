#pragma once

namespace mtk
{
    struct Range final
    {
        size_t start;
        size_t end;
        
        Range(size_t _start, size_t _end)
            : start(_start), end(_end)
        {
        }
    };
}
