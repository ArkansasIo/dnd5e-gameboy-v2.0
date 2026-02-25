const fs = require('fs');
const path = require('path');

const OUT_JSON = path.resolve(__dirname, '..', 'generated', 'elden_dnd5e_compendium.json');
const OUT_MD = path.resolve(__dirname, '..', 'generated', 'elden_dnd5e_compendium_guide.md');

const rarities = ['Common', 'Uncommon', 'Rare', 'Very Rare', 'Legendary'];
const damageTypes = ['Slashing', 'Piercing', 'Bludgeoning', 'Fire', 'Cold', 'Lightning', 'Necrotic', 'Radiant', 'Force'];
const statusEffects = ['Bleed', 'Frostbite', 'Scarlet Rot', 'Poison', 'Madness', 'Sleep', 'Blight'];

const prefixes = ['Ashen', 'Gloam', 'Storm', 'Blood', 'Moon', 'Sun', 'Ruin', 'Dusk', 'Dawn', 'Iron', 'Gold', 'Night'];
const suffixes = ['of Cinders', 'of Ruin', 'of the Fell Star', 'of the Erdtree', 'of Dread', 'of Dawnfire', 'of Shadow', 'of the Deep'];

const weaponArchetypes = [
  { type: 'Dagger', baseDie: '1d4', properties: ['Finesse', 'Light', 'Thrown (20/60)'] },
  { type: 'Straight Sword', baseDie: '1d8', properties: ['Versatile (1d10)'] },
  { type: 'Greatsword', baseDie: '2d6', properties: ['Heavy', 'Two-Handed'] },
  { type: 'Colossal Sword', baseDie: '2d8', properties: ['Heavy', 'Two-Handed', 'Slow'] },
  { type: 'Katana', baseDie: '1d8', properties: ['Versatile (1d10)', 'Bleed'] },
  { type: 'Curved Sword', baseDie: '1d6', properties: ['Finesse'] },
  { type: 'Curved Greatsword', baseDie: '2d6', properties: ['Heavy', 'Two-Handed'] },
  { type: 'Rapier', baseDie: '1d8', properties: ['Finesse'] },
  { type: 'Axe', baseDie: '1d8', properties: ['Versatile (1d10)'] },
  { type: 'Greataxe', baseDie: '1d12', properties: ['Heavy', 'Two-Handed'] },
  { type: 'Hammer', baseDie: '1d8', properties: ['Versatile (1d10)'] },
  { type: 'Great Hammer', baseDie: '2d6', properties: ['Heavy', 'Two-Handed'] },
  { type: 'Spear', baseDie: '1d6', properties: ['Thrown (20/60)', 'Versatile (1d8)'] },
  { type: 'Great Spear', baseDie: '1d10', properties: ['Heavy', 'Reach'] },
  { type: 'Halberd', baseDie: '1d10', properties: ['Heavy', 'Reach', 'Two-Handed'] },
  { type: 'Reaper', baseDie: '2d4', properties: ['Heavy', 'Reach', 'Two-Handed'] },
  { type: 'Twinblade', baseDie: '1d8', properties: ['Two-Handed', 'Flurry'] },
  { type: 'Whip', baseDie: '1d6', properties: ['Finesse', 'Reach'] },
  { type: 'Claw', baseDie: '1d6', properties: ['Light', 'Finesse', 'Dual'] },
  { type: 'Fist', baseDie: '1d6', properties: ['Light', 'Dual'] },
  { type: 'Bow', baseDie: '1d8', properties: ['Ammunition (150/600)', 'Two-Handed'] },
  { type: 'Greatbow', baseDie: '1d10', properties: ['Heavy', 'Ammunition (200/800)', 'Two-Handed'] },
  { type: 'Crossbow', baseDie: '1d8', properties: ['Loading', 'Ammunition (100/400)'] },
  { type: 'Staff', baseDie: '1d6', properties: ['Arcane Focus'] },
  { type: 'Sacred Seal', baseDie: '1d4', properties: ['Divine Focus'] }
];

