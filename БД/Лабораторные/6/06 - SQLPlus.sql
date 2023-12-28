-- 1.	������� �� ���������� ���������������� ����� SQLNET.ORA � TNSNAMES.ORA � ������������ � �� ����������.

-- 2.	����������� ��� ������ sqlplus � Oracle ��� ������������ SYSTEM, �������� �������� ���������� ����������
-- Oracle.
--system Qwerty12345
-- show parameter;

-- 3.	����������� ��� ������ sqlplus � ������������ ����� ������ ��� ������������ SYSTEM, �������� ������ ���������
-- �����������, ������ ��������� �����������, ����� � �������������.
-- connect system/Qwerty12345@localhost:1521/orcl.mshome.net;
--alter session set current_schema=sys;
select * from dba_tablespaces;
select * from dba_data_files;
select * from dba_roles;
select * from dba_users;

-- 4.	������������ � ����������� � HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE �� ����� ����������.

-- 5.	��������� ������� Oracle Net Manager � ����������� ������ ����������� � ������ ���_������_������������_SID,
-- ��� SID � ������������� ������������ ���� ������.

-- 6.	������������ � ������� sqlplus ��� ����������� ������������� � � ����������� �������������� ������ �����������.

-- 7.	��������� select � ����� �������, ������� ������� ��� ������������.
select * from LOGMNG_SESSION_EVOLVE$;--����� ��� ���, �� � ������ ���������

-- 8.	������������ � �������� HELP.�������� ������� �� ������� TIMING. �����������, ������� ������� ������ select
-- � ����� �������.
select * from help;
help timing;

-- 9.	������������ � �������� DESCRIBE.�������� �������� �������� ����� �������.

-- 10.	�������� �������� ���� ���������, ���������� ������� �������� ��� ������������.
SELECT segment_name, segment_type, bytes/1024 AS size_kb, extents, blocks
FROM user_segments;

-- 11.	�������� �������������, � ������� �������� ���������� ���� ���������, ���������� ���������, ������ ������ � ������ � ����������, ������� ��� ��������.
CREATE VIEW my_segments_view AS
SELECT COUNT(*) AS segment_count, SUM(extents) AS total_extents, SUM(blocks) AS total_blocks, SUM(bytes)/1024 AS total_size_kb
FROM user_segments;
select * from my_segments_view;