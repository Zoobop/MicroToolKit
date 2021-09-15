#include "ModernDT.h"

#include <array>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>

#define BENCHMARK_PRIMATIVE
#define BENCHMARK_PLAYER

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

class Player
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

//void Example_Array()
//{
//	LOG("-------------------------------------------");
//	LOG("|               Example Array             |");
//	LOG("-------------------------------------------\n");
//
//	LOG("Prints all even numbers, excluding zero.\n");
//
//	Array<int, 10> exampleArray;
//
//	// Sets all values to 1
//	exampleArray.Fill(1);
//
//	// Multiplies the values by the factor cubed
//	int factor = 0;
//	exampleArray.ForEach([&](const int& value)
//		{
//			const_cast<int&>(value) *= factor * factor * factor;
//			factor++;
//		});
//
//	LOG(exampleArray);
//
//	// Checks for even numbers
//	for (const auto& item : exampleArray) {
//		if (item % 2 == 0 && item != 0)
//			LOG(item);
//	}
//
//	LOG("\n");
//}
//
//void Example_List()
//{
//	LOG("-------------------------------------------");
//	LOG("|               Example List              |");
//	LOG("-------------------------------------------\n");
//
//	LOG("Removes all values with indices divisible by 3.\n");
//
//	List<int> exampleList;
//
//	// Adds a list of {i, i, i}
//	for (int i = 0; i < 20; i++) {
//		exampleList.AddRange({i, i, i});
//	}
//
//	LOG(exampleList);
//
//	// Removes the item by index thats divisible by 3
//	for (size_t i = 0; i < exampleList.Capacity(); i++) {
//		if (i % 3 == 0)
//			exampleList.RemoveAt(i);
//	}
//
//	LOG(exampleList);
//
//	LOG("\n");
//}
//
//void Example_Set()
//{
//	LOG("-------------------------------------------");
//	LOG("|               Example Set               |");
//	LOG("-------------------------------------------\n");
//
//	LOG("Creates a Set out of the given list data.\n")
//
//	List<int> data = {
//		1, 4, 6, 2, 4, 7, 4, 2, 10, 12, 3, 13,
//		10, 14, 89, 22, 41, 90, 21, 26, 40, 82, 33, 113,
//		651, 454, 64, 212, 42, 73, 421, 23, 30, 312, 4, 23,
//		131, 421, 363, 542, 64, 897, 498, 12, 1430, 312, 43, 213,
//		1, 41, 900, 231, 412, 700, 490, 266, 103, 124, 31, 133,
//	};
//
//	Set<int> exampleSet(data.Capacity());
//
//	exampleSet.AddRange(data);
//
//	LOG(exampleSet.Capacity());
//	LOG(exampleSet);
//
//	LOG("\n");
//}
//
//void Example_Queue()
//{
//	LOG("-------------------------------------------");
//	LOG("|               Example Queue             |");
//	LOG("-------------------------------------------\n");
//
//	Queue<int> exampleQueue;
//
//	exampleQueue.Enqueue(10);
//	exampleQueue.Enqueue(20);
//	exampleQueue.Enqueue(30);
//	exampleQueue.Enqueue(40);
//	exampleQueue.Enqueue(50);
//
//	LOG(exampleQueue.Dequeue());
//
//	LOG(exampleQueue);
//
//	LOG("\n");
//}
//
//void Example_Stack()
//{
//	LOG("-------------------------------------------");
//	LOG("|               Example Stack             |");
//	LOG("-------------------------------------------\n");
//
//	Stack<int> exampleStack;
//
//	exampleStack.Push(10);
//	exampleStack.Push(20);
//	exampleStack.Push(30);
//	exampleStack.Push(40);
//	exampleStack.Push(50);
//
//	LOG(exampleStack.Pop());
//
//	LOG(exampleStack);
//
//	LOG("\n");
//}
//
//void Example_Map()
//{
//	Map<std::string, int> map(10);
//}
//
//void Example_LinkedList()
//{
//	LinkedList<int> linkedList(10);
//}
//
//void Example_BinaryTree()
//{
//	BinaryTree<int> binaryTree(10);
//}


// std

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
		_set.Add(T());
	}
}

template<typename T1, typename T2>
void Function(Map<T1, T2>& _map)
{
	for (int i = 0; i < 1000000; i++) {
		_map.Add(T1(), T2());
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


int main()
{
	//Example_Array();
	//Example_List();
	//Example_Set();
	//Example_Queue();
	//Example_Stack();

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
	Set<int> Set;
	std::unordered_set<int> set;

	Benchmark::Start("Set");
	Function(Set);
	Benchmark::Stop();

	Benchmark::Start("std::unordered_set");
	Function(set);
	Benchmark::Stop();

	Map<int, float> Map;
	std::unordered_map<int, float> map;

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

	LOG("Benchmark with Primative Type:\n");

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

#if 0
	Set<int> intSet;
	std::unordered_set<int> intset;

	Benchmark::Start("Set");
	Function(intSet);
	Benchmark::Stop();

	Benchmark::Start("std::unordered_set");
	Function(intset);
	Benchmark::Stop();

	Map<int, int> intMap;
	std::unordered_map<int, int> intmap;

	Benchmark::Start("Map");
	Function(intMap);
	Benchmark::Stop();

	Benchmark::Start("std::unordered_map");
	Function(intmap);
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
	//std::cin.get();
}