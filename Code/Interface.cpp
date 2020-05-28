#include "Interface.h"

Interface::Interface()
{
	LoadData();
}

Interface::~Interface()
{
	SaveData();
	users.clear();
	allTests.clear();
}

list<UserBase> Interface::GetUsers()
{
	return users;
}

list<Name_id> Interface::GetAllTest()
{
	return allTests;
}

void Interface::SetUsers(list<UserBase> users)
{
	this->users = users;
}

void Interface::SetAllTests(list<Name_id> allTests)
{
	this->allTests = allTests;
}

void Interface::LoadData()
{
	ifstream LoadUser("Data\\UserData\\UserData", ios::binary);

	int count;

	if (LoadUser.is_open())
	{
		LoadUser.read((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{
			UserBase temp;
			temp.LoadLPU(LoadUser);
			users.push_back(temp);
		}

		LoadUser.close();
	}

	ifstream LoadTests("Data\\Sections\\SectionsData", ios::binary);

	if (LoadTests.is_open())
	{
		LoadTests.read((char*)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{
			Name_id temp;
			temp.LoadInfo(LoadTests);
			allTests.push_back(temp);
		}

		LoadTests.close();
	}
}

void Interface::SaveData()
{
	ofstream SaveUser("Data\\UserData\\UserData", ios::binary);

	int count = users.size();

	SaveUser.write((char*)& count, sizeof(count));

	for (list<UserBase>::iterator i = users.begin(); i != users.end(); i++)
		i->SaveLPU(SaveUser);

	SaveUser.close();

	ofstream SaveTests("Data\\Sections\\SectionsData", ios::binary);

	count = allTests.size();

	SaveTests.write((char*)& count, sizeof(count));

	for (list<Name_id>::iterator i = allTests.begin(); i != allTests.end(); i++)
		i->SaveInfo(SaveTests);

	SaveTests.close();
}

void Interface::Login()
{
	string enter;
	list<User>::iterator work_with;	// Пользователь с которым мы будем работать

	if (users.size() > 0)
	{
		while (5)
		{
			system("cls");
			cout << "\n Enter your login: ";
			cin >> enter;
			for (list<UserBase>::const_iterator i = users.begin(); i != users.end(); i++)
				if (i->GetLogin() == enter)
				{
					while (5)
					{
						cout << "\n Enter your password: ";
						cin >> enter;
						if (enter == i->GetPassword())
						{
							if (i->GetUniqNum() != "1")
							{
								User workWith{ i->GetLogin(),i->GetPassword() };
								workWith.SetUniqNum(i->GetUniqNum());
								workWith.LoadUser();
								UserMenu(workWith);
								return;
							}
							else
							{
								AdministratorMenu();
								return;
							}
						}
						else
						{
							cout << "\n ! Invalid password ! \n Continue? (1 - yes, 2 - no )\n Enter: ";
							cin >> enter;

							if (enter == "1")
								continue;
							else
								return;
						}
					}
				}
			cout << "\n ! Invalid login ! \n Continue? (1 - yes, 2 - no )\n Enter: ";
			cin >> enter;

			if (enter == "1")
				continue;
			else
				break;
		}
	}
}

void Interface::Registration()
{
	system("cls");

	// Информация о пользователе (логин, пароль, id)
	UserBase tempU;
	// Доп.информация пользователя, что храниться в файле
	User tempG;

	bool check;
	string enter;

	do
	{
		check = true;
		cout << "\n Enter login (\'e\' - exit): ";
		cin >> enter;

		if (enter == "e")
			return;

		for (list<UserBase>::const_iterator i = users.begin(); i != users.end(); i++)
			if (i->GetLogin() == enter)
			{
				cout << "\n !THIS LOGIN IS ALREADY BEING USED!" << endl;
				check = false;
				break;
			}

	} while (check != true);

	tempU.SetLogin(enter);
	tempG.SetLogin(enter);

	cout << "\n Enter password: ";
	cin >> enter;

	tempU.SetPassword(enter);
	tempG.SetPassword(enter);

	cout << "\n Enter your SNP(surname, name, patronymic): ";
	cin.ignore();
	getline(cin, enter);

	tempG.SetSNP(enter);

	cout << "\n Enter your address: ";
	getline(cin, enter);

	tempG.SetAddress(enter);

	cout << "\n Enter your phone number: ";
	cin >> enter;

	tempG.SetPhone(enter);


	// Уникальный код
	string resultUnCode;

	// Тут программа будет генерировать случайный уникальный номер
	do
	{
		check = true;

		// Уникальный номер 1 - относится к админским данным и доступу
		int randUniqNum = rand() + 2;
		resultUnCode = to_string(randUniqNum);

		for (list<UserBase>::const_iterator i = users.begin(); i != users.end(); i++)
			if (i->GetUniqNum() == resultUnCode)
			{
				check = false;
				break;
			}

	} while (check == false);

	tempU.SetUniqNum(resultUnCode);
	tempG.SetUniqNum(resultUnCode);

	tempG.SaveUser();

	users.push_back(tempU);
	SaveData();

	cout << "\n You have been successfuly registrated!" << endl;
	system("pause");
}

void Interface::MainMenu()
{
	if (users.size() == 0)
	{
		cout << "\n\t #### Welcome to AskAnswer program!  ####" << endl;
		cout << " Here you will be able to pass tests, analyze your statistic and save it in the file!";
		cout << "\n\n We have found no user data. You will be administrator." << endl;
		cout << "\n You will be able to add new sections, tests and save them in the file";
		cout << "\n You will also have access to all user's data and statistic";

		string password_a;
		cout << "\n\n Your login will be: \"admin\"" << endl;
		cout << "\n Enter your password: "; cin >> password_a;

		// Уникальный номер администратора - 1
		UserBase Admin{ "admin", password_a , "1"};

		users.push_back(Admin);
	}

	while (5)
	{
		system("cls");
		cout << "########################## - AskAnswer - ##########################" << endl << endl;
		cout << "\n\t 1. Log in" << endl;
		cout << "\n\t 2. Registration" << endl;
		cout << "\n\t 3. Exit" << endl;

		int choice;

		cout << "\t\t Enter: ";

		cin >> choice;

		switch (choice)
		{
		case 1:
			Login();
			break;
		case 2:
			Registration();
			break;
		case 3:
			return;
		default:
			break;
		}
	}
}

void Interface::ShowAllSections() const
{
	for (list<Name_id>::const_iterator i = allTests.begin(); i != allTests.end(); i++)
		cout << "\n" << setw(20) << "# " << i->name;
	cout << endl;
}

void Interface::ShowAllUsers() const
{
	for (list<UserBase>::const_iterator i = users.begin(); i != users.end(); i++)
		if (i != users.begin())
			cout << " o " << i->GetLogin() << endl;
}

void Interface::UserMenu(User& userWorkWith)
{
	while (5)
	{
		system("cls");
		cout << "######################### - " << userWorkWith.GetSNP() << " - #########################" << endl << endl;
		cout << "\tAddress: " << userWorkWith.GetAddress() << "\n\tPhone: " << userWorkWith.GetPhone() << endl;

		int choice;

		cout << "\n 1. Pass test" << endl;
		cout << "\n 2. Show tests statistics" << endl;
		cout << "\n 3. Change personal info" << endl;
		cout << "\n 4. Log out" << endl;

		cout << "\n\t Enter: ";

		cin >> choice;

		string change;

		switch (choice)
		{
		case 1:
			PassTest(userWorkWith);
			userWorkWith.SaveUser();
			break;
		case 2:
			userWorkWith.GetStatistic().sort([&](const Statistic st1, const Statistic st2) { return st1.sectName < st2.sectName; });
			ShowStat(userWorkWith);
			system("pause");
			break;
		case 3:
			cout << "\n What would you like to change?";
			cout << "\n 1. SNP";
			cout << "\n 2. Address";
			cout << "\n 3. Phone";

			cout << "\n\t Enter: ";
			cin >> choice;

			if (choice >= 1 && choice <= 3)
			{
				cin.ignore();
				cout << "\n\t Enter new information: ";
				getline(cin, change);
			}

			switch (choice)
			{
			case 1:
				userWorkWith.SetSNP(change);
				break;
			case 2:
				userWorkWith.SetAddress(change);
				break;
			case 3:
				userWorkWith.SetPhone(change);
				break;
			}
			break;
		case 4:
			userWorkWith.SaveUser();
			return;
		}
	}
}

void Interface::PassTest(User& userWorkWith)
{
	cin.ignore();
	while (5)
	{
		system("cls");
		cout << "#################### - Sections - ####################"  << endl;
		ShowAllSections();

		string choice;

		cout << "\n\t Enter name of the section you wish to proceed to( \'e\' - exit ): ";
		getline(cin, choice);

		if (choice == "e")
			return;

		list<Name_id>::iterator p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) {return check.name == choice; });

		if (p != allTests.end())
		{
			Section temp;
			temp.ChangeID(p->id);
			temp.LoadTests();

			temp.PassTest(userWorkWith.GetStatistic(), choice);
		}
		else
			cout << "\n Incorrect name of the section!" << endl;
	}
}

void Interface::ShowStat(User& userWorkWith)
{
	if (userWorkWith.GetStatistic().size() > 0)
	{
		cout << setw(14) << "Section Name" << setw(30) << "Test Name" << setw(20) << "Time" << setw(40) << "Correct Answers(Num)" << setw(15) << "(Percents)" << setw(25) << "(12-grade system)" << setw(15) << "Finished" << endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

		for (list<Statistic>::iterator p = userWorkWith.GetStatistic().begin(); p != userWorkWith.GetStatistic().end(); p++)
		{
			p->ShowStatistic();
			cout << endl;
		}
	}
	else
		cout << "\n There is no information in the statistics at the moment" << endl;
}

void Interface::AdministratorMenu()
{
	while (5)
	{
		system("cls");
		cout << "######################### - Admin Window - #########################" << endl << endl;
		cout << "\n 1. Add new Section" << endl;
		cout << "\n 2. Delete Section" << endl;
		cout << "\n 3. Add new Test in Section" << endl;
		cout << "\n 4. Delete Test in Section" << endl;
		cout << "\n 5. Edit Test" << endl;
		cout << "\n 6. Add new User" << endl;
		cout << "\n 7. Delete User" << endl;
		cout << "\n 8. Modify User" << endl;
		cout << "\n 9. Statistic" << endl;
		cout << "\n 10. Change admin info" << endl;
		cout << "\n 0. Log out" << endl;

		int choice;

		cout << "\n\t Enter: ";

		cin >> choice;

		string change;
		list<Name_id>::iterator lookfor;

		switch (choice)
		{
		case 1:
			cout << "\n Enter new name of the section: ";
			cin >> change;
			AddNewSection(change);
			break;
		case 2:
			DeleteSection();
			break;
		case 3:
			AddNewTest();
			break;
		case 4:
			DeleteTest();
			break;
		case 5:
			EditTest();
			break;
		case 6:
			Registration();
			continue;
		case 7:
			DeleteUser();
			break;
		case 8:
			ModifyUser();
			break;
		case 9:
			WorkWithStatistic();
			break;
		case 10:
			cout << "\n Enter new admin login: ";
			cin.ignore();
			getline(cin, change);
			users.begin()->SetLogin(change);

			cout << "\n Enter new admin password: ";

			getline(cin, change);
			users.begin()->SetPassword(change);

			SaveData();
			break;
		case 0:
			SaveData();
			return;
		}

		cout << endl;
		system("pause");
	}
}

void Interface::AddNewSection(Name_id newSect)
{
	allTests.push_back(newSect);
}

void Interface::AddNewSection(string name)
{
	list<Name_id>::iterator p = allTests.begin();
	
	p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) { return check.name == name; });

	if (p == allTests.end())
	{
		while (5)
		{
			int idGiver = rand();
			string idGiver_string = to_string(idGiver);

			p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) { return check.id == idGiver_string; });

			if (p == allTests.end())
			{
				Name_id temp;

				temp.id = idGiver_string;
				temp.name = name;

				allTests.push_back(temp);

				SaveData();
				return;
			}
			else
				cout << "\n This sections is already exist" << endl;
		}
	}
	else
		cout << "\n This section is already exist!" << endl;
}

