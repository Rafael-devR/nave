# Nave — o mesmo jogo em 8 linguagens

Uma nave que atira em quadrados vermelhos que descem. Janela gráfica de 480x640.

**Controles:** ← → (ou A D) movem, **ESPAÇO** atira (segure), **ENTER** reinicia, **ESC** sai.
**Regras:** 3 vidas. Perde uma vida se um inimigo bate na nave ou chega no fundo. Cada inimigo vale 10 pontos e o jogo acelera com a pontuação.

Abra um terminal dentro da pasta `nave` e siga a sua linguagem.

## Os mais fáceis (quase nada para instalar)

| Linguagem | O que instalar | Como rodar |
|---|---|---|
| **TypeScript** | nada | dê dois cliques em `typescript/index.html` |
| **Java** | JDK 11 ou mais novo | `cd java` → `javac Jogo.java` → `java Jogo` |
| **Python** | `pip install pygame` | `python python/jogo.py` |
| **Lua** | LÖVE: love2d.org (Linux: `sudo apt install love`) | `love lua` (Windows: arraste a pasta `lua` sobre `love.exe`) |

## Os que baixam bibliotecas sozinhos

| Linguagem | O que instalar | Como rodar |
|---|---|---|
| **Rust** | Rust: rustup.rs | `cd rust` → `cargo run --release` (a 1ª vez demora, baixa o macroquad) |
| **C#** | .NET 8 SDK: dotnet.microsoft.com | `cd csharp` → `dotnet run` (baixa o Raylib-cs) |

Linux, se o Rust reclamar de biblioteca: `sudo apt install libx11-dev libxi-dev libgl-dev libasound2-dev`

## C e C++ (precisam do raylib)

**Windows:** instale o raylib em raylib.com (o instalador já traz o compilador). Depois, num terminal do raylib:

    gcc c/jogo.c -o jogo_c.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
    g++ cpp/jogo.cpp -o jogo_cpp.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

**Linux (Ubuntu/Debian):**

    sudo apt install build-essential git cmake libgl-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    git clone --depth 1 --branch 5.5 https://github.com/raysan5/raylib.git
    cd raylib && cmake -B build -DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=ON && cmake --build build -j4
    sudo cmake --install build && sudo ldconfig && cd ..
    gcc c/jogo.c -o jogo_c -lraylib -lm && ./jogo_c
    g++ cpp/jogo.cpp -o jogo_cpp -lraylib -lm && ./jogo_cpp

## O que foi testado

Rodei numa tela virtual (Linux) com teclas simuladas: **Python, Lua (LÖVE 11.5), TypeScript, C, C++ e Java** abrem, movem, atiram, pontuam, perdem as vidas, mostram FIM DE JOGO e reiniciam.
**Rust e C# não consegui compilar no ambiente de testes.** Conferi o código contra a API das bibliotecas, mas se algum der erro, me mande a mensagem.
