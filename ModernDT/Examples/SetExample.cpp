#include "mtkpch.h"
#include "Example.h"

#include "Structures/Set.h"

#define BENCHMARK_SET_INT
#define BENCHMARK_SET_VECTOR3

int main()
{
#ifdef BENCHMARK_SET_INT
	LOG("ModernDT Set Benchmarks (int)");
	LOG("-------------------------------\n");

	mtk::Set<int> dummyIntSet;
	bool resultInt = false;

	LOG("Methods:\n");

	LOG("Reserve(50)");
	dummyIntSet.Reserve(50);
	LOG("");

	LOG("Insert(1)");
	dummyIntSet.Insert(1);
	LOG(dummyIntSet);
	LOG("");

	LOG("InsertRange({10, 20, 30, 40, 50, 60, 70, 80, 90, 100})");
	dummyIntSet.InsertRange({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
	LOG(dummyIntSet);
	LOG("");

	LOG("Erase(50)");
	dummyIntSet.Erase(50);
	LOG(dummyIntSet);
	LOG("");

	LOG("EraseKey(0)");
	dummyIntSet.EraseKey(0);
	LOG(dummyIntSet);
	LOG("");

	LOG("Find(40)");
	resultInt = dummyIntSet.Find(40);
	if (resultInt) LOG("Found 40\n");

	LOG("Find(49)");
	resultInt = dummyIntSet.Find(49);
	if (!resultInt) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyIntSet.Clear();
	LOG(dummyIntSet);
	LOG("");



	LOG("\nSet: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<int> setInt100;
	Benchmark::Stop();

	Benchmark::Start("Insert(100 times...)");
	for (auto i = 0; i < 100; i++)
		setInt100.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50)");
	setInt100.Erase(50);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(5)");
	setInt100.EraseKey(5);
	Benchmark::Stop();

	Benchmark::Start("Find(40)");
	setInt100.Find(40);
	Benchmark::Stop();

	Benchmark::Start("Find(49)");
	setInt100.Find(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setInt100.Clear();
	Benchmark::Stop();



	LOG("\nSet: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<int> setInt1000;
	Benchmark::Stop();

	Benchmark::Start("Insert(1000 times...)");
	for (auto i = 0; i < 1000; i++)
		setInt1000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(500)");
	setInt1000.Erase(500);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(50)");
	setInt1000.EraseKey(50);
	Benchmark::Stop();

	Benchmark::Start("Find(400)");
	setInt1000.Find(400);
	Benchmark::Stop();

	Benchmark::Start("Find(490)");
	setInt1000.Find(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setInt1000.Clear();
	Benchmark::Stop();



	LOG("\nSet: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<int> setInt10000;
	Benchmark::Stop();

	Benchmark::Start("Insert(10000 times...)");
	for (auto i = 0; i < 10000; i++)
		setInt10000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(5000)");
	setInt10000.Erase(5000);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(500)");
	setInt10000.EraseKey(500);
	Benchmark::Stop();

	Benchmark::Start("Find(4000)");
	setInt10000.Find(4000);
	Benchmark::Stop();

	Benchmark::Start("Find(4900)");
	setInt10000.Find(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setInt10000.Clear();
	Benchmark::Stop();



	LOG("\nSet: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<int> setInt100000;
	Benchmark::Stop();

	Benchmark::Start("Insert(100000 times...)");
	for (auto i = 0; i < 100000; i++)
		setInt100000.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50000)");
	setInt100000.Erase(50000);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(5000)");
	setInt100000.EraseKey(5000);
	Benchmark::Stop();

	Benchmark::Start("Find(40000)");
	setInt100000.Find(40000);
	Benchmark::Stop();

	Benchmark::Start("Find(49000)");
	setInt100000.Find(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setInt100000.Clear();
	Benchmark::Stop();
#endif

	LOG("\n\n");

#ifdef BENCHMARK_SET_VECTOR3
	LOG("ModernDT Set Benchmarks (Vector3)");
	LOG("------------------------------\n");

	mtk::Set<Vector3> dummyVector3Set;
	bool resultVec3 = false;

	LOG("Methods:\n");

	LOG("SetHash(hash function...)");
	dummyVector3Set.SetHash(MDTHash<Vector3>);
	LOG("");

	LOG("Reserve(50)");
	dummyVector3Set.Reserve(50);
	LOG("");

	LOG("Insert(10 times...)");
	for (float i = 0.0f; i < 10.0f; i++)
		dummyVector3Set.Insert(i);
	LOG(dummyVector3Set);
	LOG("");

	LOG("Erase(5)");
	dummyVector3Set.Erase(5);
	LOG(dummyVector3Set);
	LOG("");

	LOG("EraseKey(8)");
	dummyVector3Set.EraseKey(8);
	LOG(dummyVector3Set);
	LOG("");

	LOG("Find({6.0, 6.0, 6.0})");
	resultVec3 = dummyVector3Set.Find(6);
	if (resultVec3) LOG("Found 40\n");

	LOG("Find({49.0, 49.0, 49.0})");
	resultVec3 = dummyVector3Set.Find(49);
	if (!resultVec3) LOG("Couldn't find 49\n");

	LOG("Clear()");
	dummyVector3Set.Clear();
	LOG(dummyVector3Set);
	LOG("");


	LOG("\nSet: 100");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<Vector3> setVec3100(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(100)");
	setVec3100.Reserve(100);
	Benchmark::Stop();

	Benchmark::Start("Insert(100 times...)");
	for (float i = 0.0f; i < 100.0f; i++)
		setVec3100.Insert(i * 2);
	Benchmark::Stop();

	Benchmark::Start("Erase(50)");
	setVec3100.Erase(50);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(5)");
	setVec3100.EraseKey(5);
	Benchmark::Stop();

	Benchmark::Start("Find({40.0, 40.0, 40.0})");
	setVec3100.Find(40);
	Benchmark::Stop();

	Benchmark::Start("Find({49.0, 49.0, 49.0})");
	setVec3100.Find(49);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setVec3100.Clear();
	Benchmark::Stop();



	LOG("\nSet: 1000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<Vector3> setVec31000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(1000)");
	setVec31000.Reserve(1000);
	Benchmark::Stop();

	Benchmark::Start("Insert(1000 times...)");
	for (float i = 0.0f; i < 1000.0f; i++)
		setVec31000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(500)");
	setVec31000.Erase(500);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(50)");
	setVec31000.EraseKey(50);
	Benchmark::Stop();

	Benchmark::Start("Find({400.0, 400.0, 400.0})");
	setVec31000.Find(400);
	Benchmark::Stop();

	Benchmark::Start("Find({490.0, 490.0, 490.0})");
	setVec31000.Find(490);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setVec31000.Clear();
	Benchmark::Stop();



	LOG("\nSet: 10000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<Vector3> setVec310000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(10000)");
	setVec310000.Reserve(10000);
	Benchmark::Stop();

	Benchmark::Start("Insert(10000 times...)");
	for (float i = 0.0f; i < 10000.0f; i++)
		setVec310000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(5000)");
	setVec310000.Erase(5000);
	Benchmark::Stop();

	Benchmark::Start("EraseKey(500)");
	setVec310000.EraseKey(500);
	Benchmark::Stop();

	Benchmark::Start("Find({4000.0, 4000.0, 4000.0})");
	setVec310000.Find(4000);
	Benchmark::Stop();

	Benchmark::Start("Find({4900.0, 4900.0, 4900.0})");
	setVec310000.Find(4900);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setVec310000.Clear();
	Benchmark::Stop();


	LOG("\nSet: 100000");
	LOG("---------------------------------------------");

	Benchmark::Start("Construction");
	mtk::Set<Vector3> setVec3100000(MDTHash<Vector3>);
	Benchmark::Stop();

	Benchmark::Start("Reserve(100000)");
	setVec3100000.Reserve(100000);
	Benchmark::Stop();

	Benchmark::Start("Insert(100000 times...)");
	for (float i = 0.0f; i < 100000.0f; i++)
		setVec3100000.Insert(i * 2.0f);
	Benchmark::Stop();

	Benchmark::Start("Erase(50000)");
	setVec3100000.Erase(50000);
	Benchmark::Stop();

	Benchmark::Start("Erase(5000)");
	setVec3100000.EraseKey(5000);
	Benchmark::Stop();

	Benchmark::Start("Find({40000.0, 40000.0, 40000.0})");
	setVec3100000.Find(40000);
	Benchmark::Stop();

	Benchmark::Start("Find({49000.0, 49000.0, 49000.0})");
	setVec3100000.Find(49000);
	Benchmark::Stop();

	Benchmark::Start("Clear()");
	setVec3100000.Clear();
	Benchmark::Stop();

#endif
	//std::cin.get();
}