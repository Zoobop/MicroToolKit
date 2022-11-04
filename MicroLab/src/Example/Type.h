#pragma once
#include "Common/String.h"

using namespace mtk;

class Type final
{
public:
    Type(const String& _name, newhash_t _hashCode)
        : m_TypeName(_name), m_HashCode(_hashCode)
    {
    }

    NODISCARD const String& Name() const { return m_TypeName; }
    NODISCARD newhash_t HashCode() const { return m_HashCode; }

private:
    const String m_TypeName;
    const newhash_t m_HashCode;
};
