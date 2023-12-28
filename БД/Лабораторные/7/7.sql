SELECT sys_context('USERENV', 'CURRENT_SCHEMA') FROM dual;
SELECT SYS_CONTEXT('USERENV', 'CON_NAME') AS container_name FROM DUAL;
ALTER USER TOV_PDB_ADMIN QUOTA UNLIMITED ON USERS;
GRANT CREATE ANY MATERIALIZED VIEW,
    CREATE ANY TABLE,
    CREATE PROCEDURE,
    ON COMMIT REFRESH,
    CREATE TRIGGER TO TOV_PDB_ADMIN;


-- 2.	�������� ������������������ S1 (SEQUENCE), �� ���������� ����������������: ��������� �������� 1000;
-- ���������� 10; ��� ������������ ��������; ��� ������������� ��������; �� �����������; �������� �� ���������� �
-- ������; ���������� �������� �� �������������.
-- �������� ��������� �������� ������������������.
-- �������� ������� �������� ������������������.
CREATE SEQUENCE S1
START WITH 1000
INCREMENT BY 10
NOCACHE
NOMINVALUE
NOMAXVALUE
NOORDER;
SELECT S1.NEXTVAL FROM DUAL;
SELECT S1.CURRVAL FROM DUAL;
DROP SEQUENCE S1;

-- 3.	�������� ������������������ S2 (SEQUENCE), �� ���������� ����������������: ��������� �������� 10;
-- ���������� 10; ������������ �������� 100; �� �����������.
-- �������� ��� �������� ������������������.
-- ����������� �������� ��������, ��������� �� ������������ ��������.
CREATE SEQUENCE S2
START WITH 10
INCREMENT BY 10
MAXVALUE 100
NOCYCLE;
SELECT S2.NEXTVAL FROM DUAL
CONNECT BY LEVEL <= 10;
-- ORA-08004: ��������. S2.NEXTVAL exceeds MAXVALUE � �� ����� ���� �����������
DROP SEQUENCE S2;

-- 4.	�������� ������������������ S3 (SEQUENCE), �� ���������� ����������������: ��������� �������� 10;
-- ���������� -10; ����������� �������� -100; �� �����������; ������������� ���������� ��������.
-- �������� ��� �������� ������������������.
-- ����������� �������� ��������, ������ ������������ ��������.
CREATE SEQUENCE S3
START WITH 10
INCREMENT BY -10
MAXVALUE 20
MINVALUE -100
NOCYCLE
ORDER;
SELECT S3.NEXTVAL FROM DUAL
CONNECT BY LEVEL <= 12;
-- ORA-08004: ��������. S3.NEXTVAL goes below MINVALUE � �� ����� ���� �����������
DROP SEQUENCE S3;

-- 5.	�������� ������������������ S4 (SEQUENCE), �� ���������� ����������������: ��������� �������� 1;
-- ���������� 1; ����������� �������� 10; �����������; ���������� � ������ 5 ��������; ���������� �������� ��
-- �������������. ����������������� ����������� ��������� �������� ������������������� S4.
CREATE SEQUENCE S4
START WITH 1
INCREMENT BY 1
MAXVALUE 10
CYCLE
CACHE 5
NOORDER;
SELECT S4.NEXTVAL FROM DUAL
CONNECT BY LEVEL <= 15;
DROP SEQUENCE S4;

-- 6.	�������� ������ ���� ������������������� � ������� ���� ������, ���������� ������� �������� ������������ XXX.
SELECT SEQUENCE_NAME, SEQUENCE_OWNER FROM ALL_SEQUENCES WHERE SEQUENCE_OWNER = 'TOV_PDB_ADMIN';

-- 7.	�������� ������� T1, ������� ������� N1, N2, N3, N4, ���� NUMBER (20), ���������� � ������������� �
-- �������� ���� KEEP. � ������� ��������� INSERT �������� 7 �����, �������� �������� ��� �������� ������
-- ������������� � ������� ������������������� S1, S2, S3, S4.
CREATE TABLE T1 (
    N1 NUMBER(20),
    N2 NUMBER(20),
    N3 NUMBER(20),
    N4 NUMBER(20)
) STORAGE(BUFFER_POOL KEEP) CACHE;

declare
    i int;
begin
    for i in 1..7 loop
        INSERT INTO T1 (N1, N2, N3, N4) VALUES (S1.NEXTVAL, S2.NEXTVAL, S3.NEXTVAL, S4.NEXTVAL);
    end loop;
end;
SELECT * FROM T1;

-- 8.	�������� ������� ABC, ������� hash-��� (������ 200) � ���������� 2 ����: X (NUMBER (10)), V (VARCHAR2(12)).
CREATE CLUSTER ABC
(
    X NUMBER(10),
    V VARCHAR2(12)
)
HASHKEYS 200;

