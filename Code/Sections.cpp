#include "Sections.h"

Section::Section(string id, list<Id_Name> tests)
{
	this->id = id;
	this->tests = tests;
}

Section::~Section()
{
	tests.clear();
}

string Section::GetID() const
{
	return id;
}

list<Id_Name> Section::GetTests() const
{
	return tests;
}

string Section::GetExactTestId(string name) 
{
	list<Id_Name>::iterator p = find_if(tests.begin(), tests.end(), [&](const Id_Name& id_n) { return name == id_n.name; });

	if (p != tests.end())
	{
		return p->id;
	}

	return "-1";
}

void Section::ChangeID(string newID)
{
	this->id = newID;
}

void Section::ChangeTests(list<Id_Name> newTests)
{
	tests.clear(); tests = newTests;
}

void Section::DeleteTestsFile()
{
	for (list<Id_Name>::iterator p = tests.begin(); p != tests.end(); p++)
	{
		string to_delete;
		to_delete += "Data\\Sections\\Tests\\Exams\\";
		to_delete += p->id;

		remove(to_delete.c_str());
	}
}

void Section::DeleteTestsFile(string name)
{
	list<Id_Name>::iterator p = find_if(tests.begin(), tests.end(), [&](const Id_Name& check) { return name == check.name; });

	if (p != tests.end())
	{
		string to_delete;
		to_delete += "Data\\Sections\\Tests\\Exams\\";
		to_delete += p->id;

		remove(to_delete.c_str());
	}
}

void Section::SaveTests()
{
	ofstream SaveTests("Data\\Sections\\Tests\\" + id, ios::binary);

	size_t Temp_Length;

	Temp_Length = id.length() + 1;
	SaveTests.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveTests.write((char*)id.c_str(), Temp_Length);

	int count = tests.size();

	SaveTests.write((char*)& count, sizeof(count));

	for (list<Id_Name>::iterator i = tests.begin(); i != tests.end(); i++)
		i->SaveInfo(SaveTests);

	SaveTests.close();
}

void Section::LoadTests()
{
	ifstream LoadTests("Data\\Sections\\Tests\\" + id, ios::binary);

	if ((LoadTests.is_open()))
	{
		if (LoadTests.peek() == EOF) return;

		size_t Temp_Length;
		char* Temp;

		string convert;

		int count;

		LoadTests.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadTests.read(Temp, Temp_Length);
		convert = Temp;
		id = convert;

		delete[] Temp;

		LoadTests.read((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{
			Id_Name temp;
			temp.LoadInfo(LoadTests);
			tests.push_back(temp);
		}
	}

	LoadTests.close();
}

void Section::PassTest(list<Statistic>& userWorkWith, string sectName)
{
	while (5)
	{
		system("cls");
		cout << "######################## - " << sectName << " - ########################" << endl;
		ShowTests();

		string choice;

		cout << "\n\t Enter name of the test you wish to proceed to( \'e\' - get back ): ";
		getline(cin, choice);

		if (choice == "e")
			return;

		list<Id_Name>::iterator p = find_if(tests.begin(), tests.end(), [&](const Id_Name& check) {return check.name == choice; });

		if (p != tests.end())
		{
			Test temp;
			temp.ChangeID(p->id);
			temp.LoadTest();

			list<Statistic>::iterator check = find_if(userWorkWith.begin(), userWorkWith.end(), [&](const Statistic& check2) { return check2.testName == choice; });

			temp.ChangeQuestPos(0);

			if (check != userWorkWith.end())
			{
				if (check->correctAnswersNum <= temp.GetQuestions().size())
				{
					temp.ChangeCorrectAnswersNum(check->correctAnswersNum);
					temp.ChangeQuestPos(check->pos);
				}
				userWorkWith.erase(check);
			}

			temp.ChangeSectionName(sectName);
			temp.ChangeTestName(choice);

			if (temp.GetQuestions().size() != 0)
				userWorkWith.push_back(temp.PassTest());

			return;
		}
		else
			cout << "\n Incorrect name of the test!" << endl;
	}
}

void Section::ShowTests() const
{
	if (tests.size() == 0)
		cout << "\n No tests avaible!" << endl;

	for (list<Id_Name>::const_iterator i = tests.begin(); i != tests.end(); i++)
		cout << "\n\t * " << i->name << endl;
}

void Section::AddTest(string name)
{
	while (5)
	{
		int idGiver = rand();
		string idGiver_string = to_string(idGiver);

		list<Id_Name>::iterator p = tests.begin();

		p = find_if(tests.begin(), tests.end(), [&](const Id_Name check) { return check.id == idGiver_string; });

		if (p == tests.end())
		{
			tests.push_back({ id + idGiver_string, name });
			SaveTests();
			break;
		}
	}
}

void Section::DelTest(string name)
{
	list<Id_Name>::iterator p = tests.begin();

	p = find_if(tests.begin(), tests.end(), [&](const Id_Name check) { return check.name == name; });

	if (p != tests.end())
	{
		DeleteTestsFile(name);
		tests.erase(p);
		SaveTests();
	}
}
