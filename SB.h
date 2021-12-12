/*
* Курсова робота
* Студента 2 курсу ЛЕСНЕНКО Богдана Олександровича
* Завдання № 33
* Тема завдання: Гра Морський Бій
*/

#ifndef _SB_H_
#define _SB_H_
#include <string>
#include <vector>
#include <sstream>

using namespace std;
#pragma region SeaBattle_Declare
void Test_SB();
void Test_MakeMtx(string testname, int param, fstream &outfile);
#pragma endregion
//Глобальні змінні умови для гри
#pragma region SeaBattle_Declare
//Розмір поля бою
int sbf=10;
//Розмір екрану терміналу
int scrsize=100;
//Кількість пострілів
int cnt_shot=1;
//Кількість палуб, при враженні яких настає перемога
int WIN_CNT=20;
//Матриця ескадр кораблів, 10 - кораблів : кількість палуб, номер корабля
int Escadr[10][2] =
{
	{1,1},{1,2},{1,3},{1,4},
	{2,1},{2,2},{2,3},
	{3,1},{3,2},
	{4,1}
};

//Об'ява двомірних матриць полів бою
int** mtx1, ** mtx2;

#pragma endregion

//Кольори шрифту та фону
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
//Колір для результатів пострілу
int ColorResult[14] = { White,LightRed,LightBlue,LightGreen,Yellow,Yellow,Yellow,Yellow,Yellow ,Yellow ,Yellow ,Yellow ,Yellow ,Yellow };
int ColorPlayerStat[5] = { White,LightGreen,LightRed,Yellow };

//Вектори Тип гри = 0- локальна 1- мережева
std::vector<string> vGame_Mode;
//Вектори Тип ігрового місця = 0- комп 1- сервер 2- кліент
std::vector<string> vGame_Place;
//Вектори СТАН грака
std::vector<string> Player_stan;
//Вектори ТИП грака
std::vector<string> Player_type;
//Вектори РІВЕНЬ грака
std::vector<string> Player_level;
//Вектори РЕЗУЛЬТАТ пострілу
std::vector<string> Shot_Result;
//Вектори РЕЗУЛЬТАТ пострілу СКОРОЧЕНО
std::vector<string> Shot_Result_Short;
#pragma endregion
#pragma region AllEnum
//Стан коравля - enum
enum class ShipStan { alive, wound, killed };
//Перелік ігрового місця
enum class enGame_Place {
	comp,
	server,
	client
};
//Перелік рівнів гри
enum class enGame_Mode{
	loc,
	net
};
//Перелік рівнів гри
enum class Player_Level {
	level0,
	level1
};
//Режими гри: комп-комп=0, комп-людина=1, людина-людина=2
//enum Game_Mode
//{
//	comp_comp = 0,
//	comp_human = 1,
//	human_human = 2,
//	human_comp = 3
//};

//Напрямок наступного пострілу: вгору-0,вниз-1,вліво-2,вправо-3
//enum class Direction
//{
//	toup,
//	todown,
//	toleft,
//	toright
//};