void Interface::DeleteSection()
{
	if (allTests.size() > 0)
	{
		list<Name_id>::iterator lookfor;
		string change;

		cout << endl;
		ShowAllSections();

		cout << "\n\t Enter name of the section you wish to delete: ";
		cin >> change;

		lookfor = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) { return change == check.name; });

		if (lookfor != allTests.end())
		{
			Section temp;
			temp.ChangeID(lookfor->id);
			temp.LoadTests();

			temp.DeleteTestsFile();

			lookfor->DeleteSectionFile();
			allTests.erase(lookfor);
			SaveData();
		}
	}
}

void Interface::AddNewTest()
{
	system("cls");
	cout << "#################### - Sections - ####################" << endl;
	ShowAllSections();

	string choice;

	cin.ignore();
	cout << "\n\t Enter name of the section in which you wish to add test to( \'e\' - exit ): ";
	getline(cin, choice);

	if (choice == "e")
		return;

	list<Name_id>::iterator p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) {return check.name == choice; });

	if (p != allTests.end())
	{
		Section temp;
		temp.ChangeID(p->id);
		temp.LoadTests();

		cout << "\n\t Enter name of the test you wish to add: ";
		getline(cin, choice);

		temp.AddTest(choice);
		temp.SaveTests();
	}
	else
		cout << "\n Incorrect name of the section!" << endl;
}

