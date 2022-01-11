-- Schema for ExecuteAnything DB
-- ------------------------------------------------------
-- Notes:
--		The DB can be referred to via "main". Example: "main"."v_CompilerProfileAll"
--		Use the following querry to get all the ExecuteAnything tables: 
--		select sql || ';' || Char(10) as Sql  FROM sqlite_master where type = 'table' AND tbl_name not like 'z%' order by tbl_name
--		Example table delete: 
--				DROP TABLE IF EXISTS foofoo;
-- ------------------------------------------------------

CREATE TABLE "AdvProfile_Variables" (
	"PrgLangName"	TEXT NOT NULL,
	"ProfileName"	TEXT NOT NULL DEFAULT 'Default',
	"VarName"		TEXT NOT NULL,		-- Name of the variable to set in NppExec script.
	"VarValue"		TEXT NOT NULL,		-- Value of the variable. Can include other variable names IE: $(FILE_DIR). When fields Dir or File is set, this value is the name of the file/directory.
	"SetVar"		BOOLEAN DEFAULT 1,	-- If true, sets a regular variable. 
	"SetEnvVar"		BOOLEAN,			-- If true, sets an envirnmental variable. 
	"AddToPath"		BOOLEAN,			-- If true, adds variable value to environmental path $(SYS.PATH)
	"AddVarValueToEnvVar"	TEXT,		-- If populated, adds VarValue to the specified environmental variable
	"AddVarNameToEnvVar"	TEXT,		-- If populated, adds $(VarName) to the specified environmental variable
	"SetLocalVar"	BOOLEAN,			-- If true, sets a local variable. Avoid using local variables, because they do not display in the NppExecConsole after execution completes
	"GetValueFromSrcFile" BOOLEAN,		-- If true, uses VarValue to do a regex search
	"Dir"	INTEGER,					-- If VarValue has no drive letter, then treaded as sub dir under FILE_DIR. Bit arguments. 128=Ignore 1,4,8,&16 if dest dir exists;1=creates dir having dir name VarValue;4=copies dir from VarValue to FileArg;8=moves dir from VarValue to FileArg path;16=renames dir from VarValue to FileArg name;32=deletes dir named VarValue;
	"File"	INTEGER,					-- Bit arguments. 128=Ignore 1,2,4,8,&16 if dest file exists;1=creates empty file having file name VarValue;2=creates file with FileArg content having file name VarValue;4=copies file from VarValue to FileArg;8=moves file from VarValue to FileArg path;16=renames file from VarValue to FileArg name;32=deletes file named VarValue;
	"FileArg"	TEXT,					-- If not NULL, creates a file with VarValue name, and populated with FileContent.
	"ProcessOrder" INTEGER NOT NULL DEFAULT 0,
	"Notes"	TEXT,
	CONSTRAINT "LangVariable" PRIMARY KEY("PrgLangName","ProfileName","VarName"),
	--CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName"),
	--CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName"),
	CONSTRAINT "LangVariable" UNIQUE("PrgLangName","ProfileName","VarName")
);


CREATE TABLE "AdvProfile_Settings" (
	"PrgLangName"						TEXT NOT NULL,
	"ProfileName"						TEXT NOT NULL,
	"Notes"								TEXT,
	"PreBuildCmdBin"					TEXT,
	"PreBuildCmdFormat"					TEXT,
	"RebuildCmdBin"						TEXT,
	"RebuildCmdFormat"					TEXT,
	"CleanCmdBin"						TEXT,
	"CleanCmdFormat"					TEXT,
	"DebuggerCmdBin"					TEXT,
	"DebuggerCmdFormat"					TEXT,
	"SourceBasePath"					TEXT,
	"NppExecConsoleLogging"				BOOLEAN,
	"EnableConsoleLogging"				BOOLEAN,
	"ChangeFocusToConsleBeforeExecute"	BOOLEAN,
	"DoNotSaveDocBeforeExecute"			BOOLEAN,	-- If true, source code file is NOT saved before running/compiling the file.
	"NpeExecuteType"					INTEGER,	-- 0=eNpeExecuteParam2;1=eNpeExecuteQueuedParam;2=eNpeExecuteCollateralParam
	CONSTRAINT "LangProfile" UNIQUE("PrgLangName","ProfileName"),
	CONSTRAINT "LangProfile" PRIMARY KEY("PrgLangName","ProfileName"),
	CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName"),
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName")
);

CREATE TABLE "FileExt" (
	"Ext"	TEXT NOT NULL CHECK("Ext" LIKE ".%"),
	"PrgLangName"	TEXT NOT NULL,
	"DoNotRun"	BOOLEAN, -- Use for files like *.h or *.hpp
	"Notes"	TEXT,
	CONSTRAINT "LangFileExt" UNIQUE("Ext","PrgLangName"),
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName"),
	CONSTRAINT "LangFileExt" PRIMARY KEY("Ext","PrgLangName")
);

