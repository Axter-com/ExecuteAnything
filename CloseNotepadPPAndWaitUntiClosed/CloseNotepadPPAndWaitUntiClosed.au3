ConsoleWrite("Closing Notepad++" & Chr(13))
If WinExists("[CLASS:Notepad++]") Then
	ConsoleWrite("Notepad++ exist" & Chr(13))
	WinClose ("[CLASS:Notepad++]")
	WinWaitClose("[CLASS:Notepad++]", "", 60)
	ConsoleWrite("Notepad++ is closed" & Chr(13))
else
	ConsoleWrite("Notepad++ does NOT exist" & Chr(13))
EndIf
ConsoleWrite("Exiting...." & Chr(13))



