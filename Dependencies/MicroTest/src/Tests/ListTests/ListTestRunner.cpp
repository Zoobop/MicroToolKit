//#include "ListTestRunner.h"
//
//#define TEST(func) TestRunner<List<ComplexType>>::ShowResults(func, NAMEOF(func))
//
//void TestRunner<List<ComplexType>>::Run()
//{
//    TEST_MESSAGE       ("======================================================================\n");
//    TEST_MESSAGE_FORMAT("  Zoobop's {} TestRunner                                              \n", NAMEOF(List<ComplexType>));
//    TEST_MESSAGE       ("======================================================================\n");
//
//    TEST(Test_Constructors);
//    TEST(Test_Add);
//    TEST(Test_Emplace);
//}
//
//void TestRunner<List<ComplexType>>::ShowResults(const Func<bool>& _function, const String& _testName)
//{
//    TEST_INFO_FORMAT("Running Test ({})... ", _testName.Data());
//    if (_function())
//    {
//        TEST_GOOD_RESULT("[SUCCESS]");
//        return;
//    }
//    
//    TEST_BAD_RESULT("[FAILURE]");
//}
//
//bool TestRunner<List<ComplexType>>::Test_Constructors()
//{
//    try
//    {
//        //// Constructor()
//        {
//            // Setup
//            const List<ComplexType> list;
//
//            // Assert
//            TEST_ASSERT(list.Size() == 0);
//            TEST_ASSERT(list.Capacity() == 4);
//            TEST_ASSERT(list.Data() != nullptr);
//        }
//
//        //// Constructor(const List<T>&)
//        {
//            // Setup
//            const List<ComplexType> list1 = { { "Zoobop", "Boobop", 0 } };
//            const List<ComplexType> list2 = list1;
//
//            // Assert
//            TEST_ASSERT(list1.Size() == 1);
//            TEST_ASSERT(list2.Size() == 1);
//        }
//        
//        //// Constructor(List<T>&&)
//        {
//            // Setup
//            List<ComplexType> list1 = { { "Zoobop", "Boobop", 0 } };
//            const List<ComplexType> list2 = std::move(list1);
//
//            // Assert
//            TEST_ASSERT(list1.Data() == nullptr);
//            TEST_ASSERT(list1.Size() == 0);
//            TEST_ASSERT(list2.Size() == 1);
//            TEST_ASSERT(list2.Capacity() == 1);
//        }
//            
//        //// Constructor(const std::vector<T>&)
//        {
//            // Setup
//            const std::vector<ComplexType> vector = { { "Zoobop", "Boobop", 0 } };
//            const List<ComplexType> list = vector;
//
//            // Assert
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 1);
//        }
//
//        //// Constructor(std::vector<T>&&)
//        {
//            // Setup
//            std::vector<ComplexType> vector = { { "Zoobop", "Boobop", 0 } };
//            const List<ComplexType> list = std::move(vector);
//
//            // Assert
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 1);
//        }
//
//        //// Constructor(std::initializer_list<T>&&)
//        {
//            // Setup
//            const ComplexType complex = { "Zoobop", "Boobop", 0 };
//            const List<ComplexType> list = { complex };
//
//            // Assert
//            TEST_ASSERT(list.Data()[0].ToString() == complex.ToString());
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 1);
//        }
//
//        //// explicit Constructor(size_t)
//        {
//            const List<ComplexType> list(20);
//
//            // Assert
//            TEST_ASSERT(list.Size() == 0);
//            TEST_ASSERT(list.Capacity() == 20);
//        }
//        
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_Add()
//{
//    try
//    {
//        //// Add(const T&)
//        {
//            // Setup
//            List<ComplexType> list;
//            const ComplexType complex = { "Brandon", "Cunningham", 21 };
//            
//            // Add l-value reference
//            list.Add(complex);
//            
//            // Assert
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 4);
//            TEST_ASSERT(list.Data() != nullptr);
//        }
//
//        //// Add(T&&)
//        {
//            // Setup
//            List<ComplexType> list;
//            ComplexType complex = { "Brandon", "Cunningham", 21 };
//            
//            // Add r-value reference
//            list.Add(std::move(complex));
//            
//            // Assert
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 4);
//            TEST_ASSERT(list.Data() != nullptr);
//        }
//        
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<Person>>::Test_Emplace()
//{
//    try
//    {
//        //// Emplace(Args&&...)
//        {
//            // Setup
//            List<ComplexType> list;
//            
//            // Add r-value reference
//            list.Emplace("Brandon", "Cunningham", 21);
//            
//            // Assert
//            TEST_ASSERT(list.Size() == 1);
//            TEST_ASSERT(list.Capacity() == 4);
//            TEST_ASSERT(list.Data() != nullptr);
//        }
//        
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_Insert()
//{
//    try
//    {
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_Remove()
//{
//    try
//    {
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_RemoveAt()
//{
//    try
//    {
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_Contains()
//{
//    try
//    {
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
//
//bool TestRunner<List<ComplexType>>::Test_Clear()
//{
//    try
//    {
//        return true;
//    }
//    catch (const std::exception&)
//    {
//        return false;
//    }
//}
