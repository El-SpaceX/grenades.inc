#include <a_samp>
#include <Pawn.CMD>
#include <foreach>
#include <ColAndreas>
#include <grenades>




new bool:isUsingLaunchGrenade[MAX_PLAYERS];

CMD:launchgrenade(playerid)
{
    isUsingLaunchGrenade[playerid] = true;
    GivePlayerWeapon(playerid, WEAPON_SHOTGUN, 10);
    return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if (weaponid == WEAPON_SHOTGUN)
    {
        new Float:pos[4];
        GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
        GetPlayerFacingAngle(playerid, pos[3]);
        LaunchGrenade(pos[0], pos[1], pos[2], pos[3], "OnGrenadeExplode", _, 20.0, 13.0, true, 15 * 1000);
        return 0;
    }
    return 1;
}

