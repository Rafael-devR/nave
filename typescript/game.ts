// Nave - TypeScript + canvas. Abra index.html no navegador.
// Para recompilar depois de editar:  npx tsc --target es2017 game.ts
const W = 480, H = 640;
const canvas = document.getElementById("c") as HTMLCanvasElement;
const ctx = canvas.getContext("2d")!;

interface Ponto { x: number; y: number }
interface Estrela extends Ponto { v: number }
interface Jogo { x: number; balas: Ponto[]; inimigos: Ponto[]; pontos: number; vidas: number; recarga: number; spawn: number }

const novo = (): Jogo => ({ x: W / 2, balas: [], inimigos: [], pontos: 0, vidas: 3, recarga: 0, spawn: 0.5 });
const rnd = (a: number, b: number) => a + Math.random() * (b - a);
const bate = (a: Ponto, aw: number, ah: number, b: Ponto, bw: number, bh: number) =>
  Math.abs(a.x - b.x) < (aw + bw) / 2 && Math.abs(a.y - b.y) < (ah + bh) / 2;

const estrelas: Estrela[] = Array.from({ length: 60 }, () => ({ x: rnd(0, W), y: rnd(0, H), v: rnd(20, 90) }));
let jogo = novo();
let recorde = 0;
const teclas = new Set<string>();

addEventListener("keydown", e => {
  teclas.add(e.code);
  if (["Space", "ArrowLeft", "ArrowRight"].includes(e.code)) e.preventDefault();
  if (e.code === "Enter" && jogo.vidas <= 0) jogo = novo();
});
addEventListener("keyup", e => teclas.delete(e.code));

function atualizar(dt: number): void {
  estrelas.forEach(s => (s.y = (s.y + s.v * dt) % H));
  if (jogo.vidas <= 0) return;

  const dir = (teclas.has("ArrowRight") || teclas.has("KeyD") ? 1 : 0) - (teclas.has("ArrowLeft") || teclas.has("KeyA") ? 1 : 0);
  jogo.x = Math.max(20, Math.min(W - 20, jogo.x + dir * 320 * dt));

  jogo.recarga -= dt;
  if (teclas.has("Space") && jogo.recarga <= 0) {
    jogo.balas.push({ x: jogo.x, y: H - 80 });
    jogo.recarga = 0.2;
  }
  jogo.spawn -= dt;
  if (jogo.spawn <= 0) {
    jogo.inimigos.push({ x: rnd(20, W - 20), y: -20 });
    jogo.spawn = Math.max(0.3, 0.9 - jogo.pontos * 0.002);
  }

  const vel = Math.min(280, 100 + jogo.pontos * 0.6);
  jogo.balas.forEach(b => (b.y -= 560 * dt));
  jogo.inimigos.forEach(i => (i.y += vel * dt));

  const jogador = { x: jogo.x, y: H - 60 };
  jogo.inimigos = jogo.inimigos.filter(i => {
    const idx = jogo.balas.findIndex(b => bate(i, 30, 30, b, 6, 14));
    if (idx >= 0) {
      jogo.balas.splice(idx, 1);
      jogo.pontos += 10;
      return false;
    }
    if (bate(i, 30, 30, jogador, 36, 30) || i.y > H + 20) {
      jogo.vidas--;
      return false;
    }
    return true;
  });
  jogo.balas = jogo.balas.filter(b => b.y > -20);
  recorde = Math.max(recorde, jogo.pontos);
}

function desenhar(): void {
  ctx.fillStyle = "#0a0a1e";
  ctx.fillRect(0, 0, W, H);
  estrelas.forEach(s => {
    const c = Math.floor(80 + s.v);
    ctx.fillStyle = `rgb(${c},${c},${c})`;
    ctx.fillRect(s.x, s.y, 2, 2);
  });
  ctx.fillStyle = "#ffe650";
  jogo.balas.forEach(b => ctx.fillRect(b.x - 3, b.y - 7, 6, 14));
  jogo.inimigos.forEach(i => {
    ctx.fillStyle = "#e63c46";
    ctx.fillRect(i.x - 15, i.y - 15, 30, 30);
    ctx.fillStyle = "#ffdc64";
    ctx.fillRect(i.x - 9, i.y - 5, 6, 6);
    ctx.fillRect(i.x + 3, i.y - 5, 6, 6);
  });
  if (jogo.vidas > 0) {
    ctx.fillStyle = "#50dcff";
    ctx.beginPath();
    ctx.moveTo(jogo.x, H - 78);
    ctx.lineTo(jogo.x - 18, H - 46);
    ctx.lineTo(jogo.x + 18, H - 46);
    ctx.closePath();
    ctx.fill();
  }
  ctx.textAlign = "left";
  ctx.font = "20px sans-serif";
  ctx.fillStyle = "#fff";
  ctx.fillText(`Pontos: ${jogo.pontos}   Vidas: ${Math.max(0, jogo.vidas)}`, 10, 28);
  ctx.fillStyle = "#aaaac8";
  ctx.fillText(`Recorde: ${recorde}`, W - 130, 28);
  if (jogo.vidas <= 0) {
    ctx.textAlign = "center";
    ctx.fillStyle = "#ff5a5a";
    ctx.font = "bold 48px sans-serif";
    ctx.fillText("FIM DE JOGO", W / 2, H / 2 - 20);
    ctx.fillStyle = "#fff";
    ctx.font = "20px sans-serif";
    ctx.fillText("ENTER para jogar de novo", W / 2, H / 2 + 25);
  }
}

let antes = performance.now();
function quadro(agora: number): void {
  const dt = Math.min(0.05, (agora - antes) / 1000);
  antes = agora;
  atualizar(dt);
  desenhar();
  requestAnimationFrame(quadro);
}
requestAnimationFrame(quadro);
