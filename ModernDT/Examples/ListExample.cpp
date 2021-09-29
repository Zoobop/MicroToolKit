#include "mdtpch.h"
#include "Example.h"

#include "Structures/List.h"

#define BENCHMARK_LIST_INT
#define BENCHMARK_LIST_VECTOR3

int main()
{
#ifdef BENCHMARK_LIST_INT
	LOG("ModernDT List Benchmarks (int)");
	LOG("------------------------------\n");

	mdt::List<int> dummyIntList;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Add(10)");
	dummyIntList.Add(10);
	LOG(dummyIntList);
	LOG("");

	LOG("AddRange({ 10, 20, 30, 40, 50 })");
	dummyIntList.AddRange({ 10, 20, 30, 40, 50 });
	LOG(dummyIntList);
	LOG("");

	LOG("RSort()");
	dummyIntList.RSort();
	LOG(dummyIntList);
	LOG("");

	LOG("Sort()");
	dummyIntList.Sort();
	LOG(dummyIntList);
	LOG("");

	LOG("Remove(10)");
	dummyIntList.Remove(10);
	LOG(dummyIntList);
	LOG("");

	LOG("RemoveAt(0)");
	dummyIntList.RemoveAt(0);
	LOG(dummyIntList);
	LOG("");

	LOG("Contains(40)");
	resultInt = dummyIntList.Contains(40);
	if (resultInt) LOG("Found 40\n");

	LOG("Contains(49)");
	resultInt = dummyIntList.Contains(49);
	if (!resultInt) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyIntList.Clear();
	LOG(dummyIntList);
	LOG("");



	LOG("\nList: 100");
	LOG("---------------------------------------------");

	mdt::List<int> listInt100;

	Benchmark::Start("Add(100 times...)");
	for (auto i = 0; i < 100; i++)
		listInt100.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listInt100.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listInt100.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(50)");
	listInt100.Remove(50);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50)");
	listInt100.RemoveAt(50);
	Benchmark::Stop();

	Benchmark::Start("Contains(40)");
	listInt100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains(49)");
	listInt100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listInt100.Clear();
	Benchmark::Stop();



	LOG("\nList: 1000");
	LOG("---------------------------------------------");

	mdt::List<int> listInt1000;

	Benchmark::Start("Add(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		listInt1000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listInt1000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listInt1000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(500)");
	listInt1000.Remove(500);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(500)");
	listInt1000.RemoveAt(500);
	Benchmark::Stop();

	Benchmark::Start("Contains(400)");
	listInt1000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains(490)");
	listInt1000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listInt1000.Clear();
	Benchmark::Stop();



	LOG("\nList: 10000");
	LOG("---------------------------------------------");

	mdt::List<int> listInt10000;

	Benchmark::Start("Add(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		listInt10000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listInt10000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listInt10000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(5000)");
	listInt10000.Remove(5000);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(5000)");
	listInt10000.RemoveAt(5000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4000)");
	listInt10000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4900)");
	listInt10000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listInt10000.Clear();
	Benchmark::Stop();



	LOG("\nList: 100000");
	LOG("---------------------------------------------");

	mdt::List<int> listInt100000;

	Benchmark::Start("Add(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		listInt100000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listInt100000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listInt100000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(50000)");
	listInt100000.Remove(listInt100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50000)");
	listInt100000.RemoveAt(listInt100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains(40000)");
	listInt100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains(49000)");
	listInt100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listInt100000.Clear();
	Benchmark::Stop();
#endif

#ifdef BENCHMARK_LIST_VECTOR3
	LOG("ModernDT List Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mdt::List<Vector3> dummyVector3List;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("Add({1.0, 1.0, 1.0})");
	dummyVector3List.Add(1);
	LOG(dummyVector3List);
	LOG("");

	LOG("AddRange({ 10, 20, 30, 40, 50 })");
	dummyVector3List.AddRange({ 10, 20, 30, 40, 50 });
	LOG(dummyVector3List);
	LOG("");

	LOG("RSort()");
	dummyVector3List.RSort();
	LOG(dummyVector3List);
	LOG("");

	LOG("Sort()");
	dummyVector3List.Sort();
	LOG(dummyVector3List);
	LOG("");

	LOG("Remove({10.0, 10.0, 10.0})");
	dummyVector3List.Remove(10);
	LOG(dummyVector3List);
	LOG("");

	LOG("RemoveAt(0)");
	dummyVector3List.RemoveAt(0);
	LOG(dummyVector3List);
	LOG("");

	LOG("Contains({40.0, 40.0, 40.0})");
	resultVec3 = dummyVector3List.Contains(40);
	if (resultVec3) LOG("Found 40\n");

	LOG("Contains({49.0, 49.0, 49.0})");
	resultVec3 = dummyVector3List.Contains(49);
	if (!resultVec3) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyVector3List.Clear();
	LOG(dummyVector3List);
	LOG("");


	LOG("\nList: 100");
	LOG("---------------------------------------------");

	mdt::List<Vector3> listVec3100;

	Benchmark::Start("Add(100 times...)");
	for (auto i = 0; i < 100; i++)
		listVec3100.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listVec3100.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listVec3100.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({50.0, 50.0, 50.0})");
	listVec3100.Remove(listVec3100.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50)");
	listVec3100.RemoveAt(listVec3100.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({40.0, 40.0, 40.0})");
	listVec3100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains({49.0, 49.0, 49.0})");
	listVec3100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listVec3100.Clear();
	Benchmark::Stop();



	LOG("\nList: 1000");
	LOG("---------------------------------------------");

	mdt::List<Vector3> listVec31000;

	Benchmark::Start("Add(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		listVec31000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listVec31000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listVec31000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({500.0, 500.0, 500.0})");
	listVec31000.Remove(listVec31000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(500)");
	listVec31000.RemoveAt(listVec31000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({400.0, 400.0, 400.0})");
	listVec31000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains({490.0, 490.0, 490.0})");
	listVec31000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listVec31000.Clear();
	Benchmark::Stop();



	LOG("\nList: 10000");
	LOG("---------------------------------------------");

	mdt::List<Vector3> listVec310000;

	Benchmark::Start("Add(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		listVec310000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listVec310000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listVec310000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({5000.0, 5000.0, 5000.0})");
	listVec310000.Remove(listVec310000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(5000)");
	listVec310000.RemoveAt(listVec310000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({4000.0, 4000.0, 4000.0})");
	listVec310000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains({4900.0, 4900.0, 4900.0})");
	listVec310000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listVec310000.Clear();
	Benchmark::Stop();


	LOG("\nList: 100000");
	LOG("---------------------------------------------");

	mdt::List<Vector3> listVec3100000;

	Benchmark::Start("Add(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		listVec3100000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	listVec3100000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	listVec3100000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({50000.0, 50000.0, 50000.0})");
	listVec3100000.Remove(listVec3100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50000)");
	listVec3100000.RemoveAt(listVec3100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({40000.0, 40000.0, 40000.0})");
	listVec3100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains({49000.0, 49000.0, 49000.0})");
	listVec3100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	listVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}