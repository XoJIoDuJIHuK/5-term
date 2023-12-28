--1. простейший АБ без операторов
    begin
        null;
    end;

--2. АБ, вывод 'hello world'
    begin
        dbms_output.put_line('Hello, world');
    end;

--3. Продемонстр исключ SQLERRM и встр.ф SQLCODE
    declare
        x number(3) := 3;
        y number(3) := 0;
        z number (10,2);
    begin
        z:=x/y;
        exception when others
           then dbms_output.put_line(sqlcode||': error = '||sqlerrm);
    end;

--4. Влож блок, принцип исключений    
    declare
        x number(3) := 3;
    begin
        begin
            declare x number(3) :=1;
            begin dbms_output.put_line('x = '||x); end;
        end;
        dbms_output.put_line('x = '||x);
    end;

--5. Какие типы предупрежд компилятора поддерж в д-й момент?
    alter system set plsql_warnings = 'ENABLE:INFORMATIONAL';
    select name, value from v$parameter where name = 'plsql_warnings';
    
--6. Просмотр спецсимволы PL/SQL
    select keyword from v$reserved_words
        where length = 1 and keyword != 'A';

--7. Просмотр ключевые слова PL/SQL
    select keyword from v$reserved_words
        where length > 1 and keyword!='A' order by keyword;
    
--8. Просмотр параметры Oracle/Server связ с PL/SQL (sql+ show)
    select name,value from v$parameter
        where name like 'plsql%';

--9-17. АБ, демонстр:
    declare
        c1 number(3)        := 25;
        c2 number(3)        := 10;
        div number(10,2);
        fix number(10,2)    := 3.12;
        otr number(4, -5)   := 32.12345;
        en number(32,10)    := 12345E-10;
        bf binary_float     := 123456789.12345678911;
        bd binary_double    := 123456789.12345678911;
        b1 boolean          := true;
    
    begin
        div := mod(c1,c2);
        dbms_output.put_line('c1 = '||c1);
        dbms_output.put_line('c2 = '||c2);
        dbms_output.put_line('c1%c2 = '||div);
        
        dbms_output.put_line('fix = '||fix);
        dbms_output.put_line('otr = '||otr);
        dbms_output.put_line('en = '||en);
        dbms_output.put_line('bf = '||bf);
        dbms_output.put_line('bd = '||bd);
        if b1 then dbms_output.put_line('b1 = '||'true'); end if;
    end;
    
--18. АБ с объявл констант
    declare
        n1 constant number(5) := 5;
        vc constant varchar(25) := 'Hello world';
        c constant char(7) := 'Julia';  
    begin
        dbms_output.put_line('vc = '||vc);
        dbms_output.put_line('n1 = '||n1);
        dbms_output.put_line('c = '||c);
        
        n1:=10;
        exception when others
            then dbms_output.put_line('error = '||n1);
    end;
    
--19-20. объяв опцией %TYPE, %ROWTYPE
    declare
        name varchar(25) := 'Smith';
        surname name%TYPE := 'Jones';
        x  dual%ROWTYPE;
    begin
        select 'J' into x from dual;
        dbms_output.put_line('name = '||name);
        dbms_output.put_line(x.dummy);
    end;
    
--21. Конструкции IF, CASE
    declare
        x pls_integer := 17;
    begin
        if 8>x then dbms_output.put_line('8>'||x);
        elsif 8=x then dbms_output.put_line('8='||x);
        else dbms_output.put_line('8<'||x);
        end if;
    end;

--23. Оператор CASE
    declare
        x pls_integer := 17;
    begin
        case x
            when 17 then dbms_output.put_line('17');
        end case;
        case
            when 8>x then dbms_output.put_line('8>'||x);
            when x between 13 and 20 then dbms_output.put_line('yes');
            else dbms_output.put_line('else');
        end case;
    end;
    
--24-26. Работа LOOP, WHILE, FOR
    declare
        x pls_integer :=0;
    begin
        loop x:=x+1;
             dbms_output.put(x);
            exit when x>5;
            end loop;
        for k in 1..5
            loop dbms_output.put_line(k); end loop;
        while (x>0)
            loop x:=x-1;
            dbms_output.put_line(x);
            end loop;
    end;





