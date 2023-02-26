/*
	Assist.inc by Aleks4k
	
	Description:
	  An include which allows checking who assisted in player's death.
	  Assists go to players who most recently assisted in kills **damageTick**.
	  This is an updated version of https://gist.github.com/billz0101/3002d3263bf082d137c9e45a2b3a050e made by billz0101 (as filterscript).
	  Advice:
		If your OnPlayerDamage/OnPlayerTakeDamage have return 0 (damage rejection) in gamemode/module,
		I suggest you to #include this include at very end of your gamemode or after including your modules. (If you are not using weapon-config)
		Why? Well if you reject damage in some cases, it probably means that you dont want player to get assistention from rejected damage.
	
	Start Date:
 	  26/02/2023 @ 13:01 AM.
	
	Last Update:
	  26/02/2023 @ 13:01 AM.
	
	Callbacks:
		OnPlayerAssist(playerid, deathid, killerid)
	    Description: Called when player (playerid) assists the killer (killerid) in the killing of a player (deathid).
	
	Definitions:
		MAX_ASSIST
		Description: The maximum amount of players that can assist in single death (excluding killerid).
	
	Credits:
		billz0101
*/
//==============================================================================//
#if defined _INC_ASSIST
	#endinput
#endif
#define _INC_ASSIST

#if !defined _samp_included
	#tryinclude <a_samp>
	#if !defined _samp_included
		#error "Please include a_samp before assist."
	#endif
#endif

#if !defined MAX_ASSIST
	#define MAX_ASSIST 2
