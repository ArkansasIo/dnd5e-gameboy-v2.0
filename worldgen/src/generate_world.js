const fs = require('fs');
const path = require('path');

const WORLDGEN_DIR = path.resolve(__dirname, '..');
const OUTPUT_DIR = path.join(WORLDGEN_DIR, 'generated');
const LORE_DIR = path.join(OUTPUT_DIR, 'lore');

const biomeTaxonomy = [
  {
    class: 'Temperate',
    subclasses: [
      {
        subclass: 'Woodlands',
        types: [
          { type: 'Forest', subtypes: ['Deciduous', 'Mixed', 'Old-Growth'] },
          { type: 'Grassland', subtypes: ['Prairie', 'Steppe', 'Wildflower Meadow'] }
        ]
      },
      {
        subclass: 'Highlands',
        types: [
          { type: 'Hills', subtypes: ['Rolling', 'Rocky', 'Mistbound'] },
          { type: 'Mountain', subtypes: ['Alpine', 'Pinecrest', 'Frostcap'] }
        ]
      }
    ]
  },
  {
    class: 'Arid',
    subclasses: [
      {
        subclass: 'Drylands',
        types: [
          { type: 'Desert', subtypes: ['Dune Sea', 'Salt Flat', 'Red Sand'] },
          { type: 'Badlands', subtypes: ['Canyon', 'Mesa', 'Dust Basin'] }
        ]
      },
      {
        subclass: 'Savanna',
        types: [
          { type: 'Savanna', subtypes: ['Golden Grass', 'Acacia', 'Thunderplain'] }
        ]
      }
    ]
  },
  {
    class: 'Tropical',
    subclasses: [
      {
        subclass: 'Rain Regions',
        types: [
          { type: 'Rainforest', subtypes: ['Canopy Dense', 'River Jungle', 'Fog Jungle'] },
          { type: 'Wetland', subtypes: ['Mangrove', 'Fen', 'Swamp'] }
        ]
      }
    ]
  },
  {
    class: 'Cold',
    subclasses: [
      {
        subclass: 'Frozen',
        types: [
          { type: 'Tundra', subtypes: ['Permafrost', 'Lichen Plain', 'Ice Scrub'] },
          { type: 'Glacier', subtypes: ['Blue Ice', 'Crevasse Field', 'Shattered Shelf'] }
        ]
      }
    ]
  },
  {
    class: 'Subterranean',
    subclasses: [
      {
        subclass: 'Deep Earth',
        types: [
          { type: 'Cavern', subtypes: ['Limestone', 'Crystal', 'Magma Hollow'] },
          { type: 'Underdark', subtypes: ['Fungal Sea', 'Obsidian Deeps', 'Shadow Rift'] }
        ]
      }
    ]
  }
];

const races = [
  { race: 'Human', subtypes: ['Calishite', 'Chondathan', 'Damaran'] },
  { race: 'Elf', subtypes: ['High Elf', 'Wood Elf', 'Drow'] },
  { race: 'Dwarf', subtypes: ['Hill Dwarf', 'Mountain Dwarf', 'Duergar'] },
  { race: 'Halfling', subtypes: ['Lightfoot', 'Stout'] },
  { race: 'Gnome', subtypes: ['Forest Gnome', 'Rock Gnome', 'Deep Gnome'] },
  { race: 'Dragonborn', subtypes: ['Chromatic', 'Metallic', 'Gem'] },
  { race: 'Tiefling', subtypes: ['Infernal', 'Abyssal', 'Chthonic'] },
  { race: 'Half-Elf', subtypes: ['Moon-Touched', 'Sun-Touched'] },
  { race: 'Half-Orc', subtypes: ['Gray Orc Bloodline', 'Green Orc Bloodline'] },
  { race: 'Orc', subtypes: ['Mountain Orc', 'Plains Orc'] },
  { race: 'Aasimar', subtypes: ['Protector', 'Scourge', 'Fallen'] },
  { race: 'Goliath', subtypes: ['Cloud Clan', 'Stone Clan'] },
  { race: 'Tabaxi', subtypes: ['Jungle Stalker', 'Sand Runner'] },
  { race: 'Triton', subtypes: ['Abyss Guard', 'Wave Herald'] },
  { race: 'Genasi', subtypes: ['Air', 'Earth', 'Fire', 'Water'] }
];

