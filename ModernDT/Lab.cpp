#include "ModernDT.h"

#include <iostream>


#define LOG(x)		std::cout << x << std::endl;

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
		LOG("Copyed! Vec");
	}

	Vector3(Vector3&& _other) noexcept
		: x(_other.x), y(_other.y), z(_other.z)
	{
		LOG("Moved! Vec");
	}

	~Vector3()
	{
		LOG("Destroyed! Vec");
	}

	Vector3& operator=(Vector3&& _other) noexcept
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

		LOG("Moved! Vec");

		return *this;
	}

	Vector3& operator=(const Vector3& _other)
	{
		x = _other.x;
		y = _other.y;
		z = _other.z;

		LOG("Copyed! Vec");

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
		LOG("Copyed! Play");
	}

	Player(Player&& _other) noexcept
		: m_Name(_other.m_Name), m_Level(_other.m_Level), m_Class(_other.m_Class), m_Position(std::move(_other.m_Position))
	{
		LOG("Moved! Play");
	}

	~Player()
	{
		LOG("Destroyed! Play");
	}

	Player& operator=(const Player& _other)
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = _other.m_Position;

		LOG("Copyed! Play");

		return *this;
	}

	Player& operator=(Player&& _other) noexcept
	{
		m_Name = _other.m_Name;
		m_Level = _other.m_Level;
		m_Class = _other.m_Class;
		m_Position = std::move(_other.m_Position);

		LOG("Moved! Play");

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

int main()
{
	//Example_Array();
	//Example_List();
	//Example_Set();
	//Example_Queue();
	//Example_Stack();

	Map<std::string, Player> map(10);

	map.Emplace("Warrior", Player{"Bellona", 20, Player::Warrior});
	map.Add("Assassin", { "Ravana", 20, Player::Assassin });
	map.Add("Mage", { "Zeus", 20, Player::Mage });
	map.Add("Hunter", { "Hou Yi", 20, Player::Hunter });
	map.Add("Guardian", { "Cabrakan", 20, Player::Guardian });
	map.RemoveKey("Warrior");

	for (auto& pair : map)
		LOG(pair);

	LOG(map.ContainsKey("Hunter"));
	Player player;
	LOG(map.TryGetValue("Guardian", player));
	LOG(player);
	map.Clear();

	LOG(map);

	//std::cin.get();
}