#endif
//==================================Enumerators=================================//
enum killAssit {
	assisterID,
	damageTick,
}
//====================================Arrays====================================//
static AssistInfo[MAX_PLAYERS][MAX_ASSIST+1][killAssit];
//==================================Callbacks===================================//
forward OnPlayerAssist(playerid, deathid, killerid);
//==============================================================================//
public OnGameModeInit()
{
	for(new i = 0; i < MAX_PLAYERS; i++){
		for(new j = 0; j < MAX_ASSIST+1; j++){
			AssistInfo[i][j][assisterID] = INVALID_PLAYER_ID;
			AssistInfo[i][j][damageTick] = 0;
		}
	}
	#if defined assist_OnGameModeInit
		return assist_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit assist_OnGameModeInit
#if defined assist_OnGameModeInit
	forward assist_OnGameModeInit();
#endif
//==============================================================================//
public OnPlayerDisconnect(playerid, reason)
{
	for(new i = 0; i < MAX_ASSIST+1; i++){
		AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
		AssistInfo[playerid][i][damageTick] = 0;
	}
	#if defined assist_OnPlayerDisconnect
		return assist_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect assist_OnPlayerDisconnect
#if defined assist_OnPlayerDisconnect
	forward assist_OnPlayerDisconnect(playerid, reason);
#endif
//==============================================================================//
public OnPlayerConnect(playerid)
{
	for(new i = 0; i < MAX_ASSIST+1; i++){
		AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
		AssistInfo[playerid][i][damageTick] = 0;
	}
	#if defined assist_OnPlayerConnect
		return assist_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect assist_OnPlayerConnect
#if defined assist_OnPlayerConnect
	forward assist_OnPlayerConnect(playerid);
#endif
//==============================================================================//
public OnPlayerSpawn(playerid)
{
	for(new i = 0; i < MAX_ASSIST+1; i++){
		AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
		AssistInfo[playerid][i][damageTick] = 0;
	}
	#if defined assist_OnPlayerSpawn
		return assist_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn assist_OnPlayerSpawn
#if defined assist_OnPlayerSpawn
	forward assist_OnPlayerSpawn(playerid);
#endif
//==============================================================================//
#if defined _INC_WEAPON_CONFIG
	public OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart)
	{
		if(issuerid != INVALID_PLAYER_ID){
			new bool:recentlyAssisted;
			for(new i = 0; i < MAX_ASSIST+1; i++){
				//Check if player already assisted.
				if(AssistInfo[playerid][i][assisterID] == issuerid){
					recentlyAssisted = true;
					AssistInfo[playerid][i][damageTick] = GetTickCount();
					break;
				}
			}
			if(!recentlyAssisted){ //Add player to assistentions.
				new slot = 0, minimumTick = AssistInfo[playerid][0][damageTick];
				//We determine slot to place the player (placing them where damageTick is the least).
				for(new i = 0; i < MAX_ASSIST+1; i++){
					if(AssistInfo[playerid][i][assisterID] == INVALID_PLAYER_ID){
						slot = i;
						break;
					}
					if(minimumTick > AssistInfo[playerid][i][damageTick]){
						slot = i;
						minimumTick = AssistInfo[playerid][i][damageTick];
					}
				}
				AssistInfo[playerid][slot][assisterID] = issuerid;
				AssistInfo[playerid][slot][damageTick] = GetTickCount();
			}
		}
		#if defined assist_OnPlayerDamageDone
			return assist_OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnPlayerDamageDone
		#undef OnPlayerDamageDone
	#else
		#define _ALS_OnPlayerDamageDone
	#endif

	#define OnPlayerDamageDone assist_OnPlayerDamageDone
	#if defined assist_OnPlayerDamageDone
		forward assist_OnPlayerDamageDone(playerid, Float:amount, issuerid, weapon, bodypart);
	#endif
#else
	public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
	{
		if(issuerid != INVALID_PLAYER_ID){
			new bool:recentlyAssisted;
			for(new i = 0; i < MAX_ASSIST+1; i++){
				//Check if player already assisted.
				if(AssistInfo[playerid][i][assisterID] == issuerid){
					recentlyAssisted = true;
					AssistInfo[playerid][i][damageTick] = GetTickCount();
					break;
				}
			}
			if(!recentlyAssisted){ //Znači da nije u asistencijama već i treba ga dodati.
				new slot = 0, minimumTick = AssistInfo[playerid][0][damageTick];
				//We determine slot to place the player (placing them where damageTick is the least).
				for(new i = 0; i < MAX_ASSIST+1; i++){
					if(AssistInfo[playerid][i][assisterID] == INVALID_PLAYER_ID){
						slot = i;
						break;
					}
					if(minimumTick > AssistInfo[playerid][i][damageTick]){
						slot = i;
						minimumTick = AssistInfo[playerid][i][damageTick];
					}
				}
				AssistInfo[playerid][slot][assisterID] = issuerid;
				AssistInfo[playerid][slot][damageTick] = GetTickCount();
			}
		}
		#if defined assist_OnPlayerTakeDamage
			return assist_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnPlayerTakeDamage
		#undef OnPlayerTakeDamage
	#else
		#define _ALS_OnPlayerTakeDamage
	#endif

	#define OnPlayerTakeDamage assist_OnPlayerTakeDamage
	#if defined assist_OnPlayerTakeDamage
		forward assist_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
	#endif
#endif
//==============================================================================//
public OnPlayerDeath(playerid, killerid, reason)
{
	if(killerid != INVALID_PLAYER_ID){
		for(new i = 0; i < MAX_ASSIST+1; i++){
			//Killerid is always among assistentions because he cant kill if he don't deal damage first.
			if(AssistInfo[playerid][i][assisterID] == killerid){
				AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
			}
			if((AssistInfo[playerid][i][assisterID] != INVALID_PLAYER_ID) && (AssistInfo[playerid][i][assisterID] != killerid))
			{
				#if defined _players_included
					if(IsPlayerConnected(AssistInfo[playerid][i][assisterID]))
						CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
				#else
					//Check if playerid != INVALID_PLAYER_ID in OnPlayerAssist at your implementation.
					CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
				#endif
				AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
				AssistInfo[playerid][i][damageTick] = 0;
			}
		}
	} else {
		new howmany; //Check how many players assisted (if less than MAX_ASSIST+1 we dont need additional checks).
		for(new i = 0; i < MAX_ASSIST+1; i++){
			if(AssistInfo[playerid][i][assisterID] != INVALID_PLAYER_ID){
				howmany++;
			}
		}
		if(howmany == MAX_ASSIST+1){ //Here we should take MAX_ASSIST most recent assistentions from AssistInfo.
			new slot = 0, minimumTick = AssistInfo[playerid][0][damageTick];
			for(new i = 1; i < MAX_ASSIST+1; i++){
				if(minimumTick > AssistInfo[playerid][i][damageTick]){
					slot = i;
					minimumTick = AssistInfo[playerid][i][damageTick];
				}
			}
			for(new i = 0; i < MAX_ASSIST+1; i++){
				if(i != slot){ //Dajemo svima assist osim onom koji je najranije asistiro.
					#if defined _players_included
						if(IsPlayerConnected(AssistInfo[playerid][i][assisterID]))
							CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
					#else
						//Check if playerid != INVALID_PLAYER_ID in OnPlayerAssist at your implementation.
						CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
					#endif
					AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
					AssistInfo[playerid][i][damageTick] = 0;
				}
			}
		} else {
			for(new i = 0; i < MAX_ASSIST+1; i++){
				if(AssistInfo[playerid][i][assisterID] != INVALID_PLAYER_ID){
					#if defined _players_included
						if(IsPlayerConnected(AssistInfo[playerid][i][assisterID]))
							CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
					#else
						//Check if playerid != INVALID_PLAYER_ID in OnPlayerAssist at your implementation.
						CallLocalFunction("OnPlayerAssist", "iii", AssistInfo[playerid][i][assisterID], playerid, killerid);
					#endif
					AssistInfo[playerid][i][assisterID] = INVALID_PLAYER_ID;
					AssistInfo[playerid][i][damageTick] = 0;
				}
			}
		}
	}
	
	#if defined assist_OnPlayerDeath
		return assist_OnPlayerDeath(playerid, killerid, reason);
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath assist_OnPlayerDeath
#if defined assist_OnPlayerDeath
	forward assist_OnPlayerDeath(playerid, killerid, reason);
#endif
//==============================================================================//
