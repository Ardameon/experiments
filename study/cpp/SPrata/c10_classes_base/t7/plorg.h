#ifndef PLORG_H
#define PLORG_H
#include <string>

class Plorg
{
    enum {eNameLenMax = 19};
    std::string name_;
    int contmt_idx_;

public:
    Plorg(const std::string &name = "Plorga", int contentment_idx = 50);
    Plorg(const char *name);
    void SetCI(int ci);
    void Show() const;
};

#endif // PLORG_H
