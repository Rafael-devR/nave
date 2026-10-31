// Nave - Rust + macroquad.  Rode com:  cargo run --release
use macroquad::prelude::*;

const W: f32 = 480.0;
const H: f32 = 640.0;

fn conf() -> Conf {
    Conf {
        window_title: "Nave - Rust".to_owned(),
        window_width: W as i32,
        window_height: H as i32,
        window_resizable: false,
        ..Default::default()
    }
}

struct Jogo {
    x: f32,
    balas: Vec<Vec2>,
    inimigos: Vec<Vec2>,
    pontos: u32,
    vidas: i32,
    recarga: f32,
    spawn: f32,
}

impl Jogo {
    fn novo() -> Self {
        Jogo { x: W / 2.0, balas: vec![], inimigos: vec![], pontos: 0, vidas: 3, recarga: 0.0, spawn: 0.5 }
    }
}

fn bate(a: Vec2, aw: f32, ah: f32, b: Vec2, bw: f32, bh: f32) -> bool {
    (a.x - b.x).abs() < (aw + bw) / 2.0 && (a.y - b.y).abs() < (ah + bh) / 2.0
}

#[macroquad::main(conf)]
async fn main() {
    rand::srand(std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH).unwrap().as_nanos() as u64);
    let mut estrelas: Vec<(f32, f32, f32)> =
        (0..60).map(|_| (rand::gen_range(0.0, W), rand::gen_range(0.0, H), rand::gen_range(20.0, 90.0))).collect();
    let mut jogo = Jogo::novo();
    let mut recorde: u32 = 0;

    loop {
        if is_key_pressed(KeyCode::Escape) {
            break;
        }
        let dt = get_frame_time().min(0.05);
        for s in estrelas.iter_mut() {
            s.1 = (s.1 + s.2 * dt) % H;
        }

        if jogo.vidas <= 0 && is_key_pressed(KeyCode::Enter) {
            jogo = Jogo::novo();
        }

        if jogo.vidas > 0 {
            let mut dir: f32 = 0.0;
            if is_key_down(KeyCode::Right) || is_key_down(KeyCode::D) { dir += 1.0; }
            if is_key_down(KeyCode::Left) || is_key_down(KeyCode::A) { dir -= 1.0; }
            jogo.x = (jogo.x + dir * 320.0 * dt).clamp(20.0, W - 20.0);

            jogo.recarga -= dt;
            if is_key_down(KeyCode::Space) && jogo.recarga <= 0.0 {
                jogo.balas.push(vec2(jogo.x, H - 80.0));
                jogo.recarga = 0.2;
            }

            jogo.spawn -= dt;
            if jogo.spawn <= 0.0 {
                jogo.inimigos.push(vec2(rand::gen_range(20.0, W - 20.0), -20.0));
                jogo.spawn = (0.9_f32 - jogo.pontos as f32 * 0.002).max(0.3);
            }

            let vel = (100.0_f32 + jogo.pontos as f32 * 0.6).min(280.0);
            for b in jogo.balas.iter_mut() { b.y -= 560.0 * dt; }
            for i in jogo.inimigos.iter_mut() { i.y += vel * dt; }

            let jogador = vec2(jogo.x, H - 60.0);
            let mut balas = std::mem::take(&mut jogo.balas);
            let mut vivos = Vec::new();
            for i in jogo.inimigos.drain(..) {
                if let Some(idx) = balas.iter().position(|b| bate(i, 30.0, 30.0, *b, 6.0, 14.0)) {
                    balas.remove(idx);
                    jogo.pontos += 10;
                } else if bate(i, 30.0, 30.0, jogador, 36.0, 30.0) || i.y > H + 20.0 {
                    jogo.vidas -= 1;
                } else {
                    vivos.push(i);
                }
            }
            balas.retain(|b| b.y > -20.0);
            jogo.balas = balas;
            jogo.inimigos = vivos;
            recorde = recorde.max(jogo.pontos);
        }

        clear_background(Color::from_rgba(10, 10, 30, 255));
        for s in &estrelas {
            let c = (80.0 + s.2) / 255.0;
            draw_rectangle(s.0, s.1, 2.0, 2.0, Color::new(c, c, c, 1.0));
        }
        for b in &jogo.balas {
            draw_rectangle(b.x - 3.0, b.y - 7.0, 6.0, 14.0, Color::from_rgba(255, 230, 80, 255));
        }
        for i in &jogo.inimigos {
            draw_rectangle(i.x - 15.0, i.y - 15.0, 30.0, 30.0, Color::from_rgba(230, 60, 70, 255));
            draw_rectangle(i.x - 9.0, i.y - 5.0, 6.0, 6.0, Color::from_rgba(255, 220, 100, 255));
            draw_rectangle(i.x + 3.0, i.y - 5.0, 6.0, 6.0, Color::from_rgba(255, 220, 100, 255));
        }
        if jogo.vidas > 0 {
            draw_triangle(
                vec2(jogo.x, H - 78.0),
                vec2(jogo.x - 18.0, H - 46.0),
                vec2(jogo.x + 18.0, H - 46.0),
                Color::from_rgba(80, 220, 255, 255),
            );
        }
        draw_text(&format!("Pontos: {}   Vidas: {}", jogo.pontos, jogo.vidas.max(0)), 10.0, 26.0, 26.0, WHITE);
        draw_text(&format!("Recorde: {}", recorde), W - 130.0, 26.0, 26.0, Color::from_rgba(170, 170, 200, 255));
        if jogo.vidas <= 0 {
            draw_text("FIM DE JOGO", W / 2.0 - 130.0, H / 2.0 - 20.0, 56.0, Color::from_rgba(255, 90, 90, 255));
            draw_text("ENTER para jogar de novo", W / 2.0 - 120.0, H / 2.0 + 25.0, 26.0, WHITE);
        }
        next_frame().await;
    }
}