CREATE TABLE "InstallationPaths" (
	"PrgLangName"	TEXT NOT NULL DEFAULT '[any]', 		-- Associated program language, or [any]
	"ProfileName"	TEXT NOT NULL DEFAULT '[any]',	-- Associated profile name , or [any]
	"Reference"	TEXT NOT NULL,							-- Reference name for the installation.
	"DefaultInstallPath"	TEXT NOT NULL,				-- The default path installed
	"SrcBinUrlOrUnc"	TEXT NOT NULL DEFAULT '',		-- Installation source URL
	"SrcHomeWebsite"	TEXT,							-- Home page associated with the installation.
	"InstalledPath"	TEXT,								-- Do NOT manually populate this field.  RunAny should populate this field when it finds a valid Path
	"Info"	TEXT,										-- Any detail making this different from similar install
	"ValidationBinFileName"	TEXT,						-- A binary file that must be in the folder in order for the folder to be a valid path
	"ValidationTextFileName"	TEXT,					-- A text file to look for to validate the folder path is correct.
	"ValidationFileContent"	TEXT,						-- Content that needs to be in the text file
	"PathType"	TEXT NOT NULL DEFAULT 'misc',			-- Can be 'include', 'compiler', 'linker', 'lib', 'runner', 'user', 'misc'
	"InstallInstructions"	TEXT,
	"Notes"	TEXT,
	"Is_x64"	BOOLEAN DEFAULT 0,						-- Set true if x64 install, or if install is both x64 and x86
	"Is_x86"	BOOLEAN DEFAULT 0,						-- Set true if x86 (32bit) install, or if install is both x64 and x86
	"Is_UserDef"	BOOLEAN DEFAULT 0,					-- Set true if added by user. To be used by configuration GUI, and not by RunAnyExecuteAnything.	
	CONSTRAINT "LangSrcBin" UNIQUE("PrgLangName","ProfileName","Reference","DefaultInstallPath","SrcBinUrlOrUnc"),
	CONSTRAINT "LangSrcBin" PRIMARY KEY("PrgLangName","ProfileName","Reference","DefaultInstallPath","SrcBinUrlOrUnc"),
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName"),
	CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName")
);

CREATE TABLE "LangInstallationUsage" (
	"PrgLangName"	TEXT NOT NULL, 						-- Associated program language
	"ProfileName"	TEXT NOT NULL DEFAULT 'Default',	-- Associated profile name
	"Reference"		TEXT NOT NULL,						-- Reference name for the installation.
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName"),
	CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName"),
	CONSTRAINT "LangInstallUsageConstraint" PRIMARY KEY("PrgLangName","ProfileName","Reference"),
	CONSTRAINT "LangInstallUsageConstraint" UNIQUE("PrgLangName","ProfileName","Reference")
);

CREATE TABLE "MiscData" (
	"DataName"	TEXT NOT NULL UNIQUE,
	"DataValue"	CLOB NOT NULL,
	PRIMARY KEY("DataName")
);

CREATE TABLE "Prerequisites" (
		"PrgLangName"				TEXT NOT NULL,
		"ProfileName"				TEXT NOT NULL DEFAULT 'Default',
		"PathName"					TEXT NOT NULL, -- Path should be relative to the source file folder
		"IsDir"						BOOLEAN NOT NULL DEFAULT 0,
		"FileContent"				TEXT, -- If path is DIR, his can be NIL or empty.
		CONSTRAINT "FK_PrgLangName" FOREIGN KEY( "PrgLangName" ) REFERENCES "PrgLang"("PrgLangName"),
		CONSTRAINT "FK_ProfileName" FOREIGN KEY( "ProfileName" ) REFERENCES "Profile"("ProfileName"),
		CONSTRAINT "PrerequisiteLangProfile" UNIQUE( "PrgLangName", "ProfileName", "PathName" ),
		CONSTRAINT "PrerequisiteLangProfile" PRIMARY KEY( "PrgLangName", "ProfileName", "PathName" )
);

CREATE TABLE "PrgLang" (
	"PrgLangName"	TEXT NOT NULL UNIQUE,
	"CommentLineOpen"	TEXT,
	"CommentOpen"	TEXT,
	"CommentClose"	TEXT,
	"CommentLineContinue"	TEXT,
	"CommentLineClose"	TEXT,
	"Notes"	TEXT,
	PRIMARY KEY("PrgLangName")
);

CREATE TABLE "PrgLangAlias" (
		"PrgLangName" TEXT NOT NULL,
		"Alias"	TEXT NOT NULL UNIQUE,
		"Notes"	TEXT,
		CONSTRAINT FK_PrgLangName FOREIGN KEY( "PrgLangName" )	REFERENCES "PrgLang"("PrgLangName"),
		CONSTRAINT LanguageAlias UNIQUE( "PrgLangName", "Alias"  ),
		CONSTRAINT LanguageAlias PRIMARY KEY( "PrgLangName", "Alias" )
);

