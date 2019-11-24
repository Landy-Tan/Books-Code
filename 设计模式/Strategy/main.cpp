#include <iostream>
#include <time.h>
#include "calgorithmfactory.h"
#include "csortalgorithm.h"
using namespace std;

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    const size_t cunArrayLength = 10;
    int *pnArray = new int[cunArrayLength]{0};
    for(size_t nIndex = 0; nIndex < cunArrayLength; nIndex++)
        *(pnArray + nIndex) = rand()%1024;

    CAlgorithmFactory factory = CAlgorithmFactory(ALGORITHM_TYPE::QUICK);
    CSortAlgorithm algorithm(&factory);
    algorithm.Run(pnArray, cunArrayLength);


    for(size_t nIndex = 0; nIndex < cunArrayLength; nIndex++)
        cout << *(pnArray + nIndex) << "\t";
    cout << endl;

    delete[] pnArray;
    pnArray = nullptr;

    return 0;
}
