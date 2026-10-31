@echo off
chcp 65001 >nul
title Nave - escolha um jogo
cd /d "%~dp0"
rem Caminho do raylib (so para C e C++). Ajuste se instalou em outra pasta.
set "RAYLIB=C:\raylib\raylib"

:menu
cls
echo.
echo   ===== NAVE - escolha a linguagem =====
echo.
echo   1  TypeScript  - abre no navegador, nao precisa instalar nada
echo   2  Java        - precisa do JDK (adoptium.net)
echo   3  Python      - precisa do Python (python.org); instala o pygame sozinho
echo   4  Lua         - precisa do LOVE (love2d.org)
echo   5  Rust        - precisa do Rust (rustup.rs)
echo   6  C#          - precisa do .NET 8 SDK (dotnet.microsoft.com)
echo   7  C           - precisa do raylib (raylib.com) com o gcc
echo   8  C++         - precisa do raylib (raylib.com) com o g++
echo   9  Sair
echo.
choice /c 123456789 /n /m "  Digite o numero e aperte Enter: "
if errorlevel 9 exit /b
if errorlevel 8 goto cpp
if errorlevel 7 goto c
if errorlevel 6 goto csharp
if errorlevel 5 goto rust
if errorlevel 4 goto lua
if errorlevel 3 goto python
if errorlevel 2 goto java
goto ts

:ts
start "" "typescript\index.html"
goto menu

:java
where javac >nul 2>&1 || (echo Falta o JDK. Instale em adoptium.net e reinicie o PC & pause & goto menu)
javac java\Jogo.java || (pause & goto menu)
java -cp java Jogo
goto menu

:python
where python >nul 2>&1 || (echo Falta o Python. Instale em python.org marcando a opcao Add python.exe to PATH & pause & goto menu)
python -m pip install -q pygame || (pause & goto menu)
python python\jogo.py
goto menu

:lua
where love >nul 2>&1 || (echo Falta o LOVE. Instale em love2d.org e depois rode este arquivo de novo & pause & goto menu)
love lua
goto menu

:rust
where cargo >nul 2>&1 || (echo Falta o Rust. Instale em rustup.rs e reinicie o PC & pause & goto menu)
pushd rust
cargo run --release || pause
popd
goto menu

:csharp
where dotnet >nul 2>&1 || (echo Falta o .NET 8 SDK. Instale em dotnet.microsoft.com & pause & goto menu)
pushd csharp
dotnet run || pause
popd
goto menu

:c
where gcc >nul 2>&1 || (echo Falta o gcc. Instale o raylib em raylib.com, que ja traz o compilador & pause & goto menu)
if not exist "%RAYLIB%\include\raylib.h" (echo Nao achei o raylib em %RAYLIB%. Ajuste a linha RAYLIB no inicio deste arquivo & pause & goto menu)
gcc c\jogo.c -o c\jogo.exe -I"%RAYLIB%\include" -L"%RAYLIB%\lib" -lraylib -lopengl32 -lgdi32 -lwinmm || (pause & goto menu)
c\jogo.exe
goto menu

:cpp
where g++ >nul 2>&1 || (echo Falta o g++. Instale o raylib em raylib.com, que ja traz o compilador & pause & goto menu)
if not exist "%RAYLIB%\include\raylib.h" (echo Nao achei o raylib em %RAYLIB%. Ajuste a linha RAYLIB no inicio deste arquivo & pause & goto menu)
g++ cpp\jogo.cpp -o cpp\jogo.exe -I"%RAYLIB%\include" -L"%RAYLIB%\lib" -lraylib -lopengl32 -lgdi32 -lwinmm || (pause & goto menu)
cpp\jogo.exe
goto menu
