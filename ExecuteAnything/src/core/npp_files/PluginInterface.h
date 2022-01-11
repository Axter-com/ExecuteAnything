// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#pragma once

#include "Scintilla.h"
#include "Notepad_plus_msgs.h"

const int nbChar = 64;

typedef const TCHAR * (__cdecl * PFUNCGETNAME)();

struct NppData
{
	HWND _nppHandle;
	HWND _scintillaMainHandle;
	HWND _scintillaSecondHandle;
};

typedef void (__cdecl * PFUNCSETINFO)(NppData);
typedef void (__cdecl * PFUNCPLUGINCMD)();
typedef void (__cdecl * PBENOTIFIED)(SCNotification *);
typedef LRESULT (__cdecl * PMESSAGEPROC)(UINT Message, WPARAM wParam, LPARAM lParam);


struct ShortcutKey
{
	bool _isCtrl;
	bool _isAlt;
	bool _isShift;
	UCHAR _key;
};
/*
ShortcutKey::_key = (Virtual-Key)
Cheatsheet for common Virtual-Key codes:
F1 key		0x70	VK_F1
F2 key		0x71	VK_F2
F3 key		0x72	VK_F3
F4 key		0x73	VK_F4
F5 key		0x74	VK_F5
F6 key		0x75	VK_F6
F7 key		0x76	VK_F7
F8 key		0x77	VK_F8
F9 key		0x78	VK_F9
F10 key		0x79	VK_F10
F11 key		0x7A	VK_F11
F12 key		0x7B	VK_F12	(FYI: VK_F13-VK_F24 fail to work, and produces "Unlisted" on the menu)
A key		0x41
B key		0x42
C key		0x43
D key		0x44
E key		0x45
F key		0x46
G key		0x47
H key		0x48
I key		0x49
J key		0x4A
K key		0x4B
L key		0x4C
M key		0x4D
N key		0x4E
O key		0x4F
P key		0x50
Q key		0x51
R key		0x52
S key		0x53
T key		0x54
U key		0x55
V key		0x56
W key		0x57
X key		0x58
Y key		0x59
Z key		0x5A
For more Virtual-Key codes, see "WinUser.h" or the following link: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*/


struct FuncItem
{
	TCHAR _itemName[nbChar];
	PFUNCPLUGINCMD _pFunc;
	int _cmdID;
	bool _init2Check;
	ShortcutKey *_pShKey;
};

typedef FuncItem * (__cdecl * PFUNCGETFUNCSARRAY)(int *);

// You should implement (or define an empty function body) those functions which are called by Notepad++ plugin manager
extern "C" __declspec(dllexport) void setInfo(NppData);
extern "C" __declspec(dllexport) const TCHAR * getName();
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *);
extern "C" __declspec(dllexport) void beNotified(SCNotification *);
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam);

// This API return always true now, since Notepad++ isn't compiled in ANSI mode anymore
extern "C" __declspec(dllexport) BOOL isUnicode();

