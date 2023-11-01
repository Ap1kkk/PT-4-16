#include <iostream>
#include <fstream>
#include <string>

//Проверка на число
bool isNumber(std::string value)
{
	for (size_t i = 0; i < value.size(); i++)
	{
		if (!isdigit(value[i]))
		{
			return false;
		}
	}
	return true;
}

//Проверка на правильность ввода
bool validateInput(int size)
{
	return size > 0;
}

class NTree {
public:
	//Для ввода с клавиатуры
	NTree(int N, int size) :root(nullptr), N(N), size(size) {}

	//Для ввода из файла
	NTree(std::string filename) :root(nullptr)
	{
		std::ifstream file(filename);
		if (!file.is_open())
		{
			std::cout << "Не удалось открыть файл." << std::endl;
			return;
		}

		std::string value;
		file >> value;
		if (isNumber(value))
		{
			N = atoi(value.c_str());
		}
		else
		{
			std::cout << "Enter correct data!" << std::endl;
			exit(-1);
		}

		file >> value;
		if (isNumber(value))
		{
			size = atoi(value.c_str());
		}
		else
		{
			std::cout << "Enter correct data!" << std::endl;
			exit(-1);
		}

		//Считываем элементы дерева из файла
		for (int i = 0; i < size; i++)
		{
			file >> value;
			if (isNumber(value))
			{
				addValue(atoi(value.c_str()));
			}
			else
			{
				std::cout << "Enter correct data!" << std::endl;
				exit(-1);
			}
		}
		file.close();
	}

	//Запускаем рекурсию удаления дерева
	~NTree()
	{
		deleteTree(root);
	}

	//Добавляем элементы
	void addValue(const int& value)
	{
		addNode(root, value);
	}

	void print()
	{
		if (root != nullptr)
			std::cout << root->value << "\n";
		print(root, 1);
	}

	void getSubTrees()
	{
		subTrees = new Node * [size];
		getSubTrees(root);
		printSubTreeRoots();
		delete[] subTrees;
	}

private:
	//Структура, которая хранит составляющие элемента дерева
	struct Node
	{
	public:
		Node(int value, int N, int level)
		{
			this->value = value;
			this->level = level;
			size = N;
			children = new Node * [N];
			for (size_t i = 0; i < N; i++)
			{
				children[i] = nullptr;
			}
		}
		~Node()
		{
			delete[] children;
		}
		int value;
		int level;
		Node** children;
		int ptr = -1;
		int currentSize = 0;

		int pathLength;

		bool isFull()
		{
			return ptr == size - 1;
		}
	private:
		int size = 0;
	};
	Node* root;													//Указатель на корень дерева
	int N;														//Размерность дерева
	int size;													//Количество элементов в дереве
	Node** subTrees;											//Массив вершин поддеревьев
	int subTreesPtr = -1;										//Указатель на последний элемент в дереве
	int subTreesAmount = 0;										//Количество поддеревьев


	void print(Node* node, int currentLevel)
	{
		if (node == nullptr)
			return;


		std::cout << "level: " + std::to_string(currentLevel) + " | child of " + std::to_string(node->value) + "\n";
		for (size_t i = 0; i < node->currentSize; i++)
		{
			std::cout << node->children[i]->value << " ";
		}
		std::cout << "\n";

		for (size_t i = 0; i < node->currentSize; i++)
		{
			print(node->children[i], currentLevel + 1);
		}
	}

	//Рекурсивная функция удаления элементов дерева
	void deleteTree(Node* root)
	{
		if (root)
		{
			for (size_t i = 0; i < root->currentSize; i++)
			{
				deleteTree(root->children[i]);
			}
			delete root;
		}
	}

