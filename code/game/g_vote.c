/*
===========================================================================
Copyright (C) 2008-2009 Poul Sander

This file is part of Open Arena source code.

Open Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Open Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Open Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "g_local.h"

/*
==================
allowedVote
 *Note: Keep this in sync with allowedVote in ui_votemenu.c (except for cg_voteNames and g_voteNames)
==================
 */
#define MAX_VOTENAME_LENGTH 14 //currently the longest string is "/map_restart/\0" (14 chars)
int allowedVote(char *commandStr) {
    char tempStr[MAX_VOTENAME_LENGTH];
    int length;
    char voteNames[MAX_CVAR_VALUE_STRING];
    trap_Cvar_VariableStringBuffer( "g_voteNames", voteNames, sizeof( voteNames ) );
    if(!Q_stricmp(voteNames, "*" ))
        return qtrue; //if star, everything is allowed
    length = strlen(commandStr);
    if(length>MAX_VOTENAME_LENGTH-3)
    {
        //Error: too long
        return qfalse;
    }
    //Now constructing a string that starts and ends with '/' like: "/clientkick/"
    tempStr[0] = '/';
    strncpy(&tempStr[1],commandStr,length);
    tempStr[length+1] = '/';
    tempStr[length+2] = '\0';
    if(Q_stristr(voteNames,tempStr) != NULL)
        return qtrue;
    else
        return qfalse;
}

int allowedRef(char *commandStr) {
    char tempStr[MAX_VOTENAME_LENGTH*2];
    int length;
    char voteNames[MAX_CVAR_VALUE_STRING];
    trap_Cvar_VariableStringBuffer( "g_refNames", voteNames, sizeof( voteNames ) );
    if(!Q_stricmp(voteNames, "*" ))
        return qtrue; //if star, everything is allowed
    length = strlen(commandStr);
    if(length>MAX_VOTENAME_LENGTH*2-3)
    {
        //Error: too long
        return qfalse;
    }
    //Now constructing a string that starts and ends with '/' like: "/clientkick/"
    tempStr[0] = '/';
    strncpy(&tempStr[1],commandStr,length);
    tempStr[length+1] = '/';
    tempStr[length+2] = '\0';
    if(Q_stristr(voteNames,tempStr) != NULL)
        return qtrue;
    else
        return qfalse;
}

/*
==================
getMappage
==================
 */

t_mappage getMappage(int page) {
	t_mappage result;
	fileHandle_t	file;
	char *token,*pointer;
	char buffer[MAX_MAPNAME_BUFFER];
	int i, nummaps,maplen;

	memset(&result,0,sizeof(result));
        memset(&buffer,0,sizeof(buffer));

	//Check if there is a votemaps.cfg
	trap_FS_FOpenFile("votemaps.cfg",&file,FS_READ);
	if(file)
	{
		//there is a votemaps.cfg file, take allowed maps from there
		trap_FS_Read(&buffer,sizeof(buffer),file);
		pointer = buffer;
		token = COM_Parse(&pointer);
		if(token[0]==0 && page == 0) {
			//First page empty
			result.pagenumber = -1;
                        trap_FS_FCloseFile(file);
			return result;
		}
		//Skip the first pages
		for(i=0;i<MAPS_PER_PAGE*page;i++) {
			token = COM_Parse(&pointer);
		}
		if(!token || token[0]==0) {
			//Page empty, return to first page
                        trap_FS_FCloseFile(file);
			return getMappage(0);
		}
		//There is an actual page:
                result.pagenumber = page;
		for(i=0;i<MAPS_PER_PAGE && token;i++) {
			Q_strncpyz(result.mapname[i],token,MAX_MAPNAME);
			token = COM_Parse(&pointer);
		}
                trap_FS_FCloseFile(file);
		return result;
	}
        //There is no votemaps.cfg file, find filelist of maps
        nummaps = trap_FS_GetFileList("maps",".bsp",buffer,sizeof(buffer));

        if(nummaps && nummaps<=MAPS_PER_PAGE*page)
            return getMappage(0);

        pointer = buffer;
        result.pagenumber = page;

        for (i = 0; i < nummaps; i++, pointer += maplen+1) {
		maplen = strlen(pointer);
                if(i>=MAPS_PER_PAGE*page && i<MAPS_PER_PAGE*(page+1)) {
                    Q_strncpyz(result.mapname[i-MAPS_PER_PAGE*page],pointer,maplen-3);
                }
	}
        return result;

}

/*
==================
allowedMap
==================
 */

