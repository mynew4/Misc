/*
 * Copyright (C) 2005-2011 MaNGOS <http://www.getmangos.com/>
 *
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2006-2011 ScriptDev2 <http://www.scriptdev2.com/>
 *
 * Copyright (C) 2011- 2013 ArkCORE <http://www.arkania.net/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ScriptPCH.h"
#include "the_stonecore.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"

#define MAX_ENCOUNTER     4

class instance_the_stonecore : public InstanceMapScript
{
public:
    instance_the_stonecore() : InstanceMapScript("instance_the_stonecore", 725) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_the_stonecore_InstanceMapScript(map);
    }

    struct instance_the_stonecore_InstanceMapScript : public InstanceScript
    {
        instance_the_stonecore_InstanceMapScript(Map* map) : InstanceScript(map) {};

        uint64 corborus;
        uint64 slabhide;
        uint64 ozruk;
        uint64 highPriestessAzil;
        uint32 teamInInstance;
        uint32 encounter[MAX_ENCOUNTER];
        uint64 CorborusDoor;
        uint64 SlabhideDoor;

        void Initialize()
        {
             for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                 encounter[i] = NOT_STARTED;

             corborus = 0;
             slabhide = 0;
             ozruk = 0;
             highPriestessAzil = 0;
             CorborusDoor = 0;
             SlabhideDoor = 0;
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (encounter[i] == IN_PROGRESS)
                    return true;

             return false;
        }

        void OnCreatureCreate(Creature* creature)
        {
            Map::PlayerList const &players = instance->GetPlayers();

            if (!players.isEmpty())
            {
                if (Player* player = players.begin()->getSource())
                    teamInInstance = player->GetTeam();
            }

            switch(creature->GetEntry())
            {
                case BOSS_CORBORUS:
                    corborus = creature->GetGUID();
                    break;
                case BOSS_SLABHIDE:
                    slabhide = creature->GetGUID();
                    break;
                case BOSS_OZRUK:
                    ozruk = creature->GetGUID();
                    break;
                case BOSS_HIGH_PRIESTESS_AZIL:
                    highPriestessAzil = creature->GetGUID();
                    break;
            }
        }
        
        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case 207343:
                    CorborusDoor = go->GetGUID();
                break;
                
                case 204381:
                    SlabhideDoor = go->GetGUID();
                break;
            }
        }
                
        uint64 GetData64(uint32 identifier) const
        {
            switch(identifier)
            {
                case DATA_CORBORUS:                     return corborus;
                case DATA_SLABHIDE:                     return slabhide;
                case DATA_OZRUK:                        return ozruk;
                case DATA_HIGH_PRIESTESS_AZIL:          return highPriestessAzil;
                case DATA_CORBORUSDOOR:                 return CorborusDoor;
                case DATA_SLABHIDEDOOR:                 return SlabhideDoor;              
            }

            return 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch(type)
            {
                case DATA_CORBORUS_EVENT:
                    if(data == IN_PROGRESS)
                        HandleGameObject(CorborusDoor, false);
                    else
                        HandleGameObject(CorborusDoor, true);
                    encounter[0] = data;
                    break;
                    
                case DATA_SLABHIDE_EVENT:
                    if(data == IN_PROGRESS)
                        HandleGameObject(SlabhideDoor, false);
                    else
                        HandleGameObject(SlabhideDoor, true);
                    encounter[1] = data;
                    break;
                    
                case DATA_OZRUK_EVENT:
                    encounter[2] = data;
                    break;
                    
                case DATA_HIGH_PRIESTESS_AZIL_EVENT:
                    encounter[3] = data;
                    break;
            }

            if (data == DONE)
               SaveToDB();
        }

        uint32 GetData(uint32 type) const
        {
            switch(type)
            {
                case DATA_CORBORUS_EVENT:                return encounter[0];
                case DATA_SLABHIDE_EVENT:                return encounter[1];
                case DATA_OZRUK_EVENT:                    return encounter[2];
                case DATA_HIGH_PRIESTESS_AZIL_EVENT:    return encounter[3];
            }

            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;

            std::ostringstream saveStream;
            saveStream << "P S " << encounter[0] << " " << encounter[1]  << " " << encounter[2]  << " " << encounter[3];

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;
            uint16 data0, data1, data2, data3;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> data0 >> data1 >> data2 >> data3;

            if (dataHead1 == 'P' && dataHead2 == 'S')
            {
                encounter[0] = data0;
                encounter[1] = data1;
                encounter[2] = data2;
                encounter[3] = data3;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (encounter[i] == IN_PROGRESS)
                        encounter[i] = NOT_STARTED;
            }
            else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };
};

void AddSC_instance_the_stonecore()
{
    new instance_the_stonecore();
}