const classes = [
  { class: 'Artificer', subclasses: ['Alchemist', 'Armorer', 'Artillerist', 'Battle Smith'] },
  { class: 'Barbarian', subclasses: ['Berserker', 'Totem Warrior', 'Zealot'] },
  { class: 'Bard', subclasses: ['Lore', 'Valor', 'Whispers'] },
  { class: 'Cleric', subclasses: ['Life', 'Light', 'War', 'Trickery'] },
  { class: 'Druid', subclasses: ['Land', 'Moon', 'Stars'] },
  { class: 'Fighter', subclasses: ['Champion', 'Battle Master', 'Eldritch Knight'] },
  { class: 'Monk', subclasses: ['Open Hand', 'Shadow', 'Kensei'] },
  { class: 'Paladin', subclasses: ['Devotion', 'Ancients', 'Vengeance'] },
  { class: 'Ranger', subclasses: ['Hunter', 'Beast Master', 'Gloom Stalker'] },
  { class: 'Rogue', subclasses: ['Thief', 'Assassin', 'Arcane Trickster'] },
  { class: 'Sorcerer', subclasses: ['Draconic', 'Wild Magic', 'Shadow Magic'] },
  { class: 'Warlock', subclasses: ['Fiend', 'Archfey', 'Great Old One'] },
  { class: 'Wizard', subclasses: ['Evocation', 'Abjuration', 'Necromancy'] }
];

const creatures = {
  monsters: [
    { name: 'Goblin Raider', type: 'Humanoid', subtype: 'Goblinoid', biomes: ['Forest', 'Badlands'] },
    { name: 'Hobgoblin Captain', type: 'Humanoid', subtype: 'Goblinoid', biomes: ['Hills', 'Mountain'] },
    { name: 'Orc Berserker', type: 'Humanoid', subtype: 'Orc', biomes: ['Mountain', 'Steppe'] },
    { name: 'Kobold Trapper', type: 'Humanoid', subtype: 'Reptilian', biomes: ['Cavern', 'Badlands'] },
    { name: 'Skeleton Legionnaire', type: 'Undead', subtype: 'Skeleton', biomes: ['Catacomb', 'Ruins'] },
    { name: 'Ghoul Stalker', type: 'Undead', subtype: 'Ghoul', biomes: ['Swamp', 'Catacomb'] },
    { name: 'Wraith of the Rift', type: 'Undead', subtype: 'Incorporeal', biomes: ['Shadow Rift', 'Glacier'] },
    { name: 'Owlbear', type: 'Monstrosity', subtype: 'Hybrid Beast', biomes: ['Forest', 'Prairie'] },
    { name: 'Displacer Beast', type: 'Monstrosity', subtype: 'Fey-Touched', biomes: ['Rainforest', 'Underdark'] },
    { name: 'Gelatinous Cube', type: 'Ooze', subtype: 'Cube', biomes: ['Cavern', 'Underdark'] },
    { name: 'Mind Flayer', type: 'Aberration', subtype: 'Psionic', biomes: ['Underdark', 'Shadow Rift'] },
    { name: 'Beholder', type: 'Aberration', subtype: 'Eye Tyrant', biomes: ['Cavern', 'Obsidian Deeps'] },
    { name: 'Young Red Dragon', type: 'Dragon', subtype: 'Chromatic', biomes: ['Mountain', 'Magma Hollow'] },
    { name: 'Will-o-Wisp', type: 'Undead', subtype: 'Spirit', biomes: ['Swamp', 'Fen'] },
    { name: 'Basilisk', type: 'Monstrosity', subtype: 'Petrifier', biomes: ['Badlands', 'Canyon'] }
  ],
  creatures: [
    { name: 'Treant Guardian', type: 'Plant', subtype: 'Sentient Tree', biomes: ['Old-Growth', 'Forest'] },
    { name: 'Dryad Envoy', type: 'Fey', subtype: 'Nymph', biomes: ['Forest', 'Wildflower Meadow'] },
    { name: 'Stone Giant', type: 'Giant', subtype: 'Earthborn', biomes: ['Mountain', 'Cavern'] },
    { name: 'Frost Giant', type: 'Giant', subtype: 'Iceborn', biomes: ['Tundra', 'Glacier'] },
    { name: 'Merrow Hunter', type: 'Monstrosity', subtype: 'Aquatic', biomes: ['Mangrove', 'Wetland'] },
    { name: 'Pegasus', type: 'Celestial', subtype: 'Winged Equine', biomes: ['Prairie', 'Alpine'] },
    { name: 'Griffon', type: 'Monstrosity', subtype: 'Sky Predator', biomes: ['Mountain', 'Steppe'] },
    { name: 'Naga Oracle', type: 'Monstrosity', subtype: 'Serpentine', biomes: ['Rainforest', 'River Jungle'] }
  ],
  animals: [
    { name: 'Wolf', type: 'Mammal', subtype: 'Pack Hunter', biomes: ['Forest', 'Tundra'] },
    { name: 'Dire Boar', type: 'Mammal', subtype: 'Large Herbivore', biomes: ['Forest', 'Savanna'] },
    { name: 'Brown Bear', type: 'Mammal', subtype: 'Omnivore', biomes: ['Forest', 'Alpine'] },
    { name: 'Mountain Goat', type: 'Mammal', subtype: 'Climber', biomes: ['Mountain', 'Hills'] },
    { name: 'Snow Hare', type: 'Mammal', subtype: 'Forager', biomes: ['Tundra', 'Glacier'] },
    { name: 'Giant Eagle', type: 'Bird', subtype: 'Raptor', biomes: ['Mountain', 'Prairie'] },
    { name: 'Crocodile', type: 'Reptile', subtype: 'Ambush Predator', biomes: ['Swamp', 'Mangrove'] },
    { name: 'Desert Viper', type: 'Reptile', subtype: 'Venomous', biomes: ['Desert', 'Badlands'] },
    { name: 'River Salmon', type: 'Fish', subtype: 'Migratory', biomes: ['River Jungle', 'Fen'] },
    { name: 'Cave Bat', type: 'Mammal', subtype: 'Nocturnal', biomes: ['Cavern', 'Underdark'] }
  ]
};

