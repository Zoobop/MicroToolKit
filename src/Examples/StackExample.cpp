#include "mtkpch.h"
#include "Example.h"

#include "Structures/Stack.h"

#define BENCHMARK_STACK_INT
#define BENCHMARK_STACK_VECTOR3

int main()
{
#ifdef BENCHMARK_STACK_INT
	LOG("ModernDT Stack Benchmarks (int)");
	LOG("-------------------------------\n");

	mtk::Stack<int> dummyIntStack;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Push(1)");
	dummyIntStack.Push(1);
	LOG(dummyIntStack);
	LOG("");

	LOG("PushRange({10, 20, 30, 40, 50, 60, 70, 80, 90, 100})");
	dummyIntStack.PushRange({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
	LOG(dummyIntStack);
	LOG("");

	LOG("RSort()");
	mtk::RSort(dummyIntStack);
	LOG(dummyIntStack);
	LOG("");

	LOG("Sort()");
	mtk::Sort(dummyIntStack);
	LOG(dummyIntStack);
	LOG("");

	LOG("Pop()");
	LOG(dummyIntStack.Pop());
	LOG("");

	LOG("Peek()");
	LOG(dummyIntStack.Peek());
	LOG("");

	LOG("Contains(40)");
	resultInt = dummyIntStack.Contains(40);
	if (resultInt) LOG("Found 40\n");

	LOG("Contains(49)");
	resultInt = dummyIntStack.Contains(49);
	if (!resultInt) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyIntStack.Clear();
	LOG(dummyIntStack);
	LOG("");



	LOG("\nStack: 100");
	LOG("---------------------------------------------");

	mtk::Stack<int> stackInt100;

	Benchmark::Start("Push(100 times...)");
	for (auto i = 0; i < 100; i++)
		stackInt100.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackInt100);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackInt100);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackInt100.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackInt100.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains(40)");
	stackInt100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains(49)");
	stackInt100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackInt100.Clear();
	Benchmark::Stop();



	LOG("\nStack: 1000");
	LOG("---------------------------------------------");

	mtk::Stack<int> stackInt1000;

	Benchmark::Start("Push(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		stackInt1000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackInt1000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackInt1000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackInt1000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackInt1000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains(400)");
	stackInt1000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains(490)");
	stackInt1000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackInt1000.Clear();
	Benchmark::Stop();



	LOG("\nStack: 10000");
	LOG("---------------------------------------------");

	mtk::Stack<int> stackInt10000;

	Benchmark::Start("Push(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		stackInt10000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackInt10000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackInt10000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackInt10000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackInt10000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains(4000)");
	stackInt10000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4900)");
	stackInt10000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackInt10000.Clear();
	Benchmark::Stop();



	LOG("\nStack: 100000");
	LOG("---------------------------------------------");

	mtk::Stack<int> stackInt100000;

	Benchmark::Start("Push(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		stackInt100000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackInt100000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackInt100000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackInt100000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackInt100000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains(40000)");
	stackInt100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains(49000)");
	stackInt100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackInt100000.Clear();
	Benchmark::Stop();
#endif

	LOG("\n\n");

#ifdef BENCHMARK_STACK_VECTOR3
	LOG("ModernDT Stack Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mtk::Stack<Vector3> dummyVector3Stack;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("Push(10 times...)");
	for (int i = 0; i < 10; i++)
		dummyVector3Stack.Push(1);
	LOG(dummyVector3Stack);
	LOG("");

	LOG("RSort()");
	mtk::RSort(dummyVector3Stack);
	LOG(dummyVector3Stack);
	LOG("");

	LOG("Sort()");
	mtk::Sort(dummyVector3Stack);
	LOG(dummyVector3Stack);
	LOG("");

	LOG("Pop()");
	LOG(dummyVector3Stack.Pop());
	LOG(dummyVector3Stack);
	LOG("");

	LOG("Peek()");
	LOG(dummyVector3Stack.Peek());
	LOG(dummyVector3Stack);
	LOG("");

	LOG("Contains({40.0, 40.0, 40.0})");
	resultVec3 = dummyVector3Stack.Contains(40);
	if (resultVec3) LOG("Found 40\n");

	LOG("Contains({49.0, 49.0, 49.0})");
	resultVec3 = dummyVector3Stack.Contains(49);
	if (!resultVec3) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyVector3Stack.Clear();
	LOG(dummyVector3Stack);
	LOG("");


	LOG("\nStack: 100");
	LOG("---------------------------------------------");

	mtk::Stack<Vector3> stackVec3100;

	Benchmark::Start("Push(100 times...)");
	for (auto i = 0; i < 100; i++)
		stackVec3100.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackVec3100);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackVec3100);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackVec3100.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackVec3100.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains({40.0, 40.0, 40.0})");
	stackVec3100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains({49.0, 49.0, 49.0})");
	stackVec3100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackVec3100.Clear();
	Benchmark::Stop();



	LOG("\nStack: 1000");
	LOG("---------------------------------------------");

	mtk::Stack<Vector3> stackVec31000;

	Benchmark::Start("Push(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		stackVec31000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackVec31000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackVec31000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackVec31000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackVec31000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains({400.0, 400.0, 400.0})");
	stackVec31000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains({490.0, 490.0, 490.0})");
	stackVec31000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackVec31000.Clear();
	Benchmark::Stop();



	LOG("\nStack: 10000");
	LOG("---------------------------------------------");

	mtk::Stack<Vector3> stackVec310000;

	Benchmark::Start("Push(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		stackVec310000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackVec310000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackVec310000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackVec310000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackVec310000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains({4000.0, 4000.0, 4000.0})");
	stackVec310000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains({4900.0, 4900.0, 4900.0})");
	stackVec310000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackVec310000.Clear();
	Benchmark::Stop();


	LOG("\nStack: 100000");
	LOG("---------------------------------------------");

	mtk::Stack<Vector3> stackVec3100000;

	Benchmark::Start("Push(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		stackVec3100000.Push(i * 2);
	Benchmark::Stop();

	Benchmark::Start("RSort()");
	mtk::RSort(stackVec3100000);
	Benchmark::Stop();

	Benchmark::Start("Sort()");
	mtk::Sort(stackVec3100000);
	Benchmark::Stop();

	Benchmark::Start("Pop()");
	stackVec3100000.Pop();
	Benchmark::Stop();

	Benchmark::Start("Peek()");
	stackVec3100000.Peek();
	Benchmark::Stop();

	Benchmark::Start("Contains({40000.0, 40000.0, 40000.0})");
	stackVec3100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains({49000.0, 49000.0, 49000.0})");
	stackVec3100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	stackVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}