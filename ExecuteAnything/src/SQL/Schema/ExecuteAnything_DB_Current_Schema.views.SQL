-- Views for productions DB
-- select 'DROP VIEW IF EXISTS ' || tbl_name || ';' || Char(10) || sql || ';' || Char(10) as Sql  FROM sqlite_master where type = 'view' AND tbl_name not like 'z%' order by tbl_name

DROP VIEW IF EXISTS v_ProgrammingLanguageProfiles;
CREATE VIEW v_ProgrammingLanguageProfiles
AS
select Ext, s.PrgLangName, s.ProfileName, cast((length(s.Launcher) > 0) as Boolean)  isScript, cast((length(s.Compiler) > 0) as Boolean) isCompiler
from "FileExt" x, ProgrammingLanguageProfiles s
where x.PrgLangName like s.PrgLangName;

CREATE VIEW v_LastPrgLangTraitsID
AS
SELECT MAX(ID) as MaxID FROM PrgLangTraits;

