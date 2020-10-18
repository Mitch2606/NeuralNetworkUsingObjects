#include <iostream>
#include <random>
#include <time.h>
#include <string>

using namespace std;

//Global Variable
double lr = 0.25;

//Functions
double randFlt() // used to initialize weight values
{
	double randomvalue = (double)rand() / 100000;
	return randomvalue;
}

double relu(double x)
{
	if (x <= 0)
	{
		return 0.000000001;
	}
	else if (x > 1)
	{
		return 1;
	}
	return x;
}

double derRelu(double x)
{
	if (x <= 0)
	{
		return 0;
	}
	return 1;
}

double softMax(double x, double total)
{
	return (x / total);
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

	DLnode<X>* getTail()
	{
		return tail;
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

	int findValue(X searchValue)
	{
		traversalPtr = head;
		for (int i = 0; i < sizeOfList; i++)
		{
			if (traversalPtr->data == searchValue)
			{
				return i;
			}
			traversalPtr = traversalPtr->next;
		}
		cout << "OH FUCK" << endl;
		return -1;
	}
};

//NeuralNetwork Things
//Base unit for the neural network, a nueron
struct nueron
{
	double data;
	string name;

	DLList<double> weights;

	DLList<nueron*> NextLayerNodes;
	DLList<nueron*> PreviousLayerNodes;
};
//The Network itslef, is just a mesh data structure
class Network
{
private:
	DLList<nueron*> nueronsList;
	nueron* traversalNueron;

public:
	int numberOfNodes = 0;

	//Manipulating placement Section
	Network()
	{
		numberOfNodes = 0;
	}
	
	void createNeuron(string name)
	{
		numberOfNodes += 1;
		nueron* tmp = new nueron;
		tmp->data = NULL;
		tmp->name = name;

		nueronsList.createNode(tmp);
	}

	void deleteNeuron(string name)
	{
		nueron* nueron1 = findNueronPointer(name);
		
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			//To Node
			if (connectionexists(nueronsList.getValue(i), nueron1))
			{
				disconnectNeuron(nueronsList.getValue(i)->name, name);
			}
			//From Node
			if (connectionexists(nueron1, nueronsList.getValue(i)))
			{
				disconnectNeuron(name, nueronsList.getValue(i)->name);
			}
		}
		nueronsList.removeNode(nueron1);
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
		
		if (connectionexists(nueron1, nueron2))
		{
			cout << "Connection Already Exists! " << endl;
			return;
		}
		nueron1->NextLayerNodes.createNode(nueron2);
		nueron2->PreviousLayerNodes.createNode(nueron1);
	}

	void connectNeuron(nueron* nueron1, nueron* nueron2)//NueronPointers
	{
		
		if (connectionexists(nueron1, nueron2))
		{
			cout << "Connection Already Exists! " << endl;
			return;
		}
		nueron1->NextLayerNodes.createNode(nueron2);
		nueron2->PreviousLayerNodes.createNode(nueron1);
	}

	void disconnectNeuron(string Name1, string Name2)
	{

		nueron* nueron1 = findNueronPointer(Name1);
		nueron* nueron2 = findNueronPointer(Name2);

		if (connectionexists(nueron1, nueron2))
		{
			nueron1->NextLayerNodes.removeNode(nueron2);
			nueron2->PreviousLayerNodes.removeNode(nueron1);
		}
		return;
	}

	bool connectionexists(nueron* nueron1, nueron* nueron2) //Know this works SO DONT DEBUG IT
	{
		//Find Node pointers
	
		//Loop over every node in the next layer nodes list of nueron 1 and previous layer nodes list of nueron 2
		for (int i = 0; i < nueron1->NextLayerNodes.sizeOfList; i++)
		{
			//Check if names is in the list
			if (nueron1->NextLayerNodes.getValue(i)->name == nueron2->name)
			{
				return true;
			}
		}
		return false;
	}

	//NOT FINISHED
	void addFullGroupOfNuerons(string inputNodeName, string outputNodeName, string GroupName, int numLayers, int numNodes)
	{
		nueron* nueron1 = findNueronPointer(inputNodeName);
		nueron* nueron2 = findNueronPointer(outputNodeName);
		
		string name = GroupName;
		string name2;

		//Connect to inputNueron
		for (int i = 0; i < numNodes; i++)
		{
			name2 = name + to_string(i);
			createNeuron(name2);
			connectNeuron(nueron1, nueron2);
		}




	}

	//Show Functions
	void showAllConnections()
	{
		traversalNueron = nueronsList.getValue(0);

		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			traversalNueron = nueronsList.getValue(i);

			cout << "\nNode " << traversalNueron->name << " is connected to "; 
			if (traversalNueron->NextLayerNodes.sizeOfList == 0)
			{
				cout << "nothing.";
				continue;
			}
			traversalNueron->NextLayerNodes.printList();

		}

	}
	
	void showAllNetworkValues()
	{
		traversalNueron = nueronsList.getValue(0);
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			traversalNueron = nueronsList.getValue(i);
			for (int i = 0; i < traversalNueron->NextLayerNodes.sizeOfList; i++)
			{
				cout << traversalNueron->name << ":" << traversalNueron->data << " is connected to ";
				cout << traversalNueron->NextLayerNodes.getValue(i)->name << ":" << traversalNueron->data;
				cout << " with a weight of ";
				cout << traversalNueron->weights.getValue(i) << endl;
			}
		} 

	}

	void showAllNodeNames()
	{
		traversalNueron = nueronsList.getValue(0);
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			traversalNueron = nueronsList.getValue(i);

			cout << traversalNueron->name << ", ";
		}
	}
	
	//Actual Math Section
	void run(int apoches, double inputs[4][2], double PO[4][2])
	{
		nueron* temppointer;
		initializeWeightsOfNodes();

		temppointer = findNueronPointer("EndNode");
		
		for (int i = 0; i < (apoches * 4); i++)
		{
			cout << endl << "___________________" << endl;
			setInputs(inputs, PO);

			calculateData(temppointer);
			getOutputs();
			//backpropagation(findNueronPointer("fakeNueron"));
			resetNetwork();
			cout << "Apoche: " << i;
		}
	}

	void setInputs(double inputs[4][2], double PO[4][2])
	{
		int i1 = 0;
		static int picker = 0;

		cout << "Inputs: " << inputs[picker][0] << " , " << inputs[picker][1] << endl;
		cout << "Prefer: " << inputs[picker][0] << " , " << inputs[picker][1] << endl;

		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			if (nueronsList.getValue(i)->PreviousLayerNodes.sizeOfList == 0)
			{
				nueronsList.getValue(i)->data = inputs[picker][i1];
				i1++;
			}
		}
		picker++;
		if (picker > 3)
		{
			picker = 0;
		}
	}

	void getOutputs()
	{
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			if (nueronsList.getValue(i)->NextLayerNodes.sizeOfList != 0) {
				if (nueronsList.getValue(i)->NextLayerNodes.getValue(0)->name == "EndNode")
				{
					cout << nueronsList.getValue(i)->data << ", ";
				}
			}
		}
		cout << endl;
	}

	void initializeWeightsOfNodes()
	{
		nueron* traversalNueron = nueronsList.getValue(0);
		for (int i = 0; i < nueronsList.sizeOfList; i++)
		{
			traversalNueron = nueronsList.getValue(i);
			for (int i = 0; i < traversalNueron->NextLayerNodes.sizeOfList; i++)
			{
				traversalNueron->weights.createNode(randFlt());
			}
			
			//cout << traversalNueron->name << endl;
		}
	}

	void calculateData(nueron* passedNueron)
	{
		nueron* pNueron;
		//cout << endl << passedNueron->name << endl;
		int index;
		double temp;
		//Calculates the values
		temp = 0;
		for (int i = 0; i < passedNueron->PreviousLayerNodes.sizeOfList; i++)
		{
			pNueron = passedNueron->PreviousLayerNodes.getValue(i);

			//Checks if the values of the previous node have been calculated yet
			if (pNueron->data == 0)
			{
				calculateData(pNueron);
			}

			index = pNueron->NextLayerNodes.findValue(passedNueron);

			//cout << index << " " << passedNueron->name << " " << pNueron->name << " ";
			//cout << pNueron->data << " " << pNueron->weights.getValue(index) << endl;
			temp += pNueron->data * pNueron->weights.getValue(index);
		}
		//cout << " temp: " << temp << endl;
		passedNueron->data = relu(temp);

	}

	void backpropagation()
	{
		//todo
	}

	void resetNetwork()
	{
		nueron* TNueron = nueronsList.getValue(0);
		for (int i = 0; i <= nueronsList.sizeOfList; i++)
		{	
			if (TNueron->PreviousLayerNodes.sizeOfList != 0)
			{
				TNueron->data = 0.0;
			}
			TNueron = nueronsList.getValue(i);
		}
	}

};


