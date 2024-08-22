/*
    GRENADES.INC

@credits: El-SpaceX
WARNING: As this library is for use with physics.inc, it is extremely important to be careful, as this can cause a lot of lag on your server if several grenades are created simultaneously. 
Good use!
*/

#if defined GRENADE_INCLUDED
    #endinput
#endif
#define GRENADE_INCLUDED

//foreach.inc or y_iterate.inc
#tryinclude <foreach>
#if !defined _FOREACH_LOCAL_VERSION
    #tryinclude <YSI_Data/y_iterate>
    #if !defined _inc_y_iterate
        #error "foreach or y_iterate required"
    #endif
#endif

	
//ColAndreas.inc
#tryinclude <ColAndreas>
#if !defined _colandreas_included
    #error "ColAndreas required"
#endif

#if !defined MAX_GRENADES_THROWN
    #define MAX_GRENADES_THROWN (100)
#endif

#define GrenadeID: _:
#define INVALID_GRENADE_ID (-1)





enum e_grenade_info
{
    objid,
    bool:explode_on_impact,
    timer_id,
    callback_name[32]
};




new Iterator:GrenadesThrown<MAX_GRENADES_THROWN>;
static g_GrenadesThrownInfo[MAX_GRENADES_THROWN][e_grenade_info] = { {INVALID_OBJECT_ID, false, -1, ""}, ... };

stock bool:IsValidGrenade(GrenadeID:grenade_id)
{
    return bool:Iter_Contains(GrenadesThrown, grenade_id);
}

static stock bool:SetGrenadeLifespan(GrenadeID:grenade_id, lifeSpan)
{   
    if (!IsValidGrenade(grenade_id))
        return false;

    g_GrenadesThrownInfo[grenade_id][timer_id] = SetTimerEx("GND_OnGrenadeExplodeByTimer", lifeSpan, false, "d", grenade_id);
    return true;
}


stock bool:GetExplodeOnImpact(GrenadeID:grenade_id)
{
    if (!IsValidGrenade(grenade_id))
        return false;

    return g_GrenadesThrownInfo[grenade_id][explode_on_impact];
}

stock bool:ToggleGrenadeExplodeOnImpact(GrenadeID:grenade_id, bool:toggle)
{
    if (!IsValidGrenade(grenade_id))
        return false;

    g_GrenadesThrownInfo[grenade_id][explode_on_impact] = toggle;
    return true;
}

stock bool:GetGrenadePos(GrenadeID:grenade_id, &Float:X, &Float:Y, &Float:Z)
{
    if (!IsValidGrenade(grenade_id))
    {
        X = 0.0;
        Y = 0.0;
        Z = 0.0;
        return false;
    }

    GetObjectPos(g_GrenadesThrownInfo[grenade_id][objid], X, Y, Z);
    return true;
}

GrenadeID:LaunchGrenade(Float:X, Float:Y, Float:Z, Float:angle, const callback[] = "", modelid = 1672, Float:speedX = 20.0, Float:speedZ = 9.0, bool:explodeOnImpact = false, lifespan = 20 * 1000)
{
    new GrenadeID:grenade_id = Iter_Free(GrenadesThrown);
    if (grenade_id == ITER_NONE)
    {
        return INVALID_GRENADE_ID;
    }

    Iter_Add(GrenadesThrown, grenade_id);

    new grenade = CreateObject(modelid, X, Y, Z, 0, 0, 0);
    PHY_InitObject(grenade, modelid, 2.4, _, PHY_MODE_3D);
    PHY_RollObject(grenade, _, PHY_ROLLING_MODE_ADVANCED);
    PHY_UseColAndreas(grenade);
    PHY_SetObjectVelocity(grenade, speedX * floatsin(-angle, degrees), speedX * floatcos(-angle, degrees), speedZ);
    PHY_SetObjectFriction(grenade, 1.0);
    PHY_SetObjectAirResistance(grenade, 0.1);
    PHY_SetObjectGravity(grenade, 9.1);
    PHY_SetObjectZBound(grenade, _, _, 0.5);

    ToggleGrenadeExplodeOnImpact(grenade_id, explodeOnImpact);
    SetGrenadeLifespan(grenade_id, lifespan);
    format(g_GrenadesThrownInfo[grenade_id][callback_name], 32, callback);
    g_GrenadesThrownInfo[grenade_id][objid] = grenade;
    return grenade_id;
}


