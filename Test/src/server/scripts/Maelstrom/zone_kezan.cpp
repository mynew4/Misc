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

#include "ScriptedEscortAI.h"

#include "SpellMgr.h"

#include "Player.h"

#include "Creature.h"




enum NPC_DefiantTroll

{

    DEFFIANT_KILL_CREDIT              = 34830,

    SPELL_LIGHTNING_VISUAL            = 45870,

    QUEST_GOOD_HELP_IS_HARD_TO_FIND   = 14069,

    GO_DEPOSIT                        = 195488,

};
 


#define SAY_WORK_1 "Oups, fin de la pause."

#define SAY_WORK_2 "Pas le taser, mec !"

#define SAY_WORK_3 "La DRH, elle va entendre parler d'vous !"

#define SAY_WORK_4 "Le travail c'était mieux à Terremine !"

#define SAY_WORK_5 "J'y vais, j'y vais !"

#define SAY_WORK_6 "Désolé, mec. Ça se r'produira plus."

#define SAY_WORK_7 "Qu'est-c'qui va pas ? Pourquoi ma pause déjeuner et mes deux pauses goûter par jour, j'y ai pas droit, mec ?"

#define SAY_WORK_8 "Ouille ! Ça fait mal !"
 



/*####
# npc_defiant_troll
####*/


class npc_defiant_troll : public CreatureScript

{
public:

    npc_defiant_troll() : CreatureScript("npc_defiant_troll") { }

 
   CreatureAI* GetAI(Creature* creature) const

    {

        return new npc_defiant_trollAI(creature);

    }

 
   struct npc_defiant_trollAI : public ScriptedAI
    {

        npc_defiant_trollAI(Creature* creature) : ScriptedAI(creature) {}

 
       uint32 rebuffTimer;

        uint32 auraTimer;

        bool work;

 
       void Reset ()

        {

            rebuffTimer = 0;

            work = false;

            auraTimer = 0;

        }

 
       //void MovementInform(uint32 /*type*/, uint32 id)

        //{

        //    if (id == 1)

        //        work = true;

        //}

 
       void SpellHit(Unit* caster, SpellInfo const* spell)

        {                   
            // Remove Aura from Player

            caster->RemoveAurasDueToSpell(SPELL_LIGHTNING_VISUAL);

 
           if (spell->Id == SPELL_LIGHTNING_VISUAL && caster->GetTypeId() == TYPEID_PLAYER
 && caster->ToPlayer()->GetQuestStatus(QUEST_GOOD_HELP_IS_HARD_TO_FIND) == QUEST_STATUS_INCOMPLETE && work == false)

            {

                caster->ToPlayer()->KilledMonsterCredit(DEFFIANT_KILL_CREDIT, me->GetGUID());

 
               switch (urand(0, 7))

                {

                    case 0:

                        me->MonsterYell(SAY_WORK_1, LANG_UNIVERSAL, 0);

                        break;

                    case 1:

                        me->MonsterYell(SAY_WORK_2, LANG_UNIVERSAL, 0);

                        break;

                    case 2:

                        me->MonsterYell(SAY_WORK_3, LANG_UNIVERSAL, 0);

                        break;

                    case 3:

                        me->MonsterYell(SAY_WORK_4, LANG_UNIVERSAL, 0);

                        break;

                    case 4:

                        me->MonsterYell(SAY_WORK_5, LANG_UNIVERSAL, 0);

                        break;

                    case 5:

                        me->MonsterYell(SAY_WORK_6, LANG_UNIVERSAL, 0);

                        break;

                    case 6:

                        me->MonsterYell(SAY_WORK_7, LANG_UNIVERSAL, 0);

                        break;

                    case 7:

                        me->MonsterYell(SAY_WORK_8, LANG_UNIVERSAL, 0);

                        break;

                }

                me->RemoveAllAuras();
                // Add Aura to Troll

                me->AddAura(SPELL_LIGHTNING_VISUAL, me);
                // set work here so you can't gossip npc while they are walking to ore

                work = true;

                if (GameObject* Deposit = me->FindNearestGameObject(GO_DEPOSIT, 20))

                    me->GetMotionMaster()->MovePoint(1, Deposit->GetPositionX()-1, Deposit->GetPositionY(), Deposit->GetPositionZ());
                // Set timer here so he despawns in 2 minutes, set 2 sec aura timer

                rebuffTimer = 120000;

                auraTimer = rebuffTimer - 2000;

 
                //work = true;

            }

        }

 
       void UpdateAI(const uint32 diff)