void Interface::DeleteTest()
{
	system("cls");
	cout << "#################### - Sections - ####################" << endl;
	ShowAllSections();

	string choice;

	cin.ignore();
	cout << "\n\t Enter name of the section in which you wish to delete test ( \'e\' - exit ): ";
	getline(cin, choice);

	if (choice == "e")
		return;

	list<Name_id>::iterator p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) {return check.name == choice; });

	if (p != allTests.end())
	{
		Section temp;
		temp.ChangeID(p->id);
		temp.LoadTests();

		cout << endl;
		temp.ShowTests();
		cout << endl;

		if (temp.GetTests().size() > 0)
		{
			cout << "\n\t Enter name of the test you wish to delete: ";
			getline(cin, choice);

			temp.DelTest(choice);
			temp.SaveTests();
		}
		else
			cout << "\n There are no tests to delete" << endl;
	}
	else
		cout << "\n Incorrect name of the section!" << endl;
}

void Interface::EditTest()
{
	system("cls");
	cout << "#################### - Sections - ####################" << endl;
	ShowAllSections();

	string choice;

	cin.ignore();
	cout << "\n\t Enter name of the section in which you wish to edit the test ( \'e\' - exit ): ";
	getline(cin, choice);

	if (choice == "e")
		return;

	list<Name_id>::iterator p = find_if(allTests.begin(), allTests.end(), [&](const Name_id& check) {return check.name == choice; });

	if (p != allTests.end())
	{
		Section temp;
		temp.ChangeID(p->id);
		temp.LoadTests();

		cout << endl;
		temp.ShowTests();
		cout << endl;

		if (temp.GetTests().size() > 0)
		{
			cout << "\n\t Enter name of the test you wish to edit: ";
			getline(cin, choice);

			Test tempT(temp.GetExactTestId(choice));

			if (tempT.GetID() != "-1")
			{
				tempT.LoadTest();
				tempT.ConstructTest();
				tempT.SaveTest();
			}
			else
				cout << "\n There is no test with this name" << endl;
		}
		else
			cout << "\n There are no tests to delete" << endl;
	}
	else
		cout << "\n Incorrect name of the section!" << endl;
}

