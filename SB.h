/*
* ������� ������
* �������� 2 ����� �������� ������� ��������������
* �������� �
* ���� ��������: ��� �������� ���
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
//�������� ���� ����� ��� ���
#pragma region SeaBattle_Declare
//����� ���� ���
int sbf=10;
//����� ������ ��������
int scrsize=100;
//ʳ������ �������
int cnt_shot=1;
//ʳ������ �����, ��� ������� ���� ����� ��������
int WIN_CNT=20;
//������� ������ �������, 10 - ������� : ������� �����, ����� �������
int Escadr[10][2] =
{
	{1,1},{1,2},{1,3},{1,4},
	{2,1},{2,2},{2,3},
	{3,1},{3,2},
	{4,1}
};

//��'��� �������� ������� ���� ���
int** mtx1, ** mtx2;

#pragma endregion

//������� ������ �� ����
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
//���� ��� ���������� �������
int ColorResult[14] = { White,LightRed,LightBlue,LightGreen,Yellow,Yellow,Yellow,Yellow,Yellow ,Yellow ,Yellow ,Yellow ,Yellow ,Yellow };
int ColorPlayerStat[5] = { White,LightGreen,LightRed,Yellow };

//������� ��� ��� = 0- �������� 1- ��������
std::vector<string> vGame_Mode;
//������� ��� �������� ���� = 0- ���� 1- ������ 2- �����
std::vector<string> vGame_Place;
//������� ���� �����
std::vector<string> Player_stan;
//������� ��� �����
std::vector<string> Player_type;
//������� в���� �����
std::vector<string> Player_level;
//������� ��������� �������
std::vector<string> Shot_Result;
//������� ��������� ������� ���������
std::vector<string> Shot_Result_Short;
#pragma endregion
#pragma region AllEnum
//���� ������� - enum
enum class ShipStan { alive, wound, killed };
//������ �������� ����
enum class enGame_Place {
	comp,
	server,
	client
};
//������ ���� ���
enum class enGame_Mode{
	loc,
	net
};
//������ ���� ���
enum class Player_Level {
	level0,
	level1
};
//������ ���: ����-����=0, ����-������=1, ������-������=2
//enum Game_Mode
//{
//	comp_comp = 0,
//	comp_human = 1,
//	human_human = 2,
//	human_comp = 3
//};

//�������� ���������� �������: �����-0,����-1,����-2,������-3
//enum class Direction
//{
//	toup,
//	todown,
//	toleft,
//	toright
//};

//��� ������ ����'����-0, ������-1, �볺�� -2
enum class Player_Mode
{
	comp = 0,
	human = 1,
	client = 2
};
//������ ������: �������-0, ����������-1, �����������-2
enum class Player_Stat
{
	InGame = 0,
	Winner = 1,
	Loser = 2,
	Surrendered=3
};
// �������� ����� �� ����
enum class PointVal
{
	sea = 0,	//����
	ship1 = 1,	//��������1
	ship2 = 2,	//��������2
	ship3 = 3,	//��������3
	ship4 = 4,	//��������4
	wound = 5,	//�������
	kill = 9,	//����
	miss = 6,	// ��������
	reserv2 = 7,//������ ������� ������� ��������
	reserv = 8,	//������ ��� �������� �������� �� ���
	reserv3 = 10,//������ 3 - ��� �������
	wasmiss =11,	//���� ��������
	waswound=12,	//���� �������
	waskill=14		//���� ����
};
//��������� �������: �����=4,������=1, �������=2, ����=3, ��������� ������-5, ��������� ������ - ���� �������-6 
enum class ShotRes
{
	New = 0, // ����� ������
	Miss = 1, // ��������
	Wound = 2, // �������
	Kill = 3, // ����
	Exit=4, // �� �����
	TryAgain=5,//���������� ����� - ��� ����
	TryShot=6 //������� �� ���� - �����
};
//��������� �������: 1-���� ������, 2-�� ������, 3- �� ��������
enum class enShotErr
{
	None = 0,
	NotValid = 1, // ���� ������
	NotLogic, // ��� ��������
	NotUniq // �� ��������
};
//���� ��ϲ� ���������� - ��� �������� ���
enum class SC_Com_type
{
	none,
	runExitLoop,
	ans_ok,
	ans_error,
	setPl1Name, // ������ ����� ������ 1
	setPl2Name, // ������ ����� ������ 2
	setPl1type, // ������ ��� ������ 1
	setPl2type, // ������ ��� ������ 2
	setPl1level, // ������ ����� ������ 1 
	setPl2level, // ������ ����� ������ 2
	setSBF,	// ���������� ����� �������� ����
	setCntShot, // ������ ������� �������
	runGame, // ��������� ���
	clearSMS = 15, // ������ �����������
	sendSMS, // �������� ����������
	ansShot, // �������� ������ �� �볺��� �� ������� - �� ������� �� �볺���
	ansResult // �������� ��������� �-�
};
//��������� �̲��� ����� ��� _Game_Mode
enGame_Mode _Game_Mode = enGame_Mode::loc;
//��������� �̲��� ����� ��� _Game_Place
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

//��'��� ������� "� �������?";

#pragma endregion


#pragma region myClasses

//��������� - ������� �� �������� �� �볺����
struct SC_Comand
{
	SC_Com_type type = SC_Com_type::none;
	string command;
	string message;
};
//��������� - ������� �� �������� �� �볺����
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
//��������� Point - �����
struct Point {
public: int x, y;
	  int bound = sbf;
//��������� ����� �� ������
	  Point() { x = -1; y = -1; };
	  Point(int x1, int y1) { x = x1; y = y1; };
	  bool valid() {
		  if (x < 0 || y < 0 || x > bound - 1 || y > bound - 1) return false;
		  else return true;
	  }
	  bool operator==(const Point& pnt) { return (x == pnt.x && y == pnt.y); } // ���� ��������� ������� �����
	  bool operator==(const Point* pnt) { return (x == pnt->x && y == pnt->y); } // ���� ��������� ������� �����

};
//��������� ������ �������
struct Desk : public Point{
public: 
	  int value = 0;
	  bool alive = true; // ����� ������
	  int bound = sbf;
	  Desk() { x = -1; y = -1; }; //��������� ����� ������
	  Desk(int x1, int y1) { x = x1; y = y1; }; // ��������� ������ � ������������ �,�
	  Desk(Point pt) { x = pt.y; y = pt.y; }; // ��������� ������ � ������
	  void Kill();
	  bool valid() { return Point(x, y).valid(); }
	  bool operator==(const Desk& dsk)	{ return (x == dsk.x && y == dsk.y);  }
	  bool operator==(const Desk* dsk) { return (x == dsk->x && y == dsk->y); }
};

//��������� ������ �������
struct  Ship{
public: 
	int desks=0; // ������� ����� � �������
	int alive = true; // ����� �������
	int orient=-1; // �������� ������� 0 - �������������, 1- �����������
	int deskskill = 0; // ����� ����� � �������
	ShipStan stan = ShipStan::alive; // ���� ������� - ��� �������� = �����
	vector<Desk> Desks; // ������ - ������ �������
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
//���������  - ������� ������� ������
struct  Escadra{
public:
	int ships = 0; // �������
	vector<Ship> Ships; // ������ ������
	Escadra() { ships = 0; Ships.clear(); }; // ��������� �������
	void Add(Ship *ship); // ������ �������� �� �������
	//bool GetShip(int x, int y, Ship * ship); // ������ �������� � ������ �� ������������ ������
	Ship* SearchShip(Point pnt); // ������ �������� � ������ �� ������������ ������
	Ship* SearchShip(int x, int y); // ������ �������� � ������ �� ������������ ������
};

//��������� ����в�
struct myShot
{
public:
	Point pnt; // ����� �������
	ShotRes shres = ShotRes::New; // ��������� �������
	vector<enShotErr> ShotError; // ������ - ������� �������
	myShot() { shres = ShotRes::New; pnt.x = -1; pnt.y = -1; } // ��������� ������ ������� 
	myShot(Point pnt1) { pnt = pnt1; shres = ShotRes::New;} // ��������� ������ ������� �� ������������ ����� �������
	myShot(int x, int y) { shres = ShotRes::New; pnt.x = x; pnt.y = y;	} // ��������� ������ ������� �� ������������ �,� �������
	myShot(string str) { shres = ShotRes::New; from_txt(str); } // ��������� ������ ������� �� ���������� ������������ ����� �������
	string to_txt() { return (char)(pnt.x + 65) + to_string(pnt.y + 1); } // ��������� �������� ������������� ����в�� � ������������
	// ����� ���������� ������� � ������ ���������
	void from_txt(string ss) {
		pnt.x = (int)(ss[0]) - 65; 
		pnt.y = atoi(ss.substr(1, ss.length() - 1).c_str()) - 1;
	}
	// ��������� �� � ���������� ������� ����������� ��� ������ ���� ��� MTX
	bool valid(int **mtx) {
		ShotError.clear();
		if (Valid_Coord(pnt.x, pnt.y, mtx) == false)
		{
			ShotError.push_back(enShotErr::NotValid); return false;
		}
		else { return true; }
	}
	// ��������� �� � ���������� ������� ����������� ��� ���� ����� ���� ��� MTX 
	bool valid() {
		ShotError.clear();
		if (pnt.valid() == false)
		{
			ShotError.push_back(enShotErr::NotValid); return false;
		}
		else { return true; }

	}
 // � �����
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
	} // �������
	bool logic(int** mtx) {
		bool retCode=false;
		if (pnt.valid())
		{
			int val = mtx[pnt.y][pnt.x];
			if (val != (int)PointVal::reserv2) {
				ShotError.push_back(enShotErr::NotLogic); retCode = false;
			}
			else retCode = true;
		} // �����
		return retCode;
	}
	};
//���� �������-�������;
class Gamer {
public:
	string Name;
};
//����� �������
class Player : public Gamer
{
public:
	Player_Stat ps;		//������ ������
	Player_Mode pm;		//��� ������
	Player_Level level;
	Escadra *myEscadra;
	//�� �������� ������� ������
	vector<myShot> myShots;
	//��� ����в�� ������
	vector<myShot> myNewShots;
	//���������� �������
	ShotRes myShotsRes;
	//��������� ����� ������ ����������
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
	//����� ����в�
	myShot myNewShot;
	//������ ����в˲�
	string strMyShots;
	//������ ��������Ҳ� ����в˲�
	string strMyShotsRes;
	//��������� � ������ ���������� ����� ������� � ������� ����� �������
	string strMyNewShotsRes() { string sr; 		for (auto& s : myNewShots) { sr += int(s.shres) + ","; } return sr; }
	//��������� � ������ ��� ������� � ������� ����� �������
	string strMyNewShots() {
		string ss; 
		for (auto& s : myNewShots) { ss += (char)(s.pnt.x + 65) + to_string(s.pnt.y + 1) + ",";} 
		return ss;
	}
	//�������� ���������� �������
	int ShotDirection = -1;	
	//�������� ������� ������� �������
	bool SecondCompShot = false;
	//������ ������ ������
	bool CompHit = false;
	//��������� ������ ������
	bool NextCompHit = false;
	//������ �� ������ �������� ������� 
	bool CompMiss[4] = { false, false, false, false };

	int shot;			//�������� �������
	int kill;			//�������� ������
	int cnt_shots = cnt_shot; //������� ������� � ���ϲ � ��������� ������� ������� 
	void New_Game();	//������� ������� ���� ���
	void New_Shot();	//������� ������� ���� ���
	//������ ������ ������ � ������
	//������ �������Ͳ ������� � ������ �������
	void Add_ShotFromNew(myShot myNewShot);
	// ������ ������ ��� ����������� ���� ��� � ����в��
	bool Add_Shot(myShot ms, int **mtx);
	// ������ ������ ��� ����������� ���� ��� � ������ ����в˲�
	bool Add_Shot(string s, int **mtx);
	// ������ �� ������������ ������� � ������ �������
	bool Add_AllShot(string s, int** mtx);
	//������ ��������� ������� � ������ �������
	vector<myShot> Add_Shots(string ss, int** mtx);
	//������ ��������� �Ѳ� ������� � ������ �������
	vector<myShot> Add_AllShots(string ss, int** mtx);
	// ���������� ���������� ������� � ������ ����в˲� �� ��������� ���� ��� mtx
	void Apply_ShotsRes(string sr, int** mtx); 
	//��������� ������ � ����������� �� �����������
	Player();
	//��������� ������ � �����������, �� ����������� 
	Player(string name, Player_Mode pm1);
	//�������� ������
	~Player();
private:
};
//��������� �� ���� �������, ���� �������
Player* pl1, * pl2;

#pragma endregion

#pragma region SeaBattle_Show_Functions
//³��������� ������� ������ ��������
void Show_MainHead();
//³���������� 1-�� �������� ���� - ����� �������
void Show(int** mtx, int k, Player* pl);
//³���������� ����� �� �������� ���
char ShowPoint(int val, bool hide);
//³���������� ���� ������� �����
void Show2(int** mtx1, int** mtx2, Player* pl1, Player* pl2);
//�������� ���� �� ������ ���� ����������
void clearMsgPlace(int s);
void clearMsgPlace(int p, int fp);
#pragma endregion

#pragma region BattleField_Function
//�� � ����� - ��������
bool Is_Ship(int x, int y, int** mtx);
//�� �� ����� - ����� ��� ������
bool Has_Neib(int x, int y, int** mtx);
//��������� �������� ����
int** MakeMtx(int mtxs);
//���������� �������� ���� �����
void ResetBattleField(int** mtx, int param);
//���������� �������� ���� ���������
void SetShipToBattleField(Player* pl, int** mtx, int param);
//������������ ���� ������� �������
void Rezerv(int x1, int y1, int x2, int y2, int** mtx, int marker);
//������������ ���� ������� ������� �������
void Rezerv_Shot(int x, int y, int** mtx, int marker, bool alldesk);
#pragma endregion


#pragma region SeaBattle_MenuFunction
//���� ������������ ������ ����
void CHM_SettingSize();
//���� ������������ ������� ������� � ���� 
void CHM_SettingShot();
//���� ������������ ��� ������
void CHM_SettingPlayer(int i, Player* pl);
//���� ����� ���
void CHM_SettingGameMode(bool mode);
//���� ������ ������ - �˲���� ��� �������� ���
void CHM_NetClientPlayer(int i, Player* pl);
//������� ����
int M_MainMenu();
//������������ ����
void M_Info();
//��������� ����
void M_Setting();
//���� ��� ���� ����ֲ�
void M_SettingPlayer();
//������� ���� ���
void M_Game();
#pragma endregion
//����� ������� � ������ - ������
string ascForShot(Player* pl);
void Game_ChatSC1();
void Game_ChatSC(int place);
//³�������� ���������� ����+����
void SendSMS(SC_Com_type type, string msg);
//�������� ���������� � ����� fname
void Get_Info_From_File(string fname);
//��� �� �������� �� �볺����
void ChatSC();
//����� - ��� ���������� ��䳿
int Spiner(int s);
//������ ����������� �� ���������� ������
void WaitSMS(string s, Player * pl, int ** mtx);
//������� ����в� ������ �� ����
string MakeShot(Player* pl, int** mtx, Player* pl_e);
//string ApplyShot(Player* pl, int** mtx);
//�������� �� ��������� ������� command � ���������� �� ���������� �볺��� ������ 
bool ApplyCommandFromMessage(string command, Player* pl, int** mtx);
//������ ������ pl 
void Shot(Player* pl, int** mtx, Player* pl_e, int** mtx_e);
//�������� ������� �� ������ - ������
string GetUserShot(Player* pl, int** mtx);
//�������� ������� �� ������ - ����'�����
string GetCompShot(Player* pl, int** mtx);
//�������� ������ �� ������
string GetShot(Player* pl, int** mtx);
//��������� ��� �������� �������
int ShowErrorShot(Player* pl, myShot sh);
void SaveScore(Player* pl1,Player* pl2);
void ShowScore();
//void M_SB_Test();
void Init_Lib();

#endif