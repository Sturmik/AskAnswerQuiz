#pragma once

#include "Sections.h"

// Класс "UserBase" является абстрактным классом
// В нем будет храниться логин и пароль пользователя 
class UserBase
{
protected:
	string login;
	string password;
	string uniqNum;	// Уникальный номер - ссылка на файл
public:
	UserBase() : UserBase("qwerty") {}
	UserBase(string login) : UserBase(login, "123456789") {}
	UserBase(string login, string password) : UserBase(login, password, "0") {}
	UserBase(string login, string password, string uniqNum);


	// Инспекторы

	string GetLogin() const;
	string GetPassword() const;
	string GetUniqNum() const;

	// Модификаторы

	void SetLogin(string login);
	void SetPassword(string password);
	void SetUniqNum(string uniqNum);

	// Метод сохранения логина, пароля и уникального номера
	void SaveLPU(ofstream& SaveLPU);
	// Метод загрузки логина, пароля и уникального номера
	void LoadLPU(ifstream& LoadLPU);
};


// Класс "User" является дочерним классом "UserBase"
// Класс будет хранить разделы тестов, которые он прошел
// Пользователь будет иметь возможность проходить тесты, смотреть статистику их прохождения
class User final : public UserBase
{
private:
	string SNP; // фамилия, имя, отчество
	string address;
	string phone;
	// Разделы пройденные пользователем
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

	// Инспекторы

	string GetSNP() const;
	string GetAddress() const;
	string GetPhone() const;
	list<Statistic>& GetStatistic();

	// Модификаторы

	void SetSNP(string SNP);
	void SetAddress(string address);
	void SetPhone(string phone);
	void SetStatistic(list<Statistic> statistic);

	// Сохранение информации про пользователя
	void SaveUser();

	// Загрузка информации про пользователя
	void LoadUser();

	// Удаление сохранненого файла
	void DeleteSavedFile();
};