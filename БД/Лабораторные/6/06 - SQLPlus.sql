-- 1.	Найдите на компьютере конфигурационные файлы SQLNET.ORA и TNSNAMES.ORA и ознакомьтесь с их содержимым.

-- 2.	Соединитесь при помощи sqlplus с Oracle как пользователь SYSTEM, получите перечень параметров экземпляра
-- Oracle.
--system Qwerty12345
-- show parameter;

-- 3.	Соединитесь при помощи sqlplus с подключаемой базой данных как пользователь SYSTEM, получите список табличных
-- пространств, файлов табличных пространств, ролей и пользователей.
-- connect system/Qwerty12345@localhost:1521/orcl.mshome.net;
--alter session set current_schema=sys;
select * from dba_tablespaces;
select * from dba_data_files;
select * from dba_roles;
select * from dba_users;

-- 4.	Ознакомьтесь с параметрами в HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE на вашем компьютере.

-- 5.	Запустите утилиту Oracle Net Manager и подготовьте строку подключения с именем имя_вашего_пользователя_SID,
-- где SID – идентификатор подключаемой базы данных.

-- 6.	Подключитесь с помощью sqlplus под собственным пользователем и с применением подготовленной строки подключения.

-- 7.	Выполните select к любой таблице, которой владеет ваш пользователь.
select * from LOGMNG_SESSION_EVOLVE$;--строк там нет, ну и ничего страшного

-- 8.	Ознакомьтесь с командой HELP.Получите справку по команде TIMING. Подсчитайте, сколько времени длится select
-- к любой таблице.
select * from help;
help timing;

-- 9.	Ознакомьтесь с командой DESCRIBE.Получите описание столбцов любой таблицы.

-- 10.	Получите перечень всех сегментов, владельцем которых является ваш пользователь.
SELECT segment_name, segment_type, bytes/1024 AS size_kb, extents, blocks
FROM user_segments;

-- 11.	Создайте представление, в котором получите количество всех сегментов, количество экстентов, блоков памяти и размер в килобайтах, которые они занимают.
CREATE VIEW my_segments_view AS
SELECT COUNT(*) AS segment_count, SUM(extents) AS total_extents, SUM(blocks) AS total_blocks, SUM(bytes)/1024 AS total_size_kb
FROM user_segments;
select * from my_segments_view;