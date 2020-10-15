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
		cout << "trying to remove" << endl;
		while (traversalPtr->next != NULL)
		{
			if (traversalPtr->data == data)
			{
				if (traversalPtr == head)
				{
					//Define new Head
					head = traversalPtr->next;
					//Delete Connection to deleted Node
					traversalPtr->next->previous = NULL;
					//Empty All its Contents
					EmptyNode();

				}
				else if (traversalPtr == tail)
				{
					//Define New Tail
					tail = traversalPtr->previous;
					//Delete Connection to deleted Node
					traversalPtr->previous->next = NULL;
					//Empty All its contents
					EmptyNode();
				}
				else
				{
					//Connect the previous to next and visa versa
					traversalPtr->next->previous = traversalPtr->previous;
					traversalPtr->previous->next = traversalPtr->next;

					//EmptyNodeOfContents
					EmptyNode();
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
		cout << traversalPtr->data << ", ";
		if (traversalPtr->next == NULL)
		{
			traversalPtr = head;
			return;
		}
		else
		{
			traversalPtr = traversalPtr->next;
			printList();
		}

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

class FullyConnectedNetwork
{
private:
	int numberOfNodes = 0;
	DLList<nueron*> nueronsList;
	DLList<nueron*> InputNuerons;
	DLList<nueron*> OutputNuerons;

public:
	FullyConnectedNetwork()
	{
		numberOfNodes = 0;
	}
	
	void createNeuron(double data, string name, int place)
	{
		numberOfNodes += 1;
		nueron* tmp = new nueron;

		tmp->name = name;
		nueronsList.createNode(tmp);

		if (place == 0)
		{
			InputNuerons.createNode(tmp);
		}
		else if(place == 1)
		{
			OutputNuerons.createNode(tmp);
		}
	}

	void connectNeuron(string Name1, string Name2)
	{
		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		nueron1->NextLayerNodes.createNode(nueron2);
		nueron2->PreviousLayerNodes.createNode(nueron1);

	}

	

	bool connectionexists(string Name1, string Name2)
	{
		
		//Find Node pointers
		nueron* nueron1 = findNueronPointer(Name1);
		cout << "got to nueron 2";
		nueron* nueron2 = findNueronPointer(Name2);

		//Loop over every node in the next layer nodes list of nueron 1 and previous layer nodes list of nueron 2
		for (int i = 0; i < nueron1->NextLayerNodes.sizeOfList; i++)
		{
			//Check if names is in the list
			if ((nueron1->NextLayerNodes.getValue(i)->name == Name2) ||  (nueron2->PreviousLayerNodes.getValue(i)->name == Name1))
			{
				cout << "There is a connection! " << endl;
				return true;
			}
		}
		cout << "There is no connection" << endl;
		return false;
	}

	void disconnectNeuron(string Name1, string Name2)
	{
		cout << "got to variabels: " << endl;
		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		cout << "does connection exist? " << endl;
		if (connectionexists(Name1, Name2))
		{
			nueron1->NextLayerNodes.removeNode(nueron2);
			nueron2->PreviousLayerNodes.removeNode(nueron1);
		}
		return;

	}

	void showAllConnections()
	{
		nueron* nueron1 = nueronsList.getValue(0);
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			*nueron1 = *(nueronsList.getValue(i));
			if ((nueron1->NextLayerNodes).sizeOfList == 0)
			{
				return;
			}
			else
			{
				for (int i1 = 0; i1 < (nueron1->NextLayerNodes).sizeOfList; i1++)
				{
					cout << endl << "From: ";
					cout << nueron1->name << " to ";
					cout << (nueron1->NextLayerNodes).getValue(i1)->name;
				}
			}
			cout << endl;
			
		}
	}
	
	nueron* findNueronPointer(string searchString)
	{
		cout << "Finding Pointer Value for " << searchString << endl;
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

};




void main()
{
	FullyConnectedNetwork network;
	DLList<int> tempList;

	int i = 0;
	/*
	//Basic GUI for before VR setup
	while (i == i)
	{

	}
	*/

	network.createNeuron(1, "a", 0);

	network.createNeuron(1, "h1", 2);
	
	network.createNeuron(1, "h2", 2);

	
	network.createNeuron(2, "b", 1);

	network.connectNeuron("a", "b");

	network.connectNeuron("a", "h1");
	network.connectNeuron("a", "h2");

	network.connectNeuron("h1", "b");
	network.connectNeuron("h2", "b");


	cout << endl << network.connectionexists("a" , "h1") << endl;
	network.disconnectNeuron("a", "h1");

	cout << endl << "show Connection: ";
	network.showAllConnections();

}