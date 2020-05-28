#include "User.h"

UserBase::UserBase(string login, string password, string uniqNum)
{
	this->login = login;
	this->password = password;
	this->uniqNum = uniqNum;
}

string UserBase::GetLogin() const
{
	return login;
}

string UserBase::GetPassword() const
{
	return password;
}

string User::GetSNP() const
{
	return SNP;
}

string User::GetAddress() const
{
	return address;
}

string User::GetPhone() const
{
	return phone;
}

list<Statistic>& User::GetStatistic()
{
	return statistic;
}

string UserBase::GetUniqNum() const
{
	return uniqNum;
}

void UserBase::SetLogin(string login)
{
	this->login = login;
}

void UserBase::SetPassword(string password)
{
	this->password = password;
}

void User::SetSNP(string SNP)
{
	this->SNP = SNP;
}

void User::SetAddress(string address)
{
	this->address = address;
}

void User::SetPhone(string phone)
{
	this->phone = phone;
}

void User::SetStatistic(list<Statistic> sections)
{
	this->statistic = sections;
}

void User::SaveUser()
{
	ofstream SaveUser("Data\\UserData\\" + uniqNum, ios::binary);

	size_t Temp_Length;

	Temp_Length = SNP.length() + 1;
	SaveUser.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveUser.write((char*)SNP.c_str(), Temp_Length);

	Temp_Length = address.length() + 1;
	SaveUser.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveUser.write((char*)address.c_str(), Temp_Length);

	Temp_Length = phone.length() + 1;
	SaveUser.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveUser.write((char*)phone.c_str(), Temp_Length);

	int count_size = statistic.size();

	SaveUser.write((char*)& count_size, sizeof(int));

	for (list<Statistic>::iterator i = statistic.begin(); i != statistic.end(); i++)
		i->SaveStat(SaveUser);

	SaveUser.close();
}

void User::LoadUser()
{
	ifstream LoadUser("Data\\UserData\\" + uniqNum, ios::binary);

	if ((LoadUser.is_open()))
	{
		if (LoadUser.peek() == EOF) return;

		size_t Temp_Length;
		char* Temp;

		string convert;

		int count;

		LoadUser.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadUser.read(Temp, Temp_Length);
		convert = Temp;
		SNP = convert;

		delete[] Temp;

		LoadUser.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadUser.read(Temp, Temp_Length);
		convert = Temp;
		address = convert;

		delete[] Temp;

		LoadUser.read((char*)& Temp_Length, sizeof(Temp_Length));
		Temp = new char[Temp_Length];
		LoadUser.read(Temp, Temp_Length);
		convert = Temp;
		phone = convert;

		delete[] Temp;

		LoadUser.read((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{
			Statistic temp;
			temp.LoadStat(LoadUser);
			statistic.push_back(temp);
		}
	}

	LoadUser.close();
}

void User::DeleteSavedFile()
{
	string to_delete = "Data\\UserData\\" + this->uniqNum;
	remove(to_delete.c_str());
}

void UserBase::SetUniqNum(string uniqNum)
{
	this->uniqNum = uniqNum;
}

void UserBase::SaveLPU(ofstream& SaveLPU)
{
	size_t Temp_Length;

	// Перед загрузкой данных мы их исказим для того, чтобы их нельзя было так просто счесть
	int cypher = rand() % 100 + 5;

	SaveLPU.write((char*)& cypher, sizeof(cypher));

	// Буферные переменные, что будут записаны в файл
	string tempL = login, tempP = password, tempU = uniqNum;

	for (int i = 0; i < login.length(); i++)
		tempL[i] += cypher;
	for (int i = 0; i < password.length(); i++)
		tempP[i] += cypher;
	for (int i = 0; i < uniqNum.length(); i++)
		tempU[i] += cypher;

	Temp_Length = tempL.length() + 1;
	SaveLPU.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveLPU.write((char*)tempL.c_str(), Temp_Length);

	Temp_Length = tempP.length() + 1;
	SaveLPU.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveLPU.write((char*)tempP.c_str(), Temp_Length);

	Temp_Length = tempU.length() + 1;
	SaveLPU.write((char*)& Temp_Length, sizeof(Temp_Length));
	SaveLPU.write((char*)tempU.c_str(), Temp_Length);
}

void UserBase::LoadLPU(ifstream& LoadLPU)
{
	int decypher;

	size_t Temp_Length;
	char* Temp;

	string convert;

	LoadLPU.read((char*)& decypher, sizeof(decypher));

	LoadLPU.read((char*)& Temp_Length, sizeof(Temp_Length));
	Temp = new char[Temp_Length];
	LoadLPU.read(Temp, Temp_Length);
	convert = Temp;
	login = convert;

	delete[] Temp;

	LoadLPU.read((char*)& Temp_Length, sizeof(Temp_Length));
	Temp = new char[Temp_Length];
	LoadLPU.read(Temp, Temp_Length);
	convert = Temp;
	password = convert;

	delete[] Temp;

	LoadLPU.read((char*)& Temp_Length, sizeof(Temp_Length));
	Temp = new char[Temp_Length];
	LoadLPU.read(Temp, Temp_Length);
	convert = Temp;
	uniqNum = convert;

	delete[] Temp;

	for (int i = 0; i < login.length(); i++)
		login[i] -= decypher;
	for (int i = 0; i < password.length(); i++)
		password[i] -= decypher;
	for (int i = 0; i < uniqNum.length(); i++)
		uniqNum[i] -= decypher;
}

User::User(string login, string password, string SNP, string address, string phone, string uniqNum, list<Section> sections) : UserBase(login, password, uniqNum)
{
	this->SNP = SNP;
	this->address = address;
	this->phone = phone;
}
