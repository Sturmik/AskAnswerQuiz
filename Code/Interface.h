#pragma once

#pragma once

#include "User.h"

////////////////////////////////////////////////////////////

// ��������� � ���������� � x86 ����������� ��� ������������� � ��������� �������

// ���� �� ����������� ��� ����������, �� ����� �������� ������ � �����.�������� ������� ���������: login - testadmin | password - 343049
// � ����� ���� ������ �������������� �� ������� �������� ��� ������ �� ������� ��� ���

////////////////////////////////////////////////////////////

// ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? �������� �������� ������ ��������� ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 

// �� ������ �� ��������, ��������� ����� �������� ��������� ������ � ��������� �������� ���������
// �� ����� ��������� ���� ������������� � ���������

// ������, ��� ����� ������ ��������� - ��� �������� ������ � ���� ������������� (��� ����������)

// ������, ��� ����� ������ ��������� - ��� ��������� ���������������� ������������, ����� � ��� ��������� ������� � ���������� ������ ��� � ���.
// ����� ������� ��������� �������� ���������� ��������� ��������� ���� ������������� � �� ������, ������ (�� ����������� ����������� ������), �� ����� ������� �� ����������
// � ��������� �����. ���������� ����� ��������� � bin. ������ � ��������� ���������� � �������� (����� ������ �������������� ����� � ������ ������������)
// � ������� ������� ���� ���������� �������������, ������� � ������� ����� ������ ���� � �����, � ������� � ����� �����.
// ����� ������ ��� ������ � ����� ��������� �������� ������� ������� ������ � ����� �������� ���������� �� ������� ��������

// ���������� ���� ������� � ��� ���������

// ������, ��� ����� ������ ��������� - ��� ���������� ���������� ������������ ��� ��������������.
// ������ ����� ������������� ����������� ������, �������� ���������� � ������ �������������.
 
///////////////////////////////////// ����� ������ /////////////////////////////////////

// ��������� �������� � ������ "Data", ��� ���������� � ��� � ����� �����.
// ������ "Data" ���������� ��� �����: "Sections", "UserData"

// � ��������� ��������� ���������� � �������������. � "UserData.bin" ��������� ������������ ������, ������ � ID ������ �������������.
// ID ������ ��������� ������������� ������� �� �� �������������� ����� (Ex: 34295.bin), � ������� ��������� ���.���������� � ���������������� ���������� ����������� ������.

// � ����� "Sections" �������� ����� �� ��� �����, ���� "Sections.bin" � ����� "Tests". � �������� ����� ��������� �������� �������� � ID, ��� ��������� �� 
// ����� �������� ���� ��� �������� � ����� "Tests", � ������� ��������� ���� ������ (�������� �����, ID �����), ��� ������� � �������� �� "Sections.bin". 
// � ����� ������ � ���� ������� ���������� �� ����, ��� ��������� � ���� ("Exams"), � ������� ��������� ���� ����� ��� �����������.

// ������� ������������ �������� ������:

//                        ____________________| Data |_____________________
//                       |							    			       |
//                   |Sections|				                           |UserData|
//             __________|__________                  	         __________|__________
//            |                     |                           |                     |  
// |SectionsData.bin|            |Tests|                  |UserData.bin|         |125215.bin|, ...			// " ... " - ��������, ��� �������� ������ ����� ���� �����
//                        __________|__________ 
//                       |                     |
//                 |31452.bin|, ...         |Exams|
//                                             |
//                                       |314521157.bin|, ...		// |31452.bin| � |314521157.bin| - ��������� ���� ����� ����� �� ����� � ������ "31452", ��� � ������.
																	// ���� � ���, ��� ������ ���� ������ � ���� ��������� (���� ������) � ID ������ ������ ���� ���������
																	// � ��� ���� �� ����� ������ ���������� ��� ����� ��� ���� ����� �������� ����������
																	// �������� � ������� ��������� ������� �� ������ ��������� (�������������� ����� ��������� � ������� ������� � �����).


// ������ ������ ������� � ���� ������� ������� � ������������ ������ ������:

// Login()                       ->	UserMenu()                ->  PassTest(list UserStat)       ->  Sections::PassTest(list UserStat) -> Test::PassTest() - ���� ������ ���������� UserStat-�  -> Question::Pass()
// Data\\UserData\\UserData.bin  ->	Data\\UserData\\2145.bin   |  Data\\Sections\\Sections.bin  ->  Data\\Sections\\Tests\\31452.bin  -> Data\\Sections\\Tests\\Exam\\314521157.bin                  

