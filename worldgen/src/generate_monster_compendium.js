const fs = require('fs');
const path = require('path');

const OUTPUT = path.resolve(__dirname, '..', 'generated', 'monster_compendium.json');
const MATH_OUTPUT = path.resolve(__dirname, '..', 'generated', 'monster_math_reference_5e.json');

const typeFamilies = {
  Aberration: ['Beholder', 'Mind Flayer', 'Aboleth', 'Gibbering Mouther', 'Carrion Crawler', 'Star Spawn'],
  Beast: ['Wolf', 'Bear', 'Boar', 'Panther', 'Giant Spider', 'Crocodile'],
  Celestial: ['Deva', 'Planetar', 'Solar', 'Ki-rin', 'Couatl', 'Pegasus'],
  Construct: ['Animated Armor', 'Helmed Horror', 'Shield Guardian', 'Golem', 'Modron', 'Scarecrow'],
  Dragon: ['Black Dragon', 'Blue Dragon', 'Green Dragon', 'Red Dragon', 'White Dragon', 'Brass Dragon'],
  Elemental: ['Air Elemental', 'Earth Elemental', 'Fire Elemental', 'Water Elemental', 'Myrmidon', 'Mephit'],
  Fey: ['Dryad', 'Satyr', 'Quickling', 'Hag', 'Yeth Hound', 'Redcap'],
  Fiend: ['Imp', 'Barlgura', 'Bone Devil', 'Pit Fiend', 'Balor', 'Yugoloth'],
  Giant: ['Hill Giant', 'Stone Giant', 'Frost Giant', 'Fire Giant', 'Cloud Giant', 'Storm Giant'],
  Humanoid: ['Goblin', 'Hobgoblin', 'Orc', 'Kobold', 'Bandit', 'Cultist'],
  Monstrosity: ['Owlbear', 'Basilisk', 'Chimera', 'Manticore', 'Wyvern', 'Hydra'],
  Ooze: ['Gray Ooze', 'Ochre Jelly', 'Black Pudding', 'Gelatinous Cube', 'Slithering Tracker', 'Oblex'],
  Plant: ['Shambling Mound', 'Treant', 'Vine Blight', 'Needle Blight', 'Gas Spore', 'Myconid'],
  Undead: ['Skeleton', 'Zombie', 'Ghoul', 'Wight', 'Vampire', 'Lich']
};

const subtypeMap = {
  Aberration: ['Psionic', 'Far Realm', 'Mutant'],
  Beast: ['Land', 'Aerial', 'Aquatic'],
  Celestial: ['Angel', 'Unicornian', 'Starborn'],
  Construct: ['Arcane', 'Clockwork', 'Guardian'],
  Dragon: ['Chromatic', 'Metallic', 'Gem'],
  Elemental: ['Air', 'Earth', 'Fire', 'Water'],
  Fey: ['Court', 'Wild', 'Shadow'],
  Fiend: ['Demon', 'Devil', 'Yugoloth'],
  Giant: ['True Giant', 'Kin', 'Titanic'],
  Humanoid: ['Civilized', 'Tribal', 'Cultic'],
  Monstrosity: ['Hybrid', 'Predator', 'Arcane-Bred'],
  Ooze: ['Acidic', 'Adaptive', 'Colony'],
  Plant: ['Fungal', 'Bramble', 'Ancient Growth'],
  Undead: ['Corporeal', 'Incorporeal', 'Necromantic']
};

const ranks = [
  { label: 'Minion', cr: '1/8' },
  { label: 'Scout', cr: '1/4' },
  { label: 'Warrior', cr: '1/2' },
  { label: 'Veteran', cr: '1' },
  { label: 'Champion', cr: '3' },
  { label: 'Elite', cr: '5' },
  { label: 'Mythic', cr: '8' },
  { label: 'Paragon', cr: '12' },
  { label: 'Dreadlord', cr: '16' },
  { label: 'Legend', cr: '20' }
];

const terrains = ['Forest', 'Swamp', 'Mountain', 'Desert', 'Underdark', 'Ruins', 'Tundra', 'Coast'];
const roles = ['Brute', 'Skirmisher', 'Controller', 'Caster', 'Support'];

