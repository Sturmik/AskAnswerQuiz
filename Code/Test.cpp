#include "Test.h"

Test::Test(string id, string testName, string sectName, int correctAnswersNum, float correctPercent, int finalGrade, bool testPassCheck)
{
	this->id = id;
	info.testName = testName;
	info.sectName = sectName;
	info.correctAnswersNum = correctAnswersNum;
	info.correctPercent = correctPercent;
	info.finalGrade = finalGrade;
	info.testPassCheck = testPassCheck;
	this->lastQuest = quest.end();
}

Test::~Test()
{
	quest.clear();
}

string Test::GetID() const
{
	return id;
}

string Test::GetTestName() const
{
	return info.testName;
}

string Test::GetSectionName() const
{
	return info.sectName;
}

string Test::GetTimeStart() const
{
	return info.timeStart;
}

int Test::GetCorrectAnswersNum() const
{
	return info.correctAnswersNum;
}

float Test::GetCorrectPercent() const
{
	return info.correctPercent;
}

int Test::GetFinalGrade() const
{
	return info.finalGrade;
}

bool Test::GetTestPassCheck() const
{
	return info.testPassCheck;
}

int Test::GetQuestPos() const
{
	return info.pos;
}

void Test::ChangeID(string id)
{
	this->id = id;
}

void Test::ChangeTestName(string testName)
{
	this->info.testName = testName;
}

void Test::ChangeSectionName(string sectName)
{
	info.sectName = sectName;
}

void Test::ChangeTimeStart(string time)
{
	this->info.timeStart = time;
}

void Test::ChangeCorrectAnswersNum(int correctAnswersNum)
{
	this->info.correctAnswersNum = correctAnswersNum;
}

void Test::ChangeCorrectPercent(float correctPercent)
{
	this->info.correctPercent = correctPercent;
}

void Test::ChangeFinalGrade(int finalGrade)
{
	this->info.finalGrade = finalGrade;
}

void Test::ChangeTestPassCheck(bool testPassCheck)
{
	this->info.testPassCheck = testPassCheck;
}

void Test::ChangeQuestPos(int pos)
{
	this->info.pos = pos;
}

void Test::AddQuestion(Questions newQuest)
{
	quest.push_back(newQuest);
	lastQuest = quest.begin();
}

Statistic Test::PassTest()
{
	lastQuest = quest.begin();

	if (info.pos == quest.size() || info.pos == 0)
	{
		info.pos = 0;
		info.correctAnswersNum = 0;
	}
	else
	if (info.pos > 0)
	{
		for (int i = 0; i < info.pos; i++)
			lastQuest++;
	}

	int res;
	int count = info.pos;

	while (lastQuest != quest.end())
	{
		res = lastQuest->PassTest(count + 1);

		if (res == -1)
			break;
		else
		{
			count++;
			ChangeCorrectAnswersNum(GetCorrectAnswersNum() + res);
			lastQuest++;
		}
	} 

	// Расчёт правильных ответов и анализ теста
	ChangeCorrectPercent(100.0 * (float)GetCorrectAnswersNum() / (float)quest.size());
	ChangeFinalGrade(12 * GetCorrectAnswersNum() / quest.size());
	ChangeTestPassCheck(lastQuest == quest.end());
	ChangeQuestPos(count);

	return info;
}

void Test::SaveTest()
{
	ofstream SaveTest("Data\\Sections\\Tests\\Exams\\" + id, ios::binary);

	size_t Temp_Length;

	Temp_Length = info.sectName.length() + 1;
	SaveTest.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveTest.write((char*)info.sectName.c_str(), Temp_Length);

	Temp_Length = info.testName.length() + 1;
	SaveTest.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveTest.write((char*)info.testName.c_str(), Temp_Length);

	int count = quest.size();

	SaveTest.write((char*)& count, sizeof(count));

	for (list<Questions>::iterator i = quest.begin(); i != quest.end(); i++)
		i->Save(SaveTest);


	SaveTest.close();
}

