/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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

/* ScriptData
SDName: Icecrown
SD%Complete: 100
SDComment: Quest support: 12807
SDCategory: Icecrown
EndScriptData */

/* ContentData
npc_arete
EndContentData */

#include "ScriptPCH.h"

/*######
## npc_vendor_argent_tournament
######*/
const uint32 ArgentTournamentVendor[10][4] =
{
    {33553,13726,2,14460}, // Orc
    {33554,13726,8,14464}, // Troll
    {33556,13728,6,14458}, // Tauren
    {33555,13729,5,14459}, // Undead
    {33557,13731,10,14465}, // Blood Elf
    {33307,13699,1,14456}, // Human
    {33310,13713,3,14457}, // Dwarf
    {33653,13725,4,14463}, // Night Elf
    {33650,13723,7,14462}, // Gnome
    {33657,13724,11,14461} // Draenei
};

class npc_vendor_argent_tournament : public CreatureScript
{
public:
    npc_vendor_argent_tournament() : CreatureScript("npc_vendor_argent_tournament"){}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        bool npcCheck = false;
        bool questCheck = false;
        bool raceCheck = false;
        uint32 textId = 0;

        for(int i = 0; (i < 10) && !npcCheck; i++)
        {
            if(pCreature->GetEntry() == ArgentTournamentVendor[i][0])
            {
                npcCheck = true;
                questCheck = pPlayer->GetQuestStatus(ArgentTournamentVendor[i][1]) == QUEST_STATUS_COMPLETE;
                raceCheck = pPlayer->getRace() == ArgentTournamentVendor[i][2];
                textId = ArgentTournamentVendor[i][3];
            }
        }

        if(questCheck || raceCheck)
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
        else
            pPlayer->SEND_GOSSIP_MENU(textId, pCreature->GetGUID());
        return true;
    }

};

/*######
* quest_givers_argent_tournament
######*/

class quest_givers_argent_tournament : public CreatureScript
{
public:
    quest_givers_argent_tournament() : CreatureScript("quest_givers_argent_tournament"){}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        //uint64 const guid = pCreature->GetGUID();

        if (pCreature->isQuestGiver())
        {
            Object *pObject = (Object*)pCreature;
            QuestRelations* pObjectQR = sObjectMgr.GetCreatureQuestRelationMap();
            QuestRelations* pObjectQIR = sObjectMgr.GetCreatureQuestInvolvedRelation();

            QuestMenu &qm = pPlayer->PlayerTalkClass->GetQuestMenu();
            qm.ClearMenu();

            for (QuestRelations::const_iterator i = pObjectQIR->lower_bound(pObject->GetEntry()); i != pObjectQIR->upper_bound(pObject->GetEntry()); ++i)
            {
                uint32 quest_id = i->second;
                QuestStatus status = pPlayer->GetQuestStatus(quest_id);
                if (status == QUEST_STATUS_COMPLETE && !pPlayer->GetQuestRewardStatus(quest_id))
                    qm.AddMenuItem(quest_id, 4);
                else if (status == QUEST_STATUS_INCOMPLETE)
                    qm.AddMenuItem(quest_id, 4);
                //else if (status == QUEST_STATUS_AVAILABLE)
                // qm.AddMenuItem(quest_id, 2);
            }

            bool EligibilityAlliance = pPlayer->GetQuestStatus(13686) == QUEST_STATUS_COMPLETE;
            bool EligibilityHorde = pPlayer->GetQuestStatus(13687) == QUEST_STATUS_COMPLETE;

            for (QuestRelations::const_iterator i = pObjectQR->lower_bound(pObject->GetEntry()); i != pObjectQR->upper_bound(pObject->GetEntry()); ++i)
            {
                uint32 quest_id = i->second;
                Quest const* pQuest = sObjectMgr.GetQuestTemplate(quest_id);
                if (!pQuest) continue;

                switch(quest_id)
                {
                case 13707: // Valiant Of Orgrimmar
                case 13708: // Valiant Of Sen'jin
                case 13709: // Valiant Of Thunder Bluff
                case 13710: // Valiant Of Undercity
                case 13711: // Valiant Of Silvermoon
                    if(!EligibilityHorde)
                    {
                        QuestStatus status = pPlayer->GetQuestStatus(quest_id);

                        if(pQuest->IsAutoComplete() && pPlayer->CanTakeQuest(pQuest, false))
                            qm.AddMenuItem(quest_id, 4);
                        else if(status == QUEST_STATUS_NONE && pPlayer->CanTakeQuest(pQuest, false))
                            qm.AddMenuItem(quest_id, 2);
                    }
                    break;
                case 13593: // Valiant Of Stormwind
                case 13703: // Valiant Of Ironforge
                case 13706: // Valiant Of Darnassus
                case 13704: // Valiant Of Gnomeregan
                case 13705: // Valiant Of The Exodar
                    if(!EligibilityAlliance)
                    {
                        QuestStatus status = pPlayer->GetQuestStatus(quest_id);

                        if(pQuest->IsAutoComplete() && pPlayer->CanTakeQuest(pQuest, false))
                            qm.AddMenuItem(quest_id, 4);
                        else if(status == QUEST_STATUS_NONE && pPlayer->CanTakeQuest(pQuest, false))
                            qm.AddMenuItem(quest_id, 2);
                    }
                    break;
                default:
                    QuestStatus status = pPlayer->GetQuestStatus(quest_id);

                    if (pQuest->IsAutoComplete() && pPlayer->CanTakeQuest(pQuest, false))
                        qm.AddMenuItem(quest_id, 4);
                    else if (status == QUEST_STATUS_NONE && pPlayer->CanTakeQuest(pQuest, false))
                        qm.AddMenuItem(quest_id, 2);
                    break;
                }
            }
        }
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

};

