#pragma once

#pragma once

#include "User.h"

////////////////////////////////////////////////////////////

// Работайте с программой в x86 разрядности для совместимости с бинарными файлами

// Если вы используете мои сохранения, то чтобы получить доступ в админ.контроль введите следующее: login - testadmin | password - 343049
// В самом окне работы администратора вы сможете поменять эти данные на удобные для вас

////////////////////////////////////////////////////////////

// ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? Описание механики работы программы ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 

// Не смотря на название, интерфейс будет основной отправной точкой к остальным функциям программы
// Он будет содержать лист пользователей и категорий

// Первое, что будет делать интерфейс - это загрузка данных о всех пользователях (без статистики)

// Второе, что будет делать интерфейс - это позволять зарегистрировать пользователя, войти в уже созданный аккаунт и продолжить работу уже с ним.
// Целью служило избегание ненужной перегрузки программы выгрузкой всех пользователей и их данных, тестов (со статистикой прохождения тестов), мы будем хранить их информацию
// в отдельной папке. Информация будет храниться в bin. файлах с цифровыми значениями в названии (чтобы скрыть принадлежность файла к некому пользователю)
// У каждого профиля есть уникальный идентификатор, который и поможет найти нужный файл в папке, о которой я писал ранее.
// Такой подход был выбран с целью избегания загрузки большых объемов данных и также сокрытие информации от лишнего внимания

// Аналогично дела обстоят и для категорий

// Третье, что будет делать интерфейс - это передавать управление пользователю или администратору.
// Отсюда будет производиться прохождение тестов, просмотр статистики и работа администрации.
 
///////////////////////////////////// Связь файлов /////////////////////////////////////

// Программа работает с папкой "Data", что находиться с ней в одном файле.
// Внутри "Data" находяться две папки: "Sections", "UserData"

// В последней храниться информация о пользователях. В "UserData.bin" находятся зашифрованые логины, пароли и ID номера пользователей.
// ID номера позволяют пользователям ссылать на их дополнительные файлы (Ex: 34295.bin), в которых храниться доп.информация и пользовательская статистика прохождения тестов.

// В папке "Sections" хранится опять же два файла, один "Sections.bin" и папка "Tests". В бинарном файле храняться названия разделов и ID, что ссылается на 
// некий бинарный файл что хранится в папке "Tests", в котором находится лист тестов (Название теста, ID теста), что связаны с разделом из "Sections.bin". 
// И листы тестов в свою очередь ссылаються на файл, что храниться с ними ("Exams"), в котором храняться сами тесты для прохождения.

// Попытка визуализации иерархии файлов:

//                        ____________________| Data |_____________________
//                       |							    			       |
//                   |Sections|				                           |UserData|
//             __________|__________                  	         __________|__________
//            |                     |                           |                     |  
// |SectionsData.bin|            |Tests|                  |UserData.bin|         |125215.bin|, ...			// " ... " - означает, что подобных файлов может быть много
//                        __________|__________ 
//                       |                     |
//                 |31452.bin|, ...         |Exams|
//                                             |
//                                       |314521157.bin|, ...		// |31452.bin| и |314521157.bin| - последний файл имеет такие же числа в начале "31452", что и первый.
																	// Дело в том, что первый файл хранит в себе категорию (лист тестов) и ID номера тестов этой категории
																	// И вот один из таких тестов заимствует эти цифры для того чтобы избежать совпадения
																	// значения с другими бинарными файлами из других категорий (предварительно номер сверяется с другими тестами в листе).


// Пример работы цепочки в виде вызовов функций и параллельной работы файлов:

// Login()                       ->	UserMenu()                ->  PassTest(list UserStat)       ->  Sections::PassTest(list UserStat) -> Test::PassTest() - тест вернет статистику UserStat-у  -> Question::Pass()
// Data\\UserData\\UserData.bin  ->	Data\\UserData\\2145.bin   |  Data\\Sections\\Sections.bin  ->  Data\\Sections\\Tests\\31452.bin  -> Data\\Sections\\Tests\\Exam\\314521157.bin                  

//                                         /|\                                                                                                         |
//                                          |__________________________________________________________________________________________________________|
//											 Статистика прохождение теста возращается обратно пользователю и тот добавляет её себе в лист результатов

// ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

// Структура, что будет хранить информацию о категориях
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

// Создаем класс, что наследует класс статистики для пользователя добавив в него поле имени пользователя и несколько нужных методов
// Он нужен для вывода общей статистики по всем пользователям
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

// Класс "Interface" является управляющим классом всей программы
// Здесь производится управление пользователем и администратором
class Interface
{
private:
	// Лист пользователей 
	list<UserBase> users;
	// Лист всех категорий 
	// Можно назвать единой базой данных всех пользователей, откуда последние будут черпать нужную им информацию
	list<Name_id> allTests;
public:
	Interface();

	~Interface();

	// Инспектор

	list<UserBase> GetUsers();
	list<Name_id> GetAllTest();

	// Модификатор

	void SetUsers(list<UserBase> users);
	void SetAllTests(list<Name_id> allTests);

	// Функция загрузки информации
	void LoadData();

	// Функция сохранения информации
	void SaveData();

	// Функция входа в систему
	void Login();

	// Функция регистрации
	void Registration();

	// Функция вызова главного меню
	void MainMenu();

	// Функция показа всех категорий
	void ShowAllSections() const;

	// Функция показа всех пользователей
	void ShowAllUsers() const;

	///////////////////////////////////////////////////// Гость

	// Функция вызова меню для гостя
	void UserMenu(User& userWorkWith);

	// Прохождение теста
	void PassTest(User& userWorkWith);

	// Отображение статистики
	void ShowStat(User& userWorkWith);

	//////////////////////////////////////////////////// Администратор

	// Фукнция вызова меню администратора
	void AdministratorMenu();

	// Добавление нового раздела
	void AddNewSection(Name_id newSect);

	// Добавление нового раздела (название)
	void AddNewSection(string name);

	// Удаление некого раздела
	void DeleteSection();

	// Добавление нового теста в разделе
	void AddNewTest();
	
	// Удаление теста 
	void DeleteTest();

	// Конструктор теста 
	void EditTest();

	// Удаление нового пользователя
	void DeleteUser();

	// Модификация пользователя
	void ModifyUser();

	// Загружает всю статистику в лист
	void Reupload(list<AllStatistic>& to_fill);

	// Функция работы со статистикой
	void WorkWithStatistic();
};