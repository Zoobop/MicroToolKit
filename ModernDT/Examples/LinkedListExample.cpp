#include "mdtpch.h"
#include "Example.h"

#include "Structures/LinkedList.h"

#define BENCHMARK_LINKEDLIST_INT
#define BENCHMARK_LINKEDLIST_VECTOR3

int main()
{
#ifdef BENCHMARK_LINKEDLIST_INT
	LOG("ModernDT LinkedList Benchmarks (int)");
	LOG("-------------------------------\n");

	mdt::LinkedList<int> dummyIntLinkedList;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Push(1)");
	dummyIntLinkedList.Push(1);
	LOG(dummyIntLinkedList);
	LOG("");

	LOG("PushRange({10, 20, 30, 40, 50, 60, 70, 80, 90, 100})");
	dummyIntLinkedList.PushRange({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
	LOG(dummyIntLinkedList);
	LOG("");

	LOG("Pop()");
	LOG(dummyIntLinkedList.Pop());
	LOG(dummyIntLinkedList);
	LOG("");

	LOG("Remove(50)");
	dummyIntLinkedList.Remove(50);
	LOG(dummyIntLinkedList);
	LOG("");

	LOG("Contains(40)");
	resultInt = dummyIntLinkedList.Contains(40);
	if (resultInt) LOG("Found 40\n");

	LOG("Contains(49)");
	resultInt = dummyIntLinkedList.Contains(49);
	if (!resultInt) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyIntLinkedList.Clear();
	LOG(dummyIntLinkedList);
	LOG("");



	LOG("\nLinkedList: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<int> linkedListInt100;
	Benchmark::Stop();

	Benchmark::Start("Push(100 times...)");
	for (auto i = 0; i < 100; i++)
		linkedListInt100.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListInt100.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(50)");
	linkedListInt100.Remove(50);
	Benchmark::Stop();

	Benchmark::Start("Contains(40)");
	linkedListInt100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains(49)");
	linkedListInt100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListInt100.Clear();
	Benchmark::Stop();



	LOG("\nLinkedList: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<int> linkedListInt1000;
	Benchmark::Stop();

	Benchmark::Start("Push(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		linkedListInt1000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListInt1000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(500)");
	linkedListInt1000.Remove(500);
	Benchmark::Stop();

	Benchmark::Start("Contains(400)");
	linkedListInt1000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains(490)");
	linkedListInt1000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListInt1000.Clear();
	Benchmark::Stop();



	LOG("\nLinkedList: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<int> linkedListInt10000;
	Benchmark::Stop();

	Benchmark::Start("Push(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		linkedListInt10000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListInt10000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(5000)");
	linkedListInt10000.Remove(5000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4000)");
	linkedListInt10000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4900)");
	linkedListInt10000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListInt10000.Clear();
	Benchmark::Stop();



	LOG("\nLinkedList: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<int> linkedListInt100000;
	Benchmark::Stop();

	Benchmark::Start("Push(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		linkedListInt100000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListInt100000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(50000)");
	linkedListInt100000.Remove(50000);
	Benchmark::Stop();

	Benchmark::Start("Contains(40000)");
	linkedListInt100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains(49000)");
	linkedListInt100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListInt100000.Clear();
	Benchmark::Stop();
#endif

	LOG("\n\n");

#ifdef BENCHMARK_LINKEDLIST_VECTOR3
	LOG("ModernDT LinkedList Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mdt::LinkedList<Vector3> dummyVector3LinkedList;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("Push(10 times...)");
	for (float i = 0.0f; i < 10.0f; i++)
		dummyVector3LinkedList.Push(i * 10.0f);
	LOG(dummyVector3LinkedList);
	LOG("");

	LOG("Pop()");
	LOG(dummyVector3LinkedList.Pop());
	LOG(dummyVector3LinkedList);
	LOG("");

	LOG("Remove(50)");
	LOG(dummyVector3LinkedList.Remove(50));
	LOG(dummyVector3LinkedList);
	LOG("");

	LOG("Contains({40.0, 40.0, 40.0})");
	resultVec3 = dummyVector3LinkedList.Contains(40);
	if (resultVec3) LOG("Found 40\n");

	LOG("Contains({49.0, 49.0, 49.0})");
	resultVec3 = dummyVector3LinkedList.Contains(49);
	if (!resultVec3) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyVector3LinkedList.Clear();
	LOG(dummyVector3LinkedList);
	LOG("");


	LOG("\nLinkedList: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<Vector3> linkedListVec3100;
	Benchmark::Stop();

	Benchmark::Start("Push(100 times...)");
	for (float i = 0.0f; i < 100.0f; i++)
		linkedListVec3100.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListVec3100.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(50)");
	linkedListVec3100.Remove(50);
	Benchmark::Stop();

	Benchmark::Start("Contains({40.0, 40.0, 40.0})");
	linkedListVec3100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains({49.0, 49.0, 49.0})");
	linkedListVec3100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListVec3100.Clear();
	Benchmark::Stop();



	LOG("\nLinkedList: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<Vector3> linkedListVec31000;
	Benchmark::Stop();

	Benchmark::Start("Push(1000 times...)");
	for (float i = 0.0f; i < 1000.0f; i++)
		linkedListVec31000.Push(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListVec31000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(500)");
	linkedListVec31000.Remove(500);
	Benchmark::Stop();

	Benchmark::Start("Contains({400.0, 400.0, 400.0})");
	linkedListVec31000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains({490.0, 490.0, 490.0})");
	linkedListVec31000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListVec31000.Clear();
	Benchmark::Stop();



	LOG("\nLinkedList: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<Vector3> linkedListVec310000;
	Benchmark::Stop();

	Benchmark::Start("Push(10000 times...)");
	for (float i = 0.0f; i < 10000.0f; i++)
		linkedListVec310000.Push(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListVec310000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(5000)");
	linkedListVec310000.Remove(5000);
	Benchmark::Stop();

	Benchmark::Start("Contains({4000.0, 4000.0, 4000.0})");
	linkedListVec310000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains({4900.0, 4900.0, 4900.0})");
	linkedListVec310000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListVec310000.Clear();
	Benchmark::Stop();


	LOG("\nLinkedList: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mdt::LinkedList<Vector3> linkedListVec3100000;
	Benchmark::Stop();

	Benchmark::Start("Push(100000 times...)");
	for (float i = 0.0f; i < 100000.0f; i++)
		linkedListVec3100000.Push(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	linkedListVec3100000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Remove(50000)");
	linkedListVec3100000.Remove(50000);
	Benchmark::Stop();

	Benchmark::Start("Contains({40000.0, 40000.0, 40000.0})");
	linkedListVec3100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains({49000.0, 49000.0, 49000.0})");
	linkedListVec3100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	linkedListVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}