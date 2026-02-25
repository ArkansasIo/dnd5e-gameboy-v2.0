const { randInt } = require('./dice');

class InventorySystem {
  static addItem(character, item) {
    character.inventory.push({ ...item });
  }

  static removeItem(character, itemId) {
    const idx = character.inventory.findIndex((i) => i.id === itemId);
    if (idx >= 0) character.inventory.splice(idx, 1);
  }

  static equip(character, itemId) {
    const item = character.inventory.find((i) => i.id === itemId);
    if (!item) return false;
    if (item.slot) character.equipment[item.slot] = item;
    if (item.acBonus) character.ac += item.acBonus;
    return true;
  }
}

class QuestLog {
  constructor() {
    this.quests = [];
  }

  addQuest(quest) {
    this.quests.push({ ...quest, status: 'active', progress: 0 });
  }

  progressQuest(id, value = 1) {
    const q = this.quests.find((x) => x.id === id);
    if (!q) return null;
    q.progress += value;
    if (q.goal && q.progress >= q.goal) q.status = 'complete';
    return q;
  }
}

class FactionSystem {
  constructor() {
    this.reputation = {};
  }

  adjust(name, delta) {
    this.reputation[name] = (this.reputation[name] || 0) + delta;
    return this.reputation[name];
  }
}

class TravelSystem {
  static travelDay(party, terrain = 'plains') {
    const speedMod = { road: 1.0, plains: 0.9, forest: 0.75, mountain: 0.6, swamp: 0.55, underdark: 0.5 };
    const encounterChance = { road: 10, plains: 20, forest: 30, mountain: 35, swamp: 40, underdark: 45 };
    const miles = Math.floor(24 * (speedMod[terrain] || 0.8));
    const encounter = randInt(1, 100) <= (encounterChance[terrain] || 25);
    return { partySize: party.length, terrain, miles, encounter };
  }
}

module.exports = { InventorySystem, QuestLog, FactionSystem, TravelSystem };