void Interface::DeleteUser()
{
	system("cls");
	if (users.size() > 1)
	{
		cout << "#################### - Users - ####################" << endl;
		ShowAllUsers();

		string choice;

		cin.ignore();
		cout << "\n\t Enter login of the user you wish to delete: ( \'e\' - exit ): ";
		getline(cin, choice);

		if (choice == "e")
			return;

		list<UserBase>::iterator p = find_if(users.begin(), users.end(), [&](const UserBase& check) {return choice == check.GetLogin(); });

		if (p != users.end() && p != users.begin())
		{
			User temp;
			temp.SetUniqNum(p->GetUniqNum());
			temp.DeleteSavedFile();

			users.erase(p);
			SaveData();
		}
		else
			cout << "\n User has not been found" << endl;
	}
	else
		cout << "\n There are no-one to delete" << endl;
}

void Interface::ModifyUser()
{
	system("cls");
	if (users.size() > 1)
	{
		cout << "#################### - Users - ####################" << endl;
		ShowAllUsers();

		string choice;

		cin.ignore();
		cout << "\n\t Enter login of the user you wish to modify: ( \'e\' - exit ): ";
		getline(cin, choice);

		if (choice == "e")
			return;

		list<UserBase>::iterator p = find_if(users.begin(), users.end(), [&](const UserBase& check) {return choice == check.GetLogin(); });

		if (p != users.end() && p != users.begin())
		{
			User temp;
			temp.SetUniqNum(p->GetUniqNum());
			temp.LoadUser();

			temp.GetStatistic().sort([&](const Statistic st1, const Statistic st2) { return st1.sectName > st2.sectName; });

			list<Statistic>::iterator p2;

			while (5)
			{
				int choice2;

				system("cls");
				cout << "Login: " << p->GetLogin() << endl;
				cout << "Password: " << p->GetPassword() << endl;
				cout << "SNP: " << temp.GetSNP() << endl;
				cout << "Address: " << temp.GetAddress() << endl;
				cout << "Phone: " << temp.GetPhone() << endl;

				cout << "\n User statistic: " << endl << endl;
				ShowStat(temp);

				cout << "\n\n What would you like to do? " << endl;
				cout << "1. Change user login" << endl;
				cout << "2. Change user password" << endl;
				cout << "3. Change SNP" << endl;
				cout << "4. Change address" << endl;
				cout << "5. Change phone" << endl;
				cout << "6. Clear statistic" << endl;
				cout << "7. Delete test in statistic" << endl;

				cout << "\n\n 0. Exit" << endl;

				cout << "\n\n\t Enter: ";
				
				cin >> choice2;

				switch (choice2)
				{
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					cout << "\n\n\t Enter new information: ";

					cin.ignore();
					getline(cin, choice);

					switch (choice2)
					{
					case 1:
						p->SetLogin(choice);
						break;
					case 2:
						p->SetPassword(choice);
						break;
					case 3:
						temp.SetSNP(choice);
						break;
					case 4:
						temp.SetAddress(choice);
						break;
					case 5:
						temp.SetPhone(choice);
						break;
					}

					temp.SaveUser();
					SaveData();
					break;
				case 6:
					temp.GetStatistic().clear();
					temp.SaveUser();
					SaveData();
					break;
				case 7:
					system("cls");

					cout << "\n User statistic: " << endl << endl;
					ShowStat(temp);

					cout << "\n Enter name of the section: ";
					cin.ignore();
					getline(cin, choice);

					p2 = find_if(temp.GetStatistic().begin(), temp.GetStatistic().end(), [&](const Statistic s1) { return s1.sectName == choice; });

					if (p2 != temp.GetStatistic().end())
					{
						cout << "\n Enter name of the test: ";
						getline(cin, choice);

						p2 = find_if(temp.GetStatistic().begin(), temp.GetStatistic().end(), [&](const Statistic s1) { return s1.testName == choice; });
					
						if (p2 != temp.GetStatistic().end())
							temp.GetStatistic().erase(p2);
						else
						{
							cout << "\n This test doesn't exist" << endl << endl;
							system("pause");
						}
					}
					else
					{
						cout << "\n This section doesn't exist" << endl << endl;
						system("pause");
					}
					temp.SaveUser();
					SaveData();
					break;
				case 0:
					temp.SaveUser();
					SaveData();
					return;
				}
			}

		}
		else
			cout << "\n User has not been found" << endl;
	}
	else
		cout << "\n There are no-one to delete" << endl;
}

