#include "stdafx.h"
#include "TConsoleCmd.h"



void TConsoleCmd::Initialize()
{
	std::string cmd;

	while(1)
	{
		std::cin >> cmd;

		if(!cmd.empty())
		{
			if(cmd.compare("/end") == 0)
			{

			} else
			{
				std::cout << "Unknown Command" << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