bool:ExplodeGrenade(GrenadeID:grenade_id)
{
    if (!IsValidGrenade(grenade_id))
        return false;

    if (g_GrenadesThrownInfo[grenade_id][timer_id] != -1)
        KillTimer(g_GrenadesThrownInfo[grenade_id][timer_id]);

    if (funcidx(g_GrenadesThrownInfo[grenade_id][callback_name]) == -1)
    {
        printf("[GRENADES] Callback \"%s\" not found.", g_GrenadesThrownInfo[grenade_id][callback_name]);
    }
    else 
    {
        CallLocalFunction(g_GrenadesThrownInfo[grenade_id][callback_name], "d", grenade_id);
    }



    Iter_Remove(GrenadesThrown, grenade_id);
    DestroyObject(g_GrenadesThrownInfo[grenade_id][objid]);
    PHY_DeleteObject(g_GrenadesThrownInfo[grenade_id][objid]);

    format(g_GrenadesThrownInfo[grenade_id][callback_name], 32, "");
    g_GrenadesThrownInfo[grenade_id][objid] = INVALID_OBJECT_ID;
    g_GrenadesThrownInfo[grenade_id][timer_id] = -1;
    g_GrenadesThrownInfo[grenade_id][explode_on_impact] = false;    
    return true;
}

// --------------------- callback ---------------------

