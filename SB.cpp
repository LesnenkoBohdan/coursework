/*
* Курсова робота 
* Студента 2 курсу ЛЕСНЕНКО Богдана Олександровича
* Завдання №
* Тема завдання: Гра Морський Бій
*/
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <locale>
#include <fstream>
#include <iomanip>
#include <random>
#include <ctime>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <time.h>
#include <Windows.h>

using namespace std;

#include "SB_TCP.h"
#include "SB.h"


#pragma region Системний: БІБЛІОТЕКИ для текстового відображення значень

//Ініціалізація бібліотек
void Init_Lib()
{
	vGame_Mode.clear(); // Вектор РЕЖИМ ГРИ
	vGame_Mode.push_back("локальна");
	vGame_Mode.push_back("мережева");
	vGame_Place.clear();// Вектор РОБОЧЕ МІСЦЕ
	vGame_Place.push_back("комп'ютер");
	vGame_Place.push_back("сервер");
	vGame_Place.push_back("клієнт");
	Shot_Result_Short.clear();// Вектор Коротка назва результату пострілу
	Shot_Result_Short.push_back("N");//новий постріл
	Shot_Result_Short.push_back("M");//промазав
	Shot_Result_Short.push_back("W");//поранив
	Shot_Result_Short.push_back("K");//вбив
	Shot_Result_Short.push_back("E");//вихід
	Shot_Result_Short.push_back("T");//повторити 
	Shot_Result_Short.push_back("S");//знову
	Shot_Result.clear();// Вектор - назва результату пострілу
	Shot_Result.push_back("нічого");
	Shot_Result.push_back("промазав");
	Shot_Result.push_back("поранив");
	Shot_Result.push_back("вбив");
	Shot_Result.push_back("вихід");
	Shot_Result.push_back("повтор");
	Shot_Result.push_back("спроба");
	Player_stan.clear(); // Вектор Стан гравця
	Player_stan.push_back("Гравець");
	Player_stan.push_back("Переможець");
	Player_stan.push_back("Програвший");
	Player_stan.push_back("Здався");
	Player_type.clear(); // Вектор ТИП гравця
	Player_type.push_back("комп`ютер");
	Player_type.push_back("людина");
	Player_type.push_back("клієнт");
	Player_level.clear(); // Вектор РІВЕНЬ гравця
	Player_level.push_back("низький");
	Player_level.push_back("високий");
}
#pragma endregion

#pragma region Системний: координати + колір + повідомлення
//Зміна кольору тексту та фону, за замовченням білим на чорному
void SetColor(int text = White, int bg = Black)
{
	HANDLE Red = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(Red, (WORD)((bg << 4) | text));
}
//Повернути координату Х точки
int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}
//Повернути координату У точки
int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.Y;
}
//Перейти до координат Х,У
void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}
//Спинер для чекання повідомлень
int Spiner(int i)
{
	int x, y;
	char spiner[5] = "-/|\\";
	if (i > 3) i = 0; //else i++;
	y = wherey();
	cout << spiner[i];
	x = wherex();
	--x; i++;
	gotoxy(x, y);
	return i;
}
//Очистити екран під повідомлення
void clearMsgPlace(int p)
{
	clearMsgPlace(p, 6 + sbf);
}
//Очистити екран під повідомлення
void clearMsgPlace(int p, int fp = 7 + sbf)
{
	int wend = p; // 2 точка на екрані - для стирання повідомлення
	int wstart = fp; // 1 точка на екрані - для стирання повідомлення
	for (int f = wstart; f <= wend + 1; f++) { 
		gotoxy(0, f); //SetColor(Black, Black);
	cout << "\r" <<setw(2) << string(scrsize - 1, ' ');
	cout.flush();
	}
	gotoxy(0, wstart - 1);
}
//Повідомити про помилковий постріл
int ShowErrorShot(Player* pl, myShot sh)
{
	int p;
	for (auto& err : sh.ShotError)
	{
		switch (err)
		{
		case enShotErr::NotValid:
			cout << "Помилкові координати" << endl;
			break;
		case enShotErr::NotUniq:
			cout << "Не унікальні координати" << endl;
			break;
		case enShotErr::NotLogic:
			cout << "Не логічні - вже використані" << endl;
			break;
		default: break;
		}
	}
	system("pause");
	p = wherey();
	return p;
}
//Вивести повідомлення на екран
void Wait(string strmsg)
{
	clearMsgPlace(7 + sbf, 6 + sbf);
	cout << "\r" << strmsg << string(scrsize - strmsg.length(), ' ');
	cout.flush();
}
//Отримати координати пострілу від людини
string ascForShot(Player* pl)
{
	string _Shot = "";
	SetColor();	cout << "Ваш крок, ";
	SetColor(LightCyan); cout << pl->Name;
	SetColor(); cout << " (кількість пострілів " << pl->cnt_shots << ")";
	SetColor(); cout << " {формат " << ((pl->cnt_shots > 1) ? "A1,B4,F5)" : "A1)");
	cout << " (0 - вихід)" << ": ";
	SetColor(); cin >> _Shot;
	return _Shot;
}
#pragma endregion

#pragma region Desks function
void Desk::Kill() { alive = false; };
#pragma endregion

#pragma region Class-Functions Ship - Корабель
//Класс КОРАБЕЛЬ
Ship::Ship()
{
	desks = 0;
	Desks.clear();
};
//Класс КОРАБЕЛЬ 2
Ship::Ship(int orient, int desks)
{
	desks = desks;
	orient = orient;
};
//Класс КОРАБЕЛЬ - Додати
void Ship::Add(Desk* dsk)
{
	desks++;
	Desks.push_back(*dsk);
};
//Класс КОРАБЕЛЬ - Вбити  + палубу
ShipStan Ship::Kill(Desk* dsk)
{	
	return Kill(dsk->x,dsk->y);
};
ShipStan Ship::Kill(int x, int y)
{
	Desk* dsk_s = SearchDesk(x,y);
	if (dsk_s != nullptr) {
		if (dsk_s->alive == true)
		{
			dsk_s->Kill();
			deskskill++;
			if (deskskill == desks) stan = ShipStan::killed; else stan = ShipStan::wound;
		}
	}
	return stan;
};

//Класс КОРАБЕЛЬ - Отримати СТАН Корабля
ShipStan Ship::GetStan() {
	int alive_dsk = 0, wound_dsk = 0;
	for (auto& _Desks : Desks)
	{
		if (_Desks.alive == true) alive_dsk++; else wound_dsk++;
	}
	if (alive_dsk == desks) return ShipStan::alive;
	else if (wound_dsk > 0) return ShipStan::wound;
	else if (wound_dsk == desks) return ShipStan::killed;
	else return ShipStan::alive;
}
//Класс КОРАБЕЛЬ - видалення Корабля
Ship::~Ship()
{
};
void Ship::ReservAround(int** mtx, PointVal pv) 
{
	//Резервування полів навколо вдалого влучення
	//void Rezerv_Shot(int x, int y, int** mtx, int marker, bool alldesk = false)
	for (auto &dsk : Desks)
	{
		//угли по діагоналі
		if (Valid_Coord(dsk.x - 1, dsk.y - 1) && 
			(mtx[dsk.y - 1][dsk.x - 1] == (int)PointVal::reserv || 
				mtx[dsk.y - 1][dsk.x - 1] == (int)PointVal::sea)) mtx[dsk.y - 1][dsk.x - 1] = (int)pv;
		if (Valid_Coord(dsk.x - 1, dsk.y + 1) &&
			(mtx[dsk.y + 1][dsk.x - 1] == (int)PointVal::reserv || 
				mtx[dsk.y + 1][dsk.x - 1] == (int)PointVal::sea)) mtx[dsk.y + 1][dsk.x - 1] = (int)pv;
		if (Valid_Coord(dsk.x + 1, dsk.y - 1) &&
			(mtx[dsk.y - 1][dsk.x + 1] == (int)PointVal::reserv || 
				mtx[dsk.y - 1][dsk.x + 1] == (int)PointVal::sea)) mtx[dsk.y - 1][dsk.x + 1] = (int)pv;
		if (Valid_Coord(dsk.x + 1, dsk.y + 1) &&
			(mtx[dsk.y + 1][dsk.x + 1] == (int)PointVal::reserv || 
				mtx[dsk.y + 1][dsk.x + 1] == (int)PointVal::sea)) mtx[dsk.y + 1][dsk.x + 1] = (int)pv;
		if (Valid_Coord(dsk.x, dsk.y - 1) &&
			(mtx[dsk.y - 1][dsk.x] == (int)PointVal::reserv || 
				mtx[dsk.y - 1][dsk.x] == (int)PointVal::sea)) mtx[dsk.y - 1][dsk.x] = (int)pv;
		if (Valid_Coord(dsk.x, dsk.y + 1) &&
			(mtx[dsk.y + 1][dsk.x] == (int)PointVal::reserv || 
				mtx[dsk.y + 1][dsk.x] == (int)PointVal::sea)) mtx[dsk.y + 1][dsk.x] = (int)pv;
		if (Valid_Coord(dsk.x - 1, dsk.y) &&
			(mtx[dsk.y][dsk.x - 1] == (int)PointVal::reserv || 
				mtx[dsk.y][dsk.x - 1] == (int)PointVal::sea)) mtx[dsk.y][dsk.x - 1] = (int)pv;
		if (Valid_Coord(dsk.x + 1, dsk.y) &&
			(mtx[dsk.y][dsk.x + 1] == (int)PointVal::reserv || 
				mtx[dsk.y][dsk.x + 1] == (int)PointVal::sea)) mtx[dsk.y][dsk.x + 1] = (int)pv;
	}
};

