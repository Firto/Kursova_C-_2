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



int countSym(string str, char sym) { // ������� ��� ����������� ������� �������
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

class Category /*������*/ {
	string name; // ��� �������
	vector<string> namesCategories; // ����� �� ��������
public:

	Category(const string &name) {
		SetName(name);
	}

	bool SetName(const string &name) { // ��������� ��� �������
		if (name.length() > 0) {
			this->name = name;
			return true;
		}
		else return false;
	}

	int GetCountOfPidCategories() const { // ������������ ������ �� ���������
		return namesCategories.size();
	}

	const string GetName() const { // ����������� ��� �������
		return name;
	}

	const string GetNamePidCategory(const int &i) const { // ����������� ��� �� �������
		return namesCategories[i];
	}

	bool AddPidCategory(string name) { // ���������� ���� �� �������
		if (name.length() > 0) {
			namesCategories.push_back(name);
			return true;
		}
		else return false;
	}

	bool isOkPidCategory(const int &category) const { // �������� �� �������
		if (category < namesCategories.size() && category > -1) return true;
		else return false;
	}
};

class AllCategory /*�� �������*/ {	
public:
	vector<Category *> ctg;

	bool isOkCategory(const int &category) const { // �������� �������
		if (category < ctg.size() && category > -1) return true;
		else return false;
	}
};


AllCategory ACtg;


class Product { // ������
	string name; // ���
	Date termOfConsumption; // ����� ����������
	float price; // ֳ��
	int count, category, pidCategory; // ʳ������, �������� ,�� �������� 
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
	string name;
	bool permission = false;
	int countOfPidPerm = 0;
public:
	Permission * permissions = nullptr;
	Permission() {};
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
		}
	}

	string GetName() const {
		return name;
	}

	bool IsOkName(const string &name) const {
		if (name.length() > 0) return true;
		else return false;
	}
	bool GetPermission() const {
		return permission;
	}

	bool SetPermission(const bool &perm) {
		permission = perm;
		return true;
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

	int GetCountOfPidPermissions() const {
		return countOfPidPerm;
	}

	/*void operator = (const string& name) {
		SetName(name);
	}*/
};

class TypeOfTrader /*��� ����������*/ {

	string name; // ��� ���� ����������

	static const int countOfPermsions = 2; // ʳ������ �������

	float salary; // ��������

	

public:

