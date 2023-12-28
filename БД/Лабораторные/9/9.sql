--1. select с точной выборкой (1 строка!)
    declare
        fac faculty%rowtype;
    begin
        select * into fac from faculty where faculty='ФИТ' and ROWNUM = 1;
        dbms_output.put_line(fac.faculty||' '||fac.faculty_name);
    end;
    
--2-4. select с неточной выборкой + When others
    declare
        fac faculty%rowtype;
    begin
        --select * into fac from faculty;     --кол-во строк > запрош
        select * into fac from faculty where faculty='XXX';
    exception
        when no_data_found
        then dbms_output.put_line('Данные не найдены');
        when too_many_rows
        then dbms_output.put_line('В результате несколько строк');
        when others
        then dbms_output.put_line(sqlerrm);
    end;


select * from auditorium;
--5-10. Оператор UPDATE + commit/rollback + искл
    begin
        --update auditorium set auditorium = '207-1' where auditorium = '206-1';
        --insert into auditorium values ('301-9', '301-1', 90, 'ЛК');
        delete from auditorium where auditorium = '301-8';
        if sql%found then dbms_output.put('found '); end if;
        if sql%isopen then dbms_output.put('opened '); end if;
        if sql%notfound then dbms_output.put('not found '); end if;
        dbms_output.put_line('Измененных строк: '||sql%rowcount);
        commit;
    exception
        when others then dbms_output.put_line('error = '||sqlerrm);
        rollback;
    end;
    
    ------------------------------
     declare
        sub auditorium%rowtype;
     begin
        --update auditorium set auditorium_capacity='Z' where auditorium='301-1';
        --insert into auditorium values ('x', 'x', 'x', 'x');
        delete from auditorium where auditorium_capacity='x';
        select * into sub from auditorium where auditorium_name='301-1';
        commit;
    exception
        when others then dbms_output.put_line(sqlerrm);
        rollback;
    end;

---------------- ЯВНЫЕ КУРСОРЫ ---------------------
select * from TEACHER;

--11. Распеч TEACHER с курсором LOOP-цикла + счит д-е в %TYPE
    declare
        cursor cur is select teacher_name, pulpit from TEACHER;
        m_name      teacher.teacher_name%type;
        m_pulpit    teacher.pulpit%type;
    begin
        open cur;
        dbms_output.put_line('rowcount = '||cur%rowcount);
        loop
            fetch cur into m_name, m_pulpit;
            exit when cur%notfound;
            dbms_output.put_line(cur%rowcount||' '||m_name||' '||
                                 m_pulpit);
        end loop;
        dbms_output.put_line('rowcount = '||cur%rowcount);
        close cur;
    exception
        when others then dbms_output.put_line(sqlerrm);
    end;


select * from subject;
--12. Распеч SUBJECT с курсором while-цикла + счит д-е в RECORD(%ROWTYPE)
    declare
        cursor cur is select subject, subject_name, pulpit from SUBJECT;
        rec subject%rowtype;
    begin
        open cur;
        dbms_output.put_line('rowcount = '||cur%rowcount);
        fetch cur into rec;
        while cur%found
            loop
            dbms_output.put_line(cur%rowcount||' '||rec.subject||' '||
                                 rec.subject_name||' '||rec.pulpit);
            fetch cur into rec;
            end loop;
        dbms_output.put_line('rowcount = '||cur%rowcount);
        close cur;
    end;


--13. Расп все кафедры (PULPIT) и фамилии (TEACHER) с JOIN + курсор + for
    declare
        cursor cur
            is select pulpit.pulpit, teacher.teacher_name
            from pulpit inner join teacher on pulpit.pulpit=teacher.pulpit;
        rec cur%rowtype;
    begin
        for rec in cur
        loop
            dbms_output.put_line(cur%rowcount||' '||rec.teacher_name||' '||rec.pulpit);
        end loop;
    end;


--14. Расп все ауд с вместимостью <20, 21-30, 31-60, 61-80, >81
declare
    cursor cur_first(cap1 auditorium.AUDITORIUM_CAPACITY%type,cap2 auditorium.AUDITORIUM_CAPACITY%type)
        is select auditorium, auditorium_capacity
           from auditorium
           where auditorium_capacity >=cap1 and AUDITORIUM_CAPACITY <= cap2;
    cursor cur_second is select auditorium, auditorium_capacity
           from auditorium
           where auditorium_capacity >=21 and AUDITORIUM_CAPACITY <= 30;
    cursor cur_third is select auditorium
           from auditorium
           where auditorium_capacity >=31 and AUDITORIUM_CAPACITY <= 60;
    cursor_result cur_first%rowtype;
    char_cursor_result char(10);
    cursor cur_fourth is select auditorium
           from auditorium
           where auditorium_capacity >=61 and AUDITORIUM_CAPACITY <= 80;
    cursor cur_fifth is select auditorium
           from auditorium
           where auditorium_capacity >=81;
