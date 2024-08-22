# Grenades include

I was playing around with physics.inc and did this, basically you can throw grenades and treat them when they explode.

With this you can create normal grenades, smoke grenades, grenade launchers... whatever your imagination can come up with.

Usage is very simple. we have some examples [here](examples).

## Usage

```cpp
#include a_samp
#include zcmd
#include foreach
#include ColAndreas
#include physics physics.inc pro default has no checks to not be included more than once, so include it before grenade.inc
#include grenades

CMDgrenade(playerid)
{
    new Floatpos[4];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);

    ApplyAnimation(playerid, GRENADE, null, 4.1, 0, 1, 1, 0, 0, 1);
    ApplyAnimation(playerid, GRENADE, WEAPON_start_throw, 4.1, 0, 1, 1, 0, 0, 1);
    LaunchGrenade(pos[0], pos[1], pos[2], pos[3], OnGrenadeExplode, _, 12.0, 7.0, false, 4  1000);
    return 1;
}

forward OnGrenadeExplode(GrenadeIDgrenade_id);
public OnGrenadeExplode(GrenadeIDgrenade_id)
{
    new Floatpos[3];
    GetGrenadePos(grenade_id, pos[0], pos[1], pos[2]);
    CreateExplosion(pos[0], pos[1], pos[2], 11, 3.0);
    return 1;
}
```


## Functions


```cpp
GrenadeIDLaunchGrenade(FloatX, FloatY, FloatZ, Floatangle, const callback[], modelid = 1672, FloatspeedX = 20.0, FloatspeedZ = 9.0, boolexplodeOnImpact = false, lifespan = 20  1000)
```
Creates and throws a grenade.
 `X, Y, Z` - Position where it will be created.
 `angle` - Angle at which it will be thrown (player angle for example).
 `callback[]` - Callback that will be called when the grenade activates.
 `modelid` - Grenade object ID (by default 1672, which is a large light).
 `speedX` - Speed ​​that will tell you how far you will advance.
 `speedZ` - Speed that will tell you how high it will go.
 `explodeOnImpact` - Set whether the grenade will activate on any impact or only when its timer runs out.
 `lifespan` - Time (in milliseconds) that the grenade will remain active; at the end of this time, it will detonate. The default is 20 seconds.

Returns the ID of the created grenade or INVALID_GRENADE_ID if it couldn't be created.


```cpp
boolIsValidGrenade(GrenadeIDgrenade_id)
```
Returns whether a grenade is valid.
 `grenade_id` - obvious.

Returns true if the grenade is valid, and false if it is not.




## Required
- [physics.inc](httpsgithub.comuPeppephysics.inctreemaster)
- [ColAndreas](httpsgithub.comPottusColAndreas)
- [foreach](httpsgithub.comkarimcambridgesamp-foreach) or [y_iterate](httpsgithub.compawn-langYSI-Includes)
- [modelsizes](httpsgithub.comCrayderModel-Sizes-Plus) (necessary for physics.inc to work)