        {
       
     if (work == true)

            {

                me->HandleEmoteCommand(467);

                if (rebuffTimer <= auraTimer)

                    me->RemoveAurasDueToSpell(SPELL_LIGHTNING_VISUAL);

            }

            if (rebuffTimer <= diff)

            {
                // If working and timer hits 2 minutes, despawn

                if (work == true)

                {

                    me->DespawnOrUnsummon();

                }

                switch (urand(0, 2))

                {

                    case 0:

                        me->HandleEmoteCommand(412);

                        break;

                    case 1:

                        me->HandleEmoteCommand(10);

                        break;

                    case 2:

                        me->HandleEmoteCommand(0);

                        break;

                }

                rebuffTimer = 120000;                //Rebuff again in 2 minutes

            }

            else

                rebuffTimer -= diff;

 
            if (!UpdateVictim())

                return;

 
            DoMeleeAttackIfReady();

        }

    };

 
   bool OnGossipHello(Player* player, Creature* creature)

    {

        if (player->GetQuestStatus(QUEST_GOOD_HELP_IS_HARD_TO_FIND) == QUEST_STATUS_INCOMPLETE) // && work == false)

        {

            player->CastSpell(creature, SPELL_LIGHTNING_VISUAL, true);

            SpellEntry const* spell = sSpellStore.LookupEntry(SPELL_LIGHTNING_VISUAL);

            CAST_AI(npc_defiant_troll::npc_defiant_trollAI, creature->AI())/*->SpellHit(player, spell)*/;

            return true;

        }

        return false;

    }

};



/*####
# npc_fourth_and_goal_target
####*/


class npc_fourth_and_goal_target : public CreatureScript

{

public:

    npc_fourth_and_goal_target() : CreatureScript("npc_fourth_and_goal_target") { }


    struct npc_fourth_and_goal_targetAI : public ScriptedAI

    {

        npc_fourth_and_goal_targetAI(Creature* creature) : ScriptedAI(creature) { }


        void Reset() {}


        void UpdateAI(const uint32 diff)

        {

            Unit* target = NULL;

            target = me->SelectNearestTarget(5.0f);

            if (target && target->GetTypeId() == TYPEID_PLAYER)

                if (target->ToPlayer()->GetQuestStatus(28414) == QUEST_STATUS_INCOMPLETE)

                    target->ToPlayer()->KilledMonsterCredit(37203, 0);

        }

    };


    CreatureAI* GetAI(Creature* creature) const

    {

        return new npc_fourth_and_goal_targetAI(creature);

    }

};



/*####
# spell_kabummbomb
####*/

class spell_kabummbomb : public SpellScriptLoader

{

public:

    spell_kabummbomb() : SpellScriptLoader("spell_kabummbomb") { }


    class spell_kabummbombSpellScript : public SpellScript

    {

        PrepareSpellScript(spell_kabummbombSpellScript);


        bool Validate(SpellInfo const * spellEntry)

        {

            return true;

        }


        bool Load()

        {

            return true;

        }


        void HandleActivateGameobject(SpellEffIndex effIndex)

        {

            if(Unit* caster = GetCaster())

            {

                caster->MonsterSay("test",0,0);

            }

            if(Unit* caster = GetCastItem()->GetOwner())

            {

                caster->MonsterSay("test",0,0);

            }

            /*if(GameObject* gob = GetTargetGObj())

            {

                gob->Delete();

            }*/

        }


        void Register()

        {

            OnEffectHit += SpellEffectFn(spell_kabummbombSpellScript::HandleActivateGameobject,EFFECT_0,SPELL_EFFECT_ACTIVATE_OBJECT);

        }

    };


    SpellScript *GetSpellScript() const

    {

        return new spell_kabummbombSpellScript();

    }

};

void AddSC_kezan()

{

    new npc_defiant_troll;
    new npc_fourth_and_goal_target;
    new spell_kabummbomb();
}