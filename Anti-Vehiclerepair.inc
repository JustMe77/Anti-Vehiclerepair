#include <a_samp> //SA-MP Team


/*

@ Titel: Anti-Vehicle Repair
@ Author: JustMe.77
@ Version: 1.0.0
@ Download: <https://github.com/JustMe77/Anti-Vehiclerepair>


Credits: 

@ MP2 - PNS Coordinates
@ Jelly23 - Scripting Support
@ Kaliber - Scripting Support

Available Callback:

@ OnPlayerVehicleHealthHack

Protected/Hooked Callbacks / Functions:

@ OnPlayerDisconnect
@ OnVehicleSpawn
@ OnEnterExitModShop
@ OnPlayerStateChange

@ AddStaticVehicle
@ AddStaticVehicleEx
@ CreateVehicle
@ RepairVehicle
@ SetVehicleToRespawn
@ SetVehicleHealth

*/


#define MAX_VEHICLEHEALTH 999.0

static Checkvehicle[MAX_PLAYERS];
static bool:PlayerInModShop[MAX_PLAYERS];

static const Float:PAYNSPRAY[][] = 
{
  {1025.05, -1024.23, 32.1}, // LS Temple
  {487.68, -1740.87, 11.13}, // LS Santa Maria
  {-1420.73, 2583.37, 55.56}, // El Quebrados
  {-1904.39, 284.97, 40.75}, // Wang Cars
  {-2425.91, 1022.33, 50.10}, // Juniper Hill
  {1975.60, 2162.16, 10.77}, // LV Redsands
  {2065.38, -1831.51, 13.25}, // Idlewood
  {-99.55, 1118.36, 19.44}, // Fort Carson
  {721.07, -455.94, 16.04}, // Dillimore
  {2393.74, 1493.01, 10.52} // LV Unused (Pyramid)
};

forward CheckVehicleHealth(playerid);
forward OnPlayerVehicleHealthHack(playerid);



//Callbacks



public OnPlayerDisconnect(playerid, reason)
{
  KillTimer(Checkvehicle[playerid]);

  #if defined AntiRepair_OnPlayerDisconnect
  return AntiRepair_OnPlayerDisconnect(playerid, reason);
  #else
  return 1;
  #endif
}

public OnVehicleSpawn(vehicleid)
{
  SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);

  #if defined AntiRepair_OnVehicleSpawn
  return AntiRepair_OnVehicleSpawn(vehicleid);
  #else
  return 1;
  #endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) 
    {
      Checkvehicle[playerid] = SetTimerEx("CheckVehicleHealth", 1000, 1, "i",  playerid);
    }

    if(oldstate == PLAYER_STATE_DRIVER && newstate == PLAYER_STATE_ONFOOT)
    {
      KillTimer(Checkvehicle[playerid]);
    }

  #if defined AntiRepair_OnPlayerStateChange
  return AntiRepair_OnPlayerStateChange(playerid, newstate, oldstate);
  #else
  return 1;
  #endif
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
  if(enterexit == 1) // Player entered a Mod Shop
  {
    PlayerInModShop[playerid] = true;
  }

  if(enterexit == 0) // Player left a Mod Shop
  {
    SetVehicleHealth(GetPlayerVehicleID(playerid), MAX_VEHICLEHEALTH);
    PlayerInModShop[playerid] = false;
  }

  #if defined AntiRepair_OnEnterExitModShop
  return AntiRepair_OnEnterExitModShop(playerid, enterexit, interiorid);
  #else
  return 1;
  #endif
}

public CheckVehicleHealth(playerid)
{
  new Float:Vehiclehealth;
  GetVehicleHealth(GetPlayerVehicleID(playerid), Vehiclehealth);

  for(new i=0; i<sizeof(PAYNSPRAY); i++)
  {
    if(IsPlayerInRangeOfPoint(playerid, 10, PAYNSPRAY[i][0], PAYNSPRAY[i][1], PAYNSPRAY[i][2]))
    {
      if(Vehiclehealth == 1000 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
      {
        SetVehicleHealth(GetPlayerVehicleID(playerid), MAX_VEHICLEHEALTH);
        return 1;
      }
    }
  }

  if(Vehiclehealth == 1000 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && PlayerInModShop[playerid] == false)
  {
    SetVehicleToRespawn(GetPlayerVehicleID(playerid));
    OnPlayerVehicleHealthHack(playerid);
    return 1;
  }

  return 1;
}

//Callback Hooks

#if defined _ALS_OnPlayerDisconnect
#undef OnPlayerDisconnect
#else
#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect AntiRepair_OnPlayerDisconnect
#if defined AntiRepair_OnPlayerDisconnect
forward AntiRepair_OnPlayerDisconnect();
#endif

#if defined _ALS_OnVehicleSpawn
#undef OnVehicleSpawn
#else
#define _ALS_OnVehicleSpawn
#endif

#define OnVehicleSpawn AntiRepair_OnVehicleSpawn
#if defined AntiRepair_OnVehicleSpawn
forward AntiRepair_OnVehicleSpawn();
#endif

#if defined _ALS_OnPlayerStateChange
#undef OnPlayerStateChange
#else
#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange AntiRepair_OnPlayerStateChange
#if defined AntiRepair_OnPlayerStateChange
forward AntiRepair_OnPlayerStateChange();
#endif


#if defined _ALS_OnEnterExitModShop
#undef OnEnterExitModShop
#else
#define _ALS_OnEnterExitModShop
#endif

#define OnEnterExitModShop AntiRepair_OnEnterExitModShop
#if defined AntiRepair_OnEnterExitModShop
forward AntiRepair_OnEnterExitModShop();
#endif


//Function Hooks

stock AddStaticVehicleHooked(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2)
{
  new vehicleid = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2);
  if(vehicleid)
  { 
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  return vehicleid;
}

#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif
 
#define AddStaticVehicle AddStaticVehicleHooked

stock AddStaticVehicleExHooked(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren=0)
{
  new vehicleid = AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay, addsiren);
  if(vehicleid)
  {
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  return vehicleid;
}

#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
 
#define AddStaticVehicleEx AddStaticVehicleExHooked


stock CreateVehicleHooked(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay, addsiren=0)
{
  new vehicleid = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay, addsiren);
  if(vehicleid)
  {
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  return vehicleid;
}

#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif
 
#define CreateVehicle CreateVehicleHooked


stock RepairVehicleHooked(vehicleid)
{
  RepairVehicle(vehicleid);
  if(vehicleid)
  {
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  return vehicleid;
}

#if defined _ALS_RepairVehicle
    #undef RepairVehicle
#else
    #define _ALS_RepairVehicle
#endif
 
#define RepairVehicle RepairVehicleHooked


stock SetVehicleToRespawnHooked(vehicleid)
{
  SetVehicleToRespawn(vehicleid);
  if(vehicleid)
  {
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  return vehicleid;
}

#if defined _ALS_SetVehicleToRespawn
    #undef SetVehicleToRespawn
#else
    #define _ALS_SetVehicleToRespawn
#endif
 
#define SetVehicleToRespawn SetVehicleToRespawnHooked


stock SetVehicleHealthHooked(vehicleid, Float:health)
{
  if(health > MAX_VEHICLEHEALTH)
  {
    SetVehicleHealth(vehicleid, MAX_VEHICLEHEALTH);
  }
  else
  {
    SetVehicleHealth(vehicleid, health);
  }
  return 1;
}

#if defined _ALS_SetVehicleHealth
    #undef SetVehicleHealth
#else
    #define _ALS_SetVehicleHealth
#endif
 
#define SetVehicleHealth SetVehicleHealthHooked