CREATE TABLE "ProgrammingLanguageProfiles" (
	"PrgLangName"				TEXT NOT NULL,
	"ProfileName"				TEXT NOT NULL DEFAULT 'Default',
	"Launcher"					TEXT, 									-- For scripting languages. The FQN (Fully Qualified Name), aka full path, of the script launcher.
	"LauncherCmdFormat"			TEXT DEFAULT '$(Runner) "$(FQN)"' , 	-- For scripting languages. The command line format in which to call the launcher.
	"Compiler"					TEXT, 									-- FQN of the compiler/builder.  If this value is NOT defined, the remaining fields are ignored.
	"CompilerCmdFormat"			TEXT DEFAULT '$(Compiler) "$(FQN)"', 	-- The command line format in which to call the compiler/builder.
	"PostBuildCmdFormat"		TEXT, 									-- The command line format in which to call the linker/post builder.
	"PostBuildCmdBin"			TEXT, 									-- Defines FQN for the linker/post-builder. This field can be empty if the binary is the same as the compiler/builder.
	"BinaryToCompile"			TEXT, 									-- Only needed when target binary is NOT $(FILE_BASE).exe (Like Java)
	"BinaryToExecute"			TEXT, 									-- Only needed when different from BinaryToCompile
	"FilesToDeleteBeforeBuild"	TEXT DEFAULT 'Default',					-- A comma deleimited list of files to delete, or equal to 'Default'.  If equal to Default, the following files get deleted: $(FILE_BASE).exe,$(FILE_BASE).exp,$(FILE_BASE).lib,$(FILE_BASE).o,$(FILE_BASE).obj,$(FILE_BASE).nl,$(FILE_BASE).ilk,$(FILE_BASE).pdb,$(FILE_BASE).ali
	"ClearSysEnvPath"			BOOLEAN,								-- If false, existing environmental path is postfixed instead of replaced.
	"Uses_UNIX_PATH"			BOOLEAN,								-- If true, adds script variables UNIX_PATH and UNIX_DIR which have UNIX seperaters and converts drive prefix to a UNIX path.  IE: (C:\User\Foo\mydata.txt) converts to (/C/User/Foo/mydata.txt)
	CONSTRAINT "PK_PrgLangProfile" PRIMARY KEY("PrgLangName","ProfileName","Launcher","LauncherCmdFormat","Compiler","CompilerCmdFormat"),
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName"),
	CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName"),
	CONSTRAINT "PK_PrgLangProfile" UNIQUE("PrgLangName","ProfileName","Launcher","LauncherCmdFormat","Compiler","CompilerCmdFormat")
);

CREATE TABLE "SrcCode" ( -- For now, duplicates are allowed.  ToDo: Clean up this data, remove duplicates, and add unique CONSTRAINT based on TypeName and PrgLangName
	"TypeName"	TEXT NOT NULL,
	"PrgLangName" TEXT NOT NULL,
	"ProfileName"	TEXT,			-- Needed for test automation
	"SourceCode"	CLOB NOT NULL,
	"Notes"	TEXT,
	CONSTRAINT FK_TypeName FOREIGN KEY( "TypeName" )	REFERENCES "zzTemp_DeleteMe_SrcCodeType"("TypeName"),
	CONSTRAINT FK_PrgLangName FOREIGN KEY( "PrgLangName" )	REFERENCES "PrgLang"("PrgLangName")
);

CREATE TABLE "SrcCodeType" (
		"TypeName"	TEXT NOT NULL, -- Type of source code: IE: 'Hello World', 'What is your name', etc...
		"Description"	TEXT NOT NULL,
		"Notes"	TEXT,
		CONSTRAINT "PK_SrcCodeType" PRIMARY KEY("TypeName"),
		CONSTRAINT "PK_SrcCodeType" UNIQUE("TypeName")
);

CREATE TABLE "TestAutomationSrcCode" (
	"PrgLangName" 	TEXT NOT NULL,
	"ProfileName"	TEXT DEFAULT "Default",
	"SourceCode"	CLOB NOT NULL,
	"Ext" 			TEXT NOT NULL,
	"TestType"		INTEGER, 		-- If value NULL or 0, test type is basic. If value 1, test type is advanced. See ExecuteAnythingValidation2 description
	"Notes"			TEXT,
	CONSTRAINT "LangProfile" UNIQUE("PrgLangName","ProfileName","Ext"),
	CONSTRAINT "LangProfile" PRIMARY KEY("PrgLangName","ProfileName","Ext"),
	CONSTRAINT "FK_ProfileName" FOREIGN KEY("ProfileName") REFERENCES "Profile"("ProfileName"),
	CONSTRAINT "FK_PrgLangName" FOREIGN KEY("PrgLangName") REFERENCES "PrgLang"("PrgLangName")
);

CREATE TABLE "TestAutomationTestType" (
	"TestType"		INTEGER NOT NULL, 		-- Bit value
	"Name"			TEXT NOT NULL,			-- Test name
	"Description"	TEXT NOT NULL,			-- Test Description
	CONSTRAINT AutomationTestType UNIQUE( "TestType"  ),
	CONSTRAINT AutomationName UNIQUE("Name"),
	CONSTRAINT PK_Automation PRIMARY KEY("TestType")
);



