int allowedMap(char *mapname) {
    int length;
    fileHandle_t	file;           //To check that the map actually exists.
    char                buffer[MAX_MAPS_TEXT];
    char                *token,*pointer;
    qboolean            found;

    trap_FS_FOpenFile(va("maps/%s.bsp",mapname),&file,FS_READ);
    if(!file)
        return qfalse; //maps/MAPNAME.bsp does not exist
    trap_FS_FCloseFile(file);

    //Now read the file votemaps.cfg to see what maps are allowed
    trap_FS_FOpenFile("votemaps.cfg",&file,FS_READ);

    if(!file)
        return qtrue; //if no file, everything is allowed
    length = strlen(mapname);
    if(length>MAX_MAPNAME_LENGTH-3)
    {
        //Error: too long
        trap_FS_FCloseFile(file);
        return qfalse;
    }

    //Add file checking here
    trap_FS_Read(&buffer,MAX_MAPS_TEXT,file);
    found = qfalse;
    pointer = buffer;
    token = COM_Parse(&pointer);
    while(token[0]!=0 && !found) {
        if(!Q_stricmp(token,mapname))
            found = qtrue;
        token = COM_Parse(&pointer);
    }

    trap_FS_FCloseFile(file);
    //The map was not found
    return found;
}

/*
==================
allowedGametype
==================
 */
#define MAX_GAMETYPENAME_LENGTH 5 //currently the longest string is "/12/\0" (5 chars)
int allowedGametype(char *gametypeStr) {
    char tempStr[MAX_GAMETYPENAME_LENGTH];
    int length;
    char voteGametypes[MAX_CVAR_VALUE_STRING];
    trap_Cvar_VariableStringBuffer( "g_voteGametypes", voteGametypes, sizeof( voteGametypes ) );
    if(!Q_stricmp(voteGametypes, "*" ))
        return qtrue; //if star, everything is allowed
    length = strlen(gametypeStr);
    if(length>MAX_GAMETYPENAME_LENGTH-3)
    {
        //Error: too long
        return qfalse;
    }
    tempStr[0] = '/';
    strncpy(&tempStr[1],gametypeStr,length);
    tempStr[length+1] = '/';
    tempStr[length+2] = '\0';
    if(Q_stristr(voteGametypes,tempStr) != NULL)
        return qtrue;
    else {
        return qfalse;
    }
}

/*
==================
allowedTimelimit
==================
 */
int allowedTimelimit(int limit) {
    int min, max;
    min = g_voteMinTimelimit.integer;
    max = g_voteMaxTimelimit.integer;
    if(limit<min && limit != 0)
        return qfalse;
    if(max!=0 && limit>max)
        return qfalse;
    if(limit==0 && max > 0)
        return qfalse;
    return qtrue;
}

/*
==================
allowedFraglimit
==================
 */
int allowedFraglimit(int limit) {
    int min, max;
    min = g_voteMinFraglimit.integer;
    max = g_voteMaxFraglimit.integer;
    if(limit<min && limit != 0)
        return qfalse;
    if(max != 0 && limit>max)
        return qfalse;
    if(limit==0 && max > 0)
        return qfalse;
    return qtrue;
}

#define MAX_CUSTOM_VOTES   48 

char            custom_vote_info[4096];

/*
==================
VoteParseCustomVotes
 *Reads the file votecustom.cfg. Finds all the commands that can be used with
 *"/callvote custom COMMAND" and adds the commands to custom_vote_info
==================
 */
int VoteParseCustomVotes( void ) {
    fileHandle_t	file;
    char            buffer[16*1024];
    int             commands;
    char	*token,*pointer;

    trap_FS_FOpenFile("votecustom.cfg",&file,FS_READ);

    if(!file)
        return 0;

    memset(&buffer,0,sizeof(buffer));
    memset(&custom_vote_info,0,sizeof(custom_vote_info));

    commands = 0;

    trap_FS_Read(&buffer,sizeof(buffer),file);
    
    pointer = buffer;

    while ( commands < MAX_CUSTOM_VOTES ) {
	token = COM_Parse( &pointer );
        if ( !token[0] ) {
            break;
	}

        if ( !strcmp( token, "votecommand" ) ) {
            token = COM_Parse( &pointer );
            Q_strcat(custom_vote_info,sizeof(custom_vote_info),va("%s ",token));
            commands++;
	}
    }

    trap_FS_FCloseFile(file);

    return commands;
}
/*
==================
getCustomVote
 *Returns a custom vote. This will go beyond MAX_CUSTOM_VOTES.
==================
 */