Desk* Ship::SearchDesk(Point* pnt) {
	return SearchDesk(pnt->x, pnt->y);
};
Desk* Ship::SearchDesk(int x, int y) {
	Desk dsk(x, y);
	Desk* search_dsk = nullptr;
	if (dsk.valid())
	{
			auto searchDesk = find(Desks.begin(), Desks.end(), dsk);
			if (searchDesk != Desks.end())
			{
				search_dsk = &searchDesk[0];
			}
	}
	return search_dsk;
};

#pragma endregion

#pragma region Class-Functions - Escadra - Ескадра кораблів для Гравця
//Додати Корабель в Ескадру
void Escadra::Add(Ship* ship)
{
	ships++;
	Ships.push_back(*ship);
};
Ship* Escadra::SearchShip(Point pnt) // знайти корабель в ескадрі за координатами палуби
{
	return SearchShip(pnt.x, pnt.y);
}
Ship* Escadra::SearchShip(int x, int y) // знайти корабель в ескадрі за координатами палуби
{
	Desk dsk(x, y);
	Ship* srch_ship= nullptr;
	if (dsk.valid()) 
	{
		for (auto& ship : Ships)
		{
			auto searchDesk = find(ship.Desks.begin(), ship.Desks.end(), dsk);
			if (searchDesk != ship.Desks.end())
			{
				srch_ship = &ship;
			}
		}
	}
	return srch_ship;
}
#pragma endregion

#pragma region Class-Functions - Player - ГРАВЕЦЬ
//функція початку нової гри для гравця
void Player::New_Shot()
{
	myShotsRes = ShotRes::New;
	SecondCompShot = false;
	CompHit = false;
	NextCompHit = false;
	ShotDirection = -1;
	CompMiss[0] = false;
	CompMiss[1] = false;
	CompMiss[2] = false;
	CompMiss[3] = false;
};
//функція початку нової гри для гравця
void Player::New_Game()
{
	shot = 0;
	kill = 0;
	cnt_shots = cnt_shot;
	ps = Player_Stat::InGame;
	myShots.clear();
	New_Shot();
};
//Створення нового гравця
Player::Player()
{
	Name = "Гравець";
	New_Game();
	level = Player_Level::level0;
	pm = Player_Mode::comp;	//за замовченням тип гравця = коп'ютер
	myEscadra = new Escadra();
};
//Створення нового гравця
Player::Player(string name, Player_Mode pm1 = Player_Mode::comp)
{
	New_Game();
	Name = name;
	level = Player_Level::level0;
	pm = pm1;
	myEscadra = new Escadra();
};
//Видалення ГРАВЦЯ
Player::~Player()
{
};
//Додати постріли з СТРОКИ в ГРАВЕЦЬ--Нові постріли
void Player::Add_ShotFromNew(myShot myNewShot) {
	myShots.push_back(myNewShot);
};

//Додати постріл з СТРОКИ в ГРАВЕЦЬ--Нові постріли
bool Player::Add_AllShot(string val, int** mtx)
{
	myShot ms(val); // постріл зі строки 
	if (ms.valid() == true)
	{
		ms.shres = ShotRes::New;
		if (ms.logic(mtx)) {}
		else if (ms.uniq(mtx)) {}
		myNewShots.push_back(ms);
		return true;
	}
	else
	{
		ms.shres = ShotRes::TryAgain;
		if (ms.logic(mtx)) {}
		else if (ms.uniq(mtx)) {}
		myNewShots.push_back(ms);
		return false;		//не додавати
	}
}
//Додати постріл з класу ПОСТРІЛ в ГРАВЕЦЬ--Нові постріли
bool Player::Add_Shot(myShot ms, int** mtx)
{
	if (ms.valid(mtx) == true)
	{
		ms.shres = ShotRes::New;
		if (ms.logic(mtx)) {}
		else if (ms.uniq(mtx)) {}
		myNewShots.push_back(ms);
		return true;
	}
	else
	{
	/*	ms.shres = ShotRes::TryAgain;
		if (ms.logic(mtx)) {}
		else if (ms.uniq(mtx)) {}
		myNewShots.push_back(ms);*/
		return false;
		//не додавати
	}
}
//Додати постріл з СТРОКИ в ГРАВЕЦЬ--Нові постріли
bool Player::Add_Shot(string val, int** mtx)
{
	myShot ms(val);
	return Add_Shot(ms, mtx);
}
//Прийняти РЕЗУЛЬТАТИ пострілів в ГРАВЕЦЬ- НОВІ постріли
void Player::Apply_ShotsRes(string ss, int** mtx)
{
	// визначаємо СЕПАРАТОР для кількох пострілів
	const char separator = ',';
	//Визначаємо динамічний масив для збереження кількох пострілів
	std::vector<string> outputArray;
	//Створюємо stream з string
	std::stringstream streamData(ss);
	//Строкова змінна для збереження після розподілу строки пострілів
	std::string val;
	int c = 0;
	//Цикл заповнення динамічного масиву з строки пострілів
	while (getline(streamData, val, separator) && c < this->cnt_shots) {
		outputArray.push_back(val); c++;
	}
	//Передача масиву нових пострілів в клас Ігрок
	int d = 0;
	int vs = myNewShots.size();
	for (auto& val : outputArray) {
		if (d < vs)
		{
			try
			{
				ShotRes shotres = static_cast<ShotRes>(stoi(val));
				int mtxpt = 0;
				myNewShots[d].shres = shotres;
				switch (shotres)
				{
				case ShotRes::New:
					mtxpt = (int)PointVal::sea;
					break;
				case ShotRes::Wound:
					mtxpt = (int)PointVal::waswound;
					break;
				case ShotRes::Kill:
					mtxpt = (int)PointVal::waskill;
					break;
				case ShotRes::Miss:
					mtxpt = (int)PointVal::wasmiss;
					break;
				case ShotRes::TryAgain:
					mtxpt = mtx[myNewShots[d].pnt.y][myNewShots[d].pnt.x];//(int)PointVal::reserv3;
					break;
				case ShotRes::TryShot: break;
				default:
					break;
				}
				mtx[myNewShots[d].pnt.y][myNewShots[d].pnt.x] = mtxpt;
				d++;
			}
			catch (exception ex) { cout << "Error: " << ex.what() << endl; }
		}
	}

}
//Додати ПОСТРІЛИ в ГРАВЕЦЬ з строки пострілів ?
vector<myShot> Player::Add_Shots(string ss, int** mtx) {
	//	vector<myShot> myNS;
	this->strMyShots="";
	transform(ss.begin(), ss.end(), ss.begin(), ::toupper);
	// визначаємо СЕПАРАТОР для кількох пострілів
	const char separator = ',';
	//Визначаємо динамічний масив для збереження кількох пострілів
	std::vector<string> outputArray;
	//Створюємо stream з string
	std::stringstream streamData(ss);
	//Строкова змінна для збереження після розподілу строки пострілів
	std::string val;
	int c = 0;
	//Цикл заповнення динамічного масиву з строки пострілів
	while (getline(streamData, val, separator) && c < this->cnt_shots) {
		outputArray.push_back(val); c++;
	}
	//Передача масиву нових пострілів в клас Ігрок
	for (auto& val : outputArray) {		Add_Shot(val, mtx); this->strMyShots += val + ","; }
	return this->myNewShots;
};

vector<myShot> Player::Add_AllShots(string ss, int **mtx) {
	//	vector<myShot> myNS;
	this->strMyShots = "";
	transform(ss.begin(), ss.end(), ss.begin(), ::toupper);
	// визначаємо СЕПАРАТОР для кількох пострілів
	const char separator = ',';
	//Визначаємо динамічний масив для збереження кількох пострілів
	std::vector<string> outputArray;
	//Створюємо stream з string
	std::stringstream streamData(ss);
	//Строкова змінна для збереження після розподілу строки пострілів
	std::string val;
	int c = 0;
	//Цикл заповнення динамічного масиву з строки пострілів
	while (getline(streamData, val, separator) && c < this->cnt_shots) {
		outputArray.push_back(val); c++;
	}
	//Передача масиву нових пострілів в клас Ігрок
	for (auto& val : outputArray) {
		Add_AllShot(val,mtx);
		this->strMyShots += val + ",";
	}
	return this->myNewShots;
};
#pragma endregion

#pragma region Function_work_with_file - Функції для роботи з файлами
//Отримати інформацію з файлу fname та вивести на екран
void Get_Info_From_File(string fname)
{
	string line;
	ifstream f(fname);
	if (f.is_open())
	{
		int l = 0;
		while (getline(f, line))
		{
			std::cout << line << endl;
			if (l++ > 50) { system("pause"); system("cls"); l = 0; }	else l++;
		}
		f.close();
	}
	else
	{
		std::cout << "Відсутній файл: " << fname << endl;
	}
	system("pause");
}