begin
    dbms_output.put_line('Вместимость <20 :');
    for aum in cur_first(0, 20)
        loop dbms_output.put(aum.auditorium||' ');
    end loop;

    dbms_output.put_line(chr(10)||'Вместимость 21-30 :');
    open cur_second;
    fetch cur_second into cursor_result;
    while cur_second%found loop
        dbms_output.put(cursor_result.auditorium||' ');
        fetch cur_second into cursor_result;
    end loop;

    dbms_output.put_line(chr(10)||'Вместимость 31-60 :');
    open cur_third;
    loop
        fetch cur_third into char_cursor_result;
        exit when cur_third%notfound;
        dbms_output.put(char_cursor_result||' ');
    end loop;
    close cur_third;

    dbms_output.put_line(chr(10)||'Вместимость 61-80 :');
    open cur_fourth;
    loop
        fetch cur_fourth into char_cursor_result;
        exit when cur_fourth%notfound;
        dbms_output.put(char_cursor_result||' ');
    end loop;
    close cur_fourth;

    dbms_output.put_line(chr(10)||'Вместимость выше 80 :');
    open cur_fifth;
    loop
        fetch cur_fifth into char_cursor_result;
        exit when cur_fifth%notfound;
        dbms_output.put(char_cursor_result||' ');
    end loop;
    close cur_fifth;
    dbms_output.put_line('');
end;

--15. Курсор.пер refcursor
    declare 
      type teacher_name is ref cursor return teacher%rowtype;
      xcurs teacher_name;
      rec_teacher teacher%rowtype;
    begin
      open xcurs for select * from teacher;
      fetch xcurs into rec_teacher;
      while xcurs%found loop
          dbms_output.put_line(rec_teacher.TEACHER_NAME||' '||rec_teacher.PULPIT||' ');
          fetch xcurs into rec_teacher;
      end loop;
    end;

  
--16. Курсорный подзапрос
    declare 
        cursor curs_aut is select auditorium_type,
              cursor (select auditorium from auditorium aum
                      where aut.auditorium_type = aum.auditorium_type)
                      from auditorium_type aut;
        curs_aum sys_refcursor;
        aut auditorium_type.auditorium_type%type;
        aum auditorium.auditorium%type;
    begin
      open curs_aut;
      fetch curs_aut into aut, curs_aum;
      while(curs_aut%found)
      loop
        loop
          fetch curs_aum into aum;
          dbms_output.put_line(aum);
          exit when curs_aum%notfound;
        end loop;
        dbms_output.new_line;
        fetch curs_aut into aut, curs_aum;
      end loop;
      close curs_aut;
    end;


--17. Уменьш вместимость 40-18 на 10%
    declare 
      cursor cur(cap1 auditorium.auditorium%type, cap2 auditorium.auditorium%type)
            is select auditorium, auditorium_capacity from auditorium
            where auditorium_capacity between cap1 and cap2 for update;
      aum auditorium.auditorium%type;
      cap auditorium.auditorium_capacity%type;
    begin
      open cur(40,80);
      fetch cur into aum, cap;
      while(cur%found)
      loop
          cap := cap * 0.9;
          update auditorium
            set auditorium_capacity = cap
            where current of cur;
          dbms_output.put_line(' '||aum||' '||cap);
          fetch cur into aum, cap;
      end loop;
      close cur; rollback;
   end;


--18. Удалить все ауд cap<20
    declare
      cursor cur(cap1 auditorium.auditorium%type,cap2 auditorium.auditorium%type)
            is select auditorium, auditorium_capacity from auditorium
            where auditorium_capacity between cap1 and cap2 for update;
      aum auditorium.auditorium%type;
      cap auditorium.auditorium_capacity%type;
    begin
      open cur(0,20);
      fetch cur into aum, cap;
      while(cur%found)
      loop
          delete auditorium where current of cur;
          fetch cur into aum, cap;
      end loop;
      close cur;
      
      for pp in cur(0,120) loop
        dbms_output.put_line(pp.auditorium||' '||pp.auditorium_capacity);
      end loop; rollback;
    end;
    rollback;


--19. Применение псевдостолбца ROWID в update, delete
    declare
    cursor cur(capacity auditorium.auditorium%type)
            is select auditorium, auditorium_capacity, rowid
            from auditorium
            where auditorium_capacity >=capacity for update;
      aum auditorium.auditorium%type;
    begin
        fetch cur(80)
      for xxx in cur(80)
      loop
        if xxx.auditorium_capacity >=80
          then update auditorium
          set auditorium_capacity = auditorium_capacity+3
          where rowid = xxx.rowid;
        end if;
      end loop;
      for yyy in cur(80)
      loop
        dbms_output.put_line(yyy.auditorium||' '||yyy.auditorium_capacity);
      end loop; rollback;
   end;
  select * from auditorium;
  
  
--20. Все преподы, разделив на группы по 3
    declare 
      cursor curs_teacher is select teacher, teacher_name, pulpit
          from teacher;
      m_teacher teacher.teacher%type;
      m_teacher_name teacher.teacher_name%type;
      m_pulpit teacher.pulpit%type;
      k integer :=1;
    begin
    open curs_teacher;
    loop
    fetch curs_teacher into m_teacher, m_teacher_name, m_pulpit;
    exit when curs_teacher%notfound;
    DBMS_OUTPUT.PUT_LINE(' '||curs_teacher%rowcount||' '
                          ||m_teacher||' '
                          ||m_teacher_name||' '
                          ||m_pulpit);
    if (k mod 3 = 0) then DBMS_OUTPUT.PUT_LINE('-------------------------------------------'); end if;
    k:=k+1;
    end loop;
     DBMS_OUTPUT.PUT_LINE('rowcount = '|| curs_teacher%rowcount);
     close curs_teacher;
    exception
    when others
    then  DBMS_OUTPUT.PUT_LINE(sqlerrm);
    end;
