#pragma once

class GraphicsInterface;
class BmpManager;

#include <Windows.h>  // Include Win32 API
#include <tchar.h>	  // Include UNICODE support
#include <sstream>	  // Include ostringstream support
#include <ctime>
using namespace std;

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.

#include <commdlg.h> // Include the Common Dialogs.

#include <process.h>

//#include "resource.h" // Include the Resource IDs.

#include "BmpAnimate.h"
#include "HTable.h"
#include "SLList.h"
#include "GraphicsInterface.h"
#include "BmpManager.h"
