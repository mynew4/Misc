/* 
* Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/> 
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

#ifndef FIRELANDS_H_

#define FIRELANDS_H_
#define MAX_ENCOUNTERS 7

#include "Map.h"
#include "Creature.h"
#define FirelandsScriptName "instance_firelands"

uint32 const EncounterCount = 7;

enum Datas
{
	DATA_BETHTILAC              	= 1,    
	DATA_RHYOLITH               	= 2,    
	DATA_ALYSRAZAR              	= 3,    
	DATA_SHANNOX                	= 4,    
	DATA_BALOROC                	= 5,    
	DATA_MAJORDOMUS			= 6,    
	DATA_RAGNAROS               	= 7,	
	DATA_BALEROC                	= 8,
	DATA_Crystal_Shard          	= 9,
    	DATA_CURRENT_ENCOUNTER_PHASE    = 10,
};

enum GameObjectIds

{
    
	GO_RAGNAROS_DOOR		= 209073,   
	GO_RAGNAROS_PLATFORM        	= 208835,   
	GO_CACHE_OF_THE_FIRELORD    	= 208967,
	GOB_SULFURON_BRIDGE		= 209251,
	GOB_DOOR_MAJORDOMO		= 208873,
	GOB_DOOR_BETHILAC		= 208877,
	GOB_DOOR_BALOROC		= 209066,
};

enum CreatureIds
{
    
    	BOSS_RAGNAROS                   = 52409,
	BOSS_ALYSRAZAR			= 52530,
	BOSS_BETHTILAC    		= 52498,
	BOSS_MAJORDOMUS			= 52571,
	BOSS_RHYOLITH			= 52558,
	BOSS_SHANNOX			= 53691,
	NPC_ENGULFING_FLAMES_TRIGGER    = 53485,
	NPC_SPLITTING_BLOW_TRIGGER      = 53393,
	NPC_MAGMA_POOL_TRIGGER          = 53729,
	NPC_PLATFORM_TRIGGER            = 53952,
	NPC_MAGMA_TRAP                  = 53086,
	NPC_SULFURAS_HAMMER             = 53420,
	NPC_SULFURAS_FLAME_WALL         = 38327,
	NPC_SULFURAS_SMASH_TARGET_1     = 53268,
	NPC_LAVA_WAVE                   = 53363,  
	NPC_SON_OF_FLAME                = 53140,	
	BOSS_BALEROC                    = 53494,	
	NPC_Crystal_Shard               = 53495,	
	NPC_BLAZING_MONSTROSITY_LEFT 	= 53786,    
	NPC_BLAZING_MONSTROSITY_RIGHT 	= 53791,    
	NPC_EGG_PILE 			= 53795,    
	NPC_HARBINGER_OF_FLAME 		= 53793,
	NPC_MOLTEN_EGG_TRASH 		= 53914,
	NPC_SMOULDERING_HATCHLING 	= 53794,
	NPC_SHANNOX_SPEAR		= 53752,
	NPC_RIPLIMB			= 53694,
	NPC_RAGEFACE			= 53695,
	NPC_CRYSTAL_TRAP		= 53713,
	NPC_CRYSTAL_PRISON		= 53819,
	NPC_CINDERWEB_SPINNER		= 52524,
	NPC_SPIRIT_OF_THE_FLAME		= 52593,
	NPC_LIVING_METEOR       	= 53500,
	NPC_MOLTEN_ELEMENTAR    	= 53189,
	NPC_CINDERWEB_DRONE 		= 52581,
	NPC_CINDERWEB_SPIDERLING 	= 52447,
	NPC_ENGORGED_BROODLING 		= 53745,
	NPC_SPIDERWEB_FILAMENT 		= 53082,
};
enum Phase
{
	PHASE_NON 			= 0,
	PHASE_SHANNOX_HAS_SPEER 	= 1,
	PHASE_SPEAR_ON_THE_GROUND 	= 2,
	PHASE_RIPLIMB_GOS_TO_SPEER 	= 3,
	PHASE_RIPLIMB_BRINGS_SPEER 	= 4,
	PHASE_BETHILAC_UPPER 		= 5,
	PHASE_BETHILAC_LOWER 		= 6, 
};
enum PathIDs
{
	PATH_SHANNOX 			= 154280940,
};
enum DataVar
{
	DATA_PHASE,
}; 
enum MovePoints
{
	POINT_HAMMER,
	POINT_RAGNAROS_DOWN,
	POINT_RAGNAROS_UP,
	POINT_RAGNAROS_STANDUP,
	POINT_SULFURAS_SMASH,
}; 
enum Actions
{
	ACTION_RAGNAROS_DOWN,
	ACTION_RAGNAROS_UP,
	ACTION_RAGNAROS_STANDUP,
	ACTION_SULFURAS_SMASH,
	ACTION_SAY_RIPLIMB,
};

class DelayedAttackStartEvent : public BasicEvent
{    
	public:
        
		DelayedAttackStartEvent(Creature* owner) : 
			_owner(owner) { }        
		bool Execute(uint64 /*e_time*/, uint32 /*p_time*/)        
		{      
			_owner->AI()->DoZoneInCombat(_owner, 200.0f);         
			return true;        
		}    
		private:        
			Creature* _owner;
};
template<class AI>CreatureAI* GetFirelandsAI(Creature* creature)
{    
	if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())        
	if (instance->GetInstanceScript())            
	if (instance->GetScriptId() == sObjectMgr->GetScriptId(FirelandsScriptName))                
		return new AI(creature);    
		return NULL;
}

#endif