//                                         /|\                                                                                                         |
//                                          |__________________________________________________________________________________________________________|
//											 ���������� ����������� ����� ����������� ������� ������������ � ��� ��������� � ���� � ���� �����������

// ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

// ���������, ��� ����� ������� ���������� � ����������
struct Name_id
{
	string id;
	string name;

	Name_id(string id = "0", string name = "0")
	{
		this->id = id;
		this->name = name;
	}

	void DeleteSectionFile()
	{
		Section temp;
		temp.ChangeID(id);
		temp.LoadTests();
		temp.DeleteTestsFile();

		string to_delete = "Data\\Sections\\Tests\\" + id;

		remove(to_delete.c_str());
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

};

// ������� �����, ��� ��������� ����� ���������� ��� ������������ ������� � ���� ���� ����� ������������ � ��������� ������ �������
// �� ����� ��� ������ ����� ���������� �� ���� �������������
class AllStatistic final : public Statistic
{
public:
	string userName;

	AllStatistic() : Statistic() {}

	AllStatistic(string userName, string timeStart, string sectName, string testName, int correctAnswersNum, float correctPercent, int finalGrade, bool testPassCheck) : Statistic(sectName, testName, correctAnswersNum, correctPercent, finalGrade, testPassCheck)
	{
		this->userName = userName;
		this->timeStart = timeStart;
	}

	void ShowStatistic() const override
	{
		cout << boolalpha;
		cout << setw(15) << userName << setw(20) << sectName << setw(25) << testName << setw(35) << timeStart << setw(20) << correctAnswersNum << setw(20) << setprecision(4) << correctPercent << setw(20) << finalGrade << setw(20) << testPassCheck;
	}

	friend ostream& operator<<(ostream& out, AllStatistic& s)
	{
		cout << boolalpha;
		out << setw(15) << s.userName << setw(20) << s.sectName << setw(25) << s.testName << setw(35) << s.timeStart << setw(20) << s.correctAnswersNum << setw(20) << setprecision(4) << s.correctPercent << setw(20) << s.finalGrade << setw(20) << s.testPassCheck;
		return out;
	}
};

// ����� "Interface" �������� ����������� ������� ���� ���������
// ����� ������������ ���������� ������������� � ���������������
class Interface
{
private:
	// ���� ������������� 
	list<UserBase> users;
	// ���� ���� ��������� 
	// ����� ������� ������ ����� ������ ���� �������������, ������ ��������� ����� ������� ������ �� ����������
	list<Name_id> allTests;
public:
	Interface();

	~Interface();

	// ���������

	list<UserBase> GetUsers();
	list<Name_id> GetAllTest();

	// �����������

	void SetUsers(list<UserBase> users);
	void SetAllTests(list<Name_id> allTests);

	// ������� �������� ����������
	void LoadData();

	// ������� ���������� ����������
	void SaveData();

	// ������� ����� � �������
	void Login();

	// ������� �����������
	void Registration();

	// ������� ������ �������� ����
	void MainMenu();

	// ������� ������ ���� ���������
	void ShowAllSections() const;

	// ������� ������ ���� �������������
	void ShowAllUsers() const;

	///////////////////////////////////////////////////// �����

	// ������� ������ ���� ��� �����
	void UserMenu(User& userWorkWith);

	// ����������� �����
	void PassTest(User& userWorkWith);

	// ����������� ����������
	void ShowStat(User& userWorkWith);

	//////////////////////////////////////////////////// �������������

	// ������� ������ ���� ��������������
	void AdministratorMenu();

	// ���������� ������ �������
	void AddNewSection(Name_id newSect);

	// ���������� ������ ������� (��������)
	void AddNewSection(string name);

	// �������� ������ �������
	void DeleteSection();

	// ���������� ������ ����� � �������
	void AddNewTest();
	
	// �������� ����� 
	void DeleteTest();

	// ����������� ����� 
	void EditTest();

	// �������� ������ ������������
	void DeleteUser();

	// ����������� ������������
	void ModifyUser();

	// ��������� ��� ���������� � ����
	void Reupload(list<AllStatistic>& to_fill);

	// ������� ������ �� �����������
	void WorkWithStatistic();
};