void SaveScore(Player *pl1, Player* pl2) {
	fstream fs("SB_Score.txt", ios::app); //, ios::app
	fs.clear();
	time_t _time = time(NULL);
	char buf[80]; 
	struct tm *tstruct = localtime(&_time);
	strftime(buf, sizeof(buf), "%d.%m.%Y %X", tstruct);
	if (fs.is_open())
	{
		fs << buf << " ";
		fs << Player_stan[(static_cast<int>(pl1->ps))] << ": ";
		fs << pl1->Name << "(" << Player_type[(static_cast<int>(pl1->pm))] << ")"; //<< ":" << Player_level[(static_cast<int>(pl1->level))] 
		fs << " постр=" << pl1->shot << " влуч.=" << pl1->kill;
		fs << " *VS* ";
		fs << Player_stan[(static_cast<int>(pl2->ps))] << ": ";
		fs << pl2->Name << "(" << Player_type[(static_cast<int>(pl2->pm))] << ")"; //<< ":" << Player_level[(static_cast<int>(pl2->level))]
		fs << " постр=" << pl2->shot << " влуч." << pl2->kill;
		fs << " [" << vGame_Mode[(int)_Game_Mode] << " / " << vGame_Place[(int)_Game_Place] << "] ";
		fs << endl;
		fs.close();
	}

};
void ShowScore() { Get_Info_From_File("SB_score.txt"); };

#pragma endregion

#pragma region SeaBattle_Menu - Функції МЕНЮ
//Головне меню програми
int M_MainMenu()
{
	int retCode = -1;
	cout << "\n\tГоловне МЕНЮ\n\n";
	cout << "1. Інформація\n" ;
	cout << "2. Налаштування\n";
	cout << "3. Гра\n" ;
	cout << "4. Результати ігор\n";
	cout << "\n0. Вихід\n";
	cout << "Ваш вибір: ";
	cin >> retCode;
	return retCode;
}
//Інформаційне меню
void M_Info()
{
	int M_chs = -1;
	while (M_chs != 0)
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tІнформація\n\n";
		cout << "1. Правила гри\n";
		cout << "2. Умови курсової\n";
		cout << "3. Про розробника\n";
		cout << "\n0. Вихід\n";
		cout << "Ваш вибір: ";
		cin >> M_chs;
		system("cls");
		switch (M_chs)
		{
		case 1: {Get_Info_From_File("sb_info_Rule.txt");	}	break;
		case 2: {Get_Info_From_File("sb_info_Task.txt");	}	break;
		case 3: {Get_Info_From_File("sb_info_Autor.txt");	}	break;
		default:												break;
		}
	}
}
//Меню налаштування
void M_Setting()
{
	int M_chs = -1;
	while (M_chs != 0)
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tНалаштування програми\n\n";
		cout << "1. Розмір ігрового поля\n";
		cout << "2. Кількість пострілів\n";
		cout << "3. Ім'я, Тип гравця\n";
		cout << "4. Режим гри\n";
		cout << "5. Відключити мережу" << endl;
		cout << "6. Обмін даними" << endl;
		cout << "7. Тести\n";
		cout << "\n0. Вихід\n";
		cout << "Ваш вибір: ";
		cin >> M_chs;
		switch (M_chs)
		{
		case 1: {	CHM_SettingSize();		}break;
		case 2: {	CHM_SettingShot();		}break;
		case 3: {	M_SettingPlayer();		}break;
		case 4: {	CHM_SettingGameMode(false);	}break;
		case 5: {
			if (_Game_Mode == enGame_Mode::net)
			{
				if (_Game_Place == enGame_Place::server) StopServer();
				else if (_Game_Place == enGame_Place::client) StopClient();
			}
		}
		case 6: {
			if (_Game_Mode == enGame_Mode::net)
			{
				if (_Game_Place == enGame_Place::server) {
					system("cls");
					Show_MainHead(); RunServer(); ChatSC();
				}
				else {
					system("cls");
					Show_MainHead(); RunClient(); ChatSC();
				}
			}
			break;
		}
		case 7: {	Test_SB();}
			  break;
		default:			break;
		}
	}
}
//Меню Налаштування гравців
void M_SettingPlayer()
{
	bool cont_player = true;
	while (cont_player)
	{
		system("cls");
		Show_MainHead();
		int nameForPlay = 0;
		cout << "\n\tНалаштування гравців\n\n";
		cout << "Ім'я гравця 1: " << pl1->Name << " (" << Player_type[(static_cast<int>(pl1->pm))] << ")" <<
			" [" << Player_level[(static_cast<int>(pl1->level))] << "]\n";
		cout << "Ім'я гравця 2: " << pl2->Name << " (" << Player_type[(static_cast<int>(pl2->pm))] << ")" <<
			" [" << Player_level[(static_cast<int>(pl2->level))] << "]\n";
		cout << "\n0.Вихід\n";
		cout << "Якого граця змінити ?: ";
		cin >> nameForPlay;
		switch (nameForPlay)
		{
		case 1: { CHM_SettingPlayer(1, pl1);	} break;
		case 2: { CHM_SettingPlayer(2, pl2);	} break;
		case 0: { cont_player = false; } break;
		default:	cont_player = false; break;
		}
	}
}
#pragma endregion

#pragma region SeaBattle_ChangeSetting - Функції МЕНЮ по зміні налаштувань
//Налаштування програми: режим гри / ігрового поля
void CHM_SettingGameMode(bool mode=false) 
{
	//Режим гри _Game_Mode
	int _gm = -1;
	int _gp = -1;
	int _plClient = -1;
	while (_gm != 0)
	//do 
	{
		system("cls");
		Show_MainHead();
		cout << "\n\tНалаштування програми РЕЖИМ гри. Поточне значення: " << vGame_Mode[(int)_Game_Mode] << endl << endl;
		cout << "1. Локальна гра" << endl;
		cout << "2. Мережева гра" << endl;
		cout << "\n0. Вихід" << endl;
		cout << "Ваш вибір: ";
		cin >> _gm;
		switch (_gm)
		{
		case 1: { _Game_Mode = enGame_Mode::loc; _Game_Place = enGame_Place::comp; _gm = (mode == true) ? 0 : _gm; break; }
		case 2: 
		{ 
			while(_gp!=0)
			{
				_Game_Mode = enGame_Mode::net;
				cout << "\tНалаштування програми РЕЖИМ РОБОЧОГО МІСЦЯ. Поточне значення: " << vGame_Place[(int)_Game_Place] << endl << endl;
				cout << "1. Сервер" << endl;
				cout << "2. Клієнт" << endl << endl;
				cout << "0. Вихід" << endl;
				cout << "\nВаш вибір: ";
				cin >> _gp;
				switch (_gp)
				{
				case 1: {	_Game_Place = enGame_Place::server;	
					while (_plClient != 0)
					{
						cout << "\tВиберіть гравця, який буде віддаленим клієнтом. Поточне значення: " << _plClient << endl << endl;
						cout << "1. Гравець: " << pl1->Name << " (" << Player_type[(static_cast<int>(pl1->pm))] << ")" <<
							" [" << Player_level[(static_cast<int>(pl1->level))] << "]" << endl;
						cout << "2. Гравець: " << pl2->Name << " (" << Player_type[(static_cast<int>(pl2->pm))] << ")" <<
							" [" << Player_level[(static_cast<int>(pl2->level))] << "]" << endl << endl;
						cout << "0. Вихід" << endl;
						cout << "Ваш вибір: ";				cin >> _plClient;
						switch (_plClient)
						{
						case 1: {pl1->pm = Player_Mode::client;					_plClient = (mode == true) ? 0 : _plClient; break; }
						case 2: {pl2->pm = Player_Mode::client;					_plClient = (mode == true) ? 0 : _plClient; break; }
						case 0: break;
						default: break;
						}
					}
					system("cls");
					Show_MainHead();
					RunServer(); //ChatSC();
					_gp = (mode == true) ? 0 : _gp;
					break; }
				case 2: {	_Game_Place = enGame_Place::client;	
					system("cls");
					Show_MainHead(); 
					RunClient(); //ChatSC(); 
					_gp = (mode == true) ? 0 : _gp;
					break; }
				case 0: {break;}
				}
				_gm = (mode == true) ? 0 : _gm;	break;
			}
			break;
		}
		case 0: { break; }
		default: { break; }
		}
	}
}
//Зміна розміру екрану в залежності від розміру ігрового поля
void SetScrSize() {
	scrsize = max(90,(sbf * 3 + 4) * 2 + 4);
	string s1 = "mode con cols=", s2 = " lines=50";
	s1 = s1 + to_string(scrsize) + s2;
	system(s1.c_str());
}
//Налаштування програми: розмір ігрового поля
void CHM_SettingSize()
{
	bool cont = true;
	int x;
	while (cont)
	{
		cout << "Вкажіть розмір ігрового поля: ([10;20]) (поточний= " << sbf << " ): ";
		cin >> x;
		if (x > 20 || x < 10) { cout << "Помилка!\n"; }
		else {	cont = false;	sbf = x; SetScrSize();}
	}
}
//Налаштування програми: кількість дозволений пострілів в ЗАЛПІ
void CHM_SettingShot()
{
	bool cont = true;
	int y;
	while (cont)
	{
		cout << "Вкажіть кількість пострілів: ([1;5]) (поточний= " << cnt_shot << " ): ";
		cin >> y;
		if (y > 5 || y < 1)	{ cout << "Помилка!\n"; }
		else {	cont = false;	cnt_shot = y;		}
	}
}
//Введення типу мережевого гравця
void CHM_NetClientPlayer(int i, Player* pl)
{
	string newName = pl->Name; int pmode = 0; int plevel = 0;
	cout << "\nІм'я гравця: " << newName << endl;
	cout << "\nТип гравця " << newName << " (0-комп'ютер, 1-людина): "; cin >> pmode;
	if (pmode == 0) {
		cout << "\nРівень складності " << newName << " (0-новачок, 1-профі): "; cin >> plevel;
	}
	switch (pmode)
	{
	case 0: { pl->pm = Player_Mode::comp;
		switch (plevel)
		{
		case 0: pl->level = Player_Level::level0; break;
		case 1: pl->level = Player_Level::level1; break;
		}
		break; }
	case 1: { pl->pm = Player_Mode::human; break; }
	default: { break; }
	}

}
//Введення імені, типу гравця
void CHM_SettingPlayer(int i, Player* pl)
{
	string newName = ""; int pmode = 0; int plevel = 0;
	newName = pl->Name;
	cout << "\nНове ім'я гравця: " << i << ": ";	cin >> newName;
	cout << "\nТип гравця " << newName << " (0-комп'ютер, 1-людина, 2-мережевий клієнт): "; cin >> pmode;
	if (pmode == 0) {
		cout << "\nРівень складності " << newName << " (0-новачок, 1-профі): "; cin >> plevel;
	}
	pl->Name = newName;
	switch (pmode)
	{
	case 0: { pl->pm = Player_Mode::comp;
		switch (plevel)
		{
		case 0: pl->level = Player_Level::level0; break;
		case 1: pl->level = Player_Level::level1; break;
		}
		break; }
	case 1: { pl->pm = Player_Mode::human; break; }
	case 2: { pl->pm = Player_Mode::client; break; }
	default: { pl->pm = Player_Mode::comp; break; }
	}
}
#pragma endregion

