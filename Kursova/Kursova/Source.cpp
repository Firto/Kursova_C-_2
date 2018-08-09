#include <windows.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>
#include <initializer_list>
#include <Wincrypt.h>
#include <regex>
using namespace std;



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
		return SetAll(date.day, date.year, date.month);
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

	bool isOkPidCategory(const int &category) const { // Перевiрка пiд категорiї
		if (category < namesCategories.size() && category > -1) return true;
		else return false;
	}
};

class AllCategory /*Всi категорiї*/ {	
public:
	vector<Category *> ctg;

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

	bool SetTermOfConsumption(const int &day, const int &month, const int &year) {
		if (IsOkDate(day, month, year)) {
			this->termOfConsumption.SetAll(day, month, year);
			return true;
		}
		else return false;
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
		
			if (ACtg.ctg[category]->isOkPidCategory(pidCategory)) {
				this->pidCategory = pidCategory;
				return true;
			}
			else return false;
	}

	bool SetCategory(const int & category, const int &pidCategory) {
		if (ACtg.isOkCategory(category)) {
			if (ACtg.ctg[category]->isOkPidCategory(pidCategory)) {
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
			if (ACtg.ctg[category]->isOkPidCategory(pidCategory)) {
				this->name = name;
				this->termOfConsumption.SetAll(day, month, year);
				this->price = price;
				this->count = count;
				this->category = category;
				this->pidCategory = pidCategory;
			}
			else return false;
		}
		else return false;
	}
};

class AllProducts {
	vector<Product *> products;
	//friend void Init(Shop &shop);
public:

	bool AddProduct(const string &name, const int &day, const int &month, const int &year, const float &price, const int &count, const int &category, const int & pidCategory) {
		Product *testProduct = new Product;
		if (testProduct->SetAllComponents(name, day, month, year, price, count, category, pidCategory)) {
			products.push_back(testProduct);
			return true;
		}
		else return false;
	}

	bool ISOkIdProduct(const int &id) {
		if (id > -1 && id < products.size()) return true;
		else return false;
	}

