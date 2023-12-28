alter session set container=cdb$root;

--созд таблицы куда копир д-е
 create table job_table_1 (
    id int primary key
);

create table job_table_2 (
    id int primary key
);

create table job_status (
    status   nvarchar2(50),
    datetime timestamp default current_timestamp
);

--копирование части д-х по усл.из 1 табл в другую из первой табл д-е удаляются
--провер.выполнено ли задание + сохран.сведения о попытках выполения (успешн и нет)
create or replace procedure jobprocedure is
    rowcount int;
    cursor c_1 is select * from job_table_1;
    cursor c_2 is select * from job_table_2;
begin
    select count(*) into rowcount from job_table_1;
    if rowcount = 0 then
        insert into job_status values ('2->1', sysdate);
        for v_row in c_2 loop
            insert into job_table_1(id) values(v_row.id);
        end loop;
        delete job_table_2;
    else
        insert into job_status values ('1->2', sysdate);
        for v_row in c_1 loop
            insert into job_table_2(id) values(v_row.id);
        end loop;
        delete job_table_1;
    end if;
    commit;
    exception when others then insert into job_status values ('FAIL', sysdate);
    commit ;
end;

begin
  jobprocedure();
end;

select * from job_table_1;
select * from job_table_2;
select * from job_status;


--создать задание (задание выполняется в опред.время (через неделю))
declare v_job number;
begin
  SYS.dbms_job.submit(
      job => v_job,
      what => 'begin jobprocedure(); end;',
      next_date => TRUNC(SYSDATE) + (10+30/60)/24,
        interval => 'NEXT_DAY(TRUNC(SYSDATE), 5) + 6/24'
      );
  commit;
end;

begin
    SYS.DBMS_JOB.REMOVE(2);
end;
select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;

--запустить немедленно
begin
  dbms_job.run(25);
end;
select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;

--разрешение задания
begin
  dbms_job.broken(63, broken => true);
end;
select job, what, last_date, last_sec, next_date, next_sec, broken from user_jobs;

--удалить задание из очереди
begin
  dbms_job.remove(63);
end;
BEGIN
  DBMS_SCHEDULER.CREATE_PROGRAM(
    program_name        => 'JOB_PROGRAM',
    program_type        => 'STORED_PROCEDURE',
    program_action      => 'jobprocedure',
    number_of_arguments => 0,
    enabled             => TRUE
  );
end;
begin
  DBMS_SCHEDULER.CREATE_JOB(
    job_name        => 'JOB_TASK',
    job_type => 'plsql_block',
    job_action => 'begin jobprocedure(); commit; end;',
    start_date      => TRUNC(sysdate) - (3/24) + (10+41/60)/24,
    enabled         => TRUE
  );
  commit;
END;

begin
    dbms_scheduler.STOP_JOB('job_task');
end;
begin
    dbms_scheduler.drop_job('JOB_TASK');
end;

select job_name, job_type, job_action, start_date, repeat_interval, next_run_date, enabled from user_scheduler_jobs;
select job_name, state from  user_scheduler_jobs;

select * from job_table_1;
select * from job_table_2;
select * from job_status order by datetime;
