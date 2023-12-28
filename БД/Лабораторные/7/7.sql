SELECT sys_context('USERENV', 'CURRENT_SCHEMA') FROM dual;
SELECT SYS_CONTEXT('USERENV', 'CON_NAME') AS container_name FROM DUAL;
ALTER USER TOV_PDB_ADMIN QUOTA UNLIMITED ON USERS;
GRANT CREATE ANY MATERIALIZED VIEW,
    CREATE ANY TABLE,
    CREATE PROCEDURE,
    ON COMMIT REFRESH,
    CREATE TRIGGER TO TOV_PDB_ADMIN;


-- 2.	Создайте последовательность S1 (SEQUENCE), со следующими характеристиками: начальное значение 1000;
-- приращение 10; нет минимального значения; нет максимального значения; не циклическая; значения не кэшируются в
-- памяти; хронология значений не гарантируется.
-- Получите несколько значений последовательности.
-- Получите текущее значение последовательности.
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

-- 3.	Создайте последовательность S2 (SEQUENCE), со следующими характеристиками: начальное значение 10;
-- приращение 10; максимальное значение 100; не циклическую.
-- Получите все значения последовательности.
-- Попытайтесь получить значение, выходящее за максимальное значение.
CREATE SEQUENCE S2
START WITH 10
INCREMENT BY 10
MAXVALUE 100
NOCYCLE;
SELECT S2.NEXTVAL FROM DUAL
CONNECT BY LEVEL <= 10;
-- ORA-08004: последов. S2.NEXTVAL exceeds MAXVALUE и не может быть установлено
DROP SEQUENCE S2;

-- 4.	Создайте последовательность S3 (SEQUENCE), со следующими характеристиками: начальное значение 10;
-- приращение -10; минимальное значение -100; не циклическую; гарантирующую хронологию значений.
-- Получите все значения последовательности.
-- Попытайтесь получить значение, меньше минимального значения.
CREATE SEQUENCE S3
START WITH 10
INCREMENT BY -10
MAXVALUE 20
MINVALUE -100
NOCYCLE
ORDER;
SELECT S3.NEXTVAL FROM DUAL
CONNECT BY LEVEL <= 12;
-- ORA-08004: последов. S3.NEXTVAL goes below MINVALUE и не может быть установлено
DROP SEQUENCE S3;

-- 5.	Создайте последовательность S4 (SEQUENCE), со следующими характеристиками: начальное значение 1;
-- приращение 1; минимальное значение 10; циклическая; кэшируется в памяти 5 значений; хронология значений не
-- гарантируется. Продемонстрируйте цикличность генерации значений последовательностью S4.
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

-- 6.	Получите список всех последовательностей в словаре базы данных, владельцем которых является пользователь XXX.
SELECT SEQUENCE_NAME, SEQUENCE_OWNER FROM ALL_SEQUENCES WHERE SEQUENCE_OWNER = 'TOV_PDB_ADMIN';

-- 7.	Создайте таблицу T1, имеющую столбцы N1, N2, N3, N4, типа NUMBER (20), кэшируемую и расположенную в
-- буферном пуле KEEP. С помощью оператора INSERT добавьте 7 строк, вводимое значение для столбцов должно
-- формироваться с помощью последовательностей S1, S2, S3, S4.
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

-- 8.	Создайте кластер ABC, имеющий hash-тип (размер 200) и содержащий 2 поля: X (NUMBER (10)), V (VARCHAR2(12)).
CREATE CLUSTER ABC
(
    X NUMBER(10),
    V VARCHAR2(12)
)
HASHKEYS 200;

-- 9.	Создайте таблицу A, имеющую столбцы XA (NUMBER (10)) и VA (VARCHAR2(12)), принадлежащие кластеру ABC, а
-- также еще один произвольный столбец.
CREATE TABLE A (
    XA NUMBER(10),
    VA VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XA, VA);

-- 10.	Создайте таблицу B, имеющую столбцы XB (NUMBER (10)) и VB (VARCHAR2(12)), принадлежащие кластеру ABC, а
-- также еще один произвольный столбец.
CREATE TABLE B (
    XB NUMBER(10),
    VB VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XB, VB);

-- 11.	Создайте таблицу С, имеющую столбцы XС (NUMBER (10)) и VС (VARCHAR2(12)), принадлежащие кластеру ABC, а
-- также еще один произвольный столбец.
CREATE TABLE C (
    XC NUMBER(10),
    VC VARCHAR2(12),
    Y NUMBER(10)
) CLUSTER ABC(XC, VC);

-- 12.	Найдите созданные таблицы и кластер в представлениях словаря Oracle.
select cluster_name, owner from DBA_CLUSTERS where cluster_name = 'ABC';
select * from dba_tables where cluster_name='ABC';

-- 13.	Создайте частный синоним для таблицы XXX.С и продемонстрируйте его применение.
CREATE SYNONYM SC FOR С;
DROP SYNONYM SC;
SELECT * FROM SC;
-- ORA-00980: синоним далее не будет транслироваться правильно ?\_(?)_/?
SELECT * FROM C; -- ok
SELECT * FROM DBA_SYNONYMS WHERE SYNONYM_NAME='SC' OR SYNONYM_NAME='SB';

-- 14.	Создайте публичный синоним для таблицы XXX.B и продемонстрируйте его применение.
CREATE PUBLIC SYNONYM SB FOR B;
SELECT * FROM SB; --ok

-- 15.	Создайте две произвольные таблицы A и B (с первичным и внешним ключами), заполните их данными, создайте
-- представление V1, основанное на SELECT... FOR A inner join B. Продемонстрируйте его работоспособность.
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

-- 16.	На основе таблиц A и B создайте материализованное представление MV, которое имеет периодичность обновления
-- 2 минуты. Продемонстрируйте его работоспособность.
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