const infusions = [
  { name: 'Standard', bonus: 0, addType: null },
  { name: 'Heavy', bonus: 1, addType: null },
  { name: 'Keen', bonus: 1, addType: null },
  { name: 'Quality', bonus: 1, addType: null },
  { name: 'Fire', bonus: 1, addType: 'Fire' },
  { name: 'Lightning', bonus: 1, addType: 'Lightning' },
  { name: 'Frost', bonus: 1, addType: 'Cold' },
  { name: 'Sacred', bonus: 1, addType: 'Radiant' }
];

const armorSets = [
  'Banished Knight', 'Raging Wolf', 'Briar', 'Carian Knight', 'Radahn', 'Malformed Dragon',
  'Godskin Noble', 'Black Knife', 'Veteran', 'Cleanrot', 'Royal Remains', 'Bull-Goat',
  'Spellblade', 'Fingerprint', 'Night Cavalry', 'Mausoleum Knight', 'Tree Sentinel', 'Hoslow',
  'Twinned', 'Fire Prelate', 'Blaidd', 'Ranni\'s Snow Witch'
];

const itemKinds = {
  consumables: ['Flask Fragment', 'Rune Arc', 'Warming Stone', 'Bolstering Meat', 'Preserving Bolus', 'Sleep Pot', 'Fire Pot', 'Lightning Grease', 'Freezing Grease', 'Holyproof Dried Liver'],
  talismans: ['Warrior Crest', 'Dragoncrest Sigil', 'Shard of Valor', 'Green Turtle Sigil', 'Erdtree Favor', 'Claw Sigil', 'Ritual Shield Sigil', 'Magic Scorpion Sigil'],
  crafting: ['Smoldering Butterfly', 'Flight Pinion', 'Crystal Bud', 'Root Resin', 'Faded Erdleaf Flower', 'Sanguine Sliver', 'Ancient Bone', 'Grave Violet'],
  sorceries: ['Glintstone Pebble', 'Comet', 'Carian Slicer', 'Loretta\'s Bow', 'Stars of Ruin', 'Night Comet', 'Adula\'s Moonblade'],
  incantations: ['Heal', 'Lightning Spear', 'Black Flame', 'Flame Grant Me Strength', 'Golden Vow', 'Bestial Sling', 'Dragon Maw'],
  ashesOfWar: ['Lion\'s Claw', 'Bloody Slash', 'Flaming Strike', 'Thunderbolt', 'Hoarfrost Stomp', 'Sacred Blade', 'Unsheathe', 'Quickstep'],
  rings: [
    'Arsenal Charm', 'Arsenal Charm +1', 'Great-Jar\'s Arsenal', 'Erdtree\'s Favor', 'Erdtree\'s Favor +1', 'Erdtree\'s Favor +2',
    'Green Turtle Talisman', 'Crimson Amber Medallion', 'Crimson Amber Medallion +1', 'Crimson Amber Medallion +2',
    'Cerulean Amber Medallion', 'Cerulean Amber Medallion +1', 'Cerulean Amber Medallion +2',
    'Viridian Amber Medallion', 'Viridian Amber Medallion +1', 'Viridian Amber Medallion +2',
    'Radagon\'s Scarseal', 'Radagon\'s Soreseal', 'Marika\'s Scarseal', 'Marika\'s Soreseal',
    'Two Fingers Heirloom', 'Stargazer Heirloom', 'Prosthesis-Wearer Heirloom', 'Starscourge Heirloom',
    'Faithful\'s Canvas Talisman', 'Flock\'s Canvas Talisman', 'Graven-School Talisman', 'Graven-Mass Talisman',
    'Bull-Goat\'s Talisman', 'Blue Dancer Charm', 'Claw Talisman', 'Curved Sword Talisman', 'Axe Talisman',
    'Spear Talisman', 'Lance Talisman', 'Arrow\'s Reach Talisman', 'Arrow\'s Sting Talisman',
    'Dragoncrest Shield Talisman', 'Dragoncrest Shield Talisman +1', 'Dragoncrest Shield Talisman +2', 'Dragoncrest Greatshield Talisman',
    'Pearldrake Talisman', 'Pearldrake Talisman +1', 'Pearldrake Talisman +2',
    'Flamedrake Talisman', 'Flamedrake Talisman +1', 'Flamedrake Talisman +2',
    'Boltdrake Talisman', 'Boltdrake Talisman +1', 'Boltdrake Talisman +2',
    'Haligdrake Talisman', 'Haligdrake Talisman +1', 'Haligdrake Talisman +2',
    'Immunizing Horn Charm', 'Immunizing Horn Charm +1', 'Stalwart Horn Charm', 'Stalwart Horn Charm +1',
    'Clarifying Horn Charm', 'Clarifying Horn Charm +1', 'Mottled Necklace', 'Mottled Necklace +1',
    'Roar Medallion', 'Companion Jar', 'Ritual Sword Talisman', 'Ritual Shield Talisman',
    'Shard of Alexander', 'Warrior Jar Shard', 'Godfrey Icon', 'Carian Filigreed Crest',
    'Moon of Nokstella', 'Old Lord\'s Talisman', 'Primal Glintstone Blade', 'Ancestral Spirit\'s Horn',
    'Crepus\'s Vial', 'Concealing Veil', 'Assassin\'s Crimson Dagger', 'Assassin\'s Cerulean Dagger',
    'Lord of Blood\'s Exultation', 'Kindred of Rot\'s Exultation', 'Millicent\'s Prosthesis', 'Rotten Winged Sword Insignia',
    'Winged Sword Insignia', 'Magic Scorpion Charm', 'Fire Scorpion Charm', 'Lightning Scorpion Charm', 'Sacred Scorpion Charm',
    'Red-Feathered Branchsword', 'Blue-Feathered Branchsword', 'Blessed Dew Talisman', 'Taker\'s Cameo',
    'Daedicar\'s Woe', 'Shabriri\'s Woe', 'Gold Scarab', 'Silver Scarab'
  ]
};

