--1. ���������� ����� ������ ������� SGA
select sum(value) from v$sga;

--2.���������� ������� ������� �������� ����� SGA
select component, current_size from v$sga_dynamic_components where current_size > 0;

--3.���������� ������� ��� ������� ����
select component, granule_size from v$sga_dynamic_components where current_size > 0;

--4.���������� ����� ��������� ��������� ������ � SGA
select current_size from v$sga_dynamic_free_memory;

--5.���������� ������������ � ������� ������ ������� SGA.
--show parameter sga_max_size;--1648M
--show parameter sga_target;--0
--�������� ������ � sqlplus

--6.���������� ������� ����� KEEP, DEFAULT, RECYCLE ��������� ����
select component, current_size, min_size from v$sga_dynamic_components where component ='KEEP buffer cache' or component = 'DEFAULT buffer cache' or component = 'RECYCLE buffer cache';

--7.�������� �������, ������� ����� ���������� � ��� KEEP. ����������������� ������� �������
create table Mytable(x int) storage(buffer_pool keep);
select segment_name, segment_type, tablespace_name, buffer_pool from user_segments where segment_name='MYTABLE';

--8.�������� �������, ������� ����� ������������ � ���� default. ������������������ ������ �������
create table Mytable2(x int) cache storage(buffer_pool default);
select segment_name, segment_type, tablespace_name, buffer_pool from user_segments where segment_name='MYTABLE2';

--9.������� ������ ������ �������� �������
--show parameter log_buffer;--7008K
--���� ������ � sqlplus

--10. ������� ������ ��������� ������ � ������� ����
select pool, name, bytes from v$sgastat where pool = 'large pool' and name = 'free memory';

--11.���������� ������ ������� ���������� � ��������� (dedicated, shared).
select sid, username, server from v$session;

--12.�������� ������ ������ ���������� � ��������� ����� ������� ���������
select name, description from v$bgprocess;

--13.�������� ������ ���������� � ��������� ����� ��������� ���������.
SELECT * FROM v$process;

--14.	����������, ������� ��������� DBWn �������� � ��������� ������.
--show parameter db_writer_processes;
--1, ���� sqlplus

--15.	���������� ������� (����� ����������� ����������).
SELECT name, network_name, pdb FROM v$services;
--sys$users, orcl.mshome.net

--16.	�������� ��������� ��� ��������� �����������.
SELECT * FROM V$DISPATCHER;
--show parameter DISPATCHERS;
--dispatchers (string, protocol=tcp, service=orclXDB), max_dispatchers (integer, ������ ��������)

--17.	������� � ������ Windows-�������� ������, ����������� ������� LISTENER.
--oracledb.mshome.net, � listener.ora
--� ���������� OracleOraDB19Home1TNSListener

--18.	����������������� � �������� ���������� ����� LISTENER.ORA.
--SID_LIST_LISTENER: ���� �������� ���������� ����������� ������� (SID) ��� ������������ ������� �����������.
--�� ��������� ������������ � ���� ������, ���� ���� ��� �� �������� ��� ��������� � ��������� NOMOUNT
--LISTENER: ��������� �� ������������ ��������, ������� ������ �������������

--19.	��������� ������� lsnrctl � �������� �� �������� �������.
--start: Initiates the listener
--stop: Stops the listener
--services: Displays a list of registered services (SID) that the listener is aware of
--status: Shows the current status of the listener
--reload: Reloads the listener configuration without stopping it

--20.	�������� ������ ����� ��������, ������������� ��������� LISTENER
--lsnrctl services