	bool RemoveProduct(const int &id) {
		if (ISOkIdProduct(id)) {
			delete products[id];
			products.erase(products.begin() + id);
			return true;
		}
		else return false;
	}
};

class Permission {
	string name, InMenu, message, nameMenu;
	bool permission = false;
	int countOfPidPerm = 0;
public:
	Permission * permissions = nullptr;
	Permission * nadPermission = nullptr;
	Permission() {};
	Permission(const string &name, const string &InMenu) {
		SetName(name);
		SetInMenu(InMenu);
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

class TypeOfTrader /*Тип працiвника*/ {

	string name; // iмя типу працiвника

	static const int countOfPermsions = 2; // Кiлькiсть дозволiв

	float salary; // Зарплата

	

public:

	Permission permissions[countOfPermsions] = { /*Дозволи*/ 
		{string("дозвiл на редагування продуктiв"), string("Ви в меню редагування продуктiв!"), string("Меню редагування продуктiв"),string("Редагувати продукти"), 9 ,
			new Permission[9]{
				{ string("дозвiл на додавння продуктiв"), string("Додати продукт") },
				{ string("дозвiл на редактування кiлькостi продукту"), string("Редагувати кiлькiсть продукту") },
				{ string("дозвiл на редагування цiни продукту"), string("Редагувати цiну продукту") },
				{ string("дозвiл на редагування термiну споживання продукту"), string("Ви в меню редагування термiну споживання продукту!"), string("Меню редагування термiну споживання продукту"),string("Редагувати термiн споживання продукту"), 3, new Permission[3]{
					{ string("дозвiл на редагування дня термiну споживання продукту"), string("Редагувати день термiну споживання продукту") },
					{ string("дозвiл на редагування мiсяця термiну споживання продукту"), string("Редагувати мiсяць термiну споживання продукту") },
					{ string("дозвiл на редагування року термiну споживання продукту") , string("Редагувати рiк термiну споживання продукту") },
				}},
				{ string("дозвiл на редагування категорiї продукту"), string("Редагувати категорiю продукту") },
				{ string("дозвiл на редагування пiд категорiї продукту"), string("Редагувати пiд категорiї продукту") },
				{ string("дозвiл на редагування iменi продукту") , string("Редагувати iм'я продукту") },
				{ string("дозвiл на покупку продукту") , string("Пробити продукт на кассi") },
				{ string("дозвiл на редагування категорiй продуктiв"), string("Ви в меню редагування категорiй продуктiв"), string("Меню редагування категорiй продуктiв"), string("Редагувати категорiї продуктiв"), 4, new Permission[4]{
					{ string("дозвiл на додавання нової категорiї продуктiв") , string("Додати нову категорiю продуктiв") },
					{ string("дозвiл на переiменування категорiї продуктiв") , string("Перейменувати категорiю продуктiв") },
					{ string("дозвiл на видалення категорiї продуктiв"), string("Видалити категорiю продуктiв") },
					{ string("дозвiл на редагування пiд категорiї продуктiв"), string("Ви в меню редагування пiд категорiй продуктiв"), string("Меню редагування пiд кактегорiй продуктiв"), string("Редагувати пiд категорiї продуктiв") , 3, new Permission[4]{
						{ string("дозвiл на додавання пiд категорiяй продуктiв"), string("Додати пiд категорiю продуктiв") },
						{ string("дозвiл на видалення пiд категорiяй продуктiв"), string("Видалити пiд категорiю продуктiв") },
						{ string("дозвiл на перейменування пiд категорiяй продуктiв"), string("Перейменувати пiд категорiю продуктiв") }
					}},
				}}
			}
		},
		{string("дозвiл на редагування працiвникiв"), string("Редагувати працiвникiв"), 7, new Permission[7]{
			{ string("дозвiл на редагування ПiБ працiвника"), 3, new Permission[3]{
				{ string("дозвiл на редагування iменi працiвника") },
				{ string("дозвiл на редагування прiзвища працiвника") },
				{ string("дозвiл на редагування по батьковi працiвника") }
			}},
			{ string("дозвiл на редагування пароля працiвника") },
			{ string("дозвiл на редагування емейла працiвника") },
			{ string("дозвiл на редагування дати народження працiвника") , 3, new Permission[3]{
				{ string("дозвiл на редагування дня народження") },
				{ string("дозвiл на редагування мiсяця народження") },
				{ string("дозвiл на редагування року народження") }
			}},
			{ string("дозвiл на додавання новох працiвникiв") },
			{ string("дозвiл на звiльнення працiвникiв") },
			{ string("дозвiл на редагування типiв працiвникiв") , 3, new Permission[3]{
				{ string("дозвiл на редагування iменi типу працiвника") },
				{ string("дозвiл на редагування зарплати типу працiвника") },
				{ string("дозвiл на редагування дозволiв"), 2, new Permission[2]{
						{string("дозвiл на дозвiл на редагування продуктiв"), 9 , new Permission[9]{
							{ string("дозвiл на дозвiл на додавння продуктiв") },
							{ string("дозвiл на дозвiл на редактування кiлькостi продукту") },
							{ string("дозвiл на дозвiл на редагування цiни продукту") },
							{ string("дозвiл на дозвiл на редагування термiну споживання продукту"), 3, new Permission[3]{
								{ string("дозвiл на дозвiл на редагування дня термiну споживання продукту") },
								{ string("дозвiл на дозвiл на редагування мiсяця термiну споживання продукту") },
								{ string("дозвiл на дозвiл на редагування року термiну споживання продукту") },
							}},
							{ string("дозвiл на дозвiл на редагування категорiї продукту") },
							{ string("дозвiл на дозвiл на редагування пiд категорiї продукту") },
							{ string("дозвiл на дозвiл на редагування iменi продукту") },
							{ string("дозвiл на дозвiл на покупку продукту") },
							{ string("дозвiл на дозвiл на редагування категорiй продуктiв"), 4, new Permission[4]{
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
					{string("дозвiл на дозвiл на редагування працiвникiв"), 7, new Permission[7]{
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
	friend void Init(Shop &shop);
	friend void InitLLC(Shop &shop);
public:

	Trader * LogIn(const string &email, const string &pass) {
		return traders.LogIn(email, pass);
	}
};



// Work With text

HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbInfo;

void cl() {
	system("cls");
}

void gotoxy(int x, int y) {
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}

int GetBufferChars() {
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Right - csbInfo.srWindow.Left;

}

int GetBufferCharsbot() {
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Bottom - csbInfo.srWindow.Top;
}

void SetVisibleCursor(const bool &rst) {
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hCon, &structCursorInfo);
	structCursorInfo.bVisible = rst;
	SetConsoleCursorInfo(hCon, &structCursorInfo);


}

POINT consoleSize = { GetBufferChars() , GetBufferCharsbot() };

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
	int _chars = GetBufferChars();
	for (int i = 0; i < _chars; i++)
	{
		cout << a;
	}
	cout << endl;
	SetConsoleTextAttribute(hCon, (WORD)((0 << 4) | 7));
}

void printHeader(const string& str) {
	for (int i = 0; i < (GetBufferChars() - str.size()) / 2 - 1; i++) cout << '\0';
	cout << char(201);
	for (int i = 0; i < str.size(); i++) cout << char(205);
	cout << char(187);
	cout << endl;
	
	cout << char(201);
	for (int i = 0; i < (GetBufferChars() - str.size()) / 2 - 2; i++) cout << char(205);
	string rr = setlocale(LC_ALL, NULL);
	
	cout << char(185);
	setlocale(LC_ALL, "Russian");
	cout << str;
	setlocale(LC_ALL, rr.c_str());
	cout << char(204);

	for (int i = 0; i < (GetBufferChars() - str.size()) / 2 - 1; i++) cout << char(205);
	cout << char(187);
	cout << endl;

	cout << char(204);
	for (int i = 0; i < (GetBufferChars() - str.size()) / 2 - 2; i++) cout << char(205);
	cout << char(202);
	for (int i = 0; i < str.size(); i++) cout << char(205);
	cout << char(202);
	for (int i = 0; i < (GetBufferChars() - str.size()) / 2 - 1; i++) cout << char(205);
	cout << char(185);
	cout << endl;
}

void printLine(const string & str, int size = GetBufferChars(),const int textColor = 7, const int background = 0) {
	SetColorConsole(textColor, background); // Зміна кольору консолі 15 колір тла, 0 колір тексту
	for (int i = 0; i < (size - str.size()) / 2; i++) cout << " "; // Виведення відступу
	string rr = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "Russian");
	cout << str;
	setlocale(LC_ALL, rr.c_str());
	for (int i = 0; i < (size - str.size()) / 2 + 1; i++) cout << " "; // виведення другого відступу
	SetColorConsole(); // Певернення консолі до початкового стану
	cout << endl;
}

//int ShowMenu(vector<string *> *menu) {
//
//	const int speedOpen = 100;
//
//	static bool
//		exit = false, // Флажок для виходу з програми
//		opened = false, // Флажок положення меню
//		opened_show = false, // Флажок положення меню зараз
//		reload = true; // Флажок для перерисовки
//	static int 
//		selected = 0, // Вибраний пункт меню 
//		selected_show = 0; // Пункт меню який показаний на екранi
//	char act; // Нажата кнопка
//	if (reload) { // Перевiряєм чи флажо перерисовки true
//		if (opened) { // Перевiряєм чи флажок положення меню true
//			if (!opened_show) { // Перевiряєм чи вiдкрите меню виведене на екран
//	
//				for (int i = 0; i < menu->size(); i++) // По кадрово виводимо вiдкрите меню меню
//				{
//					gotoxy(consoleSize.x, consoleSize.y - i); // Перемiщаємося в кiнець останнього рядка - i
//					clearToPos(0, consoleSize.y - i); // Очищаємо останнiй рядок - i
//					printLine(consoleSize.y - i - 1, "\1", 0, 15); // Виводимо лiнiю зi стрiлкою
//
//					for (int s = 0; s <= i; s++) // Потроху виводимо елементи массиву menu
//					{
//						if ((i - s) != selected) printLine(consoleSize.y - s, *(*menu)[s]);
//						else printLine(consoleSize.y - s, ">>> " + *(*menu)[s] + " <<<");
//
//					}
//					gotoxy(0, 0); // Перемiщаємось на самий початок
//					Sleep(speedOpen / menu->size()); // Пауза
//				}
//			}
//			else if (selected != selected_show){ // Перевiряєм чи вибраний елемент массиву menu виведений на еран 
//				printLine(consoleSize.y - (menu->size() - 1 - selected_show), *(*menu)[selected_show]); // Переписуєму строку там де був вибраний елемент
//				printLine(consoleSize.y - (menu->size() - 1 - selected), ">>> " + *(*menu)[selected] + " <<<"); // Переписуємо строку там де має бути вибраний елемент
//				selected_show = selected; // Вибранийбункт є на еранi
//				gotoxy(0, 0); // Перемiщаємося на самий початок
//			}
//			opened_show = opened; // Вiдкрите меню виведене на екран
//		}
//		else { // Перевiряєм чи флажок положення меню false
//			if (!opened_show) { // Перевiряєм чи флажок виведеного меню false
//				printLine(consoleSize.y, "\1", 0, 15); // Виводимо лiнiю зi стрiлкою
//				gotoxy(0, 0); // Перемiщаємось на самий початок
//			}
//			else { // Якщо флажок виведеного меню true
//				for (int i = menu->size() -1; i >= 0; i--)
//				{
//					gotoxy(consoleSize.x, consoleSize.y - i-1); // Перемiщаємося в кiнець останнього рядка - i
//					clearToPos(0, consoleSize.y - i - 1); // Очищаємо останнiй рядок - i
//					printLine(consoleSize.y - i, "\1", 0, 15); // Виводимо лiнiю зi стрiлкою
//					gotoxy(0, 0); // Перемiщаємось на самий початок
//					Sleep(speedOpen / menu->size()); // Пауза
//
//				}
//				
//			}
//			opened_show = opened;// Закрите меню виведене на екран
//		}
//		reload = false; // Флажок перерисовки false
//		
//	}
//	act = _getch(); // Нажата кнопка
//	switch (act)
//	{
//	case 13:
//		return selected;
//		break;
//	case 80:
//		
//		
//		if (opened) {
//			if (selected < menu->size() - 1) selected++;
//			else selected = 0;
//			reload = true;
//		}
//		break;
//	case 72:
//		if (opened) {
//			if (selected > 0) selected--;
//			else selected = menu->size() - 1;
//			reload = true;
//		}
//		break;
//	case 27:
//		if (!opened) opened = true;
//		else opened = false;
//		reload = true;
//	}
//	return -1;
//}

int CoutMenu(vector<string *> *menus) {
	char ch = 0;
	int selected = 1, cout_selected = 0;
	POINT startCout = GetPosCur();
	
	for (int i = 0; i < menus->size(); i++) {
		cout << char(186);
		printLine(*(*menus)[i], GetBufferChars() - 2);
		gotoxy(GetBufferChars() - 1, startCout.y + (i * 2));
		cout << char(185);
		cout << endl;
		if (menus->size() - 1 != i) {
			cout << char(199);
			for (int i = 0; i < GetBufferChars() - 2; i++) cout << char(196);
			cout << char(182);
		}
		else {
			cout << char(200);
			for (int i = 0; i < GetBufferChars() - 2; i++) cout << char(205);
			cout << char(188);
		}
		cout << endl;
	}
	while (ch != 13)
	{
		if (selected != cout_selected) {
			gotoxy(1, startCout.y + (cout_selected*2));
			printLine(*(*menus)[cout_selected], GetBufferChars() - 2);
			gotoxy(1, startCout.y + (selected*2));
			printLine(*(*menus)[selected], GetBufferChars() - 2, 0, 15);
			cout_selected = selected;
			for (int i = 0; i < menus->size(); i++)
			{
				gotoxy(GetBufferChars() - 1, startCout.y + (i * 2));
				cout << char(186);
			}
		}
		ch = _getch();
		switch (ch)
		{
			case 80:
				if (selected < menus->size() - 1) selected++;
				else selected = 0;
			break;
			case 72: 
				if (selected > 0) selected--;
				else selected = menus->size() - 1;
			break;
		}
	}
	return selected;
}

//class Edit {
//	string in;
//	POINT posSt;
//public:
//	char(*sym)(const char&, const string&) = nullptr;
//	bool(*isOk)(const string&) = nullptr;
//	string GetTextClear(const string &osm, const char &source, const bool &pass = 0) {
//		FullLine(source);
//		cout << "-" << osm;
//		posSt = GetPosCur();
//		cout << endl;
//		FullLine(source);
//		gotoxy(posSt.x, posSt.y);
//		char symGet = 0;
//		while (true) {
//			symGet = _getch();
//			if (sym != nullptr) {
//				symGet = sym(symGet, in);
//			}
//			if (symGet > 0) {
//				if (symGet != 8 && symGet != 13 && GetPosCur().x < GetBufferChars()) {
//					in += symGet;
//					if (pass!) cout << symGet;
//				}
//				else if (symGet == 8 && GetPosCur().x > posSt.x) {
//					if (pass!) cout << symGet << '\0' << symGet;
//					in.pop_back();
//				}
//				else if (symGet == 13) {
//					if (isOk != nullptr) {
//						try
//						{
//							isOk(in);
//						}
//						catch (const char * r)
//						{
//							gotoxy(GetBufferChars(), posSt.y);
//							clearToPos(0, posSt.y);
//							cout << source << r << source << osm;
//							posSt = GetPosCur();
//						}
//					}
//					else break;
//				}
//			}
//		}
//		gotoxy(0, posSt.y + 2);
//		return in;
//	}
//};

void SetAll(Permission *pr) {
	for (int i = 0; i < pr->GetCountOfPidPermissions(); i++) SetAll(&(pr->permissions[i]));
	pr->SetPermission(true);
}

Trader * LogIn(Shop &shop) {
	Trader * ret = nullptr;
	POINT pos;
	string pass;
	string email;
	cout << ":: Log to base ::" << endl;
	do
	{
		{
			Trader rr;
			cout << "Input your email: ";
			pos = GetPosCur();
			do
			{
				clearToPos(pos);
				getline(cin, email);
			} while (!rr.IsOkEmail(email));
		}

		cout << "Input your password: ";
		pos = GetPosCur();
		do
		{
			clearToPos(pos);
			getline(cin, pass);
		} while (pass.size() < 1);
		ret = shop.LogIn(email, pass);
		cl();
		if (ret == nullptr) {
			cout << "Incorrect email or password!" << endl;
		}
	} while (ret == nullptr);

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
		POINT pos;
		string pass;
		string email;

		cout << ":: First log to base. Register ::" << endl;

		{
			Trader rr;
			cout << "Input your email\t: ";
			pos = GetPosCur();
			do
			{
				clearToPos(pos);
				getline(cin, email);
			} while (!rr.IsOkEmail(email));
		}

		cout << "Input your password\t: ";
		pos = GetPosCur();
		do
		{
			clearToPos(pos);
			getline(cin, pass);
		} while (pass.size() < 1);
		
		shop.traders.AddTrader("root", "root", "root", pass, email, 1, 1, 1900, 0);
	}
}

int main() {

	//for (int i = 0; i < 255; i++) cout << i << " " << char(i) << endl; system("pause");return 0;
	SetVisibleCursor(false);
	Shop shop;
	InitLLC(shop);
	cl();
	Trader * user = shop.LogIn("1@1.1", "1111");
	bool exit = false;
	int Imenu;
	vector<string *> menu;
	vector<int *> Inmenu;
	Permission *perm = new Permission;
	perm->permissions = typesOfTraders.TOFT[user->GetType()]->permissions;
	perm->SetCountPidPermissions(typesOfTraders.TOFT[user->GetType()]->GetCountOfPermissions());
	perm->SetNameMenu("Головне Меню");
	for (int i = 0; i < perm->GetCountOfPidPermissions(); i++) perm->permissions[i].nadPermission = perm;
	while (!exit)
	{
		cl();
		gotoxy(0, 0);
		if (menu.size() == 0) {
			int maxSize = 0; // Максимальний довжина меню

			menu.push_back(new string("Вийти з бази")); 
			Inmenu.push_back(new int(-1));
			

			if (perm->nadPermission != nullptr) {                                                    
				menu.push_back(new string("<< Назад в прошлое"));
				Inmenu.push_back(new int(-2));
			}
		
				
			
			for (int i = 0; i < perm->GetCountOfPidPermissions(); i++)
				if (perm->permissions[i].GetPermission()) {
					menu.push_back(new string(perm->permissions[i].GetInMenu()));
					Inmenu.push_back(new int(i));
					//for (int r = 0; r < maxSize - perm->permissions[i].GetInMenu().length(); r++) *menu[menu.size() - 1] += " ";
					//if (perm->permissions[i].permissions != nullptr) *menu[menu.size() - 1] += " +";
					//else *menu[menu.size() - 1] += "  ";
				}
			
			for (int i = 0; i < menu.size(); i++) {
				if (maxSize < menu[i]->length()) maxSize = menu[i]->length();
			}

			printHeader(perm->GetNameMenu()); // Виводимо header

			for (int i = 0; i < menu.size(); i++)
			{
				int m = maxSize - menu[i]->length();
				for (int r = 0; r < m; r++) {
					*menu[i] += " ";
				}
				if (*Inmenu[i] > -1) {
					if (perm->permissions[*Inmenu[i]].permissions != nullptr) *menu[i] += " + ";
					else *menu[i] += "   ";
				}
				else *menu[i] += "   ";
			}

		}
		Imenu = CoutMenu(&menu);
		if (*Inmenu[Imenu] > -1) {
			if (perm->permissions[*Inmenu[Imenu]].permissions != nullptr) perm = &perm->permissions[*Inmenu[Imenu]];
			for (int i = 0; i < menu.size(); i++) delete menu[i];
			menu.resize(0);
			for (int i = 0; i < Inmenu.size(); i++) delete Inmenu[i];
			Inmenu.resize(0);
		}
		else {
			switch (*Inmenu[Imenu])
			{
				case -2: 
					perm = perm->nadPermission;
					for (int i = 0; i < menu.size(); i++) delete menu[i];
					menu.resize(0);
					for (int i = 0; i < Inmenu.size(); i++) delete Inmenu[i];
					Inmenu.resize(0);
				break;
				case -1:
					exit = true;
				break;
			}
		}
	}


	return 0;
}