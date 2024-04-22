@echo off
setlocal enabledelayedexpansion

set "failed_tests=0"
set "successful_tests=0"

for %%i in (test*.txt) do (
    E:\Vincent\Language_C++\Code_At_VS\Ocen_Client\x64\Debug\Ocen_Client.exe < "%%i" > "output%%i"
    
    fc "output%%i" "answer%%i" > nul
    if errorlevel 1 (
        echo %%i : BAD
        set /a failed_tests+=1
    ) else (
        set /a successful_tests+=1
    )
)

echo.
echo Tests passed: %successful_tests%
echo Tests failed: %failed_tests%

if %failed_tests% equ 0 (
    echo All tests passed successfully!
) else (
    echo Some tests failed. Check the output for details.
)

endlocal