	Permission permissions[countOfPermsions] = { /*�������*/ 
		{string("����� �� ����������� ��������"), 9 ,
			new Permission[9]{
				{ string("����� �� �������� ��������") },
				{ string("����� �� ������������ ������� ��������") },
				{ string("����� �� ����������� ���� ��������") },
				{ string("����� �� ����������� ������ ���������� ��������"), 3, new Permission[3]{
					{ string("����� �� ����������� ��� ������ ���������� ��������") },
					{ string("����� �� ����������� ����� ������ ���������� ��������") },
					{ string("����� �� ����������� ���� ������ ���������� ��������") },
				}},
				{ string("����� �� ����������� ������� ��������") },
				{ string("����� �� ����������� �� ������� ��������") },
				{ string("����� �� ����������� ���� ��������") },
				{ string("����� �� ������� ��������") },
				{ string("����� �� ����������� �������� ��������"), 4, new Permission[4]{
					{ string("����� �� ��������� ���� ������� ��������") },
					{ string("����� �� ������������� ������� ��������") },
					{ string("����� �� ��������� ������� ��������") },
					{ string("����� �� ��������� ������� ��������") , 3, new Permission[4]{
						{ string("����� �� ��������� �� ��������� ��������") },
						{ string("����� �� �������������� �� ��������� ��������") },
						{ string("����� �� ��������� �� ��������� ��������")}
					}},
				}}
			}
		},
		{string("����� �� ����������� ����������"), 7, new Permission[7]{
			{ string("����� �� ����������� ϲ� ����������"), 3, new Permission[3]{
				{ string("����� �� ����������� ���� ����������") },
				{ string("����� �� ����������� ������� ����������") },
				{ string("����� �� ����������� �� ������� ����������") }
			}},
			{ string("����� �� ����������� ������ ����������") },
			{ string("����� �� ����������� ������ ����������") },
			{ string("����� �� ����������� ���� ���������� ����������") , 3, new Permission[3]{
				{ string("����� �� ����������� ��� ����������") },
				{ string("����� �� ����������� ����� ����������") },
				{ string("����� �� ����������� ���� ����������") }
			}},
			{ string("����� �� ��������� ����� ����������") },
			{ string("����� �� ��������� ����������") },
			{ string("����� �� ����������� ���� ����������") , 3, new Permission[3]{
				{ string("����� �� ����������� ���� ���� ����������") },
				{ string("����� �� ����������� �������� ���� ����������") },
				{ string("����� �� ����������� �������"), 2, new Permission[2]{
						{string("����� �� ����� �� ����������� ��������"), 9 , new Permission[9]{
							{ string("����� �� ����� �� �������� ��������") },
							{ string("����� �� ����� �� ������������ ������� ��������") },
							{ string("����� �� ����� �� ����������� ���� ��������") },
							{ string("����� �� ����� �� ����������� ������ ���������� ��������"), 3, new Permission[3]{
								{ string("����� �� ����� �� ����������� ��� ������ ���������� ��������") },
								{ string("����� �� ����� �� ����������� ����� ������ ���������� ��������") },
								{ string("����� �� ����� �� ����������� ���� ������ ���������� ��������") },
							}},
							{ string("����� �� ����� �� ����������� ������� ��������") },
							{ string("����� �� ����� �� ����������� �� ������� ��������") },
							{ string("����� �� ����� �� ����������� ���� ��������") },
							{ string("����� �� ����� �� ������� ��������") },
							{ string("����� �� ����� �� ����������� �������� ��������"), 4, new Permission[4]{
								{ string("����� �� ����� �� ��������� ���� ������� ��������") },
								{ string("����� �� ����� �� ������������� ������� ��������") },
								{ string("����� �� ����� �� ��������� ������� ��������") },
								{ string("����� �� ����� �� ��������� ������� ��������") , 3, new Permission[4]{
									{ string("����� �� ����� �� ��������� �� ��������� ��������") },
									{ string("����� �� ����� �� �������������� �� ��������� ��������") },
									{ string("����� �� ����� �� ��������� �� ��������� ��������")}
								}},
							}}
						}
					},
					{string("����� �� ����� �� ����������� ����������"), 7, new Permission[7]{
						{ string("����� �� ����� �� ����������� ϲ� ����������"), 3, new Permission[3]{
							{ string("����� �� ����� �� ����������� ���� ����������") },
							{ string("����� �� ����� �� ����������� ������� ����������") },
							{ string("����� �� ����� �� ����������� �� ������� ����������") }
						}},
						{ string("����� �� ����� �� ����������� ������ ����������") },
						{ string("����� �� ����� �� ����������� ������ ����������") },
						{ string("����� �� ����� �� ����������� ���� ���������� ����������") , 3, new Permission[3]{
							{ string("����� �� ����� �� ����������� ��� ����������") },
							{ string("����� �� ����� �� ����������� ����� ����������") },
							{ string("����� �� ����� �� ����������� ���� ����������") }
						}},
						{ string("����� �� ����� �� ��������� ����� ����������") },
						{ string("����� �� ����� �� ��������� ����������") },
						{ string("����� �� ����� �� ����������� ���� ����������") , 3, new Permission[3]{
							{ string("����� �� ����� �� ����������� ���� ���� ����������") },
							{ string("����� �� ����� �� ����������� �������� ���� ����������") },
							{ string("����� �� ����� �� ����������� �������") }
						}}
					}}
				}}
			}}	
		}}
	};