void Interface::Reupload(list<AllStatistic>& to_fill)
{
	for (list<UserBase>::iterator p = users.begin(); p != users.end(); p++)
	{
		if (p != users.begin())
		{
			User temp;
			temp.SetUniqNum(p->GetUniqNum());
			temp.LoadUser();

			for (list<Statistic>::iterator p2 = temp.GetStatistic().begin(); p2 != temp.GetStatistic().end(); p2++)
			{
				AllStatistic temp2(p->GetLogin(), p2->timeStart, p2->sectName, p2->testName, p2->correctAnswersNum, p2->correctPercent, p2->finalGrade, p2->testPassCheck);

				to_fill.push_back(temp2);
			}
		}
	}
}

void Interface::WorkWithStatistic()
{
	// Общая статистика по всем пользователям
	list<AllStatistic> allstatistic;

	Reupload(allstatistic);

	if (allstatistic.size() > 0)
	{
		while (5)
		{
			system("cls");
			cout << setw(15) << "User" << setw(22) << "Section Name " << setw(24) << "Test Name" << setw(16) << "Time" << setw(40) << "Correct Answers(Num)" << setw(19) << "(Percents)" << setw(21) << "(12-grade system)" << setw(15) << "Finished" << endl;
			cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

			for (list<AllStatistic>::iterator p = allstatistic.begin(); p != allstatistic.end(); p++)
			{
				p->ShowStatistic();
				cout << endl;
			}

			cout << "\n\n 1. Sort statistic";
			cout << "\n 2. Statistic find";
			cout << "\n 3. Save statistic";
			cout << "\n 4. Exit";

			cout << "\n\n\t Enter: ";

			int choice;
			string buff;
			list<AllStatistic>::iterator to_find = allstatistic.begin();

			ofstream saveTests;
			
			if (saveTests.is_open())
				saveTests.close();

			cin >> choice;

			switch (choice)
			{
			case 1:
				cout << "\n Choose way of sorting: ";
				cout << "\n\n 1. User";
				cout << "\n 2. Section name";
				cout << "\n 3. Test name";
				cout << "\n 4. Percents";
				cout << "\n 5. Final grade";
				cout << "\n 6. State";

				cout << "\n\n\t Enter: ";

				cin >> choice;

				switch (choice)
				{
				case 1:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.userName < st2.userName; });
					break;
				case 2:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.sectName < st2.sectName; });
					break;
				case 3:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.testName < st2.testName; });
					break;
				case 4:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.correctPercent > st2.correctPercent; });
					break;
				case 5:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.finalGrade > st2.finalGrade; });
					break;
				case 6:
					allstatistic.sort([&](const AllStatistic st1, const AllStatistic st2) { return st1.testPassCheck < st2.testPassCheck; });
					break;
				}
				break;
			case 2:
				cout << "\n What do you wish to find: ";
				cout << "\n\n 1. User";
				cout << "\n 2. Section name";
				cout << "\n 3. Test name";
				cout << "\n 4. Percents";
				cout << "\n 5. Final grade";
				cout << "\n 6. State";
				cout << "\n 7. Show all";

				cout << "\n\n\t Enter: ";

				cin >> choice;

				switch (choice)
				{
				case 1:
				case 2:
				case 3:
					cin.ignore();

					cout << "\n\n\t Enter name: ";
					getline(cin, buff);

					switch (choice)
					{
					case 1:
						while(5)
						{
							to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.userName != buff; });

							if (to_find != allstatistic.end())
								allstatistic.erase(to_find);
							else
								break;
						}
						break;
					case 2:
						while (5)
						{
							to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.sectName != buff; });

							if (to_find != allstatistic.end())
								allstatistic.erase(to_find);
							else
								break;
						} 
						break;
					case 3:
						while (5)
						{
							to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.testName != buff; });

							if (to_find != allstatistic.end())
								allstatistic.erase(to_find);
							else
								break;
						} 
						break;
					}
					break;
				case 4:
					cout << "\n\n\t Enter percents: ";
					cin >> choice;
					while (5)
					{
						to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return (int)st1.correctPercent != choice; });

						if (to_find != allstatistic.end())
							allstatistic.erase(to_find);
						else
							break;
					} 
					break;
				case 5:
					cout << "\n\n\t Enter final grade to find: ";
					cin >> choice;
					while (5)
					{
						to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.finalGrade != choice; });

						if (to_find != allstatistic.end())
							allstatistic.erase(to_find);
						else
							break;
					} 
					break;
				case 6:
					cout << "\n\n\t Enter ('1' - true or '-1' - false): ";
					cin >> choice;

					while (5)
					{
						if (choice == 1)
							to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.testPassCheck != true; });
						else
							to_find = find_if(allstatistic.begin(), allstatistic.end(), [&](const AllStatistic st1) {return st1.testPassCheck != false; });

						if (to_find != allstatistic.end())
							allstatistic.erase(to_find);
						else
							break;
					} 
					break;
				case 7:
					allstatistic.clear();
					Reupload(allstatistic);
					break;
				}
				break;
				case 3:
				{
					cout << "\n Enter name of the future save file: ";
					cin.ignore();
					getline(cin, buff);

					ofstream saveTests("Statistic\\" + buff + ".txt");
				
					saveTests << setw(15) << "User" << setw(22) << "Section Name " << setw(24) << "Test Name" << setw(16) << "Time" << setw(40) << "Correct Answers(Num)" << setw(19) << "(Percents)" << setw(21) << "(12-grade system)" << setw(15) << "Finished" << endl;
					saveTests << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

					for (list<AllStatistic>::iterator p = allstatistic.begin(); p != allstatistic.end(); p++)
						saveTests << *p << endl;

					saveTests.close();

				}
				case 4:
					return;
			}
		}
	}
	else
		cout << "\n There is no information in the statistics at the moment" << endl;
}

