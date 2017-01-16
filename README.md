# Anti-Vehiclerepair
Include to check if a player repaired his vehicle using illegal modifications

Available Callback:

```

public OnPlayerVehicleHealthHack(playerid)
{
  return 1;
}

```

Example Usage


```

public OnPlayerVehicleHealthHack(playerid)
{
  new str[128];
  format(str, sizeof(str),"{%06x}%s {FFFFFF}has been {FF0000}banned{FFFFFF} for using illegal {FF0000}Vehicle-Repair Hacks.", GetPlayerColor(playerid) >>> 8, GetName(playerid));
  SendClientMessageToAll(-1, str);
  Ban(playerid);
  return 1;
}

stock GetName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	return name;
}

```
