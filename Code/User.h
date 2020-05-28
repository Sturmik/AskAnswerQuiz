#pragma once

#include "Sections.h"

// ����� "UserBase" �������� ����������� �������
// � ��� ����� ��������� ����� � ������ ������������ 
class UserBase
{
protected:
	string login;
	string password;
	string uniqNum;	// ���������� ����� - ������ �� ����
public:
	UserBase() : UserBase("qwerty") {}
	UserBase(string login) : UserBase(login, "123456789") {}
	UserBase(string login, string password) : UserBase(login, password, "0") {}
	UserBase(string login, string password, string uniqNum);


	// ����������

	string GetLogin() const;
	string GetPassword() const;
	string GetUniqNum() const;

	// ������������

	void SetLogin(string login);
	void SetPassword(string password);
	void SetUniqNum(string uniqNum);

	// ����� ���������� ������, ������ � ����������� ������
	void SaveLPU(ofstream& SaveLPU);
	// ����� �������� ������, ������ � ����������� ������
	void LoadLPU(ifstream& LoadLPU);
};


// ����� "User" �������� �������� ������� "UserBase"
// ����� ����� ������� ������� ������, ������� �� ������
// ������������ ����� ����� ����������� ��������� �����, �������� ���������� �� �����������
class User final : public UserBase
{
private:
	string SNP; // �������, ���, ��������
	string address;
	string phone;
	// ������� ���������� �������������
	list<Statistic> statistic;
public:
	User() : User("qwerty") {}
	User(string login) : User(login, "123456789") {}
	User(string login, string password) : User(login, password, "EMPTY") {}
	User(string login, string password, string SNP) : User(login, password, SNP, "EMPTY") {}
	User(string login, string password, string SNP, string address) : User(login, password, SNP, address, "EMPTY") {}
	User(string login, string password, string SNP, string address, string phone) : User(login, password, SNP, address, phone, "0") {}
	User(string login, string password, string SNP, string address, string phone, string uniqNum) : User(login, password, SNP, address, phone, uniqNum, {}) {}
	User(string login, string password, string SNP, string address, string phone, string uniqNum, list<Section> sections);

	// ����������

	string GetSNP() const;
	string GetAddress() const;
	string GetPhone() const;
	list<Statistic>& GetStatistic();

	// ������������

	void SetSNP(string SNP);
	void SetAddress(string address);
	void SetPhone(string phone);
	void SetStatistic(list<Statistic> statistic);

	// ���������� ���������� ��� ������������
	void SaveUser();

	// �������� ���������� ��� ������������
	void LoadUser();

	// �������� ������������ �����
	void DeleteSavedFile();
};