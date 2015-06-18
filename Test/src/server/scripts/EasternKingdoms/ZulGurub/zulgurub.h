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



#ifndef DEF_ZULGURUB_H

#define DEF_ZULGURUB_H



uint32 const EncounterCount = 19;



#define ZGScriptName "instance_zulgurub"



enum DataTypes

{

    DATA_JEKLIK             = 0,  // Main boss

    DATA_VENOXIS            = 1,  // Main boss

    DATA_MARLI              = 2,  // Main boss

    DATA_ARLOKK             = 3,  // Main boss

    DATA_THEKAL             = 4,  // Main boss

    DATA_HAKKAR             = 5,  // End boss

    DATA_MANDOKIR           = 6,  // Optional boss

    DATA_JINDO              = 7,  // Optional boss

    DATA_GAHZRANKA          = 8,  // Optional boss

    DATA_EDGE_OF_MADNESS    = 9,  // Optional Event Edge of Madness - one of: Gri'lek, Renataki, Hazza'rah, or Wushoolay

    DATA_LORKHAN            = 10, // Zealot Lor'Khan add to High priest Thekal!

    DATA_ZATH               = 11, // Zealot Zath add to High priest Thekal!

    DATA_OHGAN              = 12, // Bloodlord Mandokir's raptor mount

    TYPE_EDGE_OF_MADNESS    = 13, // Boss storage
    DATA_ZANZIL		    = 14
,
    DATA_KILNARA            = 15,
    DATA_HAZZARAH           = 16, // Cache of Madness
    DATA_RENATAKI           = 17,
    DATA_WUSHOOLAY          = 18,
    DATA_GRILEK             = 19,
    DATA_JINDOR_TRIGGER, 	  // Jin'do the Godbreaker
};



enum CreatureIds

{

    NPC_ARLOKK              	= 14515, // Arlokk Event

    NPC_PANTHER_TRIGGER     	= 15091, // Arlokk Event

    NPC_ZULIAN_PROWLER      	= 15101, // Arlokk Event

    NPC_ZEALOT_LORKHAN      	= 11347,

    NPC_ZEALOT_ZATH         	= 11348,

    NPC_HIGH_PRIEST_THEKAL  	= 14509,

    NPC_JINDO_THE_HEXXER    	= 11380,

    NPC_NIGHTMARE_ILLUSION  	= 15163,

    NPC_SHADE_OF_JINDO      	= 14986,

    NPC_SACRIFICED_TROLL    	= 14826,

    NPC_MANDOKIR            	= 11382, // Mandokir Event

    NPC_OHGAN               	= 14988, // Mandokir Event

    NPC_VILEBRANCH_SPEAKER  	= 11391, // Mandokir Event

    NPC_CHAINED_SPIRT       	= 15117,  // Mandokir Event
    NPC_VENOXIS                 = 52155,
    NPC_MANDOKIR1                = 52151,
    NPC_KILNARA                 = 52059,
    NPC_ZANZIL                  = 52053,
    NPC_JINDO                   = 52148,
    NPC_HAZZARAH                = 52271, // Cache of Madness
    NPC_RENATAKI                = 52269,
    NPC_WUSHOOLAY               = 52286,
    NPC_GRILEK                  = 52258,
    NPC_CHAINED_SPIRIT          = 52156, // Bloodlord Mandokir
    NPC_OHGAN1                 = 52157,
    NPC_JINDO_TRIGGER           = 52150, // Jin'do the Godbreaker
    NPC_SPIRIT_OF_HAKKAR        = 52222,
    NPC_SHADOW_OF_HAKKAR        = 52650

};



enum GameobjectIds

{

    GO_FORCEFIELD           	= 180497, // Arlokk Event
    GO_GONG_OF_BETHEKK      	= 180526, // Arlokk Event
    GO_VENOXIS_COIL             = 208844, // High Priest Venoxis
    GO_ARENA_DOOR_1             = 208845, // Bloodlord Mandokir
    GO_ARENA_DOOR_2             = 208847,
    GO_ARENA_DOOR_3             = 208848,
    GO_ARENA_DOOR_4             = 208846,
    GO_ARENA_DOOR_5             = 208849,
    GO_ZANZIL_DOOR              = 208850, // Zanzil
    GO_THE_CACHE_OF_MADNESS_DOOR= 208843, // Cache of Madness
};



template<class AI>
CreatureAI* GetZulGurubAI(Creature* creature)

{

    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())

        if (instance->GetInstanceScript())

            if (instance->GetScriptId() == sObjectMgr->GetScriptId(ZGScriptName))

                return new AI(creature);

    return NULL;

}


#endif