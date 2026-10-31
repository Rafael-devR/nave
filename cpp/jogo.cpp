// Nave - C++ + raylib (https://www.raylib.com)
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

constexpr int W = 480, H = 640;

struct Estrela { float x, y, v; };

static bool bate(Vector2 a, float aw, float ah, Vector2 b, float bw, float bh) {
    return std::fabs(a.x - b.x) < (aw + bw) / 2 && std::fabs(a.y - b.y) < (ah + bh) / 2;
}
static float rnd(float a, float b) { return a + (b - a) * GetRandomValue(0, 10000) / 10000.0f; }

struct Jogo {
    float x = W / 2.0f, recarga = 0, spawn = 0.5f;
    int pontos = 0, vidas = 3;
    std::vector<Vector2> balas, inimigos;
};

int main() {
    InitWindow(W, H, "Nave - C++");
    SetTargetFPS(60);

    std::vector<Estrela> estrelas;
    for (int i = 0; i < 60; i++) estrelas.push_back({ rnd(0, W), rnd(0, H), rnd(20, 90) });
    Jogo j;
    int recorde = 0;

    while (!WindowShouldClose()) {
        float dt = std::min(GetFrameTime(), 0.05f);
        for (auto &s : estrelas) { s.y += s.v * dt; if (s.y > H) s.y -= H; }

        if (j.vidas <= 0 && IsKeyPressed(KEY_ENTER)) j = Jogo();

        if (j.vidas > 0) {
            float dir = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
            j.x = std::clamp(j.x + dir * 320 * dt, 20.0f, W - 20.0f);

            j.recarga -= dt;
            if (IsKeyDown(KEY_SPACE) && j.recarga <= 0) { j.balas.push_back({ j.x, H - 80.0f }); j.recarga = 0.2f; }

            j.spawn -= dt;
            if (j.spawn <= 0) {
                j.inimigos.push_back({ rnd(20, W - 20), -20.0f });
                j.spawn = std::max(0.3f, 0.9f - j.pontos * 0.002f);
            }

            float vel = std::min(280.0f, 100 + j.pontos * 0.6f);
            for (auto &b : j.balas) b.y -= 560 * dt;
            for (auto &i : j.inimigos) i.y += vel * dt;

            Vector2 jogador = { j.x, H - 60.0f };
            std::vector<Vector2> vivos;
            for (auto &i : j.inimigos) {
                auto b = std::find_if(j.balas.begin(), j.balas.end(), [&](Vector2 &t) { return bate(i, 30, 30, t, 6, 14); });
                if (b != j.balas.end()) { j.balas.erase(b); j.pontos += 10; }
                else if (bate(i, 30, 30, jogador, 36, 30) || i.y > H + 20) j.vidas--;
                else vivos.push_back(i);
            }
            j.inimigos = vivos;
            j.balas.erase(std::remove_if(j.balas.begin(), j.balas.end(), [](Vector2 &b) { return b.y < -20; }), j.balas.end());
            recorde = std::max(recorde, j.pontos);
        }

        BeginDrawing();
        ClearBackground({ 10, 10, 30, 255 });
        for (auto &s : estrelas) {
            auto c = (unsigned char)(80 + s.v);
            DrawRectangle((int)s.x, (int)s.y, 2, 2, { c, c, c, 255 });
        }
        for (auto &b : j.balas) DrawRectangle((int)b.x - 3, (int)b.y - 7, 6, 14, { 255, 230, 80, 255 });
        for (auto &i : j.inimigos) {
            DrawRectangle((int)i.x - 15, (int)i.y - 15, 30, 30, { 230, 60, 70, 255 });
            DrawRectangle((int)i.x - 9, (int)i.y - 5, 6, 6, { 255, 220, 100, 255 });
            DrawRectangle((int)i.x + 3, (int)i.y - 5, 6, 6, { 255, 220, 100, 255 });
        }
        if (j.vidas > 0)  // vértices em sentido anti-horário, como o raylib pede
            DrawTriangle({ j.x, H - 78.0f }, { j.x - 18, H - 46.0f }, { j.x + 18, H - 46.0f }, { 80, 220, 255, 255 });
        DrawText(TextFormat("Pontos: %d   Vidas: %d", j.pontos, std::max(0, j.vidas)), 10, 10, 20, WHITE);
        DrawText(TextFormat("Recorde: %d", recorde), W - 130, 10, 20, { 170, 170, 200, 255 });
        if (j.vidas <= 0) {
            DrawText("FIM DE JOGO", W / 2 - MeasureText("FIM DE JOGO", 48) / 2, H / 2 - 50, 48, { 255, 90, 90, 255 });
            DrawText("ENTER para jogar de novo", W / 2 - MeasureText("ENTER para jogar de novo", 20) / 2, H / 2 + 20, 20, WHITE);
        }
        EndDrawing();
    }
    CloseWindow();
}