	//Рекурсивная функция добавления элементов дерева
	void addNode(Node* root, const int& value)
	{
		if (root == nullptr)
		{
			root = new Node(value, N, 0);
			this->root = root;
			return;
		}
		Node* parentNode = nullptr;

		if (!root->isFull())
		{
			for (size_t i = 0; i < N; i++)
			{
				if (root->children[i] == nullptr)
				{
					root->currentSize++;
					root->ptr++;
					root->children[i] = new Node(value, N, root->level + 1);
					return;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < N; i++)
			{
				if (!root->children[i]->isFull())
				{
					addNode(root->children[i], value);
					return;
				}
			}
		}

	}

	void getSubTrees(Node* root)
	{
		if (root == nullptr)
			return;

		if (root->level % 2 == root->value % 2)
		{ 
			subTrees[++subTreesPtr] = root;
			subTreesAmount++;
		}
		for (size_t i = 0; i < root->currentSize; i++)
		{
			getSubTrees(root->children[i]);
		}
	}

	void printSubTreeRoots()
	{
		for (size_t i = 0; i < subTreesAmount; i++)
		{
			std::cout << subTrees[i]->value << " ";
		}
		std::cout << "\n";
	}

	/*void getPath(Node* root)
	{
		if (root == nullptr)
			return;

		int maxLength = 0;
		int maxOddIndex = -1;
		for (size_t i = 0; i < root->currentSize; i++)
		{
			getPath(root->children[i]);

			maxLength = std::max(maxLength, root->children[i]->pathLength);
			if (root->children[i]->pathLength > maxLength)
			{
				maxLength = root->children[i]->pathLength;
				if (root->children[i]->value % 2 != 0)
					maxOddIndex = i;
			}
		}

		if (root->value % 2 == 0)
			if (maxOddIndex != -1)
				root->pathLength = maxLength;
			else
				root->pathLength = 0;
		else
			if (maxOddIndex != -1)
				root->pathLength = maxLength + 1;
			else
				root->pathLength = 1;
	}

	void printPath(Node* root)
	{
		if (root == nullptr)
			return;

		int maxPathLength = 0;
		int maxOddIndex = -1;
		for (size_t i = 0; i < root->currentSize; i++)
		{
			auto node = root->children[i];
			if (node->value % 2 != 0 && node->pathLength > maxPathLength)
			{
				maxOddIndex = i;
				maxPathLength = node->pathLength;
			}
		}

		std::cout << std::to_string(root->value) << " ";
		if (maxOddIndex != -1)
			printPath(root->children[maxOddIndex]);
	}*/
};


int main() {
	setlocale(LC_ALL, "ru");
	int N;										//размерность дерева
	int size;									//размер дерева
	int value;									//значение узла
	int choice;									//выбор работы программы
	int count = 0;								//кол-во добавленных узлов
	while (true)
	{
		std::cout << "Введите способ работы: 1 - с файла, 2 - с клавиатуры, 3 - выход." << std::endl;
		//проверка на корректность ввода
		while (true)
		{
			if (!(std::cin >> choice))
			{
				std::cout << "Enter correct data! Enter correct value." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else
			{
				break;
			}
		}

		//с файла
		if (choice == 1)
		{
			std::string filename;
			std::cout << "Введите имя файла: " << std::endl;
			std::cin >> filename;
			NTree tree(filename);
			tree.getSubTrees();
		}
		//с клавиатуры
		else if (choice == 2)
		{
			std::cout << "Введите размерность дерева: " << std::endl;
			//проверка на корректность ввода
			while (true)
			{
				if (!(std::cin >> N))
				{
					std::cout << "Enter correct data! Enter correct value." << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else
				{
					break;
				}
			}

			std::cout << "Введите размер дерева: " << std::endl;
			//проверка на корректность ввода
			while (true)
			{
				if (!(std::cin >> size))
				{
					std::cout << "Enter correct data! Enter correct value." << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				else
				{
					break;
				}
			}

			NTree tree(N, size);
			while (count < size)
			{
				//проверка на корректность ввода
				while (true)
				{
					if (!(std::cin >> value))
					{
						std::cout << "Enter correct data! Enter correct value." << std::endl;
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					}
					else
					{
						break;
					}
				}
				tree.addValue(value);
				count++;
			}
			tree.getSubTrees();
		}
		//выход с программы
		else
		{
			exit(0);
		}
	}
}