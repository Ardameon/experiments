#include <iostream>
#include <string>

using namespace std;

class Person
{
    string _name;

public:

    Person()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Person(string &name)
    {
        _name = name;
        cout << __PRETTY_FUNCTION__ << endl;
    }

    virtual ~Person()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    void ShowName(void)
    {
        cout << "Name: " << _name << endl;
    }
    
};

class Human : public Person
{
public:
    Human()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Human(string &name) : Person(name)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    ~Human()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
    
};

class Student : public Human
{
public:
    Student()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    Student(string &name) : Human(name)
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }

    ~Student()
    {
        cout << __PRETTY_FUNCTION__ << endl;
    }
    
};

int main(int argc, char const *argv[])
{
    string name = "John";

    Person *person = new Student(name);

    person->ShowName();

    delete person;

    return 0;
}

