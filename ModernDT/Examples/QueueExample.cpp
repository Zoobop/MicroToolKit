#include "mdtpch.h"
#include "Example.h"

#include "Structures/Queue.h"

//#define BENCHMARK_QUEUE_INT
//#define BENCHMARK_QUEUE_VECTOR3

int main()
{
#ifdef BENCHMARK_QUEUE_INT
	LOG("ModernDT Queue Benchmarks (int)");
	LOG("-------------------------------\n");

	mdt::Queue<int> dummyIntQueue;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Add(10)");
	dummyIntQueue.Add(10);
	LOG(dummyIntQueue);
	LOG("");

	LOG("AddRange({ 10, 20, 30, 40, 50 })");
	dummyIntQueue.AddRange({ 10, 20, 30, 40, 50 });
	LOG(dummyIntQueue);
	LOG("");

	LOG("RSort()");
	dummyIntQueue.RSort();
	LOG(dummyIntQueue);
	LOG("");

	LOG("Sort()");
	dummyIntQueue.Sort();
	LOG(dummyIntQueue);
	LOG("");

	LOG("Remove(10)");
	dummyIntQueue.Remove(10);
	LOG(dummyIntQueue);
	LOG("");

	LOG("RemoveAt(0)");
	dummyIntQueue.RemoveAt(0);
	LOG(dummyIntQueue);
	LOG("");

	LOG("Contains(40)");
	resultInt = dummyIntQueue.Contains(40);
	if (resultInt) LOG("Found 40\n");

	LOG("Contains(49)");
	resultInt = dummyIntQueue.Contains(49);
	if (!resultInt) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyIntQueue.Clear();
	LOG(dummyIntQueue);
	LOG("");



	LOG("\nQueue: 100");
	LOG("---------------------------------------------");

	mdt::Queue<int> queueInt100;

	Benchmark::Start("Add(100 times...)");
	for (auto i = 0; i < 100; i++)
		queueInt100.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueInt100.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueInt100.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(50)");
	queueInt100.Remove(50);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50)");
	queueInt100.RemoveAt(50);
	Benchmark::Stop();

	Benchmark::Start("Contains(40)");
	queueInt100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains(49)");
	queueInt100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueInt100.Clear();
	Benchmark::Stop();



	LOG("\nQueue: 1000");
	LOG("---------------------------------------------");

	mdt::Queue<int> queueInt1000;

	Benchmark::Start("Add(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		queueInt1000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueInt1000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueInt1000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(500)");
	queueInt1000.Remove(500);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(500)");
	queueInt1000.RemoveAt(500);
	Benchmark::Stop();

	Benchmark::Start("Contains(400)");
	queueInt1000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains(490)");
	queueInt1000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueInt1000.Clear();
	Benchmark::Stop();



	LOG("\nQueue: 10000");
	LOG("---------------------------------------------");

	mdt::Queue<int> queueInt10000;

	Benchmark::Start("Add(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		queueInt10000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueInt10000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueInt10000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(5000)");
	queueInt10000.Remove(5000);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(5000)");
	queueInt10000.RemoveAt(5000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4000)");
	queueInt10000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4900)");
	queueInt10000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueInt10000.Clear();
	Benchmark::Stop();



	LOG("\nQueue: 100000");
	LOG("---------------------------------------------");

	mdt::Queue<int> queueInt100000;

	Benchmark::Start("Add(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		queueInt100000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueInt100000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueInt100000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove(50000)");
	queueInt100000.Remove(queueInt100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50000)");
	queueInt100000.RemoveAt(queueInt100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains(40000)");
	queueInt100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains(49000)");
	queueInt100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueInt100000.Clear();
	Benchmark::Stop();
#endif

#ifdef BENCHMARK_QUEUE_VECTOR3
	LOG("ModernDT Queue Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mdt::Queue<Vector3> dummyVector3Queue;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("Add({1.0, 1.0, 1.0})");
	dummyVector3Queue.Add(1);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("AddRange({ 10, 20, 30, 40, 50 })");
	dummyVector3Queue.AddRange({ 10, 20, 30, 40, 50 });
	LOG(dummyVector3Queue);
	LOG("");

	LOG("RSort()");
	dummyVector3Queue.RSort();
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Sort()");
	dummyVector3Queue.Sort();
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Remove({10.0, 10.0, 10.0})");
	dummyVector3Queue.Remove(10);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("RemoveAt(0)");
	dummyVector3Queue.RemoveAt(0);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Contains({40.0, 40.0, 40.0})");
	resultVec3 = dummyVector3Queue.Contains(40);
	if (resultVec3) LOG("Found 40\n");

	LOG("Contains({49.0, 49.0, 49.0})");
	resultVec3 = dummyVector3Queue.Contains(49);
	if (!resultVec3) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyVector3Queue.Clear();
	LOG(dummyVector3Queue);
	LOG("");


	LOG("\nQueue: 100");
	LOG("---------------------------------------------");

	mdt::Queue<Vector3> queueVec3100;

	Benchmark::Start("Add(100 times...)");
	for (auto i = 0; i < 100; i++)
		queueVec3100.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueVec3100.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueVec3100.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({50.0, 50.0, 50.0})");
	queueVec3100.Remove(queueVec3100.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50)");
	queueVec3100.RemoveAt(queueVec3100.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({40.0, 40.0, 40.0})");
	queueVec3100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains({49.0, 49.0, 49.0})");
	queueVec3100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueVec3100.Clear();
	Benchmark::Stop();



	LOG("\nQueue: 1000");
	LOG("---------------------------------------------");

	mdt::Queue<Vector3> queueVec31000;

	Benchmark::Start("Add(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		queueVec31000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueVec31000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueVec31000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({500.0, 500.0, 500.0})");
	queueVec31000.Remove(queueVec31000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(500)");
	queueVec31000.RemoveAt(queueVec31000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({400.0, 400.0, 400.0})");
	queueVec31000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains({490.0, 490.0, 490.0})");
	queueVec31000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueVec31000.Clear();
	Benchmark::Stop();



	LOG("\nQueue: 10000");
	LOG("---------------------------------------------");

	mdt::Queue<Vector3> queueVec310000;

	Benchmark::Start("Add(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		queueVec310000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueVec310000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueVec310000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({5000.0, 5000.0, 5000.0})");
	queueVec310000.Remove(queueVec310000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(5000)");
	queueVec310000.RemoveAt(queueVec310000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({4000.0, 4000.0, 4000.0})");
	queueVec310000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains({4900.0, 4900.0, 4900.0})");
	queueVec310000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueVec310000.Clear();
	Benchmark::Stop();


	LOG("\nQueue: 100000");
	LOG("---------------------------------------------");

	mdt::Queue<Vector3> queueVec3100000;

	Benchmark::Start("Add(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		queueVec3100000.Add(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	queueVec3100000.RSort();
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	queueVec3100000.Sort();
	Benchmark::Stop();

	Benchmark::Start("Remove({50000.0, 50000.0, 50000.0})");
	queueVec3100000.Remove(queueVec3100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("RemoveAt(50000)");
	queueVec3100000.RemoveAt(queueVec3100000.Size() / 2);
	Benchmark::Stop();

	Benchmark::Start("Contains({40000.0, 40000.0, 40000.0})");
	queueVec3100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains({49000.0, 49000.0, 49000.0})");
	queueVec3100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	queueVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}