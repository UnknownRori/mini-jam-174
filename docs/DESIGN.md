# Design

This is design docs for game about small salvager on deep space.

## Planned Feature

- Roguelite system (Exp based on collected scrap)
- Power up / Upgrade for every level up
- Main Menu
- Physic
- Dust cloud / Nebula for environment
- Minimap
- Mothership

## Mechanic

Since it's only limited for one resources, all of the mechanic will be around
`scrap` a resources that can be obtained via destroying asteroid or enemy ship,
every shot will consume scrap, initially starting scrap is 50, damage to the player will
also reduce scrap in inventory.

### Drop rates

- Small Asteroid : 12 scrap
- Medium Asteroid : 24 scrap

- Small Enemy Ship : 20 scrap
- Medium Enemy Ship : 32 scrap
- Enemy Mothership : 100 scrap

### Scoring System

Requirement to level up is multiplied by 2 and it start at 10
for clarity there is some paragraph that explain scrap that needed
```
Level 1 : 100
Level 2 : 200
Level 3 : 400
Level 4 : 800
...
Level 10 : 10000
```

### Upgrade

Every level up it will grant an upgrade

- Increased collection range
- Fast fire rate
- Multiply drop rate