t_customvote getCustomVote(char* votecommand) {
    t_customvote result;
    fileHandle_t	file;
    char            buffer[16*1024];
    char	*token,*pointer;
    char	key[MAX_TOKEN_CHARS];

    trap_FS_FOpenFile("votecustom.cfg",&file,FS_READ);

    if(!file) {
        memset(&result,0,sizeof(result));
        return result;
    }

    memset(&buffer,0,sizeof(buffer));

    trap_FS_Read(&buffer,sizeof(buffer),file);

    pointer = buffer;

    while ( qtrue ) {
	token = COM_Parse( &pointer );
        if ( !token[0] ) {
            break;
	}

        if ( strcmp( token, "{" ) ) {
		Com_Printf( "Missing { in votecustom.cfg\n" );
		break;
	}

        memset(&result,0,sizeof(result));

        while ( 1 ) {
            token = COM_ParseExt( &pointer, qtrue );
            if ( !token[0] ) {
                    Com_Printf( "Unexpected end of customvote.cfg\n" );
                    break;
            }
            if ( !strcmp( token, "}" ) ) {
                    break;
            }
            Q_strncpyz( key, token, sizeof( key ) );

            token = COM_ParseExt( &pointer, qfalse );
            if ( !token[0] ) {
                Com_Printf("Invalid/missing argument to %s in customvote.cfg\n",key);
            }
            if(!Q_stricmp(key,"votecommand")) {
                Q_strncpyz(result.votename,token,sizeof(result.votename));
            } else if(!Q_stricmp(key,"displayname")) {
                Q_strncpyz(result.displayname,token,sizeof(result.displayname));
            } else if(!Q_stricmp(key,"command")) {
                Q_strncpyz(result.command,token,sizeof(result.command));
            } else {
                Com_Printf("Unknown key in customvote.cfg: %s\n",key);
            }

	}

        if(!Q_stricmp(result.votename,votecommand)) {
            return result;
        }
    }

    //Nothing was found
    memset(&result,0,sizeof(result));
        return result;
}

/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( level.voteExecuteTime && level.voteExecuteTime < level.time ) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
	}
	if ( !level.voteTime ) {
		return;
	}
	if ( level.time - level.voteTime >= VOTE_TIME ) {
            //Let pass if there was at least twice as many for as against
            if ( level.voteYes > level.voteNo*2 ) {
                trap_SendServerCommand( -1, "print \"Vote passed. At least 2 of 3 voted yes\n\"" );
		level.voteExecuteTime = level.time + 3000;
            } else
            //Let pass if there is more yes than no and at least 2 yes votes and at least 30% yes of all on the server
            if ( level.voteYes > level.voteNo && level.voteYes >= 2 && (level.voteYes*10)>=(level.numVotingClients*3) ) {
                trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
		level.voteExecuteTime = level.time + 3000;
            } else
                trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
	} else {
		// ATVI Q3 1.32 Patch #9, WNF
		if ( level.voteYes > (level.numVotingClients)/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
			level.voteExecuteTime = level.time + 3000;
		} else if ( level.voteNo >0 && level.voteNo >= (level.numVotingClients)/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );

}

/*
 * G_PickMap_f
Internal function of mapchooser.
usage: __pickmap <nummaps> [ <mapname1> <arena1> ] * nummaps
assumes nummaps < 10;
*/


