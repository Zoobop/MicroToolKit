#include "mtkpch.h"
#include "Example.h"

#include "Structures/Array.h"

#if 0
#define BENCHMARK_ARRAY_INT
#define BENCHMARK_ARRAY_VECTOR3
#define BENCHMARK_ARRAY_PLAYER
#endif

int main()
{
#ifdef BENCHMARK_ARRAY_INT
	LOG("ModernDT Array Benchmarks (int)");
	LOG("-------------------------------\n");

	mtk::Array<int, 10> dummyArray;
	bool result = false;

	LOG("Methods:\n");

	LOG("Fill(10)");
	dummyArray.Fill(10);
	LOG(dummyArray);
	LOG("");

	LOG("Set({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 })");
	dummyArray.Set({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
	LOG(dummyArray);
	LOG("");

	LOG("Swap(other array...)");
	Array<int, 10> otherArray = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	dummyArray.Swap(otherArray);
	LOG(dummyArray);
	LOG("");

	LOG("Contains(40)");
	result = dummyArray.Contains(40);
	if (result) LOG("Found 40\n");

	LOG("Contains(49)");
	result = dummyArray.Contains(49);
	if (!result) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyArray.Clear();
	LOG(dummyArray);
	LOG("");



	LOG("\nArray: 100");
	LOG("---------------------------------------------");

	mtk:Array<int, 100> array100;

	Benchmark::Start("operator[](100 times...)");
	for (auto i = 0; i < 100; i++)
		array100[i] = (i * 2);
	Benchmark::Stop();

	Benchmark::Start("Fill(10)");
	array100.Fill(10);
	Benchmark::Stop();

	Benchmark::Start("Contains(40)");
	array100.Contains(40);
	Benchmark::Stop();

	Benchmark::Start("Contains(49)");
	array100.Contains(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	array100.Clear();
	Benchmark::Stop();



	LOG("\nArray: 1000");
	LOG("---------------------------------------------");

	mtk::Array<int, 1000> array1000;

	Benchmark::Start("operator[](1000 times...)");
	for (auto i = 0; i < 1000; i++)
		array1000[i] = (i * 2);
	Benchmark::Stop();

	Benchmark::Start("Fill(10)");
	array1000.Fill(10);
	Benchmark::Stop();

	Benchmark::Start("Contains(400)");
	array1000.Contains(400);
	Benchmark::Stop();

	Benchmark::Start("Contains(490)");
	array1000.Contains(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	array1000.Clear();
	Benchmark::Stop();



	LOG("\nArray: 10000");
	LOG("---------------------------------------------");

	mtk::Array<int, 10000> array10000;

	Benchmark::Start("operator[](10000 times...)");
	for (auto i = 0; i < 10000; i++)
		array10000[i] = (i * 2);
	Benchmark::Stop();

	Benchmark::Start("Fill(10)");
	array10000.Fill(10);
	Benchmark::Stop();

	Benchmark::Start("Contains(4000)");
	array10000.Contains(4000);
	Benchmark::Stop();

	Benchmark::Start("Contains(4900)");
	array10000.Contains(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	array10000.Clear();
	Benchmark::Stop();



	LOG("\nArray: 100000");
	LOG("---------------------------------------------");

	mtk::Array<int, 100000> array100000;

	Benchmark::Start("operator[](100000 times...)");
	for (auto i = 0; i < 100000; i++)
		array100000[i] = (i * 2);
	Benchmark::Stop();

	Benchmark::Start("Fill(10)");
	array100000.Fill(10);
	Benchmark::Stop();

	Benchmark::Start("Contains(40000)");
	array100000.Contains(40000);
	Benchmark::Stop();

	Benchmark::Start("Contains(49000)");
	array100000.Contains(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	array100000.Clear();
	Benchmark::Stop();
#endif



	//std::cin.get();
}