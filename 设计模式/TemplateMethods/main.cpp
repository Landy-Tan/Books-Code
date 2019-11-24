#include <iostream>
#include "clib.h"
#include "capp.h"
using namespace std;

int main(int argc, char *argv[])
{
    CApp *p = new CApp;
    p->Run();
    delete p;
    return 0;
}
