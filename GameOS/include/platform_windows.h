#pragma once

#define NOMINMAX

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <imagehlp.h>
    #include <tchar.h>
#else

    #include "platform_windef.h"
    #include "platform_winbase.h"
    #include "platform_winuser.h"
    #include "platform_winnls.h"
    #include "platform_mmsystem.h"
#endif // PLATFORM_WINDOWS_H