//Тип гравця комп'ютер-0, людина-1, клієнт -2
enum class Player_Mode
{
	comp = 0,
	human = 1,
	client = 2
};
//Статус гравця: гравець-0, переможець-1, переможений-2
enum class Player_Stat
{
	InGame = 0,
	Winner = 1,
	Loser = 2,
	Surrendered=3
};
// значення точок на карті
enum class PointVal
{
	sea = 0,	//море
	ship1 = 1,	//корабель1
	ship2 = 2,	//корабель2
	ship3 = 3,	//корабель3
	ship4 = 4,	//корабель4
	wound = 5,	//поранив
	kill = 9,	//вбив
	miss = 6,	// промазав
	reserv2 = 7,//резерв навколо вдалого влучення
	reserv = 8,	//резерв при розміщенні коравблів на полі
	reserv3 = 10,//резерв 3 - для повтору
	wasmiss =11,	//було промазав
	waswound=12,	//було поранив
	waskill=14		//було вбив
};
//Результат пострілу: вихід=4,промах=1, поранив=2, вбив=3, повторити спробу-5, повторити постріл - після вдалого-6 
enum class ShotRes
{
	New = 0, // новий постріл
	Miss = 1, // промазав
	Wound = 2, // поранив
	Kill = 3, // вбив
	Exit=4, // на вихід
	TryAgain=5,//спробувати знову - вже було
	TryShot=6 //зробити ще один - попав
};
//Результат пострілу: 1-поза межами, 2-не логічна, 3- не унікальна
enum class enShotErr
{
	None = 0,
	NotValid = 1, // поза межами
	NotLogic, // вже вибирали
	NotUniq // не унікальна
};
//Коди ТИПІВ повідомлень - для мережевої гри
enum class SC_Com_type
{
	none,
	runExitLoop,
	ans_ok,
	ans_error,
	setPl1Name, // змінити назву гравця 1
	setPl2Name, // змінити назву гравця 2
	setPl1type, // змінити тип гравця 1
	setPl2type, // змінити тип гравця 2
	setPl1level, // змінити рівень гравця 1 
	setPl2level, // змінити рівень гравця 2
	setSBF,	// встановити розмір ігрового поля
	setCntShot, // змінити кількість пострілів
	runGame, // запустити гру
	clearSMS = 15, // очисти повідомлення
	sendSMS, // надіслати повіомлення
	ansShot, // відповісти постріл від клієнта до сервера - від сервера до клієнта
	ansResult // відповісти результат К-С
};
//ГЛОБАЛЬНА ЗМІННА Режим гри _Game_Mode
enGame_Mode _Game_Mode = enGame_Mode::loc;
//ГЛОБАЛЬНА ЗМІННА Режим гри _Game_Place
enGame_Place _Game_Place = enGame_Place::comp;
#pragma endregion

#pragma region Function_Cursor
void SetColor(int text, int bg);
int wherex();
int wherey();
void gotoxy(int column, int line);
void SetScrSize();
void clearMsgPlace(int p);
void clearMsgPlace(int p, int fp);
void Wait(string strmsg);
bool Valid_Coord(int x, int y);
bool Valid_Coord(int x, int y, int** mtx);
//string ascForShot(Player* pl);
#pragma endregion

#pragma region SeaBattle_functions

//Об'ява Функції "Є корабль?";

#pragma endregion


#pragma region myClasses

//Структура - КОМАНДА між Сервером та Клієнтом
struct SC_Comand
{
	SC_Com_type type = SC_Com_type::none;
	string command;
	string message;
};
//Структура - КОМАНДИ між Сервером та Клієнтом
struct SC_Comands
{
	vector<SC_Comand> SCCs;
	const char separator = '|';
	const char separator2 = '&';
	void myGetMessage(string msg) {
		SCCs.clear();
		std::stringstream streamData(msg);
		string val;
		while (getline(streamData, val, separator))
		{
			stringstream streamData2(val);
			string val2; bool f = true;
			SC_Comand sc;// = new SC_Comand;
			sc.message = val;
			while (getline(streamData2, val2, separator2))
			{
				if (f == true) { sc.type = (SC_Com_type)stoi(val2); f = false; }
				else { sc.command = val2; f = true; }
			}
			SCCs.push_back(sc);
		}
	};
	void mySetMessage(SC_Com_type type, string msg) {
		SC_Comand sc;
		sc.type = type;
		sc.command = msg;
		sc.message = to_string((int)type) + separator2 + msg;
		SCCs.push_back(sc);
	};
};
//Структура Point - Точка
struct Point {
public: int x, y;
	 //  int bound = sbf;
	//Створення ТОЧКИ за межами
	  Point() { x = -1; y = -1; };
	  Point(int x1, int y1) { x = x1; y = y1; };
	  bool valid() {
		  if (x < 0 || y < 0 || x > sbf - 1 || y > sbf - 1) return false;
		  else return true;
	  }
	  bool operator==(const Point& pnt) { return (x == pnt.x && y == pnt.y); } // зміна оператора рівняння ТОЧОК
	  bool operator==(const Point* pnt) { return (x == pnt->x && y == pnt->y); } // зміна оператора рівняння ТОЧОК

};
//Структура Палуба корабля
struct Desk : public Point{
public: 
	  int value = 0;
	  bool alive = true; // живая ПАЛУБА
	 // int bound = sbf;
	  Desk() { x = -1; y = -1; }; //створення пустої палуби
	  Desk(int x1, int y1) { x = x1; y = y1; }; // створення палуби з координатами х,у
	  Desk(Point pt) { x = pt.y; y = pt.y; }; // створення палуби з точкою
	  void Kill();
	  bool valid() { return Point(x, y).valid(); }
	  bool operator==(const Desk& dsk)	{ return (x == dsk.x && y == dsk.y);  }
	  bool operator==(const Desk* dsk) { return (x == dsk->x && y == dsk->y); }
};

