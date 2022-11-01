#include "mtkpch.h"
#include "Example.h"

#include "Structures/List.h"

#if 0
#define BENCHMARK_LIST_INT
#define BENCHMARK_LIST_VECTOR3
#define BENCHMARK_LIST_PLAYER
#endif

int main()
{

#ifdef BENCHMARK_LIST_INT
	LOG("ModernDT List Benchmarks (int)");
	LOG("------------------------------");

	LOG("\nSize: 100");
	LOG("---------------------------------------------");

	mtk::List<int> listInt100;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 50; i++)
			listInt100.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 50; i++)
			listInt100.Emplace(i * 2 + 100);
	}
	{
		TEST_PROFILER("Contains (True)");
		listInt100.Contains(100);
	}
	{
		TEST_PROFILER("Contains (False)");
		listInt100.Contains(101);
	}
	{
		TEST_PROFILER("Remove");
		listInt100.Remove(100);
	}
	{
		TEST_PROFILER("RemoveAt");
		listInt100.RemoveAt(50);
	}
	{
		TEST_PROFILER("Clear");
		listInt100.Clear();
	}
	LOG("");


	LOG("\nSize: 1000");
	LOG("---------------------------------------------");

	mtk::List<int> listInt1000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 500; i++)
			listInt1000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 500; i++)
			listInt1000.Emplace(i * 2 + 1000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listInt1000.Contains(1000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listInt1000.Contains(1001);
	}
	{
		TEST_PROFILER("Remove");
		listInt1000.Remove(1000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listInt1000.RemoveAt(500);
	}
	{
		TEST_PROFILER("Clear");
		listInt1000.Clear();
	}
	LOG("");


	LOG("\nSize: 10000");
	LOG("---------------------------------------------");

	mtk::List<int> listInt10000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 5000; i++)
			listInt10000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 5000; i++)
			listInt10000.Emplace(i * 2 + 10000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listInt10000.Contains(10000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listInt10000.Contains(10001);
	}
	{
		TEST_PROFILER("Remove");
		listInt10000.Remove(10000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listInt10000.RemoveAt(5000);
	}
	{
		TEST_PROFILER("Clear");
		listInt10000.Clear();
	}
	LOG("");


	LOG("\nSize: 100000");
	LOG("---------------------------------------------");

	mtk::List<int> listInt100000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 50000; i++)
			listInt100000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 50000; i++)
			listInt100000.Emplace(i * 2 + 100000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listInt100000.Contains(100000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listInt100000.Contains(100001);
	}
	{
		TEST_PROFILER("Remove");
		listInt100000.Remove(100000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listInt100000.RemoveAt(50000);
	}
	{
		TEST_PROFILER("Clear");
		listInt100000.Clear();
	}
	LOG("");


	LOG("\nSize: 1000000");
	LOG("---------------------------------------------");

	mtk::List<int> listInt1000000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 500000; i++)
			listInt1000000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 500000; i++)
			listInt1000000.Emplace(i * 2 + 1000000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listInt1000000.Contains(1000000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listInt1000000.Contains(1000001);
	}
	{
		TEST_PROFILER("Remove");
		listInt1000000.Remove(1000000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listInt1000000.RemoveAt(500000);
	}
	{
		TEST_PROFILER("Clear");
		listInt1000000.Clear();
	}
	LOG("");
#endif // BENCHMARK_LIST_INT

	LOG("\n\n");

#ifdef BENCHMARK_LIST_VECTOR3
	LOG("ModernDT List Benchmarks (Vector3)");
	LOG("----------------------------------\n");

	LOG("Size: 100");
	LOG("---------------------------------------------");

	mtk::List<Vector3> listVec100;
	{
		TEST_PROFILER("Add");
		for (float i = 0; i < 50; i++)
			listVec100.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (float i = 0; i < 50; i++)
			listVec100.Emplace(i * 2 + 100);
	}
	{
		TEST_PROFILER("Contains (True)");
		listVec100.Contains(100);
	}
	{
		TEST_PROFILER("Contains (False)");
		listVec100.Contains(101);
	}
	{
		TEST_PROFILER("Remove");
		listVec100.Remove(100);
	}
	{
		TEST_PROFILER("RemoveAt");
		listVec100.RemoveAt(50);
	}
	{
		TEST_PROFILER("Clear");
		listVec100.Clear();
	}
	LOG("");


	LOG("Size: 1000");
	LOG("---------------------------------------------");

	mtk::List<Vector3> listVec1000;
	{
		TEST_PROFILER("Add");
		for (float i = 0; i < 500; i++)
			listVec1000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (float i = 0; i < 500; i++)
			listVec1000.Emplace(i * 2 + 1000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listVec1000.Contains(1000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listVec1000.Contains(1001);
	}
	{
		TEST_PROFILER("Remove");
		listVec1000.Remove(1000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listVec1000.RemoveAt(500);
	}
	{
		TEST_PROFILER("Clear");
		listVec1000.Clear();
	}
	LOG("");


	LOG("Size: 10000");
	LOG("---------------------------------------------");

	mtk::List<Vector3> listVec10000;
	{
		TEST_PROFILER("Add");
		for (float i = 0; i < 5000; i++)
			listVec10000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (float i = 0; i < 5000; i++)
			listVec10000.Emplace(i * 2 + 10000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listVec10000.Contains(10000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listVec10000.Contains(10001);
	}
	{
		TEST_PROFILER("Remove");
		listVec10000.Remove(10000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listVec10000.RemoveAt(5000);
	}
	{
		TEST_PROFILER("Clear");
		listVec10000.Clear();
	}
	LOG("");


	LOG("Size: 100000");
	LOG("---------------------------------------------");

	mtk::List<Vector3> listVec100000;
	{
		TEST_PROFILER("Add");
		for (float i = 0; i < 50000; i++)
			listVec100000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (float i = 0; i < 50000; i++)
			listVec100000.Emplace(i * 2 + 100000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listVec100000.Contains(100000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listVec100000.Contains(100001);
	}
	{
		TEST_PROFILER("Remove");
		listVec100000.Remove(100000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listVec100000.RemoveAt(50000);
	}
	{
		TEST_PROFILER("Clear");
		listVec100000.Clear();
	}
	LOG("");


	LOG("Size: 1000000");
	LOG("---------------------------------------------");

	mtk::List<Vector3> listVec1000000;
	{
		TEST_PROFILER("Add");
		for (float i = 0; i < 500000; i++)
			listVec1000000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (float i = 0; i < 500000; i++)
			listVec1000000.Emplace(i * 2 + 1000000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listVec1000000.Contains(1000000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listVec1000000.Contains(1000001);
	}
	{
		TEST_PROFILER("Remove");
		listVec1000000.Remove(1000000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listVec1000000.RemoveAt(500000);
	}
	{
		TEST_PROFILER("Clear");
		listVec1000000.Clear();
	}
	LOG("");

#endif // BENCHMARK_LIST_VECTOR3

	LOG("\n\n");

#ifdef BENCHMARK_LIST_PLAYER
	LOG("ModernDT List Benchmarks (Player)");
	LOG("----------------------------------\n");

	LOG("Size: 100");
	LOG("---------------------------------------------");

	mtk::List<Player> listPlay100;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 50; i++)
			listPlay100.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 50; i++)
			listPlay100.Emplace(i * 2 + 100);
	}
	{
		TEST_PROFILER("Contains (True)");
		listPlay100.Contains(100);
	}
	{
		TEST_PROFILER("Contains (False)");
		listPlay100.Contains(101);
	}
	{
		TEST_PROFILER("Remove");
		listPlay100.Remove(100);
	}
	{
		TEST_PROFILER("RemoveAt");
		listPlay100.RemoveAt(50);
	}
	{
		TEST_PROFILER("Clear");
		listPlay100.Clear();
	}
	LOG("");


	LOG("Size: 1000");
	LOG("---------------------------------------------");

	mtk::List<Player> listPlay1000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 500; i++)
			listPlay1000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 500; i++)
			listPlay1000.Emplace(i * 2 + 1000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listPlay1000.Contains(1000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listPlay1000.Contains(1001);
	}
	{
		TEST_PROFILER("Remove");
		listPlay1000.Remove(1000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listPlay1000.RemoveAt(500);
	}
	{
		TEST_PROFILER("Clear");
		listPlay1000.Clear();
	}
	LOG("");


	LOG("Size: 10000");
	LOG("---------------------------------------------");

	mtk::List<Player> listPlay10000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 5000; i++)
			listPlay10000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 5000; i++)
			listPlay10000.Emplace(i * 2 + 10000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listPlay10000.Contains(10000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listPlay10000.Contains(10001);
	}
	{
		TEST_PROFILER("Remove");
		listPlay10000.Remove(10000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listPlay10000.RemoveAt(5000);
	}
	{
		TEST_PROFILER("Clear");
		listPlay10000.Clear();
	}
	LOG("");


	LOG("Size: 100000");
	LOG("---------------------------------------------");

	mtk::List<Player> listPlay100000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 50000; i++)
			listPlay100000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 50000; i++)
			listPlay100000.Emplace(i * 2 + 100000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listPlay100000.Contains(100000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listPlay100000.Contains(100001);
	}
	{
		TEST_PROFILER("Remove");
		listPlay100000.Remove(100000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listPlay100000.RemoveAt(50000);
	}
	{
		TEST_PROFILER("Clear");
		listPlay100000.Clear();
	}
	LOG("");


	LOG("Size: 1000000");
	LOG("---------------------------------------------");

	mtk::List<Player> listPlay1000000;
	{
		TEST_PROFILER("Add");
		for (auto i = 0; i < 500000; i++)
			listPlay1000000.Add(i * 2);
	}
	{
		TEST_PROFILER("Emplace");
		for (auto i = 0; i < 500000; i++)
			listPlay1000000.Emplace(i * 2 + 1000000);
	}
	{
		TEST_PROFILER("Contains (True)");
		listPlay1000000.Contains(1000000);
	}
	{
		TEST_PROFILER("Contains (False)");
		listPlay1000000.Contains(1000001);
	}
	{
		TEST_PROFILER("Remove");
		listPlay1000000.Remove(1000000);
	}
	{
		TEST_PROFILER("RemoveAt");
		listPlay1000000.RemoveAt(500000);
	}
	{
		TEST_PROFILER("Clear");
		listPlay1000000.Clear();
	}
	LOG("");

#endif // BENCHMARK_LIST_PLAYER


//std::cin.get();
}