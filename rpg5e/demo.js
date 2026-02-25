const {
  Character,
  CombatEncounter,
  InventorySystem,
  QuestLog,
  FactionSystem,
  TravelSystem
} = require('./src');

const hero = new Character({
  name: 'Astra',
  className: 'Paladin',
  level: 3,
  abilities: { STR: 16, DEX: 10, CON: 14, INT: 10, WIS: 12, CHA: 16 },
  ac: 16
});

const goblin = new Character({
  name: 'Goblin Raider',
  className: 'Rogue',
  level: 1,
  abilities: { STR: 8, DEX: 14, CON: 10, INT: 8, WIS: 8, CHA: 8 },
  ac: 13,
  maxHP: 9
});

InventorySystem.addItem(hero, { id: 'shield_wood', name: 'Wooden Shield', slot: 'shield', acBonus: 2 });
InventorySystem.equip(hero, 'shield_wood');

const quests = new QuestLog();
quests.addQuest({ id: 'q1', title: 'Clear the Road', goal: 3 });
quests.progressQuest('q1');

const factions = new FactionSystem();
factions.adjust('Order of Dawn', 5);

const travel = TravelSystem.travelDay([hero], 'forest');

const combat = new CombatEncounter([hero, goblin]);
let actor;
for (let i = 0; i < 6; i++) {
  actor = combat.nextTurn();
  if (!actor) break;
  const target = actor === hero ? goblin : hero;
  if (actor.hp > 0 && target.hp > 0) combat.attack(actor, target, { damage: '1d8' });
}

console.log(JSON.stringify({
  travel,
  quests: quests.quests,
  factionReputation: factions.reputation,
  combat: combat.summary(),
  combatLogTail: combat.log.slice(-3)
}, null, 2));
