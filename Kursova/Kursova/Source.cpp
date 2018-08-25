#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <regex>
using namespace std;

const string FILE_TO_SAVE = "Base_LVL";


HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbInfo;

void cl() {
	system("cls");
}

void gotoxy(const int &x, const int &y) {
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}

void gotoxy(const POINT &pos) {
	gotoxy(pos.x, pos.y);
}

int GetBufferChars() {
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Right - csbInfo.srWindow.Left;

}

int GetBufferCharsbot() {
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Bottom - csbInfo.srWindow.Top;
}

float Onlynums(string ttext) {
	
	char temp[30], text[30];
	strcpy_s(temp, 29, ttext.c_str());
	int h = 0, m = 0;
	while (text[m] != '\0')
	{
		if ((text[m] >= '0' && text[m] <= '9') || (text[m] == '.' && h>0 && text[m - 1] >= '0' && text[m - 1] <= '9') || (m == 0 && text[m] == '-')) {
			temp[h] = text[m];
			h++;
		}
		else return 0;
		m++;
	}
	temp[h] = '\0';
	if (strlen(temp) == 0) {
		temp[0] = '0';
		temp[1] = '\0';
	}
	strcpy_s(text, 29, temp);
	return atof(text);
}

int CountDigits(int digit) {
	int i = 0;
	if (digit < 0 || digit == 0) i++;
	while (digit > 0) {
		digit /= 10;
		i++;
	}
	return i;
}

POINT consoleSize = { GetBufferChars() , GetBufferCharsbot() };


void SetVisibleCursor(const bool &rst) {
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hCon, &structCursorInfo);
	structCursorInfo.bVisible = rst;
	SetConsoleCursorInfo(hCon, &structCursorInfo);


}

const POINT GetPosCur() {
	POINT pos;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bi);
	pos.x = bi.dwCursorPosition.X;
	pos.y = bi.dwCursorPosition.Y;
	return pos;
}

void clearToPos(int x, int y) {
	POINT current_pos = GetPosCur();
	while (current_pos.y >= y)
	{
		if (current_pos.y > y) {
			gotoxy(0, current_pos.y);
			for (int i = 0; i < consoleSize.x; i++)
			{
				cout << '\0';
			}
		}
		else if (current_pos.y == y) {
			gotoxy(x, current_pos.y);
			if (current_pos.x != x) for (int i = 0; i <= consoleSize.x - x; i++)
			{
				cout << '\0';
			}
		}

		current_pos.y--;
	}
	gotoxy(x, y);
}

void clearLine(const int &s = 0) {
	POINT pos = GetPosCur();
	for (int i = 0; i < consoleSize.x - pos.x - s; i++)
	{
		cout << '\0';
	}
	gotoxy(pos);
}

void clearToPos(POINT t) {
	clearToPos(t.x, t.y);
}

bool SetColorConsole(const int textColor = 7, const int background = 0) {
	if (textColor > -1 && textColor < 16 && background > -1 && background < 16) {
		SetConsoleTextAttribute(hCon, (WORD)((background << 4) | textColor));
		return true;
	}
	else return false;
}

void FullLine(char a, int font = 7, int back = 0) {
	SetConsoleTextAttribute(hCon, (WORD)((back << 4) | font));
	int _chars = consoleSize.x;
	for (int i = 0; i < _chars; i++)
	{
		cout << a;
	}
	cout << endl;
	SetConsoleTextAttribute(hCon, (WORD)((0 << 4) | 7));
}

int countSym(string str, char sym) { // функцiя для пiдрахування кiлькостi символiв
	int r = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == sym) r++;
	}
	return r;
}

class Date {
	int day = 0, month = 0, year = 0;
public:

	bool IsLeapYear(const int &iYear) const {
		return ((!(iYear % 4) && (iYear % 100)) || (!(iYear % 400))) ? true : false;
	}

	bool SetDay(const int &day) {
		if (IsOkDay(day, month, year)) {
			this->day = day;
			return true;
		}
		else return false;
	}

	bool SetYear(const int &year) {
		if (IsOkYear(year)) {
			this->year = year;
			return true;
		}
		else return false;
	}

	bool SetMonth(const int &month) {
		if (IsOkMonth(month)) {
			this->month = month;
			return true;
		}
		else return false;
	}

	int CountDays(const int &month, const int &year) const {
		WORD countDays = 0;
		switch (month)
		{
			case 1: countDays = 31; break;
			case 2: if (IsLeapYear(year)) countDays = 29; else countDays = 28; break;
			case 3: countDays = 31; break;
			case 4: countDays = 30; break;
			case 5: countDays = 31; break;
			case 6: countDays = 30; break;
			case 7: countDays = 31; break;
			case 8: countDays = 31; break;
			case 9: countDays = 30; break;
			case 10: countDays = 31; break;
			case 11: countDays = 30; break;
			case 12: countDays = 31; break;
		}
		return countDays;
	}

	bool IsOkYear(const int &year) const {
		if (year > 0) return true;
		else return false;
	}

	bool IsOkMonth(const int &month) const {
		if (month > 0 && month < 13) return true;
		else return false;
	}

	bool IsOkDay(const int &day, const int &month, const int &year) const {
		if (day > 0 && day < CountDays(month, year)) return true;
		else return false;
	}

	bool IsOkAll(const int &day, const int &month, const int &year) const {
		if (IsOkDay(day, month, year) && IsOkMonth(month) && IsOkYear(year)) return true;
		else return false;
	}

	Date() {
		time_t tt = time(NULL);
		tm tim;
		localtime_s(&tim, &tt);
		this->day = tim.tm_mday;
		this->month = tim.tm_mon + 1;
		this->year = 1900 + tim.tm_year;
	}

	Date(int day, int month, int year) {
		SetAll(day, month, year);
	}

	Date(const Date &date) {
		SetAll(date);
	}

	bool SetAll(const int &day, const int &month, const int &year) {
		if (IsOkAll(day, month, year)) {
			this->day = day;
			this->year = year;
			this->month = month;
			return true;
		}
		else return false;
	}

	bool SetAll(const Date &date) {
		return SetAll(date.day, date.month, date.year);
	}

	int GetDay() const {
		return day;
	}

	int GetYear() const {
		return year;
	}

	int GetMonth() const {
		return month;
	}

	string GetAllStr(const string &miz = ".") const {
		return to_string(day) + miz + to_string(month) + miz + to_string(year);
	}

	void GetAllConsole(const string &miz = ".") const {
		cout << day << miz << month << miz << year;
	}

	Date & operator = (const Date &date) {
		this->day = date.day;
		this->month = date.month;
		this->year = date.year;
		return *this;
	}

	friend const bool operator > (const Date &date, const Date &date2);

	friend const bool operator < (const Date &date, const Date &date2);

	friend const bool operator == (const Date &date, const Date &date2);
};

