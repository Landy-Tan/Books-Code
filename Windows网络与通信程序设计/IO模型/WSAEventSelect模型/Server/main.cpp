#include <iostream>
#include <string>
#include "CEventSelect.h"

int main()
{
	try {
		CEventSelect event(2, 2);
		if (!event.CreateServer(AF_INET, htons(8090), INADDR_ANY))throw std::exception("The server creation project failed.");
		while (true)event.Select(TRUE, FALSE, FALSE, 3000);
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
	system("pause");
	return 0;
}
