-- 9. В созданный файл MyStart скрипт, создающий таблицу с именем XXX_t
create table TOV_T(
    x number(3) primary key,
    s varchar2(50)
);
-- 10. Найдите в папке Tables, созданную таблицу XXX_t.
-- 11. Дополните скрипт операторами INSERT, добавляющими 3 строки в 
-- таблицу XXX_t. Выполните операторы INSERT. Выполните оператор COMMIT
insert into TOV_T (x, s) values (1, 'first');
insert into TOV_T (x, s) values (2, 'second');
insert into TOV_T (x, s) values (3, 'third');
select * from TOV_T
commit;
-- 12. Дополните скрипт оператором UPDATЕ, изменяющим 2 строки в 
-- таблице XXX_t. Выполните оператор UPDATЕ. Выполните оператор COMMIT
update TOV_T set x=x+10 where x in (1, 2);
commit;
-- 13. Дополните скрипт операторами SELECT (выборка по условию, 
-- применение агрегатных функций);
select sum(x) from TOV_T;
select * from TOV_T where x < 10;
-- 14. Дополните скрипт оператором DELELE, удаляющим 1 строку в таблице
-- XXX_t. Выполните оператор DELETE. Выполните оператор COMMIT;
delete from TOV_T where x = 3;
-- 15. Создайте таблицу XXX_t1, связанную отношением внешнего ключа с 
-- таблицей XXX_t. Добавьте данные в таблицу XXX_t1.
create table TOV_T1 (
    y number(3) primary key,
    x number(3) references TOV_T(x)
);
insert into TOV_T1 values (10, 11);
insert into TOV_T1 values (20, 12);
select * from TOV_T1;
-- 16. Дополните скрипт операторами SELECT из обеих таблиц (левое и правое 
-- соединение, внутреннее соединение);
select t.x, t.s, t1.y from TOV_T t left join TOV_T1 t1 on t.x = t1.x;
select t.x, t.s, t1.y from TOV_T t right join TOV_T1 t1 on t.x = t1.x;
select t.x, t.s, t1.y from TOV_T t inner join TOV_T1 t1 on t.x = t1.x;
-- 18. Дополните скрипт оператором DROP, удаляющим таблицы XXX_t, XXX_t1
drop table TOV_T1;
drop table TOV_T;