class GUI
{

public:

	Network networkBoi;
	double inputs[4][2] = { {0,0},{0,1},{1,0},{1,1} };
	double preffered[4][2] = { {0,0},{0,1},{0,1},{1,1} };
	int apoches = 1;

	GUI()
	{
		cout << "Program Started: " << endl;
		//cout << "default 2,3,2 network initialized: " << endl;
		
		networkBoi.createNeuron("i1");
		networkBoi.createNeuron("i2");

		networkBoi.createNeuron("o1");
		networkBoi.createNeuron("o2");

		networkBoi.createNeuron("EndNode");

		networkBoi.connectNeuron("o1", "EndNode");
		networkBoi.connectNeuron("o2", "EndNode");

		addaBunch("h", 4, 100);
		networkBoi.run(1, inputs, preffered);

	}

	void FrontPage()
	{

		int i = 1;
		while (i != 0)
		{
			cout << endl << "____________________________" << endl;
			cout << "1 - Add Node: " << endl;
			cout << "2 - Connect Nodes: " << endl;
			cout << "3 - Display Connections: " << endl;
			cout << "4 - Display Network Values: " << endl;
			cout << "5 - Run Network: " << endl;
			cout << "0 - End Program: " << endl;
			cout << "____________________________" << endl;

			cin >> i;
			switch (i)
			{
			case(0):
				break;
			case(1):
				
				addSubNodes();
				break;
			case(2):
				if (networkBoi.numberOfNodes == 0)
				{
					cout << "You have an empty network D:";
					break;
				}
				dis_connectNodes();
				break;
			case(3):
				if (networkBoi.numberOfNodes == 0)
				{
					cout << "You have an empty network D:";
					break;
				}
				networkBoi.showAllConnections();
				break;
			case(4):
				if (networkBoi.numberOfNodes == 0)
				{
					cout << "You have an empty network D:";
					break;
				}
				networkBoi.showAllNetworkValues();
				break;
			case(5):
				if (networkBoi.numberOfNodes == 0)
				{
					break;
				}
				cout << "Eoches: ";
				cin >> apoches;

				networkBoi.run(apoches, inputs, preffered);
				break;
			default:
				break;
			}
		}
	}
	void addSubNodes()
	{
		int i = 1;

		string name;

		while (i != 0)
		{
			
			cout << "____________________________" << endl; 
			cout << "Manipulate Nodes:" << endl;
			cout << "1 - Add Node:" << endl;
			cout << "2 - Sub Node:" << endl;
			cout << "0 - return: " << endl;
			cout << "____________________________" << endl;

			cin >> i;

			switch (i)
			{
			case(0):
				break;
			case(1):
				cout << "Input UNIQUE name for node: " << endl;
				cin >> name;
				
				networkBoi.createNeuron(name);
				break;
			case(2):
				cout << "Input name of node: " << endl;
				cin >> name;

				networkBoi.deleteNeuron(name);
				break;
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
				cout << "Input Nueron1's name, then nueron 2's" << endl << "Avaliable node names: ";
				networkBoi.showAllNodeNames();
				cout << endl;
				
				cin >> name1;
				cin >> name2;

				networkBoi.connectNeuron(name1, name2);
				break;
			case(2):
				cout << "Input Nueron1's name, then nueron 2's" << endl << "Avaliable node names: ";
				networkBoi.showAllNodeNames();
				cout << endl;

				cin >> name1;
				cin >> name2;

				networkBoi.disconnectNeuron(name1, name2);
				break;
			}

		}
	}

