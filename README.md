# `assist.inc`
An include which allows checking who assisted in player's death.

Assists go to players who most recently assisted in kill.

Works with every ***SA-MP*** version.

# How to use

1. `#include <assist>` 

**Note:** If your `OnPlayerDamage`/`OnPlayerTakeDamage` have return 0 (damage rejection) in gamemode/module, I suggest you to `#include` at very end of your gamemode or after including your modules with damage rejection.

Why? Well if you reject damage in some cases, it probably means that you dont want player to get assistention from rejected damage.

2. Add `OnPlayerAssist` in your gamemode. For more help about how to use it, check `Example`.

# Callback
```
//Called when player (playerid) assists the killer (killerid) in the killing of a player (deathid).
forward OnPlayerAssist(playerid, deathid, killerid);
```

# Definitions
```
//The maximum amount of players that can assist in single death (excluding killerid).
MAX_ASSIST
```

# Example
```pawn
public OnPlayerAssist(playerid, deathid, killerid)
{
    SendClientMessage(playerid, 0xFF0000FF, "You have assisted in kill!");
    GivePlayerMoney(playerid, 500);
    return 1;
}
```
