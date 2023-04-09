#ifndef __header__
#define __header__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>

static std::vector<std::string> specializations{"Пехота", "Спецназ", "Разведчик", "Снайпер", "Сапёр", "РХБЗ", "Санитар", "Радист"};
static std::vector<std::string> titles{"Рядовой", "Ефрейтор", "Сержант", "Прапорщик", "Лейтенант", "Капитан", "Майор", "Полковник"};
static std::vector<std::string> health{"Здоров", "Условно ранен", "Условно убит", "Условно пропал без вести"};

class Commander;
class FieldCommander;
class Fighter;
class Officer;
class Table;
class ConstTableIt;
class Soldier;
class Division;
class Training;

class Soldier{
	friend class Training;
	protected:
		std::string snp;
		std::string title;
		unsigned int size;
	public:
		std::string getsnp() const{return snp;}
		unsigned int getsize() const{return size;}
		std::string gettitle() const{return title;}
		void setsnp(std::string arg){snp=arg;}
		void settitle(std::string arg){title=arg;}
		void setsize(unsigned int arg){size=arg;}
		virtual ~Soldier(){}
		virtual std::ostream& show(std::ostream&)const=0;
		virtual std::istream& get(std::istream&)=0;
		friend std::istream& operator>>(std::istream& is, Soldier &arg){return arg.get(is);}
		friend std::ostream& operator<<(std::ostream& os, const Soldier &arg){return arg.show(os);}
};

std::ostream& operator<<(std::ostream&, const std::pair<unsigned int, std::pair<std::string, Soldier*>>&);

class ConstTableIt{
	friend class Training;
	private:
		std::map<unsigned int, std::pair<std::string, Soldier*>>::const_iterator now;
	public:
		ConstTableIt(){}
		ConstTableIt(std::map<unsigned int, std::pair<std::string, Soldier*>>::iterator it): now(it){}
		ConstTableIt(std::map<unsigned int, std::pair<std::string, Soldier*>>::const_iterator it): now(it){}
		int operator!=(const ConstTableIt& it) const{return now!=it.now;}
		int operator==(const ConstTableIt& it) const{return now==it.now;}
		const std::pair<unsigned int, std::pair<std::string, Soldier*>> operator*(){return *now;}
		ConstTableIt &operator++();
		ConstTableIt operator++(int);
};

class Table{
	friend class Training;
	private:
		std::map<unsigned int, std::pair<std::string, Soldier*>> array;
	public:
		Table(){}
		~Table();
		friend class ConstTableIt;
		typedef ConstTableIt Const_Iterator;
		void insert(unsigned int arg1, std::pair<std::string, Soldier*> arg2){array.insert(std::make_pair(arg1, arg2));}
		void remove(const unsigned int&);
		Const_Iterator find(const unsigned int &arg) const{return ConstTableIt(array.find(arg));}
		Const_Iterator begin() const{return ConstTableIt(array.begin());}
		Const_Iterator end() const{return ConstTableIt(array.end());}
		std::string gethealth(const unsigned int &arg){return array[arg].first;}
		void sethealth(unsigned int k, std::string arg){array[k].first=arg;}
		unsigned int counthealth(const std::string&);
		friend std::istream& operator>>(std::istream&, Table&);
		friend std::ostream& operator<<(std::ostream&, const Table&);
};

class Division{
	friend class Training;
	friend class Commander;
	private:
		int x;
		int y;
	public:
		Table *list=nullptr;
		void setx(int arg){x=arg;}
		void sety(int arg){y=arg;}
		int getx(){return x;}
		int gety(){return y;}
		friend std::ostream& operator<<(std::ostream&, const Division&);
		friend std::istream& operator>>(std::istream&, Division&);
};

class Officer: virtual public Soldier{
	friend class Training;
	protected:
		std::string post;
		std::map<std::string, int> instructions;
		virtual std::ostream& show(std::ostream&)const;
		virtual std::istream& get(std::istream&);
	public:
		std::string getpost() const{return post;};
		void setpost(std::string arg){post=arg;}
		void addinstruction(std::string arg1, int arg2){instructions[arg1]=arg2;}
		void deleteinstruction(const std::string &arg){instructions.erase(arg);}
};

class Commander: virtual public Officer{
	friend class Training;
	protected:
		Division *troop=nullptr;
		virtual std::ostream& show(std::ostream&)const;
		virtual std::istream& get(std::istream&);
	public:
		Division *gettroop() const{return troop;}
		void settroop(Division *arg){troop=arg;}
};

class Fighter: virtual public Soldier{
	friend class Training;
	protected:
		std::string specialization;
		std::vector<int> orders;
		virtual std::ostream& show(std::ostream&)const;
		virtual std::istream& get(std::istream&);
	public:
		std::string getspecialization() const{return specialization;}
		void setspecialization(std::string arg){specialization=arg;}
		void addorder(int arg){orders.push_back(arg);}
		void deleteorder(const int &arg);
};

class FieldCommander: virtual public Fighter, virtual public Commander{
	friend class Training;
	protected:
		virtual std::ostream& show(std::ostream&)const;
		virtual std::istream& get(std::istream&);
	public:
		friend std::istream& operator>>(std::istream&, Training&);
		friend std::ostream& operator<<(std::ostream&, const Training&);

};

class Training{
	protected:
		FieldCommander *first=nullptr;
		FieldCommander *second=nullptr;
	public:
		friend std::istream& operator>>(std::istream&, Training&);
		friend std::ostream& operator<<(std::ostream&, const Training&);
		FieldCommander *getfirst(){return first;}
		FieldCommander *getsecond(){return second;}
		void setfirst(FieldCommander *arg){first=arg;}
		void setsecond(FieldCommander *arg){second=arg;}
		void setwarrior();
		void getwarrior();
		void counttitle();
		void countdivision();
		void freewarrior();
		void addwarrior();
		void movewarrior();
		~Training();
};

#endif