public PHY_OnObjectCollideWithObject(object1, object2) 
{
    foreach(new i : GrenadesThrown)
    {
        if ((g_GrenadesThrownInfo[i][objid] == object1 || g_GrenadesThrownInfo[i][objid] == object2) && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
     #if defined GND_OnObjectCollideWithObject
		return GND_OnObjectCollideWithObject(object1, object2);
	#else
		return 1;
	#endif
}

public PHY_OnObjectCollideWithZBound(objectid, lowhigh) 
{
    foreach(new i : GrenadesThrown)
    {
        if (g_GrenadesThrownInfo[i][objid] == objectid && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
    #if defined GND_OnObjectCollideWithZBound
		return GND_OnObjectCollideWithZBound(objectid, lowhigh);
	#else
		return 1;
	#endif
}

public PHY_OnObjectCollideWithSAWorld(objectid, Float:cx, Float:cy, Float:cz) 
{
    foreach(new i : GrenadesThrown)
    {
        if (g_GrenadesThrownInfo[i][objid] == objectid && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
    #if defined GND_OnObjectCollideWithSAWorld
        return GND_OnObjectCollideWithSAWorld(objectid, cx, cy, cz);
	#else
		return 1;
	#endif
}

public PHY_OnObjectCollideWithWall(objectid, wallid) 
{
    foreach(new i : GrenadesThrown)
    {
        if (g_GrenadesThrownInfo[i][objid] == objectid && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
    #if defined GND_OnObjectCollideWithWall
        return GND_OnObjectCollideWithWall(objectid, wallid);
	#else
		return 1;
	#endif
}

public PHY_OnObjectCollideWithCylinder(objectid, cylinderid) 
{
    foreach(new i : GrenadesThrown)
    {
        if (g_GrenadesThrownInfo[i][objid] == objectid && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
    #if defined GND_OnObjectCollideWithCylinder
        return GND_OnObjectCollideWithCylinder(objectid, cylinderid);
	#else
		return 1;
	#endif
}

public PHY_OnObjectCollideWithPlayer(objectid, playerid) 
{
    foreach(new i : GrenadesThrown)
    {
        if (g_GrenadesThrownInfo[i][objid] == objectid && GetExplodeOnImpact(i))
        {
            ExplodeGrenade(i);
            break;
        }
    }
    #if defined GND_OnObjectCollideWithPlayer
        return GND_OnObjectCollideWithPlayer(objectid, playerid);
	#else
		return 1;
	#endif
}


forward GND_OnGrenadeExplodeByTimer(grenade_id);
public GND_OnGrenadeExplodeByTimer(grenade_id)
{
    ExplodeGrenade(grenade_id);
    g_GrenadesThrownInfo[grenade_id][timer_id] = -1;
    return 1;
}

// --------------------- hooks ---------------------


// ------------------- PHY_OnObjectCollideWithObject -------------------
#if defined _ALS_OnObjectCollideWithObject
	#undef PHY_OnObjectCollideWithObject
#else
	#define _ALS_OnObjectCollideWithObject
#endif

#define PHY_OnObjectCollideWithObject GND_OnObjectCollideWithObject

#if defined GND_OnObjectCollideWithObject
	forward GND_OnObjectCollideWithObject(object1, object2);
#endif
// ----------------------------------------------


// ------------------- PHY_OnObjectCollideWithZBound -------------------
#if defined _ALS_OnObjectCollideWithZBound
	#undef PHY_OnObjectCollideWithZBound
#else
	#define _ALS_OnObjectCollideWithZBound
#endif

#define PHY_OnObjectCollideWithZBound GND_OnObjectCollideWithZBound

#if defined GND_OnObjectCollideWithZBound
	forward GND_OnObjectCollideWithZBound(objectid, lowhigh);
#endif
// ----------------------------------------------


// ------------------- PHY_OnObjectCollideWithSAWorld -------------------
#if defined _ALS_OnObjectCollideWithSAWorld
	#undef PHY_OnObjectCollideWithSAWorld
#else
	#define _ALS_OnObjectCollideWithSAWorld
#endif

#define PHY_OnObjectCollideWithSAWorld GND_OnObjectCollideWithSAWorld

#if defined GND_OnObjectCollideWithSAWorld
	forward GND_OnObjectCollideWithSAWorld(objectid, Float:cx, Float:cy, Float:cz);
#endif
// ----------------------------------------------


// ------------------- PHY_OnObjectCollideWithWall -------------------
#if defined _ALS_OnObjectCollideWithWall
	#undef PHY_OnObjectCollideWithWall
#else
	#define _ALS_OnObjectCollideWithWall
#endif

#define PHY_OnObjectCollideWithWall GND_OnObjectCollideWithWall

#if defined GND_OnObjectCollideWithWall
	forward GND_OnObjectCollideWithWall(objectid, wallid) 
#endif
// ----------------------------------------------


// ------------------- PHY_OnObjectCollideWithCylinder -------------------
#if defined _ALS_OnObjectCollideWithCylinde
	#undef PHY_OnObjectCollideWithCylinder
#else
	#define _ALS_OnObjectCollideWithCylinde
#endif

#define PHY_OnObjectCollideWithCylinder GND_OnObjectCollideWithCylinder

#if defined GND_OnObjectCollideWithCylinder
	forward GND_OnObjectCollideWithCylinder(objectid, cylinderid) 
#endif
// ----------------------------------------------

// ------------------- PHY_OnObjectCollideWithPlayer -------------------
#if defined _ALS_OnObjectCollideWithPlayer
	#undef PHY_OnObjectCollideWithPlayer
#else
	#define _ALS_OnObjectCollideWithPlayer
#endif

#define PHY_OnObjectCollideWithPlayer GND_OnObjectCollideWithPlayer

#if defined GND_OnObjectCollideWithPlayer
	forward GND_OnObjectCollideWithPlayer(objectid, playerid)
#endif
// ----------------------------------------------