const continents = [
  { id: 'ct01', name: 'Aurelion Reach' },
  { id: 'ct02', name: 'Thornwild Expanse' },
  { id: 'ct03', name: 'Skydusk Crown' }
];

const countries = [
  { id: 'co01', name: 'Valemer', continentId: 'ct01' },
  { id: 'co02', name: 'Duskhal', continentId: 'ct01' },
  { id: 'co03', name: 'Ironveil', continentId: 'ct02' },
  { id: 'co04', name: 'Verdanor', continentId: 'ct02' },
  { id: 'co05', name: 'Sablemere', continentId: 'ct03' },
  { id: 'co06', name: 'Stormmarch', continentId: 'ct03' }
];

const kingdomNames = [
  'Kingdom of Highcrest',
  'Kingdom of Emberfall',
  'Kingdom of Dawnmere',
  'Kingdom of Blackfen',
  'Kingdom of Ironroot',
  'Kingdom of Goldhaven',
  'Kingdom of Mistbarrow',
  'Kingdom of Stonewake',
  'Kingdom of Sunscar',
  'Kingdom of Frosthelm',
  'Kingdom of Moonreach',
  'Kingdom of Riftgard'
];

const cityPrefixes = ['Ar', 'Bel', 'Cor', 'Drak', 'Eld', 'Fal', 'Gal', 'Hal', 'Ith', 'Jor', 'Kel', 'Lor'];
const citySuffixes = ['haven', 'port', 'spire', 'watch', 'ford', 'gard', 'mere', 'keep', 'crest', 'gate', 'vale', 'hold'];
const townPrefixes = ['North', 'South', 'East', 'West', 'High', 'Low', 'Old', 'New'];
const townSuffixes = ['brook', 'field', 'cross', 'hollow', 'point', 'bridge', 'market', 'stead'];
const villageSuffixes = ['ham', 'thorpe', 'wick', 'den', 'run', 'grove', 'mill', 'rest'];

