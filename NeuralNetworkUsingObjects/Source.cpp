#include <iostream>
#include <random>
#include <time.h>
#include <string>

using namespace std;

//Functions

double randFlt() // used to initialize weight values
{
	double randomvalue = (double)rand() / 100000;
	return randomvalue;
}

//Structures and objects

template<typename X>
struct DLnode // Double linked list node for variable arrays
{
	X data;

	DLnode* next;
	DLnode* previous;
};

//Double Linked List
template<typename X>
class DLList // The double link list object used for variable array
{
//Variables
private:
	
	DLnode<X>* head; //Front of list
	DLnode<X>* tail; //End of list

	DLnode<X>* traversalPtr = head; //Used to look around the list, it should always be reset to the head after a function is called

public:
	int sizeOfList = 0;

	DLList() //Default constructor makes an empty list
	{
		head = NULL;
		tail = NULL;
		traversalPtr = head;
	}
	
	void createNode(X data)//Created a node 
	{
		sizeOfList += 1;

		//Create a new node
		DLnode<X>* tmp = new DLnode<X>;
		tmp->data = data;
		tmp->next = NULL;
		tmp->previous = tail;

		//Update head and tail
		if (head == NULL)
		{
			head = tmp;
			tail = tmp;
		}
		else
		{
			tail->next = tmp;
			tail = tail->next;
		}
		traversalPtr = head; //Always set traversal back
	}
	
	void removeNode(X data)
	{
		sizeOfList -= 1;
		traversalPtr = head;

		while (traversalPtr->next != NULL)
		{
			if (traversalPtr->data == data)
			{
				if (traversalPtr == head)
				{
					//Define new Head
					head = traversalPtr->next;
					//Delete Connection to deleted Node
					head->previous = NULL;
					//Empty All its Contents
					EmptyNode();
					return;
				}
				else if (traversalPtr == tail)
				{
					//Define New Tail
					tail = traversalPtr->previous;
					//Delete Connection to deleted Node
					tail->next = NULL;
					//Empty All its contents
					EmptyNode();
					return;
				}
				else
				{
					//Connect the previous to next and visa versa
					traversalPtr->next->previous = traversalPtr->previous;
					traversalPtr->previous->next = traversalPtr->next;

					EmptyNode();
					return;
				}
			}
			traversalPtr = traversalPtr->next;
		}
		//ALWAYS move traversal pointer back to head
		traversalPtr = head;
		return;
	}

	//Get Values either 1 at a time or all
	X getValue(int iterator)
	{
		X dataToReturn;
		for (int i = 0; i < iterator; i++)
		{
			if (traversalPtr->next != NULL)
			{
				traversalPtr = traversalPtr->next;
			}
		}
		dataToReturn = traversalPtr->data;
		traversalPtr = head;
		return dataToReturn;
	}

	void EmptyNode()
	{
		traversalPtr->data = NULL;
		traversalPtr->next = NULL;
		traversalPtr->previous = NULL;
	}

	//DebugFunctions
	void printList()//prints just the data values
	{
		traversalPtr = head;
		cout << traversalPtr->data->name;
		while (traversalPtr->next != NULL)
		{
			cout << ", " << traversalPtr->next->data->name;
			traversalPtr = traversalPtr->next;
		}
		traversalPtr = head;
	}

	void printValues()//Suppose to print the property values of each node in the list
	{
		cout << endl << "Node: " << endl;
		
		cout << "Self: " << (traversalPtr->data)->data << endl;
		
		
		if (traversalPtr->next == NULL)
		{
			traversalPtr = head;
			return;
		}
		else
		{
			cout << "Next: " << (*((traversalPtr->next)->data)).data << endl;
			
			traversalPtr = traversalPtr->next;
			printValues();
		}
	}
};

struct nueron
{
	double data;
	string name;

	DLList<double> weights;

	DLList<nueron*> NextLayerNodes;
	DLList<nueron*> PreviousLayerNodes;
};

class Network
{
private:
	DLList<nueron*> nueronsList;
	DLList<nueron*> InputNuerons;
	DLList<nueron*> OutputNuerons;

public:
	int numberOfNodes = 0;

	Network()
	{
		numberOfNodes = 0;
	}
	
	void createNeuron(double data, string name, int place)
	{
		numberOfNodes += 1;
		nueron* tmp = new nueron;

		tmp->name = name;
		nueronsList.createNode(tmp);

		if (place == 1)
		{
			InputNuerons.createNode(tmp);
		}
		else if(place == 0)
		{
			OutputNuerons.createNode(tmp);
		}
	}

