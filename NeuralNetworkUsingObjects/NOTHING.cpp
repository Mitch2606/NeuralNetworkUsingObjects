/*
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
		cout << i << " | ";
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
	*/