start cmd.exe /c "OS11_CREATE.exe 50 1 6 30 ./../../storage/HTspace.ht"
timeout /t 1 >nul
start cmd.exe /c "OS11_START ./../../storage/HTspace.ht"
timeout /t 1 >nul
start cmd.exe /c "OS11_02.exe ./../../storage/HTspace.ht"
timeout /t 2 >nul
start cmd.exe /c "OS11_04.exe ./../../storage/HTspace.ht"
timeout /t 2 >nul
start cmd.exe /c "OS11_03.exe ./../../storage/HTspace.ht"