#include <a_samp>
#include <Pawn.CMD>
#include <foreach>
#include <ColAndreas>
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