#pragma region SeaBattle_Show_Functions - Функції відображення ігрового поля
//Головна строка
void Show_MainHead()
{
	int sz = scrsize / 2;
	gotoxy(15, 0);	SetColor();	cout << string(5, '*'); SetColor(LightBlue); cout << "Sea Battle"; SetColor();
	cout << " [ " << vGame_Mode[(int)_Game_Mode] << " / " << vGame_Place[(int)_Game_Place] << " ] ";
	if (_Game_Place == enGame_Place::server) {	cout << GetServerName();	}
	else if (_Game_Place == enGame_Place::client) {	cout << GetClientName();	}
	cout << string(5, '*');
	gotoxy(0, 1); 	SetColor(ColorPlayerStat[(static_cast<int>(pl1->ps))]);
	cout << Player_stan[(static_cast<int>(pl1->ps))] << ": ";	SetColor();
	SetColor(LightGreen); 	cout << pl1->Name; 	SetColor();
	cout << "(" << Player_type[(static_cast<int>(pl1->pm))]
		<< ":" << Player_level[(static_cast<int>(pl1->level))] << ")";
	gotoxy(sz, 1);	SetColor(ColorPlayerStat[(static_cast<int>(pl2->ps))]);
	cout << Player_stan[(static_cast<int>(pl2->ps))] << ": ";	SetColor();
	SetColor(Yellow); 	cout << pl2->Name; 	SetColor();
	cout << "(" << Player_type[(static_cast<int>(pl2->pm))]
		<< ":" << Player_level[(static_cast<int>(pl2->level))] << ")";
	SetColor();
	cout << endl;
}
//Відображення 1-го ігрового поля - стара функція
void Show(int** mtx, int k, Player* pl)
{
	cout << "Гравець " << k << ": " << pl->Name << endl << "Постріл: " << pl->shot << " Влучень: " << pl->kill << endl;
	cout << "   ";
	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65); }
	cout << endl;
	for (int i = 0; i < sbf; i++)
	{
		cout << setw(2) << i + 1 << " ";
		for (int j = 0; j < sbf; j++)
		{
			switch (mtx[j][i])
			{
			case 0: { cout << setw(2) << "-";			break; }
			case 8: { cout << setw(2) << (char)(183);	break; }
			case 1:
			case 2:
			case 3:
			case 4: {cout << setw(2) << mtx[j][i]; break; }
			case 6: {cout << setw(2) << "/"; break; }
			case 7: {cout << setw(2) << "+"; break; }
			case 9: {SetColor(LightRed); cout << setw(2) << "X"; break; }
			default:				break;
			};
		}
		cout << endl;
	}
}
//Відображення точки на ігровому полі
char ShowPoint(int val, bool hide = false)
{
	char RetVal = '\0';
	switch (val)
	{
	case 0: RetVal = (hide)?' ':'-'; break;
	case 8: RetVal = (hide) ? ' ' : (char)(183); break;
	case 1:
	case 2:
	case 3:
	case 4: RetVal = (hide) ? ' ' : val + 48; break;
	case 5: SetColor(LightRed, Blue); RetVal = 'x'; break;
	case 6: SetColor(Black, LightGray); RetVal = '/'; break;
	case 7: SetColor(Black, LightGray); RetVal = (hide) ? ' ' : '+'; break;
	case 9: SetColor(LightRed, Blue); RetVal = 'X';   break;
	default:
		break;
	}; return RetVal;
}
//Відображення двох ігрових полей
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2)
{
	bool hideMtx1 = true;
	bool hideMtx2 = true;
	if (_Game_Mode == enGame_Mode::loc)
	{
		if (pl1->pm == Player_Mode::human && pl2->pm == Player_Mode::comp) { hideMtx1 = false; hideMtx2 = true;}
		else if (pl1->pm == Player_Mode::comp && pl2->pm == Player_Mode::human) { hideMtx1 = true; hideMtx2 = false; }
	}
	else if (_Game_Mode == enGame_Mode::net)
	{
		if ((pl1->pm == Player_Mode::human || pl1->pm == Player_Mode::comp) && pl2->pm == Player_Mode::client) { hideMtx1 = false; hideMtx2 = true; }
		else if ((pl2->pm == Player_Mode::human || pl2->pm == Player_Mode::comp) && pl1->pm == Player_Mode::client) { hideMtx1 = true; hideMtx2 = false; }
	}
	int srcs = scrsize / 2; 
	SetColor(Blue);	Show_MainHead();	SetColor();
	gotoxy(0, 2);	cout << "Постріл: " << setw(2) << pl1->shot << " Влучень: " << setw(2) << pl1->kill;
	gotoxy(srcs, 2);	cout << "Постріл: " << setw(2) << pl2->shot << " Влучень: " << setw(2) << pl2->kill << endl;
	gotoxy(3, 4);	SetColor(Green);	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65) << " "; }
	gotoxy(srcs + 3, 4);	SetColor(Green);	for (int i = 0; i < sbf; i++) { cout << setw(2) << (char)(i + 65) << " "; }
	cout << endl;
	SetColor();
	for (int i = 0; i < sbf; i++)
	{
		gotoxy(0, 5+i);	SetColor(Green); cout << setw(2) << i + 1 << " "; SetColor();
		for (int j = 0; j < sbf; j++) { SetColor(White); cout << setw(2) << ShowPoint(mtx1[i][j], hideMtx1) << " "; }
		gotoxy(srcs, 5+i);SetColor(Green); cout << setw(2) << i + 1 << " "; SetColor();
		for (int j = 0; j < sbf; j++) { SetColor(White); cout << setw(2) << ShowPoint(mtx2[i][j], hideMtx2) << " "; }
		cout << endl;
	}
	SetColor();
	cout << endl << endl;
	gotoxy(0, 10 + sbf);	cout << "Ходи гравця " << pl1->Name << ": ";
	gotoxy(srcs, 10 + sbf);	cout << "Ходи гравця " << pl2->Name << ": ";	cout << endl;	//for (auto& shot : pl1->ShotResults)
	int l = 0;
	int c1 = 0, c2 = 0;
	int sizev1 = pl1->myShots.size();
	int sizev2 = pl2->myShots.size();
	for (int f = 0; f < max(sizev1, sizev2); f++)
	{
		if (f < sizev1 && c1 < srcs-5 && sizev1>0) {
			gotoxy(c1, 11 + sbf + l);
			SetColor(ColorResult[(int)pl1->myShots[f].shres]);
			cout << setw(2) << (char)(pl1->myShots[f].pnt.x + 65) << pl1->myShots[f].pnt.y + 1 << "=" << Shot_Result_Short[(int)pl1->myShots[f].shres] << "";
			c1=c1+6;
		}
		SetColor();
		if (f < sizev2 && c2 < srcs-5 && sizev2>0) {
			gotoxy(srcs + c2, 11 + sbf + l);
			SetColor(ColorResult[(int)pl2->myShots[f].shres]);
			cout << setw(2) << (char)(pl2->myShots[f].pnt.x + 65) << pl2->myShots[f].pnt.y + 1 << "=" << Shot_Result_Short[(int)pl2->myShots[f].shres] << "";
			c2=c2+6;
		}
		SetColor();
		if (c2 >= srcs-4 || c1 >= srcs-4) { cout << endl; l++; c1 = 0; c2 = 0; }
	}
}
#pragma endregion

