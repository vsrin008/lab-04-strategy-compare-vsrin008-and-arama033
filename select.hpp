#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains: public Select_Column{
private:
	std::string name;
public: 
      Select_Contains(const Spreadsheet* sheet, const std::string& columnName, const std::string& name): Select_Column(sheet&, columnName){
this->name=name;
}
bool select(const std::string& s) const{ 
return (s.find(name,0) != string::npos);
}
};

class Select_And : public Select_Column{
private : 
	Select* a= nullptr;
	Select* b = nullptr;
public:
	Select_And(const Select*one, const Select* two){
		a=one;
		b=two;
}
~Select_And(){
	delete a;
	delete b;
}
bool select(const Spreadsheet* sheet, int row) const{
	return a->select(sheet,row) && b->select(sheet,row);
}


class Select_Or: public Select_Column {
private: 
	Select* a = nullptr;
	Select* b = nullptr;
public:
	Select_Or(const Select* one, const Select* two) {
		a = one;
		b = two;
        }
	~Select_Or(){
		delete a;
		delete b;
	}
	bool select(const Spreadsheet* sheet, int row) const{
		return a->select(sheet,row) ||b->select(sheet, row);
	}
};

class Select_Not: public Select_Column {
private:
	Select* a = nullptr;
public:
	Select_Not(const Select* one) {
		a = one;
	}
	~Select_Not(){
		delete a;
	}
	bool select(const Spreadsheet* sheet, int row) const{
		return !(a->select(sheet,row));
};
		

#endif //__SELECT_HPP__