/*######
* npc_training_dummy_argent
######*/
#define SPELL_DEFEND_AURA 62719
#define SPELL_DEFEND_AURA_1 64100
#define SPELL_ARGENT_CHARGE 68321
#define SPELL_ARGENT_BREAK_SHIELD 62626
#define SPELL_ARGENT_MELEE 62544

class npc_training_dummy_argent : public CreatureScript
{
public:
    npc_training_dummy_argent() : CreatureScript("npc_training_dummy_argent"){}

    struct npc_training_dummy_argentAI : Scripted_NoMovementAI
    {
        npc_training_dummy_argentAI(Creature *c) : Scripted_NoMovementAI(c)
        {
            m_Entry = c->GetEntry();
        }

        uint64 m_Entry;
        uint32 ResetTimer;
        uint32 DespawnTimer;
        uint32 ShielTimer;
        void Reset()
        {
            me->SetControlled(true,UNIT_STAT_STUNNED);//disable rotate
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);//imune to knock aways like blast wave
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            ResetTimer = 10000;
            DespawnTimer = 15000;
            ShielTimer=0;
        }

        void EnterEvadeMode()
        {
            if (!_EnterEvadeMode())
                return;

            Reset();
        }

        void DamageTaken(Unit * /*done_by*/, uint32 &damage)
        {
            ResetTimer = 10000;
            damage = 0;
        }

        void EnterCombat(Unit * /*who*/)
        {
            if (m_Entry != 2674 && m_Entry != 2673)
                return;
        }

        void SpellHit(Unit* caster,const SpellEntry* spell)
        {
            if(caster->GetOwner())
            {
                if(m_Entry==33272)
                    if(spell->Id==SPELL_ARGENT_CHARGE)
                        if(!me->GetAura(SPELL_DEFEND_AURA))
                            caster->GetOwner()->ToPlayer()->KilledMonsterCredit(33340, 0);
                if(m_Entry==33229){
                    if(spell->Id==SPELL_ARGENT_MELEE)
                    {
                        caster->GetOwner()->ToPlayer()->KilledMonsterCredit(33341, 0);
                        me->CastSpell(caster,62709,true);
                    }
                }

            }

            if(m_Entry==33243)
                if(spell->Id==SPELL_ARGENT_BREAK_SHIELD)
                    if(!me->GetAura(SPELL_DEFEND_AURA))
                        if(caster->GetTypeId()==TYPEID_PLAYER)
                            caster->ToPlayer()->KilledMonsterCredit(33339, 0);
        }


        void UpdateAI(const uint32 diff)
        {
            if (ShielTimer <= diff)
            {
                if(m_Entry==33243)
                    me->CastSpell(me,SPELL_DEFEND_AURA,true);

                if(m_Entry==33272 && !me->GetAura(SPELL_DEFEND_AURA_1))
                    me->CastSpell(me,SPELL_DEFEND_AURA_1,true);
                ShielTimer = 8000;
            }
            else
                ShielTimer -= diff;

            if (!UpdateVictim())
                return;
            if (!me->hasUnitState(UNIT_STAT_STUNNED))
                me->SetControlled(true,UNIT_STAT_STUNNED);//disable rotate

            if (m_Entry != 2674 && m_Entry != 2673)
            {
                if (ResetTimer <= diff)
                {
                    EnterEvadeMode();
                    ResetTimer = 10000;
                }
                else
                    ResetTimer -= diff;
                return;
            }
            else
            {
                if (DespawnTimer <= diff)
                    me->ForcedDespawn();
                else
                    DespawnTimer -= diff;
            }
        }
        void MoveInLineOfSight(Unit * /*who*/){return;}
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_training_dummy_argentAI(pCreature);
    }

};

/*######
* npc_quest_givers_for_crusaders
######*/

class npc_quest_givers_for_crusaders : public CreatureScript
{
public:
    npc_quest_givers_for_crusaders() : CreatureScript("npc_quest_givers_for_crusaders"){}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->HasTitle(TITLE_CRUSADER))
            if (pCreature->isQuestGiver())
                pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        return true;
    }

};


void AddSC_icecrown()
{
    new npc_vendor_argent_tournament;
    new quest_givers_argent_tournament;
    new npc_training_dummy_argent;
    new npc_quest_givers_for_crusaders;
}
