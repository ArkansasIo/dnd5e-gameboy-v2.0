const fs = require('fs');
const path = require('path');

const OUTPUT = path.resolve(__dirname, '..', 'generated', 'player_classes_5e.json');

const proficiencyBonusByLevel = [
  2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6
];

function levels(maxLevel = 20) {
  return Array.from({ length: maxLevel }, (_, i) => i + 1);
}

function classEntry(data) {
  return {
    ...data,
    progression: {
      levels: levels(),
      proficiencyBonusByLevel
    }
  };
}

const classes = [
  classEntry({
    class: 'Artificer',
    hitDie: 'd8',
    primaryAbilities: ['INT'],
    savingThrows: ['CON', 'INT'],
    armorProficiencies: ['Light', 'Medium', 'Shields'],
    weaponProficiencies: ['Simple'],
    toolProficiencies: ['Thieves\' Tools', 'Tinker\'s Tools', 'Artisan\'s Tools'],
    spellcasting: { ability: 'INT', type: 'Half Caster' },
    subclasses: ['Alchemist', 'Armorer', 'Artillerist', 'Battle Smith']
  }),
  classEntry({
    class: 'Barbarian',
    hitDie: 'd12',
    primaryAbilities: ['STR', 'CON'],
    savingThrows: ['STR', 'CON'],
    armorProficiencies: ['Light', 'Medium', 'Shields'],
    weaponProficiencies: ['Simple', 'Martial'],
    toolProficiencies: [],
    spellcasting: null,
    subclasses: ['Path of the Berserker', 'Path of the Totem Warrior', 'Path of the Zealot', 'Path of the Ancestral Guardian']
  }),
  classEntry({
    class: 'Bard',
    hitDie: 'd8',
    primaryAbilities: ['CHA'],
    savingThrows: ['DEX', 'CHA'],
    armorProficiencies: ['Light'],
    weaponProficiencies: ['Simple', 'Hand Crossbows', 'Longswords', 'Rapiers', 'Shortswords'],
    toolProficiencies: ['Musical Instruments'],
    spellcasting: { ability: 'CHA', type: 'Full Caster' },
    subclasses: ['College of Lore', 'College of Valor', 'College of Glamour', 'College of Swords', 'College of Whispers']
  }),
  classEntry({
    class: 'Cleric',
    hitDie: 'd8',
    primaryAbilities: ['WIS'],
    savingThrows: ['WIS', 'CHA'],
    armorProficiencies: ['Light', 'Medium', 'Shields'],
    weaponProficiencies: ['Simple'],
    toolProficiencies: [],
    spellcasting: { ability: 'WIS', type: 'Full Caster' },
    subclasses: ['Life Domain', 'Light Domain', 'War Domain', 'Trickery Domain', 'Tempest Domain', 'Grave Domain']
  }),
  classEntry({
    class: 'Druid',
    hitDie: 'd8',
    primaryAbilities: ['WIS'],
    savingThrows: ['INT', 'WIS'],
    armorProficiencies: ['Light', 'Medium', 'Shields (non-metal)'],
    weaponProficiencies: ['Clubs', 'Daggers', 'Darts', 'Javelins', 'Maces', 'Quarterstaffs', 'Scimitars', 'Sickles', 'Slings', 'Spears'],
    toolProficiencies: ['Herbalism Kit'],
    spellcasting: { ability: 'WIS', type: 'Full Caster' },
    subclasses: ['Circle of the Land', 'Circle of the Moon', 'Circle of Stars', 'Circle of Spores', 'Circle of the Shepherd']
  }),
  classEntry({
    class: 'Fighter',
    hitDie: 'd10',
    primaryAbilities: ['STR', 'DEX'],
    savingThrows: ['STR', 'CON'],
    armorProficiencies: ['All Armor', 'Shields'],
    weaponProficiencies: ['Simple', 'Martial'],
    toolProficiencies: [],
    spellcasting: null,
    subclasses: ['Champion', 'Battle Master', 'Eldritch Knight', 'Cavalier', 'Samurai']
  }),
  classEntry({
    class: 'Monk',
    hitDie: 'd8',
    primaryAbilities: ['DEX', 'WIS'],
    savingThrows: ['STR', 'DEX'],
    armorProficiencies: [],
    weaponProficiencies: ['Simple', 'Shortswords'],
    toolProficiencies: ['Artisan\'s Tools or Musical Instrument'],
    spellcasting: null,
    subclasses: ['Way of the Open Hand', 'Way of Shadow', 'Way of the Four Elements', 'Way of Mercy', 'Way of the Kensei']
  }),
  classEntry({
    class: 'Paladin',
    hitDie: 'd10',
    primaryAbilities: ['STR', 'CHA'],
    savingThrows: ['WIS', 'CHA'],
    armorProficiencies: ['All Armor', 'Shields'],
    weaponProficiencies: ['Simple', 'Martial'],
    toolProficiencies: [],
    spellcasting: { ability: 'CHA', type: 'Half Caster' },
    subclasses: ['Oath of Devotion', 'Oath of the Ancients', 'Oath of Vengeance', 'Oath of Conquest', 'Oath of Redemption']
  }),
  classEntry({
    class: 'Ranger',
    hitDie: 'd10',
    primaryAbilities: ['DEX', 'WIS'],
    savingThrows: ['STR', 'DEX'],
    armorProficiencies: ['Light', 'Medium', 'Shields'],
    weaponProficiencies: ['Simple', 'Martial'],
    toolProficiencies: [],
    spellcasting: { ability: 'WIS', type: 'Half Caster' },
    subclasses: ['Hunter', 'Beast Master', 'Gloom Stalker', 'Fey Wanderer', 'Swarmkeeper']
  }),
  classEntry({
    class: 'Rogue',
    hitDie: 'd8',
    primaryAbilities: ['DEX'],
    savingThrows: ['DEX', 'INT'],
    armorProficiencies: ['Light'],
    weaponProficiencies: ['Simple', 'Hand Crossbows', 'Longswords', 'Rapiers', 'Shortswords'],
    toolProficiencies: ['Thieves\' Tools'],
    spellcasting: null,
    subclasses: ['Thief', 'Assassin', 'Arcane Trickster', 'Swashbuckler', 'Soulknife']
  }),
  classEntry({
    class: 'Sorcerer',
    hitDie: 'd6',
    primaryAbilities: ['CHA'],
    savingThrows: ['CON', 'CHA'],
    armorProficiencies: [],
    weaponProficiencies: ['Daggers', 'Darts', 'Slings', 'Quarterstaffs', 'Light Crossbows'],
    toolProficiencies: [],
    spellcasting: { ability: 'CHA', type: 'Full Caster' },
    subclasses: ['Draconic Bloodline', 'Wild Magic', 'Shadow Magic', 'Divine Soul', 'Aberrant Mind']
  }),
  classEntry({
    class: 'Warlock',
    hitDie: 'd8',
    primaryAbilities: ['CHA'],
    savingThrows: ['WIS', 'CHA'],
    armorProficiencies: ['Light'],
    weaponProficiencies: ['Simple'],
    toolProficiencies: [],
    spellcasting: { ability: 'CHA', type: 'Pact Magic' },
    subclasses: ['The Fiend', 'The Archfey', 'The Great Old One', 'The Hexblade', 'The Genie', 'The Celestial']
  }),
  classEntry({
    class: 'Wizard',
    hitDie: 'd6',
    primaryAbilities: ['INT'],
    savingThrows: ['INT', 'WIS'],
    armorProficiencies: [],
    weaponProficiencies: ['Daggers', 'Darts', 'Slings', 'Quarterstaffs', 'Light Crossbows'],
    toolProficiencies: [],
    spellcasting: { ability: 'INT', type: 'Full Caster' },
    subclasses: ['School of Abjuration', 'School of Evocation', 'School of Divination', 'School of Necromancy', 'Bladesinging', 'Chronurgy Magic']
  })
];

const out = {
  metadata: {
    generatedAt: new Date().toISOString(),
    source: 'worldgen/src/generate_player_classes_5e.js',
    style: 'DND5E-inspired',
    note: 'All base 5e player classes with class fundamentals and subclass lists.'
  },
  totals: {
    classes: classes.length,
    subclasses: classes.reduce((n, c) => n + c.subclasses.length, 0)
  },
  classes
};

fs.mkdirSync(path.dirname(OUTPUT), { recursive: true });
fs.writeFileSync(OUTPUT, JSON.stringify(out, null, 2) + '\n', 'utf8');
console.log(`Wrote ${out.totals.classes} classes and ${out.totals.subclasses} subclasses to ${OUTPUT}`);