function pick(arr, i) { return arr[i % arr.length]; }

function makeWeapons() {
  const weapons = [];
  let id = 1;
  for (let a = 0; a < weaponArchetypes.length; a++) {
    const arch = weaponArchetypes[a];
    for (let i = 0; i < infusions.length; i++) {
      const inf = infusions[i];
      const p = pick(prefixes, a + i);
      const s = pick(suffixes, a * 2 + i);
      weapons.push({
        id: `w-${String(id).padStart(4, '0')}`,
        name: `${p} ${arch.type} ${s}`,
        theme: 'Elden-inspired DND5E',
        archetype: arch.type,
        infusion: inf.name,
        rarity: pick(rarities, a + i),
        dnd5e: {
          weaponCategory: arch.type,
          damage: arch.baseDie,
          damageType: inf.addType || pick(damageTypes, a + i),
          attackBonus: inf.bonus,
          properties: arch.properties,
          scaling: {
            STR: arch.type.includes('Great') || arch.type.includes('Colossal') ? 'A' : 'B',
            DEX: arch.type.includes('Katana') || arch.type.includes('Rapier') ? 'A' : 'C',
            INT: inf.name === 'Frost' ? 'B' : 'D',
            FTH: inf.name === 'Sacred' ? 'B' : 'D',
            ARC: arch.type.includes('Claw') ? 'C' : 'D'
          },
          passive: arch.properties.includes('Bleed') ? ['Bleed (DC 13 CON)'] : []
        }
      });
      id += 1;
    }
  }
  return weapons;
}

