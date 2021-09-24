#include "mdtpch.h"
#include "ModernDT.h"
#include "Interfaces/IHashable.h"

#include <array>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

//#define BENCHMARK_PRIMATIVE
//#define BENCHMARK_PLAYER

struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vector3()
	{
	}

	Vector3(const float& _scalar)
		: x(_scalar), y(_scalar), z(_scalar)
	{
	}

	Vector3(const float& _x, const float& _y, const float& _z)
		: x(_x), y(_y), z(_z)
	{
	}

	Vector3(const Vector3& _other)
		: x(_other.x), y(_other.y), z(_other.z)
	{
#ifdef DEBUG
		LOG("Copyed! Vec");
#endif
	}

	Vector3(Vector3&& _other) noexcept
		: x(_other.x), y(_other.y), z(_other.z)
	{
#ifdef DEBUG
		LOG("Moved! Vec");
#endif
	}

	~Vector3()
	{
#ifdef DEBUG
		LOG("Destroyed! Vec");
#endif
	}

	Vector3& operator=(Vector3&& _other) noexcept
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

#ifdef DEBUG
		LOG("Moved! Vec");
#endif

		return *this;
	}

	Vector3& operator=(const Vector3& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

#ifdef DEBUG
		LOG("Copyed! Vec");
#endif

		return *this;
	}

	bool operator==(const Vector3& _other)
	{
		return x == _other.x && y == _other.y && z == _other.z;
	}

	friend std::ostream& operator<<(std::ostream& _stream, const Vector3& _current)
	{
		_stream << "[ " << _current.x << ", " << _current.y << ", " << _current.z << " ]";
		return _stream;
	}
};

class Player : public IHashable<>
{
public:
	enum Class
	{
		Warrior = 0,
		Mage,
		Assassin,
		Guardian,
		Hunter
	};

public:
	Player()
		: m_Name("New Player"), m_Level(0), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name)
		: m_Name(_name), m_Level(1), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, uint32_t _level)
		: m_Name(_name), m_Level(_level), m_Class(Warrior), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, Class _class)
		: m_Name(_name), m_Level(1), m_Class(_class), m_Position(0, 0, 0)
	{
	}

	Player(const char* _name, uint32_t _level, Class _class)
		: m_Name(_name), m_Level(_level), m_Class(_class), m_Position(0, 0, 0)
	{
	}

	Player(const Player& _other)
		: m_Name(_other.m_Name), m_Level(_other.m_Level), m_Class(_other.m_Class), m_Position(_other.m_Position)
	{
#ifdef DEBUG
		LOG("Copyed! Play");
#endif
	}

	Player(Player&& _other) noexcept
		: m_Name(_other.m_Name), m_Level(_other.m_Level), m_Class(_other.m_Class), m_Position(std::move(_other.m_Position))
	{
#ifdef DEBUG
		LOG("Moved! Play");
#endif
	}

	~Player()
	{
#ifdef DEBUG
		LOG("Destroyed! Play");
#endif
	}

	inline const char* GetName() const { return m_Name; }
	inline uint32_t GetLevel() const { return m_Level; }
	inline Class GetClass() const { return m_Class; }
	inline Vector3 GetPosition() const { return m_Position; }

	newhash_t GetHash() const override
	{
		newhash_t size = (newhash_t)(sizeof(Player) + sizeof(newhash_t)) + m_Level;
		newhash_t factor = (newhash_t)((float)size - (float)m_Class + ((float)size * 0.5f));
		return factor;
	}

	Player& operator=(const Player& _other)
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = _other.m_Position;
#ifdef DEBUG
		LOG("Copyed! Play");
#endif
		return *this;
	}

	Player& operator=(Player&& _other) noexcept
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = std::move(_other.m_Position);
#ifdef DEBUG
		LOG("Moved! Play");
#endif

		return *this;
	}

	bool operator==(const Player& _other)
	{
		return m_Level == _other.m_Level && m_Class == _other.m_Class && m_Position == _other.m_Position;
	}

	friend std::ostream& operator<<(std::ostream& _stream, const Player& _current)
	{
		_stream << _current.m_Name << " [ Class: " << _current.m_Class << " Level: " << _current.m_Level << " ]";
		return _stream;
	}

protected:
	const char* m_Name;
	uint32_t m_Level;
	Class m_Class;
	Vector3 m_Position;
};

