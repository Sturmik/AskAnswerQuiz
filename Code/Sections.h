#pragma once

#include "Test.h"

// ��������� ��������� ���� ������ ��������� ���������� � ����������������� ����� � ������ ��� ������
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
	// �������� �������
	string sectionName;
	// ���� ������
	list<Id_Name> tests;
public:
	Section() : Section("0") {}
	Section(string id) : Section(id, {}) {}
	Section(string id, list<Id_Name> tests);

	~Section();

	// ����������

	string GetID() const;
	list<Id_Name> GetTests() const;

	// ��������� id �����
	string GetExactTestId(string name);

	// ������������

	void ChangeID(string newID);
	void ChangeTests(list<Id_Name> newTests);

	// ����������� ������� ��� ������� ��� ����� (�� �����)
	void DeleteTestsFile();

	// ����������� ������� �������� ����������� �����
	void DeleteTestsFile(string name);

	// ���������� ����������
	void SaveTests();

	// �������� ����������
	void LoadTests();

	// ����������� �����, �� ����� �������� ���������� ��� ������ ������������ ����������
	void PassTest(list<Statistic>& userWorkWith, string sectName);

	// ����������� ���������� ��� ����� �� �������
	void ShowTests() const;

	// ������� ���������� ������ �����
	void AddTest(string name);

	// ������� �������� ����
	void DelTest(string name);
};