	/*
		string("����� �� ����������� ��������") :
			string("����� �� �������� ��������")
			string("����� �� ������������ ������� ��������")
			string("����� �� ����������� ���� ��������")
			string("����� �� ����������� ������ ���������� ��������"):
				string("����� �� ����������� ��� ������ ���������� ��������),
				string("����� �� ����������� ����� ������ ���������� ��������"),
				string("����� �� ����������� h��� ������ ���������� ��������"),
			string("����� �� ����������� ������� ��������")
			string("����� �� ����������� �� ������� ��������")
			string("����� �� ����������� ���� ��������")
			string("����� �� ������� ��������")
			string("����� �� ����������� �������� ��������"):
				string("����� �� ��������� ���� ������� ��������"),
				string("����� �� ������������� ������� ��������"),
				string("����� �� ��������� ������� ��������"),
				string("����� �� ����������� �� �������� ��������"):
					string("����� �� ��������� �� ��������� ��������"),
					string("����� �� �������������� �� ��������� ��������"),
					string("����� �� ��������� �� ��������� ��������"),
		string("����� �� ����������� ����������") :
			string("����� �� ����������� ϲ� ����������"):
				string("����� �� ����������� ���� ����������"),
				string("����� �� ����������� ������� ����������"),
				string("����� �� ����������� �� ������� ����������")
			string("����� �� ����������� ������ ����������")
			string("����� �� ����������� ������ ����������")
			string("����� �� ����������� ���� ���������� ����������"):
				string("����� �� ����������� ��� ����������"),
				string("����� �� ����������� ����� ����������"),
				string("����� �� ����������� ���� ����������"),
			string("����� �� ��������� ����� ����������"),
			string("����� �� ��������� ����������")
			string("����� �� ����������� ���� ����������"):
				string("����� �� ����������� ���� ���� ����������"),
				string("����� �� ����������� �������� ���� ����������"),
				string("����� �� ����������� �������"):
					string("����� �� ����������� ������� �� ����������� ��������") :
							string("����� �� ����������� ������� �� �������� ��������")
							string("����� �� ����������� ������� �� ������������ ������� ��������")
							string("����� �� ����������� ������� �� ����������� ���� ��������")
							string("����� �� ����������� ������� �� ����������� ������ ���������� ��������"):
								string("����� �� ����������� ������� �� ����������� ��� ������ ���������� ��������),
								string("����� �� ����������� ������� �� ����������� ����� ������ ���������� ��������"),
								string("����� �� ����������� ������� �� ����������� h��� ������ ���������� ��������"),
							string("����� �� ����������� ������� �� ����������� ������� ��������")
							string("����� �� ����������� ������� �� ����������� �� ������� ��������")
							string("����� �� ����������� ������� �� ����������� ���� ��������")
							string("����� �� ����������� ������� �� ������� ��������")
							string("����� �� ����������� ������� �� ����������� �������� ��������"):
								string("����� �� ����������� ������� �� ��������� ���� ������� ��������"),
								string("����� �� ����������� ������� �� ������������� ������� ��������"),
								string("����� �� ����������� ������� �� ��������� ������� ��������"),
								string("����� �� ����������� ������� �� ����������� �� �������� ��������"):
									string("����� �� ����������� ������� �� ��������� �� ��������� ��������"),
									string("����� �� ����������� ������� �� �������������� �� ��������� ��������"),
									string("����� �� ����������� ������� �� ��������� �� ��������� ��������"),
					string("����� �� ����������� ������� �� ����������� ����������") :
						string("����� �� ����������� ������� �� ����������� ϲ� ����������"):
							string("����� �� ����������� ������� �� ���� ����������"),
							string("����� �� ����������� ������� �� ������� ����������"),
							string("����� �� ����������� ������� �� �� ������� ����������")
						string("����� �� ����������� ������� �� ����������� ������ ����������")
						string("����� �� ����������� ������� �� ����������� ������ ����������")
						string("����� �� ����������� ������� �� ����������� ���� ���������� ����������"):
							string("����� �� ����������� ������� �� ����������� ��� ����������"),
							string("����� �� ����������� ������� �� ����������� ����� ����������"),
							string("����� �� ����������� ������� �� ����������� ���� ����������"),
						string("����� �� ����������� ������� �� ��������� ����� ����������"),
						string("����� �� ����������� ������� �� ��������� ����������")
						string("����� �� ����������� ������� �� ����������� ���� ����������"):
							string("����� �� ����������� ������� �� ����������� ���� ���� ����������"),
							string("����� �� ����������� ������� �� ����������� �������� ���� ����������"),
							string("����� �� ����������� ������� �� ����������� �������")
			
				
				

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

class Trader /*���������*/ {
	string name, lastName, serName, passwordMD5, email; // ��� , ������� , �� �������, ������������ ������, �����
	Date birthday; // ���� ����������
	int Type; // ��� ����������

	string cryptPass(const string &pass) const {
		return pass;
	}
	//friend void Init(Shop &shop);
public:

	Trader() {};

	Trader(const string &name, const string &lastName, const string &serName, const string &password, const string &email, const int &day, const int &month, const int &year, const int &type) { // ������������ ��� ��������� ��� ������� ��������
		SetAll(name, lastName, serName, password, email, day, month, year, type);
	}

	bool IsOkName(const string &name) { // �������� ����
		if (name.length() > 0) return true;
		else return false;
	}

	bool IsOkEmail(const string& email) const { // �������� Email
		const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		return regex_match(email, pattern);
	}

	bool IsOkLastName(const string &lastName) { // �������� ��������
		if (lastName.length() > 0) return true;
		else return false;
	}

	bool IsOkSerName(const string &serName) { // �������� �� �������
		if (serName.length() > 0) return true;
		else return false;
	}

	bool IsOkDate(const int &day, const int &month, const int &year) const { // ������� ����
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

	bool SetName(const string &name) { // ������������ ����
		if (IsOkName(name)) {
			this->name = name;
			return true;
		}
		else return false;
	}

	bool SetLastName(const string &lastName) { // ������������ �������
		if (IsOkName(lastName)) {
			this->lastName = lastName;
			return true;
		}
		else return false;
	}

	bool SetSerName(const string &serName) { // ������������ �� �������
		if (IsOkName(serName)) {
			this->serName = serName;
			return true;
		}
		else return false;
	}

	bool SetDate(const int &day, const int &month, const int &year) { // ������������ ����
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

	bool SetPassword(const string &pass, const string &testpass = "") { // ������������ ������
		if (IsOkPassword(pass, testpass)) {
			passwordMD5 = cryptPass(pass);
			return true;
		}
		else return false;
	}

	bool SetEmail(const string &email) { // ������������ email
		if (IsOkEmail(email)) {
			this->email = email;
			return true;
		}
		else return false;
	}

	bool SetAll(const string &name, const string &lastName, const string &serName, const string &password, const string &email, const int &day, const int &month, const int &year, const int &type) { // ������������ ��� ���������
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

	string GetName() const { // ����������� ����
		return name;
	}

	string GetLastName() const { // ����������� �������
		return lastName;
	}

	string GetSerName() const { // ����������� �� �������
		return serName;
	}
	
	string GetDate() const { // ����������� ����
		return birthday.GetAllStr();
	}

	int GetType() const { // ����������� ���� ����������
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
	friend Trader* Init(Shop &shop);
};

class Shop {
	AllProducts products;
	AllTraders traders;
	friend Trader* Init(Shop &shop);
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

void printLine(const int &bot, const string & str, const int textColor = 7, const int background = 0) {
	gotoxy(0, bot); // ���������� ������ � ����� ��� ������
	SetColorConsole(textColor, background); // ���� ������� ������ 15 ���� ���, 0 ���� ������
	for (int i = 0; i < (consoleSize.x - str.size()) / 2; i++) cout << " "; // ��������� �������
	cout << str;
	for (int i = 0; i < (consoleSize.x - str.size()) / 2 + 1; i++) cout << " "; // ��������� ������� �������
	SetColorConsole(); // ���������� ������ �� ����������� �����
	gotoxy(0, bot); // ���������� ������ � ����� ��� ������
}

int ShowMenu(vector<string *> *menu) {

	const int speedOpen = 100;

	static bool
		exit = false, // ������ ��� ������ � ��������
		opened = false, // ������ ��������� ����
		opened_show = false, // ������ ��������� ���� �����
		reload = true; // ������ ��� �����������
	static int 
		selected = 0, // �������� ����� ���� 
		selected_show = 0; // ����� ���� ���� ��������� �� �����
	char act; // ������ ������
	if (reload) { // ��������� �� ����� ����������� true
		if (opened) { // ��������� �� ������ ��������� ���� true
			if (!opened_show) { // ��������� �� ������� ���� �������� �� �����
	
				for (int i = 0; i < menu->size(); i++) // �� ������� �������� ������� ���� ����
				{
					gotoxy(consoleSize.x, consoleSize.y - i); // ����������� � ����� ���������� ����� - i
					clearToPos(0, consoleSize.y - i); // ������� ������� ����� - i
					printLine(consoleSize.y - i - 1, "\1", 0, 15); // �������� ��� � �������

					for (int s = 0; s <= i; s++) // ������� �������� �������� ������� menu
					{
						if ((i - s) != selected) printLine(consoleSize.y - s, *(*menu)[s]);
						else printLine(consoleSize.y - s, ">>> " + *(*menu)[s] + " <<<");

					}
					gotoxy(0, 0); // ����������� �� ����� �������
					Sleep(speedOpen / menu->size()); // �����
				}
			}
			else if (selected != selected_show){ // ��������� �� �������� ������� ������� menu ��������� �� ���� 
				printLine(consoleSize.y - (menu->size() - 1 - selected_show), *(*menu)[selected_show]); // ���������� ������ ��� �� ��� �������� �������
				printLine(consoleSize.y - (menu->size() - 1 - selected), ">>> " + *(*menu)[selected] + " <<<"); // ���������� ������ ��� �� �� ���� �������� �������
				selected_show = selected; // ������������� � �� ����
				gotoxy(0, 0); // ����������� �� ����� �������
			}
			opened_show = opened; // ³������ ���� �������� �� �����
		}
		else { // ��������� �� ������ ��������� ���� false
			if (!opened_show) { // ��������� �� ������ ���������� ���� false
				printLine(consoleSize.y, "\1", 0, 15); // �������� ��� � �������
				gotoxy(0, 0); // ����������� �� ����� �������
			}
			else { // ���� ������ ���������� ���� true
				for (int i = menu->size() -1; i >= 0; i--)
				{
					gotoxy(consoleSize.x, consoleSize.y - i-1); // ����������� � ����� ���������� ����� - i
					clearToPos(0, consoleSize.y - i - 1); // ������� ������� ����� - i
					printLine(consoleSize.y - i, "\1", 0, 15); // �������� ��� � �������
					gotoxy(0, 0); // ����������� �� ����� �������
					Sleep(speedOpen / menu->size()); // �����

				}
				
			}
			opened_show = opened;// ������� ���� �������� �� �����
		}
		reload = false; // ������ ����������� false
		
	}
	act = _getch(); // ������ ������
	switch (act)
	{
	case 13:
		return selected;
		break;
	case 80:
		
		
		if (opened) {
			if (selected < menu->size() - 1) selected++;
			else selected = 0;
			reload = true;
		}
		break;
	case 72:
		if (opened) {
			if (selected > 0) selected--;
			else selected = menu->size() - 1;
			reload = true;
		}
		break;
	case 27:
		if (!opened) opened = true;
		else opened = false;
		reload = true;
	}
	return -1;
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

Trader* Init(Shop &shop) {
	POINT pos;
	string pass;
	string email;

	if (shop.traders.traders.size() == 0) { // ���� �������� �������� ������ ���
		typesOfTraders.TOFT.push_back(new TypeOfTrader("root", 1)); // ���������� ����� ��� ����������
		for (int i = 0; i < typesOfTraders.TOFT[0]->GetCountOfPermissions(); i++)  SetAll(&(typesOfTraders.TOFT[0]->permissions[i]));
		cout << "Input your password: ";
		pos = GetPosCur();
		do
		{
			clearToPos(pos);
			getline(cin, pass);
		} while (pass.size() < 1);
		
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
		shop.traders.AddTrader("root", "root", "root", pass, email, 1, 1, 1900, 0);
	}

	Trader * ret = nullptr;
	cl();
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

int main() {
	setlocale(LC_ALL, "Russian");
	Shop shop;
	Trader * user = Init(shop);
	bool exit = false;
	vector<string *> menu;
	for (int i = 0; i < typesOfTraders.TOFT[user->GetType()]->GetCountOfPermissions(); i++)
		if (typesOfTraders.TOFT[user->GetType()]->permissions[i].GetPermission()) menu.push_back(new string(typesOfTraders.TOFT[user->GetType()]->permissions[i].GetName()));
	
	while (!exit)
	{
		ShowMenu(&menu);
	}

	system("pause");

	return 0;
}