-- 9.	�������� ������� A, ������� ������� XA (NUMBER (10)) � VA (VARCHAR2(12)), ������������� �������� ABC, �
-- ����� ��� ���� ������������ �������.
CREATE TABLE A (
    XA NUMBER(10),
    VA VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XA, VA);

-- 10.	�������� ������� B, ������� ������� XB (NUMBER (10)) � VB (VARCHAR2(12)), ������������� �������� ABC, �
-- ����� ��� ���� ������������ �������.
CREATE TABLE B (
    XB NUMBER(10),
    VB VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XB, VB);

-- 11.	�������� ������� �, ������� ������� X� (NUMBER (10)) � V� (VARCHAR2(12)), ������������� �������� ABC, �
-- ����� ��� ���� ������������ �������.
CREATE TABLE C (
    XC NUMBER(10),
    VC VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XC, VC);

-- 12.	������� ��������� ������� � ������� � �������������� ������� Oracle.
select cluster_name, owner from DBA_CLUSTERS where cluster_name = 'ABC';
select * from dba_tables where cluster_name='ABC';

-- 13.	�������� ������� ������� ��� ������� XXX.� � ����������������� ��� ����������.
CREATE SYNONYM SC FOR �;
DROP SYNONYM SC;
SELECT * FROM SC;
-- ORA-00980: ������� ����� �� ����� ��������������� ��������� ?\_(?)_/?
SELECT * FROM C; -- ok
SELECT * FROM DBA_SYNONYMS WHERE SYNONYM_NAME='SC' OR SYNONYM_NAME='SB';

-- 14.	�������� ��������� ������� ��� ������� XXX.B � ����������������� ��� ����������.
CREATE PUBLIC SYNONYM SB FOR B;
SELECT * FROM SB; --ok

-- 15.	�������� ��� ������������ ������� A � B (� ��������� � ������� �������), ��������� �� �������, ��������
-- ������������� V1, ���������� �� SELECT... FOR A inner join B. ����������������� ��� �����������������.
create table A15 (
    X number(20) primary key
);
drop table A15;
create table B15 (
    X number(20) primary key,
    Y number(20), constraint fk_column foreign key (Y) references A15(X)
);
drop table B15;
insert into A15(X) values (1);
insert into A15(X) values (2);
insert into B15(X, Y) values (2, 1);
insert into B15(X, Y) values (1, 2);
create view V1 as select A15.X, B15.Y from A15 inner join B15 on A15.X=B15.Y;
select * from V1;
drop view V1;

-- 16.	�� ������ ������ A � B �������� ����������������� ������������� MV, ������� ����� ������������� ����������
-- 2 ������. ����������������� ��� �����������������.
select * from A15;
select * from B15;

create materialized view log on A15 with primary key, rowid, sequence including new values;
create materialized view log on B15 with primary key, rowid, sequence including new values;

create materialized view MV
    build immediate
    refresh complete
    on demand
    next sysdate + numtodsinterval(2, 'MINUTE')
    as (select A15.X as AX, B15.X as BX from A15 inner join B15 on A15.X=B15.Y);
alter materialized view MV compile;
select A15.X as AX, B15.X as BX from A15 inner join B15 on A15.X=B15.Y;


BEGIN
  -- Create the materialized view
  DBMS_SCHEDULER.CREATE_JOB(
    job_name        => 'REFRESH_MV',
    job_type        => 'PLSQL_BLOCK',
    job_action      => 'BEGIN ' ||
                       'DBMS_MVIEW.REFRESH(''MV'', ''C''); ' ||
                       'insert into auxil values(systimestamp); ' ||
                       'commit;' ||
                       'END;',
    start_date      => systimestamp,
    repeat_interval => 'FREQ=MINUTELY;INTERVAL=2',
    enabled         => TRUE
  );
  COMMIT;
END;

begin
    DBMS_SCHEDULER.Drop_Job('REFRESH_MV');
end;

begin
    dbms_mview.refresh('MV', 'C');
    insert into auxil values(systimestamp);
end;

create table auxil (xd timestamp);
drop table auxil;
select * from auxil;
delete from auxil;

select * from MV;
select * from A15;
insert into A15(X) values (8);
insert into B15(X, Y) values (8, 8);
delete from A15 where X = 8;
delete from B15 where Y = 8;

SELECT * FROM DBA_SCHEDULER_JOBS;
SELECT * FROM DBA_SCHEDULER_JOB_LOG;
select * from dba_mviews;
select * from dba_mview_logs;
ALTER MATERIALIZED VIEW MV
   REFRESH FORCE ON DEMAND NEXT sysdate + interval '5' second;


drop materialized view MV;