const firstNames = ['Aelar', 'Brina', 'Corin', 'Dara', 'Eldrin', 'Fiora', 'Garrik', 'Helia', 'Iris', 'Joran', 'Kael', 'Lyra', 'Marek', 'Neris', 'Orin', 'Pyria', 'Quin', 'Riven', 'Selene', 'Torin'];
const lastNames = ['Ashford', 'Blackbriar', 'Crowley', 'Dawnstrider', 'Emberfall', 'Frostmere', 'Glimmerveil', 'Holloway', 'Ironwood', 'Jadebrook', 'Keenblade', 'Lightfoot', 'Moonwhisper', 'Nightrun', 'Oakenshield', 'Proudmoore', 'Quickwater', 'Ravencrest', 'Stormborn', 'Thornfield'];

const questTemplates = [
  { kind: 'Bounty', objective: 'Eliminate a roaming threat', reward: 'Gold and renown' },
  { kind: 'Escort', objective: 'Protect a caravan between settlements', reward: 'Trade access and gold' },
  { kind: 'Recovery', objective: 'Retrieve a relic from hostile ruins', reward: 'Artifact and favor' },
  { kind: 'Defense', objective: 'Hold a frontier outpost against waves', reward: 'Land grant and title' },
  { kind: 'Diplomacy', objective: 'Negotiate peace between rival factions', reward: 'Political influence' },
  { kind: 'Expedition', objective: 'Chart unmapped territory', reward: 'Maps and exploration rights' }
];

function pick(arr, index) {
  return arr[index % arr.length];
}

function slugify(str) {
  return String(str).toLowerCase().replace(/[^a-z0-9]+/g, '-').replace(/^-+|-+$/g, '');
}

function flattenBiomeTypes() {
  const rows = [];
  for (const bClass of biomeTaxonomy) {
    for (const sub of bClass.subclasses) {
      for (const type of sub.types) {
        for (const subtype of type.subtypes) {
          rows.push({ class: bClass.class, subclass: sub.subclass, type: type.type, subtype });
        }
      }
    }
  }
  return rows;
}

function makeCityName(kIndex, cIndex) {
  return `${pick(cityPrefixes, kIndex + cIndex)}${pick(citySuffixes, cIndex + kIndex * 2)}`;
}

function makeTownName(kIndex, cIndex, tIndex) {
  return `${pick(townPrefixes, kIndex + tIndex)} ${pick(townSuffixes, cIndex + tIndex * 2)}`;
}

function makeVillageName(kIndex, cIndex, vIndex) {
  return `${pick(cityPrefixes, kIndex + vIndex + cIndex)}${pick(villageSuffixes, cIndex + vIndex * 3)}`;
}

function makeZoneMap(kingdomId, seedOffset) {
  const allBiomes = flattenBiomeTypes();
  const zones = [];
  for (let i = 0; i < 12; i++) {
    const biome = allBiomes[(seedOffset + i * 3) % allBiomes.length];
    zones.push({
      zoneId: `${kingdomId}-zone-${String(i + 1).padStart(2, '0')}`,
      name: `${biome.type} ${i + 1}`,
      class: biome.class,
      subclass: biome.subclass,
      type: biome.type,
      subtype: biome.subtype,
      dangerTier: 1 + ((seedOffset + i) % 5)
    });
  }
  return { mapType: 'Overworld Region Map', dimensions: { cols: 24, rows: 18 }, zones };
}

function makeUndergroundZones(kingdomId, seedOffset) {
  const archetypes = [
    { type: 'Cavern', subtype: 'Limestone Networks' },
    { type: 'Underdark', subtype: 'Fungal Wilds' },
    { type: 'Catacombs', subtype: 'Royal Ossuary' },
    { type: 'Magma Hollow', subtype: 'Basalt Tunnels' },
    { type: 'Shadow Rift', subtype: 'Echoing Chasm' }
  ];
  const underground = [];
  for (let i = 0; i < 6; i++) {
    const a = archetypes[(seedOffset + i) % archetypes.length];
    underground.push({
      zoneId: `${kingdomId}-under-${String(i + 1).padStart(2, '0')}`,
      name: `${a.type} ${i + 1}`,
      class: 'Subterranean',
      subclass: 'Deep Earth',
      type: a.type,
      subtype: a.subtype,
      depthLevel: 1 + i
    });
  }
  return underground;
}