const crMath = {
  '1/8': { crValue: 0.125, xp: 25, proficiencyBonus: 2, expectedAC: 13, expectedHP: 18, expectedDPR: 4, expectedAttackBonus: 3, expectedSaveDC: 13 },
  '1/4': { crValue: 0.25, xp: 50, proficiencyBonus: 2, expectedAC: 13, expectedHP: 24, expectedDPR: 6, expectedAttackBonus: 3, expectedSaveDC: 13 },
  '1/2': { crValue: 0.5, xp: 100, proficiencyBonus: 2, expectedAC: 13, expectedHP: 36, expectedDPR: 8, expectedAttackBonus: 3, expectedSaveDC: 13 },
  '1': { crValue: 1, xp: 200, proficiencyBonus: 2, expectedAC: 13, expectedHP: 50, expectedDPR: 12, expectedAttackBonus: 3, expectedSaveDC: 13 },
  '3': { crValue: 3, xp: 700, proficiencyBonus: 2, expectedAC: 13, expectedHP: 85, expectedDPR: 24, expectedAttackBonus: 4, expectedSaveDC: 13 },
  '5': { crValue: 5, xp: 1800, proficiencyBonus: 3, expectedAC: 15, expectedHP: 130, expectedDPR: 33, expectedAttackBonus: 6, expectedSaveDC: 15 },
  '8': { crValue: 8, xp: 3900, proficiencyBonus: 3, expectedAC: 16, expectedHP: 170, expectedDPR: 48, expectedAttackBonus: 7, expectedSaveDC: 16 },
  '12': { crValue: 12, xp: 8400, proficiencyBonus: 4, expectedAC: 17, expectedHP: 230, expectedDPR: 72, expectedAttackBonus: 8, expectedSaveDC: 17 },
  '16': { crValue: 16, xp: 15000, proficiencyBonus: 5, expectedAC: 18, expectedHP: 300, expectedDPR: 95, expectedAttackBonus: 9, expectedSaveDC: 18 },
  '20': { crValue: 20, xp: 25000, proficiencyBonus: 6, expectedAC: 19, expectedHP: 370, expectedDPR: 125, expectedAttackBonus: 10, expectedSaveDC: 19 }
};

const roleAbilityPriority = {
  Brute: ['STR', 'CON', 'DEX'],
  Skirmisher: ['DEX', 'CON', 'WIS'],
  Controller: ['INT', 'WIS', 'CON'],
  Caster: ['CHA', 'INT', 'CON'],
  Support: ['WIS', 'CHA', 'CON']
};

const typeHitDie = {
  Aberration: 8,
  Beast: 8,
  Celestial: 10,
  Construct: 10,
  Dragon: 12,
  Elemental: 10,
  Fey: 8,
  Fiend: 10,
  Giant: 12,
  Humanoid: 8,
  Monstrosity: 10,
  Ooze: 10,
  Plant: 10,
  Undead: 8
};

function toModifier(score) {
  return Math.floor((score - 10) / 2);
}

function tierFromCR(crValue) {
  if (crValue <= 1) return 0;
  if (crValue <= 5) return 1;
  if (crValue <= 12) return 2;
  return 3;
}

function makeAbilityScores(role, crValue, seed) {
  const tier = tierFromCR(crValue);
  const base = {
    STR: 10,
    DEX: 10,
    CON: 10,
    INT: 10,
    WIS: 10,
    CHA: 10
  };
  const priority = roleAbilityPriority[role];
  base[priority[0]] = 14 + tier * 3 + (seed % 2);
  base[priority[1]] = 12 + tier * 2;
  base[priority[2]] = 11 + tier;
  base.INT += tier === 3 ? 2 : 0;
  base.WIS += tier >= 2 ? 1 : 0;
  base.CHA += tier >= 1 ? 1 : 0;

  for (const k of Object.keys(base)) {
    if (base[k] > 30) base[k] = 30;
  }
  return base;
}

function formatDice(avgDamage, dieSize) {
  const count = Math.max(1, Math.round((avgDamage * 2) / (dieSize + 1)));
  const avg = Math.floor((count * (dieSize + 1)) / 2);
  return `${count}d${dieSize} + ${Math.max(0, avgDamage - avg)}`;
}