void Test::LoadTest()
{
	ifstream LoadTest("Data\\Sections\\Tests\\Exams\\" + id, ios::binary);

	if ((LoadTest.is_open()))
	{
		if (LoadTest.peek() == EOF) return;

		size_t Temp_Length;
		char* Temp;

		string convert;

		int count;

		LoadTest.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadTest.read(Temp, Temp_Length);
		convert = Temp;
		info.sectName = convert;

		delete[] Temp;

		LoadTest.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadTest.read(Temp, Temp_Length);
		convert = Temp;
		info.testName = convert;

		delete[] Temp;

		LoadTest.read((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{
			Questions temp;
			temp.Load(LoadTest);
			quest.push_back(temp);
		}
	}

	LoadTest.close();
}

void Test::ConstructTest()
{
	list<Questions>::iterator p = quest.begin();

	if (p == quest.end())
	{
		Questions Example;
		quest.push_back(Example);
		p = quest.begin();
	}

	int count = 1;

	while (5)
	{
		system("cls");
		cout << "### " << count << ") " << p->question << " ###" << endl;

		cout << "\n Answers: ";
		for (int i = 0; i < p->answers.size(); i++)
			cout << "\n" << i + 1 << ". " << p->answers[i];

		cout << "\n Correct answers are: ";

		for (int i = 0; i < p->rightAnswer.size(); i++)
			cout << " " << p->rightAnswer[i];

		cout << "\n         ___________";
		cout << "\n        |  Controls |";
			 
		// Это конструктор тестов. Им пользуется админ для построения тела теста. Начиная с добавления ответов заканчивая созданием новых вопросов

		cout << "\n\n 1. Change question";
		cout << "\n 2. Change answers by number";
		cout << "\n 3. Delete answers by number";
		cout << "\n 4. Add new answer";
		cout << "\n 5. Change right answers numbers";
		cout << "\n 6. Add new question";
		cout << "\n 7. Delete this question" << endl;
		
		cout << "\n x. Finish work" << endl;

		if (count != quest.size())
			cout << "\n +. Next question";
		if (count != 1)
			cout << "\n -. Previous question";

		cout << "\n\n\t Enter: ";

		char choice;
		int choice_int;

		cin >> choice;

		choice_int = choice - '0';

		Questions newQuest;

		string newName;

		switch (choice)
		{
		case '1':
			cout << "\n Enter new question name: ";
			cin.ignore();
			getline(cin, p->question);
			break;
		case '2':
			if (p->answers.size() > 0)
			{
				cout << "\n Enter question number: ";
				cin >> choice_int;

				for (int i = 0; i < p->rightAnswer.size(); i++)
				{
					if (p->rightAnswer[i] == choice_int)
						p->rightAnswer.erase(p->rightAnswer.begin() + (choice_int - 1));
				}

				if (choice_int > 0 && choice_int <= p->answers.size())
				{
					cout << "\n Enter new answer: ";
					cin.ignore();
					getline(cin, p->answers[choice_int - 1]);
				}
			}
			break;
		case '3':
			if (p->answers.size() > 0)
			{
				cout << "\n Enter question number: ";
				cin >> choice_int;

				if (choice_int > 0 && choice_int <= p->answers.size())
				{
					p->answers.erase(p->answers.begin() + (choice_int - 1));
				}
			}
			break;
		case '4':
			cout << "\n Enter new aswer: ";
			cin.ignore();
			getline(cin, newName);
			p->answers.push_back(newName);
			break;
		case '5':
			while (5)
			{
				system("cls");

				for (int i = 0; i < p->answers.size(); i++)
					cout << "\n" << i + 1 << ". " << p->answers[i];

				cout << "\n Correct answers are: ";

				for (int i = 0; i < p->rightAnswer.size(); i++)
					cout << " " << p->rightAnswer[i];

				cout << "\n Enter new right answer ( 0 - exit, -1 - clear all right answers ): ";
				cin >> choice_int;

				if (choice_int == 0)
					break;

				if (choice_int == -1)
					p->rightAnswer.clear();

				if (choice_int > 0 && choice_int <= p->answers.size())
				{
					for (int i = 0; i < p->rightAnswer.size(); i++)
						if (choice_int == p->rightAnswer[i])
						{
							cout << "\n This right answer is already exist";
							system("pause");
							break;
						}

					p->rightAnswer.push_back(choice_int);
				}
			}
			break;
		case '6':
			quest.push_back(newQuest);
			break;
		case '7':
			if (quest.size() == 1)
			{
				cout << "\n All questions have been deleted" << endl;
				quest.clear();
				SaveTest();
				return;
			}
			else
			{
				quest.erase(p);
				p = quest.begin();
			}
			break;
		case '+':
			if (count != quest.size())
			{
				count++;
				p++;
			}
			break;
		case '-':
			if (count != 1)
			{
				count--;
				p--;
			}
			break;
		case 'x':
			SaveTest();
			return;
		}
	}
}