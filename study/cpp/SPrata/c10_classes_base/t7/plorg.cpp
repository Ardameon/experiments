#include "plorg.h"
#include <iostream>
#include <cstring>

Plorg::Plorg(const std::string &name, int contentment_idx)
{
    contmt_idx_ = contentment_idx;
    // Plorg's name can't be longer then eNameLenMax
    name_.insert(0, name, 0, eNameLenMax);
}

Plorg::Plorg(const char *name)
{
    int len = strlen(name);
    contmt_idx_ = 50;
    name_.insert(0, name, len <= eNameLenMax ? len : eNameLenMax);
}

void Plorg::SetCI(int ci)
{
    contmt_idx_ = ci;
}

void Plorg::Show() const
{
    using std::cout;

    cout << "Name: " << name_ << "(" << name_.length() << ") " << "CI: " << contmt_idx_ << "\n";
}
