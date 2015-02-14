// stdafx.h : Includedatei f�r Standardsystem-Includedateien
// oder h�ufig verwendete projektspezifische Includedateien,
// die nur in unregelm��igen Abst�nden ge�ndert werden.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <thread>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


#include "mysql_connection.h"
#include "TDatabase.h"
#include "TServer.h"
#include "TPacketCypher.h"
#include "TLogin.h"
#include "TGeneral.h"
#include "TConfig.h"


#pragma comment(lib, "mysqlcppconn.lib")
#pragma comment(lib, "ws2_32.lib")



// TODO: Hier auf zus�tzliche Header, die das Programm erfordert, verweisen.
