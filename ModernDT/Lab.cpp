#include "ModernDT.h"

#include <iostream>

#define LOG(x)		std::cout << x << std::endl;

static int count = 0;

#define DEBU

template<typename T>
__interface Interface
{
	void Foreach(const Param<const T&>& _param);
};

struct Data
{
	std::string data;
	int* intPtr = nullptr;
	size_t size = 0;

	Data()
	{
		size = 5;
		intPtr = new int[size];
		Refresh();

#ifdef DEBUG
		count++;
		LOG("Created!");
#endif
	}

	Data(const std::string& _data)
		: data(_data) 
	{
		intPtr = new int[data.length()];
		size = data.length();
		Refresh();

#ifdef DEBUG
		count++;
		LOG("Created!");
#endif
	}

	Data(const Data& _other)
	{
		data = _other.data;
		intPtr = _other.intPtr;
		size = _other.size;

#ifdef DEBUG
		LOG("Copyed!");
#endif
	}

	Data(Data&& _other) noexcept
	{
		data = _other.data;
		intPtr = _other.intPtr;
		size = _other.size;

		free_amem(_other.intPtr);

#ifdef DEBUG
		LOG("Moved!");
#endif
	}

	~Data()
	{
		free_amem(intPtr);

#ifdef DEBUG
		count--;
		LOG("Destroyed!");
#endif
	}

	void Refresh()
	{
		for (size_t i = 0; i < size; i++)
			intPtr[i] = int();
	}

	void operator=(const Data& _other)
	{
		data = _other.data;
		intPtr = _other.intPtr;
		size = _other.size;
	}

	friend std::ostream& operator<<(std::ostream& _stream, const Data& _current)
	{
		_stream << _current.data << " [ ";

		if (_current.intPtr) {
			for (size_t i = 0; i < _current.size; i++) {
				_stream << _current.intPtr[i];
				if (i != _current.size - 1)
					_stream << ", ";
			}
		}
		_stream << " ]" << std::endl;
		return _stream;
	}

	friend bool operator==(const Data& _current, const Data& _other)
	{
		return _current.data == _other.data && _current.intPtr == _other.intPtr;
	}

	friend bool operator>(const Data& _current, const Data& _other)
	{
		return _current.data > _other.data && _current.intPtr > _other.intPtr;
	}

	friend bool operator<(const Data& _current, const Data& _other)
	{
		return _current.data < _other.data && _current.intPtr < _other.intPtr;
	}
};


int main()
{
	Array<Data, 5> array;
	List<Data> list(10);
	Set<Data> set(10);
	Map<std::string, Data> map(10);
	Queue<Data> queue(10);
	Stack<Data> stack(10);
	LinkedList<Data> linkedList;
	BinaryTree<Data> binaryTree;

	Data data1("Data 1");
	Data data2("Data 2");
	Data data3("Data 3");
	Data data4("Data 4");
	Data data5("Data 5");

	binaryTree.ForEach([&list](const Data& x)
		{
			list.Add(x);
		});

	binaryTree.Insert(data1);
	binaryTree.Insert(data2);
	binaryTree.Insert(data3);
	binaryTree.Insert(data4);

	LOG(binaryTree);
}