# AfterShock XE Changelog
## 2019-01-21 [Revision 327]
- Fixed a bug when One Flag Capture's white flag was displayed in other 
modes on some maps.
- Fixed a number of multiview bugs.
- Added TAB completion for a number of console commands.
- Added `coinflip` command to flip a coin.
- Fixed a bug when players would get disconnected from a server after 
`callvote shuffle`
- Fixed a bug when spectators would accumulate extra deaths.
- A map will no longer reload instead of restarting when there's no 
rotation or next map on the rotation is the same map.
- Added `switchserverforce` command to forcibly switch a client to 
another server.  
Usage: `switchserverforce <client number> <ip:port>`.
- Added Rocket Arena 3 multi-arena maps support.
  - Use teleporters in the lobby arena on a RA3 map to access other
  arenas or `arena <number>`.
  - `g_lockArena <number>` allows forcing everyone into the same arena 
  and locking it. If it's set to `0` (the default) people will spawn in 
  the lobby arena and there will be no lock.  
  `g_lockArena` is votable by default (included in `g_voteNames`).
  - `callvote <RA3 map name> <arena number>` to vote for a particular
  arena (it automatically sets `g_lockArena` to the `<number>`
  you chose), for example `callvote map ra3map20 4`.  
  If you just do `callvote <RA3 map name>` without specifying a number 
  of the arena it will keep `g_lockArena` on the same value it used to
  have.
  - RA3 map's arena can be added to mapcycle config with `<ra3 map 
  name> <minimum number of players> <maximum number of players> <arena 
  number>`, for example `ra3map20 6 10 4`.
- Added a posibility to use custom votes without the word `custom`.
So instead of `callvote custom <vote name>` you can do `callvote 
<vote name>`. For example `callvote ctf` instead of 
`callvote custom ctf`.
- Now on `callvote <map name>` a map image is displayed in the top-left 
corner.

## 2018-12-22 [Revision 326]
- Added `g_legacyWeaponAmmo` cvar.
  - When set to `0` weapons you pick up will always add the "standard"
amount of ammo and all ammo you picked up before picking up a weapon
will be kept.
  - When set to `1` (default) weapon pickups will fill up the ammo of a
player to "standard" amount and add 1 bullet if already reached the 
"standard" amount and if you picked ammo before picking a weapon it
will add 1 bullet.
- Added `g_allowKill -1` that allows `kill` during warmup but disables
it during the game.
- `g_gametype 5` (One Flag Capture), `6` (Overload) and `7` (Harvester)
names are now properly displayed on the map loading screen.
- On instagib the scoreboard now shows Railgun accuracy instead of
damage done and received.
- Death and award count are now kept when switching team or joining 
spectators. When joining spectators it adds 1 more death unless a 
player was already dead.
- Backported `g_timestamp` from OpenArena gamecode.
- Fixed a bug when powerups would run out during a timeout.
- Fixed a bug when sound cues for flag dropped/taken would not play.

## 2018-10-27 [Revision 325]
- Added cvars to change weapon properties:
  - `g_gauntletRate`, `g_gauntletDamage`,
  - `g_machinegunRate`, `g_machinegunDamage`,
  `g_machinegunDamageReduced`, `g_machinegunDamageTeam`,
  `g_machinegunSpread`,
  - `g_shotgunRate`, `g_shotgunDamage`, `g_shotgunCount`,
  `g_shotgunSpread`,
  - `g_plasmaRate`, `g_plasmaDamage`, `g_plasmaSplashDamage`,
  `g_plasmaSplashRadius`, `g_plasmaVelocity`,
  - `g_lightningRate`, `g_lightningDamage`, `g_lightningDamageReduced`,
  `g_lightningRange`,
  - `g_grenadeRate`, `g_grenadeDamage`, `g_grenadeSplashDamage`,
  `g_grenadeSplashRadius`, `g_grenadeVelocity`, `g_grenadeLifetime`,
  - `g_rocketRate`, `g_rocketDamage`, `g_rocketSplashDamage`,
  `g_rocketSplashRadius`, `g_rocketVelocity`,
  - `g_railRate`, `g_railDamage`, `g_railDamageReduced`,
  - `g_bfgRate`, `g_bfgDamage`, `g_bfgSplashDamage`,
  `g_bfgSplashRadius`, `g_bfgVelocity`.
- Now during a warmup "Warmup" sign is shown instead of bogus round
time.
- The round timer in Elimination mode now stops during a timeout.

## 2018-10-08 [Revision 324]
- Speaker sounds are no longer played if `s_ambient` is set to `0`.
- Fixed a bug when the "quad factor" was applied twice to Machinegun.

## 2018-09-12 [Revision 323]
- Added an option for spectators to see when players use zoom:
  - `cg_spectatorZoom 1` (default) = on,
  - `cg_spectatorZoom 0` = off (old behavior).
- Improved demos compatibility with
[q3mme](https://github.com/entdark/q3mme) by moving changes to the
bottom of the event and item list so that the other elements are in the
same order as in vanilla Quake 3.

## 2018-08-31 [Revision 322]
- Added an experimental ASXE ruleset based on AS one with a very light
version of air control. This ruleset will be shaped with community
feedback to create a better default ruleset for the mod.
- Added CPM and QW rulesets with `g_ruleset` cvar that replaces
`g_aftershockPhysic`.  
 Available options are:
  - `as` (default),
  - `asxe`,
  - `vq3`,
  - `cpm`,
  - `qw`.
- Added `callvote ruleset`.
- Removed unused movement checks.

## 2018-08-13 [Revision 321]
- Added `g_furthestTeamSpawns`, default is`0`.  
 If `g_furthestTeamSpawns` is `1` and the mode is CA/Elimination, a
random spawn point will be picked for one team and the other team will
be spawned at the furthest away spawn point. The same two spawn points
will be used every round but the teams will switch between them
alternatingly.
- Global sounds and ET_MOVER loop sounds are no longer played when
`s_ambient` is `0`.
- Spawn and teleport bug fixes.

## 2018-08-09 [Revision 320]
- Flag icons on the HUD are now properly displayed for spectators.
- Fixed a bug when players would still get stuck on the same spawn
point on some maps.

## 2018-07-29 [Revision 319]
- Improved spawn behavior in team modes.
- Added `g_spawnPush` cvar for improved spawn behavior.
  - If `g_spawnPush` is set to `1`, a player that spawns inside of a
team member will get pushed forward and spawn in front of him.
  - If `g_spawnPush` is set to `0`, a player that spawns inside of a
team member will get stuck (default OA behavior).
- Added `g_telefragTeamBehavior` cvar for improved telefrag
behavior:
  - `g_telefragTeamBehavior 0`: if a player teleports into a team
member, they both will get stuck (default OA behavior),
  - `g_telefragTeamBehavior 1`: if a player teleports into a team
member, the player wil be pushed forward and spawn in front of him,
  - `g_telefragTeamBehavior 2`: if a player would teleport into a team
member, the teleport will not work and the player will stay at the
teleport entry until the team member moved.
- Added support for projectile teleportation sound (in/out).  
 The new sound files can be put in:
  - `sound/world/teleinProjectile.wav`,
  - `sound/world/teleoutProjectile.wav`.

## 2018-07-27 [Revision 318]
- Fixed a bug when the projectiles would get stopped mid air in some
places on some maps.

## 2018-07-27 [Revision 317]
- Added support for shooting projectiles through teleports with two new
cvars.
  - `g_allowProjectileTeleport` is a bitmask:
     - bit `0` = rockets,
     - bit `1` = plasma,
     - bit `2` = grenades.  
  Examples:
     - `7` => allow all three (default),
     - `1` => allow only rockets,
     - `3` => allow rockets and grenades.
  - `g_projectileTeleportKeepAngle` controls the angle of the
projectile.
    - If this is set to `1` (default), the projectile will go out of
the teleport at the same angle it entered,
    - If this is set to `0`, the projectile will always exit the
teleport straight. This is the behavior that players entering a
teleport have.

## 2018-07-25 [Revision 316]
- Fixed a number of scoreboard bugs.
- Now when the scoreboard key is pressed down the scoreboard will
update once a second.

## 2018-07-22 [Revision 315]
- Fixed a bug when the scoreboard disappears with 6v6 and more.
- Frags are now shown instead of the score as kills on the scoreboard
in CTF.
- Kills are now shown in warmup.
- The time on the scoreboard is now always printed with 5 characters
and the font with the same width as for damage done and received is now
used for time and ping.
- "You have lost the lead" sound cue can never play twice in a row
anymore.
- Various other bug fixes.

## 2018-07-21 [Revision 314]
- Added `g_damagePlums` and `cg_damagePlums` (both default to `1`).  
These allow displaying damage plums on hit, `g_damagePlums` is a
server-side variable and `cg_damagePlums` is client-side.
- Added `g_crosshairNamesFog`, default is `0`.  
Setting this to `1` will allow names to be seen in the fog.
- Renamed `plasmaSpark` shader to `plasmaSparkAs`, because OpenArena
0.8.8 has a shader with the same name.
- Changed time format on the scoreboard from `hh:mm` to `mm:ss`.
- Various other improvements and bug fixes.

## 2018-07-17 [Revision 313]
- Added support for sorting servers by the number of human players.
- Removed flood protection for admin commands.

## 2018-07-16 [Revision 312]
- Fixed vote passed/vote failed bug completely.
- Fixed a bug when flood limit would have false positives.
- Added `cg_fastforward` and `cg_fastforwardSpeed`.
  - `cg_fastforward` gives the seconds a demo should be forwarded.
  - `cg_fastforwardSpeed` gives the speed of the forwarding (like
 `timescale`).

So if you set `cg_fastforwardSpeed 100` and `cg_fastforward 600`,
the demo will forward 10 minutes (600 seconds) with 100x speed.
- Fixed powerup disappearing bug (that happens on ps37ctf-mmp map).
- Fixed a bug when maplist wouldn't be displayed if it got over 1024
characters.
- Players no longer drown in water after a timeout is over.
- The timer now shows the correct time during a timeout.
- The flags can no longer disappear during a timeout.

## 2018-07-07 [Revision 311]
- added `cg_soundOption <0-9>` to alter between sound packs,
default value is `1`.  
A value of `0` sets all sounds to default. A value of `X`(with `X` =
`1-9`) loads option `X` for all sounds. A sound S.wav with option X has
to have the name `S_optX.wav` in the `.pk3`.  
If `S_optX.wav` does not exist it falls back to `S.wav`.
- Added `cg_soundOption* <-1-9>` to alter between individual sounds
for weapons:
  - `cg_soundOptionGauntlet`,
  - `cg_soundOptionLightning`,
  - `cg_soundOptionMachinegun`,
  - `cg_soundOptionShotgun`,
  - `cg_soundOptionRocket`,
  - `cg_soundOptionGrenade`,
  - `cg_soundOptionPlasma`,
  - `cg_soundOptionRail`,
  - `cg_soundOptionBFG`.

`cg_soundOption*` can overwrite the sounds set by `cg_soundOption`.  
If the option does not exist it falls back to `cg_soundOption` (which
falls back to the default sound).

## 2018-07-07 [Revision 310]
- Teams can never end up the same after shuffle anymore.
- `cg_blood 0` now turns all the blood off including the splatters of
blood on the screen when you get hit.
- Mapcycle now relies on the number of players in game instead of the
total number of clients on the server.
- The vote string for `callvote nextmap` now displays the name of the
map in the brackets instead of the current map.
- Added colors to the vote string, now "yes" votes are shown in green
and "no" votes in red.
- Increased a maximum count of maps in mapcycle config from 64 to 128.

## 2018-07-01 [Revision 309]
- Fixed a bug when a player could rename themselves to "vote passed"
and "vote failed" and the corresponding sounds were played.
- Changed to `xx.xk` format for the damage on the scoreboard.
- If a player accidentially joins a team he is already is on, he can
join a new team immediately and does not have to wait 5 seconds
anymore.
- Now a new line is started if spectator names don't fit in a single
line.
- Various other bug fixes.

## 2018-07-01 [Revision 308]
- Disabled dropping Machinegun with `dropweapon` command.
- Added `droppowerup` command.
- Fixed a bug with flags disappearing from the scoreboard on CTF.
- Extended CA and CTF scoreboards with full damage done and received,
kills and deaths.
- Extended CTF scoreboard with amount of captures, assist and defend
rewards.
- "<name\> is the new team leader" info message is now always white.
- Fixed a bug when spectator name inherits color from a spectator whose
name is displayed before them.
- Fixed `!mute` command not working.
- Added support for flood limiting.

## 2018-06-28 [Revision 307]
- added `g_reduceMachinegunDamage <0|1>`.
  - `0` (vanilla) - Machinegun does 7 damage for all modes except Team
Deathmatch where it's 5.
  - `1` (default) - Machinegun does 6 damage for all modes except Team
Deathmatch where it's 5.
- Added `g_rocketVelocity <integer>`.
Specifies rocket speed in UPS, default value is `1000`, vanilla used to
be `900`.
- Added `g_gravityModifier <float>`.
Sets a multiplier for `g_gravity`, default value is `1`.
- Removed 125 FPS lock.
- Removed "not logged in" message since the AfterShock site is long
gone.
- Now compiles with GCC-4.8+.

[Revision 327]:https://github.com/Irbyz/aftershock-xe/compare/326...327
[Revision 326]:https://github.com/Irbyz/aftershock-xe/compare/325...326
[Revision 325]:https://github.com/Irbyz/aftershock-xe/compare/324...325
[Revision 324]:https://github.com/Irbyz/aftershock-xe/compare/323...324
[Revision 323]:https://github.com/Irbyz/aftershock-xe/compare/322...323
[Revision 322]:https://github.com/Irbyz/aftershock-xe/compare/321...322
[Revision 321]:https://github.com/Irbyz/aftershock-xe/compare/320...321
[Revision 320]:https://github.com/Irbyz/aftershock-xe/compare/319...320
[Revision 319]:https://github.com/Irbyz/aftershock-xe/compare/318...319
[Revision 318]:https://github.com/Irbyz/aftershock-xe/compare/317...318
[Revision 317]:https://github.com/Irbyz/aftershock-xe/compare/316...317
[Revision 316]:https://github.com/Irbyz/aftershock-xe/compare/315...316
[Revision 315]:https://github.com/Irbyz/aftershock-xe/compare/314...315
[Revision 314]:https://github.com/Irbyz/aftershock-xe/compare/313...314
[Revision 313]:https://github.com/Irbyz/aftershock-xe/compare/312...313
[Revision 312]:https://github.com/Irbyz/aftershock-xe/compare/311...312
[Revision 311]:https://github.com/Irbyz/aftershock-xe/compare/310...311
[Revision 310]:https://github.com/Irbyz/aftershock-xe/compare/309...310
[Revision 309]:https://github.com/Irbyz/aftershock-xe/compare/308...309
[Revision 308]:https://github.com/Irbyz/aftershock-xe/compare/307...308
[Revision 307]:https://github.com/Irbyz/aftershock-xe/compare/299...307