//Структура Палуба корабля
struct  Ship{
public: 
	int desks=0; // кількість палуб у корабля
	int alive = true; // живий крабель
	int orient=-1; // орієнтація корабля 0 - горизонтально, 1- вертикально
	int deskskill = 0; // вбито палуб у корабля
	ShipStan stan = ShipStan::alive; // стан корабля - при створенні = ЖИВИЙ
	vector<Desk> Desks; // вектор - палуби корабля
	Ship();
	Ship(int orient, int desks);
	void Add(Desk *dsk);
	ShipStan Kill(Desk* dsk);
	ShipStan Kill(int x, int y);
	Desk* SearchDesk(Point *pnt);
	Desk* SearchDesk(int x, int y);
	void ReservAround(int **mtx0, PointVal pv);
	ShipStan GetStan();
	~Ship();
};
//Структура  - Ескадра кораблів ГРАВЦЯ
struct  Escadra{
public:
	int ships = 0; // кораблів
	vector<Ship> Ships; // вектор кораблі
	Escadra() { ships = 0; Ships.clear(); }; // Створення ескарди
	void Add(Ship *ship); // додати корабель до ескадри
	//bool GetShip(int x, int y, Ship * ship); // знайти корабель в ескадрі за координатами палуби
	Ship* SearchShip(Point pnt); // знайти корабель в ескадрі за координатами палуби
	Ship* SearchShip(int x, int y); // знайти корабель в ескадрі за координатами палуби
};

//Структура ПОСТРІЛ
struct myShot
{
public:
	Point pnt; // точка пострілу
	ShotRes shres = ShotRes::New; // результат пострілу
	vector<enShotErr> ShotError; // Вектор - помилки Пострілів
	myShot() { shres = ShotRes::New; pnt.x = -1; pnt.y = -1; } // Створення НОВОГО пострілу 
	myShot(Point pnt1) { pnt = pnt1; shres = ShotRes::New;} // Створення НОВОГО пострілу за координатами ТОЧКИ пострілу
	myShot(int x, int y) { shres = ShotRes::New; pnt.x = x; pnt.y = y;	} // Створення НОВОГО пострілу за координатами х,у пострілу
	myShot(string str) { shres = ShotRes::New; from_txt(str); } // Створення НОВОГО пострілу за строковими координатами ТОЧКИ пострілу
	string to_txt() { return (char)(pnt.x + 65) + to_string(pnt.y + 1); } // повернути строкове представлення ПОСТРІЛУ з координатами
	// Взяти координати пострілу зі строки КООРДИНАТ
	void from_txt(string ss) {
		pnt.x = (int)(ss[0]) - 65; 
		pnt.y = atoi(ss.substr(1, ss.length() - 1).c_str()) - 1;
	}
	// Визначити чи є КООРДИНАТИ пострілу ПРАВИЛЬНИМИ для даного поля бою MTX
	bool valid(int **mtx) {
		ShotError.clear();
		if (Valid_Coord(pnt.x, pnt.y, mtx) == false)
		{
			ShotError.push_back(enShotErr::NotValid); return false;
		}
		else { return true; }
	}
	// Визначити чи є КООРДИНАТИ пострілу ПРАВИЛЬНИМИ для БУДЬ ЯКОГО поля бою MTX 
	bool valid() {
		ShotError.clear();
		if (pnt.valid() == false)
		{
			ShotError.push_back(enShotErr::NotValid); return false;
		}
		else { return true; }

	}
 // в межах
	bool uniq(int** mtx) {
		if (pnt.valid())
		{
			int val = mtx[pnt.y][pnt.x];
			if (val != (int)PointVal::wound && val != (int)PointVal::miss && val != (int)PointVal::kill)
			{
				ShotError.push_back(enShotErr::NotUniq); return false;
			}
			else return true;
		}
		else return false;
	} // унікальні
	bool logic(int** mtx) {
		bool retCode=false;
		if (pnt.valid())
		{
			int val = mtx[pnt.y][pnt.x];
			if (val != (int)PointVal::reserv2) {
				ShotError.push_back(enShotErr::NotLogic); retCode = false;
			}
			else retCode = true;
		} // логічні
		return retCode;
	}
	};
