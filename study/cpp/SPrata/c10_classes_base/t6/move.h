#ifndef MOVE_H
#define MOVE_H

class Move
{
    double x_;
    double y_;

public:
    Move(double x = 0, double y = 0);
    void Show(void) const;
    Move Add(const Move &adding) const;
    void Reset(double x = 0, double y = 0);
};

#endif // MOVE_H