function buildWorld() {
  const kingdoms = [];
  let kingdomCounter = 0;

  for (let c = 0; c < countries.length; c++) {
    for (let localK = 0; localK < 2; localK++) {
      const kIndex = kingdomCounter;
      const country = countries[c];
      const continent = continents.find((x) => x.id === country.continentId);
      const kingdomId = `kg${String(kIndex + 1).padStart(2, '0')}`;
      const cities = [];

      for (let cityIndex = 0; cityIndex < 9; cityIndex++) {
        const cityId = `${kingdomId}-city-${String(cityIndex + 1).padStart(2, '0')}`;
        const towns = [];
        for (let townIndex = 0; townIndex < 2; townIndex++) {
          towns.push({ townId: `${cityId}-town-${townIndex + 1}`, name: makeTownName(kIndex, cityIndex, townIndex) });
        }

        const villages = [];
        for (let villageIndex = 0; villageIndex < 3; villageIndex++) {
          villages.push({ villageId: `${cityId}-village-${villageIndex + 1}`, name: makeVillageName(kIndex, cityIndex, villageIndex) });
        }

        cities.push({
          cityId,
          name: makeCityName(kIndex, cityIndex),
          cityType: cityIndex === 0 ? 'Capital City' : 'City',
          towns,
          villages
        });
      }

      kingdoms.push({
        kingdomId,
        name: kingdomNames[kIndex],
        continentId: continent.id,
        continent: continent.name,
        countryId: country.id,
        country: country.name,
        government: ['Monarchy', 'Council Realm', 'Mage Regency', 'Theocratic Crown'][kIndex % 4],
        alignmentTone: ['Lawful', 'Neutral', 'Chaotic'][kIndex % 3],
        cities,
        zoneMap: makeZoneMap(kingdomId, kIndex * 5),
        undergroundZones: makeUndergroundZones(kingdomId, kIndex * 2)
      });

      kingdomCounter += 1;
    }
  }

  return {
    metadata: {
      generator: 'worldgen/src/generate_world.js',
      generatedAt: new Date().toISOString(),
      settingStyle: 'DND5E-inspired fantasy scaffold'
    },
    continents,
    countries,
    kingdoms,
    taxonomy: { biomes: biomeTaxonomy },
    races,
    classes,
    bestiary: creatures,
    zoneMaps: kingdoms.map((k) => ({
      kingdomId: k.kingdomId,
      kingdom: k.name,
      country: k.country,
      continent: k.continent,
      zoneMap: k.zoneMap,
      undergroundZones: k.undergroundZones
    }))
  };
}

function makeKingdomLore(kingdom, index) {
  const capital = kingdom.cities[0].name;
  const frontier = kingdom.cities[kingdom.cities.length - 1].name;
  const headlineThreat = pick(['cult of the rift', 'wyvern migration', 'orc warbands', 'arcane blight', 'undead uprising'], index);
  const culturalVirtue = pick(['honor', 'innovation', 'hospitality', 'discipline', 'piety'], index + 2);

  const markdown = [
    `# ${kingdom.name}`,
    '',
    `- Kingdom ID: ${kingdom.kingdomId}`,
    `- Continent: ${kingdom.continent}`,
    `- Country: ${kingdom.country}`,
    `- Government: ${kingdom.government}`,
    `- Alignment Tone: ${kingdom.alignmentTone}`,
    '',
    '## Overview',
    `${kingdom.name} is known for ${culturalVirtue}, with ${capital} as its political center and ${frontier} as its outer frontier bastion.`,
    '',
    '## Notable Conflict',
    `Current intelligence suggests escalating activity from a ${headlineThreat} near border zones.`,
    '',
    '## City Network',
    ...kingdom.cities.map((city, i) => `${i + 1}. ${city.name} (${city.cityType})`),
    '',
    '## Underground Access',
    ...kingdom.undergroundZones.map((u) => `- ${u.name}: ${u.subtype} (Depth ${u.depthLevel})`),
    ''
  ].join('\n');

  return {
    kingdomId: kingdom.kingdomId,
    kingdom: kingdom.name,
    title: `${kingdom.name} Gazetteer`,
    summary: `${kingdom.name} centers on ${capital} and projects influence through nine cities across ${kingdom.country}.`,
    keyThemes: [culturalVirtue, kingdom.government, kingdom.alignmentTone],
    currentThreat: headlineThreat,
    markdown
  };
}

