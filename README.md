# ExecuteAnything
ExecuteAnything is A Notepad++ plugin that can execute source code for any programming language. By pressing the execution key (F5), ExecuteAnything automatically determines if the source code needs to be compiled first, or if it can be immediately executed via runtime interpreter. It takes the appropriate action based on the file extension. If required, the source file is compiled/built before executed.


## Intro
This program is based on an NppExec script I wrote that supported almost 50 programming languages.  For reference purposes, the original script is included in this repository [npes_saved.txt](https://github.com/David-Maisonave/ExecuteAnything/blob/master/ExecuteAnything/src/Misc/npes_saved.txt)

While developing the script, I started to notice a performance impact when I reached the 25th language.  By the time I integrated the 47th language, the script start up had too much of a delay (5-10 seconds).
Moreover it was becoming hard to manage additional languages. Some of the program language installations did not play nice with each other, and caused conflicts.

At that point, I decided to port the code to C++ plugin, and use a SQLite database to manage the supported programming languages. ExecuteAnything still uses NppExec to run a script which executes the source file, but the script is much smaller and some preprocessing is done before the script starts.


## How it works
* The database contains information about default installation paths for each supported language.  When a source file type is executed for the first time, ExecuteAnything will search for the installation path based on the default installation path. If the installation is not in the default path, it will search other well known paths.
* When the correct path is found, the path is added to the database, so it doesn't have to search for it on the next run.
* Although the database include source URL's for most of the language installations, the program does not yet have the implementation to prompt the user to download the installation. That logic is expected to be implemented shortly before the last Beta.
* Once the installation path is determined, the Cmd-Format is pulled from the language profile, which dictates how to execute the source file.

Of course a lot more happens in the background code, and this is just a simplified explanation on how it works.


## Status
Currently, ExecuteAnything only supports 80% (39) of the languages supported in the script. ExecuteAnything will stay in Beta until it reaches 100% of the supported languages in the original script.

## Current Supported Programming Languages
* AWK
* Ada
* AutoHotKey
* AutoIt
* Batch
* Batch
* C
* C++
* COBOL
* CSharp
* D
* Dart
* Fortran
* Go
* Groovy
* IO
* Java
* Javascript
* Lisp
* Logo
* Lua
* Nim
* PHP
* Pascal
* Perl
* Powershell
* Prolog
* Python
* Python
* Qi
* R
* Racket
* Ruby
* Rust
* SED
* SICP
* Scala
* VB.NET
* VBScript

## Adding Additional Programming Languages
A GUI will be added (Post-Beta) to allow users to add additional programming languages.
For now, users can add languages by using SQLiteBrowser (https://sqlitebrowser.org/). 

**Important!!**
Make sure to backup the database before making any changes.  After making changes, make sure to click "Write Changes" on SqliteBrowser before testing it with ExecuteAnything.

Use the ProgrammingLanguageProfiles table to add new languages. The fields are mostly self explanatory.  When time permits, more details will be added in this section to describe each field.

