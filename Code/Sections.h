#pragma once

#include "Test.h"

// Структура выполняет роль некого хранилища информации о месторасположении файла с нужным нам тестом
struct Id_Name
{
	Id_Name(string id = "0", string name = "0")
	{
		this->id = id;
		this->name = name;
	}

	void SaveInfo(ofstream& SaveInfo)
	{
		size_t Temp_Length;

		Temp_Length = id.length() + 1;
		SaveInfo.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveInfo.write((char*)id.c_str(), Temp_Length);

		Temp_Length = name.length() + 1;
		SaveInfo.write((char*)& Temp_Length, sizeof(Temp_Length));
		SaveInfo.write((char*)name.c_str(), Temp_Length);
	}

	void LoadInfo(ifstream& LoadInfo)
	{
		size_t Temp_Length;
		char* Temp;

		string convert;

		int count;

		LoadInfo.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadInfo.read(Temp, Temp_Length);
		convert = Temp;
		id = convert;

		delete[] Temp;

		LoadInfo.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadInfo.read(Temp, Temp_Length);
		convert = Temp;
		name = convert;

		delete[] Temp;
	}

	string id;
	string name;
};

class Section
{
private:
	// ID
	string id;
	// Название раздела
	string sectionName;
	// Лист тестов
	list<Id_Name> tests;
public:
	Section() : Section("0") {}
	Section(string id) : Section(id, {}) {}
	Section(string id, list<Id_Name> tests);

	~Section();

	// Инспекторы

	string GetID() const;
	list<Id_Name> GetTests() const;

	// Возращает id теста
	string GetExactTestId(string name);

	// Модификаторы

	void ChangeID(string newID);
	void ChangeTests(list<Id_Name> newTests);

	// Специальная функция что удаляет все тесты (их файлы)
	void DeleteTestsFile();

	// Специальная функция удаления конкретного теста
	void DeleteTestsFile(string name);

	// Сохранение информации
	void SaveTests();

	// Загрузка информации
	void LoadTests();

	// Прохождение теста, мы также передаем статистику для сверки актуальности информации
	void PassTest(list<Statistic>& userWorkWith, string sectName);

	// Отображение информации про тесты из раздела
	void ShowTests() const;

	// Функция добавления нового теста
	void AddTest(string name);

	// Функция удаления теса
	void DelTest(string name);
};