//Клас Гравець-базовий;
class Gamer {
public:
	string Name;
};
//Класс ГРАВЕЦЬ
class Player : public Gamer
{
public:
	Player_Stat ps;		//статус гравця
	Player_Mode pm;		//тип гравця
	Player_Level level;
	Escadra *myEscadra;
	//Всі проведені постріли гравця
	vector<myShot> myShots;
	//Нові ПОСТРІЛИ гравця
	vector<myShot> myNewShots;
	//Результати пострілу
	ShotRes myShotsRes;
	//Присвоєння тільки одного результату
	void OnlyOneSR(ShotRes msr) { 
		switch (msr) 
		{
		case ShotRes::Miss: myShotsRes = ShotRes::Miss; break;
		case ShotRes::Kill: if (myShotsRes == ShotRes::TryAgain || myShotsRes == ShotRes::Miss || myShotsRes == ShotRes::New || myShotsRes == ShotRes::TryShot) myShotsRes = ShotRes::TryShot; break;
		case ShotRes::Wound: if (myShotsRes == ShotRes::TryAgain || myShotsRes == ShotRes::Miss || myShotsRes == ShotRes::New || myShotsRes == ShotRes::TryShot) myShotsRes = ShotRes::TryShot; break;
		case ShotRes::Exit: myShotsRes = ShotRes::Exit; break;
		case ShotRes::New: myShotsRes = ShotRes::New; break;
		case ShotRes::TryAgain: if (myShotsRes != ShotRes::Exit) myShotsRes = ShotRes::TryAgain; break;
		case ShotRes::TryShot: if (myShotsRes != ShotRes::Exit) myShotsRes = ShotRes::TryShot; break;
		default: break;
		}
	};
	//Новий ПОСТРІЛ
	myShot myNewShot;
	//Строка ПОСТРІЛІВ
	string strMyShots;
	//Строка РЕЗУЛЬТАТІВ ПОСТРІЛІВ
	string strMyShotsRes;
	//Повернути в строку результати Нових Пострілів з вектору Нових пострілів
	string strMyNewShotsRes() { string sr; 		for (auto& s : myNewShots) { sr += int(s.shres) + ","; } return sr; }
	//Повернути в строку Нові Постріли з вектору Нових пострілів
	string strMyNewShots() {
		string ss; 
		for (auto& s : myNewShots) { ss += (char)(s.pnt.x + 65) + to_string(s.pnt.y + 1) + ",";} 
		return ss;
	}
	//напрямок наступного пострілу
	int ShotDirection = -1;	
	//Покажчик другого Вдалого пострілу
	bool SecondCompShot = false;
	//Вдалий постріл гравця
	bool CompHit = false;
	//Наступний постріл гравця
	bool NextCompHit = false;
	//Перелік НЕ вдалих напрямків пострілів 
	bool CompMiss[4] = { false, false, false, false };

	int shot;			//лічильник пострілів
	int kill;			//лічильник вбитих
	int cnt_shots = cnt_shot; //кількість пострілів в ЗАЛПІ з глобальної кількості пострілів 
	void New_Game();	//функція початку нової гри
	void New_Shot();	//функція початку нової гри
	//Добати постріл іграка в вектор
	//додати ПРОВЕДЕНІ постріли з нового пострілу
	void Add_ShotFromNew(myShot myNewShot);
	// додати постріл для конкретного поля бою з ПОСТРІЛУ
	bool Add_Shot(myShot ms, int **mtx);
	// додати постріл для конкретного поля бою з строки ПОСТРІЛІВ
	bool Add_Shot(string s, int **mtx);
	// додати всі запропановані постріли з СТРОКИ пострілів
	bool Add_AllShot(string s, int** mtx);
	//Вектор додавання пострілів зі строки пострілів
	vector<myShot> Add_Shots(string ss, int** mtx);
	//Вектор додавання ВСІХ пострілів зі строки пострілів
	vector<myShot> Add_AllShots(string ss, int** mtx);
	// призначити результати пострілів зі строки ПОСТРІЛІВ на конкретне поле бою mtx
	void Apply_ShotsRes(string sr, int** mtx); 
	//СТворення гравця з параметрами за замовченням
	Player();
	//СТворення гравця з параметрами, які передаються 
	Player(string name, Player_Mode pm1);
	//Знищення гравця
	~Player();
private:
};
//Вказівники на двох гравців, типу ГРАВЕЦЬ
Player* pl1, * pl2;

