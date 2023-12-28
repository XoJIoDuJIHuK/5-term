@echo off
set p1=%1
set p2=%2
set p3=%3
set res=0
echo -- строка параметров: %*
echo -- параметр 1: %p1%
echo -- параметр 2: %p2%
echo -- параметр 3: %p3%
if "+"=="%p3%" (set /a res=%p1%+%p2%
) else if "-"=="%p3%" (set /a res=%p1%-%p2%
) else if "*"=="%p3%" (set /a res=%p1%*%p2%
) else if "/"=="%p3%" (set /a res=%p1%/%p2%
) else echo Нет такой операции
echo результат: %res%