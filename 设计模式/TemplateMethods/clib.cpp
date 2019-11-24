#include "clib.h"
#include <iostream>

CLib::CLib()
{
    std::cout << "CLib::CLib" << std::endl;
}

CLib::~CLib()
{
    std::cout << "CLib::~CLib" << std::endl;
}

bool CLib::Initialize()
{
    std::cout << "CLib::Initialize" << std::endl;
    return true;
}

bool CLib::Create()
{
    std::cout << "CLib::Create" << std::endl;
    return true;
}

bool CLib::Work()
{
    std::cout << "CLib::Work" << std::endl;
    return true;
}

void CLib::Close()
{
    std::cout << "CLib::Close" << std::endl;
}

bool CLib::Run()
{
    std::cout << "CLib::Run" << std::endl;
    if(!Initialize())return false;
    if(!Create())return false;
    if(!Work())return false;
    Close();
    return true;
}
