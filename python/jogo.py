# Nave - Python + pygame   (pip install pygame)
import random
import sys

import pygame

W, H = 480, 640
pygame.init()
tela = pygame.display.set_mode((W, H))
pygame.display.set_caption("Nave - Python")
relogio = pygame.time.Clock()
fonte = pygame.font.SysFont(None, 28)
grande = pygame.font.SysFont(None, 60)

estrelas = [[random.uniform(0, W), random.uniform(0, H), random.uniform(20, 90)] for _ in range(60)]


def novo_jogo():
    return {"x": W / 2, "balas": [], "inimigos": [], "pontos": 0, "vidas": 3, "recarga": 0.0, "spawn": 0.5}


def bate(ax, ay, aw, ah, bx, by, bw, bh):
    return abs(ax - bx) < (aw + bw) / 2 and abs(ay - by) < (ah + bh) / 2


def texto(msg, f, cor, x, y, centro=False):
    img = f.render(msg, True, cor)
    tela.blit(img, (x - img.get_width() / 2 if centro else x, y))


jogo = novo_jogo()
recorde = 0

while True:
    dt = relogio.tick(60) / 1000
    for e in pygame.event.get():
        if e.type == pygame.QUIT or (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE):
            pygame.quit()
            sys.exit()
        if e.type == pygame.KEYDOWN and e.key == pygame.K_RETURN and jogo["vidas"] <= 0:
            jogo = novo_jogo()

    for s in estrelas:
        s[1] = (s[1] + s[2] * dt) % H

    if jogo["vidas"] > 0:
        k = pygame.key.get_pressed()
        dx = (k[pygame.K_RIGHT] or k[pygame.K_d]) - (k[pygame.K_LEFT] or k[pygame.K_a])
        jogo["x"] = max(20, min(W - 20, jogo["x"] + dx * 320 * dt))

        jogo["recarga"] -= dt
        if k[pygame.K_SPACE] and jogo["recarga"] <= 0:
            jogo["balas"].append([jogo["x"], H - 80])
            jogo["recarga"] = 0.2

        jogo["spawn"] -= dt
        if jogo["spawn"] <= 0:
            jogo["inimigos"].append([random.uniform(20, W - 20), -20])
            jogo["spawn"] = max(0.3, 0.9 - jogo["pontos"] * 0.002)

        vel = min(280, 100 + jogo["pontos"] * 0.6)
        for b in jogo["balas"]:
            b[1] -= 560 * dt
        for i in jogo["inimigos"]:
            i[1] += vel * dt

        for i in jogo["inimigos"][:]:
            for b in jogo["balas"][:]:
                if bate(i[0], i[1], 30, 30, b[0], b[1], 6, 14):
                    jogo["inimigos"].remove(i)
                    jogo["balas"].remove(b)
                    jogo["pontos"] += 10
                    break
        for i in jogo["inimigos"][:]:
            if bate(i[0], i[1], 30, 30, jogo["x"], H - 60, 36, 30):
                jogo["inimigos"].remove(i)
                jogo["vidas"] -= 1
            elif i[1] > H + 20:
                jogo["inimigos"].remove(i)
                jogo["vidas"] -= 1
        jogo["balas"] = [b for b in jogo["balas"] if b[1] > -20]
        recorde = max(recorde, jogo["pontos"])

    tela.fill((10, 10, 30))
    for s in estrelas:
        c = int(80 + s[2])
        tela.fill((c, c, c), (s[0], s[1], 2, 2))
    for b in jogo["balas"]:
        pygame.draw.rect(tela, (255, 230, 80), (b[0] - 3, b[1] - 7, 6, 14))
    for i in jogo["inimigos"]:
        pygame.draw.rect(tela, (230, 60, 70), (i[0] - 15, i[1] - 15, 30, 30))
        pygame.draw.rect(tela, (255, 220, 100), (i[0] - 9, i[1] - 5, 6, 6))
        pygame.draw.rect(tela, (255, 220, 100), (i[0] + 3, i[1] - 5, 6, 6))
    if jogo["vidas"] > 0:
        x = jogo["x"]
        pygame.draw.polygon(tela, (80, 220, 255), [(x, H - 78), (x - 18, H - 46), (x + 18, H - 46)])
    texto(f"Pontos: {jogo['pontos']}   Vidas: {max(0, jogo['vidas'])}", fonte, (255, 255, 255), 10, 10)
    texto(f"Recorde: {recorde}", fonte, (170, 170, 200), W - 130, 10)
    if jogo["vidas"] <= 0:
        texto("FIM DE JOGO", grande, (255, 90, 90), W / 2, H / 2 - 50, True)
        texto("ENTER para jogar de novo", fonte, (255, 255, 255), W / 2, H / 2 + 10, True)
    pygame.display.flip()