function makeArmor() {
  const pieces = ['Helm', 'Chest', 'Gauntlets', 'Greaves'];
  const armors = [];
  let id = 1;

  for (let i = 0; i < armorSets.length; i++) {
    for (let p = 0; p < pieces.length; p++) {
      armors.push({
        id: `a-${String(id).padStart(4, '0')}`,
        name: `${armorSets[i]} ${pieces[p]}`,
        slot: pieces[p],
        rarity: pick(rarities, i + p),
        dnd5e: {
          acBonus: pieces[p] === 'Chest' ? 2 + (i % 3) : 1,
          resistances: [pick(damageTypes, i + p)],
          weightClass: i % 3 === 0 ? 'Light' : i % 3 === 1 ? 'Medium' : 'Heavy'
        }
      });
      id += 1;
    }
  }

  for (let s = 0; s < 24; s++) {
    armors.push({
      id: `a-${String(id).padStart(4, '0')}`,
      name: `${pick(prefixes, s)} Shield ${pick(suffixes, s)}`,
      slot: 'Shield',
      rarity: pick(rarities, s),
      dnd5e: {
        acBonus: 2 + (s % 3),
        blockValue: 50 + (s % 5) * 10,
        resistances: [pick(damageTypes, s + 2)]
      }
    });
    id += 1;
  }

  return armors;
}

function makeItems() {
  const items = [];
  let id = 1;

  function pushBatch(kind, names, count, effectPool) {
    for (let i = 0; i < count; i++) {
      const n = `${pick(names, i)} ${i > names.length - 1 ? `+${(i % 5) + 1}` : ''}`.trim();
      items.push({
        id: `it-${String(id).padStart(5, '0')}`,
        name: n,
        kind,
        rarity: pick(rarities, i),
        dnd5e: {
          actionType: kind === 'consumables' ? 'Action' : 'Passive',
          uses: kind === 'consumables' ? 1 : null,
          effect: pick(effectPool, i),
          suggestedValueGP: 25 + (i % 10) * 50
        }
      });
      id += 1;
    }
  }

  pushBatch('consumables', itemKinds.consumables, 60, ['Heal 2d4+2', 'Cure one status', 'Gain resistance 1 minute', 'Restore spell slot level 1']);
  pushBatch('talismans', itemKinds.talismans, 40, ['+1 attack rolls', '+1 AC', '+10 movement', 'Advantage on one save type']);
  pushBatch('crafting', itemKinds.crafting, 50, ['Crafting material', 'Upgrade catalyst', 'Infusion reagent']);
  pushBatch('sorcery', itemKinds.sorceries, 36, ['Spell attack', 'AOE burst', 'Projectile barrage']);
  pushBatch('incantation', itemKinds.incantations, 36, ['Heal/buff', 'Holy strike', 'Fire cone', 'Lightning bolt']);
  pushBatch('ashesOfWar', itemKinds.ashesOfWar, 24, ['Weapon skill unlock', 'Mobility skill', 'Elemental weapon art']);
  pushBatch('rings', itemKinds.rings, itemKinds.rings.length, ['Passive ring bonus', 'Conditional combat bonus', 'Defense or stamina modifier', 'Spell scaling bonus']);

  return items;
}

function makeEnemies() {
  const families = ['Soldier', 'Knight', 'Beast', 'Undead', 'Cultist', 'Sorcerer', 'Assassin', 'Construct', 'Dragonkin', 'Omen'];
  const enemies = [];
  let id = 1;

  for (let i = 0; i < 220; i++) {
    const fam = pick(families, i);
    const tier = 1 + (i % 10);
    enemies.push({
      id: `e-${String(id).padStart(4, '0')}`,
      name: `${pick(prefixes, i)} ${fam} ${pick(suffixes, i).replace('of ', '')}`,
      role: ['Brute', 'Skirmisher', 'Controller', 'Caster'][i % 4],
      biome: ['Castle', 'Catacomb', 'Field', 'Swamp', 'Mountain', 'Underdark'][i % 6],
      dnd5e: {
        challengeRating: String(Math.max(0.125, Math.min(16, tier / 2))),
        armorClass: 10 + tier,
        hitPoints: 15 + tier * 12,
        attackBonus: 2 + Math.floor(tier / 2),
        damagePerRound: 5 + tier * 4,
        resistances: i % 3 === 0 ? [pick(damageTypes, i + 1)] : [],
        statusInflict: i % 5 === 0 ? [pick(statusEffects, i)] : []
      }
    });
    id += 1;
  }

  return enemies;
}