#pragma region SeaBattle_Setting_Point - Функції з точками ігрового поля
//Допустимі координати
bool Valid_Coord(int x, int y)
{
	if (x < 0 || y < 0 || x > sbf - 1 || y > sbf - 1) return false;
	else return true;
}
//Допустимі координати 2
bool Valid_Coord(int x, int y, int **mtx)
{
	if (Valid_Coord(x, y))
	{
		int val = mtx[y][x];
		if (val != (int)PointVal::wound && 
			val != (int)PointVal::kill && 
			val != (int)PointVal::reserv2 && 
			val != (int)PointVal::miss && 
			val != (int)PointVal::reserv3) return true; // без 6
		else return false;
	}
	else return false;
}
//Резурвування полів навколо точки
void Rezerv(int x1, int y1, int x2, int y2, int** mtx, int marker)
{
	for (int x = x1; x <= x2; x++) // проходимо по всіх точках від х1 до х2
	{
		for (int y = y1; y <= y2; y++) // проходимо по всіх точках від у1 до у2
		{
			if (Valid_Coord(x - 1, y - 1) && mtx[y - 1][x - 1] == 0) mtx[y - 1][x - 1] = marker;
			if (Valid_Coord(x - 1, y) && mtx[y][x - 1] == 0)         mtx[y][x - 1] = marker;
			if (Valid_Coord(x - 1, y + 1) && mtx[y + 1][x - 1] == 0) mtx[y + 1][x - 1] = marker;
			if (Valid_Coord(x, y - 1) && mtx[y - 1][x] == 0)         mtx[y - 1][x] = marker;
			if (Valid_Coord(x, y + 1) && mtx[y + 1][x] == 0)         mtx[y + 1][x] = marker;
			if (Valid_Coord(x + 1, y - 1) && mtx[y - 1][x + 1] == 0) mtx[y - 1][x + 1] = marker;
			if (Valid_Coord(x + 1, y) && mtx[y][x + 1] == 0)         mtx[y][x + 1] = marker;
			if (Valid_Coord(x + 1, y + 1) && mtx[y + 1][x + 1] == 0) mtx[y + 1][x + 1] = marker;
		}
	}
}
//Резервування полів навколо вдалого влучення
void Rezerv_Shot(int x, int y, int** mtx, int marker, bool alldesk=false)
{
	//угли по діагоналі
	if (Valid_Coord(x - 1, y - 1) && (mtx[y - 1][x - 1] == (int)PointVal::reserv || mtx[y - 1][x - 1] == (int)PointVal::sea)) mtx[y - 1][x - 1] = marker;
	if (Valid_Coord(x - 1, y + 1) && (mtx[y + 1][x - 1] == (int)PointVal::reserv || mtx[y + 1][x - 1] == (int)PointVal::sea)) mtx[y + 1][x - 1] = marker;
	if (Valid_Coord(x + 1, y - 1) && (mtx[y - 1][x + 1] == (int)PointVal::reserv || mtx[y - 1][x + 1] == (int)PointVal::sea)) mtx[y - 1][x + 1] = marker;
	if (Valid_Coord(x + 1, y + 1) && (mtx[y + 1][x + 1] == (int)PointVal::reserv || mtx[y + 1][x + 1] == (int)PointVal::sea)) mtx[y + 1][x + 1] = marker;
	if (alldesk == true) // крестом
	{
		if (Valid_Coord(x, y - 1) && (mtx[y - 1][x] == (int)PointVal::reserv|| mtx[y - 1][x] == (int)PointVal::sea)) mtx[y - 1][x] = marker;
		if (Valid_Coord(x, y + 1) && (mtx[y + 1][x] == (int)PointVal::reserv|| mtx[y + 1][x] == (int)PointVal::sea)) mtx[y + 1][x] = marker;
		if (Valid_Coord(x-1, y) && (mtx[y][x-1] == (int)PointVal::reserv || mtx[y][x - 1] == (int)PointVal::sea)) mtx[y ][x - 1] = marker;
		if (Valid_Coord(x+1, y) && (mtx[y][x+1] == (int)PointVal::reserv || mtx[y][x + 1] == (int)PointVal::sea)) mtx[y ][x + 1] = marker;
	}
}
//Точка є кораблем?
bool Is_Ship(int x, int y, int** mtx)
{
	if (mtx[y][x] == (int)PointVal::ship1 ||
		mtx[y][x] == (int)PointVal::ship2 ||
		mtx[y][x] == (int)PointVal::ship3 ||
		mtx[y][x] == (int)PointVal::ship4) return true;
	else return false;
}
//Точка має сусідні точки-кораблі/палуби
bool Has_Neib(int x, int y, int** mtx)
{
	int cnt = 0;
	if (Valid_Coord(x - 1, y) && Is_Ship(x - 1, y, mtx)) cnt++;
	if (Valid_Coord(x, y - 1) && Is_Ship(x, y - 1, mtx)) cnt++;
	if (Valid_Coord(x, y + 1) && Is_Ship(x, y + 1, mtx)) cnt++;
	if (Valid_Coord(x + 1, y) && Is_Ship(x + 1, y, mtx)) cnt++;
	if (cnt > 0) return true;
	else return false;
}

#pragma endregion

#pragma region SeaBattle_Fill_Battle_Fields - заповнення ігрового поля МОРЕМ або КОРАБЛЯМИ
//Заповнення ігрового поля морем
void ResetBattleField(int** mtx, int param)
{
	for (int i = 0; i < param; i++)
	{
		for (int j = 0; j < param; j++)
		{
			mtx[i][j] = (int)PointVal::sea;
		}
	}
}
//Заповнення ігрового поля кораблями
void SetShipToBattleField(Player *pl, int** mtx, int sbf)
{
	bool Ok = false;
	ResetBattleField(mtx,sbf);
	pl->myEscadra->Ships.clear();
	for (int e = 9; e >= 0; e--)
	{
		int Complite = 0, z = 0;
		while (Ok == false)
		{
			int x, y;
			x = rand() % sbf;
			y = rand() % sbf;
			int v = rand() % 2, h = 1 - v;
			for (int ss = 0; ss < Escadr[e][0]; ss++)
			{
				if (x + ss * h < sbf && y + ss * v < sbf)
				{
					if (mtx[y + ss * v][x + ss * h] == (int)PointVal::sea)
					{
						Complite++;
					}
				}
			}
			int x2 = 0, y2 = 0;
			if (Complite == Escadr[e][0])
			{
				Ship* ship = new Ship();
				ship->orient = (h == 1) ? 0 : 1;
				for (int ss = 0; ss < Escadr[e][0]; ss++)
				{
					if (x + ss * h < sbf && y + ss * v < sbf)
					{
						if (mtx[y + ss * v][x + ss * h] == (int)PointVal::sea)
						{
							mtx[y + (ss * v)][x + (ss * h)] = Escadr[e][0];
							Ok = true;
							x2 = x + ss * h;
							y2 = y + ss * v;
							Desk* dsk = new Desk(x2, y2);
							dsk->value = Escadr[e][0];
							ship->Add(dsk);
							//delete dsk;
						}
					}
				}
				pl->myEscadra->Add(ship);
			}
			Complite = 0;
			Rezerv(x, y, x2, y2, mtx, (int)PointVal::reserv);
		}Ok = false;
		z++;
	}
}

#pragma endregion