	void addaBunch(string name1, int num, int num1)
	{
		string name = name1;
		string name2 = "i"; 
		string name3 = "o";

		string tempName;

		//FULLY connect to first layer
		for (int i = 0; i < num1; i++)
		{
			name1 = name + to_string(i);
			networkBoi.createNeuron(name1);
			//cout << "________________" << endl;
			for (int i1 = 1; i1 < 3; i1++)
			{
				tempName = name2 + to_string(i1);
				//cout << "Name: " << name << " Name1: " << name1 << " Name2: " << name2 << " Name3 " << name3 << " tName " << tempName << endl;

				networkBoi.connectNeuron(tempName, name1);
			}
		}
		
		name2 = name;
		
		tempName = name + to_string(0);
		
		for (int i = 0; i < num; i++)
		{
			cout << i  << " | ";
			for (int i = 0; i < num1; i++)
			{
				name1 = tempName + to_string(i);
				networkBoi.createNeuron(name1);
				for (int i2 = 0; i2 < num1; i2++)
				{
					name2 = name + to_string(i2);
					//cout << "Name: " << name << " Name1: " << name1 << " Name2: " << name2 << " Name3 " << name3 << " tName " << tempName << endl;
					networkBoi.connectNeuron(name2, name1);
				}
			}
			name = tempName;
			tempName = tempName + to_string(0);
			
		}
		
		name1 = name3;
		for (int i1 = 1; i1 < 3; i1++)
		{
			name3 = name1 + to_string(i1);
			//cout << "________________" << endl;
			for (int i = 0; i < num1; i++)
			{
				tempName = name + to_string(i);
			//	cout << "Name: " << name << " Name1: " << name1 << " Name2: " << name2 << " Name3 " << name3 << " tName " << tempName << endl;
				networkBoi.connectNeuron(tempName, name3);
			}
			
		}
	
	}
};


void main()
{
	GUI newGui;
}