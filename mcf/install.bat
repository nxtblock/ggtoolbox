@echo off
setlocal enabledelayedexpansion



set RETRY_COUNT=0
set MAX_RETRIES=3

:retry
cd ../mcf/
echo yyy | cmcl.exe modpack --file=nbsmc.mrpack --storage=nbsmc -t=2048
echo yyy | cmcl nbsmc

tasklist /FI "IMAGENAME eq java.exe" | find /I "java.exe" >nul
if errorlevel 1 (
    echo java.exe 未运行，脚本自动退出。
    exit /b 1
)
if %ERRORLEVEL% equ 0 (
    echo io-ok
    exit /b 0
) else (
    set /a RETRY_COUNT+=1
    if !RETRY_COUNT! lss %MAX_RETRIES% (
        goto retry
    ) else (
        echo io-error
        exit /b 1
    )
)