#pragma region Chat_Server_Client - Функції для МЕРЕЖЕВОЇ гри
// Отримання повідомлення від мережі клієнта або сервера
void WaitSMS(string strmsg, Player* pl, int** mtx)
{
	int spin = 0, c = 0;
	bool cont = true;
	string msg;
	clearMsgPlace(8 + sbf, 7 + sbf); 
	cout << "\r"<<strmsg << string(scrsize - strmsg.length(), ' ');
	cout.flush();
	while (cont)
	{
		spin = Spiner(spin);
		msg = (_Game_Place == enGame_Place::client) ? ReceiveMessageFromServer() : ReceiveMessageFromClient();
		cont = ApplyCommandFromMessage(msg, pl, mtx);
	}
}
// Циклічний чат між сервером та клієнтом
void Game_ChatSC1() {
	bool cont = true;
	int step = 0;
	while (cont)
	{
		if (_Game_Mode == enGame_Mode::net)
		{
			switch (_Game_Place)
			{
			case enGame_Place::server:
			{
				SC_Comands newcom;
				newcom.mySetMessage(SC_Com_type::setPl1Name, pl1->Name);
				newcom.mySetMessage(SC_Com_type::setPl2Name, pl2->Name);
				newcom.mySetMessage(SC_Com_type::setPl1type, to_string(int(pl2->pm)));
				newcom.mySetMessage(SC_Com_type::setPl2type, to_string(int(pl1->pm)));
				newcom.mySetMessage(SC_Com_type::setPl1level, to_string(int(pl1->level)));
				newcom.mySetMessage(SC_Com_type::setPl2level, to_string(int(pl2->level)));
				string msg;
				for (auto& s : newcom.SCCs) { msg += s.message + newcom.separator; }
				SendMessageToClient(msg);
				msg = ReceiveMessageFromClient();
				cont = false;
				break;
			}
			case enGame_Place::client:
			{
				cont = ApplyCommandFromMessage(ReceiveMessageFromServer(),nullptr,nullptr);
				SendSMS(SC_Com_type::ans_ok, "Принято");
				cont = false;
				break;
			}
			default: break;
			}
		}
		else { cont = false; }
	}
}
//Ігровий чат - не використовується
void Game_ChatSC(int place) {
	bool cont = true;
	int spin = 0;
	while (cont)
	{
		if (_Game_Mode == enGame_Mode::loc)
		{
			if (place == 1) { Shot(pl1, mtx2, pl2, mtx1); cont = false; }
			else { Shot(pl2, mtx1, pl1, mtx2); cont = false; }
		}
		else if (_Game_Mode == enGame_Mode::net)
		{
			switch (_Game_Place)
			{
			case enGame_Place::server:
			{
				if (place == 1) { Shot(pl1, mtx2, pl2, mtx1); cont = false; }
				else { Shot(pl2, mtx1, pl1, mtx2); cont = false; }
				break;
			}
			case enGame_Place::client:
			{
				gotoxy(0, 7 + sbf);
				std::cout << "Чекаємо на команду сервера ";
				while (true) {
					spin = Spiner(spin);
					cont = ApplyCommandFromMessage(ReceiveMessageFromServer(),nullptr,nullptr);
				}

				break;
			}
			default: cont = false;  break;
			}
		}
		else { cont = false; }
	}
}
// Відправити повідомлення серверу-клієнту
void SendSMS(SC_Com_type type, string msg)
{
	switch (_Game_Place)
	{
	case enGame_Place::server:
	{
		SendMessageToClient(to_string((int)type) + "&" + msg);
	} break;
	case enGame_Place::client:
	{
		SendMessageToServer(to_string((int)type) + "&" + msg);
	} break;
	default: break;
	}
}
//Прийняти та опрацювати повідомлення від мережі
bool ApplyCommandFromMessage(string command, Player* pl = nullptr,int**mtx=nullptr )
{
	string shotres;
	bool cont = true;
	SC_Comands sccs;
	sccs.myGetMessage(command);
	for (auto& cmd : sccs.SCCs) { //cout << "Тип сообщения: " << cmd.type << " команда: " << cmd.command << endl; 
		switch ((SC_Com_type)cmd.type) {
		case SC_Com_type::setCntShot: cnt_shot = stoi(cmd.command); break;
		case SC_Com_type::setPl1Name: pl1->Name = cmd.command; break;
		case SC_Com_type::setPl2Name: pl2->Name = cmd.command; break;
		case SC_Com_type::setPl1type: {
			pl1->pm = static_cast<Player_Mode>(stoi(cmd.command)); break; }
		case SC_Com_type::setPl2type: pl2->pm = static_cast<Player_Mode>(stoi(cmd.command)); break;
		case SC_Com_type::setPl1level: pl1->level = static_cast<Player_Level>(stoi(cmd.command)); break;
		case SC_Com_type::setPl2level: pl2->level = static_cast<Player_Level>(stoi(cmd.command)); break;
		case SC_Com_type::runGame: M_Game(); break;
		case SC_Com_type::runExitLoop: cont = false; break;
		case SC_Com_type::clearSMS: 	for (int f = 7 + sbf; f <= 4; f++) { gotoxy(0, f); cout << string(100, ' ') << endl; } break; // оситити екран під смс
		case SC_Com_type::sendSMS: cout << cmd.command;  break; // вивести повідомлення на екран
		case SC_Com_type::ansShot:
		{
			Wait("Отримано Постріл " + cmd.command + " від гравця " + pl->Name);
			pl->strMyShots = cmd.command;
			cont = false;//виходимо з циклу чату 
		}	break; 
		case SC_Com_type::ansResult: 
		{
			Wait("Отримано РЕЗУЛЬТАТ " + cmd.command + " [" + Shot_Result[stoi(cmd.command)] + "] від " + pl->Name);
			pl->strMyShotsRes = cmd.command;
			cont = false;//виходимо з циклу чату
		}	break;
		default: break;
		}
	}
	return cont;
}
// Циклічний чат між сервером та клієнтом
void ChatSC() {
	bool cont = true;
	while (cont)
	{
		system("cls");
		Show_MainHead();
		if (_Game_Mode == enGame_Mode::net)
		{
			switch (_Game_Place)
			{
			case enGame_Place::server:
			{
				int typ;
				string msg;
				cout << "Тип сообщения (0-вихід): "; cin >> typ;
				switch (typ)
				{
				case 0: cont = false;					break;
				default:cout << " команда: "; cin >> msg; SendMessageToClient(typ + "&" + msg);
					break;
				}
				break;
			}
			case enGame_Place::client:
			{
				cont = ApplyCommandFromMessage(ReceiveMessageFromServer());
				break;
			}
			default: cont = false;  break;
			}
		}
		else { cont = false; }
	}
}
#pragma endregion

#pragma region SeaBattle_Shot_Function
// Отримати повідомлення від людини
string GetUserShot(Player* pl, int **mtx)
{
	int p = 7 + sbf;
	bool repeat = false;
	do
	{
		repeat = false;
		string _Shot = "";
		pl->myNewShots.clear();
		clearMsgPlace(p,7+sbf);
		_Shot = ascForShot(pl);
		if (_Shot == "0")
		{
			repeat = false;
			pl->myShotsRes = ShotRes::Exit;
		}
		else {
			{
				for (auto& s : pl->Add_AllShots(_Shot, mtx))
				{
					if (s.valid(mtx) == true)
					{
						repeat = (repeat) ? true : false;
					}
					else
					{
						p = ShowErrorShot(pl, s);
						repeat = true;
					}
				}
			}
		}
	} while (repeat);
	return pl->strMyNewShots();
}

//Згенерувати постріл для комп'ютера
string GetCompShot(Player* pl, int** mtx)
{
	bool repeat = true; int i = 0;
	//Point pt = pl->myNewShot->pnt;
	while (repeat && i<1000)
	{
		//i++; //противозависатель
		switch (pl->level)
		{
		case Player_Level::level0:
			pl->myNewShot.pnt.x = rand() % sbf;
			pl->myNewShot.pnt.y = rand() % sbf;
			break;
		case Player_Level::level1:
			if (pl->CompHit == false)
			{
				pl->myNewShot.pnt.x = rand() % sbf;
				pl->myNewShot.pnt.y = rand() % sbf;
			}
			else
			{
				if (pl->SecondCompShot == false)
				{
					int i = 0;
					while (i < 5)
					{//1y--/2x--/3y++/4x++
						//pl->ShotDirection = 1 + rand() % 4; // випадковий напрямок
						++i;
						pl->ShotDirection = i;
						if (pl->ShotDirection == 1 &&
							pl->CompMiss[0] == false &&
							Valid_Coord(pl->myNewShot.pnt.x, pl->myNewShot.pnt.y - 1)) {
							pl->myNewShot.pnt.y--; break; //Стреляет выше
						}
						else if (pl->ShotDirection == 2 &&
							pl->CompMiss[1] == false &&
							Valid_Coord(pl->myNewShot.pnt.x - 1, pl->myNewShot.pnt.y)) {
							pl->myNewShot.pnt.x--; break; //Стреляет левее
						}
						else if (pl->ShotDirection == 3 &&
							pl->CompMiss[2] == false &&
							Valid_Coord(pl->myNewShot.pnt.x, pl->myNewShot.pnt.y + 1)) {
							pl->myNewShot.pnt.y++; break; //Стреляет ниже
						}
						else if (pl->ShotDirection == 4 &&
							pl->CompMiss[3] == false &&
							Valid_Coord(pl->myNewShot.pnt.x + 1, pl->myNewShot.pnt.y)) {
							pl->myNewShot.pnt.x++; break; //Стреляет правее
						}
						else
						{
							pl->CompMiss[pl->ShotDirection - 1] = true; //Если никуда не получилось выстрелить
						}
						if (pl->CompMiss[0] == true && pl->CompMiss[1] == true &&
							pl->CompMiss[2] == true && pl->CompMiss[3] == true)
						{
							break; // спроб більше немає
						}
					}
				}
				else
				{ //При втором попадании стреляет в направлении, в котором раньше стрелял
					if (pl->ShotDirection == 1 &&
						Valid_Coord(pl->myNewShot.pnt.x, pl->myNewShot.pnt.y - 1)) {
						pl->myNewShot.pnt.y--;
					}
					else if (pl->ShotDirection == 2 &&
						Valid_Coord(pl->myNewShot.pnt.x - 1, pl->myNewShot.pnt.y)) {
						pl->myNewShot.pnt.x--;
					}
					else if (pl->ShotDirection == 3 &&
						Valid_Coord(pl->myNewShot.pnt.x, pl->myNewShot.pnt.y + 1)) {
						pl->myNewShot.pnt.y++;
					}
					else if (pl->ShotDirection == 4 &&
						Valid_Coord(pl->myNewShot.pnt.x + 1, pl->myNewShot.pnt.y)) {
						pl->myNewShot.pnt.x++;
					}
					else { pl->SecondCompShot = false; }
				}
			}
			break;
		}
		if (pl->myNewShot.valid(mtx) == true) {
			repeat = false; pl->myNewShot.shres = ShotRes::New; pl->myShotsRes = ShotRes::New;
		}
		else i++;
	}
	
	return pl->myNewShot.to_txt();
}

