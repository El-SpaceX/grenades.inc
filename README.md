# Grenades include

I was playing around with "physics.inc" and did this, basically you can throw grenades and treat them when they "explode".

With this you can create normal grenades, smoke grenades, grenade launchers... whatever your imagination can come up with.

Usage is very simple. we have some examples [here](examples/).

## Usage

```cpp
#include <a_samp>
#include <zcmd>
#include <foreach>
#include <ColAndreas>
#include <physics> //"physics.inc" pro default has no checks to not be included more than once, so include it before "grenade.inc"
#include <grenades>

CMD:grenade(playerid)
{
    new Float:pos[4];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);

    ApplyAnimation(playerid, "GRENADE", "null", 4.1, 0, 1, 1, 0, 0, 1);
    ApplyAnimation(playerid, "GRENADE", "WEAPON_start_throw", 4.1, 0, 1, 1, 0, 0, 1);
    LaunchGrenade(pos[0], pos[1], pos[2], pos[3], "OnGrenadeExplode", _, 12.0, 7.0, false, 4 * 1000);
    return 1;
}

forward OnGrenadeExplode(GrenadeID:grenade_id);
public OnGrenadeExplode(GrenadeID:grenade_id)
{
    new Float:pos[3];
    GetGrenadePos(grenade_id, pos[0], pos[1], pos[2]);
    CreateExplosion(pos[0], pos[1], pos[2], 11, 3.0);
    return 1;
}
```


## Functions


```cpp
GrenadeID:LaunchGrenade(Float:X, Float:Y, Float:Z, Float:angle, const callback[], modelid = 1672, Float:speedX = 20.0, Float:speedZ = 9.0, bool:explodeOnImpact = false, lifespan = 20 * 1000)
```
Creates and throws a grenade.
* `X, Y, Z` - Position where it will be created.
* `angle` - Angle at which it will be thrown (player angle for example).
* `callback[]` - Callback that will be called when the grenade activates.
* `modelid` - Grenade object ID (by default 1672, which is a large light).
* `speedX` - Speed ??that will tell you how far you will advance.
* `speedZ` - Speed that will tell you how high it will go.
* `explodeOnImpact` - Set whether the grenade will activate on any impact or only when its timer runs out.
* `lifespan` - Time (in milliseconds) that the grenade will remain active; at the end of this time, it will detonate. The default is 20 seconds.

Returns the ID of the created grenade or INVALID_GRENADE_ID if it couldn't be created.


```cpp
bool:IsValidGrenade(GrenadeID:grenade_id)
```
Returns whether a grenade is valid.
* `grenade_id` - obvious.

Returns true if the grenade is valid, and false if it is not.




## Required:
- [physics.inc](https://github.com/uPeppe/physics.inc)
- [ColAndreas](https://github.com/Pottus/ColAndreas)
- [foreach](https://github.com/karimcambridge/samp-foreach) or [y_iterate](https://github.com/pawn-lang/YSI-Includes)
- [modelsizes](https://github.com/Crayder/Model-Sizes-Plus) (necessary for physics.inc to work)