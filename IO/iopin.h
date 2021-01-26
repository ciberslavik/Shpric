#ifndef IOPIN_H
#define IOPIN_H


class IOPin
{
    enum Direction
    {
        Input,
        Output
    };

public:
    IOPin(Direction dir);
    Direction Direction();
    bool State();
    void setState(bool state);
private:
    enum Direction _dir;
    bool _state;

};

#endif // IOPIN_H