#ifdef BENCHMARK_PRIMATIVE or BENCHMARK_PLAYER
template<typename T, size_t _Size>
void Function(std::array<T, _Size>& _array)
{
	for (size_t count = 0; count < 1000; count++) {
		for (size_t i = 0; i < _array.size(); i++) {
			_array[i] = T();
		}
	}
}

template<typename T>
void Function(std::list<T>& _list)
{
	for (int i = 0; i < 1000000; i++) {
		_list.emplace_back(T());
	}
}

template<typename T>
void Function(std::vector<T>& _vector)
{
	for (size_t i = 0; i < 1000000; i++) {
		_vector.emplace_back(T());
	}
}

template<typename T>
void Function(std::unordered_set<T>& _set)
{
	for (int i = 0; i < 1000000; i++) {
		_set.insert(T());
	}
}

template<typename T1, typename T2>
void Function(std::unordered_map<T1, T2>& _map)
{
	for (int i = 0; i < 1000000; i++) {
		_map.insert({ T1(), T2() });
	}
}

template<typename T>
void Function(std::stack<T>& _stack)
{
	for (int i = 0; i < 1000000; i++) {
		_stack.push(T());
	}
}

template<typename T>
void Function(std::queue<T>& _queue)
{
	for (int i = 0; i < 1000000; i++) {
		_queue.push(T());
	}
}

// New API

template<typename T, size_t _Size>
void Function(Array<T, _Size>& _array)
{
	for (size_t count = 0; count < 1000; count++) {
		for (size_t i = 0; i < _array.Capacity(); i++) {
			_array[i] = T();
		}
	}
}

template<typename T>
void Function(List<T>& _list)
{
	for (int i = 0; i < 1000000; i++) {
		_list.Emplace(T());
	}
}

template<typename T>
void Function(Set<T>& _set)
{
	for (int i = 0; i < 1000000; i++) {
		_set.Insert(T());
	}
}

template<typename T1, typename T2>
void Function(Map<T1, T2>& _map)
{
	for (int i = 0; i < 1000000; i++) {
		_map.Insert(T1(), T2());
	}
}

template<typename T>
void Function(Stack<T>& _stack)
{
	for (int i = 0; i < 1000000; i++) {
		_stack.Push(T());
	}
}

template<typename T>
void Function(Queue<T>& _queue)
{
	for (int i = 0; i < 1000000; i++) {
		_queue.Enqueue(T());
	}
}

template<typename T>
void PrintVector(std::vector<T> _vector)
{
	for (const auto& item : _vector)
		LOG(item);
}
// std

#endif

newhash_t TestHash(const Player& _obj, size_t _capacity)
{
	newhash_t size = (newhash_t)(sizeof(Player) + sizeof(newhash_t)) + _obj.GetLevel();
	newhash_t factor = (newhash_t)((float)size - (float)_obj.GetClass() + ((float)size * 0.5f));
	return factor % _capacity;
}

