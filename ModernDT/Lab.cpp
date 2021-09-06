#include "ModernDT.h"

#include <iostream>

#define LOG(x)		std::cout << x << std::endl;


int main()
{
	List<int> intList(10);
	Array<int, 5> intArray;
	Set<int> intSet;
	Map<std::string, float> map(10);
	Tuple<std::string, float> tuple;
	Queue<int> queue(10);
	Stack<int> stack(10);
	LinkedList<int> linkedList(10);
	BinaryTree<int> binaryTree(0);

	binaryTree.Insert(10);
	binaryTree.Insert(20);
	binaryTree.Insert(30);
	binaryTree.Insert(40);
	binaryTree.Insert(15);
	binaryTree.Insert(25);
	binaryTree.Insert(35);
	binaryTree.Insert(45);

	binaryTree.ForEach([](const int& x)
		{
			LOG(x);
		});

	LOG(binaryTree);
	LOG(binaryTree.Size());
}