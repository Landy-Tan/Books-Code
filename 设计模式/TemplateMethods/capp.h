#ifndef CAPP_H
#define CAPP_H

#include "clib.h"

class CApp : public CLib
{
public:
    CApp();
    virtual ~CApp();

protected:
    virtual bool Work();
    virtual void Close();
};

#endif // CAPP_H