int main()
{

#ifdef BENCHMARK_PLAYER

	LOG("Benchmark with Player class:\n");

	List<Player> List;
	std::list<Player> list;
	std::vector<Player> vector;

	Benchmark::Start("List");
	Function(List);
	Benchmark::Stop();

	Benchmark::Start("std::list");
	Function(list);
	Benchmark::Stop();

	Benchmark::Start("std::vector");
	Function(vector);
	Benchmark::Stop();


	Array<Player, 100> Array;
	std::array<Player, 100> array;

	Benchmark::Start("Array");
	Function(Array);
	Benchmark::Stop();

	Benchmark::Start("std::array");
	Function(array);
	Benchmark::Stop();

#if 0
	Set<Player> Set;
	Set.SetHash(TestHash);
	std::unordered_set<Player> set;

	Benchmark::Start("Set");
	Function(Set);
	Benchmark::Stop();

	Benchmark::Start("std::unordered_set");
	Function(set);
	Benchmark::Stop();

	Map<int, Player> Map;
	std::unordered_map<int, Player> map;

	Benchmark::Start("Map");
	Function(Map);
	Benchmark::Stop();

	Benchmark::Start("std::unordered_map");
	Function(map);
	Benchmark::Stop();
#endif

	Stack<Player> Stack;
	std::stack<Player> stack;

	Benchmark::Start("Stack");
	Function(Stack);
	Benchmark::Stop();

	Benchmark::Start("std::stack");
	Function(stack);
	Benchmark::Stop();


	Queue<Player> Queue;
	std::queue<Player> queue;

	Benchmark::Start("Queue");
	Function(Queue);
	Benchmark::Stop();

	Benchmark::Start("std::queue");
	Function(queue);
	Benchmark::Stop();

	LOG("");
#endif

#ifdef BENCHMARK_PRIMATIVE

	LOG("Benchmark with Primative Type: (int)\n");

	mdt::List<int> intList;
	std::list<int> intlist;
	std::vector<int> intvector;

	Benchmark::Start("List");
	Function(intList);
	Benchmark::Stop();

	Benchmark::Start("std::list");
	Function(intlist);
	Benchmark::Stop();

	Benchmark::Start("std::vector");
	Function(intvector);
	Benchmark::Stop();


	mdt::Array<int, 100> intArray;
	std::array<int, 100> intarray;

	Benchmark::Start("Array");
	Function(intArray);
	Benchmark::Stop();

	Benchmark::Start("std::array");
	Function(intarray);
	Benchmark::Stop();


	mdt::Set<int> intSet;
	std::unordered_set<int> intset;

	Benchmark::Start("Set");
	for (int i = 0; i < 1000000; i++) {
		intSet.Insert(i);
	}
	Benchmark::Stop();

	Benchmark::Start("std::unordered_set");
	for (int i = 0; i < 1000000; i++) {
		intset.insert(i);
	}
	Benchmark::Stop();

#if 0
	Map<int> intMap;
	std::unordered_map<int, int> intmap;

	Benchmark::Start("Map");
	for (int i = 0; i < 100000; i++) {
		intMap.Insert(i);
	}
	Benchmark::Stop();

	Benchmark::Start("std::unordered_map");
	for (int i = 0; i < 100000; i++) {
		intmap.insert(std::pair<int, int>(i, i));
	}
	Benchmark::Stop();
#endif

	mdt::Stack<int> intStack;
	std::stack<int> intstack;

	Benchmark::Start("Stack");
	Function(intStack);
	Benchmark::Stop();

	Benchmark::Start("std::stack");
	Function(intstack);
	Benchmark::Stop();


	mdt::Queue<int> intQueue;
	std::queue<int> intqueue;

	Benchmark::Start("Queue");
	Function(intQueue);
	Benchmark::Stop();

	Benchmark::Start("std::queue");
	Function(intqueue);
	Benchmark::Stop();

	LOG("");

#endif

	Set<int> set;
	set.Reserve(100000);
	//std::unordered_set<Player> stdset;
	//stdset.reserve(100000);

	Benchmark::Start("Set");
	for (uint32_t i = 0; i < 100; i++) {
		set.Insert(i * i);
	}
	Benchmark::Stop();

	//Benchmark::Start("std::unordered_set");
	//for (uint32_t i = 0; i < 10; i++) {
	//	stdset.insert(Player());
	//}
	//Benchmark::Stop();

	LOG(set);

	LOG("");

	bool test = false;

	Benchmark::Start("Find: 64");
	test = set.Find(64);
	Benchmark::Stop();
	if (test) {
		LOG("Found Target!");
	}
	else {
		LOG("Not Found!");
	}

	Benchmark::Start("Find: 99");
	test = set.Find(99);
	Benchmark::Stop();
	if (test) {
		LOG("Found Target!");
	}
	else {
		LOG("Not Found!");
	}

	Benchmark::Start("Find: 100");
	test = set.Find(100);
	Benchmark::Stop();
	if (test) {
		LOG("Found Target!");
	}
	else {
		LOG("Not Found!");
	}

	Benchmark::Start("Find: 6724");
	test = set.Find(6724);
	Benchmark::Stop();
	if (test) {
		LOG("Found Target!");
	}
	else {
		LOG("Not Found!");
	}

	Benchmark::Start("Erase: 6724");
	test = set.Erase(6724);
	Benchmark::Stop();
	if (test) {
		LOG("Erased Target!");
	}
	else {
		LOG("Not Found!");
	}

	Benchmark::Start("EraseKey: 91180");
	test = set.EraseKey(91180);
	Benchmark::Stop();
	if (test) {
		LOG("Erased Target!");
	}
	else {
		LOG("Not Found!");
	}

	LOG("");
	LOG(set);

	//std::cin.get();
}