function makeBosses() {
  const titles = ['Fell Lord', 'Shardbearer', 'Night Tyrant', 'Dragon Herald', 'Apostate Saint', 'Abyss Warden', 'Elden Pretender', 'Starborn Colossus'];
  const bosses = [];
  let id = 1;
  for (let i = 0; i < 72; i++) {
    const t = pick(titles, i);
    bosses.push({
      id: `b-${String(id).padStart(3, '0')}`,
      name: `${pick(prefixes, i)} ${t}`,
      phaseCount: 2 + (i % 2),
      domain: ['Divine', 'Abyssal', 'Arcane', 'Draconic', 'Blood'][i % 5],
      dnd5e: {
        challengeRating: String(8 + (i % 13)),
        armorClass: 16 + (i % 5),
        hitPoints: 180 + i * 6,
        legendaryActions: 3,
        lairActions: i % 2 === 0,
        signatureDamageType: pick(damageTypes, i + 3),
        signatureStatus: pick(statusEffects, i + 2)
      },
      loot: [
        `${pick(prefixes, i)} Remembrance`,
        `${pick(prefixes, i+1)} Great Rune Fragment`,
        `${pick(rewardPool(), i)}`
      ]
    });
    id += 1;
  }
  return bosses;
}

function rewardPool() {
  return ['Legendary Weapon Core', 'Ancient Smithing Relic', 'Sovereign Talisman', 'Erd Sigil Rune', 'Mythic Armor Weave'];
}

function buildCompendium() {
  const weapons = makeWeapons();
  const armors = makeArmor();
  const items = makeItems();
  const enemies = makeEnemies();
  const bosses = makeBosses();

  return {
    metadata: {
      generatedAt: new Date().toISOString(),
      source: 'worldgen/src/generate_elden_dnd5e_compendium.js',
      theme: 'Elden-inspired DND5E',
      note: 'Original inspired content for RPG use; names/stats are generated.'
    },
    totals: {
      weapons: weapons.length,
      armors: armors.length,
      items: items.length,
      enemies: enemies.length,
      bosses: bosses.length,
      allEntries: weapons.length + armors.length + items.length + enemies.length + bosses.length
    },
    weapons,
    armors,
    items,
    enemies,
    bosses
  };
}

function writeGuide(comp) {
  const lines = [];
  lines.push('# Elden-Inspired DND5E Compendium');
  lines.push('');
  lines.push(`- Weapons: ${comp.totals.weapons}`);
  lines.push(`- Armors: ${comp.totals.armors}`);
  lines.push(`- Items: ${comp.totals.items}`);
  lines.push(`- Enemies: ${comp.totals.enemies}`);
  lines.push(`- Bosses: ${comp.totals.bosses}`);
  lines.push(`- Total Entries: ${comp.totals.allEntries}`);
  lines.push('');
  lines.push('## Usage');
  lines.push('- Use entries as source data for loot tables, encounters, and progression tiers.');
  lines.push('- Boss CR and enemy CR are tuned for scalable chapter-based pacing.');
  lines.push('- Weapon and armor rows include DND5E-compatible stat fields.');
  return lines.join('\n') + '\n';
}

const comp = buildCompendium();
fs.mkdirSync(path.dirname(OUT_JSON), { recursive: true });
fs.writeFileSync(OUT_JSON, JSON.stringify(comp, null, 2) + '\n', 'utf8');
fs.writeFileSync(OUT_MD, writeGuide(comp), 'utf8');

console.log(`Wrote ${OUT_JSON}`);
console.log(`Wrote ${OUT_MD}`);
console.log(JSON.stringify(comp.totals, null, 2));
