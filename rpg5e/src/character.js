const { roll, d20 } = require('./dice');
const { ABILITIES, CLASSES, abilityMod, proficiencyBonus, levelFromXP } = require('./rules');

class Character {
  constructor({ name, className, level = 1, xp = 0, abilities, ac = 10, maxHP }) {
    if (!CLASSES[className]) throw new Error(`Unknown class: ${className}`);
    this.name = name;
    this.className = className;
    this.level = level;
    this.xp = xp;
    this.abilities = { ...abilities };
    for (const a of ABILITIES) if (typeof this.abilities[a] !== 'number') this.abilities[a] = 10;
    this.ac = ac;
    this.maxHP = maxHP || this.estimateMaxHP();
    this.hp = this.maxHP;
    this.conditions = new Set();
    this.inventory = [];
    this.equipment = { armor: null, shield: null, weapon: null };
    this.features = [];
    this.spellSlots = this.defaultSpellSlots();
    this.usedSpellSlots = {};
  }

  estimateMaxHP() {
    const hd = CLASSES[this.className].hitDie;
    const con = abilityMod(this.abilities.CON);
    const first = hd + con;
    const perLevel = Math.ceil((hd + 1) / 2) + con;
    return Math.max(1, first + Math.max(0, this.level - 1) * perLevel);
  }

  mod(ability) { return abilityMod(this.abilities[ability]); }
  prof() { return proficiencyBonus(this.level); }
  passive(ability = 'WIS') { return 10 + this.mod(ability); }

  check(ability, { proficient = false, advantage = false, disadvantage = false, bonus = 0 } = {}) {
    const r = d20({ advantage, disadvantage });
    const total = r.total + this.mod(ability) + (proficient ? this.prof() : 0) + bonus;
    return { type: 'check', ability, ...r, total };
  }

  save(ability, { proficient = false, dc = 10, advantage = false, disadvantage = false, bonus = 0 } = {}) {
    const r = this.check(ability, { proficient, advantage, disadvantage, bonus });
    return { ...r, dc, success: r.total >= dc };
  }

  attack(target, {
    attackAbility = 'STR',
    proficient = true,
    attackBonus = 0,
    damage = '1d6',
    damageAbility = attackAbility,
    damageBonus = 0,
    advantage = false,
    disadvantage = false
  } = {}) {
    const hitRoll = d20({ advantage, disadvantage });
    const toHit = hitRoll.total + this.mod(attackAbility) + (proficient ? this.prof() : 0) + attackBonus;
    const hit = toHit >= target.ac || hitRoll.total === 20;
    let damageOut = null;
    if (hit) {
      const base = roll(damage);
      damageOut = Math.max(1, base.total + this.mod(damageAbility) + damageBonus);
      target.takeDamage(damageOut);
    }
    return { attacker: this.name, target: target.name, hitRoll, toHit, targetAC: target.ac, hit, damage: damageOut };
  }

  addCondition(name) { this.conditions.add(name); }
  removeCondition(name) { this.conditions.delete(name); }
  hasCondition(name) { return this.conditions.has(name); }

  takeDamage(amount) {
    this.hp = Math.max(0, this.hp - Math.max(0, amount));
    return this.hp;
  }

  heal(amount) {
    this.hp = Math.min(this.maxHP, this.hp + Math.max(0, amount));
    return this.hp;
  }

  gainXP(amount) {
    this.xp += Math.max(0, amount);
    const nextLevel = levelFromXP(this.xp);
    if (nextLevel > this.level) {
      const old = this.level;
      this.level = nextLevel;
      const hpGain = (nextLevel - old) * (Math.ceil((CLASSES[this.className].hitDie + 1) / 2) + this.mod('CON'));
      this.maxHP += Math.max(1, hpGain);
      this.hp = this.maxHP;
    }
    return this.level;
  }

  restShort() {
    const hd = CLASSES[this.className].hitDie;
    const healed = roll(`1d${hd}`).total + this.mod('CON');
    this.heal(healed);
    return healed;
  }

  restLong() {
    this.hp = this.maxHP;
    this.usedSpellSlots = {};
  }

  defaultSpellSlots() {
    const byClass = {
      Bard: { 1: 2 }, Cleric: { 1: 2 }, Druid: { 1: 2 }, Sorcerer: { 1: 2 }, Wizard: { 1: 2 },
      Paladin: { 1: 0 }, Ranger: { 1: 0 }, Artificer: { 1: 0 }, Warlock: { 1: 1 }
    };
    return byClass[this.className] || {};
  }
}

module.exports = { Character };