//Функція отримання пострілів від людини, компа, клієнта мережі
string GetShot(Player* pl, int** mtx)
{
	string s_shots;	//pl->myNewShots.clear();
	// для локальної гри
	switch (pl->pm)
	{
	case Player_Mode::comp: 	// грає комп'ютер
		for (int f = 1; f <= pl->cnt_shots; f++)
		{
			s_shots += GetCompShot(pl, mtx);
			pl->myNewShots.push_back(pl->myNewShot);
		}break;
	case Player_Mode::human: 	// грає людина
		s_shots = GetUserShot(pl, mtx);
		break;
	default: break;
	}
	return s_shots;
}
// Проведення пострілу на карту
string MakeShot(Player* pl, int** mtx, Player* pl_e=nullptr)
{
	string answ;
	Point* pt;
	//Ship* ship = new Ship();
	for (auto& newS : pl->myNewShots)
	{
		pt = &newS.pnt;
		if (pl->myShotsRes != ShotRes::Exit && pt->valid())// && (newS.shres == ShotRes::New || newS.shres == ShotRes::TryAgain))
		{
			switch (mtx[pt->y][pt->x])
			{
			case (int)PointVal::sea: //промазал
			case (int)PointVal::reserv: //промазал
			case (int)PointVal::wasmiss: //промазал
				mtx[pt->y][pt->x] = (int)PointVal::miss; //промазал
				pl->shot++;
				pl->OnlyOneSR(ShotRes::Miss);
				newS.shres = ShotRes::Miss;
				pl->Add_ShotFromNew(newS);
				if (pl->pm == Player_Mode::comp && pl->level == Player_Level::level1)
				{
					pl->NextCompHit = false;

					if (pl->ShotDirection != -1) {
						pl->CompMiss[pl->ShotDirection - 1] = true;
						if ((pl->CompMiss[0] == true && pl->CompMiss[1] == true && pl->CompMiss[2] == true && pl->CompMiss[3] == true)
							|| pl->SecondCompShot == true) {
							pl->CompHit = false;
							for (int x = 0; x != 4; x++) {
								pl->CompMiss[x] = false;
							}
						}
						if (pl->SecondCompShot == false && pl->CompHit == true) {//1y--/2x--/3y++/4x++
							if (pl->ShotDirection == 1) {
								pt->y++; //Направляет координату ниже
							}
							else if (pl->ShotDirection == 2) {
								pt->x++; //Направляет координату правее
							}
							else if (pl->ShotDirection == 3) {
								pt->y--; //Направляет координату віше
							}
							else if (pl->ShotDirection == 4) {
								pt->x--; //Направляет координату левее
							}
							pl->myNewShot.pnt.x = pt->x;
							pl->myNewShot.pnt.y = pt->y;
						}
					}
					pl->SecondCompShot = false;
				}
				break;
			case (int)PointVal::ship1:
			case (int)PointVal::ship2:
			case (int)PointVal::ship3:
			case (int)PointVal::ship4:
			case (int)PointVal::waskill: //попал у клієнта
			case (int)PointVal::waswound: //поранив у клієнта
				pl->shot++;
				pl->kill++;
				if (pl->pm == Player_Mode::comp && pl->level == Player_Level::level1)
				{
					if (pl->CompHit == true) {
						pl->NextCompHit = true;
					}
					if (pl->NextCompHit == true) {
						pl->SecondCompShot = true;
					}
					pl->CompHit = true;
				}
				if (pl_e != nullptr) // для локальної таблиці бою
				{
					Ship *enemy_ship = pl_e->myEscadra->SearchShip(pt->x,pt->y);
					if (enemy_ship != nullptr)
					{
						ShipStan en_st = enemy_ship->Kill(pt->x, pt->y); // Знаходимо та вбиваємо корабель - повертаємо стан корабля
						{						
							switch (en_st)
							{
							case ShipStan::wound: 
								mtx[pt->y][pt->x] = (int)PointVal::wound;
								newS.shres = ShotRes::Wound;
								Rezerv_Shot(pt->x, pt->y, mtx, (int)PointVal::reserv2, false);
								break;
							case ShipStan::killed: 
								mtx[pt->y][pt->x] = (int)PointVal::kill;
								newS.shres = ShotRes::Kill;
								if(_Game_Mode == enGame_Mode::loc) enemy_ship->ReservAround(mtx, PointVal::reserv2);
								else Rezerv_Shot(pt->x, pt->y, mtx, (int)PointVal::reserv2, true);
								pl->New_Shot();
								break;
							default: 						
								break;
							}

						}
					}
				} 
				else	
				{
					//Для мережевої гри
					if (mtx[pt->y][pt->x] == (int)PointVal::waswound)
					{
						mtx[pt->y][pt->x] = (int)PointVal::wound;
						newS.shres = ShotRes::Wound;
					}
					else if (mtx[pt->y][pt->x] == (int)PointVal::waskill)
					{
						mtx[pt->y][pt->x] = (int)PointVal::kill;
						newS.shres = ShotRes::Kill;
						Rezerv_Shot(pt->x, pt->y, mtx, (int)PointVal::reserv2, true);
						pl->New_Shot();
					} 
					else if (Has_Neib(pt->x, pt->y, mtx)) {
						mtx[pt->y][pt->x] = (int)PointVal::wound;
						newS.shres = ShotRes::Wound;
					} 
					else {
						mtx[pt->y][pt->x] = (int)PointVal::kill;
						newS.shres = ShotRes::Kill;
						Rezerv_Shot(pt->x, pt->y, mtx, (int)PointVal::reserv2, true);
						pl->New_Shot();
					}
				}
				pl->OnlyOneSR(ShotRes::TryShot);
				Rezerv_Shot(pt->x, pt->y, mtx, (int)PointVal::reserv2,false);
				pl->Add_ShotFromNew(newS);
				break;
			case (int)PointVal::wound:		//Уже поранив
			case (int)PointVal::miss:       //Уже промазал
			case (int)PointVal::reserv2:    //Резерв диагональ 
			case (int)PointVal::kill:		//Уже убив
				newS.shres = ShotRes::TryAgain;
				pl->OnlyOneSR(ShotRes::TryAgain);
				if (pl->pm == Player_Mode::human) {
					SetColor(Red);
					cout << pl->Name << ", ви вже поцілили в цю точку, оберіть іншу!\n";
					SetColor();
					system("pause");
				}
				pl->cnt_shots = 1;
				if (pl->pm == Player_Mode::comp && pl->level == Player_Level::level1)
				{
					pl->NextCompHit = false;
					if (pl->ShotDirection != -1) {
						pl->CompMiss[pl->ShotDirection - 1] = true;
						if ((pl->CompMiss[0] == true && pl->CompMiss[1] == true && pl->CompMiss[2] == true && pl->CompMiss[3] == true)
							|| pl->SecondCompShot == true) 
						{
							pl->CompHit = false;
							for (int x = 0; x != 4; x++) {	pl->CompMiss[x] = false;}
						}
						if (pl->SecondCompShot == false && pl->CompHit == true) {//1y--/2x--/3y++/4x++
							if (pl->ShotDirection == 1) {
								pt->y++; //Направляет координату правее
							}
							else if (pl->ShotDirection == 2) {
								pt->x++; //Направляет координату ниже
							}
							else if (pl->ShotDirection == 3) {
								pt->y--; //Направляет координату левее
							}
							else if (pl->ShotDirection == 4) {
								pt->x--; //Направляет координату выше
							}
							pl->myNewShot.pnt.x = pt->x;
							pl->myNewShot.pnt.y = pt->y;
						}
					}
					pl->SecondCompShot = false;
				}
				break;                                                  //Уже попал
			default:
				break;
			};
		}
		answ += to_string((int)newS.shres) + ",";
	}
	return answ;
}

