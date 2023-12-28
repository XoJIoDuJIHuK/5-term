--1. Созд табл с неск атрибутами, 1 - ПК
    CREATE TABLE tabl(a int primary key,b varchar(30));


--2. Заполн 10 строками
    DECLARE 
        i int :=0;
    BEGIN
        WHILE i<10
        LOOP
            INSERT INTO tabl(a,b)
            values (i,'a');
            i:= i+1;
        END LOOP;
    END;

    SELECT * FROM tabl;
    

--3. Before-триг ур.оператора на i.d.u.
    CREATE OR REPLACE TRIGGER Input_trigger_before
    before insert on tabl
    BEGIN dbms_output.put_line('Insert trigger before activate'); END;
    insert into tabl values (25,'xd');
    
    
    CREATE OR REPLACE TRIGGER Delete_trigger_before
    before delete on tabl
    BEGIN dbms_output.put_line('Delete trigger before  activate'); END;
    delete tabl where a=25;
    
    
    CREATE OR REPLACE TRIGGER Update_trigger_before
    before update on tabl
    BEGIN dbms_output.put_line('Update trigger before  activate'); END;
    update tabl set a=101 where a=3;
    
       
--5. ... ур. строки 
    CREATE OR REPLACE TRIGGER Input_for_each_trigger_before
    before insert on tabl
    for each row
    BEGIN dbms_output.put_line('Input_for_each_trigger before activate'); END;

    CREATE OR REPLACE TRIGGER Update_for_each_trigger_before
    before update on tabl
    for each row
    BEGIN dbms_output.put_line('Update_for_each_trigger before activate'); END;

    CREATE OR REPLACE TRIGGER Delete_for_each_trigger_before
    before delete on tabl
    for each row
    BEGIN dbms_output.put_line('Delete_for_each_trigger before activate'); END;
    
    
--6. Примените предикаты
    CREATE OR REPLACE TRIGGER Trigger_ing
    after insert or update or delete on tabl
    BEGIN
    IF INSERTING then
        dbms_output.put_line('Inserting after');
    ELSIF UPDATING then
        dbms_output.put_line('Updating after');
    ELSIF DELETING then
        dbms_output.put_line('Deleting after');
    END IF;
    END;    
    
    
--7. After-триггеры ур.оператора на i.d.u. 
    CREATE OR REPLACE TRIGGER Input_trigger
    after insert on tabl
    BEGIN dbms_output.put_line('Insert trigger after activate'); END;

    CREATE OR REPLACE TRIGGER Delete_trigger
    after delete on tabl
    BEGIN dbms_output.put_line('Delete trigger after  activate'); END;

    CREATE OR REPLACE TRIGGER Update_trigger
    after update on tabl
    BEGIN dbms_output.put_line('Update trigger after  activate'); END;


--8. ... ур. строки
    CREATE OR REPLACE TRIGGER Input_for_each_trigger
    after insert on tabl
    for each row
    BEGIN dbms_output.put_line('Input_for_each_trigger after activate'); END;
    
    CREATE OR REPLACE TRIGGER Update_for_each_trigger
    after update on tabl
    for each row
    BEGIN dbms_output.put_line('Update_for_each_trigger after activate'); END;

    CREATE OR REPLACE TRIGGER Delete_for_each_trigger
    after delete on tabl
    for each row
    BEGIN dbms_output.put_line('Delete_for_each_trigger after activate'); END;
    

--9. Созд табл audit
    create table AUDITS(
        OperationDate date,         --('DD-MM-YYYY  HH24:MI:SS'),
        OperationType varchar2(40), --i.d.u.
        TriggerName varchar2(40),
        Data varchar2(40)           --знач полей до и после операции
        );
        

--10. Измен триггеры: регистр операции с tabl в audit
    CREATE OR REPLACE TRIGGER AUDITS_trigger_before
    before insert or update  or delete on tabl
    BEGIN
        if inserting then
            dbms_output.put_line('before_insert_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Insert', 'AUDITS_trigger_before',concat(tabl.a ,tabl.b)
            FROM tabl;
        elsif updating then
            dbms_output.put_line('before_update_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Update', 'AUDITS_trigger_before',concat(tabl.a ,tabl.b)
            FROM tabl;
        elsif deleting then
            dbms_output.put_line('before_delete_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Delete', 'AUDITS_trigger_before',concat(a ,b)
            FROM tabl;
        END if;
    END;
    -------------------------------
    CREATE OR REPLACE TRIGGER AUDITS_trigger_after
    after insert or update  or delete on tabl
    BEGIN
        if inserting then
            dbms_output.put_line('after_insert_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Insert', 'AUDITS_trigger_after',concat(tabl.a ,tabl.b)
            FROM tabl;
        elsif updating then
            dbms_output.put_line('after_update_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Update', 'AUDITS_trigger_after',concat(tabl.a ,tabl.b)
            FROM tabl;
        elsif deleting then
            dbms_output.put_line('after_delete_AUDITS');
            INSERT INTO AUDITS(OperationDate, OperationType, TriggerName, Data)
            SELECT sysdate,'Delete', 'AUDITS_trigger_after',concat(a ,b)
            FROM tabl;
        END if;
    END;

------------------------------------------------
SELECT * from tabl;
SELECT * from audits;
UPDATE tabl SET b = 'B';
DELETE tabl;
delete AUDITS;

-- 11. нарушение целостности
insert into tabl (a, b) VALUES (0, 'xd');
-- before_insert_AUDITS
-- Insert trigger before activate
-- Input_for_each_trigger before activate
    
select object_name, status from dba_objects where object_type='TRIGGER';
---------------------------------------------

--12. Удад tabl, объясн рез
drop table tabl;
select * from DBA_TABLES where table_name = 'TABL';
FLASHBACK table tabl TO BEFORE DROP;
create or replace trigger INSTEAD_OF_DROP
    before drop on TOV_PDB_ADMIN.SCHEMA
BEGIN
    RAISE_APPLICATION_ERROR(-20001, 'Dropping tables is not allowed.');
END;
    
--13. Удал audit, посмотр сост триггеров
drop table audits;
FLASHBACK table audits TO BEFORE DROP;
select object_name, status from dba_objects where object_type='TRIGGER';
--.. доб триггер, запрещ. удал tabl
    CREATE OR REPLACE TRIGGER no_drop_trg
    BEFORE DROP ON TOV_PDB_ADMIN.SCHEMA
    BEGIN
        IF DICTIONARY_OBJ_NAME = 'TABL'
        THEN
        RAISE_APPLICATION_ERROR (-20905, 'Нельзя удалить TABL!!!');
        END IF;
    END; 
---------------------------------------------

--14. Созд предст над tabl, insteadof insert триггер
    create view tablview as SELECT * FROM tabl;
    
    CREATE OR REPLACE TRIGGER tabl_trigg
    instead of insert on tablview
    BEGIN
        if inserting then
            dbms_output.put_line('insert');
            insert into tabl VALUES (102, 'bye');
        end if;
    END tabl_trigg;
    
    INSERT INTO tablview (a,b) values(12,'c');
    SELECT * FROM tablview;

-- 15 Продемонстрировать порядок
-- instead of
-- before (compound) operator
-- before operator
-- before row
-- after row
-- after (compound) operator
-- after operator
-- after compound (?xd)