	nueron* findNueronPointer(string searchString)
	{
		//Counting Variable
		int i = 0;
		{
			//Nuerons List hold nuerons, so getValue gets an element
			while (nueronsList.getValue(i) != NULL)
			{
				//Check each Nueron's name until it is found, then return its memory location
				if (nueronsList.getValue(i)->name == searchString)
				{
					//This should return a pointer value to the node that can be used later in makeConnection
					return (nueronsList.getValue(i));
				}
				//Iterate
				i++;
			}
		}
	}
	
	void connectNeuron(string Name1, string Name2)
	{
		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		nueron1->NextLayerNodes.createNode(nueron2);
		nueron2->PreviousLayerNodes.createNode(nueron1);

	}

	void disconnectNeuron(string Name1, string Name2)
	{

		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		if (connectionexists(Name1, Name2))
		{
			
			nueron1->NextLayerNodes.removeNode(nueron2);
			
			nueron2->PreviousLayerNodes.removeNode(nueron1);

		}
		return;
	}

	bool connectionexists(string Name1, string Name2) //Know this works SO DONT DEBUG IT
	{
		//Find Node pointers
		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		//Loop over every node in the next layer nodes list of nueron 1 and previous layer nodes list of nueron 2
		for (int i = 0; i < nueron1->NextLayerNodes.sizeOfList; i++)
		{
			//Check if names is in the list
			if ((nueron1->NextLayerNodes.getValue(i)->name == Name2) || (nueron2->PreviousLayerNodes.getValue(i)->name == Name1))
			{
				return true;
			}
		}
		return false;
	}

	void showAllConnections()
	{
		nueron* nueron1 = nueronsList.getValue(0);

		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			nueron1 = nueronsList.getValue(i);

			cout << "\nNode " << nueron1->name << " is connected to "; 
			if (nueron1->NextLayerNodes.sizeOfList == 0)
			{
				cout << "nothing.";
				continue;
			}
			nueron1->NextLayerNodes.printList();

		}

	}
	
	
};


class GUI
{

public:

	Network networkBoi;

	GUI()
	{
		cout << "Program Started: " << endl;
	}

	void FrontPage()
	{
		int i = 1;
		while (i != 0)
		{
			cout << "Home Page: " << endl;
			cout << "____________________________" << endl;
			cout << "1 - Add Node: " << endl;
			cout << "2 - Connect Nodes: " << endl;
			cout << "3 - Display Network: " << endl;
			cout << "3 - Run Network: " << endl;
			cout << "0 - End Program: " << endl;
			cout << "____________________________" << endl;

			cin >> i;
			switch (i)
			{
			case(0):
			case(1):
				
				addSubNodes();
				cout << "called function";
				break;
			case(2):
				dis_connectNodes();
				break;
			case(3):
				networkBoi.showAllConnections();
				break;
			default:
				break;
				//case(4):
				//	network.run();
			}
		}
	}

	void addSubNodes()
	{
		int i = 1;

		double data = 0;
		string name;
		int place;


		while (i != 0)
		{
			
			cout << "____________________________" << endl; 
			cout << "Manipulate Nodes:" << endl;
			cout << "1 - Add Node:" << endl;
			cout << "2 - Sub Node:" << endl;
			cout << "0 - return: " << endl;

			cin >> i;

			switch (i)
			{
			case(0):
				break;
			case(1):
				cout << "Input UNIQUE name for node: " << endl;
				cin >> name;
				cout << "Specifive I / O or H: 1,0,2: ";
				cin >> place;

				networkBoi.createNeuron(data, name, place);
			}
		}
	}
	void dis_connectNodes()
	{
		int i = 1;

		string name1, name2;

		while (i != 0)
		{
			cout << "____________________________" << endl;
			cout << "1 - Connect Nodes" << endl;
			cout << "2 - Disconnect Nodes" << endl;
			cout << "0 - Return: " << endl;
			cout << "____________________________" << endl;

			cin >> i;

			switch (i)
			{
			case(0):
				break;
			case(1):
				cout << "Input Nueron1's name, then nueron 2's" << endl; 
				
				cin >> name1;
				cin >> name2;

				networkBoi.connectNeuron(name1, name2);
				break;
			case(2):
				cout << "Input Nueron1's name, then nueron 2's" << endl;

				cin >> name1;
				cin >> name2;

				networkBoi.disconnectNeuron(name1, name2);
				break;
			}

		}
	}


};


void main()
{
	GUI newGui;
	newGui.FrontPage();
}