//////////////////////////////////////////////////////////////////////////////////
//// Filename: main.cpp
//////////////////////////////////////////////////////////////////////////////////
//#include "systemclass.h"
//
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
//{
//	SystemClass* System;
//	bool result;
//
//
//	// Create the system object.
//	System = new SystemClass;
//
//	// Initialize and run the system object.
//	result = System->Initialize();
//	if (result)
//	{
//		System->Run();
//	}
//
//	// Shutdown and release the system object.
//	System->Shutdown();
//	delete System;
//	System = 0;
//
//	return 0;
//}


////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    SystemClass* System;
    bool result;

    // Создаем объект системы
    System = new SystemClass;
    if (!System)
    {
        return 0;
    }

    // Инициализируем и запускаем систему
    result = System->Initialize();
    if (result)
    {
        System->Run();
    }

    // Освобождаем ресурсы системы
    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}