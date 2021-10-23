#include "mtkpch.h"
#include "Example.h"

#include "Structures/Map.h"

#define BENCHMARK_MAP_CHAR
#define BENCHMARK_MAP_VECTOR3

int main()
{
#ifdef BENCHMARK_MAP_CHAR
	LOG("ModernDT Map Benchmarks (char, int)");
	LOG("-------------------------------\n");

	mtk::Map<char, int> dummyCharMap;

	LOG("Methods:\n");

	LOG("Reserve(50)");
	dummyCharMap.Reserve(50);
	LOG("");

	LOG("Insert('z')");
	dummyCharMap.Insert('z', 1);
	LOG(dummyCharMap);

	LOG("InsertRange({ a, b, c, d, e, f, g, h, i, j })");
	dummyCharMap.InsertRange({ 97, 98, 99, 100, 101, 102, 103, 104, 105, 106 });
	LOG(dummyCharMap);

	LOG("Erase('d')");
	dummyCharMap.Erase(100);
	LOG(dummyCharMap);

	LOG("Find('a')");
	LOG(dummyCharMap.Find(97));
	LOG("");

	LOG("At('b')");
	LOG(dummyCharMap.At(98));
	LOG("");

	LOG("Clear()");
	dummyCharMap.Clear();
	LOG(dummyCharMap);



	LOG("\nMap: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<char, int> mapChar100;
	Benchmark::Stop();

	Benchmark::Start("Insert(100 times...)");
	for (auto i = 0; i < 100; i++)
		mapChar100.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50)");
	mapChar100.Erase(50);
	Benchmark::Stop();

	Benchmark::Start("Find(40)");
	mapChar100.Find(40);
	Benchmark::Stop();

	Benchmark::Start("At(40)");
	mapChar100.At(40);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapChar100.Clear();
	Benchmark::Stop();



	LOG("\nMap: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<char, int> mapChar1000;
	Benchmark::Stop();

	Benchmark::Start("Insert(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		mapChar1000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(500)");
	mapChar1000.Erase(500);
	Benchmark::Stop();

	Benchmark::Start("Find(400)");
	mapChar1000.Find(400);
	Benchmark::Stop();

	Benchmark::Start("At(400)");
	mapChar1000.At(400);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapChar1000.Clear();
	Benchmark::Stop();



	LOG("\nMap: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<char, int> mapChar10000;
	Benchmark::Stop();

	Benchmark::Start("Insert(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		mapChar10000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(5000)");
	mapChar10000.Erase(5000);
	Benchmark::Stop();

	Benchmark::Start("Find(4000)");
	mapChar10000.Find(4000);
	Benchmark::Stop();

	Benchmark::Start("At(4000)");
	mapChar10000.At(4000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapChar10000.Clear();
	Benchmark::Stop();



	LOG("\nMap: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<char, int> mapChar100000;
	Benchmark::Stop();

	Benchmark::Start("Insert(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		mapChar100000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50000)");
	mapChar100000.Erase(50000);
	Benchmark::Stop();

	Benchmark::Start("Find(40000)");
	mapChar100000.Find(40000);
	Benchmark::Stop();

	Benchmark::Start("At(40000)");
	mapChar100000.At(40000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapChar100000.Clear();
	Benchmark::Stop();
#endif

	LOG("\n\n");

#ifdef BENCHMARK_MAP_VECTOR3
	LOG("ModernDT Map Benchmarks (Vector3, int)");
	LOG("------------------------------\n");

	mtk::Map<Vector3, int> dummyVector3Map;

	LOG("Methods:\n");

	LOG("SetHash(hash function...)");
	dummyVector3Map.SetHash(MDTHash<Vector3>);	LOG("");

	LOG("Reserve(50)");
	dummyVector3Map.Reserve(50);
	LOG("");

	LOG("Insert({1.0, 3.5, 7.44})");
	dummyVector3Map.Insert({ 1.0, 3.5, 7.44 });
	LOG(dummyVector3Map);

	LOG("InsertRange({ 2.0, 3.0, 4.0 })");
	dummyVector3Map.InsertRange({ 2, 3, 4 });
	LOG(dummyVector3Map);

	LOG("Erase({3.0, 3.0, 3.0})");
	dummyVector3Map.Erase(3);
	LOG(dummyVector3Map);

	LOG("Find({4.0, 4.0, 4.0})");
	LOG(dummyVector3Map.Find(4));
	LOG("");

	LOG("At({2.0, 2.0, 2.0})");
	LOG(dummyVector3Map.At(2));
	LOG("");

	LOG("Clear()");
	dummyVector3Map.Clear();
	LOG(dummyVector3Map);


	LOG("\nMap: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<Vector3, int> mapVec3100(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(100)");
	mapVec3100.Reserve(100);
	Benchmark::Stop();

	Benchmark::Start("Insert(100 times...)");
	for (float i = 0.0f; i < 100.0f; i++)
		mapVec3100.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50)");
	mapVec3100.Erase(50);
	Benchmark::Stop();

	Benchmark::Start("Find({40.0, 40.0, 40.0})");
	mapVec3100.Find(40);
	Benchmark::Stop();

	Benchmark::Start("At({40.0, 40.0, 40.0})");
	mapVec3100.At(40);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapVec3100.Clear();
	Benchmark::Stop();



	LOG("\nMap: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<Vector3, int> mapVec31000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(1000)");
	mapVec31000.Reserve(1000);
	Benchmark::Stop();

	Benchmark::Start("Insert(1000 times...)");
	for (float i = 0.0f; i < 1000.0f; i++)
		mapVec31000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(500)");
	mapVec31000.Erase(500);
	Benchmark::Stop();

	Benchmark::Start("Find({400.0, 400.0, 400.0})");
	mapVec31000.Find(400);
	Benchmark::Stop();

	Benchmark::Start("At({400.0, 400.0, 400.0})");
	mapVec31000.At(400);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapVec31000.Clear();
	Benchmark::Stop();



	LOG("\nMap: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<Vector3, int> mapVec310000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(10000)");
	mapVec310000.Reserve(10000);
	Benchmark::Stop();

	Benchmark::Start("Insert(10000 times...)");
	for (float i = 0.0f; i < 10000.0f; i++)
		mapVec310000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(5000)");
	mapVec310000.Erase(5000);
	Benchmark::Stop();

	Benchmark::Start("Find({4000.0, 4000.0, 4000.0})");
	mapVec310000.Find(4000);
	Benchmark::Stop();

	Benchmark::Start("At({4000.0, 4000.0, 4000.0})");
	mapVec310000.Find(4000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapVec310000.Clear();
	Benchmark::Stop();


	LOG("\nMap: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Map<Vector3, int> mapVec3100000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(100000)");
	mapVec3100000.Reserve(100000);
	Benchmark::Stop();

	Benchmark::Start("Insert(100000 times...)");
	for (float i = 0.0f; i < 100000.0f; i++)
		mapVec3100000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(50000)");
	mapVec3100000.Erase(50000);
	Benchmark::Stop();

	Benchmark::Start("Find({40000.0, 40000.0, 40000.0})");
	mapVec3100000.Find(40000);
	Benchmark::Stop();

	Benchmark::Start("At({40000.0, 40000.0, 40000.0})");
	mapVec3100000.At(40000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	mapVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}