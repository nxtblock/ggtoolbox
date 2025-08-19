chcp 65001
@echo off

cls

:: 清屏并显示标题
echo ========================================
echo    VSCode C++ 自动安装脚本
echo         开发者：liyifan202201
echo ========================================
echo.

:: 创建工作目录
md C:\VSc-cpp >nul 2>nul

:: VSCode安装检查
if exist C:\VSc-cpp\install-ok.txt goto ainstall

echo [开始] 正在下载 Visual Studio Code...
echo.

:download_vscode
powershell -Command "Invoke-WebRequest -Uri 'https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user' -OutFile 'VSC.exe'"
if %errorlevel%==1 (
    echo [错误] VSCode下载失败，正在重试...
    goto download_vscode
)

echo [完成] 已成功下载VSCode
echo [开始] 正在静默安装VSCode...
echo.

start /wait VSC.exe /VERYSILENT /SP-
del VSC.exe
taskkill /f /im code.exe >nul 2>nul

echo [完成] VSCode安装成功
echo [提示] 已创建安装标识文件
echo IAKIOI >> C:\VSc-cpp\install-ok.txt
echo.

:ainstall

:: MinGW安装检查
if exist C:\VSc-cpp\insMinGW-ok.txt goto binstall

echo [开始] 正在下载 MinGW64...
echo.

:download_mingw
powershell -Command "Invoke-WebRequest -Uri 'https://gitproxy.click/https://github.com/OI-liyifan202201/OI-liyifan202201/releases/download/v1.0/MinGW-64.zip' -OutFile 'MinGW.zip'"
if %errorlevel%==1 (
    echo [错误] MinGW下载失败，正在重试...
    goto download_mingw
)

echo [完成] 已成功下载MinGW
echo [开始] 正在解压MinGW...
echo.

powershell -Command "Expand-Archive -Path 'MinGW.zip' -DestinationPath 'C:\VSc-cpp\MinGW\'"
echo [完成] MinGW解压完成
echo [提示] 已创建MinGW安装标识文件
echo IAKIOI >> C:\VSc-cpp\insMinGW-ok.txt
del MinGW.zip >nul 2>nul
echo.

:: 设置PATH环境变量
echo [信息] 正在更新系统环境变量...
echo.

for /f "tokens=2,*" %%A in ('reg query HKCU\Environment /v PATH 2^>nul') do set my_user_path=%%B
setx path "%my_user_path%;C:\VSc-cpp\MinGW\bin;%appdata%\..\Local\Programs\Microsoft VS Code\bin"
echo [完成] 环境变量更新完成
echo.

goto main

:: 环境准备函数
:SetFromReg
    "%WinDir%\System32\Reg" QUERY "%~1" /v "%~2" > "%TEMP%\_envset.tmp" 2>NUL
    for /f "usebackq skip=2 tokens=2,*" %%A IN ("%TEMP%\_envset.tmp") do (
        echo/set "%~3=%%B"
    )
    goto :EOF

:GetRegEnv
    "%WinDir%\System32\Reg" QUERY "%~1" > "%TEMP%\_envget.tmp"
    for /f "usebackq skip=2" %%A IN ("%TEMP%\_envget.tmp") do (
        if /I not "%%~A"=="Path" (
            call :SetFromReg "%~1" "%%~A" "%%~A"
        )
    )
    goto :EOF

:main
    echo [信息] 正在准备系统环境...
    echo/@echo off >"%TEMP%\_env.cmd"
    call :GetRegEnv "HKLM\System\CurrentControlSet\Control\Session Manager\Environment" >> "%TEMP%\_env.cmd"
    call :GetRegEnv "HKCU\Environment" >> "%TEMP%\_env.cmd"
    call :SetFromReg "HKLM\System\CurrentControlSet\Control\Session Manager\Environment" Path Path_HKLM >> "%TEMP%\_env.cmd"
    call :SetFromReg "HKCU\Environment" Path Path_HKCU >> "%TEMP%\_env.cmd"
    echo/set "Path=%%Path_HKLM%%;%%Path_HKCU%%" >> "%TEMP%\_env.cmd"
    del /f /q "%TEMP%\_envset.tmp" 2>nul
    del /f /q "%TEMP%\_envget.tmp" 2>nul
    set "OriginalUserName=%USERNAME%"
    set "OriginalArchitecture=%PROCESSOR_ARCHITECTURE%"
    call "%TEMP%\_env.cmd"
    del /f /q "%TEMP%\_env.cmd" 2>nul
    set "USERNAME=%OriginalUserName%"
    set "PROCESSOR_ARCHITECTURE=%OriginalArchitecture%"
    echo [完成] 系统环境准备完成
    echo.

:: 安装VSCode扩展
:binstall
echo [开始] 安装VSCode扩展包...
echo.

set code_bin="%appdata%\..\Local\Programs\Microsoft VS Code\bin\code"

echo [扩展] 正在安装: 中文语言包
start /wait /b powershell code --install-extension MS-CEINTL.vscode-language-pack-zh-hans >nul 2>nul
echo { "locale": "zh-cn", "enable-crash-reporter": false} > %USERPROFILE%\.vscode\argv.json

echo [扩展] 正在安装: C++ 包
start /wait /b powershell code --install-extension ms-vscode.cpptools-extension-pack >nul 2>nul
start /wait /b powershell code --install-extension ms-vscode.cpptools >nul 2>nul

echo [扩展] 正在安装: 洛谷助手
start /wait /b powershell code --install-extension yltx.vscode-luogu >nul 2>nul

echo [扩展] 正在安装: 竞赛编程助手
start /wait /b powershell code --install-extension DivyanshuAgrawal.competitive-programming-helper >nul 2>nul

echo [扩展] 正在安装: CodeGeeX
start /wait /b powershell code --install-extension aminer.codegeex >nul 2>nul

echo [扩展] 正在安装: Error Lens
start /wait /b powershell code --install-extension usernamehw.errorlens >nul 2>nul

echo [完成] 所有VSCode扩展安装完成
echo.

:: 最终启动
echo [提示] 即将启动VSCode...
echo [信息] 请稍候...
echo ========================================
explorer "%appdata%\..\Local\Programs\Microsoft VS Code\code.exe"
ping -n 10 127.0.0.1 >nul
taskkill /f /im code.exe >nul 2>nul
powershell "[system.Reflection.Assembly]::LoadWithPartialName('System.Windows.Forms') | Out-Null;$balloon = New-Object System.Windows.Forms.NotifyIcon;$path = Get-Process -id $pid | Select-Object -ExpandProperty Path;$icon = [System.Drawing.Icon]::ExtractAssociatedIcon($path);$balloon.Icon = $icon;$balloon.BalloonTipIcon = 'Info';$balloon.BalloonTipText = '你可以使用你的vscode了。';$balloon.BalloonTipTitle = 'VScode安装完成';$balloon.Visible = $true;$balloon.ShowBalloonTip(1)"
echo ========================================
echo [完成] 安装流程结束，请按任意键退出...
pause
explorer "%appdata%\..\Local\Programs\Microsoft VS Code\code.exe"

exit

