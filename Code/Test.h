#pragma once

// Данное определение нам понадобиться для возможности использования времени системы
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

// STL
#include <list>
#include <vector>

#include <iterator>
#include <algorithm>

// Additional
#include <time.h>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Класс призванный выступать статистикой для пользователя
class Statistic
{
public:
	Statistic()
	{
		sectName = "Example_E";

		testName = "Example";

		time_t now = time(0);
		this->timeStart = ctime(&now);

		// Ввиду использования строк описанных выше, в конец строки добавляется символ "конец строки", что приводит к не нужному нам отступлению
		// Данным методом мы убираем этот символ
		timeStart.pop_back();

		this->correctAnswersNum = 0;
		this->correctPercent = 0;
		this->finalGrade = 0;
		this->testPassCheck = 0;

		this->pos = 0;
	}
	Statistic(string sectName, string testName, int correctAnswersNum, float correctPercent, int finalGrade, bool testPassCheck, int pos = 0)
	{
		this->sectName = sectName;

		this->testName = testName;

		// С помощью данных строк мы получаем текущее время системы (включая день, месяц, год)
		time_t now = time(0);
		this->timeStart = ctime(&now);

		// Ввиду использования строк описанных выше, в конец строки добавляется символ "конец строки", что приводит к не нужному нам отступлению
		// Данным методом мы убираем этот символ
		timeStart.pop_back();

		this->correctAnswersNum = correctAnswersNum;
		this->correctPercent = correctPercent;
		this->finalGrade = finalGrade;
		this->testPassCheck = testPassCheck;

		this->pos = pos;
	}


	// Отображение статистики
	virtual	void ShowStatistic() const
	{
		cout << boolalpha;
		cout << setw(15) << sectName << setw(30) << testName << setw(30) << timeStart << setw(20) << correctAnswersNum << setw(20) << setprecision(4) << correctPercent << setw(20) << finalGrade << setw(20) << testPassCheck;
	}

	// Сохранение статистики
	void SaveStat(ofstream& SaveStat)
	{
		size_t Temp_Length;

		Temp_Length = sectName.length() + 1;
		SaveStat.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveStat.write((char*)sectName.c_str(), Temp_Length);

		Temp_Length = testName.length() + 1;
		SaveStat.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveStat.write((char*)testName.c_str(), Temp_Length);

		Temp_Length = timeStart.length() + 1;
		SaveStat.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveStat.write((char*)timeStart.c_str(), Temp_Length);

		SaveStat.write((char*)& correctAnswersNum, sizeof(correctAnswersNum));

		SaveStat.write((char*)& correctPercent, sizeof(correctPercent));

		SaveStat.write((char*)& testPassCheck, sizeof(testPassCheck));

		SaveStat.write((char*)& finalGrade, sizeof(finalGrade));

		SaveStat.write((char*)& pos, sizeof(pos));
	}

	// Загрузка статистики
	void LoadStat(ifstream& LoadStat)
	{
		if ((LoadStat.is_open()))
		{
			if (LoadStat.peek() == EOF) return;

			size_t Temp_Length;
			char* Temp;

			string convert;

			int count;

			LoadStat.read((char*)& Temp_Length, sizeof(Temp_Length));
			Temp = new char[Temp_Length];
			LoadStat.read(Temp, Temp_Length);
			convert = Temp;
			sectName = convert;

			delete[] Temp;

			LoadStat.read((char*)& Temp_Length, sizeof(Temp_Length));
			Temp = new char[Temp_Length];
			LoadStat.read(Temp, Temp_Length);
			convert = Temp;
			testName = convert;

			delete[] Temp;

			LoadStat.read((char*)& Temp_Length, sizeof(Temp_Length));
			Temp = new char[Temp_Length];
			LoadStat.read(Temp, Temp_Length);
			convert = Temp;
			timeStart = convert;

			delete[] Temp;

			LoadStat.read((char*)& correctAnswersNum, sizeof(correctAnswersNum));

			LoadStat.read((char*)& correctPercent, sizeof(correctPercent));

			LoadStat.read((char*)& testPassCheck, sizeof(testPassCheck));

			LoadStat.read((char*)& finalGrade, sizeof(finalGrade));

			LoadStat.read((char*)& pos, sizeof(pos));
		}
	}

