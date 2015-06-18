/*
 
* Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 
*
 
* This program is free software; you can redistribute it and/or modify it
 
* under the terms of the GNU General Public License as published by the
 
* Free Software Foundation; either version 2 of the License, or (at your
 
* option) any later version.
 
*
 
* This program is distributed in the hope that it will be useful, but WITHOUT
 
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 
* more details.
 
*
 
* You should have received a copy of the GNU General Public License along
 
* with this program. If not, see <http://www.gnu.org/licenses/>.
 
*/



#ifndef DEF_DARKPORTAL_H

#define DEF_DARKPORTAL_H




enum DataTypes

{

    TYPE_MEDIVH                         = 1,

    TYPE_RIFT                           = 2,

    TYPE_AEONUS                         = 3,

    TYPE_TEMPORUS                       = 4,

    TYPE_CRONO_LORD_DEJA                = 5,

    DATA_MEDIVH                         = 10,

    DATA_PORTAL_COUNT                   = 11,

    DATA_SHIELD                         = 12

};



enum WorldStateIds

{

    WORLD_STATE_BM                      = 2541,

    WORLD_STATE_BM_SHIELD               = 2540,

    WORLD_STATE_BM_RIFT                 = 2784

};



enum QuestIds

{

    QUEST_OPENING_PORTAL                = 10297,

    QUEST_MASTER_TOUCH                  = 9836

};



enum CreatureIds

{

    NPC_MEDIVH                          = 15608,

    NPC_TIME_RIFT                       = 17838,

    C_TIME_KEEPER                     	= 17918,

    C_RKEEP                     	= 21104,

    C_RLORD                       	= 17839,

    C_DEJA                 		= 17879,

    C_TEMPO                        	= 17880,

    C_AEONUS                          	= 17881,

    C_ASSAS                		= 17835,

    C_WHELP                  		= 21818,

    C_CHRON            			= 17892,

    C_EXECU           			= 18994,

    C_VANQU            			= 18995

};


#endif

