#include "mtkpch.h"
#include "Example.h"

#include "Structures/Queue.h"

#define BENCHMARK_QUEUE_INT
#define BENCHMARK_QUEUE_VECTOR3

int main()
{
#ifdef BENCHMARK_QUEUE_INT
	LOG("ModernDT Queue Benchmarks (int)");
	LOG("-------------------------------\n");

	mtk::Queue<int> dummyIntQueue;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Enqueue(10 times...)");
	for (int i = 0; i < 10; i++)
		dummyIntQueue.Enqueue(i * 10);
	LOG(dummyIntQueue);
	LOG("");

	LOG("RSort()");
	mtk::RSort(dummyIntQueue);
	LOG(dummyIntQueue);
	LOG("");

	LOG("Sort()");
	mtk::Sort(dummyIntQueue);
	LOG(dummyIntQueue);
	LOG("");

	LOG("Dequeue()");
	LOG(dummyIntQueue.Dequeue());
	LOG("");

	LOG("Peek()");
	LOG(dummyIntQueue.Peek());
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

	mtk::Queue<int> queueInt100;

	Benchmark::Start("Enqueue(100 times...)");
	for (auto i = 0; i < 100; i++)
		queueInt100.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueInt100);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueInt100);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueInt100.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueInt100.Peek();
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

	mtk::Queue<int> queueInt1000;

	Benchmark::Start("Enqueue(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		queueInt1000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueInt1000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueInt1000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueInt1000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueInt1000.Peek();
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

	mtk::Queue<int> queueInt10000;

	Benchmark::Start("Enqueue(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		queueInt10000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueInt10000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueInt10000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueInt10000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueInt10000.Peek();
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

	mtk::Queue<int> queueInt100000;

	Benchmark::Start("Enqueue(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		queueInt100000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueInt100000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueInt100000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueInt100000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueInt100000.Peek();
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

	LOG("\n\n");

#ifdef BENCHMARK_QUEUE_VECTOR3
	LOG("ModernDT Queue Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mtk::Queue<Vector3> dummyVector3Queue;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("Enqueue(10 times...)");
	for (int i = 0; i < 10; i++)
		dummyVector3Queue.Enqueue(1);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("RSort()");
	mtk::RSort(dummyVector3Queue);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Sort()");
	mtk::Sort(dummyVector3Queue);
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Dequeue()");
	LOG(dummyVector3Queue.Dequeue());
	LOG(dummyVector3Queue);
	LOG("");

	LOG("Peek()");
	LOG(dummyVector3Queue.Peek());
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

	mtk::Queue<Vector3> queueVec3100;

	Benchmark::Start("Enqueue(100 times...)");
	for (auto i = 0; i < 100; i++)
		queueVec3100.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueVec3100);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueVec3100);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueVec3100.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueVec3100.Peek();
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

	mtk::Queue<Vector3> queueVec31000;

	Benchmark::Start("Enqueue(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		queueVec31000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueVec31000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueVec31000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueVec31000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueVec31000.Peek();
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

	mtk::Queue<Vector3> queueVec310000;

	Benchmark::Start("Enqueue(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		queueVec310000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueVec310000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueVec310000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueVec310000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueVec310000.Peek();
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

	mtk::Queue<Vector3> queueVec3100000;

	Benchmark::Start("Enqueue(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		queueVec3100000.Enqueue(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(queueVec3100000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(queueVec3100000);
	Benchmark::Stop();

	Benchmark::Start("Dequeue()");
	queueVec3100000.Dequeue();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	queueVec3100000.Peek();
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