//Функція пострілів
void Shot(Player* pl, int** mtx, Player* pl_e, int** mtx_e) //, Player* pl_enemy
{
	do
	{
		string _shots;
		string _shotsRes;
		pl->myNewShots.clear();
		_shots = GetShot(pl, mtx);
		// для локальної гри
		if (_Game_Mode == enGame_Mode::loc)
		{
			switch (pl->pm)
			{
			case Player_Mode::comp: 	// грає комп'ютер
			case Player_Mode::human: 	// грає людина
				MakeShot(pl, mtx, pl_e);
				break;
			default: break;
			}
		}
		// для мережі та сторони сервера
		else if (_Game_Mode == enGame_Mode::net && (_Game_Place == enGame_Place::server || _Game_Place == enGame_Place::client))
		{
			switch (pl->pm)
			{
			case Player_Mode::comp: 	// грає комп'ютер
			case Player_Mode::human: 	// грає людина
				pl->strMyShots = _shots;
				Wait("Надсилаємо постріл " + _shots + " на клієнта " + pl_e->Name);
				SendSMS(SC_Com_type::ansShot, _shots); // посилаємо клієнту координати пострілу
				WaitSMS("Чекаємо РЕЗУЛЬТАТ від клієнта > " + pl_e->Name, pl, mtx); // чекаємо від клієнта результати пострілу
				_shotsRes = pl->strMyShotsRes;
				pl->Apply_ShotsRes(_shotsRes, mtx);
				MakeShot(pl, mtx);
				break;
			case Player_Mode::client: 	// грає клієнт //змінити на клієнта
				WaitSMS("Чекаємо постріл від клієнта >" + pl->Name, pl, mtx);
				_shots = pl->strMyShots;
				pl->Add_AllShots(_shots,mtx);//додаємо новий постріл до гравця
				_shotsRes = MakeShot(pl, mtx, pl_e); //опрацьовуємо постріл та отримуємо результат пострілу
				SendSMS(SC_Com_type::ansResult, _shotsRes); // надсилаємо результат пострілу
				Wait("Відправлено РЕЗУЛЬТАТ " + _shotsRes + " [" + Shot_Result[stoi(_shotsRes)] + "] Пострілу " + pl->strMyNewShots() + " на " + pl->Name);
				break;
			default: break;
			}
		}
	} while (pl->myShotsRes == ShotRes::TryAgain);//(pl->ps == Player_Stat::InGame && pl->myShotsRes != ShotRes::Exit);
	pl->cnt_shots= cnt_shot;
}

#pragma endregion
//Створення ігрового поля
int** MakeMtx(int sizesbf)
{
	int** mtx;
	mtx = new int* [sizesbf];	for (int i = 0; i < sizesbf; i++)	mtx[i] = new int[sizesbf]; //ігрове поле 1
	return mtx;
}
//Функція ГРИ
long M_Game()
{
	bool cont = true;
	srand(static_cast<unsigned int>(time(NULL))); // генератор довільних чисел
	mtx1= MakeMtx(sbf); //створюємо поле бою для 1 гравця
	mtx2= MakeMtx(sbf); //створюємо поле бою для 2 гравця
	Game_ChatSC1(); // початковий чат для обміну параметрами сервера з клієнтом
	if (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::client)
	{
		if(pl1->pm==Player_Mode::client) CHM_NetClientPlayer(2, pl2);
		else CHM_NetClientPlayer(1, pl1);
	}
	system("cls");	gotoxy(0, 0); cout << "Гра";
	//По режиму ГРИ вибрати генерацію ІГРОВИХ ПОЛІВ
	switch (_Game_Mode)
	{
	case enGame_Mode::loc:// ДЛЯ ЛОКЛЬНОЇ гри генерація обох полів з кораблями
		SetShipToBattleField(pl1, mtx1,sbf);
		SetShipToBattleField(pl2, mtx2,sbf);
		break;
	case enGame_Mode::net: // для мережевої гри генерація кораблів на власному полі та заповнення нулями поле супротивника (клієнта)
		switch (_Game_Place)
		{
		case enGame_Place::server:
		case enGame_Place::client:
			switch (pl1->pm)
			{
			case Player_Mode::client:
				Sleep(rand()%1000);
				srand(static_cast<unsigned int>(time(NULL))); // генератор довільних чисел
				ResetBattleField(mtx1,sbf);
				SetShipToBattleField(pl2, mtx2, sbf);
				break;
			case Player_Mode::human:
			case Player_Mode::comp:
				srand(static_cast<unsigned int>(time(NULL))); // генератор довільних чисел
				ResetBattleField(mtx2, sbf);
				SetShipToBattleField(pl1, mtx1, sbf);
				break;
			default: break;
			}
			switch (pl2->pm)
			{
			case Player_Mode::client:
				Sleep(rand() % 1000);
				srand(static_cast<unsigned int>(time(NULL))); // генератор довільних чисел
				ResetBattleField(mtx2, sbf);
				SetShipToBattleField(pl1, mtx1, sbf);
				break;
			case Player_Mode::human:
			case Player_Mode::comp:
				ResetBattleField(mtx1, sbf);
				SetShipToBattleField(pl2, mtx2, sbf);
				break;
			}
			break;
		}
		break;
	default: break;
	}
	//Почати нові ігри для ГРАВЦЯ 1,2
	pl1->New_Game();
	pl2->New_Game();
	pl1->New_Shot();
	pl2->New_Shot();
	//Початкове відображення ігрового поля
	Show2(mtx1, mtx2, pl1, pl2);
	//Головний цикл ГРИ 
	clock_t t_start=clock();
	do
	{
		//Перший постріл ГРАВЦЯ 1 - виконується ПОКИ є можливість пострілу
		do
		{		
			Shot(pl1,mtx2, pl2, mtx1);
			Show2(mtx1, mtx2, pl1, pl2);
			if (pl1->kill == WIN_CNT)
			{
				cont = false;
				pl1->ps = Player_Stat::Winner;
				pl2->ps = Player_Stat::Loser;
				break;
			}
		} while (pl1->myShotsRes == ShotRes::TryShot);
		//Перехід ХОДУ постріл ГРАВЦЯ 2 - виконується ПОКИ є можливість пострілу
		do
		{	
			Shot(pl2, mtx1,pl1, mtx2);
			Show2(mtx1, mtx2, pl1, pl2);
			if (pl2->kill == WIN_CNT)
			{
				cont = false;
				pl2->ps = Player_Stat::Winner;
				pl1->ps = Player_Stat::Loser;
				break;
			}
		} while (pl2->myShotsRes == ShotRes::TryShot);

	// виконувати доки Є можливість продовжувати або ОДИН  з гравців не ПРИПИНИВ ГРУ
	} while (cont == true && pl1->myShotsRes != ShotRes::Exit && pl2->myShotsRes != ShotRes::Exit);
	//Відображення на екрані завершення ГРИ
	clock_t t_end=clock();
	gotoxy(0, 0); cout << "Гра ЗАВЕРШЕНА" << endl;
	// Зміна стану гравців ЯКЩО ОДИН  з гравців не ПРИПИНИВ ГРУ
	if (pl1->myShotsRes == ShotRes::Exit) { pl1->ps = Player_Stat::Surrendered; pl2->ps = Player_Stat::Winner;}
	if (pl2->myShotsRes == ShotRes::Exit) { pl2->ps = Player_Stat::Surrendered; pl1->ps = Player_Stat::Winner; }
	//Кінцеве відображення ігрового поля
	Show2(mtx1, mtx2, pl1, pl2);
	cout << endl <<" Гра тривала: " << (t_end - t_start) / CLOCKS_PER_SEC << " сек. ";
	system("pause");
//Трохи почисти пам'ять - видалення масивів ігрових полів
	if (mtx1 != nullptr) delete []mtx1;
	if (mtx2 != nullptr) delete []mtx2;
	if (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::client)
	{
		StopClient();
	}
	else if (_Game_Mode == enGame_Mode::net && _Game_Place == enGame_Place::server)
	{
		StopServer();
	}
	return (t_end - t_start);
}
#pragma region SeaBattle_Shot_Function
void Test_SB()
{
	fstream Test_out("SB_Test.txt", ios::app); //, ios::app
	Test_out.clear();
	if (Test_out.is_open())
	{
		sbf = 10; Test_MakeMtx("Створення ігрового поля розміром", 10, Test_out);
		Test_out.close();
	}
}

void Test_MakeMtx(string testname,int param, fstream &outfile)
{
	outfile << "Тест: " << testname << " " << param << " x " << param << endl;
	cout << "Тест: " << testname << " " << param << " x " << param;

	//int s = 10;
	int** mtx = MakeMtx(param);
	ResetBattleField(mtx, param);
	Player* pl = new Player("ГравецьТест",Player_Mode::comp);
	SetShipToBattleField(pl,mtx, param);
		outfile << "Гравець: " << pl->Name << endl;
		for (int i = 0; i < param; i++)
		{		for (int j = 0; j < param; j++)
			{
			outfile << mtx[i][j];
			}
		outfile << endl;
		}
	delete pl;
	
	cout << " виконано" << endl; 
	Sleep(100);
}
#pragma endregion

int main()
{
	setlocale(LC_ALL, "ru");
	SetScrSize();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Init_Lib();
	//Beep(500,338);
	int retCode = 1;
	//Створення нових гравців 1,2 типу КОМПЮТЕР
	pl1 = new Player("Комп'ютер", Player_Mode::comp);
	pl2 = new Player("Бодя", Player_Mode::comp);
	//Завдання високого рівня для гравцв 1,2
	pl1->level = Player_Level::level1;
	pl2->level = Player_Level::level1;
	//ГОЛОВНИЙ Цикл програмного меню
	while (retCode != 0)
	{
		system("cls");
		Show_MainHead();
		retCode = M_MainMenu();
		switch (retCode)
		{
		case 1: { M_Info();				} break;
		case 2: { M_Setting();			} break;
		case 3: { 
			CHM_SettingGameMode(true); 
			M_Game();		SaveScore(pl1, pl2);
			pl1->pm = Player_Mode::comp;
			pl2->pm = Player_Mode::comp;	} break;
		case 4: { ShowScore();			} break;
		case 0: { cout << "\t\t\t\tКінець гри!\n";	} break;
		default: break;
		}
	}
	system("pause");
	return 0;
}