function makeNpcs(world) {
  const npcs = [];
  let npcIndex = 1;
  for (const kingdom of world.kingdoms) {
    for (const city of kingdom.cities) {
      const citySeed = Number(city.cityId.slice(-2));
      const roles = ['Governor', 'Guildmaster', 'Temple Warden', 'Spymaster'];
      for (let r = 0; r < 3; r++) {
        const race = pick(races, npcIndex + r);
        const classDef = pick(classes, npcIndex + r * 2);
        npcs.push({
          npcId: `npc-${String(npcIndex).padStart(4, '0')}`,
          name: `${pick(firstNames, npcIndex + r)} ${pick(lastNames, citySeed + r + npcIndex)}`,
          kingdomId: kingdom.kingdomId,
          cityId: city.cityId,
          city: city.name,
          role: pick(roles, npcIndex + r),
          race: race.race,
          raceSubtype: pick(race.subtypes, npcIndex + r),
          class: classDef.class,
          subclass: pick(classDef.subclasses, npcIndex + r)
        });
        npcIndex += 1;
      }
    }
  }
  return npcs;
}

function makeQuests(world, npcs) {
  const quests = [];
  let qIndex = 1;
  for (const kingdom of world.kingdoms) {
    for (let i = 0; i < 12; i++) {
      const tmpl = pick(questTemplates, i + qIndex);
      const zone = kingdom.zoneMap.zones[i % kingdom.zoneMap.zones.length];
      const issuer = pick(npcs.filter((n) => n.kingdomId === kingdom.kingdomId), i + qIndex);
      quests.push({
        questId: `q-${String(qIndex).padStart(4, '0')}`,
        kingdomId: kingdom.kingdomId,
        title: `${tmpl.kind}: ${zone.name}`,
        type: tmpl.kind,
        tier: zone.dangerTier,
        issuerNpcId: issuer.npcId,
        issuerName: issuer.name,
        location: zone.name,
        biomeType: zone.type,
        objective: tmpl.objective,
        reward: tmpl.reward,
        hooks: [
          `Rumors started in ${kingdom.cities[0].name}.`,
          `Threat confirmed around ${zone.zoneId}.`,
          `Secondary lead points toward ${kingdom.undergroundZones[i % kingdom.undergroundZones.length].name}.`
        ]
      });
      qIndex += 1;
    }
  }
  return quests;
}

function uniqueBiomeKeys() {
  const byType = new Map();
  for (const row of flattenBiomeTypes()) {
    if (!byType.has(row.type)) {
      byType.set(row.type, { class: row.class, subclass: row.subclass, type: row.type });
    }
  }
  return Array.from(byType.values());
}

function getCreaturePoolForBiome(biomeType) {
  const all = [...creatures.monsters, ...creatures.creatures, ...creatures.animals];
  const direct = all.filter((entry) => entry.biomes.includes(biomeType));
  if (direct.length >= 6) return direct;
  return direct.concat(all.filter((entry) => !direct.includes(entry))).slice(0, 20);
}

function makeEncounterTables() {
  const tables = [];
  for (const biome of uniqueBiomeKeys()) {
    const pool = getCreaturePoolForBiome(biome.type);
    for (let tier = 1; tier <= 5; tier++) {
      const entries = [];
      for (let i = 0; i < 6; i++) {
        const p = pool[(tier * 3 + i) % pool.length];
        entries.push({
          rollRange: `${i * 16 + 1}-${(i + 1) * 16}`,
          name: p.name,
          creatureType: p.type,
          creatureSubtype: p.subtype,
          count: `${1 + (tier % 2)}d${2 + (i % 3)}`,
          challengeBand: tier
        });
      }
      tables.push({
        biomeClass: biome.class,
        biomeSubclass: biome.subclass,
        biomeType: biome.type,
        dangerTier: tier,
        entries
      });
    }
  }
  return tables;
}

