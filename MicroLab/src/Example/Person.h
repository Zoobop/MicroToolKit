#pragma once
#include "Object.h"
#include "Common/String.h"

using namespace mtk;

class Person final : public BaseObject<Person>
{
public:
    
    Person() = default;
    //Person(const Person&) = default;
    //Person(Person&&) = default;

    Person(const String& _firstName, const String& _lastName, uint16_t _age)
    : m_FirstName(_firstName), m_LastName(_lastName), m_Age(_age)
    {
    }
    ~Person() = default;

    NODISCARD const String& FirstName() const { return m_FirstName; }
    NODISCARD const String& LastName() const { return m_LastName; }
    NODISCARD uint16_t Age() const { return m_Age; }
    
    Person& Assign(const Person& _object) override
    {
        m_FirstName = _object.m_FirstName;
        m_LastName = _object.m_LastName;
        m_Age = _object.m_Age;
        
        return *this;
    }
    
    Person& Assign(Person&& _object) override
    {
        m_FirstName = _object.m_FirstName;
        m_LastName = _object.m_LastName;
        m_Age = _object.m_Age;

        _object.m_FirstName.Clear();
        _object.m_LastName.Clear();
        _object.m_Age = 0;
        
        return *this;
    }
    
    NODISCARD bool Equals(const Person& _object) const override
    {
        return m_FirstName.Data() == _object.m_FirstName.Data() &&
               m_LastName.Data() == _object.m_LastName.Data() &&
               m_Age == _object.m_Age;
    }
    
    NODISCARD String ToString() const override
    {
        return m_FirstName + " " + m_LastName;
    }

private:
    String m_FirstName;
    String m_LastName;
    uint16_t m_Age = 0;
};

typedef Person ComplexType;
