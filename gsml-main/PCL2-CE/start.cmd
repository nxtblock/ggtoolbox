chcp 65001

@echo off
if exist "./PCL2/Plain Craft Launcher.exe" goto skip
echo ========================================
echo 正在下载 PCL2 启动器
echo ========================================
powershell -Command "Start-BitsTransfer -Source 'https://gitproxy.click/https://raw.githubusercontent.com/OI-liyifan202201/nbsmc-PCL2-in-GSML/refs/heads/main/nbsmc.zip' -Destination nbsmc.zip"
echo 下载完成！
echo ========================================
echo 正在解压文件
echo ========================================
powershell -Command "Expand-Archive -Path nbsmc.zip -DestinationPath ./PCL2"
echo 解压完成！
:skip
echo ========================================
echo 启动 PCL2 启动器
echo ========================================
start "" "./PCL2/Plain Craft Launcher.exe"
ping 127.0.0.1 -n 3 >nul
exit