void G_PickMap_f ( void ) {
	char		str[MAX_TOKEN_CHARS];
  int     len,num,i;
  int     tmp;
  int     winner=0,winamount=0;
  int     arena;
  int     nummaps;
	trap_Argv( 1, str, sizeof( str ) );
	//trap_Argv( 2, otherserver, sizeof( otherserver ) );
  if ( strlen(str) != 1 || str[0]<'0' || str[0]>'9' ) {
      G_Printf("pickmap is an internal function part of the mapchooser system\n");
      return;
  }
  nummaps = atoi(str);
  
  // parse the vote_yes string to pick the winner
  trap_GetConfigstring(CS_VOTE_YES, str, sizeof( str ));
  len = strlen(str);
  if (len<(nummaps+1)*2) {
      G_Printf("pickmap expected more data in CS_VOTE_YES\n");
      return;
  }
  i = str[0]-'0';
  if (i!=nummaps || str[1]!=' ') {
      G_Printf("pickmap expected different nummaps in CS_VOTE_YES %s\n",str);
      return;
  }
  // use i to scan the CS_VOTE_YES string, num is the current option we are looking at
  // tmp is the parsed decimal value of the current option
  i=2;
  num=1;
  tmp=0;
  while (i<len) {
      if (str[i]==' ') {
          if (str[i-1]!=' ' && tmp>winamount) {
              winner=num;
              winamount=tmp;
          } 
          tmp=0;
          num++;
      } else if (str[i]>='0' && str[i]<='9') {
          tmp*=10;
          tmp += str[i]-'0';
      } else {
          G_Printf("pickmap expected different data in CS_VOTE_YES %s\n",str);
          return;
      }

      i++;
  }
  // the last trailing space in CS_VOTE_YES will set num one higher than the nummaps
  if (num!=nummaps+1) {
      G_Printf("pickmap expected different nummaps, found %i not %i after in CS_VOTE_YES %s\n",num,nummaps,str);
      return;
  }

  if (winner<=0) {
      G_Printf("pickmap found no winner in CS_VOTE_YES\n");
      return;
  }

  // winner found
  // arena
	trap_Argv( 2 + ((winner-1)*2) + 1, str, sizeof( str ) );
  if ( strlen(str) != 1 || str[0]<'0' || str[0]>'9' ) {
      G_Printf("pickmap lockarena or 0 expected at arg %i\n", 2 + ((winner-1)*2) + 1);
      return;
  }
  arena = atoi(str);
	trap_Argv( 2 + ((winner-1)*2) + 0, str, sizeof( str ) );

  if (arena > 0) {
      trap_SendConsoleCommand( EXEC_APPEND, va("map %s; g_lockArena %i",str,arena));
  } else {
      trap_SendConsoleCommand( EXEC_APPEND, va("map %s",str));
  }


}

/*
==================
CountVotes

 Iterates through all the clients and counts the votes
==================
*/
void CountVotes( void ) {
    int i,j;
    int yes=0,no=0;
    int mapVotes[10];
    //int winner=0;
    int winamount=0;
    // very short string
    char string[64],entry[6];
    int stringlength=0;
    if (level.mapVote) {
        for (i=0;i<level.mapVote+1;i++) {
            mapVotes[i]=0;
        }
    }

    level.numVotingClients=0;

    for ( i = 0 ; i < level.maxclients ; i++ ) {
            if ( level.clients[ i ].pers.connected != CON_CONNECTED )
                continue; //Client was not connected

            if (level.clients[i].sess.sessionTeam == TEAM_SPECTATOR)
		continue; //Don't count spectators

            if ( g_entities[i].r.svFlags & SVF_BOT )
                continue; //Is a bot

            //The client can vote
            level.numVotingClients++;

            // map choose vote going on, count differently
            if (level.mapVote) {
                if(level.clients[i].vote>0 && level.clients[i].vote<=level.mapVote) {
                    mapVotes[level.clients[i].vote]++;
                }
            } else {
                //Did the client vote yes?
                if(level.clients[i].vote>0)
                    yes++;

                //Did the client vote no?
                if(level.clients[i].vote<0)
                    no++;
            }
    }

    if (level.mapVote) {

        // first in string is number of maps to choose from
        Com_sprintf (entry, sizeof(entry), "%i ",
                level.mapVote);
        j = strlen(entry);
        strcpy (string + stringlength, entry);
        stringlength += j;
        // next in string voteamount for each map
        for (i=1;i<level.mapVote+1;i++) {
            if(mapVotes[i]>winamount) {
                //winner = i;
                winamount = mapVotes[i];
            }
            Com_sprintf (entry, sizeof(entry), "%i ",
                    mapVotes[i]);
            j = strlen(entry);
            strcpy (string + stringlength, entry);
            stringlength += j;
        }
        level.voteYes = winamount;

        trap_SetConfigstring( CS_VOTE_YES, va("%s", string ) );

        // TODO: maybe add another config string for this instead of abusing CS_VOTE_YES
        // cleaner but that will break demo playback
        //trap_SetConfigstring( CS_VOTE_MAP, va("%s", string ) );
    } else {
        // not a mapvote but regular vote
        //See if anything has changed
        if(level.voteYes != yes) {
            level.voteYes = yes;
            trap_SetConfigstring( CS_VOTE_YES, va("%i", level.voteYes ) );
        }

        if(level.voteNo != no) {
            level.voteNo = no;
            trap_SetConfigstring( CS_VOTE_NO, va("%i", level.voteNo ) );
        }
    }
}

