@echo off
set p1=%1
set p2=%2
set p3=%3
set /a sum=p1+p2
set /a dif=p2-p1
set /a mul=p1*p2
set /a div=p3/p2
set /a left=dif
set /a right=p1-p2
set /a op=left*right
echo -- ������ ����������: %*
echo -- �������� 1: %p1%
echo -- �������� 2: %p2%
echo -- �������� 3: %p3%
echo %p1% + %p2% = %sum%
echo %p1% * %p2% = %mul%
echo %p3% / %p2% = %div%
echo %p2% - %p1% = %dif%
echo (%p2%-%p1%)*(%p1%-%p2%) = %op%