/*
 *  tapctl -- Utility to manipulate TUN/TAP interfaces on Windows
 *            https://community.openvpn.net/openvpn/wiki/Tapctl
 *
 *  Copyright (C) 2002-2018 OpenVPN Inc <sales@openvpn.net>
 *  Copyright (C) 2008-2013 David Sommerseth <dazo@users.sourceforge.net>
 *  Copyright (C) 2018 Simon Rozman <simon@rozman.si>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define _UNICODE
#define UNICODE

#define WIN32_LEAN_AND_MEAN
#define NOCOMM

#include <windows.h>

#include <objbase.h>
#include <setupapi.h>
#include <stdio.h>
#include <tchar.h>

#include "error.h"
#include "tap.h"

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "setupapi.lib")
#endif

bool dont_mute(unsigned int flags) {
    UNREFERENCED_PARAMETER(flags);

    return true;
}

void x_msg_va(const unsigned int flags, const char *format, va_list arglist) {
    /* Output message string. Note: Message strings don't contain line terminators. */
    vfprintf(stderr, format, arglist);
    _ftprintf(stderr, TEXT("\n"));

    if ((flags & M_ERRNO) != 0) {
        /* Output system error message (if possible). */
        DWORD dwResult = GetLastError();
        LPTSTR szErrMessage = NULL;
        if (FormatMessage(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                0,
                dwResult,
                0,
                (LPTSTR)&szErrMessage,
                0,
                NULL) &&
            szErrMessage) {
            /* Trim trailing whitespace. Set terminator after the last non-whitespace character. This prevents excessive trailing line breaks. */
            for (size_t i = 0, i_last = 0;; i++) {
                if (szErrMessage[i]) {
                    if (!_istspace(szErrMessage[i])) {
                        i_last = i + 1;
                    }
                } else {
                    szErrMessage[i_last] = 0;
                    break;
                }
            }

            /* Output error message. */
            _ftprintf(stderr, TEXT("Error 0x%x: %s\n"), dwResult, szErrMessage);

            LocalFree(szErrMessage);
        } else {
            _ftprintf(stderr, TEXT("Error 0x%x\n"), dwResult);
        }
    }
}

/**
 * Program entry point
 */
int __cdecl main(int argc, LPCTSTR argv[]) {
    BOOL reboot_required = FALSE;
    GUID guid = {0};

    if (tap_create_interface(NULL, TEXT("Wintun"), &reboot_required, &guid) == ERROR_SUCCESS) {
        _ftprintf(stdout, TEXT("Ok.\n"));
        _ftprintf(stdout, TEXT("Press ENTER to delete...\n"));

        getchar();
        if (tap_delete_interface(NULL, &guid, &reboot_required) == ERROR_SUCCESS) {
            _ftprintf(stdout, TEXT("Device deleted.\n"));
        } else {
            _ftprintf(stderr, TEXT("Failed to delete.\n"));
        }

        return 0;
    } else {
        _ftprintf(stderr, TEXT("An error occurred.\n"));
        return 1;
    }
}
