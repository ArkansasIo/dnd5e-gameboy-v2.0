function randInt(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function rollDie(sides) {
  return randInt(1, sides);
}

function rollDice(count, sides) {
  const rolls = [];
  for (let i = 0; i < count; i++) rolls.push(rollDie(sides));
  return { rolls, total: rolls.reduce((a, b) => a + b, 0) };
}

function parseDice(expr) {
  const m = String(expr).trim().match(/^(\d+)d(\d+)([+-]\d+)?$/i);
  if (!m) throw new Error(`Invalid dice expression: ${expr}`);
  return {
    count: Number(m[1]),
    sides: Number(m[2]),
    bonus: m[3] ? Number(m[3]) : 0
  };
}

function roll(expr) {
  const p = parseDice(expr);
  const r = rollDice(p.count, p.sides);
  return { ...r, bonus: p.bonus, expression: expr, total: r.total + p.bonus };
}

function d20({ advantage = false, disadvantage = false } = {}) {
  const a = rollDie(20);
  const b = (advantage || disadvantage) ? rollDie(20) : null;
  let total = a;
  if (advantage && b !== null) total = Math.max(a, b);
  if (disadvantage && b !== null) total = Math.min(a, b);
  return { a, b, total, advantage, disadvantage };
}

module.exports = { randInt, rollDie, rollDice, parseDice, roll, d20 };
