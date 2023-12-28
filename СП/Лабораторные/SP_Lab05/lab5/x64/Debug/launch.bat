start cmd.exe /c "OS13_CREATE.exe 50 1 6 30 ./../../storage/HT-1.ht"
start cmd.exe /c "OS13_CREATE.exe 50 1 6 30 ./../../storage/HT-2.ht"
timeout /t 2 >nul
start cmd.exe /c "OS13_START ./../../storage/HT-1.ht"
start cmd.exe /c "OS13_START ./../../storage/HT-2.ht"
rimeout /t 2 >nul
start cmd.exe /c "OS_0402.exe ./../../storage/HT-1.ht"
start cmd.exe /c "OS_0402.exe ./../../storage/HT-2.ht"
timeout /t 5 >nul
start cmd.exe /c "OS13_STOP.exe ./../../storage/HT-2.ht"