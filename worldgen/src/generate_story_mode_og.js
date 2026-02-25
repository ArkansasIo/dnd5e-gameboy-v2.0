const fs = require('fs');
const path = require('path');

const OUT_JSON = path.resolve(__dirname, '..', 'generated', 'story_mode_og_12acts_50chapters.json');
const OUT_MD = path.resolve(__dirname, '..', 'generated', 'story_mode_og_guide.md');

const actChapterCounts = [4,4,4,4,4,4,4,4,4,4,5,5];

const factions = [
  'Order of the Dawn Sigil',
  'Candle Archivists',
  'Verdant Circle',
  'Ironbound Compact',
  'Veilwalkers Guild',
  'Ashen Covenant'
];

const biomePool = ['Forest', 'Mountain', 'Swamp', 'Desert', 'Underdark', 'Ruins', 'Coast', 'Tundra'];
const threatPool = ['Cult', 'Dragonkin', 'Undead', 'Abyssal Rift', 'Bandit Legion', 'Mind Flayer Cell', 'Fey Court Intrigue', 'Infernal Contract'];
const rewardPool = ['Rare Magic Item', 'Stronghold Upgrade', 'Faction Rank', 'Spell Scroll Cache', 'Legendary Craft Material', 'Mount or Vehicle'];

const actTitles = [
  'Embers of the Broken Sigil',
  'Roads Through the Thornwild',
  'Citadel of Oath and Iron',
  'The Deep Lantern Conspiracy',
  'Siege of the Hollow Crown',
  'Blood Moon over Sablemere',
  'March of the Riftbound',
  'The Ashen Accord Betrayal',
  'Tempest Thrones of Skydusk',
  'Vaults Beneath the Last Cathedral',
  'War of Twelve Banners',
  'Dawnfall and the World Oath'
];

function chapterTitle(i) {
  const stems = [
    'Broken Oath', 'Whispers in Stone', 'Cinders of Faith', 'Sword at Dusk', 'Trial by Moonfire',
    'Echoes of the Deep', 'Rite of the Banner', 'The Quiet Siege', 'Road of Bones', 'The Last Envoy',
    'Shards of Heaven', 'Storm on the Gate', 'Feast of Masks', 'Throne of Ash', 'The Lantern Pact'
  ];
  return stems[i % stems.length];
}

function makeQuest(id, kind, actNo, chapterNo, tierSeed) {
  const biome = biomePool[(actNo + chapterNo + tierSeed) % biomePool.length];
  const threat = threatPool[(actNo * 2 + chapterNo + tierSeed) % threatPool.length];
  const reward = rewardPool[(actNo + chapterNo + tierSeed) % rewardPool.length];
  const levelMin = Math.min(20, 1 + Math.floor((actNo - 1) * 1.6));
  const levelMax = Math.min(20, levelMin + 2);

  return {
    questId: id,
    type: kind,
    title: `${kind === 'main' ? 'Main' : 'Side'} Quest: ${threat} in ${biome}`,
    summary: `Investigate and resolve a ${threat.toLowerCase()} threat across ${biome.toLowerCase()} frontiers before it destabilizes neighboring kingdoms.`,
    dnd5e: {
      recommendedLevel: `${levelMin}-${levelMax}`,
      pillars: ['Combat', 'Exploration', 'Social'],
      skillFocus: ['Investigation', 'Perception', 'Persuasion', 'Stealth'],
      encounterBudgetStyle: 'Medium to Hard',
      expectedBossCR: Math.max(1, Math.min(20, levelMin + 2)),
      restsSuggested: ['1 Short Rest', 'Optional Long Rest at chapter midpoint']
    },
    objectives: [
      'Gather leads from at least two settlements',
      'Clear one hostile site or dungeon node',
      'Resolve final confrontation or diplomatic standoff'
    ],
    rewards: [reward, 'XP Milestone', 'Gold and Supplies'],
    linkedFactions: [factions[(actNo + chapterNo) % factions.length], factions[(actNo + chapterNo + 2) % factions.length]]
  };
}

