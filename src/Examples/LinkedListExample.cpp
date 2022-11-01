#include "mtkpch.h"
#include "Example.h"

#include "Structures/LinkedList.h"

#if 0
#define BENCHMARK_LINKEDLIST_INT
#define BENCHMARK_LINKEDLIST_VECTOR3
#define BENCHMARK_LINKEDLIST_PLAYER
#endif

#include "File/FileHandler.h"

int main()
{
#ifdef BENCHMARK_LINKEDLIST_INT
	LOG("Micro ToolKit LinkedList Benchmarks (int)");
	LOG("-----------------------------------------");

	LOG("\nSize: 100");
	LOG("---------------------------------------------");

	mtk::LinkedList<int> linkedListInt100;
	{
		TEST_PROFILER("Push");
		for (auto i = 0; i < 100; i++)
			linkedListInt100.Push(i * 2);
	}
	{
		TEST_PROFILER("Reverse");
		linkedListInt100.Reverse();
	}
	{
		TEST_PROFILER("Contains (True)");
		linkedListInt100.Contains(100);
	}
	{
		TEST_PROFILER("Contains (False)");
		linkedListInt100.Contains(101);
	}
	{
		TEST_PROFILER("Remove");
		linkedListInt100.Remove(100);
	}
	{
		TEST_PROFILER("Clear");
		linkedListInt100.Clear();
	}
	LOG("");


	LOG("\nSize: 1000");
	LOG("---------------------------------------------");

	mtk::LinkedList<int> linkedListInt1000;
	{
		TEST_PROFILER("Push");
		for (auto i = 0; i < 500; i++)
			linkedListInt1000.Push(i * 2);
	}
	{
		TEST_PROFILER("Reverse");
		linkedListInt1000.Reverse();
	}
	{
		TEST_PROFILER("Contains (True)");
		linkedListInt1000.Contains(1000);
	}
	{
		TEST_PROFILER("Contains (False)");
		linkedListInt1000.Contains(1001);
	}
	{
		TEST_PROFILER("Remove");
		linkedListInt1000.Remove(1000);
	}
	{
		TEST_PROFILER("Clear");
		linkedListInt1000.Clear();
	}
	LOG("");


	LOG("\nSize: 10000");
	LOG("---------------------------------------------");

	mtk::LinkedList<int> linkedListInt10000;
	{
		TEST_PROFILER("Push");
		for (auto i = 0; i < 5000; i++)
			linkedListInt10000.Push(i * 2);
	}
	{
		TEST_PROFILER("Reverse");
		linkedListInt10000.Reverse();
	}
	{
		TEST_PROFILER("Contains (True)");
		linkedListInt10000.Contains(10000);
	}
	{
		TEST_PROFILER("Contains (False)");
		linkedListInt10000.Contains(10001);
	}
	{
		TEST_PROFILER("Remove");
		linkedListInt10000.Remove(10000);
	}
	{
		TEST_PROFILER("Clear");
		linkedListInt10000.Clear();
	}
	LOG("");


	LOG("\nSize: 100000");
	LOG("---------------------------------------------");

	mtk::LinkedList<int> linkedListInt100000;
	{
		TEST_PROFILER("Push");
		for (auto i = 0; i < 50000; i++)
			linkedListInt100000.Push(i * 2);
	}
	{
		TEST_PROFILER("Reverse");
		linkedListInt100000.Reverse();
	}
	{
		TEST_PROFILER("Contains (True)");
		linkedListInt100000.Contains(100000);
	}
	{
		TEST_PROFILER("Contains (False)");
		linkedListInt100000.Contains(100001);
	}
	{
		TEST_PROFILER("Remove");
		linkedListInt100000.Remove(100000);
	}
	{
		TEST_PROFILER("Clear");
		linkedListInt100000.Clear();
	}
	LOG("");


	LOG("\nSize: 1000000");
	LOG("---------------------------------------------");

	mtk::LinkedList<int> linkedListInt1000000;
	{
		TEST_PROFILER("Push");
		for (auto i = 0; i < 500000; i++)
			linkedListInt1000000.Push(i * 2);
	}
	{
		TEST_PROFILER("Reverse");
		linkedListInt1000000.Reverse();
	}
	{
		TEST_PROFILER("Contains (True)");
		linkedListInt1000000.Contains(1000000);
	}
	{
		TEST_PROFILER("Contains (False)");
		linkedListInt1000000.Contains(1000001);
	}
	{
		TEST_PROFILER("Remove");
		linkedListInt1000000.Remove(1000000);
	}
	{
		TEST_PROFILER("Clear");
		linkedListInt1000000.Clear();
	}
	LOG("");
#endif
	//std::cin.get();
}