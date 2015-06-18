/*
 
* Copyright (C) 2012-2013 PhoenixiaCore
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

#include "ScriptPCH.h"

/*####
# npc_flameward
####*/

class npc_flameward : public CreatureScript
{
public:
    npc_flameward() : CreatureScript("npc_flameward") { }
    struct npc_flamewardAI : public ScriptedAI
    {
        npc_flamewardAI(Creature* pCreature) : ScriptedAI(pCreature) { }
        uint32 timer;
        bool QuestInProgress;
        uint64 PlayerGUID;
        void Reset()
        {
            PlayerGUID = NULL;
            QuestInProgress = false;
            timer = 60000;
        }
        void UpdateAI(const uint32 diff)
        {
            if(timer <= diff)
            {
                if (PlayerGUID)
                    if (Player *pPlayer = Unit::GetPlayer(*me, PlayerGUID))
                        if (pPlayer->GetQuestStatus(25502) == QUEST_STATUS_INCOMPLETE)
                            pPlayer->CastSpell(pPlayer,75471,true);                            
            } else timer-=diff;
        }
    };
    CreatureAI *GetAI(Creature *creature) const
    {
        return new npc_flamewardAI(creature);
    }
};

/*####
# go_flameward
####*/

class go_flameward : public GameObjectScript
{
public:
    go_flameward() : GameObjectScript("go_flameward") { }    
    bool OnGossipHello(Player* player, GameObject* go)
    {
        if(player->GetQuestStatus(25502) == QUEST_STATUS_INCOMPLETE)
        {
            player->CastSpell(player,75470,true);
            if(Creature* flamew = player->SummonCreature(40460, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), go->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 61000)) //1 minute
            {
                CAST_AI(npc_flameward::npc_flamewardAI, flamew->AI())->QuestInProgress = true;
                CAST_AI(npc_flameward::npc_flamewardAI, flamew->AI())->PlayerGUID = player->GetGUID();
            }
        }
        return true;
    }
};
void AddSC_mount_hyjal()
{
    new go_flameward;
    new npc_flameward;
}