	// Название раздела
	string sectName;
	// Название теста
	string testName;
	// Время прохождения теста ( когда был открыт пользователем )
	string timeStart;
	// Количество правильных ответов
	int correctAnswersNum;
	// Процент правильных ответов
	float correctPercent;
	// Итоговая оценка ( 12-ти балльной системы )
	int finalGrade;
	// Проверка теста на завершенность
	bool testPassCheck;
	// Последний вопрос
	int pos;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Отвечает за хранение вопроса и вариантов ответа
struct Questions
{
	Questions() { correctCheck = true; }
	Questions(vector<string> answers, vector<int> rightAnswer, string question = "Example", bool correctCheck = true)
	{
		ChangeQuestion(answers, rightAnswer, question, correctCheck);
	}

	~Questions() { answers.clear(); rightAnswer.clear(); }

	void ChangeQuestion(vector<string> answers, vector<int> rightAnswer, string question, bool correctCheck = true)
	{
		this->question = question;
		this->answers = answers;
		this->rightAnswer = rightAnswer;
		this->correctCheck = correctCheck;
	}

	// Сохранение информации о вопросах
	void Save(ofstream& SaveQuest)
	{
		size_t Temp_Length;

		Temp_Length = question.length() + 1;
		SaveQuest.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveQuest.write((char*)question.c_str(), Temp_Length);

		int count = answers.size();

		SaveQuest.write((char*)& count, sizeof(count));

		for (int i = 0; i < answers.size(); i++)
		{
			Temp_Length = answers[i].length() + 1;
			SaveQuest.write((char*)& Temp_Length, sizeof(Temp_Length));
			SaveQuest.write((char*)answers[i].c_str(), Temp_Length);
		}

		count = rightAnswer.size();

		SaveQuest.write((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
			SaveQuest.write((char*)& rightAnswer[i], sizeof(rightAnswer[i]));

		SaveQuest.write((char*)& correctCheck, sizeof(correctCheck));
	}

	// Загрузка информации о вопросах
	void Load(ifstream& LoadQuest)
	{
		if ((LoadQuest.is_open()))
		{
			size_t Temp_Length;
			char* Temp;

			string convert;

			int count;

			LoadQuest.read((char*)& Temp_Length, sizeof(Temp_Length));
			Temp = new char[Temp_Length];
			LoadQuest.read(Temp, Temp_Length);
			convert = Temp;
			question = convert;

			delete[] Temp;

			LoadQuest.read((char*)& count, sizeof(count));

			for (int i = 0; i < count; i++)
			{
				LoadQuest.read((char*)& Temp_Length, sizeof(Temp_Length));
				Temp = new char[Temp_Length];
				LoadQuest.read(Temp, Temp_Length);
				convert = Temp;
				answers.push_back(convert);

				delete[] Temp;
			}

			LoadQuest.read((char*)& count, sizeof(count));
			
			int buf;

			for (int i = 0; i < count; i++)
			{
				LoadQuest.read((char*)& buf, sizeof(buf));
				rightAnswer.push_back(buf);
			}

			LoadQuest.read((char*)& correctCheck, sizeof(correctCheck));
		}
	}

	// Функция прохождения самого вопроса, счётчик отправляется для того, чтобы ввести счёт вопросов(исключетльно визуальная составляющая)
	int PassTest(int count = 0)
	{
		vector<int> check;
		int choice;
		vector<int>::iterator p;

		system("cls");

		cout << "\t### " << count << ".  " << question << "  ###" << endl;
		
		for (int i = 0; i < answers.size(); i++)
			cout << i + 1 << ". " << answers[i] << endl;

		for (int i = rightAnswer.size(); i > 0; i--)
		{
			cout << "\n\nAnswers to choose(" << i << ")" << endl;
			cout << "Enter answer number ( \'-1\' - exit ): ";
			cin >> choice;

			if (choice == -1)
				return -1;

			if (choice <= 0 || choice > answers.size())
			{
				i++;
				continue;
			}

			p = find_if(check.begin(), check.end(), [&](int verification) { return verification == choice; });

			if (p == check.end())
				check.push_back(choice);
			else
			{
				i++;
				continue;
			}

			p = find_if(rightAnswer.begin(), rightAnswer.end(), [&](int rightAnswer) { return rightAnswer == choice; });

			if (p == rightAnswer.end())
			{
				// Наши тесты - страшные, один неправильный ответ и тест помечается неправильным
				check.clear();
				correctCheck = false;
				cout << "\n Wrong answer!" << endl << endl;
				system("pause");
				return 0;
			}
		}

		if (correctCheck != false)
		{
			cout << "\n Correct answer!" << endl << endl;
			system("pause");
			return 1;
		}
	}

	// Вопрос
	string question;
	// Массив ответов ( для возможности выбора количества вариантов )
	vector<string> answers;
	// Массив номеров правильных ответов
	vector<int> rightAnswer;
	// Проверка на корректность ответа
	bool correctCheck;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Класс "Test" содержит в себе лист вопросов и название теста
// Тут также будет храниться информация о сдаче теста 
// Функционал класса: Прохождение теста, отображение информации про результаты тестирования
class Test
{
private:
	// ID
	string id;
	// Статистика прохождения теста
	Statistic info;
	// Лист вопросов
	list<Questions> quest;
	// Итератор для того чтобы начать с места, где пользователь остановился
	list<Questions>::iterator lastQuest;
public:
	Test() : Test("0") {}
	Test(string id) : Test(id, "Example Test") {}
	Test(string id, string testName) : Test (id, testName, "Example_E") {}
	Test(string id, string testName, string sectName) : Test(id, testName, sectName, 0) {}
	Test(string id, string testName, string sectName, int correctAnswersNum) : Test(id, testName, sectName, correctAnswersNum, 0) {}
	Test(string id, string testName, string sectName, int correctAnswersNum, float correctPercent) : Test(id, testName, sectName, correctAnswersNum, correctPercent, 0) {}
	Test(string id, string testName, string sectName, int correctAnswersNum, float correctPercent, int finalGrade) : Test(id, testName, sectName, correctAnswersNum, correctPercent, finalGrade, false) {}
	Test(string id, string testName, string sectName, int correctAnswersNum, float correctPercent, int finalGrade, bool testPassCheck);

	~Test();

	// Инспекторы

	string GetID() const;
	string GetTestName() const;
	string GetSectionName() const;
	string GetTimeStart() const;
	list<Questions> GetQuestions() const { return quest; }
	int GetCorrectAnswersNum() const;
	float GetCorrectPercent() const;
	int GetFinalGrade() const;
	bool GetTestPassCheck() const;
	int GetQuestPos() const;

	// Модификаторы

	void ChangeID(string id);
	void ChangeTestName(string testName);
	void ChangeSectionName(string sectName);
	void ChangeTimeStart(string timeStart);
	void ChangeQuestions(list<Questions> newQuest) { quest.clear(); quest = newQuest; };
	void ChangeCorrectAnswersNum(int correctAnswersNum);
	void ChangeCorrectPercent(float correctPercent);
	void ChangeFinalGrade(int finalGrade);
	void ChangeTestPassCheck(bool testPassCheck);
	void ChangeQuestPos(int pos);

	// Добавление нового вопроса
	void AddQuestion(Questions newQuest);

	// Функция прохождения теста
	Statistic PassTest();

	// Функция сохранения теста
	void SaveTest();

	// Функция загрузки теста
	void LoadTest();

	// Коструктор тестов
	void ConstructTest();
};