#pragma endregion

#pragma region SeaBattle_Show_Functions
//Відобразити головну строку програми
void Show_MainHead();
//Відображення 1-го ігрового поля - стара функція
void Show(int** mtx, int k, Player* pl);
//Відображення точки на ігровому полі
char ShowPoint(int val, bool hide);
//Відображення двох ігрових полей
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2);
//очистити місце на консолі після повіомлення
void clearMsgPlace(int s);
void clearMsgPlace(int p, int fp);
#pragma endregion

#pragma region BattleField_Function
//Чи є точка - кораблем
bool Is_Ship(int x, int y, int** mtx);
//Чи має точка - сусідні живі кораблі
bool Has_Neib(int x, int y, int** mtx);
//Створення ігрового поля
int** MakeMtx(int mtxs);
//Заповнення ігрового поля морем
void ResetBattleField(int** mtx, int param);
//Заповнення ігрового поля кораблями
void SetShipToBattleField(Player* pl, int** mtx, int param);
//Резервування полів навколо корабля
void Rezerv(int x1, int y1, int x2, int y2, int** mtx, int marker);
//Резервування полів навколо вдалого пострілу
void Rezerv_Shot(int x, int y, int** mtx, int marker, bool alldesk);
#pragma endregion


#pragma region SeaBattle_MenuFunction
//Меню встановлення розміру поля
void CHM_SettingSize();
//Меню встановлення кількості пострілів в залпі 
void CHM_SettingShot();
//Меню встановлення для ГРАВЦЯ
void CHM_SettingPlayer(int i, Player* pl);
//Меню режим гри
void CHM_SettingGameMode(bool mode);
//Меню вибора ГРАВЦЯ - КЛІЄНТА для мережевої гри
void CHM_NetClientPlayer(int i, Player* pl);
//Головне Меню
int M_MainMenu();
//Інформаційне Меню
void M_Info();
//Встановче Меню
void M_Setting();
//Меню для зміни ГРАВЦІВ
void M_SettingPlayer();
//ГОЛОВНЕ меню гри
long M_Game();
#pragma endregion
//Запит пострілів у гравця - людини
string ascForShot(Player* pl);
void Game_ChatSC1();
void Game_ChatSC(int place);
//Відправити повіомлення типу+зміст
void SendSMS(SC_Com_type type, string msg);
//Отримати інформацію з файлу fname
void Get_Info_From_File(string fname);
//Чат між Сервером та Клієнтом
void ChatSC();
//Спінер - для очікування події
int Spiner(int s);
//Чекати повідомлення від мережевого гравця
void WaitSMS(string s, Player * pl, int ** mtx);
//ЗРОБИТИ ПОСТРІЛ гравцю на поле
string MakeShot(Player* pl, int** mtx, Player* pl_e);
//string ApplyShot(Player* pl, int** mtx);
//Прийняти до виконання команди command з повіомлення від мережевого клієнта ГРАВЦЮ 
bool ApplyCommandFromMessage(string command, Player* pl, int** mtx);
//Постріл гравця pl 
void Shot(Player* pl, int** mtx, Player* pl_e, int** mtx_e);
//Отримати постріли від гравця - людини
string GetUserShot(Player* pl, int** mtx);
//Отримати постріли від гравця - комп'ютера
string GetCompShot(Player* pl, int** mtx);
//Отримати посріли від гравця
string GetShot(Player* pl, int** mtx);
//Повідомити про помилкові постріли
int ShowErrorShot(Player* pl, myShot sh);
void SaveScore(Player* pl1,Player* pl2);
void ShowScore();
//void M_SB_Test();
void Init_Lib();

#endif