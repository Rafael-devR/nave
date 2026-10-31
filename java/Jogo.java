// Nave - Java + Swing (já vem no JDK, não precisa instalar nada).
// Rode com:  javac Jogo.java && java Jogo
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;
import javax.swing.*;

public class Jogo extends JPanel implements ActionListener {
    static final int W = 480, H = 640;
    final Random rng = new Random();
    final Set<Integer> teclas = new HashSet<>();
    final float[][] estrelas = new float[60][3];
    List<float[]> balas = new ArrayList<>(), inimigos = new ArrayList<>();
    float x = W / 2f, recarga = 0, spawn = 0.5f;
    int pontos = 0, vidas = 3, recorde = 0;
    long antes = System.nanoTime();

    Jogo() {
        setPreferredSize(new Dimension(W, H));
        setFocusable(true);
        for (float[] s : estrelas) { s[0] = rng.nextFloat() * W; s[1] = rng.nextFloat() * H; s[2] = 20 + rng.nextFloat() * 70; }
        addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                teclas.add(e.getKeyCode());
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) System.exit(0);
                if (e.getKeyCode() == KeyEvent.VK_ENTER && vidas <= 0) reiniciar();
            }
            public void keyReleased(KeyEvent e) { teclas.remove(e.getKeyCode()); }
        });
        new javax.swing.Timer(16, this).start();
    }

    void reiniciar() {
        balas.clear(); inimigos.clear();
        x = W / 2f; recarga = 0; spawn = 0.5f; pontos = 0; vidas = 3;
    }

    boolean tecla(int a, int b) { return teclas.contains(a) || teclas.contains(b); }

    static boolean bate(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
        return Math.abs(ax - bx) < (aw + bw) / 2 && Math.abs(ay - by) < (ah + bh) / 2;
    }

    public void actionPerformed(ActionEvent ev) {
        long agora = System.nanoTime();
        float dt = Math.min(0.05f, (agora - antes) / 1e9f);
        antes = agora;
        atualizar(dt);
        repaint();
    }

    void atualizar(float dt) {
        for (float[] s : estrelas) s[1] = (s[1] + s[2] * dt) % H;
        if (vidas <= 0) return;

        int dir = (tecla(KeyEvent.VK_RIGHT, KeyEvent.VK_D) ? 1 : 0) - (tecla(KeyEvent.VK_LEFT, KeyEvent.VK_A) ? 1 : 0);
        x = Math.max(20, Math.min(W - 20, x + dir * 320 * dt));

        recarga -= dt;
        if (teclas.contains(KeyEvent.VK_SPACE) && recarga <= 0) { balas.add(new float[]{x, H - 80}); recarga = 0.2f; }

        spawn -= dt;
        if (spawn <= 0) {
            inimigos.add(new float[]{20 + rng.nextFloat() * (W - 40), -20});
            spawn = Math.max(0.3f, 0.9f - pontos * 0.002f);
        }

        float vel = Math.min(280, 100 + pontos * 0.6f);
        for (float[] b : balas) b[1] -= 560 * dt;
        for (float[] i : inimigos) i[1] += vel * dt;

        List<float[]> vivos = new ArrayList<>();
        for (float[] i : inimigos) {
            float[] tiro = null;
            for (float[] b : balas) if (bate(i[0], i[1], 30, 30, b[0], b[1], 6, 14)) { tiro = b; break; }
            if (tiro != null) { balas.remove(tiro); pontos += 10; }
            else if (bate(i[0], i[1], 30, 30, x, H - 60, 36, 30) || i[1] > H + 20) vidas--;
            else vivos.add(i);
        }
        inimigos = vivos;
        balas.removeIf(b -> b[1] < -20);
        recorde = Math.max(recorde, pontos);
    }

    @Override protected void paintComponent(Graphics g0) {
        Graphics2D g = (Graphics2D) g0;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.setColor(new Color(10, 10, 30));
        g.fillRect(0, 0, W, H);
        for (float[] s : estrelas) { int c = (int) (80 + s[2]); g.setColor(new Color(c, c, c)); g.fillRect((int) s[0], (int) s[1], 2, 2); }
        g.setColor(new Color(255, 230, 80));
        for (float[] b : balas) g.fillRect((int) b[0] - 3, (int) b[1] - 7, 6, 14);
        for (float[] i : inimigos) {
            g.setColor(new Color(230, 60, 70)); g.fillRect((int) i[0] - 15, (int) i[1] - 15, 30, 30);
            g.setColor(new Color(255, 220, 100));
            g.fillRect((int) i[0] - 9, (int) i[1] - 5, 6, 6); g.fillRect((int) i[0] + 3, (int) i[1] - 5, 6, 6);
        }
        if (vidas > 0) {
            g.setColor(new Color(80, 220, 255));
            g.fillPolygon(new int[]{(int) x, (int) x - 18, (int) x + 18}, new int[]{H - 78, H - 46, H - 46}, 3);
        }
        g.setFont(new Font(Font.SANS_SERIF, Font.PLAIN, 18));
        g.setColor(Color.WHITE);
        g.drawString("Pontos: " + pontos + "   Vidas: " + Math.max(0, vidas), 10, 24);
        g.setColor(new Color(170, 170, 200));
        g.drawString("Recorde: " + recorde, W - 130, 24);
        if (vidas <= 0) {
            g.setFont(new Font(Font.SANS_SERIF, Font.BOLD, 46));
            centro(g, "FIM DE JOGO", H / 2 - 20, new Color(255, 90, 90));
            g.setFont(new Font(Font.SANS_SERIF, Font.PLAIN, 20));
            centro(g, "ENTER para jogar de novo", H / 2 + 25, Color.WHITE);
        }
        Toolkit.getDefaultToolkit().sync();
    }

    void centro(Graphics2D g, String t, int y, Color c) {
        g.setColor(c);
        g.drawString(t, (W - g.getFontMetrics().stringWidth(t)) / 2, y);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            JFrame f = new JFrame("Nave - Java");
            Jogo j = new Jogo();
            f.add(j);
            f.pack();
            f.setResizable(false);
            f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            f.setLocationRelativeTo(null);
            f.setVisible(true);
            j.requestFocusInWindow();
        });
    }
}
