const ABILITIES = ['STR', 'DEX', 'CON', 'INT', 'WIS', 'CHA'];

const PROF_BY_LEVEL = [
  2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6
];

const XP_BY_LEVEL = [
  0,300,900,2700,6500,14000,23000,34000,48000,64000,
  85000,100000,120000,140000,165000,195000,225000,265000,305000,355000
];

const CLASSES = {
  Artificer: { hitDie: 8, primary: ['INT'] },
  Barbarian: { hitDie: 12, primary: ['STR', 'CON'] },
  Bard: { hitDie: 8, primary: ['CHA'] },
  Cleric: { hitDie: 8, primary: ['WIS'] },
  Druid: { hitDie: 8, primary: ['WIS'] },
  Fighter: { hitDie: 10, primary: ['STR', 'DEX'] },
  Monk: { hitDie: 8, primary: ['DEX', 'WIS'] },
  Paladin: { hitDie: 10, primary: ['STR', 'CHA'] },
  Ranger: { hitDie: 10, primary: ['DEX', 'WIS'] },
  Rogue: { hitDie: 8, primary: ['DEX'] },
  Sorcerer: { hitDie: 6, primary: ['CHA'] },
  Warlock: { hitDie: 8, primary: ['CHA'] },
  Wizard: { hitDie: 6, primary: ['INT'] }
};

const CONDITIONS = {
  Blinded: { attackDisadvantage: true, attackersAdvantage: true },
  Charmed: { cannotHarmSource: true },
  Deafened: {},
  Frightened: { checksDisadvantageWhenSourceVisible: true },
  Grappled: { speed: 0 },
  Incapacitated: { actions: 0, reactions: 0 },
  Invisible: { attackAdvantage: true, attackersDisadvantage: true },
  Paralyzed: { speed: 0, actions: 0, autoFailStrDexSaves: true, critOnHitWithin5ft: true },
  Petrified: { speed: 0, actions: 0, resistanceAll: true },
  Poisoned: { attacksDisadvantage: true, checksDisadvantage: true },
  Prone: { meleeAttackersAdvantage: true, rangedAttackersDisadvantage: true },
  Restrained: { speed: 0, attacksDisadvantage: true, attackersAdvantage: true, dexSaveDisadvantage: true },
  Stunned: { speed: 0, actions: 0, strDexSaveFail: true, attackersAdvantage: true },
  Unconscious: { speed: 0, actions: 0, strDexSaveFail: true, attackersAdvantage: true, critOnHitWithin5ft: true }
};

function abilityMod(score) {
  return Math.floor((score - 10) / 2);
}

function proficiencyBonus(level) {
  return PROF_BY_LEVEL[Math.max(1, Math.min(20, level)) - 1];
}

function levelFromXP(xp) {
  let lvl = 1;
  for (let i = 0; i < XP_BY_LEVEL.length; i++) {
    if (xp >= XP_BY_LEVEL[i]) lvl = i + 1;
  }
  return Math.min(lvl, 20);
}

module.exports = {
  ABILITIES,
  CLASSES,
  CONDITIONS,
  PROF_BY_LEVEL,
  XP_BY_LEVEL,
  abilityMod,
  proficiencyBonus,
  levelFromXP
};
