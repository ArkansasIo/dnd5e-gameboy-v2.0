const { d20 } = require('./dice');

class CombatEncounter {
  constructor(combatants) {
    this.round = 0;
    this.log = [];
    this.combatants = combatants.map((c) => ({
      actor: c,
      initiative: d20().total + c.mod('DEX'),
      defeated: false
    })).sort((a, b) => b.initiative - a.initiative);
    this.turnIndex = 0;
  }

  active() {
    return this.combatants.filter((c) => c.actor.hp > 0);
  }

  nextTurn() {
    if (this.active().length <= 1) return null;
    const actorEntry = this.combatants[this.turnIndex];
    if (this.turnIndex === 0) this.round += 1;
    this.turnIndex = (this.turnIndex + 1) % this.combatants.length;
    if (actorEntry.actor.hp <= 0) return this.nextTurn();
    return actorEntry.actor;
  }

  attack(attacker, target, opts) {
    const result = attacker.attack(target, opts);
    this.log.push({ round: this.round, type: 'attack', ...result });
    return result;
  }

  summary() {
    return this.combatants.map((c) => ({
      name: c.actor.name,
      initiative: c.initiative,
      hp: c.actor.hp,
      maxHP: c.actor.maxHP
    }));
  }
}

module.exports = { CombatEncounter };