function makeCombatMath(monster, math, seed) {
  const abilities = makeAbilityScores(monster.role, math.crValue, seed);
  const mods = Object.fromEntries(Object.entries(abilities).map(([k, v]) => [k, toModifier(v)]));

  const primaryAbility = roleAbilityPriority[monster.role][0];
  const attackBonus = math.proficiencyBonus + mods[primaryAbility];
  const saveDC = 8 + math.proficiencyBonus + mods[primaryAbility];

  const dieSize = typeHitDie[monster.type];
  const hpTarget = math.expectedHP + (seed % 3) * 4;
  const hdCount = Math.max(1, Math.round(hpTarget / ((dieSize + 1) / 2 + mods.CON)));
  const hpAverage = hdCount * Math.floor((dieSize + 1) / 2) + hdCount * mods.CON;

  const multiattack = math.crValue < 1 ? 1 : math.crValue < 8 ? 2 : 3;
  const singleHitAvg = Math.max(2, Math.round(math.expectedDPR / multiattack));

  return {
    cr: monster.challengeRating,
    crValue: math.crValue,
    xp: math.xp,
    proficiencyBonus: math.proficiencyBonus,
    armorClass: Math.max(10, math.expectedAC + (mods.DEX > 2 ? 1 : 0)),
    hitPoints: {
      average: hpAverage,
      formula: `${hdCount}d${dieSize} + ${hdCount * mods.CON}`
    },
    speed: {
      walk: 30 + (monster.role === 'Skirmisher' ? 10 : 0),
      fly: monster.type === 'Dragon' || monster.type === 'Celestial' ? 60 : 0,
      swim: monster.type === 'Elemental' || monster.baseFamily.includes('Crocodile') ? 40 : 0
    },
    abilities,
    abilityModifiers: mods,
    savingThrows: {
      primary: `${primaryAbility} +${mods[primaryAbility] + math.proficiencyBonus}`,
      secondary: `${roleAbilityPriority[monster.role][1]} +${mods[roleAbilityPriority[monster.role][1]] + math.proficiencyBonus}`
    },
    offense: {
      role: monster.role,
      attackBonus,
      saveDC,
      multiattack,
      targetDamagePerRound: math.expectedDPR,
      mainAttack: {
        toHit: `+${attackBonus}`,
        damage: formatDice(singleHitAvg, dieSize),
        averageDamage: singleHitAvg,
        damageType: ['slashing', 'piercing', 'bludgeoning', 'fire', 'cold', 'necrotic'][seed % 6]
      }
    },
    senses: {
      passivePerception: 10 + Math.max(mods.WIS, 0),
      darkvision: monster.type === 'Humanoid' || monster.type === 'Beast' ? 0 : 60
    }
  };
}

function makeMonsterEntry(type, family, rank, seed) {
  const math = crMath[rank.cr];
  const role = roles[seed % roles.length];

  const entry = {
    name: `${family} ${rank.label}`,
    baseFamily: family,
    type,
    subtype: subtypeMap[type][seed % subtypeMap[type].length],
    challengeRating: rank.cr,
    role,
    habitats: [terrains[seed % terrains.length], terrains[(seed + 2) % terrains.length]],
    tags: ['dnd5e-style', 'generated', type.toLowerCase()]
  };

  entry.math5e = makeCombatMath(entry, math, seed);
  return entry;
}

function makeCompendium() {
  const monsters = [];
  let id = 1;

  for (const [type, families] of Object.entries(typeFamilies)) {
    for (let f = 0; f < families.length; f++) {
      const family = families[f];
      for (let r = 0; r < ranks.length; r++) {
        const rank = ranks[r];
        const monster = makeMonsterEntry(type, family, rank, id + f + r);
        monster.monsterId = `mx-${String(id).padStart(5, '0')}`;
        monsters.push(monster);
        id += 1;
      }
    }
  }

  return {
    metadata: {
      generatedAt: new Date().toISOString(),
      source: 'worldgen/src/generate_monster_compendium.js',
      style: 'DND5E-inspired',
      note: 'Generated 5e-style monster scaffold including CR math fields for game systems.'
    },
    totals: {
      types: Object.keys(typeFamilies).length,
      families: Object.values(typeFamilies).reduce((n, a) => n + a.length, 0),
      monsters: monsters.length
    },
    monsters
  };
}

function writeOutputs() {
  const out = makeCompendium();
  const mathRef = {
    metadata: {
      generatedAt: new Date().toISOString(),
      source: 'worldgen/src/generate_monster_compendium.js'
    },
    challengeRatingMath: crMath,
    roleAbilityPriority,
    typeHitDie,
    formulas: {
      abilityModifier: 'floor((abilityScore - 10) / 2)',
      saveDC: '8 + proficiencyBonus + primaryAbilityModifier',
      attackBonus: 'proficiencyBonus + primaryAbilityModifier',
      hitPointFormula: 'hitDiceCount d hitDie + (hitDiceCount * CON modifier)',
      damagePerRoundTarget: 'from CR table, split across multiattack count'
    }
  };

  fs.mkdirSync(path.dirname(OUTPUT), { recursive: true });
  fs.writeFileSync(OUTPUT, JSON.stringify(out, null, 2) + '\n', 'utf8');
  fs.writeFileSync(MATH_OUTPUT, JSON.stringify(mathRef, null, 2) + '\n', 'utf8');

  console.log(`Wrote ${out.totals.monsters} monsters to ${OUTPUT}`);
  console.log(`Wrote 5e math reference to ${MATH_OUTPUT}`);
}

writeOutputs();
