#ifndef CLIB_H
#define CLIB_H


class CLib
{
public:
    CLib();
    virtual ~CLib();

protected:
    bool Initialize();
    bool Create();
    virtual bool Work();
    virtual void Close();

public:
    bool Run();
};

#endif // CLIB_H
