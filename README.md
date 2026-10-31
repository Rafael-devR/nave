# Jogos simples: Adivinhe o Número

O mesmo jogo feito em 8 linguagens. Você pensa em um número de 1 a 100 e o programa
diz se o palpite foi "muito alto" ou "muito baixo" até você acertar. Roda no terminal e
não usa nenhuma biblioteca externa: só o compilador ou interpretador da linguagem.

## Como rodar

Abra um terminal dentro desta pasta (`jogos-simples`) e use os comandos abaixo.

**Python**
```bash
python python/adivinhe_numero.py
```

**Lua** (precisa do interpretador `lua` instalado)
```bash
lua lua/adivinhe_numero.lua
```

**TypeScript** (Node.js 22.18 ou mais novo roda o `.ts` direto)
```bash
node typescript/adivinhe_numero.ts
```
Alternativa com compilação: `tsc typescript/adivinhe_numero.ts` e depois `node typescript/adivinhe_numero.js`.

**C**
```bash
gcc c/adivinhe_numero.c -o c/adivinhe_numero.exe
c/adivinhe_numero.exe
```

**C++**
```bash
g++ -std=c++17 cpp/adivinhe_numero.cpp -o cpp/adivinhe_numero.exe
cpp/adivinhe_numero.exe
```

**Rust**
```bash
rustc rust/adivinhe_numero.rs -o rust/adivinhe_numero.exe
rust/adivinhe_numero.exe
```

**C#** (.NET SDK)
```bash
dotnet run --project csharp/AdivinheNumero
```

**Java** (JDK 11 ou mais novo)
```bash
java java/AdivinheNumero.java
```
Alternativa com compilação: `javac java/AdivinheNumero.java` e depois `java -cp java AdivinheNumero`.

## Versões com janela gráfica

Além das versões de terminal, cada linguagem tem uma versão com janela, com o mesmo jogo.

| Linguagem  | Arquivo | Como rodar | Dependência extra |
|------------|---------|------------|-------------------|
| Python     | `python/adivinhe_numero_gui.py` | `python python/adivinhe_numero_gui.py` | nenhuma (tkinter já vem com o Python) |
| Lua        | `lua/gui/` | `"C:\Program Files\LOVE\love.exe" lua/gui` | LÖVE 11 (instalado neste PC) |
| TypeScript | `typescript/gui/index.html` | abrir o arquivo no navegador | nenhuma (roda no navegador) |
| C          | `c/adivinhe_numero_gui.c` | `gcc -mwindows c/adivinhe_numero_gui.c -o c/adivinhe_numero_gui.exe -lgdi32` | nenhuma (API Win32 + gcc) |
| C++        | `cpp/adivinhe_numero_gui.cpp` | `g++ -mwindows cpp/adivinhe_numero_gui.cpp -o cpp/adivinhe_numero_gui.exe -lgdi32` | nenhuma (API Win32 + g++) |
| Rust       | `rust/adivinhe_gui/` | `cargo run --release --manifest-path rust/adivinhe_gui/Cargo.toml` | baixa o `eframe`/`egui` do crates.io na primeira vez |
| C#         | `csharp/AdivinheNumeroGui/` | `dotnet run --project csharp/AdivinheNumeroGui` | nenhuma (WinForms vem com o .NET) |
| Java       | `java/AdivinheNumeroGui.java` | `java java/AdivinheNumeroGui.java` | nenhuma (Swing vem com o JDK) |

Observações:
- A versão TypeScript usa o arquivo `adivinhe_numero.js` já compilado. Para recompilar: `tsc typescript/gui/adivinhe_numero.ts --strict --lib es2020,dom --target es2020`.
- A versão Lua precisa de OpenGL 2.1. Neste PC o OpenGL estava em modo genérico (GDI), e o LÖVE não abriu a janela. Os arquivos foram checados só na sintaxe.

## Dependências deste PC

Verificado em `C:\Users\Skyro` (Windows):

| Linguagem  | Ferramenta necessária | Encontrada neste PC                       | Situação                      |
|------------|-----------------------|-------------------------------------------|-------------------------------|
| Python     | Python 3              | Python 3.12.10 (`python`) e 3.15.1 (`py`) | OK                            |
| Lua        | interpretador `lua`   | não instalado                             | Falta instalar                |
| TypeScript | Node.js               | Node.js v22.23.4                          | OK (roda `.ts` direto)        |
| C          | gcc                   | gcc 15.2.0 (MinGW-w64)                    | OK                            |
| C++        | g++                   | g++ 15.2.0 (MinGW-w64)                    | OK                            |
| Rust       | rustc e cargo         | rustc 1.99.1, cargo 1.99.1                | OK                            |
| C#         | .NET SDK              | .NET SDK 10.0.402                         | OK                            |
| Java       | JDK                   | OpenJDK 17.0.20 (`javac` e `java`)        | OK                            |

Só falta o **Lua**. Para instalar, o jeito mais simples é com o Scoop (`scoop install lua`)
ou baixando o binário pelo site oficial (lua.org). O `tsc` (compilador TypeScript) não está
instalado, mas não é necessário: o Node roda o `.ts` direto. Se quiser o `tsc`: `npm install -g typescript`.

Todos os programas foram compilados e testados neste PC. Lua foi testado com um interpretador
Lua 5.5 embutido (via pacote `lupa` do Python), pois o `lua` standalone não está instalado.
