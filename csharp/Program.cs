// Nave - C# + Raylib-cs.  Rode com:  dotnet run
using System.Numerics;
using Raylib_cs;
using static Raylib_cs.Raylib;

const int W = 480, H = 640;

InitWindow(W, H, "Nave - C#");
SetTargetFPS(60);

var rng = new Random();
float Rnd(float a, float b) => a + (float)rng.NextDouble() * (b - a);
bool Bate(Vector2 a, float aw, float ah, Vector2 b, float bw, float bh) =>
    MathF.Abs(a.X - b.X) < (aw + bw) / 2 && MathF.Abs(a.Y - b.Y) < (ah + bh) / 2;

var estrelas = new List<Vector3>();
for (int i = 0; i < 60; i++) estrelas.Add(new Vector3(Rnd(0, W), Rnd(0, H), Rnd(20, 90)));

var jogo = new Jogo();
int recorde = 0;

while (!WindowShouldClose())
{
    float dt = MathF.Min(GetFrameTime(), 0.05f);
    for (int i = 0; i < estrelas.Count; i++)
    {
        var s = estrelas[i];
        s.Y += s.Z * dt;
        if (s.Y > H) s.Y -= H;
        estrelas[i] = s;
    }

    if (jogo.Vidas <= 0 && IsKeyPressed(KeyboardKey.Enter)) jogo = new Jogo();

    if (jogo.Vidas > 0)
    {
        float dir = (IsKeyDown(KeyboardKey.Right) || IsKeyDown(KeyboardKey.D) ? 1 : 0)
                  - (IsKeyDown(KeyboardKey.Left) || IsKeyDown(KeyboardKey.A) ? 1 : 0);
        jogo.X = Math.Clamp(jogo.X + dir * 320 * dt, 20, W - 20);

        jogo.Recarga -= dt;
        if (IsKeyDown(KeyboardKey.Space) && jogo.Recarga <= 0)
        {
            jogo.Balas.Add(new Vector2(jogo.X, H - 80));
            jogo.Recarga = 0.2f;
        }

        jogo.Spawn -= dt;
        if (jogo.Spawn <= 0)
        {
            jogo.Inimigos.Add(new Vector2(Rnd(20, W - 20), -20));
            jogo.Spawn = MathF.Max(0.3f, 0.9f - jogo.Pontos * 0.002f);
        }

        float vel = MathF.Min(280, 100 + jogo.Pontos * 0.6f);
        for (int i = 0; i < jogo.Balas.Count; i++) jogo.Balas[i] -= new Vector2(0, 560 * dt);
        for (int i = 0; i < jogo.Inimigos.Count; i++) jogo.Inimigos[i] += new Vector2(0, vel * dt);

        var jogador = new Vector2(jogo.X, H - 60);
        var vivos = new List<Vector2>();
        foreach (var i in jogo.Inimigos)
        {
            int b = jogo.Balas.FindIndex(t => Bate(i, 30, 30, t, 6, 14));
            if (b >= 0) { jogo.Balas.RemoveAt(b); jogo.Pontos += 10; }
            else if (Bate(i, 30, 30, jogador, 36, 30) || i.Y > H + 20) jogo.Vidas--;
            else vivos.Add(i);
        }
        jogo.Inimigos = vivos;
        jogo.Balas.RemoveAll(b => b.Y < -20);
        recorde = Math.Max(recorde, jogo.Pontos);
    }

    BeginDrawing();
    ClearBackground(new Color(10, 10, 30, 255));
    foreach (var s in estrelas)
    {
        byte c = (byte)(80 + s.Z);
        DrawRectangle((int)s.X, (int)s.Y, 2, 2, new Color(c, c, c, (byte)255));
    }
    foreach (var b in jogo.Balas) DrawRectangle((int)b.X - 3, (int)b.Y - 7, 6, 14, new Color(255, 230, 80, 255));
    foreach (var i in jogo.Inimigos)
    {
        DrawRectangle((int)i.X - 15, (int)i.Y - 15, 30, 30, new Color(230, 60, 70, 255));
        DrawRectangle((int)i.X - 9, (int)i.Y - 5, 6, 6, new Color(255, 220, 100, 255));
        DrawRectangle((int)i.X + 3, (int)i.Y - 5, 6, 6, new Color(255, 220, 100, 255));
    }
    if (jogo.Vidas > 0) // vértices em sentido anti-horário, como o raylib pede
        DrawTriangle(new Vector2(jogo.X, H - 78), new Vector2(jogo.X - 18, H - 46), new Vector2(jogo.X + 18, H - 46), new Color(80, 220, 255, 255));
    DrawText($"Pontos: {jogo.Pontos}   Vidas: {Math.Max(0, jogo.Vidas)}", 10, 10, 20, Color.White);
    DrawText($"Recorde: {recorde}", W - 130, 10, 20, new Color(170, 170, 200, 255));
    if (jogo.Vidas <= 0)
    {
        DrawText("FIM DE JOGO", W / 2 - MeasureText("FIM DE JOGO", 48) / 2, H / 2 - 50, 48, new Color(255, 90, 90, 255));
        DrawText("ENTER para jogar de novo", W / 2 - MeasureText("ENTER para jogar de novo", 20) / 2, H / 2 + 20, 20, Color.White);
    }
    EndDrawing();
}
CloseWindow();

class Jogo
{
    public float X = W / 2f, Recarga = 0, Spawn = 0.5f;
    public int Pontos = 0, Vidas = 3;
    public List<Vector2> Balas = new(), Inimigos = new();
    const int W = 480;
}
