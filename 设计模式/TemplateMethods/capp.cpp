#include "capp.h"
#include <iostream>

CApp::CApp()
{
    std::cout << "CApp::CApp" << std::endl;
}

CApp::~CApp()
{
    std::cout << "CApp::~CApp" << std::endl;
}

bool CApp::Work()
{
    std::cout << "CApp::Work" << std::endl;
    return true;
}

void CApp::Close()
{
    std::cout << "CApp::Close" << std::endl;
}
