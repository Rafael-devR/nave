-- Nave - Lua + LÖVE (https://love2d.org). Rode com:  love lua   (ou arraste a pasta sobre love.exe)
local W, H = 480, 640
local jogo, estrelas, recorde = nil, {}, 0
local fonte, grande

local function novo()
  return { x = W / 2, balas = {}, inimigos = {}, pontos = 0, vidas = 3, recarga = 0, spawn = 0.5 }
end

local function bate(ax, ay, aw, ah, bx, by, bw, bh)
  return math.abs(ax - bx) < (aw + bw) / 2 and math.abs(ay - by) < (ah + bh) / 2
end

function love.load()
  math.randomseed(os.time())
  fonte = love.graphics.newFont(20)
  grande = love.graphics.newFont(48)
  for i = 1, 60 do
    estrelas[i] = { x = math.random(0, W), y = math.random(0, H), v = 20 + math.random() * 70 }
  end
  jogo = novo()
end

function love.keypressed(k)
  if k == "escape" then love.event.quit() end
  if k == "return" and jogo.vidas <= 0 then jogo = novo() end
end

function love.update(dt)
  for _, s in ipairs(estrelas) do s.y = (s.y + s.v * dt) % H end
  if jogo.vidas <= 0 then return end

  local kb = love.keyboard.isDown
  local dx = ((kb("right") or kb("d")) and 1 or 0) - ((kb("left") or kb("a")) and 1 or 0)
  jogo.x = math.max(20, math.min(W - 20, jogo.x + dx * 320 * dt))

  jogo.recarga = jogo.recarga - dt
  if kb("space") and jogo.recarga <= 0 then
    table.insert(jogo.balas, { x = jogo.x, y = H - 80 })
    jogo.recarga = 0.2
  end

  jogo.spawn = jogo.spawn - dt
  if jogo.spawn <= 0 then
    table.insert(jogo.inimigos, { x = math.random(20, W - 20), y = -20 })
    jogo.spawn = math.max(0.3, 0.9 - jogo.pontos * 0.002)
  end

  local vel = math.min(280, 100 + jogo.pontos * 0.6)
  for _, b in ipairs(jogo.balas) do b.y = b.y - 560 * dt end
  for _, i in ipairs(jogo.inimigos) do i.y = i.y + vel * dt end

  for ii = #jogo.inimigos, 1, -1 do
    local i = jogo.inimigos[ii]
    local morto = false
    for bi = #jogo.balas, 1, -1 do
      local b = jogo.balas[bi]
      if bate(i.x, i.y, 30, 30, b.x, b.y, 6, 14) then
        table.remove(jogo.balas, bi)
        jogo.pontos = jogo.pontos + 10
        morto = true
        break
      end
    end
    if morto then
      table.remove(jogo.inimigos, ii)
    elseif bate(i.x, i.y, 30, 30, jogo.x, H - 60, 36, 30) or i.y > H + 20 then
      table.remove(jogo.inimigos, ii)
      jogo.vidas = jogo.vidas - 1
    end
  end
  for bi = #jogo.balas, 1, -1 do
    if jogo.balas[bi].y < -20 then table.remove(jogo.balas, bi) end
  end
  recorde = math.max(recorde, jogo.pontos)
end

function love.draw()
  love.graphics.clear(10 / 255, 10 / 255, 30 / 255)
  for _, s in ipairs(estrelas) do
    local c = (80 + s.v) / 255
    love.graphics.setColor(c, c, c)
    love.graphics.rectangle("fill", s.x, s.y, 2, 2)
  end
  love.graphics.setColor(1, 0.9, 0.3)
  for _, b in ipairs(jogo.balas) do love.graphics.rectangle("fill", b.x - 3, b.y - 7, 6, 14) end
  for _, i in ipairs(jogo.inimigos) do
    love.graphics.setColor(0.9, 0.24, 0.27)
    love.graphics.rectangle("fill", i.x - 15, i.y - 15, 30, 30)
    love.graphics.setColor(1, 0.86, 0.4)
    love.graphics.rectangle("fill", i.x - 9, i.y - 5, 6, 6)
    love.graphics.rectangle("fill", i.x + 3, i.y - 5, 6, 6)
  end
  if jogo.vidas > 0 then
    love.graphics.setColor(0.31, 0.86, 1)
    love.graphics.polygon("fill", jogo.x, H - 78, jogo.x - 18, H - 46, jogo.x + 18, H - 46)
  end
  love.graphics.setColor(1, 1, 1)
  love.graphics.setFont(fonte)
  love.graphics.print("Pontos: " .. jogo.pontos .. "   Vidas: " .. math.max(0, jogo.vidas), 10, 10)
  love.graphics.setColor(0.67, 0.67, 0.78)
  love.graphics.print("Recorde: " .. recorde, W - 130, 10)
  if jogo.vidas <= 0 then
    love.graphics.setColor(1, 0.35, 0.35)
    love.graphics.setFont(grande)
    love.graphics.printf("FIM DE JOGO", 0, H / 2 - 60, W, "center")
    love.graphics.setColor(1, 1, 1)
    love.graphics.setFont(fonte)
    love.graphics.printf("ENTER para jogar de novo", 0, H / 2 + 10, W, "center")
  end
end
