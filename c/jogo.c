/* Nave - C + raylib (https://www.raylib.com) */
#include "raylib.h"
#include <math.h>

#define W 480
#define H 640
#define MAX 128

typedef struct { float x, y; int vivo; } Obj;
typedef struct { float x, y, v; } Estrela;

static int bate(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    return fabsf(ax - bx) < (aw + bw) / 2 && fabsf(ay - by) < (ah + bh) / 2;
}
static float rnd(float a, float b) { return a + (b - a) * GetRandomValue(0, 10000) / 10000.0f; }

static void adiciona(Obj *v, float x, float y) {
    for (int i = 0; i < MAX; i++)
        if (!v[i].vivo) { v[i] = (Obj){ x, y, 1 }; return; }
}

int main(void) {
    InitWindow(W, H, "Nave - C");
    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    Estrela estrelas[60];
    for (int i = 0; i < 60; i++) estrelas[i] = (Estrela){ rnd(0, W), rnd(0, H), rnd(20, 90) };

    Obj balas[MAX] = {0}, inimigos[MAX] = {0};
    float x = W / 2, recarga = 0, spawn = 0.5f;
    int pontos = 0, vidas = 3, recorde = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        for (int i = 0; i < 60; i++) { estrelas[i].y += estrelas[i].v * dt; if (estrelas[i].y > H) estrelas[i].y -= H; }

        if (vidas <= 0 && IsKeyPressed(KEY_ENTER)) {
            for (int i = 0; i < MAX; i++) balas[i].vivo = inimigos[i].vivo = 0;
            x = W / 2; recarga = 0; spawn = 0.5f; pontos = 0; vidas = 3;
        }

        if (vidas > 0) {
            float dir = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
            x += dir * 320 * dt;
            if (x < 20) x = 20;
            if (x > W - 20) x = W - 20;

            recarga -= dt;
            if (IsKeyDown(KEY_SPACE) && recarga <= 0) { adiciona(balas, x, H - 80); recarga = 0.2f; }

            spawn -= dt;
            if (spawn <= 0) {
                adiciona(inimigos, rnd(20, W - 20), -20);
                spawn = 0.9f - pontos * 0.002f;
                if (spawn < 0.3f) spawn = 0.3f;
            }

            float vel = 100 + pontos * 0.6f;
            if (vel > 280) vel = 280;
            for (int i = 0; i < MAX; i++) {
                if (balas[i].vivo) { balas[i].y -= 560 * dt; if (balas[i].y < -20) balas[i].vivo = 0; }
                if (inimigos[i].vivo) inimigos[i].y += vel * dt;
            }
            for (int i = 0; i < MAX; i++) {
                if (!inimigos[i].vivo) continue;
                for (int j = 0; j < MAX && inimigos[i].vivo; j++) {
                    if (balas[j].vivo && bate(inimigos[i].x, inimigos[i].y, 30, 30, balas[j].x, balas[j].y, 6, 14)) {
                        inimigos[i].vivo = 0; balas[j].vivo = 0; pontos += 10;
                    }
                }
                if (inimigos[i].vivo && (bate(inimigos[i].x, inimigos[i].y, 30, 30, x, H - 60, 36, 30) || inimigos[i].y > H + 20)) {
                    inimigos[i].vivo = 0; vidas--;
                }
            }
            if (pontos > recorde) recorde = pontos;
        }

        BeginDrawing();
        ClearBackground((Color){ 10, 10, 30, 255 });
        for (int i = 0; i < 60; i++) {
            unsigned char c = (unsigned char)(80 + estrelas[i].v);
            DrawRectangle((int)estrelas[i].x, (int)estrelas[i].y, 2, 2, (Color){ c, c, c, 255 });
        }
        for (int i = 0; i < MAX; i++) {
            if (balas[i].vivo) DrawRectangle((int)balas[i].x - 3, (int)balas[i].y - 7, 6, 14, (Color){ 255, 230, 80, 255 });
            if (inimigos[i].vivo) {
                int ex = (int)inimigos[i].x, ey = (int)inimigos[i].y;
                DrawRectangle(ex - 15, ey - 15, 30, 30, (Color){ 230, 60, 70, 255 });
                DrawRectangle(ex - 9, ey - 5, 6, 6, (Color){ 255, 220, 100, 255 });
                DrawRectangle(ex + 3, ey - 5, 6, 6, (Color){ 255, 220, 100, 255 });
            }
        }
        if (vidas > 0)  /* vértices em sentido anti-horário, como o raylib pede */
            DrawTriangle((Vector2){ x, H - 78 }, (Vector2){ x - 18, H - 46 }, (Vector2){ x + 18, H - 46 }, (Color){ 80, 220, 255, 255 });
        DrawText(TextFormat("Pontos: %d   Vidas: %d", pontos, vidas > 0 ? vidas : 0), 10, 10, 20, WHITE);
        DrawText(TextFormat("Recorde: %d", recorde), W - 130, 10, 20, (Color){ 170, 170, 200, 255 });
        if (vidas <= 0) {
            DrawText("FIM DE JOGO", W / 2 - MeasureText("FIM DE JOGO", 48) / 2, H / 2 - 50, 48, (Color){ 255, 90, 90, 255 });
            DrawText("ENTER para jogar de novo", W / 2 - MeasureText("ENTER para jogar de novo", 20) / 2, H / 2 + 20, 20, WHITE);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