const bool operator > (const Date &date2, const Date &date) {
	if (date2.year > date.year) return true;
	else if (date2.year == date.year) {
		if (date2.month > date.month) return true;
		else if (date2.month == date.month) {
			if (date2.day > date.day) return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

const bool operator < (const Date &date2, const Date &date) {
	if (date2.year < date.year) return true;
	else if (date2.year == date.year) {
		if (date2.month < date.month) return true;
		else if (date2.month == date.month) {
			if (date2.day < date.day) return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

const bool operator == (const Date &date2, const Date &date) {
	if (date2.year != date.year) return false;
	else if (date2.year == date.year) {
		if (date2.month != date.month) return false;
		else if (date2.month == date.month) {
			if (date2.day == date.day) return true;
			else return false;
		}
		else return false;
	}
	else return false;
}

class Category /*Категрiї*/ {
	string name; // iмя категорiї
	vector<string> namesCategories; // Назви пiд категорiй
public:

	Category() = default;

	Category(const string &name) {
		SetName(name);
	}

	bool SetName(const string &name) { // Установка iмя категорiї
		if (name.length() > 0) {
			this->name = name;
			return true;
		}
		else return false;
	}

	bool SetNamePidCategory(const int &i, const string &name) { // Установка iмя під категорiї
		if (name.length() > 0 && IsOkPidCategory(i)) {
			namesCategories[i] = name;
			return true;
		}
		else return false;
	}

	int GetCountOfPidCategories() const { // Видобуванння кiлкостi пiд категогрiй
		return namesCategories.size();
	}

	const string GetName() const { // Видобування iмя категорiї
		return name;
	}

	const string GetNamePidCategory(const int &i) const { // Видобування iмя пiд категорiї
		return namesCategories[i];
	}

	bool AddPidCategory(string name) { // Добавлення нової пiд категорiї
		if (name.length() > 0) {
			namesCategories.push_back(name);
			return true;
		}
		else return false;
	}

	bool IsOkPidCategory(const int &category) const { // Перевiрка пiд категорiї
		if (category < namesCategories.size() && category > -1) return true;
		else return false;
	}

	bool RemovePidCategory(const int &i) {
		if (IsOkPidCategory(i)) namesCategories.erase(namesCategories.begin() + i);
		else return false;
	}

	friend ostream& operator<<(ostream &os, const Category &value);
	friend istream& operator>>(istream &is, Category &value);
};

ostream& operator<<(ostream &os, const Category &value) {
	int size = value.namesCategories.size(), strSize;
	os.write(reinterpret_cast<char *>(&size), sizeof(int));
	for (int i = 0; i < value.namesCategories.size(); i++) {
		strSize = value.namesCategories[i].size();
		os.write(reinterpret_cast<char *>(&strSize), sizeof(int));
		os << value.namesCategories[i].c_str();
	}
	strSize = value.name.size();
	os.write(reinterpret_cast<char *>(&strSize), sizeof(int));
	os << value.name.c_str();
	return os;
}

istream& operator >> (istream &is, Category &value) {
	int size, strSize;
	char *temp = nullptr;
	is.read(reinterpret_cast<char *>(&size), sizeof(int));
	for (int i = 0; i < size; i++) {
		is.read(reinterpret_cast<char *>(&strSize), sizeof(int));
		temp = new char[strSize + 1];
		is.read(reinterpret_cast<char *>(temp), sizeof(strSize));
		value.namesCategories.push_back(temp);
		if (temp != nullptr) delete temp;
		temp = nullptr;
	}
	is.read(reinterpret_cast<char *>(&strSize), sizeof(int));
	if (temp != nullptr) delete temp;
	temp = new char[strSize + 1];
	is.read(reinterpret_cast<char *>(temp), sizeof(strSize));
	temp[strSize] = '\0';
	value.name = temp;
	if (temp != nullptr) delete temp;
	temp = nullptr;
	return is;
}

class AllCategory /*Всi категорiї*/ {	
public:
	vector<Category> ctg;

	bool isOkCategory(const int &category) const { // Перевiрка категорiї
		if (category < ctg.size() && category > -1) return true;
		else return false;
	}
};


AllCategory ACtg;

class Product { // Продук
	string name; // iмя
	Date termOfConsumption; // Термiн споживання
	float price; // Цiна
	int count, category, pidCategory; // Кiлькiсть, категорiя ,пiд категорiя 
	//friend void Init(Shop &shop);
public:

	Product() {}

	Product(const string &name, const int &day, const int &month, const int &year, const float &price, const int &count, const int &category, const int & pidCategory) {
		SetAllComponents(name, day, month, year, price, count, category, pidCategory);
	}

	bool IsOkName(const string &name) const {
		if (name.length() > 0) return true;
		else return false;
	}

	bool IsOkPrice(const float &price) const {
		if (price > 0) return true;
		else return false;
	}

	bool IsOkCount(const int &count) const {
		if (count > -1) return true;
		else return false;
	}

	bool SetCount(const int &count) {
		if (IsOkCount(count)) {
			this->count = count;
			return true; 
		}
		else return false;
	}

	bool IsOkDate(const int &day, const int &month, const int &year) const {
		Date testdate;
		if (testdate.SetAll(day, month, year)) {
			Date thisdate;
			if (thisdate < testdate) {
				
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool IsOkDate(const Date &date) const {
		return IsOkDate(date.GetDay(), date.GetMonth(), date.GetYear());
	}

	bool SetTermOfConsumption(const int &day, const int &month, const int &year) {
		if (IsOkDate(day, month, year)) {
			this->termOfConsumption.SetAll(day, month, year);
			return true;
		}
		else return false;
	}

	bool SetTermOfConsumption(const Date &date) {
		return SetTermOfConsumption(date.GetDay(), date.GetMonth(), date.GetYear());
	}


	bool SetPrice(const float &price) {
		if (price > 0) {
			this->price = price;
		}
		else return false;
	}

	bool SetName(const string &name) {
		if (name.length() > 0) {
			this->name = name;
			return true;
		}
		else return false;
	}
	
	bool SetPidCategory(const int &pidCategory) {
		
			if (ACtg.ctg[category].IsOkPidCategory(pidCategory)) {
				this->pidCategory = pidCategory;
				return true;
			}
			else return false;
	}

	bool SetCategory(const int & category, const int &pidCategory) {
		if (ACtg.isOkCategory(category)) {
			if (ACtg.ctg[category].IsOkPidCategory(pidCategory)) {
				this->category = category;
				this->pidCategory = pidCategory;
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool SetAllComponents(const string &name, const int &day, const int &month, const int &year, const float &price, const int &count, const int &category, const int & pidCategory) {
		if (IsOkName(name) && IsOkDate(day, month, year) && IsOkPrice(price) && IsOkPrice(price) && IsOkCount(count) && ACtg.isOkCategory(category)) {
			if (ACtg.ctg[category].IsOkPidCategory(pidCategory)) {
				this->name = name;
				this->termOfConsumption.SetAll(day, month, year);
				this->price = price;
				this->count = count;
				this->category = category;
				this->pidCategory = pidCategory;
				return true;
			}
			else return false;
		}
		else return false;
	}

	string GetName() const {
		return this->name;
	}

	float GetPrice() const {
		return this->price;
	}

	Date GetTermOfConsumption() const {
		return this->termOfConsumption;
	}

	int GetCount() const {
		return this->count;
	}

	int GetCategory() const {
		return this->category;
	}

	int GetPidCategory() const {
		return this->pidCategory;
	}
};

class AllProducts {
	
	//friend void Init(Shop &shop);
public:

	vector<Product> products;

	bool AddProduct(const string &name, const int &day, const int &month, const int &year, const float &price, const int &count, const int &category, const int & pidCategory) {
		products.push_back({});
		if (products[products.size() -1].SetAllComponents(name, day, month, year, price, count, category, pidCategory ))return true;
		else {
			products.pop_back();
			return false;
		}
		
	}

	bool ISOkIdProduct(const int &id) {
		if (id > -1 && id < products.size()) return true;
		else return false;
	}

	bool RemoveProduct(const int &id) {
		if (ISOkIdProduct(id)) {
			products.erase(products.begin() + id);
			return true;
		}
		else return false;
	}
};

class Shop;

class Permission {
	string name, InMenu, message, nameMenu;
	bool permission = false;
	int countOfPidPerm = 0;
	
public:
	string(*func)(Shop &shop) = NULL;
	Permission * permissions = nullptr;
	Permission * nadPermission = nullptr;
	Permission() {};
	Permission(const string &name, const string &InMenu) {
		SetName(name);
		SetInMenu(InMenu);
	}

	Permission(const string &name, const string &InMenu, string(*func)(Shop &shop)) {
		SetName(name);
		SetInMenu(InMenu);
		if (func != nullptr) {
			this->func = func;
		}
	}

	Permission(const string &name) {
		SetName(name);
	}

	Permission(const string &name, const bool &perm) {
		SetName(name);
		SetPermission(perm);
	}

	Permission(const string &name, const int &countOfPidPerm, Permission *permissions) {
		SetName(name);
		if (countOfPidPerm > 0) {
			this->countOfPidPerm = countOfPidPerm;
			this->permissions = permissions;
			for (int i = 0; i < countOfPidPerm; i++) permissions[i].nadPermission = this;
			
		}
	}

	Permission(const string &name, const string &InMenu, const int &countOfPidPerm, Permission *permissions) {
		SetName(name);
		SetInMenu(InMenu);
		
		if (countOfPidPerm > 0) {
			this->countOfPidPerm = countOfPidPerm;
			this->permissions = permissions;
			for (int i = 0; i < countOfPidPerm; i++) permissions[i].nadPermission = this;
		}
	}

	Permission(const string &name, const string &message, const string &nameMenu, const string &InMenu, const int &countOfPidPerm, Permission *permissions) {
		SetName(name);
		SetInMenu(InMenu);
		SetMessage(message);
		SetNameMenu(nameMenu);
		if (countOfPidPerm > 0) {
			this->countOfPidPerm = countOfPidPerm;
			this->permissions = permissions;
			for (int i = 0; i < countOfPidPerm; i++) permissions[i].nadPermission = this;
		}
	}

	string GetName() const {
		return name;
	}

	string GetInMenu() const {
		return InMenu;
	}

	string GetNameMenu() const {
		return nameMenu;
	}

	string GetMessage() const {
		return message;
	}

	bool IsOkName(const string &name) const {
		if (name.length() > 0) return true;
		else return false;
	}

	bool IsOkInMenu(const string &InMenu) const {
		if (InMenu.length() > 0) return true;
		else return false;
	}

	bool IsOkMessage(const string &message) const {
		if (message.length() > 0) return true;
		else return false;
	}

	bool IsOkNameMenu(const string &nameMenu) const {
		if (nameMenu.length() > 0) return true;
		else return false;
	}

	bool GetPermission() const {
		return permission;
	}

	bool SetPermission(const bool &perm) {
		permission = perm;
		return true;
	}

	bool SetInMenu(const string &InMenu) {
		if (IsOkInMenu(InMenu)) {
			this->InMenu = InMenu;
			return true;
		}
		else return false;
	}

	bool SetNameMenu(const string &nameMenu) {
		if (IsOkInMenu(nameMenu)) {
			this->nameMenu = nameMenu;
			return true;
		}
		else return false;
	}


	bool SetMessage(const string &message) {
		if (IsOkMessage(message)) {
			this->message = message;
			return true;
		}
		else return false;
	}

	bool IsOkPidPermission(const int &to) const {
		if (to > -1 && to < countOfPidPerm) return true;
		else return false;
	}

	bool SetName(const string &name) {
		if (IsOkName(name)) {
			this->name = name;
			return true;
		}
		else return false;
	}

	bool SetPidPermission(const int &to, const bool &perm) {
		if (IsOkPidPermission(to)) {
			permissions[to].SetPermission(perm);
			return true;
		}
		else return false;
	}

	bool SetCountPidPermissions(const int &countOfPidPerm) {
		if (countOfPidPerm > 0) {
			this->countOfPidPerm = countOfPidPerm;
			return true;
		}
		else return false;
	}

	int GetCountOfPidPermissions() const {
		return countOfPidPerm;
	}

	/*void operator = (const string& name) {
		SetName(name);
	}*/
};

string AddNewCategory(Shop &shop);
string RenameCategoryOfProducts(Shop &shop);
string DeleteCategory(Shop &shop);
string AddPidCadegory(Shop &shop);
string RemovePidCadegory(Shop &shop);
string RenamePidCategory(Shop &shop);
string ShowTreeCategories(Shop &shop);
string AddProduct(Shop &shop);
string SetCountProduct(Shop &shop);
string SetPriceProduct(Shop &shop);
string SetTermin(Shop &shop);
string SetCategoryOfProduct(Shop &shop);
string SetPidCategoryOfProduct(Shop &shop);
string SetNameOfProduct(Shop &shop);
string BuyProduct(Shop &shop);

class TypeOfTrader /*Тип працiвника*/ {

	string name; // iмя типу працiвника

	static const int countOfPermsions = 2; // Кiлькiсть дозволiв

	float salary; // Зарплата

	

public:

	Permission permissions[countOfPermsions] = { /*Дозволи*/ 
		{string("дозвiл на редагування продуктiв"), string("Ви в меню редагування продуктiв!"), string("Меню редагування продуктiв"),string("Редагувати продукти"), 9 ,
			new Permission[9]{
				{ string("дозвiл на додавння продуктiв"), string("Додати продукт") , &AddProduct },
				{ string("дозвiл на редактування кiлькостi продукту"), string("Редагувати кiлькiсть продукту"), &SetCountProduct },
				{ string("дозвiл на редагування цiни продукту"), string("Редагувати цiну продукту"), &SetPriceProduct },
				{ string("дозвiл на редагування термiну споживання продукту"), string("Редагувати термiн споживання продукту"), &SetTermin },
				{ string("дозвiл на редагування категорiї продукту"), string("Редагувати категорiю продукту"), &SetCategoryOfProduct },
				{ string("дозвiл на редагування пiд категорiї продукту"), string("Редагувати пiд категорiї продукту"), &SetPidCategoryOfProduct },
				{ string("дозвiл на редагування iменi продукту") , string("Редагувати iм'я продукту") , &SetNameOfProduct },
				{ string("дозвiл на покупку продукту") , string("Пробити продукт на кассi"), &BuyProduct },
				{ string("дозвiл на редагування категорiй продуктiв"), string("Ви в меню редагування категорiй продуктiв"), string("Меню редагування категорiй продуктiв"), string("Редагувати категорiї продуктiв"), 5, new Permission[5]{
					{ string("дозвiл на додавання нової категорiї продуктiв") , string("Додати нову категорiю продуктiв") , &AddNewCategory },
					{ string("дозвiл на переiменування категорiї продуктiв") , string("Перейменувати категорiю продуктiв") , &RenameCategoryOfProducts},
					{ string("дозвiл на видалення категорiї продуктiв"), string("Видалити категорiю продуктiв"), &DeleteCategory },
					{ string("дозвiл на просмотр дерева категорій"), string("Подивитися дерево категорій"), &ShowTreeCategories },
					{ string("дозвiл на редагування пiд категорiї продуктiв"), string("Ви в меню редагування пiд категорiй продуктiв"), string("Меню редагування пiд кактегорiй продуктiв"), string("Редагувати пiд категорiї продуктiв") , 3, new Permission[4]{
						{ string("дозвiл на додавання пiд категорiяй продуктiв"), string("Додати пiд категорiю продуктiв"), &AddPidCadegory },
						{ string("дозвiл на видалення пiд категорiяй продуктiв"), string("Видалити пiд категорiю продуктiв"), &RemovePidCadegory },
						{ string("дозвiл на перейменування пiд категорiяй продуктiв"), string("Перейменувати пiд категорiю продуктiв"), &RenamePidCategory }
					}},
				}}
			}
		},
		{string("дозвiл на редагування працiвникiв"), string("Ви в меню редагування працiвникiв!"), string("Меню редагування працiвникiв"), string("Редагувати працiвникiв"), 7, new Permission[7]{
			{ string("дозвiл на редагування ПiБ працiвника"), string("Ви в меню редагування ПiБ працiвника"), string("Меню редагування ПiБ працiвника"), string("Редагувати ПiБ працiвника"), 3, new Permission[3]{
				{ string("дозвiл на редагування iменi працiвника"), string("Редагувати iм'я працiвника") },
				{ string("дозвiл на редагування прiзвища працiвника"), string("Редагувати прiзвище працiвника") },
				{ string("дозвiл на редагування по батьковi працiвника"), string("Редагувати по батьковi прцiвника") }
			}},
			{ string("дозвiл на редагування пароля працiвника"), string("Редагування пароль працiвника") },
			{ string("дозвiл на редагування емейла працiвника"), string("Редагувати емейл працiвника") },
			{ string("дозвiл на редагування дати народження працiвника"), string("Ви в меню редагування датит народження працiвника!"), string("Меню редагування дати народження працiвника"), string("Редагувати дату народження працiвника") , 3, new Permission[3]{
				{ string("дозвiл на редагування дня народження"), string("Редагувати день народження працiвника") },
				{ string("дозвiл на редагування мiсяця народження"), string("Редагувати мiсяць народження працiвника") },
				{ string("дозвiл на редагування року народження") , string("Редагувати рiк народження працiвника")}
			}},
			{ string("дозвiл на додавання новох працiвникiв") , string("Додати нового працiвника") },
			{ string("дозвiл на звiльнення працiвникiв") , string("Звiльнитит працiвникiв") },
			{ string("дозвiл на редагування типiв працiвникiв") , string("Ви в меню редагування типiв працiвникiв"), string("Меню редагування типiв працiвникiв"), string("Редагувати типи працiвникiв"), 3, new Permission[3]{
				{ string("дозвiл на додавання типу працiвника") , string("Додати новий тип працівника") },
				{ string("дозвiл на редагування iменi типу працiвника") , string("Редагувати iм'я типу працiвника") },
				{ string("дозвiл на редагування зарплати типу працiвника") , string("Редагувати зарплату  типу працiвника") },
				{ string("дозвiл на видалення типу працiвника") , string("Видалити типи працівників") },
				{ string("дозвiл на редагування дозволiв"), string("Ви в меню редагування дозволiв типу працiвника"), string("Меню редагувння дозволiв типу працiвника"), string("Редагувати дозволи типу працiвника"), 2, new Permission[2]{
						{string("дозвiл на редагування дозволу редагування продуктiв"), string("Ви в меню дозволів на редагування пролуктів"), string("Меню редагування дозволів на редагування продуктів"), string("Редагувати дозвіл на редагування продуктів"), 9 , new Permission[9]{
							{ string("дозвiл на редагування дозволу на додавння продуктiв")},
							{ string("дозвiл на редагування дозволу на редактування кiлькостi продукту") },
							{ string("дозвiл на редагування дозволу на редагування цiни продукту") },
							{ string("дозвiл на редагування дозволу на редагування термiну споживання продукту") },
							{ string("дозвiл на редагування дозволу на редагування категорiї продукту") },
							{ string("дозвiл на редагування дозволу на редагування пiд категорiї продукту") },
							{ string("дозвiл на редагування дозволу на редагування iменi продукту") },
							{ string("дозвiл на редагування дозволу на покупку продукту") },
							{ string("дозвiл на редагування дозволу на редагування категорiй продуктiв"), 4, new Permission[4]{
								{ string("дозвiл на дозвiл на додавання нової категорiї продуктiв") },
								{ string("дозвiл на дозвiл на переiменування категорiї продуктiв") },
								{ string("дозвiл на дозвiл на видалення категорiї продуктiв") },
								{ string("дозвiл на дозвiл на видалення категорiї продуктiв") , 3, new Permission[4]{
									{ string("дозвiл на дозвiл на видалення пiд категорiяй продуктiв") },
									{ string("дозвiл на дозвiл на перейменування пiд категорiяй продуктiв") },
									{ string("дозвiл на дозвiл на видалення пiд категорiяй продуктiв")}
								}},
							}}
						}
					},
					{string("дозвiл на редагування дозволу на редагування працiвникiв"), 7, new Permission[7]{
						{ string("дозвiл на дозвiл на редагування ПiБ працiвника"), 3, new Permission[3]{
							{ string("дозвiл на дозвiл на редагування iменi працiвника") },
							{ string("дозвiл на дозвiл на редагування прiзвища працiвника") },
							{ string("дозвiл на дозвiл на редагування по батьковi працiвника") }
						}},
						{ string("дозвiл на дозвiл на редагування пароля працiвника") },
						{ string("дозвiл на дозвiл на редагування емейла працiвника") },
						{ string("дозвiл на дозвiл на редагування дати народження працiвника") , 3, new Permission[3]{
							{ string("дозвiл на дозвiл на редагування дня народження") },
							{ string("дозвiл на дозвiл на редагування мiсяця народження") },
							{ string("дозвiл на дозвiл на редагування року народження") }
						}},
						{ string("дозвiл на дозвiл на додавання новох працiвникiв") },
						{ string("дозвiл на дозвiл на звiльнення працiвникiв") },
						{ string("дозвiл на дозвiл на редагування типiв працiвникiв") , 3, new Permission[3]{
							{ string("дозвiл на дозвiл на редагування iменi типу працiвника") },
							{ string("дозвiл на дозвiл на редагування зарплати типу працiвника") },
							{ string("дозвiл на дозвiл на редагування дозволiв") }
						}}
					}}
				}}
			}}	
		}}
	};

	/*
		string("дозвiл на редагування продуктiв") :
			string("дозвiл на додавння продуктiв")
			string("дозвiл на редактування кiлькостi продукту")
			string("дозвiл на редагування цiни продукту")
			string("дозвiл на редагування термiну споживання продукту"):
				string("дозвiл на редагування дня термiну споживання продукту),
				string("дозвiл на редагування мiсяця термiну споживання продукту"),
				string("дозвiл на редагування hоку термiну споживання продукту"),
			string("дозвiл на редагування категорiї продукту")
			string("дозвiл на редагування пiд категорiї продукту")
			string("дозвiл на редагування iменi продукту")
			string("дозвiл на покупку продукту")
			string("дозвiл на редагування категорiй продуктiв"):
				string("дозвiл на додавання нової категорiї продуктiв"),
				string("дозвiл на переiменування категорiї продуктiв"),
				string("дозвiл на видалення категорiї продуктiв"),
				string("дозвiл на редагування пiд категорiй продуктiв"):
					string("дозвiл на видалення пiд категорiяй продуктiв"),
					string("дозвiл на перейменування пiд категорiяй продуктiв"),
					string("дозвiл на видалення пiд категорiяй продуктiв"),
		string("дозвiл на редагування працiвникiв") :
			string("дозвiл на редагування ПiБ працiвника"):
				string("дозвiл на редагування iменi працiвника"),
				string("дозвiл на редагування прiзвища працiвника"),
				string("дозвiл на редагування по батьковi працiвника")
			string("дозвiл на редагування пароля працiвника")
			string("дозвiл на редагування емейла працiвника")
			string("дозвiл на редагування дати народження працiвника"):
				string("дозвiл на редагування дня народження"),
				string("дозвiл на редагування мiсяця народження"),
				string("дозвiл на редагування Року народження"),
			string("дозвiл на додавання новох працiвникiв"),
			string("дозвiл на звiльнення працiвникiв")
			string("дозвiл на редагування типiв працiвникiв"):
				string("дозвiл на редагування iменi типу працiвника"),
				string("дозвiл на редагування зарплати типу працiвника"),
				string("дозвiл на редагування дозволiв"):
					string("дозвiл на редагування дозволу на редагування продуктiв") :
							string("дозвiл на редагування дозволу на додавння продуктiв")
							string("дозвiл на редагування дозволу на редактування кiлькостi продукту")
							string("дозвiл на редагування дозволу на редагування цiни продукту")
							string("дозвiл на редагування дозволу на редагування термiну споживання продукту"):
								string("дозвiл на редагування дозволу на редагування дня термiну споживання продукту),
								string("дозвiл на редагування дозволу на редагування мiсяця термiну споживання продукту"),
								string("дозвiл на редагування дозволу на редагування hоку термiну споживання продукту"),
							string("дозвiл на редагування дозволу на редагування категорiї продукту")
							string("дозвiл на редагування дозволу на редагування пiд категорiї продукту")
							string("дозвiл на редагування дозволу на редагування iменi продукту")
							string("дозвiл на редагування дозволу на покупку продукту")
							string("дозвiл на редагування дозволу на редагування категорiй продуктiв"):
								string("дозвiл на редагування дозволу на додавання нової категорiї продуктiв"),
								string("дозвiл на редагування дозволу на переiменування категорiї продуктiв"),
								string("дозвiл на редагування дозволу на видалення категорiї продуктiв"),
								string("дозвiл на редагування дозволу на редагування пiд категорiй продуктiв"):
									string("дозвiл на редагування дозволу на видалення пiд категорiяй продуктiв"),
									string("дозвiл на редагування дозволу на перейменування пiд категорiяй продуктiв"),
									string("дозвiл на редагування дозволу на видалення пiд категорiяй продуктiв"),
					string("дозвiл на редагування дозволу на редагування працiвникiв") :
						string("дозвiл на редагування дозволу на редагування ПiБ працiвника"):
							string("дозвiл на редагування дозволу на iменi працiвника"),
							string("дозвiл на редагування дозволу на прiзвища працiвника"),
							string("дозвiл на редагування дозволу на по батьковi працiвника")
						string("дозвiл на редагування дозволу на редагування пароля працiвника")
						string("дозвiл на редагування дозволу на редагування емейла працiвника")
						string("дозвiл на редагування дозволу на редагування дати народження працiвника"):
							string("дозвiл на редагування дозволу на редагування дня народження"),
							string("дозвiл на редагування дозволу на редагування мiсяця народження"),
							string("дозвiл на редагування дозволу на редагування Року народження"),
						string("дозвiл на редагування дозволу на додавання новох працiвникiв"),
						string("дозвiл на редагування дозволу на звiльнення працiвникiв")
						string("дозвiл на редагування дозволу на редагування типiв працiвникiв"):
							string("дозвiл на редагування дозволу на редагування iменi типу працiвника"),
							string("дозвiл на редагування дозволу на редагування зарплати типу працiвника"),
							string("дозвiл на редагування дозволу на редагування дозволiв")
			
				
				

	*/

	TypeOfTrader(const string &name, const float &salary) {
		SetName(name);
		SetSalary(salary);
	}

	bool IsOkSalary(const float &salary) const {
		if (salary > 0) return true;
		else return false;
	}

	bool IsOkPermission(const int &to) const {
		if (to > -1 && to < countOfPermsions) return true;
		else return false;
	}

	bool IsOkName(const string &name) const {
		if (name.length() > 0) return true;
		else return false;
	}

	bool SetName(const string &name) {
		if (IsOkName(name)) {
			this->name = name;
			return true;
		}
		else return false;
	}

	bool SetPermission(const int &to, const bool &perm) {
		if (IsOkPermission(to)) {
			permissions[to].SetPermission(perm);
			return true;
		}
		else return false;
	}

	bool SetSalary(const float &salary) {
		if (IsOkSalary(salary)) {
			this->salary = salary;
			return true;
		}
		else return false;
	}

	int GetCountOfPermissions() const {
		return countOfPermsions;
	}
};

class AllTypesOfTraders {
public:
	vector<TypeOfTrader * > TOFT;

	bool isOkIdTypeOfTrader(const int &count) const {
		if (TOFT.size() > count && count > -1) return true;
		else return false;
	}
};

AllTypesOfTraders typesOfTraders;

class Trader /*Працiвник*/ {
	string name, lastName, serName, passwordMD5, email; // iмя , прiзвище , по батьковi, зашифрований пароль, емейл
	Date birthday; // День народження
	int Type; // Тип працiвника

	string cryptPass(const string &pass) const {
		return pass;
	}
	//friend void Init(Shop &shop);
public:

	Trader() {};

	Trader(const string &name, const string &lastName, const string &serName, const string &password, const string &email, const int &day, const int &month, const int &year, const int &type) { // Встановлення всiх параметрiв при створенii елемента
		SetAll(name, lastName, serName, password, email, day, month, year, type);
	}

	bool IsOkName(const string &name) { // Проверка iменi
		if (name.length() > 0) return true;
		else return false;
	}

	bool IsOkEmail(const string& email) const { // Проверка Email
		const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		return regex_match(email, pattern);
	}

	bool IsOkLastName(const string &lastName) { // Перевiрка призвища
		if (lastName.length() > 0) return true;
		else return false;
	}

	bool IsOkSerName(const string &serName) { // Перевiрка по батьковi
		if (serName.length() > 0) return true;
		else return false;
	}

	bool IsOkDate(const int &day, const int &month, const int &year) const { // Перевiка дати
		Date testdate;
		if (testdate.SetAll(day, month, year)) {
			Date rsm, ofdate(testdate.GetDay(), testdate.GetMonth(), rsm.GetYear() - 18);
			if (testdate < ofdate) {
				return true;
			}
			else return false;
		}
		else return false;
	}

	bool SetName(const string &name) { // Встановлення iменi
		if (IsOkName(name)) {
			this->name = name;
			return true;
		}
		else return false;
	}

	bool SetLastName(const string &lastName) { // Встановлення прiзвища
		if (IsOkName(lastName)) {
			this->lastName = lastName;
			return true;
		}
		else return false;
	}

	bool SetSerName(const string &serName) { // Встановлення по батьковi
		if (IsOkName(serName)) {
			this->serName = serName;
			return true;
		}
		else return false;
	}

	bool SetDate(const int &day, const int &month, const int &year) { // Встановлення дати
		if (IsOkDate(day, month, year) ){
			birthday.SetAll(day, month, year);
			return true;
		}
		else return false;
	}

	bool IsOkPassword(const string &pass, const string &testpass = "") const {
		if (pass.size() > 0 && ((testpass.size() > 0 && cryptPass(testpass) == passwordMD5 && passwordMD5.size() > 0) || (passwordMD5.size() == 0))) {
			return true;
		}
		else return false;
	}

	bool SetPassword(const string &pass, const string &testpass = "") { // Встановлення пароля
		if (IsOkPassword(pass, testpass)) {
			passwordMD5 = cryptPass(pass);
			return true;
		}
		else return false;
	}

	bool SetEmail(const string &email) { // Встановлення email
		if (IsOkEmail(email)) {
			this->email = email;
			return true;
		}
		else return false;
	}

	bool SetAll(const string &name, const string &lastName, const string &serName, const string &password, const string &email, const int &day, const int &month, const int &year, const int &type) { // Встановлення всiх параметрiв
		if (IsOkName(name) && IsOkLastName(lastName) && IsOkSerName(serName) && IsOkDate(day, month, year) && typesOfTraders.isOkIdTypeOfTrader(type) && IsOkEmail(email) && IsOkPassword(password)) {
			birthday.SetAll(day, month, year);
			this->name = name;
			this->lastName = lastName;
			this->serName = serName;
			this->Type = type;
			this->passwordMD5 = cryptPass(password);
			this->email = email;
			return true;
		}
	}

	bool LogIn(const string &email, const string &pass) const {
		if (this->email == email && cryptPass(pass) == this->passwordMD5) return true;
		else return false;
	}

	string GetName() const { // Видобування iменi
		return name;
	}

	string GetLastName() const { // Видобування прiзвища
		return lastName;
	}

	string GetSerName() const { // Видобування по батьковi
		return serName;
	}
	
	string GetDate() const { // Видобування дати
		return birthday.GetAllStr();
	}

	int GetType() const { // Видобування типу працiвника
		return Type;
	}
};

class AllTraders {
	vector<Trader *> traders;
	vector<Trader *> removedTraders;
	
public:

	bool AddTrader(const string &name, const string &lastName, const string &serName, const string &password, const string &email, const int &day, const int &month, const int &year, const int &type) {
		Trader test;
		if (test.IsOkName(name) && test.IsOkLastName(lastName) && test.IsOkSerName(serName) && test.IsOkDate(day, month, year) && typesOfTraders.isOkIdTypeOfTrader(type) && test.IsOkEmail(email) && test.IsOkPassword(password)) {
			traders.push_back(new Trader(name, lastName, serName, password, email, day, month, year, type));
			return true;
		}
		else
		return false;
	}

	bool IsOkIdTrader(const int &id) const {
		if (id > -1 && id < traders.size()) {
			return true;
		}
		else return false;
	}

	bool IsOkIdRemovedTraders(const int &id) const {
		if (id > -1 && id < removedTraders.size()) {
			return true;
		}
		else return false;
	}

	bool RemoveTrader(const int &id) {
		if (IsOkIdTrader(id)) {
			removedTraders.push_back(traders[id]);
			traders.erase(traders.begin() + id);
			return true;
		}
		else return false;
	}

	bool ReresumeWorker(const int &RemId) {
		if (IsOkIdRemovedTraders(RemId)) {
			traders.push_back(removedTraders[RemId]);
			removedTraders.erase(removedTraders.begin() + RemId);
			return true;
		}
		else return false;
	}

	Trader * LogIn(const string &email, const string &pass) {
		for (int i = 0; i < traders.size(); i++) if (traders[i]->LogIn(email, pass)) return traders[i];
		return nullptr;
	}

	// Friends

	friend class Shop;
	friend void Init(Shop &shop);
	friend void InitLLC(Shop &shop);
};

class Shop {
	AllProducts products;
	AllTraders traders;
	
public:
	Trader * user = nullptr;
	bool LogIn(const string &email, const string &pass) {
		user = traders.LogIn(email, pass);
		if (user != nullptr) return true;
		else return false;
			
	}



	// Friends

	friend void Init(Shop &shop);
	friend void InitLLC(Shop &shop);
	friend string AddProduct(Shop &shop);
	friend int SelectOneProduct(Shop &, string *);
	friend string SetCountProduct(Shop &shop);
	friend string SetPriceProduct(Shop &shop);
	friend string SetTermin(Shop &shop);
	friend string SetCategoryOfProduct(Shop &shop);
	friend string SetPidCategoryOfProduct(Shop &shop);
	friend string SetNameOfProduct(Shop &shop);
	friend string BuyProduct(Shop &shop);
	friend bool SaveProducts(ofstream & FileToSave, Shop &shop);
	friend bool LoadProducts(ifstream & FileToLoad, Shop &shop);
};

void printHeader(const string& str) {
	for (int i = 0; i < (consoleSize.x - str.size()) / 2 - 1; i++) cout << '\0';
	cout << char(201);
	for (int i = 0; i < str.size(); i++) cout << char(205);
	cout << char(187);
	cout << endl;
	
	cout << char(201);
	for (int i = 0; i < (consoleSize.x - str.size()) / 2 - 2; i++) cout << char(205);
	UINT rr = GetConsoleCP();
	
	cout << char(185);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << str;
	SetConsoleCP(rr);
	SetConsoleOutputCP(rr);
	cout << char(204);

	for (int i = 0; i < (consoleSize.x - str.size()) / 2 - 2; i++) cout << char(205);
	gotoxy(consoleSize.x - 3, GetPosCur().y);
	cout << char(205) << char(187) << endl;
	cout << char(204);
	for (int i = 0; i < (consoleSize.x - str.size()) / 2 - 2; i++) cout << char(205);
	cout << char(202);
	for (int i = 0; i < str.size(); i++) cout << char(205);
	cout << char(202);
	for (int i = 0; i < (consoleSize.x - str.size()) / 2 - 2; i++) cout << char(205);
	gotoxy(consoleSize.x - 3, GetPosCur().y);
	cout << char(205) << char(185) << endl;
}

void printLine(string str, int size = consoleSize.x,const int textColor = 7, const int background = 0) {
	SetColorConsole(textColor, background); // Змiна кольору консолi 15 колiр тла, 0 колiр тексту
	if (str.size() > size - 3) {
		str.erase(size - 3);
		str += "...";
	}
	for (int i = 0; i < (size - str.size()) / 2; i++) cout << " "; // Виведення вiдступу
	UINT rr = GetConsoleCP();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << str;
	SetConsoleCP(rr);
	SetConsoleOutputCP(rr);
	for (int i = 0; i < (size - str.size()) / 2 + 1; i++) cout << " "; // виведення другого вiдступу
	SetColorConsole(); // Певернення консолi до початкового стану
}

int CoutMenu(vector<string> *str, const int &def = 0, POINT pos = GetPosCur()) {
	if (def < str->size() && def > -1) {
		int ch = 0, start = 0, finish = (consoleSize.y - pos.y) / 2, item = def, cout_item = def; // нажата кнопка, iндекс з якого виводимо , iндекс на якому закiнчуємо виводити, позицыя курсора вибору
		bool reload = true, exit = false; // Флажок перерисовки, флажок виходу
		UINT mm = GetConsoleCP();
		gotoxy(pos.x, pos.y); // Перемiщуємося в початкову точку
		for (int i = 0; i < str->size() && i < finish; i++) // Виводимо сiтку(Заготовку де будуть розмiщуватися самi ыекштп ektvtynb)
		{
			cout << char(186);
			gotoxy(consoleSize.x - 2, GetPosCur().y);
			cout << char(186);
			cout << endl;
			if (str->size() - 1 != i) {
				cout << char(199);
				for (int i = 0; i < consoleSize.x - 3; i++) cout << char(196);
				cout << char(182);
			}
			else {
				cout << char(200);
				for (int i = 0; i < consoleSize.x - 3; i++) cout << char(205);
				cout << char(188);
			}
			cout << endl;
		}
		gotoxy(pos.x, pos.y);

		while (!exit)
		{
			if (reload) {

				for (int i = start; i < str->size() && i < finish; i++)
				{
					gotoxy(1, pos.y + ((i - start) * 2));
					if ((*str)[i].size() > consoleSize.x - 5) {
						(*str)[i].erase(consoleSize.x - 5);
						(*str)[i] += "...";
					}
					printLine((*str)[i], consoleSize.x - 4);
				}
			}

			if (item != cout_item || reload == true) {
				if (cout_item >= start && cout_item <= finish) {
					gotoxy(1, pos.y + ((cout_item - start) * 2));
					printLine((*str)[cout_item], consoleSize.x - 4);
				}
				gotoxy(1, pos.y + ((item - start) * 2));
				printLine((*str)[item], consoleSize.x - 4, 0, 15);
				cout_item = item;
			}
			reload = false;
			ch = _getch();
			switch (ch)
			{
			case 80:
				if (item < str->size() - 1) item++;
				else ch = 0;
				if (item + 1 > finish && finish < str->size() && ch != 0) {
					finish++;
					start++;
					reload = true;
				}
				break;
			case 72:
				if (item > 0) item--;
				else ch = 0;
				if (item < start && start > 0 && ch != 0) {
					finish--;
					start--;
					reload = true;
				}
				break;
			case 13:
				exit = true;
				break;
			}
		}
		return item;
	}
	else return -1;
}

//int CoutMenu(vector<string> *menus) {
//	char ch = 0;
//	int selected = 1, cout_selected = 0;
//	POINT startCout = GetPosCur();
//	
//	for (int i = 0; i < menus->size(); i++) {
//		cout << char(186);
//		printLine((*menus)[i], consoleSize.x - 3);
//		gotoxy(consoleSize.x - 2, startCout.y + (i * 2));
//		cout << char(186);
//		cout << endl;
//		if (menus->size() - 1 != i) {
//			cout << char(199);
//			for (int i = 0; i < consoleSize.x - 3; i++) cout << char(196);
//			cout << char(182);
//		}
//		else {
//			cout << char(200);
//			for (int i = 0; i < consoleSize.x - 3; i++) cout << char(205);
//			cout << char(188);
//		}
//		cout << endl;
//	}
//	while (ch != 13)
//	{
//		if (selected != cout_selected) {
//			gotoxy(1, startCout.y + (cout_selected*2));
//			printLine((*menus)[cout_selected], consoleSize.x - 3);
//			gotoxy(1, startCout.y + (selected*2));
//			printLine((*menus)[selected], consoleSize.x - 3, 0, 15);
//			cout_selected = selected;
//			for (int i = 0; i < menus->size(); i++)
//			{
//				gotoxy(consoleSize.x - 2, startCout.y + (i * 2));
//				cout << char(186);
//			}
//		}
//		ch = _getch();
//		switch (ch)
//		{
//			case 80:
//				if (selected < menus->size() - 1) selected++;
//				else selected = 0;
//			break;
//			case 72: 
//				if (selected > 0) selected--;
//				else selected = menus->size() - 1;
//			break;
//		}
//	}
//	return selected;
//}

void SetAll(Permission *pr) {
	for (int i = 0; i < pr->GetCountOfPidPermissions(); i++) SetAll(&(pr->permissions[i]));
	pr->SetPermission(true);
}

string GetLine(POINT pos = GetPosCur(), string in = "",const int &count = 0, const bool &pass = false) {
	gotoxy(pos.x, pos.y);
	SetVisibleCursor(true);
	int symGet, ii = in.size();
	UINT rr = GetConsoleCP();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	clearLine(2);
	if (!pass) cout << in;
	else for (int i = 0; i < in.size(); i++) cout << '*';
	while (true) {
		//scanf_s("%c", &symGet);
		symGet = _getch();
		if (symGet == 224)
		{
			switch (_getch())
			{
			case 77:
				if (ii < in.size()) {
					ii++;
					gotoxy(pos.x + ii, pos.y);
				}
				symGet = 0;
				break;

			case 75:
				if (ii > 0) {
					ii--;
					gotoxy(pos.x + ii, pos.y);
				}
				symGet = 0;
				break;
			default:
				symGet = 224;
			}
		}
		if (symGet > 0) {
			if (symGet != 8 && symGet != 13 && (in.size() < count || count == 0)) {
				
				in.insert(in.begin() + ii, char(symGet));
				ii++;
				gotoxy(pos.x, pos.y);
				if (!pass) cout << in;
				else for (int i = 0; i < in.size(); i++) cout << '*';
				gotoxy(pos.x + ii, pos.y);
			}
			else if (symGet == 8 && ii > 0) {
				
				in.erase(in.begin() + --ii);
				gotoxy(pos.x, pos.y);
				if (!pass) cout << in;
				else for (int i = 0; i < in.size(); i++) cout << '*'; 
				cout << '\0';
				gotoxy(pos.x + ii, pos.y);
			}
			else if (symGet == 13) break;
			
		}
	}
	SetVisibleCursor(false);
	SetConsoleCP(rr);
	SetConsoleOutputCP(rr);
	return in;
}

Date GetDate(POINT pos = GetPosCur(), Date *in = new Date) {
	gotoxy(pos.x, pos.y);
	int symGet, posCur = 0, coutPosCur = 0, inn;
	bool reload = true, exit = false, ss;
	clearLine(2);
	cout << in->GetDay();
	gotoxy(pos.x + 2, pos.y);
	cout << "." << in->GetMonth();
	gotoxy(pos.x + 5, pos.y);
	cout << "." << in->GetYear();
	while (!exit) {
		if (posCur != coutPosCur || reload == true) {
			int plus = 3 * coutPosCur;
			gotoxy(pos.x + plus, pos.y);
			if (coutPosCur == 0) {
				if (in->GetDay() < 10) cout << "0";
				cout << in->GetDay();
			}
			else if (coutPosCur == 1) {
				if (in->GetMonth() < 10) cout << "0";
				cout << in->GetMonth();
			}
			else if (coutPosCur == 2) cout << in->GetYear() << " ";

			SetColorConsole(0, 15);
			plus = 3 * posCur;
			gotoxy(pos.x + plus, pos.y);
			if (posCur == 0) {
				if (in->GetDay() < 10) cout << " ";
				cout << in->GetDay();
			}
			else if (posCur == 1) {
				if (in->GetMonth() < 10) cout << " ";
				cout << in->GetMonth();
			}
			else if (posCur == 2) {
				cout << in->GetYear();
				SetColorConsole();
				cout << " ";
			}
			SetColorConsole();
			coutPosCur = posCur;
			reload = false;
		}
		symGet = _getch();
		if (symGet == 224)
		{
			symGet = _getch();
			switch (symGet)
			{
			case 77: // Вправо
				if (posCur < 2) posCur++;
				break;
			case 75: // Вліво
				if (posCur > 0) posCur--;
				break;
			case 80: // Вниз
			{
				
				if (posCur == 0) {
					inn = in->GetDay();
				}
				else if (posCur == 1) {
					inn = in->GetMonth();
				}
				else if (posCur == 2) {
					inn = in->GetYear();
				}
				if (inn > 0) {
					inn--;
					if (posCur == 0) ss = in->SetDay(inn);
					else if (posCur == 1) ss = in->SetMonth(inn);
					else if (posCur == 2) ss =  in->SetYear(inn);
					if (ss) reload = true;
				}
			}
				break;
			case 72: // Вверх
			{

				if (posCur == 0) {
					inn = in->GetDay();
				}
				else if (posCur == 1) {
					inn = in->GetMonth();
				}
				else if (posCur == 2) {
					inn = in->GetYear();
				}
				if (inn > 0) {
					inn++;
					if (posCur == 0) ss = in->SetDay(inn);
					else if (posCur == 1) ss = in->SetMonth(inn);
					else if (posCur == 2) ss = in->SetYear(inn);
					if (ss) reload = true;
				}
			}
				break;
			}
		}
		else if (symGet == 13) exit = true;
	}
	return *in;
}

POINT BoxGet(const string &str) {
	POINT pos;
	UINT rr = GetConsoleCP();
	cout << char(186);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << str;
	SetConsoleCP(rr);
	SetConsoleOutputCP(rr);
	cout << char(186) << ' ';
	pos = GetPosCur();
	cout << endl;
	cout << char(200);
	for (int i = 0; i < consoleSize.x - 2; i++) cout << char(205);
	gotoxy(consoleSize.x - 3, GetPosCur().y);
	cout << char(205) << char(188) << endl;
	gotoxy(pos.x - 2, pos.y - 1);
	cout << char(203);
	gotoxy(pos.x - 2, pos.y + 1);
	cout << char(202);
	
	gotoxy(consoleSize.x - 2, pos.y);
	cout << char(186);
	gotoxy(pos.x, pos.y);
	return pos;
}

bool LogIn(Shop &shop) {
	
	POINT pos, pp, pr;
	string pass;
	string email;
	bool ret = false;
	UINT rr = GetConsoleCP();
	gotoxy(0, 0);
	printHeader("Вхiд до бази данних");
	pp = GetPosCur();
	pr = GetPosCur();
	do
	{
		gotoxy(0, 0);
		printHeader("Вхiд до бази данних");
		gotoxy(pp.x, pp.y);
		{
			Trader rr;
			pos = BoxGet("Введiть емейл ");
			do
			{
				email = GetLine(pos, email, consoleSize.x - GetPosCur().x - 2);
			} while (!rr.IsOkEmail(email));
		}

		gotoxy(pp.x, pp.y);
		pos = BoxGet("Введiть пароль");
		do
		{
			pass = GetLine(pos, pass, consoleSize.x - GetPosCur().x - 2, true);
		} while (pass.size() < 1);
		ret = shop.LogIn(email, pass);
		if (ret == false) {
			gotoxy(pr.x, pr.y);
			cout << char(186);
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			printLine("Неправильний емейл або пароль!", consoleSize.x - 3, 12);
			SetConsoleCP(rr);
			SetConsoleOutputCP(rr);
			gotoxy(consoleSize.x-1, GetPosCur().y);
			cout << char(186) << endl << char(204);
			for (int i = 0; i < consoleSize.x - 2; i++) cout << char(205);
			cout << char(185);
			cout << endl;
			pp = GetPosCur();
		}
	} while (ret == false);

	return ret;
}

void InitLLC(Shop &shop) {
	if (shop.traders.traders.size() == 0) { // Якщо програма запущена перший раз
		typesOfTraders.TOFT.push_back(new TypeOfTrader("root", 1)); // Добавляємо новий тип працiвника
		for (int i = 0; i < typesOfTraders.TOFT[0]->GetCountOfPermissions(); i++)  SetAll(&(typesOfTraders.TOFT[0]->permissions[i]));
		shop.traders.AddTrader("root", "root", "root", "1111", "1@1.1", 1, 1, 1900, 0);
	}
}

void Init(Shop &shop) {
	if (shop.traders.traders.size() == 0) { // Якщо програма запущена перший раз
		typesOfTraders.TOFT.push_back(new TypeOfTrader("root", 1)); // Добавляємо новий тип працiвника
		for (int i = 0; i < typesOfTraders.TOFT[0]->GetCountOfPermissions(); i++)  SetAll(&(typesOfTraders.TOFT[0]->permissions[i]));
		POINT pos, pp;
		string pass, Npass;
		string email;
		
		printHeader("Перший запуск бази данних");
		pp = GetPosCur();
		{
			Trader rr;
			pos = BoxGet("Введiть новий емейл ");
			do
			{
				email = GetLine(pos, email, consoleSize.x - GetPosCur().x - 2);
			} while (!rr.IsOkEmail(email));
		}
		gotoxy(pp.x, pp.y);
		pos = BoxGet("Введiть новий пароль");
		do
		{
			pass = GetLine(pos, pass, consoleSize.x - GetPosCur().x - 2, true);
		} while (pass.size() < 1);
		
		gotoxy(pp.x, pp.y);
		pos = BoxGet("Повторiть новий пароль");
		do
		{
			Npass = GetLine(pos, Npass, consoleSize.x - GetPosCur().x - 2, true);
		} while (Npass != pass);
		shop.traders.AddTrader("root", "root", "root", pass, email, 1, 1, 1900, 0);
	}
}

void PrintMessage(string &msg) {
	if (msg.size() > 0) {
		UINT mm = GetConsoleCP();
		cout << char(186);
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		printLine(msg, consoleSize.x - 3, 2);
		SetConsoleCP(mm);
		SetConsoleOutputCP(mm);
		gotoxy(consoleSize.x - 2, GetPosCur().y);
		cout << char(186) << endl << char(204);
		for (int i = 0; i < consoleSize.x - 3; i++) cout << char(205);
		cout << char(185);
		cout << endl;
		msg = "";
	}
}


void ShowItems(vector<string> *str, POINT pos = GetPosCur()) {
	pos.x = 0; // О
	int ch = 0, start = 0, finish = (consoleSize.y - pos.y) / 2; // нажата кнопка, iндекс з якого виводимо , iндекс на якому закiнчуємо виводити, позицыя курсора вибору
	bool reload = true, exit = false; // Флажок перерисовки, флажок виходу
	UINT mm = GetConsoleCP();
	string Options = "Кнопка B = Вийти"; // Зберiгаєм начальну locale
	gotoxy(pos.x, pos.y); // Перемiщуємося в початкову точку
	for (int i = 0; i < str->size() && i < finish; i++) // Виводимо сiтку(Заготовку де будуть розмiщуватися самi ыекштп ektvtynb)
	{
		cout << char(186);
		gotoxy(consoleSize.x - 2, GetPosCur().y);
		cout << char(186);
		cout << endl;
		cout << char(199);
		for (int i = 0; i < consoleSize.x - 2; i++) cout << char(196);
		gotoxy(consoleSize.x - 3, GetPosCur().y);
		cout << char(196) << char(182) << endl;
	}
	cout << char(186);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << Options;
	SetConsoleCP(mm);
	SetConsoleOutputCP(mm);
	gotoxy(consoleSize.x - 2, GetPosCur().y);
	cout << char(186);
	cout << endl;
	cout << char(200);
	for (int i = 0; i < consoleSize.x - 2; i++) cout << char(205);
	gotoxy(consoleSize.x - 3, GetPosCur().y);
	cout << char(205) << char(188);
	gotoxy(pos.x, pos.y);

	while (!exit)
	{
		if (reload) {

			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);

			for (int i = start; i < str->size() && i < finish; i++)
			{
				gotoxy(1, pos.y + ((i - start) * 2));
				if ((*str)[i].size() > consoleSize.x - 5) {
					(*str)[i].erase(consoleSize.x - 5);
					(*str)[i] += "...";
				}
				cout << (*str)[i];
				for (int s = 0; s < consoleSize.x - 3 - (*str)[i].size(); s++) cout << " ";
			}

			SetConsoleCP(mm);
			SetConsoleOutputCP(mm);
			reload = false;
		}
		
		ch = _getch();
		switch (ch)
		{
		case 80:
			if (finish < str->size()) {
				finish++;
				start++;
				reload = true;
			}
			break;
		case 72:
			if (start > 0) {
				finish--;
				start--;
				reload = true;
			}
			break;
		case 98:
		case 168:
		{
			if (str->size() >= finish)
				gotoxy(1, pos.y + (finish - start) * 2);
			else gotoxy(1, pos.y + (str->size() - start) * 2);
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			cout << "Точно ви хочете вийти з цього меню (yes ? no): ";
			clearLine(2);
			SetConsoleCP(mm);
			SetConsoleOutputCP(mm);
			POINT opos = GetPosCur();
			string YN;
			do
			{
				YN = GetLine(opos, YN, consoleSize.x - GetPosCur().x - 2);
			} while (YN != "Yes" && YN != "No" && YN != "Y" && YN != "N" && YN != "y" && YN != "n" && YN != "yes" && YN != "no");
			if (YN == "Yes" || YN == "Y" || YN == "y" || YN == "yes") exit = true;
			else {
				if (str->size() >= finish)
					gotoxy(1, pos.y + (finish - start) * 2);
				else gotoxy(1, pos.y + (str->size() - start) * 2);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << Options;
				clearLine(2);
				cout << endl;
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
			}

		}
		}
	}
}


bool * SelectItems(vector<string> *str, const int &def = 0, POINT pos = GetPosCur()) {
	if (def < str->size() && def > -1) {
		pos.x = 0; // О
		int ch = 0, start = 0, finish = (consoleSize.y - pos.y) / 2, item = def, cout_item = def; // нажата кнопка, iндекс з якого виводимо , iндекс на якому закiнчуємо виводити, позицыя курсора вибору
		bool reload = true, exit = false, reload_item = true; // Флажок перерисовки, флажок виходу
		bool *masstoseind = new bool[str->size()]; // Массив вибраних 
		for (int i = 0; i < str->size(); i++) masstoseind[i] = false; // РОбимо усi флажки false
		UINT mm = GetConsoleCP();
		string Options = "Кнопка B = Вийти, S = Вибрати все, U = НЕ вибрати все, O = Готово"; // Зберiгаєм начальну locale
		gotoxy(pos.x, pos.y); // Перемiщуємося в початкову точку
		for (int i = 0; i < str->size() && i < finish; i++) // Виводимо сiтку(Заготовку де будуть розмiщуватися самi ыекштп ektvtynb)
		{
			cout << char(186);
			gotoxy(consoleSize.x - 2, GetPosCur().y);
			cout << char(186);
			cout << endl;
			cout << char(199);
			for (int i = 0; i < consoleSize.x - 2; i++) cout << char(196);
			gotoxy(consoleSize.x - 3, GetPosCur().y);
			cout << char(196) << char(182) << endl;
		}
		cout << char(186);
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		cout << Options;
		SetConsoleCP(mm);
		SetConsoleOutputCP(mm);
		gotoxy(consoleSize.x - 2, GetPosCur().y);
		cout << char(186);
		cout << endl;
		cout << char(200);
		for (int i = 0; i < consoleSize.x - 2; i++) cout << char(205);
		gotoxy(consoleSize.x - 3, GetPosCur().y);
		cout << char(205) << char(188);
		gotoxy(pos.x, pos.y);

		while (!exit)
		{
			if (reload) {

				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);

				for (int i = start; i < str->size() && i < finish; i++)
				{
					gotoxy(1, pos.y + ((i - start) * 2));
					if ((*str)[i].size() > consoleSize.x - 5) {
						(*str)[i].erase(consoleSize.x - 5);
						(*str)[i] += "...";
					}
					if (masstoseind[i] == true) {
						SetColorConsole(0, 10);
					}
					cout << (*str)[i];
					for (int s = 0; s < consoleSize.x - 3 - (*str)[i].size(); s++) cout << " ";
					SetColorConsole(7, 0);
				}

				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);

			}

			if (item != cout_item || reload == true || reload_item == true) {
				if (cout_item >= start && cout_item <= finish && item != cout_item) {
					gotoxy(1, pos.y + ((cout_item - start) * 2));
					if (masstoseind[cout_item] == true) {
						SetColorConsole(0, 10);
					}
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);

					cout << (*str)[cout_item];
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
					for (int s = 0; s < consoleSize.x - 3 - (*str)[cout_item].size(); s++) cout << " ";
					SetColorConsole(7, 0);
				}
				gotoxy(1, pos.y + ((item - start) * 2));
				if (masstoseind[item] == true) {
					SetColorConsole(15, 9);
				}
				else SetColorConsole(0, 14);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << (*str)[item];
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				for (int s = 0; s < consoleSize.x - 3 - (*str)[item].size(); s++) cout << " ";
				SetColorConsole(7, 0);
				cout_item = item;
				reload = false;
				reload_item = false;
			}
			
			ch = _getch();
			switch (ch)
			{
			case 80:
				if (item < str->size() - 1) item++;
				else ch = 0;
				if (item + 1 > finish && finish < str->size() && ch != 0) {
					finish++;
					start++;
					reload = true;
				}
				break;
			case 72:
				if (item > 0) item--;
				else ch = 0;
				if (item < start && start > 0 && ch != 0) {
					finish--;
					start--;
					reload = true;
				}
				break;
			case 98:
			case 168:
			{
				if (str->size() >= finish)
					gotoxy(1, pos.y + (finish - start) * 2);
				else gotoxy(1, pos.y + (str->size() - start) * 2);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << "Точно ви хочете вийти з цього меню (yes ? no): ";
				clearLine(2);
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				POINT opos = GetPosCur();
				string YN;
				do
				{
					YN = GetLine(opos, YN, consoleSize.x - GetPosCur().x - 2);
				} while (YN != "Yes" && YN != "No" && YN != "Y" && YN != "N" && YN != "y" && YN != "n" && YN != "yes" && YN != "no");
				if (YN == "Yes" || YN == "Y" || YN == "y" || YN == "yes") return nullptr;
				else {
					if (str->size() >= finish)
						gotoxy(1, pos.y + (finish - start) * 2);
					else gotoxy(1, pos.y + (str->size() - start) * 2);
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);
					cout << Options;
					clearLine(2);
					cout << endl;
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
				}

			}

			break;
			case 233:
			case 111:
			{
				if (str->size() >= finish)
					gotoxy(1, pos.y + (finish - start) * 2);
				else gotoxy(1, pos.y + (str->size() - start) * 2);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << "Точно готово (yes ? no): ";
				clearLine(2);
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				POINT opos = GetPosCur();
				string YN;
				do
				{
					YN = GetLine(opos, YN, consoleSize.x - GetPosCur().x - 2);
				} while (YN != "Yes" && YN != "No" && YN != "Y" && YN != "N" && YN != "y" && YN != "n" && YN != "yes" && YN != "no");
				if (YN == "Yes" || YN == "Y" || YN == "y" || YN == "yes") exit = true;
				else {
					if (str->size() >= finish)
						gotoxy(1, pos.y + (finish - start) * 2);
					else gotoxy(1, pos.y + (str->size() - start) * 2);
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);
					cout << Options;
					clearLine(2);
					cout << endl;
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
				}

			}

			break;
			case 13:
				if (!masstoseind[item]) {
					masstoseind[item] = true;
				}
				else masstoseind[item] = false;
				reload_item = true;
				break;
			case 63:
			case 115:
			case 235:
				for (int i = 0; i < str->size(); i++)
				{
					if (!masstoseind[i]) masstoseind[i] = 1;
				}
				reload = true;
				break;
			case 117:
			case 163:
				for (int i = 0; i < str->size(); i++)
				{
					if (masstoseind[i]) masstoseind[i] = 0;
				}
				reload = true;
				break;
			}
		}
		return masstoseind;
	}return nullptr;
}

void PerhodnikInputToMenu() {
	gotoxy(0, GetPosCur().y + 1);
	cout << char(204);
	gotoxy(consoleSize.x - 2, GetPosCur().y);
	cout << char(185);
	gotoxy(0, GetPosCur().y + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////
				// Збереження стандартних змінних в файл у бінарному вигляді
////////////////////////////////////////////////////////////////////////////////////////////

bool SaveIntBIN(ofstream & FileToSave, int itg) {
	if (FileToSave.is_open()) {
		FileToSave.write(reinterpret_cast<char *>(&itg), sizeof(int));
		return true;
	}
	else return false;
}

bool SaveFloatBIN(ofstream & FileToSave, float itg) {
	if (FileToSave.is_open()) {
		FileToSave.write(reinterpret_cast<char *>(&itg), sizeof(float));
		return true;
	}
	else return false;
}

bool SaveStringBIN(ofstream & FileToSave, string str) {
	if (FileToSave.is_open()) {
		SaveIntBIN(FileToSave, str.size());
		FileToSave << str.c_str();
		return true;
	}
	else return false;
}

bool SaveDateBIN(ofstream &FileToSave, Date dtd) {
	if (FileToSave.is_open()) {
		SaveIntBIN(FileToSave, dtd.GetYear());
		SaveIntBIN(FileToSave, dtd.GetMonth());
		SaveIntBIN(FileToSave, dtd.GetDay());
		return true;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
						// Виведення з бінарного файлу стандартних змінних
////////////////////////////////////////////////////////////////////////////////////////////

int LoadIntBIN(ifstream & FileToLoad, bool *state = nullptr) {
	int tmp;
	if (FileToLoad.is_open()) {
		FileToLoad.read(reinterpret_cast<char *>(&tmp), sizeof(int));
		if (state != nullptr) *state = true;
		return tmp;
	}
	else if (state != nullptr) *state = false;
	return -1;
}

float LoadFloatBIN(ifstream & FileToLoad, bool *state = nullptr) {
	float tmp;
	if (FileToLoad.is_open()) {
		FileToLoad.read(reinterpret_cast<char *>(&tmp), sizeof(float));
		if (state != nullptr) *state = true;
		return tmp;
	}
	else if (state != nullptr) *state = false;
	return -1;
}

string LoadStringBIN(ifstream & FileToLoad, bool *state = nullptr) {
	int strSize;
	char *temp;
	if (FileToLoad.is_open()) {
		strSize = LoadIntBIN(FileToLoad);
		temp = new char[strSize + 1];
		FileToLoad.read(reinterpret_cast<char *>(temp), strSize);
		temp[strSize] = '\0';
		if (state != nullptr) *state = true;
		return temp;
	}
	else if (state != nullptr) *state = false;
	return "";
}

Date LoadDateBIN(ifstream &FileToLoad, bool *state = nullptr) {
	Date temp;
	if (FileToLoad.is_open()) {

		temp.SetYear(LoadIntBIN(FileToLoad));
		temp.SetMonth(LoadIntBIN(FileToLoad));
		temp.SetDay(LoadIntBIN(FileToLoad));

		if (state != nullptr) *state = true;
		return temp;
	}
	else if (state != nullptr) *state = false;
	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////
				// Збереження екземплярів классу в бінарний файл
////////////////////////////////////////////////////////////////////////////////////////////

bool SaveCategories(ofstream & FileToSave) {
	if (FileToSave.is_open()) {
		SaveIntBIN(FileToSave, ACtg.ctg.size());

		for (int i = 0; i < ACtg.ctg.size(); i++)
		{
			SaveStringBIN(FileToSave, ACtg.ctg[i].GetName());
			SaveIntBIN(FileToSave, ACtg.ctg[i].GetCountOfPidCategories());
			for (int s = 0; s < ACtg.ctg[i].GetCountOfPidCategories(); s++) {
				SaveStringBIN(FileToSave, ACtg.ctg[i].GetNamePidCategory(s));
			}
		}
		return true;
	}
	else return false;
}

bool SaveProducts(ofstream & FileToSave, Shop &shop) {
	if (FileToSave.is_open()) {
		SaveIntBIN(FileToSave, shop.products.products.size());

		for (int i = 0; i < shop.products.products.size(); i++)
		{
			SaveStringBIN(FileToSave, shop.products.products[i].GetName());
			SaveDateBIN(FileToSave, shop.products.products[i].GetTermOfConsumption());
			SaveFloatBIN(FileToSave, shop.products.products[i].GetPrice());
			SaveIntBIN(FileToSave, shop.products.products[i].GetCount());
			SaveIntBIN(FileToSave, shop.products.products[i].GetCategory());
			SaveIntBIN(FileToSave, shop.products.products[i].GetPidCategory());
		}
		return true;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
				// Виведення з бінарного файлу екземплярів классу
////////////////////////////////////////////////////////////////////////////////////////////

bool LoadCategories(ifstream & FileToLoad) {
	if (FileToLoad.is_open()) {
		
		int size, PIDsize;

		size = LoadIntBIN(FileToLoad);

		for (int i = 0; i < size; i++)
		{
			ACtg.ctg.push_back(LoadStringBIN(FileToLoad));
			PIDsize = LoadIntBIN(FileToLoad);

			for (int s = 0; s < PIDsize; s++)
				ACtg.ctg[i].AddPidCategory(LoadStringBIN(FileToLoad));
		
		}
		return true;
	}
	else return false;
}

bool LoadProducts(ifstream & FileToLoad, Shop &shop) {
	if (FileToLoad.is_open()) {
		int size = LoadIntBIN(FileToLoad);

		for (int i = 0; i < size; i++)
		{
			shop.products.products.push_back({});

			shop.products.products[i].SetName(LoadStringBIN(FileToLoad));
			shop.products.products[i].SetTermOfConsumption(LoadDateBIN(FileToLoad));
			shop.products.products[i].SetPrice(LoadFloatBIN(FileToLoad));
			shop.products.products[i].SetCount(LoadIntBIN(FileToLoad));
			shop.products.products[i].SetCategory(LoadIntBIN(FileToLoad), LoadIntBIN(FileToLoad));
		}
		return true;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
						// Повне збереження в файл і виведення з файлу інформації 
////////////////////////////////////////////////////////////////////////////////////////////

bool LoadOfFile(const string &my, Shop &shop) {
	ifstream FileToLoad;
	FileToLoad.open(my, ios::binary);

	if (FileToLoad.is_open()) {
		if (LoadCategories(FileToLoad)) {
			if (LoadProducts(FileToLoad, shop)) {
				return true;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

bool SaveAll(const string &my, Shop &shop) {
	ofstream FileToSave;
	FileToSave.open(my, ios::binary);

	if (FileToSave.is_open()) {
		if (SaveCategories(FileToSave)) {
			if (SaveProducts(FileToSave, shop)) {
				return true;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
									// ФУНКЦІЇ В МЕНЮ
////////////////////////////////////////////////////////////////////////////////////////////

string AddNewCategory(Shop &shop) {
	cl();
	printHeader("Додавання нової категорiї продуктiв");
	POINT pos;
	string name;
	int in;
	vector<string> menu = {
		{ "Назад"},
		{ "Готово" },
		{ "Вихiд" }
	};
	pos = BoxGet("Введiть iм'я нової категорiї продуктiв ");
	
	do
	{
		back :
		name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

	} while (name.size() < 1);
	PerhodnikInputToMenu();
	
	in = CoutMenu(&menu, 1);
	if (in == 0) goto back;
	else if (in == 2) {
		menu.clear();
		return "Жодної категорiї не додано";
	}
	ACtg.ctg.push_back(name);
	menu.clear();
	return "Додана нова категорiя продуктiв " + name;
}

int SelectOneCategory(string *msg = new string) {
	cl();
	vector<string> menu;
	menu.push_back("Вийти з цього меню");
	for (int i = 0; i < ACtg.ctg.size(); i++) menu.push_back(ACtg.ctg[i].GetName());
	printHeader("Вибiр категорiї");
	PrintMessage(*msg);
	return CoutMenu(&menu);
}

int SelectOnePidCategory(Category *ctg, string *msg = new string) {
	cl();
	vector<string> menu;
	menu.push_back("Вийти з цього меню");
	for (int i = 0; i < ctg->GetCountOfPidCategories(); i++) menu.push_back(ctg->GetNamePidCategory(i));
	printHeader("Вибiр під категорiї");
	PrintMessage(*msg);
	return CoutMenu(&menu);
}

string RenameCategoryOfProducts(Shop &shop) {
	if (ACtg.ctg.size() > 0) {
		int Imenu;
		POINT pos;
		string name;
		int in;
		vector<string> menuu = {
			{ "Назад" },
		{ "Назад до вибору категорiї"},
		{ "Готово"},
		{ "Вихiд"}
		};

		selectCategory:
		Imenu = SelectOneCategory();
		if (Imenu > 0) {
			Imenu--;
			cl();
			printHeader("Перейменування категорiї продуктiв");
			
			pos = BoxGet("Введiть нове iм'я категорiї продуктiв " + ACtg.ctg[Imenu].GetName() + " ");

			do
			{
			back:
				name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

			} while (name.size() < 1);
			PerhodnikInputToMenu();

			in = CoutMenu(&menuu, 2);
			if (in == 0) goto back;
			else if (in == 1) goto selectCategory;
			else if (in == 3) {
				menuu.clear();
				return "Жодної категорiї не перейменовано";
			}
			ACtg.ctg[Imenu].SetName(name);
			menuu.clear();
			return "Перейменована категорiя продуктiв на " + name;
		}
		else return "Жодної категорiї не перейменовано";
	}else return "Не має категорiй продуктiв";
}

string DeleteCategory(Shop &shop) {
	if (ACtg.ctg.size() > 0) {
		cl();
		vector<string> menu;
		for (int i = 0; i < ACtg.ctg.size(); i++) menu.push_back(ACtg.ctg[i].GetName());
		printHeader("Видалення категорій");
		bool * get = SelectItems(&menu);
		if (get != nullptr) {
			int i = 0, r = 0;
			for (; i < ACtg.ctg.size();)
			{
				if (get[r] == true) {
					ACtg.ctg.erase(ACtg.ctg.begin() + i);
				}
				else i++;
				r++;	
			}
			if (i < r) return "Категорiї видалено";
			else return "Категорiї не видалено";
		} 
		else return "Категорiї не видалено";
	}
	else return "Не має категорiй продуктiв";
}

string AddPidCadegory(Shop &shop) {
	
	if (ACtg.ctg.size() > 0) {
		cl();
		int ICTG, in;
		POINT pos;
		string name;
		vector<string> menuu = {
			{ "Назад" },
			{ "Назад до вибору категорiї" },
			{ "Готово" },
			{ "Вихiд" }
		};
	SelectCategory:
		ICTG = SelectOneCategory();
		if (ICTG > 0) {
			ICTG--;
			cl();
			printHeader("Додавання нової під категорiї продуктiв");
			pos = BoxGet("Введiть iм'я нової під категорiї продуктiв ");
			do
			{
			SelectName:
				name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

			} while (name.size() < 1);
			PerhodnikInputToMenu();

			in = CoutMenu(&menuu, 2);
			if (in == 0) goto SelectName;
			else if (in == 1) goto SelectCategory;
			else if (in == 3) {
				menuu.clear();
				return "Жодної під категорiї не додано";
			}
			ACtg.ctg[ICTG].AddPidCategory(name);
			return "Додано нову під категорію " + name;
		}
		else return "Під категорії не додано";
	}
	else return "Не має категорій";
}

string RemovePidCadegory(Shop &shop) {

	if (ACtg.ctg.size() > 0) {
		int ICTG;
		bool * get;
		vector<string> menu;
		string msg;
		SelectCategory:
		cl();
		ICTG = SelectOneCategory(&msg);
		if (ICTG > 0) {
			ICTG--;
			if (ACtg.ctg[ICTG].GetCountOfPidCategories() > 0) {
			SelectPidCategory:
				cl();
				printHeader("Вибір під категорії для видалення");
				menu.clear();
				for (int i = 0; i < ACtg.ctg[ICTG].GetCountOfPidCategories(); i++) menu.push_back(ACtg.ctg[ICTG].GetNamePidCategory(i));
				get = SelectItems(&menu);
				if (get != nullptr) {
					int i = 0, r = 0;
					for (; i < ACtg.ctg[ICTG].GetCountOfPidCategories();)
					{
						if (get[r] == true) ACtg.ctg[ICTG].RemovePidCategory(i);
						else i++;
						r++;
					}
					if (i < r) return "Під категорiї видалено";
					else return "Під категорiї не видалено";
				}goto SelectCategory;
			}
			else { 
				msg = "Не має підкатегорій";
				goto SelectCategory;
			};
		}
		else return "Під категорії не додано";
	}
	else return "Не має категорій";
}

string RenamePidCategory(Shop &shop) {
	if (ACtg.ctg.size() > 0) {
		int ICTG, IPIDCTG, in;
		vector<string> menu;
		string msg, name;
		POINT pos;
		vector<string> menuu = {
			{ "Назад" },
		{ "Назад до вибору під категорiї" },
		{ "Готово" },
		{ "Вихiд" }
		};
		SelectCategory:
		cl();
		ICTG = SelectOneCategory(&msg);
		if (ICTG > 0) {
			ICTG--;
			if (ACtg.ctg[ICTG].GetCountOfPidCategories() > 0) {
				SelectPidCategory:
				cl();
				IPIDCTG = SelectOnePidCategory(&ACtg.ctg[ICTG], &msg);
				if (IPIDCTG > 0) {
					IPIDCTG--;
					cl();
					printHeader("Перейменування під категорiї продуктiв");
					pos = BoxGet("Введiть нове iм'я під категорiї продуктiв " + ACtg.ctg[ICTG].GetNamePidCategory(IPIDCTG) + " ");
					do
					{
					back:
						name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

					} while (name.size() < 1);
					PerhodnikInputToMenu();

					in = CoutMenu(&menuu, 2);
					if (in == 0) goto back;
					else if (in == 1) goto SelectPidCategory;
					else if (in == 3) {
						menuu.clear();
						return "Жодної під категорiї не перейменовано";
					}
					ACtg.ctg[ICTG].SetNamePidCategory(IPIDCTG, name);
					menuu.clear();
					return "Перейменована під категорiя продуктiв на " + name;
				}else goto SelectCategory;
			}
			else {
				msg = "Не має підкатегорій";
				goto SelectCategory;
			};
		}
		else return "Під категорії не додано";
	}
	else return "Не має категорій";
}

string ShowTreeCategories(Shop &shop){
	if (ACtg.ctg.size() > 0) {
		vector<string> menu;
		cl();
		printHeader("Дерево категорій");
		for (int i = 0; i < ACtg.ctg.size(); i++)
		{
			menu.push_back(ACtg.ctg[i].GetName());
			if (ACtg.ctg[i].GetCountOfPidCategories() > 0) {
				menu[menu.size() - 1] += " + ";
				for (int r = 0; r < ACtg.ctg[i].GetCountOfPidCategories(); r++) menu.push_back("     " + ACtg.ctg[i].GetNamePidCategory(r));
			}
		}

		ShowItems(&menu);
		return "";
	}
	else return "Не має категорій";
	
}



string AddProduct(Shop &shop) {
	if (ACtg.ctg.size() > 0) {
		for (int i = 0; i < ACtg.ctg.size(); i++)
		{
			if (ACtg.ctg[i].GetCountOfPidCategories() > 0) break;
			else if (i == ACtg.ctg.size() - 1) return "Не має під категорій";
		}

		// Змінні
		string name, msg;
		POINT pos;
		vector<string> menu;
		int IDMenu, ICTG, IPIDCTG, COUNT = 0;
		float PRICE = 0;
		Date term;
		Product *test;

		SelectCategory:
		ICTG = SelectOneCategory(&msg);

		if (ICTG > 0) {
			ICTG--;
			if (ACtg.ctg[ICTG].GetCountOfPidCategories() > 0) {

				SelectPidCategory:
				IPIDCTG = SelectOnePidCategory(&ACtg.ctg[ICTG]);
				if (IPIDCTG > 0) {
					IPIDCTG--;

				InputName:
					cl();
					printHeader("Введення імені для нового продукту");
					pos = BoxGet("Введiть iм'я нового продукту ");
					do
					{
					BackInputName:
						name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

					} while (name.size() < 1);

					PerhodnikInputToMenu();
					menu = { { "Назад" },{ "Назад до вибору під категорії" },{ "Далі" },{ "Вихiд" } };
					IDMenu = CoutMenu(&menu, 2);

					if (IDMenu == 0) goto BackInputName;
					else if (IDMenu == 1) goto SelectPidCategory;
					else if (IDMenu == 3) return "Продукт не додано";

				InputPrice:
					cl();
					printHeader("Введення ціни для нового продукту");
					pos = BoxGet("Введiть ціну нового продукту ");
					do
					{
					BackInputPrice:
						PRICE = strtof(GetLine(pos, to_string(PRICE), consoleSize.x - GetPosCur().x - 2).c_str(), NULL);

					} while (PRICE < 1);

					PerhodnikInputToMenu();
					menu = { { "Назад" },{"Назад до введення імені"},{ "Далі" },{ "Вихiд" } };
					IDMenu = CoutMenu(&menu, 2);

					if (IDMenu == 0) goto BackInputPrice;
					else if (IDMenu == 1) goto InputName;
					else if (IDMenu == 3) return "Продукт не додано";

				InputCount:
					cl();
					printHeader("Введення кількісті нового продукту");
					pos = BoxGet("Введiть кількість нового продукту ");
					do
					{
					BackInputCount:
						COUNT = strtof(GetLine(pos, to_string(COUNT), consoleSize.x - GetPosCur().x - 2).c_str(), NULL);

					} while (COUNT < 0);

					PerhodnikInputToMenu();
					menu = { { "Назад" },{ "Назад до введення ціни продукту" },{ "Далі" },{ "Вихiд" } };
					IDMenu = CoutMenu(&menu, 2);

					if (IDMenu == 0) goto BackInputCount;
					else if (IDMenu == 1) goto InputPrice;
					else if (IDMenu == 3) return "Продукт не додано";

				InputDate:
					cl();
					printHeader("Введення дати пригодності нового продукту");
					pos = BoxGet("Введiть термін споживання нового продукту ");
					test = new Product;
					do
					{
					BackInputDate:
						GetDate(pos, &term);
					} while (!test->IsOkDate(term));
					PerhodnikInputToMenu();
					menu = { { "Назад" },{ "Назад до введення кількості продукту" },{ "Готово" },{ "Вихiд" } };
					IDMenu = CoutMenu(&menu, 2);

					if (IDMenu == 0) goto BackInputDate;
					else if (IDMenu == 1) goto InputCount;
					else if (IDMenu == 3) {
						delete test;
						return "Продукт не додано";
					}

					if (shop.products.AddProduct(name, term.GetDay(), term.GetMonth(), term.GetYear(), PRICE, COUNT, ICTG, IPIDCTG))
						return "Продукт успішно додано";
					else return "Збій продукт не додано";
				}
				else goto SelectCategory;
			}
			else {
				msg = "Не має підкатегорій";
				goto SelectCategory;
			}
		}
		else return "Продукт не додано";
	}
	else return "Не має категорій";
}

int SelectOneProduct(Shop &shop, string *msg = new string) {
	cl();
	vector<string> menu;
	menu.push_back("Вийти з цього меню");
	for (int i = 0; i < shop.products.products.size(); i++) menu.push_back(shop.products.products[i].GetName());
	printHeader("Вибiр продукту");
	PrintMessage(*msg);
	return CoutMenu(&menu);
}

string SetCountProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {
		
		int MM, COUNT, INM;
		string msg;
		vector<string> menu;
		POINT pos;

		SelectProduct:
		MM = SelectOneProduct(shop, &msg);

		if (MM > 0) {
			MM--;

		InputCount:
			COUNT = shop.products.products[MM].GetCount();
			cl();
			printHeader("Введення нової кількісті продукту");
			pos = BoxGet("Введiть нову кількість продукту " + shop.products.products[MM].GetName() + " ");
			do
			{
			BackInputCount:
				COUNT = strtof(GetLine(pos, to_string(COUNT), consoleSize.x - GetPosCur().x - 2).c_str(), NULL);

			} while (COUNT < 0);

			PerhodnikInputToMenu();
			menu = { { "Назад" },{ "Назад до вибору продукту" },{ "Готово" },{ "Вихiд" } };
			INM = CoutMenu(&menu, 2);

			if (INM == 0) goto BackInputCount;
			else if (INM == 1) goto SelectProduct;
			else if (INM == 3) return "Кількість продукту не змінилась";

			msg = "Змінилась кількіст продукту з " + to_string(shop.products.products[MM].GetCount()) + " на " + to_string(COUNT);
			shop.products.products[MM].SetCount(COUNT);
			
			return msg;
		}
		else return "Кількість продукту не змінилась";
	}
	else return "Не має продуктів";
}

string SetPriceProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {

		int MM, INM;
		float PRICE;
		string msg;
		vector<string> menu;
		POINT pos;

	SelectProduct:
		MM = SelectOneProduct(shop, &msg);

		if (MM > 0) {
			MM--;

		InputPrice:
			cl();
			PRICE = shop.products.products[MM].GetPrice();
			printHeader("Введення нової ціни продукту");
			pos = BoxGet("Введiть нову ціну продукту " + shop.products.products[MM].GetName() + " ");
			do
			{
			BackInputPrice:
				PRICE = strtof(GetLine(pos, to_string(PRICE), consoleSize.x - GetPosCur().x - 2).c_str(), NULL);

			} while (PRICE < 1);

			PerhodnikInputToMenu();
			menu = { { "Назад" },{ "Назад до вибору продукту" },{ "Готово" },{ "Вихiд" } };
			INM = CoutMenu(&menu, 2);

			if (INM == 0) goto BackInputPrice;
			else if (INM == 1) goto SelectProduct;
			else if (INM == 3) return "Ціна продукту не змінилась";

			msg = "Змінилась ціна продукту з " + to_string(shop.products.products[MM].GetPrice()) + " на " + to_string(PRICE);
			shop.products.products[MM].SetCount(PRICE);

			return msg;
		}
		else return "Ціна продукту не змінилась";
	}
	else return "Не має продуктів";
}

string SetTermin(Shop &shop) {
	if (shop.products.products.size() > 0) {

		int MM, INM;
		string msg;
		vector<string> menu;
		POINT pos;
		Product *test;
		Date term;

	SelectProduct:
		MM = SelectOneProduct(shop, &msg);

		if (MM > 0) {
			MM--;

			term = shop.products.products[MM].GetTermOfConsumption();
			InputDate:
			cl();
			printHeader("Введення нового терміну споживання");
			pos = BoxGet("Введiть новий термін споживання продукту " + shop.products.products[MM].GetName() + " ");
			test = new Product;
			do
			{
			BackInputDate:
				GetDate(pos, &term);
			} while (!test->IsOkDate(term));
			PerhodnikInputToMenu();
			menu = { { "Назад" },{ "Назад до вибору продукту" },{ "Готово" },{ "Вихiд" } };
			INM = CoutMenu(&menu);

			if (INM == 0) goto BackInputDate;
			else if (INM == 1) goto SelectProduct;
			else if (INM == 3) {
				delete test;
				return "Термін споживання продукту не змінився";
			}

			msg = "Змінився термін споживання продукту з " + shop.products.products[MM].GetTermOfConsumption().GetAllStr() + " на " + term.GetAllStr();
			shop.products.products[MM].SetTermOfConsumption(term.GetDay(), term.GetMonth(), term.GetYear());
			return msg;
		}
		else return "Термін споживання продукту не змінився";
	}
	else return "Не має продуктів";
}

string SetCategoryOfProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {
		if (ACtg.ctg.size() > 1) {
			int MM, ICTG, IPIDCTG;
			string msg;

		SelectProduct:
			MM = SelectOneProduct(shop, &msg);

			if (MM > 0) {
				MM--;
				SelectCategory:
				ICTG = SelectOneCategory(&msg);

				if (ICTG > 0) {
					ICTG--;
					if (ACtg.ctg[ICTG].GetCountOfPidCategories() > 0) {
						IPIDCTG = SelectOnePidCategory(&ACtg.ctg[ICTG], &msg);
						if (IPIDCTG > 0) {
							IPIDCTG--;
							shop.products.products[MM].SetCategory(ICTG, IPIDCTG);
							return "Категорія продукту змінилась";
						}
						else goto SelectCategory;
					}
					else {
						msg = "Не має підкатегорій";
						goto SelectCategory;
					}
				}
				else goto SelectProduct;
			}
			else return "Категорія продукту не змінилась";
		}
		else return "Не має іншої категорії";
	}
	else return "Не має продуктів";
}

string SetPidCategoryOfProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {
		
			int MM, IPIDCTG;
			string msg;
			vector<string> menu;

			SelectProduct:
			MM = SelectOneProduct(shop, &msg);

			if (MM > 0) {
				MM--;
				if (ACtg.ctg[shop.products.products[MM].GetCategory()].GetCountOfPidCategories() > 1) {
					IPIDCTG = SelectOnePidCategory(&ACtg.ctg[shop.products.products[MM].GetCategory()], &msg);
					if (IPIDCTG > 0) {
						IPIDCTG--;
						shop.products.products[MM].SetCategory(shop.products.products[MM].GetCategory(), IPIDCTG);
						return "Категорія продукту змінилась";
					}
					else goto SelectProduct;
				}
				else {
					msg = "Не має інших підкатегорій";
					goto SelectProduct;
				}
			}
			else return "Категорія продукту не змінилась";
	}
	else return "Не має продуктів";
}

string SetNameOfProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {

		int MM, INM;
		string msg, name;
		vector<string> menu;
		POINT pos;

	SelectProduct:
		MM = SelectOneProduct(shop, &msg);

		if (MM > 0) {
			MM--;
			name = shop.products.products[MM].GetName();
		InputName:
			cl();
			printHeader("Введення нового імені для продукту");
			pos = BoxGet("Введiть нове iм'я продукту ");
			do
			{
			BackInputName:
				name = GetLine(pos, name, consoleSize.x - GetPosCur().x - 2);

			} while (name.size() < 1);

			PerhodnikInputToMenu();
			menu = { { "Назад" },{ "Назад до вибору продукту" },{ "Готово" },{ "Вихiд" } };
			INM = CoutMenu(&menu);

			if (INM == 0) goto BackInputName;
			else if (INM == 1) goto SelectProduct;
			else if (INM == 3) return "Ім'я продукту не змінилось";

			msg = "Змінилось ім'я продукту з " + shop.products.products[MM].GetName() + " на " + name;
			shop.products.products[MM].SetName(name);

			return msg;
		}
		else return "Ім'я продукту не змінилось";
	}
	else return "Не має продуктів";
}

int * SelectProducts(vector<Product> *prd, int *masstoseind = nullptr, const int &def = 0, POINT pos = GetPosCur()) {
	if (def < prd->size() && def > -1) {
		if (masstoseind == nullptr) masstoseind = new int[prd->size()];
		pos.x = 0; // О
		int ch = 0, start = 0, finish = (consoleSize.y - pos.y) / 2, item = def, cout_item = def, maxCount = 0; // нажата кнопка, iндекс з якого виводимо , iндекс на якому закiнчуємо виводити, позицыя курсора вибору
		bool reload = true, exit = false, reload_item = true; // Флажок перерисовки, флажок виходу
		; // Массив вибраних 
		for (int i = 0; i < prd->size(); i++) {
			masstoseind[i] = 0;
			if (maxCount < CountDigits((*prd)[i].GetCount())) maxCount = CountDigits((*prd)[i].GetCount());
		}
		UINT mm = GetConsoleCP();
		string Options = "Кнопка B = Вийти, O = Готово", temp; // Зберiгаєм начальну locale
		gotoxy(pos.x, pos.y); // Перемiщуємося в початкову точку
		for (int i = 0; i < prd->size() && i < finish; i++) // Виводимо сiтку(Заготовку де будуть розмiщуватися самi ыекштп ektvtynb)
		{
			cout << char(186);
			gotoxy(consoleSize.x - 3 - maxCount, GetPosCur().y);
			cout << char(186);
			gotoxy(consoleSize.x - 2, GetPosCur().y);
			cout << char(186);
			cout << endl;
			cout << char(199);
			for (int i = 0; i < consoleSize.x - 2; i++) cout << char(196);
			gotoxy(consoleSize.x - 3, GetPosCur().y);
			cout << char(196) << char(182) << endl;
		}
		cout << char(186);
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		cout << Options;
		SetConsoleCP(mm);
		SetConsoleOutputCP(mm);
		gotoxy(consoleSize.x - 2, GetPosCur().y);
		cout << char(186);
		cout << endl;
		cout << char(200);
		for (int i = 0; i < consoleSize.x - 2; i++) cout << char(205);
		gotoxy(consoleSize.x - 3, GetPosCur().y);
		cout << char(205) << char(188);

		while (!exit)
		{
			if (reload) {

				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);

				for (int i = start; i < prd->size() && i < finish; i++)
				{
					gotoxy(1, pos.y + ((i - start) * 2));
					temp = (*prd)[i].GetName();
					if (temp.size() > consoleSize.x - 7 - maxCount) {
						temp.erase(consoleSize.x - 7 - maxCount);
						temp += "...";
					}
					if (masstoseind[i] > 0) {
						SetColorConsole(0, 10);
					}
					cout << temp;
					for (int s = 0; s < consoleSize.x - 4 - temp.size() - maxCount; s++) cout << " ";

					gotoxy(consoleSize.x - 2 - maxCount, GetPosCur().y);
					cout << masstoseind[i];
					for (int s = 0; s < maxCount - CountDigits(masstoseind[i]); s++) cout << " ";
					SetColorConsole(7, 0);
				}

				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);

			}

			if (item != cout_item || reload == true || reload_item == true) {
				if (cout_item >= start && cout_item <= finish && item != cout_item) {
					gotoxy(1, pos.y + ((cout_item - start) * 2));
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);
					temp = (*prd)[cout_item].GetName();
					if (temp.size() > consoleSize.x - 7 - maxCount) {
						temp.erase(consoleSize.x - 7 - maxCount);
						temp += "...";
					}
					if (masstoseind[cout_item] > 0) {
						SetColorConsole(0, 10);
					}
					cout << temp;
					for (int s = 0; s < consoleSize.x - 4 - temp.size() - maxCount; s++) cout << " ";
					gotoxy(consoleSize.x - 2 - maxCount, GetPosCur().y);
					cout << masstoseind[cout_item];
					for (int s = 0; s < maxCount - CountDigits(masstoseind[cout_item]); s++) cout << " ";
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
					SetColorConsole(7, 0);
				}
				gotoxy(1, pos.y + ((item - start) * 2));
				temp = (*prd)[item].GetName();
				if (temp.size() > consoleSize.x - 7 - maxCount) {
					temp.erase(consoleSize.x - 7 - maxCount);
					temp += "...";
				}
				if (masstoseind[item] > 0) {
					SetColorConsole(15, 9);
				}
				else SetColorConsole(0, 14);
				cout << temp;
				for (int s = 0; s < consoleSize.x - 4 - temp.size() - maxCount; s++) cout << " ";
				gotoxy(consoleSize.x - 2 - maxCount, GetPosCur().y);
				cout << masstoseind[item];
				for (int s = 0; s < maxCount - CountDigits(masstoseind[item]); s++) cout << " ";
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				SetColorConsole(7, 0);
				cout_item = item;
				reload = false;
			}
			
			ch = _getch();
			switch (ch)
			{
			case 80:
				if (item < prd->size() - 1) item++;
				else ch = 0;
				if (item + 1 > finish && finish < prd->size() && ch != 0) {
					finish++;
					start++;
					reload = true;
				}
				break;
			case 72:
				if (item > 0) item--;
				else ch = 0;
				if (item < start && start > 0 && ch != 0) {
					finish--;
					start--;
					reload = true;
				}
				break;
			case 77:
				if ((*prd)[item].GetCount() > masstoseind[item]) masstoseind[item]++;
				reload_item = true;

				break;
			case 75:
				if (0 < masstoseind[item]) masstoseind[item]--;
				reload_item = true;
				break;
			case 98:
			case 168:
			{
				if (prd->size() >= finish)
					gotoxy(1, pos.y + (finish - start) * 2);
				else gotoxy(1, pos.y + (prd->size() - start) * 2);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << "Точно ви хочете вийти з цього меню (yes ? no): ";
				clearLine(2);
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				POINT opos = GetPosCur();
				string YN;
				do
				{
					YN = GetLine(opos, YN, consoleSize.x - GetPosCur().x - 2);
				} while (YN != "Yes" && YN != "No" && YN != "Y" && YN != "N" && YN != "y" && YN != "n" && YN != "yes" && YN != "no");
				if (YN == "Yes" || YN == "Y" || YN == "y" || YN == "yes") return nullptr;
				else {
					if (prd->size() >= finish)
						gotoxy(1, pos.y + (finish - start) * 2);
					else gotoxy(1, pos.y + (prd->size() - start) * 2);
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);
					cout << Options;
					clearLine(2);
					cout << endl;
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
				}

			}

			break;
			case 233:
			case 111:
			{
				if (prd->size() >= finish)
					gotoxy(1, pos.y + (finish - start) * 2);
				else gotoxy(1, pos.y + (prd->size() - start) * 2);
				SetConsoleCP(1251);
				SetConsoleOutputCP(1251);
				cout << "Точно готово (yes ? no): ";
				clearLine(2);
				SetConsoleCP(mm);
				SetConsoleOutputCP(mm);
				POINT opos = GetPosCur();
				string YN;
				do
				{
					YN = GetLine(opos, YN, consoleSize.x - GetPosCur().x - 2);
				} while (YN != "Yes" && YN != "No" && YN != "Y" && YN != "N" && YN != "y" && YN != "n" && YN != "yes" && YN != "no");
				if (YN == "Yes" || YN == "Y" || YN == "y" || YN == "yes") exit = true;
				else {
					if (prd->size() >= finish)
						gotoxy(1, pos.y + (finish - start) * 2);
					else gotoxy(1, pos.y + (prd->size() - start) * 2);
					SetConsoleCP(1251);
					SetConsoleOutputCP(1251);
					cout << Options;
					clearLine(2);
					cout << endl;
					SetConsoleCP(mm);
					SetConsoleOutputCP(mm);
				}

			}

			break;
			case 13:
				if (!masstoseind[item]) {
					masstoseind[item] = true;
				}
				else masstoseind[item] = false;
				reload_item = true;
				break;
			}
		}
		return masstoseind;
	}return nullptr;
}

string BuyProduct(Shop &shop) {
	if (shop.products.products.size() > 0) {

		vector<string> menu;
		string msg;
		int IMENU, *massSelect = nullptr;
		float sum = 0;

	GO:

		cl();
		for (int i = 0; i < shop.products.products.size(); i++)
			if (shop.products.products[i].GetCount() > 0) break;
			else if (i == shop.products.products.size() - 1) return "Продукти закінчились";
		
		printHeader("Купівля продуктів");
		PrintMessage(msg);
		menu = { "Купити продукти", "Вийти" };
		IMENU = CoutMenu(&menu, 0);

		

		if (IMENU == 0) {
			
			cl();

			printHeader("Вибір продуктів для покупки");

			massSelect = SelectProducts(&shop.products.products, massSelect);

			cl();
			sum = 0;
			for (int i = 0; i < shop.products.products.size(); i++)
			{
				sum += shop.products.products[i].GetPrice() * massSelect[i];
			}
			printHeader("Підтвердження покупки");
			PrintMessage(msg);
			menu = { "Готово " + to_string(sum) + " грн", "Назад " };
			IMENU = CoutMenu(&menu, 0);
			if (IMENU == 0)
			cl();


		}else return "";
	}
	else return "Не має продуктів";
}

int main() {

	/*ShowItems(new vector<string>{
		{ "1" },
		{ "2" },
		{ "3" },
		{ "4" },
		{ "5" },
		{ "6" },
		{ "7" },
		{ "8" },
		{ "9" },
		{ "10" },
		{ "11" },
		{ "12" },
		{ "13" },
		{ "14" },
		{ "15" },
		{ "16" },
		{ "17" },
		{ "18" },
		{ "19" },
		{ "21" },
		{ "22" },
		{ "23" },
		{ "24" },
		{ "25" },
		{ "26" },
		{ "27" },
		{ "28" },
		{ "29" },
		{ "30fdsfsddddddddddddddddddddddddddddddddddddfjudfjusifsjddddddddddddddddddddsodifsdoifsdufsiodfisodfiosdiofsdiofio" }
		});

	return 0;*/

	
	
	SetVisibleCursor(false);
	
	/*GetDate();

	return 0;
*/
	Shop shop;
	LoadOfFile(FILE_TO_SAVE, shop);
	InitLLC(shop);
	// Init(shop);
	//Trader * user = LogIn(shop);
	shop.LogIn("1@1.1", "1111");
	bool exit = false;
	int Imenu;
	vector<string> menu;
	vector<int> Inmenu;
	string message;
	Permission *perm = new Permission;
	perm->permissions = typesOfTraders.TOFT[shop.user->GetType()]->permissions;
	perm->SetCountPidPermissions(typesOfTraders.TOFT[shop.user->GetType()]->GetCountOfPermissions());
	perm->SetNameMenu("Головне Меню");
	for (int i = 0; i < perm->GetCountOfPidPermissions(); i++) perm->permissions[i].nadPermission = perm;
	while (!exit)
	{
		cl();
		SetVisibleCursor(false);
		consoleSize = {GetBufferChars(), GetBufferCharsbot()};
		gotoxy(0, 0);
		if (menu.size() == 0) {
			int maxSize = 0; // Максимальний довжина меню
			

			if (perm->nadPermission != nullptr) {                                                    
				menu.push_back("<< Назад в прошлое");
				Inmenu.push_back(-2);
			}
		
				
			
			for (int i = 0; i < perm->GetCountOfPidPermissions(); i++)
				if (perm->permissions[i].GetPermission()) {
					menu.push_back(perm->permissions[i].GetInMenu());
					Inmenu.push_back(i);
				}

			menu.push_back("Вийти з бази"); 
			Inmenu.push_back(-1);

			for (int i = 0; i < menu.size(); i++) {
				if (maxSize < menu[i].length()) maxSize = menu[i].length();
			}

			printHeader(perm->GetNameMenu()); // Виводимо header

			for (int i = 0; i < menu.size(); i++)
			{
				int m = maxSize - menu[i].length();
				for (int r = 0; r < m; r++) {
					menu[i] += " ";
				}
				if (Inmenu[i] > -1) {
					if (perm->permissions[Inmenu[i]].permissions != nullptr) menu[i] += " + ";
					else menu[i] += "   ";
				}
				else menu[i] += "   ";
			}

		}
		PrintMessage(message);
		Imenu = CoutMenu(&menu, 1);
		if (Inmenu[Imenu] > -1) {
			if (perm->permissions[Inmenu[Imenu]].permissions != nullptr) perm = &perm->permissions[Inmenu[Imenu]];
			else if (perm->permissions[Inmenu[Imenu]].func != NULL) {
				message = perm->permissions[Inmenu[Imenu]].func(shop);
			}
			menu.clear();
			Inmenu.clear();
		}
		else {
			switch (Inmenu[Imenu])
			{
				case -2: 
					perm = perm->nadPermission;
					menu.clear();
					Inmenu.clear();
				break;
				case -1:
					exit = true;
				break;
			}
		}
	}

	SaveAll(FILE_TO_SAVE, shop);

	return 0;
}