function generate() {
  let globalChapter = 1;
  const acts = [];

  for (let a = 0; a < 12; a++) {
    const actNo = a + 1;
    const chaptersInAct = actChapterCounts[a];
    const act = {
      act: actNo,
      title: actTitles[a],
      scope: {
        kingdomsInFocus: [`kg${String(((a*2)%12)+1).padStart(2,'0')}`, `kg${String((((a*2)+1)%12)+1).padStart(2,'0')}`],
        factionsInConflict: [factions[a % factions.length], factions[(a + 3) % factions.length]],
        primaryThreat: threatPool[a % threatPool.length]
      },
      recommendedPartyLevel: `${Math.min(20, 1 + a)}-${Math.min(20, 3 + a)}`,
      chapters: []
    };

    for (let c = 0; c < chaptersInAct; c++) {
      const chapterNo = globalChapter;
      const mainQuest = makeQuest(`mq-${String(chapterNo).padStart(3, '0')}`, 'main', actNo, chapterNo, 0);
      const sideQuests = [
        makeQuest(`sq-${String(chapterNo).padStart(3, '0')}-a`, 'side', actNo, chapterNo, 1),
        makeQuest(`sq-${String(chapterNo).padStart(3, '0')}-b`, 'side', actNo, chapterNo, 2)
      ];

      act.chapters.push({
        chapter: chapterNo,
        title: chapterTitle(chapterNo + a),
        narrativeBeat: `Chapter ${chapterNo} escalates regional tension and reveals a new layer of the ${act.scope.primaryThreat.toLowerCase()} plot.`,
        locations: [biomePool[(chapterNo + a) % biomePool.length], biomePool[(chapterNo + a + 2) % biomePool.length]],
        mainQuest,
        sideQuests
      });

      globalChapter += 1;
    }

    acts.push(act);
  }

  const allChapters = acts.reduce((n, a) => n + a.chapters.length, 0);
  const allMain = acts.reduce((n, a) => n + a.chapters.length, 0);
  const allSide = acts.reduce((n, a) => n + a.chapters.reduce((x, ch) => x + ch.sideQuests.length, 0), 0);

  const story = {
    metadata: {
      generatedAt: new Date().toISOString(),
      source: 'worldgen/src/generate_story_mode_og.js',
      mode: 'OG Story Mode',
      style: 'DND5E-inspired campaign arc'
    },
    campaign: {
      title: 'Labyrinth of the Dragon: OG Campaign',
      premise: 'A fractured sigil that once sealed planar breaches begins to fail, forcing twelve kingdoms into a campaign of alliances, betrayal, and apocalyptic war.',
      acts,
      totals: {
        acts: acts.length,
        chapters: allChapters,
        mainQuests: allMain,
        sideQuests: allSide,
        totalQuests: allMain + allSide
      }
    }
  };

  return story;
}

function makeGuide(story) {
  const lines = [];
  lines.push('# OG Story Mode Guide');
  lines.push('');
  lines.push(`- Acts: ${story.campaign.totals.acts}`);
  lines.push(`- Chapters: ${story.campaign.totals.chapters}`);
  lines.push(`- Main Quests: ${story.campaign.totals.mainQuests}`);
  lines.push(`- Side Quests: ${story.campaign.totals.sideQuests}`);
  lines.push('');

  for (const act of story.campaign.acts) {
    lines.push(`## Act ${act.act}: ${act.title}`);
    lines.push(`- Recommended Party Level: ${act.recommendedPartyLevel}`);
    lines.push(`- Primary Threat: ${act.scope.primaryThreat}`);
    lines.push(`- Factions in Conflict: ${act.scope.factionsInConflict.join(', ')}`);
    lines.push('');

    for (const ch of act.chapters) {
      lines.push(`### Chapter ${ch.chapter}: ${ch.title}`);
      lines.push(`- Beat: ${ch.narrativeBeat}`);
      lines.push(`- Main Quest: ${ch.mainQuest.title}`);
      lines.push(`- Side Quests: ${ch.sideQuests.map((q) => q.title).join(' | ')}`);
      lines.push('');
    }
  }

  return lines.join('\n') + '\n';
}

const story = generate();
fs.mkdirSync(path.dirname(OUT_JSON), { recursive: true });
fs.writeFileSync(OUT_JSON, JSON.stringify(story, null, 2) + '\n', 'utf8');
fs.writeFileSync(OUT_MD, makeGuide(story), 'utf8');

console.log(`Wrote story JSON: ${OUT_JSON}`);
console.log(`Wrote story guide: ${OUT_MD}`);
console.log(`Acts=${story.campaign.totals.acts} Chapters=${story.campaign.totals.chapters} Main=${story.campaign.totals.mainQuests} Side=${story.campaign.totals.sideQuests}`);