function makeDungeons(world) {
  const dungeonThemes = [
    'Ancient Ruins',
    'Fungal Depths',
    'Sunken Vault',
    'Infernal Forge',
    'Crystal Labyrinth',
    'Forgotten Catacomb'
  ];
  const bosses = [
    'Lich Regent',
    'Beholder Tyrant',
    'Dragon Wyrmling',
    'Mind Flayer Arcanist',
    'Stone Hydra',
    'Wraith Marshal'
  ];
  const dungeons = [];
  let dungeonIndex = 1;

  for (const kingdom of world.kingdoms) {
    for (let i = 0; i < 6; i++) {
      const zone = kingdom.undergroundZones[i % kingdom.undergroundZones.length];
      const tier = 1 + ((dungeonIndex - 1) % 5);
      dungeons.push({
        dungeonId: `dun-${String(dungeonIndex).padStart(4, '0')}`,
        kingdomId: kingdom.kingdomId,
        kingdom: kingdom.name,
        name: `${zone.name} - ${dungeonThemes[i % dungeonThemes.length]}`,
        theme: dungeonThemes[i % dungeonThemes.length],
        biomeClass: zone.class,
        biomeType: zone.type,
        biomeSubtype: zone.subtype,
        recommendedTier: tier,
        floors: 3 + (i % 4),
        boss: bosses[(dungeonIndex + i) % bosses.length],
        zoneLink: zone.zoneId
      });
      dungeonIndex += 1;
    }
  }
  return dungeons;
}

function ensureOutputDirs() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
  fs.mkdirSync(LORE_DIR, { recursive: true });
}

function writeJson(name, data) {
  fs.writeFileSync(path.join(OUTPUT_DIR, name), JSON.stringify(data, null, 2) + '\n', 'utf8');
}

function writeLoreFiles(loreEntries) {
  for (const entry of loreEntries) {
    const filename = `${entry.kingdomId}-${slugify(entry.kingdom)}.md`;
    fs.writeFileSync(path.join(LORE_DIR, filename), entry.markdown, 'utf8');
  }
}

function main() {
  ensureOutputDirs();
  const world = buildWorld();

  const loreEntries = world.kingdoms.map((k, i) => makeKingdomLore(k, i));
  const npcs = makeNpcs(world);
  const quests = makeQuests(world, npcs);
  const encounterTables = makeEncounterTables();
  const dungeons = makeDungeons(world);
  world.dungeons = dungeons;

  writeJson('world.json', world);
  writeJson('biomes.json', world.taxonomy.biomes);
  writeJson('races.json', world.races);
  writeJson('classes.json', world.classes);
  writeJson('creatures.json', world.bestiary);
  writeJson('zone_maps.json', world.zoneMaps);
  writeJson('kingdom_lore.json', loreEntries.map((l) => ({ ...l, markdown: undefined })));
  writeJson('npcs.json', npcs);
  writeJson('quests.json', quests);
  writeJson('encounter_tables.json', encounterTables);
  writeJson('dungeons.json', dungeons);

  writeLoreFiles(loreEntries);

  const totalCities = world.kingdoms.reduce((n, k) => n + k.cities.length, 0);
  const totalTowns = world.kingdoms.reduce((n, k) => n + k.cities.reduce((x, city) => x + city.towns.length, 0), 0);
  const totalVillages = world.kingdoms.reduce((n, k) => n + k.cities.reduce((x, city) => x + city.villages.length, 0), 0);

  console.log(`Generated ${world.kingdoms.length} kingdoms.`);
  console.log(`Generated ${totalCities} cities, ${totalTowns} towns, ${totalVillages} villages.`);
  console.log(`Generated ${loreEntries.length} kingdom lore files in ${LORE_DIR}`);
  console.log(`Generated ${npcs.length} NPCs and ${quests.length} quests.`);
  console.log(`Generated ${encounterTables.length} encounter tables.`);
  console.log(`Generated ${dungeons.length} dungeons.`);
  console.log(`Wrote files to ${OUTPUT_DIR}`);
}

main();
