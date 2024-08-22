#include <a_samp>
#include <Pawn.CMD>
#include <foreach>
#include <ColAndreas>
#include <grenades>

#define TIME_SMOKE (5 * 1000)

CMD:smoke(playerid)
{
    new Float:pos[4];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);

    ApplyAnimation(playerid, "GRENADE", "null", 4.1, 0, 1, 1, 0, 0, 1);
    ApplyAnimation(playerid, "GRENADE", "WEAPON_start_throw", 4.1, 0, 1, 1, 0, 0, 1);
    LaunchGrenade(pos[0], pos[1], pos[2], pos[3], "OnSmokeExplode", _, 12.0, 7.0, false, 2 * 1000);
    return 1;
}

forward OnSmokeExplode(GrenadeID:grenade_id);
public OnSmokeExplode(GrenadeID:grenade_id)
{
    new Float:pos[3];
    GetGrenadePos(grenade_id, pos[0], pos[1], pos[2]);
    new smoke_id = CreateObject(18732, pos[0], pos[1], pos[2], 0.0, 0.0, 0.0);
    SetTimerEx("OnDeleteSmoke", TIME_SMOKE, false, "d", smoke_id);
    return 1;
}

forward OnDeleteSmoke(object_id);
public OnDeleteSmoke(object_id)
{
    DestroyObject(object_id);
    return 1;
}