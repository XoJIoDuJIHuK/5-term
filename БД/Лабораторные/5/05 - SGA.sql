--1. определить общий размер область SGA
select sum(value) from v$sga;

--2.определить текущие размеры основных пулов SGA
select component, current_size from v$sga_dynamic_components where current_size > 0;

--3.определить гранулы для каждого пула
select component, granule_size from v$sga_dynamic_components where current_size > 0;

--4.определить объем доступной свободной памяти в SGA
select current_size from v$sga_dynamic_free_memory;

--5.Определите максимальный и целевой размер области SGA.
--show parameter sga_max_size;--1648M
--show parameter sga_target;--0
--работает только в sqlplus

--6.определить размеры пулов KEEP, DEFAULT, RECYCLE буферного кэша
select component, current_size, min_size from v$sga_dynamic_components where component ='KEEP buffer cache' or component = 'DEFAULT buffer cache' or component = 'RECYCLE buffer cache';

--7.создайте таблицу, которая будет помешаться в пул KEEP. продемонстрируйте сегмент таблицы
create table Mytable(x int) storage(buffer_pool keep);
select segment_name, segment_type, tablespace_name, buffer_pool from user_segments where segment_name='MYTABLE';

--8.создайте таблицу, которая будет кэшироваться в пуле default. Продемонстрировать семент таблицы
create table Mytable2(x int) cache storage(buffer_pool default);
select segment_name, segment_type, tablespace_name, buffer_pool from user_segments where segment_name='MYTABLE2';

--9.найдите размер буфера журналов повтора
--show parameter log_buffer;--7008K
--тоже только в sqlplus

--10. найдите размер свободной памяти в большом пуле
select pool, name, bytes from v$sgastat where pool = 'large pool' and name = 'free memory';

--11.Определите режимы текущих соединений с инстансом (dedicated, shared).
select sid, username, server from v$session;

--12.Получите полный список работающих в настоящее время фоновых процессов
select name, description from v$bgprocess;

--13.Получите список работающих в настоящее время серверных процессов.
SELECT * FROM v$process;

--14.	Определите, сколько процессов DBWn работает в настоящий момент.
--show parameter db_writer_processes;
--1, тоже sqlplus

--15.	Определите сервисы (точки подключения экземпляра).
SELECT name, network_name, pdb FROM v$services;
--sys$users, orcl.mshome.net

--16.	Получите известные вам параметры диспетчеров.
SELECT * FROM V$DISPATCHER;
--show parameter DISPATCHERS;
--dispatchers (string, protocol=tcp, service=orclXDB), max_dispatchers (integer, пустое значение)

--17.	Укажите в списке Windows-сервисов сервис, реализующий процесс LISTENER.
--oracledb.mshome.net, в listener.ora
--в проводнике OracleOraDB19Home1TNSListener

--18.	Продемонстрируйте и поясните содержимое файла LISTENER.ORA.
--SID_LIST_LISTENER: Этот параметр определяет статические сервисы (SID) для обслуживания внешних подключений.
--Он позволяет подключаться к базе данных, даже если она не запущена или находится в состоянии NOMOUNT
--LISTENER: указывает на конфигурацию истенера, включая адреса прослушивания

--19.	Запустите утилиту lsnrctl и поясните ее основные команды.
--start: Initiates the listener
--stop: Stops the listener
--services: Displays a list of registered services (SID) that the listener is aware of
--status: Shows the current status of the listener
--reload: Reloads the listener configuration without stopping it

--20.	Получите список служб инстанса, обслуживаемых процессом LISTENER
--lsnrctl services