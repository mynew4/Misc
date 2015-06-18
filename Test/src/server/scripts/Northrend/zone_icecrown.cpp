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

/* ScriptData
SDName: Icecrown
SD%Complete: 100
SDComment: Quest support: 12807
SDCategory: Icecrown
EndScriptData */

/* ContentData
npc_arete
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellAuras.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "CombatAI.h"
#include "Vehicle.h"
#include "UnitAI.h"

/*######
## npc_arete
######*/

#define GOSSIP_ARETE_ITEM1 "Lord-Commander, I would hear your tale."
#define GOSSIP_ARETE_ITEM2 "<You nod slightly but do not complete the motion as the Lord-Commander narrows his eyes before he continues.>"
#define GOSSIP_ARETE_ITEM3 "I thought that they now called themselves the Scarlet Onslaught?"
#define GOSSIP_ARETE_ITEM4 "Where did the grand admiral go?"
#define GOSSIP_ARETE_ITEM5 "That's fine. When do I start?"
#define GOSSIP_ARETE_ITEM6 "Let's finish this!"
#define GOSSIP_ARETE_ITEM7 "That's quite a tale, Lord-Commander."

enum eArete
{
    GOSSIP_TEXTID_ARETE1        = 13525,
    GOSSIP_TEXTID_ARETE2        = 13526,
    GOSSIP_TEXTID_ARETE3        = 13527,
    GOSSIP_TEXTID_ARETE4        = 13528,
    GOSSIP_TEXTID_ARETE5        = 13529,
    GOSSIP_TEXTID_ARETE6        = 13530,
    GOSSIP_TEXTID_ARETE7        = 13531,

    QUEST_THE_STORY_THUS_FAR    = 12807
};

class npc_arete : public CreatureScript
{
public:
    npc_arete() : CreatureScript("npc_arete") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_THE_STORY_THUS_FAR) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE1, creature->GetGUID());
            return true;
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE2, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE3, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE4, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE5, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE6, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ARETE_ITEM7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ARETE7, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                player->CLOSE_GOSSIP_MENU();
                player->AreaExploredOrEventHappens(QUEST_THE_STORY_THUS_FAR);
                break;
        }

        return true;
    }
};

/*######
## npc_guardian_pavilion
######*/

enum eGuardianPavilion
{
    SPELL_TRESPASSER_H                            = 63987,
    AREA_SUNREAVER_PAVILION                       = 4676,

    AREA_SILVER_COVENANT_PAVILION                 = 4677,
    SPELL_TRESPASSER_A                            = 63986,
};

class npc_guardian_pavilion : public CreatureScript
{
public:
    npc_guardian_pavilion() : CreatureScript("npc_guardian_pavilion") { }

    struct npc_guardian_pavilionAI : public ScriptedAI
    {
        npc_guardian_pavilionAI(Creature* creature) : ScriptedAI(creature)
        {
            SetCombatMovement(false);
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (me->GetAreaId() != AREA_SUNREAVER_PAVILION && me->GetAreaId() != AREA_SILVER_COVENANT_PAVILION)
                return;

            if (!who || who->GetTypeId() != TYPEID_PLAYER || !me->IsHostileTo(who) || !me->isInBackInMap(who, 5.0f))
                return;

            if (who->HasAura(SPELL_TRESPASSER_H) || who->HasAura(SPELL_TRESPASSER_A))
                return;

            if (who->ToPlayer()->GetTeamId() == TEAM_ALLIANCE)
                who->CastSpell(who, SPELL_TRESPASSER_H, true);
            else
                who->CastSpell(who, SPELL_TRESPASSER_A, true);

        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_guardian_pavilionAI(creature);
    }
};

/*######
## npc_vereth_the_cunning
######*/

enum eVerethTheCunning
{
    NPC_GEIST_RETURN_BUNNY_KC   = 31049,
    NPC_LITHE_STALKER           = 30894,
    SPELL_SUBDUED_LITHE_STALKER = 58151,
};

class npc_vereth_the_cunning : public CreatureScript
{
public:
    npc_vereth_the_cunning() : CreatureScript("npc_vereth_the_cunning") { }

    struct npc_vereth_the_cunningAI : public ScriptedAI
    {
        npc_vereth_the_cunningAI(Creature* creature) : ScriptedAI(creature) {}

        void MoveInLineOfSight(Unit* who)
        {
            ScriptedAI::MoveInLineOfSight(who);

            if (who->GetEntry() == NPC_LITHE_STALKER && me->IsWithinDistInMap(who, 10.0f))
            {
                if (Unit* owner = who->GetCharmer())
                {
                    if (who->HasAura(SPELL_SUBDUED_LITHE_STALKER))
                        {
                            owner->ToPlayer()->KilledMonsterCredit(NPC_GEIST_RETURN_BUNNY_KC, 0);
                            who->ToCreature()->DisappearAndDie();

                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_vereth_the_cunningAI(creature);
    }
};

/*######
* npc_tournament_training_dummy
######*/
enum TournamentDummy
{
    NPC_CHARGE_TARGET         = 33272,
    NPC_MELEE_TARGET          = 33229,
    NPC_RANGED_TARGET         = 33243,

    SPELL_CHARGE_CREDIT       = 62658,
    SPELL_MELEE_CREDIT        = 62672,
    SPELL_RANGED_CREDIT       = 62673,

    SPELL_PLAYER_THRUST       = 62544,
    SPELL_PLAYER_BREAK_SHIELD = 62626,
    SPELL_PLAYER_CHARGE       = 62874,

    SPELL_RANGED_DEFEND       = 62719,
    SPELL_CHARGE_DEFEND       = 64100,
    SPELL_VULNERABLE          = 62665,

    SPELL_COUNTERATTACK       = 62709,

    EVENT_DUMMY_RECAST_DEFEND = 1,
    EVENT_DUMMY_RESET         = 2,
};

class npc_tournament_training_dummy : public CreatureScript
{
    public:
        npc_tournament_training_dummy(): CreatureScript("npc_tournament_training_dummy"){}

        struct npc_tournament_training_dummyAI : ScriptedAI
        {
            npc_tournament_training_dummyAI(Creature* creature) : ScriptedAI(creature)
            {
                SetCombatMovement(false);
            }

            EventMap events;
            bool isVulnerable;

            void Reset()
            {
                me->SetControlled(true, UNIT_STATE_STUNNED);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                isVulnerable = false;

                // Cast Defend spells to max stack size
                switch (me->GetEntry())
                {
                    case NPC_CHARGE_TARGET:
                        DoCast(SPELL_CHARGE_DEFEND);
                        break;
                    case NPC_RANGED_TARGET:
                        me->CastCustomSpell(SPELL_RANGED_DEFEND, SPELLVALUE_AURA_STACK, 3, me);
                        break;
                }

                events.Reset();
                events.ScheduleEvent(EVENT_DUMMY_RECAST_DEFEND, 5000);
            }

            void EnterEvadeMode()
            {
                if (!_EnterEvadeMode())
                    return;

                Reset();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                damage = 0;
                events.RescheduleEvent(EVENT_DUMMY_RESET, 10000);
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                switch (me->GetEntry())
                {
                    case NPC_CHARGE_TARGET:
                        if (spell->Id == SPELL_PLAYER_CHARGE)
                            if (isVulnerable)
                                DoCast(caster, SPELL_CHARGE_CREDIT, true);
                        break;
                    case NPC_MELEE_TARGET:
                        if (spell->Id == SPELL_PLAYER_THRUST)
                        {
                            DoCast(caster, SPELL_MELEE_CREDIT, true);

                            if (Unit* target = caster->GetVehicleBase())
                                DoCast(target, SPELL_COUNTERATTACK, true);
                        }
                        break;
                    case NPC_RANGED_TARGET:
                        if (spell->Id == SPELL_PLAYER_BREAK_SHIELD)
                            if (isVulnerable)
                                DoCast(caster, SPELL_RANGED_CREDIT, true);
                        break;
                }

                if (spell->Id == SPELL_PLAYER_BREAK_SHIELD)
                    if (!me->HasAura(SPELL_CHARGE_DEFEND) && !me->HasAura(SPELL_RANGED_DEFEND))
                        isVulnerable = true;
            }

            void UpdateAI(uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_DUMMY_RECAST_DEFEND:
                        switch (me->GetEntry())
                        {
                            case NPC_CHARGE_TARGET:
                            {
                                if (!me->HasAura(SPELL_CHARGE_DEFEND))
                                    DoCast(SPELL_CHARGE_DEFEND);
                                break;
                            }
                            case NPC_RANGED_TARGET:
                            {
                                Aura* defend = me->GetAura(SPELL_RANGED_DEFEND);
                                if (!defend || defend->GetStackAmount() < 3 || defend->GetDuration() <= 8000)
                                    DoCast(SPELL_RANGED_DEFEND);
                                break;
                            }
                        }
                        isVulnerable = false;
                        events.ScheduleEvent(EVENT_DUMMY_RECAST_DEFEND, 5000);
                        break;
                    case EVENT_DUMMY_RESET:
                        if (UpdateVictim())
                        {
                            EnterEvadeMode();
                            events.ScheduleEvent(EVENT_DUMMY_RESET, 10000);
                        }
                        break;
                }

                if (!UpdateVictim())
                    return;

                if (!me->HasUnitState(UNIT_STATE_STUNNED))
                    me->SetControlled(true, UNIT_STATE_STUNNED);
            }

            void MoveInLineOfSight(Unit* /*who*/){}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tournament_training_dummyAI(creature);
        }

};

// Battle for Crusaders' Pinnacle
enum BlessedBanner
{
    SPELL_BLESSING_OF_THE_CRUSADE       = 58026,
    SPELL_THREAT_PULSE                  = 58113,
    SPELL_CRUSADERS_SPIRE_VICTORY       = 58084,
    SPELL_TORCH                         = 58121,

    NPC_BLESSED_BANNER                  = 30891,
    NPC_CRUSADER_LORD_DALFORS           = 31003,
    NPC_ARGENT_BATTLE_PRIEST            = 30919,
    NPC_ARGENT_MASON                    = 30900,
    NPC_REANIMATED_CAPTAIN              = 30986,
    NPC_SCOURGE_DRUDGE                  = 30984,
    NPC_HIDEOUS_PLAGEBRINGER            = 30987,
    NPC_HALOF_THE_DEATHBRINGER          = 30989,
    NPC_LK                              = 31013,

    BANNER_SAY                          = 0, // "The Blessed Banner of the Crusade has been planted.\n Defend the banner from all attackers!"
    DALFORS_SAY_PRE_1                   = 0, // "BY THE LIGHT! Those damned monsters! Look at what they've done to our people!"
    DALFORS_SAY_PRE_2                   = 1, // "Burn it down, boys. Burn it all down."
    DALFORS_SAY_START                   = 2, // "Let 'em come. They'll pay for what they've done!"
    DALFORS_YELL_FINISHED               = 3, // "We've done it, lads! We've taken the pinnacle from the Scourge! Report to Father Gustav at once and tell him the good news! We're gonna get to buildin' and settin' up! Go!"
    LK_TALK_1                           = 0, // "Leave no survivors!"
    LK_TALK_2                           = 1, // "Cower before my terrible creations!"
    LK_TALK_3                           = 2, // "Feast my children! Feast upon the flesh of the living!"
    LK_TALK_4                           = 3, // "Lay down your arms and surrender your souls!"

    EVENT_SPAWN                         = 1,
    EVENT_INTRO_1                       = 2,
    EVENT_INTRO_2                       = 3,
    EVENT_INTRO_3                       = 4,
    EVENT_MASON_ACTION                  = 5,
    EVENT_START_FIGHT                   = 6,
    EVENT_WAVE_SPAWN                    = 7,
    EVENT_HALOF                         = 8,
    EVENT_ENDED                         = 9,
};

Position const DalforsPos[3] =
{
    {6458.703f, 403.858f, 490.498f, 3.1205f}, // Dalfors spawn point
    {6422.950f, 423.335f, 510.451f, 0.0f}, // Dalfors intro pos
    {6426.343f, 420.515f, 508.650f, 0.0f}, // Dalfors fight pos
};

Position const Priest1Pos[2] =
{
    {6462.025f, 403.681f, 489.721f, 3.1007f}, // priest1 spawn point
    {6421.480f, 423.576f, 510.781f, 5.7421f}, // priest1 intro pos
};

Position const Priest2Pos[2] =
{
    {6463.969f, 407.198f, 489.240f, 2.2689f}, // priest2 spawn point
    {6419.778f, 421.404f, 510.972f, 5.7421f}, // priest2 intro pos
};

Position const Priest3Pos[2] =
{
    {6464.371f, 400.944f, 489.186f, 6.1610f}, // priest3 spawn point
    {6423.516f, 425.782f, 510.774f, 5.7421f}, // priest3 intro pos
};

Position const Mason1Pos[3] =
{
    {6462.929f, 409.826f, 489.392f, 3.0968f}, // mason1 spawn point
    {6428.163f, 421.960f, 508.297f, 0.0f}, // mason1 intro pos
    {6414.335f, 454.904f, 511.395f, 2.8972f}, // mason1 action pos
};

Position const Mason2Pos[3] =
{
    {6462.650f, 405.670f, 489.576f, 2.9414f}, // mason2 spawn point
    {6426.250f, 419.194f, 508.219f, 0.0f}, // mason2 intro pos
    {6415.014f, 446.849f, 511.395f, 3.1241f}, // mason2 action pos
};

Position const Mason3Pos[3] =
{
    {6462.646f, 401.218f, 489.601f, 2.7864f}, // mason3 spawn point
    {6423.855f, 416.598f, 508.305f, 0.0f}, // mason3 intro pos
    {6417.070f, 438.824f, 511.395f, 3.6651f}, // mason3 action pos
};

class npc_blessed_banner : public CreatureScript
{
public:
    npc_blessed_banner() : CreatureScript("npc_blessed_banner") { }

    struct npc_blessed_bannerAI : public ScriptedAI
    {
        npc_blessed_bannerAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            HalofSpawned = false;
            PhaseCount = 0;
            Summons.DespawnAll();

            SetCombatMovement(false);
        }

        EventMap events;

        bool HalofSpawned;

        uint32 PhaseCount;

        SummonList Summons;

        uint64 guidDalfors;
        uint64 guidPriest[3];
        uint64 guidMason[3];
        uint64 guidHalof;

        void Reset()
        {
            me->setRegeneratingHealth(false);
            DoCast(SPELL_THREAT_PULSE);
            me->AI()->Talk(BANNER_SAY);
            events.ScheduleEvent(EVENT_SPAWN, 3000);
        }

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit* /*who*/) {}

        void JustSummoned(Creature* Summoned)
        {
            Summons.Summon(Summoned);
        }

        void JustDied(Unit* /*killer*/)
        {
            Summons.DespawnAll();
            me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 diff)
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_SPAWN:
                    {
                        if (Creature* Dalfors = DoSummon(NPC_CRUSADER_LORD_DALFORS, DalforsPos[0]))
                        {
                            guidDalfors = Dalfors->GetGUID();
                            Dalfors->GetMotionMaster()->MovePoint(0, DalforsPos[1]);
                        }
                        if (Creature* Priest1 = DoSummon(NPC_ARGENT_BATTLE_PRIEST, Priest1Pos[0]))
                        {
                            guidPriest[0] = Priest1->GetGUID();
                            Priest1->GetMotionMaster()->MovePoint(0, Priest1Pos[1]);
                        }
                        if (Creature* Priest2 = DoSummon(NPC_ARGENT_BATTLE_PRIEST, Priest2Pos[0]))
                        {
                            guidPriest[1] = Priest2->GetGUID();
                            Priest2->GetMotionMaster()->MovePoint(0, Priest2Pos[1]);
                        }
                        if (Creature* Priest3 = DoSummon(NPC_ARGENT_BATTLE_PRIEST, Priest3Pos[0]))
                        {
                            guidPriest[2] = Priest3->GetGUID();
                            Priest3->GetMotionMaster()->MovePoint(0, Priest3Pos[1]);
                        }
                        if (Creature* Mason1 = DoSummon(NPC_ARGENT_MASON, Mason1Pos[0]))
                        {
                            guidMason[0] = Mason1->GetGUID();
                            Mason1->GetMotionMaster()->MovePoint(0, Mason1Pos[1]);
                        }
                        if (Creature* Mason2 = DoSummon(NPC_ARGENT_MASON, Mason2Pos[0]))
                        {
                            guidMason[1] = Mason2->GetGUID();
                            Mason2->GetMotionMaster()->MovePoint(0, Mason2Pos[1]);
                        }
                        if (Creature* Mason3 = DoSummon(NPC_ARGENT_MASON, Mason3Pos[0]))
                        {
                            guidMason[2] = Mason3->GetGUID();
                            Mason3->GetMotionMaster()->MovePoint(0, Mason3Pos[1]);
                        }
                        events.ScheduleEvent(EVENT_INTRO_1, 15000);
                    }
                    break;
                case EVENT_INTRO_1:
                    {
                        if (Creature* Dalfors = me->GetCreature(*me, guidDalfors))
                            Dalfors->AI()->Talk(DALFORS_SAY_PRE_1);
                        events.ScheduleEvent(EVENT_INTRO_2, 5000);
                    }
                    break;
                case EVENT_INTRO_2:
                    {
                        if (Creature* Dalfors = me->GetCreature(*me, guidDalfors))
                        {
                            Dalfors->SetFacingTo(6.215f);
                            Dalfors->AI()->Talk(DALFORS_SAY_PRE_2);
                        }
                    events.ScheduleEvent(EVENT_INTRO_3, 5000);
                    }
                    break;
                case EVENT_INTRO_3:
                    {
                        if (Creature* Dalfors = me->GetCreature(*me, guidDalfors))
                        {
                            Dalfors->GetMotionMaster()->MovePoint(0, DalforsPos[2]);
                            Dalfors->SetHomePosition(DalforsPos[2]);
                        }
                        if (Creature* Priest1 = me->GetCreature(*me, guidPriest[0]))
                        {
                            Priest1->SetFacingTo(5.7421f);
                            Priest1->SetHomePosition(Priest1Pos[1]);
                        }
                        if (Creature* Priest2 = me->GetCreature(*me, guidPriest[1]))
                        {
                            Priest2->SetFacingTo(5.7421f);
                            Priest2->SetHomePosition(Priest2Pos[1]);
                        }
                        if (Creature* Priest3 = me->GetCreature(*me, guidPriest[2]))
                        {
                            Priest3->SetFacingTo(5.7421f);
                            Priest3->SetHomePosition(Priest3Pos[1]);
                        }
                        if (Creature* Mason1 = me->GetCreature(*me, guidMason[0]))
                        {
                            Mason1->GetMotionMaster()->MovePoint(0, Mason1Pos[2]);
                            Mason1->SetHomePosition(Mason1Pos[2]);
                        }
                        if (Creature* Mason2 = me->GetCreature(*me, guidMason[1]))
                        {
                            Mason2->GetMotionMaster()->MovePoint(0, Mason2Pos[2]);
                            Mason2->SetHomePosition(Mason2Pos[2]);
                        }
                        if (Creature* Mason3 = me->GetCreature(*me, guidMason[2]))
                        {
                            Mason3->GetMotionMaster()->MovePoint(0, Mason3Pos[2]);
                            Mason3->SetHomePosition(Mason3Pos[2]);
                        }
                        events.ScheduleEvent(EVENT_START_FIGHT, 5000);
                        events.ScheduleEvent(EVENT_MASON_ACTION, 15000);
                    }
                    break;
                case EVENT_MASON_ACTION:
                    {
                        if (Creature* Mason1 = me->GetCreature(*me, guidMason[0]))
                        {
                            Mason1->SetFacingTo(2.8972f);
                            Mason1->AI()->SetData(1, 1); // triggers SAI actions on npc
                        }
                        if (Creature* Mason2 = me->GetCreature(*me, guidMason[1]))
                        {
                            Mason2->SetFacingTo(3.1241f);
                            Mason2->AI()->SetData(1, 1); // triggers SAI actions on npc
                        }
                        if (Creature* Mason3 = me->GetCreature(*me, guidMason[2]))
                        {
                            Mason3->SetFacingTo(3.6651f);
                            Mason3->AI()->SetData(1, 1); // triggers SAI actions on npc
                        }
                    }
                    break;
                case EVENT_START_FIGHT:
                    {
                        if (Creature* LK = GetClosestCreatureWithEntry(me, NPC_LK, 100))
                            LK->AI()->Talk(LK_TALK_1);
                        if (Creature* Dalfors = me->GetCreature(*me, guidDalfors))
                            Dalfors->AI()->Talk(DALFORS_SAY_START);
                        events.ScheduleEvent(EVENT_WAVE_SPAWN, 1000);
                    }
                    break;
                case EVENT_WAVE_SPAWN:
                    {
                        if (PhaseCount == 3)
                        {
                            if (Creature* LK = GetClosestCreatureWithEntry(me, NPC_LK, 100))
                                LK->AI()->Talk(LK_TALK_2);
                        }
                        else if (PhaseCount == 6)
                        {
                            if (Creature* LK = GetClosestCreatureWithEntry(me, NPC_LK, 100))
                                LK->AI()->Talk(LK_TALK_3);
                        }
                        if (Creature* tempsum = DoSummon(NPC_SCOURGE_DRUDGE, Mason3Pos[0]))
                            {
                                tempsum->SetHomePosition(DalforsPos[2]);
                                tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                            }
                        if (urand(0, 1) == 0)
                        {
                            if (Creature* tempsum = DoSummon(NPC_HIDEOUS_PLAGEBRINGER, Mason1Pos[0]))
                            {
                                tempsum->SetHomePosition(DalforsPos[2]);
                                tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                            }
                            if (Creature* tempsum = DoSummon(NPC_HIDEOUS_PLAGEBRINGER, Mason2Pos[0]))
                            {
                                tempsum->SetHomePosition(DalforsPos[2]);
                                tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                            }
                        }
                        else
                        {
                            if (Creature* tempsum = DoSummon(NPC_REANIMATED_CAPTAIN, Mason1Pos[0]))
                            {
                                tempsum->SetHomePosition(DalforsPos[2]);
                                tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                            }
                            if (Creature* tempsum = DoSummon(NPC_REANIMATED_CAPTAIN, Mason2Pos[0]))
                            {
                                tempsum->SetHomePosition(DalforsPos[2]);
                                tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                            }
                        }

                        PhaseCount++;

                        if (PhaseCount < 8)
                            events.ScheduleEvent(EVENT_WAVE_SPAWN, urand(10000, 20000));
                        else
                            events.ScheduleEvent(EVENT_HALOF, urand(10000, 20000));
                    }
                    break;
                case EVENT_HALOF:
                    {
                        if (Creature* LK = GetClosestCreatureWithEntry(me, NPC_LK, 100))
                            LK->AI()->Talk(LK_TALK_4);
                        if (Creature* tempsum = DoSummon(NPC_SCOURGE_DRUDGE, Mason1Pos[0]))
                        {
                            tempsum->SetHomePosition(DalforsPos[2]);
                            tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                        }
                        if (Creature* tempsum = DoSummon(NPC_SCOURGE_DRUDGE, Mason2Pos[0]))
                        {
                            tempsum->SetHomePosition(DalforsPos[2]);
                            tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                        }
                        if (Creature* tempsum = DoSummon(NPC_HALOF_THE_DEATHBRINGER, DalforsPos[0]))
                        {
                            HalofSpawned = true;
                            guidHalof = tempsum->GetGUID();
                            tempsum->SetHomePosition(DalforsPos[2]);
                            tempsum->AI()->AttackStart(GetClosestCreatureWithEntry(me, NPC_BLESSED_BANNER, 100));
                        }
                    }
                    break;
                case EVENT_ENDED:
                    {
                        Summons.DespawnAll();
                        me->DespawnOrUnsummon();
                    }
                    break;
            }

            if (PhaseCount == 8)
                if (Creature* Halof = me->GetCreature(*me, guidHalof))
                    if (Halof->isDead())
                    {
                        DoCast(me, SPELL_CRUSADERS_SPIRE_VICTORY, true);
                        Summons.DespawnEntry(NPC_HIDEOUS_PLAGEBRINGER);
                        Summons.DespawnEntry(NPC_REANIMATED_CAPTAIN);
                        Summons.DespawnEntry(NPC_SCOURGE_DRUDGE);
                        Summons.DespawnEntry(NPC_HALOF_THE_DEATHBRINGER);
                        if (Creature* Dalfors = me->GetCreature(*me, guidDalfors))
                            Dalfors->AI()->Talk(DALFORS_YELL_FINISHED);
                        events.ScheduleEvent(EVENT_ENDED, 10000);
                    }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blessed_bannerAI(creature);
    }
};

/*######
## npc_dame_evniki_kapsalis
 ######*/

enum eDameEnvikiKapsalis {
    TITLE_CRUSADER = 123
};

class npc_dame_evniki_kapsalis: public CreatureScript {
public:
    npc_dame_evniki_kapsalis() :
            CreatureScript("npc_dame_evniki_kapsalis") {
    }

    bool OnGossipHello(Player* player, Creature* creature) {
        if (player->HasTitle(TITLE_CRUSADER))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/,
            uint32 uiAction) {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());
        return true;
    }
};

/*######
## npc_faction_valiant_champion
 ######*/

enum eFactionValiantChampion {
    SPELL_REFRESH_MOUNT			= 66483,
	SPELL_THRUST				= 62544,
	SPELL_DEFEND				= 62552,
	SPELL_DEFEND_AURA_PERIODIC	= 64553,

    SPELL_GIVE_VALIANT_MARK_1	= 62724,
    SPELL_GIVE_VALIANT_MARK_2	= 62770,
    SPELL_GIVE_VALIANT_MARK_3	= 62771,
    SPELL_GIVE_VALIANT_MARK_4	= 62995,
    SPELL_GIVE_VALIANT_MARK_5	= 62996,

    SPELL_GIVE_CHAMPION_MARK	= 63596,

    QUEST_THE_GRAND_MELEE_0		= 13665,
    QUEST_THE_GRAND_MELEE_1		= 13745,
    QUEST_THE_GRAND_MELEE_2		= 13750,
    QUEST_THE_GRAND_MELEE_3		= 13756,
    QUEST_THE_GRAND_MELEE_4		= 13761,
    QUEST_THE_GRAND_MELEE_5		= 13767,
    QUEST_THE_GRAND_MELEE_6		= 13772,
    QUEST_THE_GRAND_MELEE_7		= 13777,
    QUEST_THE_GRAND_MELEE_8		= 13782,
    QUEST_THE_GRAND_MELEE_9		= 13787,

    QUEST_AMONG_THE_CHAMPIONS_0 = 13790,
    QUEST_AMONG_THE_CHAMPIONS_1 = 13793,
    QUEST_AMONG_THE_CHAMPIONS_2 = 13811,
    QUEST_AMONG_THE_CHAMPIONS_3 = 13814,

    SPELL_BESTED_DARNASSUS		= 64805,
    SPELL_BESTED_GNOMEREGAN		= 64809,
    SPELL_BESTED_IRONFORGE		= 64810,
    SPELL_BESTED_ORGRIMMAR		= 64811,
    SPELL_BESTED_SENJIN			= 64812,
    SPELL_BESTED_SILVERMOON		= 64813,
    SPELL_BESTED_STORMWIND		= 64814,
    SPELL_BESTED_EXODAR			= 64808,
    SPELL_BESTED_UNDERCITY		= 64816,
    SPELL_BESTED_THUNDERBLUFF	= 64815,
	SPELL_CHARGE                = 63010,
    SPELL_SHIELD_BREAKER        = 65147
};

#define GOSSIP_MELEE_FIGHT      "I'am ready to fight!"

class npc_faction_valiant_champion: public CreatureScript {
public:
    npc_faction_valiant_champion() :
            CreatureScript("npc_faction_valiant_champion") {
    }

    struct npc_faction_valiant_championAI: public ScriptedAI {
        npc_faction_valiant_championAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        uint32 uiChargeTimer;
        uint32 uiShieldBreakerTimer;
        uint64 guidAttacker;
        bool chargeing;

        void Reset() {
            uiChargeTimer = 7000;
            uiShieldBreakerTimer = 10000;

            me->setFaction(35);
        }

        void EnterCombat(Unit* attacker) {
            guidAttacker = attacker->GetGUID();
            DoCast(me, SPELL_DEFEND_AURA_PERIODIC, true);
            if (Aura* aur = me->AddAura(SPELL_DEFEND, me))
                aur->ModStackAmount(1);
        }

        void MovementInform(uint32 uiType, uint32 uiId) {
            if (uiType != POINT_MOTION_TYPE)
                return;

            if (uiId != 1)
                return;

            chargeing = false;

            DoCastVictim(SPELL_CHARGE);
            if (me->getVictim())
                me->GetMotionMaster()->MoveChase(me->getVictim());
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage) {
            if (pDoneBy && pDoneBy->GetGUID() != guidAttacker)
                uiDamage = 0;

            if (uiDamage > me->GetHealth()
                    && pDoneBy->GetTypeId() == TYPEID_PLAYER) {
                uiDamage = 0;

                if (pDoneBy->HasAura(63034)) {
                    switch (me->GetEntry()) {
                    case 33559: // Darnassus
                    case 33562: // Exodar
                    case 33558: // Gnomeregan
                    case 33564: // Ironforge
                    case 33306: // Orgrimmar
                    case 33285: // Sen'jin
                    case 33382: // Silvermoon
                    case 33561: // Stormwind
                    case 33383: // Thunder Bluff
                    case 33384: // Undercity
                    {
                        pDoneBy->CastSpell(pDoneBy, SPELL_GIVE_VALIANT_MARK_1,
                                true);
                        break;
                    }
                    case 33738: // Darnassus
                    case 33739: // Exodar
                    case 33740: // Gnomeregan
                    case 33743: // Ironforge
                    case 33744: // Orgrimmar
                    case 33745: // Sen'jin
                    case 33746: // Silvermoon
                    case 33747: // Stormwind
                    case 33748: // Thunder Bluff
                    case 33749: // Undercity
                    {
                        pDoneBy->CastSpell(pDoneBy, SPELL_GIVE_CHAMPION_MARK,
                                true);
                        break;
                    }
                    }

                    switch (me->GetEntry()) {
                    case 33738: // Darnassus
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_DARNASSUS,
                                true);
                        break;
                    case 33739: // Exodar
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_EXODAR, true);
                        break;
                    case 33740: // Gnomeregan
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_GNOMEREGAN,
                                true);
                        break;
                    case 33743: // Ironforge
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_IRONFORGE,
                                true);
                        break;
                    case 33744: // Orgrimmar
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_ORGRIMMAR,
                                true);
                        break;
                    case 33745: // Sen'jin
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_SENJIN, true);
                        break;
                    case 33746: // Silvermoon
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_SILVERMOON,
                                true);
                        break;
                    case 33747: // Stormwind
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_STORMWIND,
                                true);
                        break;
                    case 33748: // Thunder Bluff
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_THUNDERBLUFF,
                                true);
                        break;
                    case 33749: // Undercity
                        pDoneBy->CastSpell(pDoneBy, SPELL_BESTED_UNDERCITY,
                                true);
                        break;
                    }
                }

                me->setFaction(35);
                EnterEvadeMode();
                me->CastSpell(me, SPELL_REFRESH_MOUNT, true);
            }
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!UpdateVictim())
                return;

            if (uiChargeTimer <= uiDiff) {
                chargeing = true;
                float x, y, z;
                me->GetNearPoint(me, x, y, z, 1.0f, 15.0f,
                        float(M_PI * 2 * rand_norm()));
                me->GetMotionMaster()->MovePoint(1, x, y, z);

                uiChargeTimer = 15000;
            } else
                uiChargeTimer -= uiDiff;

            if (uiShieldBreakerTimer <= uiDiff) {
                DoCastVictim(SPELL_SHIELD_BREAKER);
                uiShieldBreakerTimer = 10000;
            } else
                uiShieldBreakerTimer -= uiDiff;

            if (me->isAttackReady()) {
                DoCast(me->getVictim(), SPELL_THRUST, true);
                me->resetAttackTimer();
            }

            if (Player* plr = Player::GetPlayer(*me, guidAttacker))
                if (!plr->HasAura(63034))
                    EnterEvadeMode();
        }
    };

    CreatureAI *GetAI(Creature* creature) const {
        return new npc_faction_valiant_championAI(creature);
    }

    bool CanMakeDuel(Player* player, uint32 npcEntry) {
        switch (npcEntry) {
        case 33738: // Darnassus
            return !player->HasAura(SPELL_BESTED_DARNASSUS);
        case 33739: // Exodar
            return !player->HasAura(SPELL_BESTED_EXODAR);
        case 33740: // Gnomeregan
            return !player->HasAura(SPELL_BESTED_GNOMEREGAN);
        case 33743: // Ironforge
            return !player->HasAura(SPELL_BESTED_IRONFORGE);
        case 33744: // Orgrimmar
            return !player->HasAura(SPELL_BESTED_ORGRIMMAR);
        case 33745: // Sen'jin
            return !player->HasAura(SPELL_BESTED_SENJIN);
        case 33746: // Silvermoon
            return !player->HasAura(SPELL_BESTED_SILVERMOON);
        case 33747: // Stormwind
            return !player->HasAura(SPELL_BESTED_STORMWIND);
        case 33748: // Thunder Bluff
            return !player->HasAura(SPELL_BESTED_THUNDERBLUFF);
        case 33749: // Undercity
            return !player->HasAura(SPELL_BESTED_UNDERCITY);
        }
        return true;
    }

    void AddMeleeFightGossip(Player* player) {
        if (!player)
            return;

        if (player->HasAura(63034)
                && ((player->GetQuestStatus(QUEST_THE_GRAND_MELEE_0)
                        == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_1)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_2)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_3)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_4)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_5)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_6)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_7)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_8)
                                == QUEST_STATUS_INCOMPLETE)
                        || (player->GetQuestStatus(QUEST_THE_GRAND_MELEE_9)
                                == QUEST_STATUS_INCOMPLETE))) {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MELEE_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        }
    }

    bool OnGossipHello(Player* player, Creature* creature) {
        switch (creature->GetEntry()) {
        case 33559: // Darnassus
        case 33562: // Exodar
        case 33558: // Gnomeregan
        case 33564: // Ironforge
        case 33561: // Stormwind
        {
            if (player->GetTeamId() == TEAM_ALLIANCE)
                AddMeleeFightGossip(player);
            break;
        }
        case 33306: // Orgrimmar
        case 33285: // Sen'jin
        case 33382: // Silvermoon
        case 33383: // Thunder Bluff
        case 33384: // Undercity
        {
            if (player->GetTeamId() == TEAM_HORDE)
                AddMeleeFightGossip(player);
            break;
        }
        case 33738: // Darnassus
        case 33739: // Exodar
        case 33740: // Gnomeregan
        case 33743: // Ironforge
        case 33744: // Orgrimmar
        case 33745: // Sen'jin
        case 33746: // Silvermoon
        case 33747: // Stormwind
        case 33748: // Thunder Bluff
        case 33749: // Undercity
        {
            if (player->HasAura(63034)
                    && ((player->GetQuestStatus(QUEST_AMONG_THE_CHAMPIONS_0)
                            == QUEST_STATUS_INCOMPLETE)
                            || (player->GetQuestStatus(
                                    QUEST_AMONG_THE_CHAMPIONS_1)
                                    == QUEST_STATUS_INCOMPLETE)
                            || (player->GetQuestStatus(
                                    QUEST_AMONG_THE_CHAMPIONS_2)
                                    == QUEST_STATUS_INCOMPLETE)
                            || (player->GetQuestStatus(
                                    QUEST_AMONG_THE_CHAMPIONS_3)
                                    == QUEST_STATUS_INCOMPLETE))) {
                if (CanMakeDuel(player, creature->GetEntry()))
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MELEE_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            }
            break;
        }
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/,
            uint32 uiAction) {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();
        if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) {
            creature->setFaction(14);
            creature->AI()->AttackStart(player);
            return true;
        }

        if (uiAction == GOSSIP_ACTION_INFO_DEF + 2) {
            creature->setFaction(14);
            creature->AI()->AttackStart(player);
            return true;
        }
        return true;
    }
};

/*######
## spell_argent_cannon
 ######*/

enum ArgentCannonSpells {
    SPELL_ARGENT_CANNON_SHOOT = 57385,
    SPELL_ARGENT_CANNON_SHOOT_TRIGGER = 57387,
    SPELL_RECONING_BOMB = 57412,
    SPELL_RECONING_BOMB_TRIGGER = 57414
};

class spell_argent_cannon: public SpellScriptLoader {
public:
    spell_argent_cannon() :
            SpellScriptLoader("spell_argent_cannon") {
    }

    class spell_argent_cannon_SpellScript: public SpellScript {
        PrepareSpellScript(spell_argent_cannon_SpellScript)
        ;

        bool Validate(SpellInfo const* /*spellInfo*/) {
            if (!sSpellStore.LookupEntry(SPELL_ARGENT_CANNON_SHOOT_TRIGGER))
                return false;
            if (!sSpellStore.LookupEntry(SPELL_RECONING_BOMB_TRIGGER)) //sSpellStore.LookupEntry = GetSpellInfo
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex effIndex) {
            const WorldLocation* loc /*= GetTargetDest()*/;

            switch (GetSpellInfo()->Id) {
            case SPELL_ARGENT_CANNON_SHOOT:
                GetCaster()->CastSpell(loc->m_positionX, loc->m_positionY,
                        loc->m_positionZ, SPELL_ARGENT_CANNON_SHOOT_TRIGGER,
                        true);
                break;
            case SPELL_RECONING_BOMB:
                GetCaster()->CastSpell(loc->m_positionX, loc->m_positionY,
                        loc->m_positionZ, SPELL_RECONING_BOMB_TRIGGER, true);
                break;
            }

            PreventHitDefaultEffect(effIndex);
        }

        void Register() {
            OnEffectHit += SpellEffectFn(spell_argent_cannon_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnEffectHitTarget += SpellEffectFn(spell_argent_cannon_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const {
        return new spell_argent_cannon_SpellScript();
    }
};

/*######
## npc_captured_crusader
 ######*/

enum CapturedCrusader {
    SPELL_NERUBIAN_WEBS = 56726,
    SPELL_GRAB_CAPTURED_CRUSADER = 56683,
    SPELL_RIDE_VEHICLE = 56687
};

// texts not in script
class npc_captured_crusader: public CreatureScript {
public:
    npc_captured_crusader() :
            CreatureScript("npc_captured_crusader") {
    }

    struct npc_captured_crusaderAI: public ScriptedAI {
        npc_captured_crusaderAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        void Reset() {
            _spellHit = false;
            _vehicleTimer = 5000;
            DoCast(me, SPELL_NERUBIAN_WEBS, true);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) {
            if (spell->Id == SPELL_GRAB_CAPTURED_CRUSADER) {
                _spellHit = true;
                DoCast(caster, SPELL_RIDE_VEHICLE, true);
            }
        }

        void UpdateAI(uint32 const diff) {
            if (!_spellHit || _spellHit && me->GetVehicle())
                return;

            if (_vehicleTimer <= diff) {
                _spellHit = false;
                me->DespawnOrUnsummon();
            } else
                _vehicleTimer -= diff;
        }

    private:
        bool _spellHit;
        uint32 _vehicleTimer;
    };

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_captured_crusaderAI(creature);
    }
};



/*######
## npc_alorah_and_grimmin
 ######*/

enum ealorah_and_grimmin {
    SPELL_CHAIN = 68341,
    NPC_FJOLA_LIGHTBANE = 36065,
    NPC_EYDIS_DARKBANE = 36066,
    NPC_PRIESTESS_ALORAH = 36101,
    NPC_PRIEST_GRIMMIN = 36102
};

class npc_alorah_and_grimmin: public CreatureScript {
public:
    npc_alorah_and_grimmin() :
            CreatureScript("npc_alorah_and_grimmin") {
    }

    struct npc_alorah_and_grimminAI: public ScriptedAI {
        npc_alorah_and_grimminAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        bool uiCast;

        void Reset() {
            uiCast = false;
        }

        void UpdateAI(const uint32 /*uiDiff*/) {
            if (uiCast)
                return;
            uiCast = true;
            Creature* target = NULL;

            switch (me->GetEntry()) {
            case NPC_PRIESTESS_ALORAH:
                target = me->FindNearestCreature(NPC_EYDIS_DARKBANE, 10.0f);
                break;
            case NPC_PRIEST_GRIMMIN:
                target = me->FindNearestCreature(NPC_FJOLA_LIGHTBANE, 10.0f);
                break;
            }
            if (target)
                DoCast(target, SPELL_CHAIN);

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_alorah_and_grimminAI(creature);
    }
};

/*######
## npc_webbed_crusader
 ######*/

enum eWebbedCrusaderSpells {
    NPC_FORGOTTEN_DEEPS_AMBUSHER = 30204,
    NPC_FREED_CRUSADER = 30274,
    SPELL_FREED_CRUSADER = 56423
};
// UPDATE creature_template SET scriptname = 'npc_webbed_crusader' WHERE entry in (30273, 30268);
class npc_webbed_crusader: public CreatureScript {
public:
    npc_webbed_crusader() :
            CreatureScript("npc_webbed_crusader") {
    }

    struct npc_webbed_crusaderAI: public ScriptedAI {
        npc_webbed_crusaderAI(Creature* creature) : ScriptedAI(creature) { }
        void JustDied(Unit* killer) {
            switch (urand(0, 1)) {
            case 0: {
                Creature* temp = DoSpawnCreature(NPC_FREED_CRUSADER, 0, 0, 0, 0,
                        TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                if (Player* pPlayer = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
                    pPlayer->KilledMonsterCredit(NPC_FREED_CRUSADER,
                            temp->GetGUID());
                //DoCast(me, SPELL_FREED_CRUSADER, true);
            }
                break;
            case 1: {
                if (Creature* temp = DoSpawnCreature(NPC_FORGOTTEN_DEEPS_AMBUSHER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000)) {
                    temp->CastSpell(temp, 56418, true);
                    temp->SetStandState(UNIT_STAND_STATE_STAND);
                    temp->AI()->AttackStart(killer);
                }
            }
                break;
            }
        }
    };

    CreatureAI *GetAI(Creature *creature) const {
        return new npc_webbed_crusaderAI(creature);
    }
};

/*########
## npc_the_ocular
 #########*/

enum TheOcularSpells {
    SPELL_THE_OCULAR_TRANSFORM = 55162,
    SPELL_DEATHLY_STARE = 55269,
    SPELL_ITS_ALL_FUN_AND_GAMES_THE_OCULAR_ON_DEATH = 55288,
    SPELL_ITS_ALL_FUN_AND_GAMES_THE_OCULAR_KILL_CREDIT = 55289
};

enum ReqCreatures {
    NPC_THE_OCULAR = 29747, NPC_THE_OCULAR_DESTROYED_KILL_CREDIT_BUNNY = 29803
};

class npc_the_ocular: public CreatureScript {
public:
    npc_the_ocular() :
            CreatureScript("npc_the_ocular") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_the_ocularAI(creature);
    }

    struct npc_the_ocularAI: public ScriptedAI {
        npc_the_ocularAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        uint32 uiDeathlyStareTimer;

        void Reset() {
            uiDeathlyStareTimer = (urand(5000, 7000));
        }

        void DamageTaken(Unit* attacker, uint32 &damage) {
            me->LowerPlayerDamageReq(damage);
        }

        void JustDied(Unit* killer) {
            if (killer && killer->ToPlayer())
                killer->ToPlayer()->CastSpell(killer,
                        SPELL_ITS_ALL_FUN_AND_GAMES_THE_OCULAR_KILL_CREDIT,
                        true);
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!me->HasAura(SPELL_THE_OCULAR_TRANSFORM))
                DoCast(me, SPELL_THE_OCULAR_TRANSFORM, true);

            if (!UpdateVictim())
                return;

            if (uiDeathlyStareTimer <= uiDiff) {
                DoCastVictim(SPELL_DEATHLY_STARE);
                uiDeathlyStareTimer = (urand(7000, 9000));
            } else
                uiDeathlyStareTimer -= uiDiff;
        }
    };
};

/*########
## npc_free_your_mind_vile
 #########*/

enum eFreeYourMindNPCSpells {
    SPELL_SOVEREIGN_ROD = 29070, SPELL_SOVEREIGN_ROD_TRIGGERED = 29071,
    // Vile Abilities
    SPELL_VILE_ENRAGE = 56646, // <50% HP ?
    SPELL_VILE_BACKHAND = 6253,
    // Lady Nightswood Abilities
    SPELL_BANSHEE_CURSE = 5884,
    SPELL_BANSHEE_SHRIEK = 16838,
    // The Leaper Abilities
    SPELL_LEAPER_SNISTER_STRIKE = 60195,
    SPELL_LEAPER_HUNGER_FOR_BLOOD = 60177,
};

enum eFreeYourMindNPCEntrys {
    ENTRY_FYM_VILE = 29769, ENTRY_FYM_LADY = 29770, ENTRY_FYM_LEAPER = 29840
};

#define SAY_VILE_AGGRO              "Crush... maim... DESTROY!"
#define SAY_VILE_FREED              "Vile free? Vile love %n"

#define SAY_LADY_NIGHTSWOOD_AGGRO   "Who intrudes upon my ritual?"
#define SAY_LADY_NIGHTSWOOD_FREED   " You dare? Where is Baron Sliver? I would have words with him!"

#define SAY_THE_LEAPER_AGGRO        "Mrrfrmrfrmrrrrr!"
#define SAY_THE_LEAPER_FREED        "Mrmrmmrmrmrmrm... mrmrmrmr?!"

class npc_free_your_mind: public CreatureScript {
public:
    npc_free_your_mind() :
            CreatureScript("npc_free_your_mind") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_free_your_mindAI(creature);
    }

    struct npc_free_your_mindAI: public ScriptedAI {
        npc_free_your_mindAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        bool Enraged;

        uint32 uiSpell1Entry_Timer;
        uint32 uiSpell2Entry_Timer;
        uint32 uiSpell1Entry;
        uint32 uiSpell2Entry;

        void Reset() {
            switch (me->GetEntry()) {
            case ENTRY_FYM_VILE:
                uiSpell1Entry = SPELL_VILE_BACKHAND;
                uiSpell1Entry_Timer = urand(4000, 6000);
                uiSpell2Entry = SPELL_VILE_ENRAGE;
                break;
            case ENTRY_FYM_LADY:
                uiSpell1Entry = SPELL_BANSHEE_CURSE;
                uiSpell1Entry_Timer = urand(5000, 6000);
                uiSpell2Entry = SPELL_BANSHEE_SHRIEK;
                uiSpell2Entry_Timer = urand(10000, 12000);
                break;
            case ENTRY_FYM_LEAPER:
                uiSpell1Entry = SPELL_LEAPER_SNISTER_STRIKE;
                uiSpell1Entry_Timer = urand(4000, 6000);
                uiSpell2Entry = SPELL_LEAPER_HUNGER_FOR_BLOOD;
                break;
            }

            me->RestoreFaction();
        }

        void EnterCombat(Unit* who) {
            Enraged = false;
            switch (me->GetEntry()) {
            case ENTRY_FYM_VILE:
                me->MonsterSay(SAY_VILE_AGGRO, LANG_UNIVERSAL, who->GetGUID());
                break;
            case ENTRY_FYM_LEAPER:
                me->MonsterSay(SAY_THE_LEAPER_AGGRO, LANG_UNIVERSAL,
                        who->GetGUID());
                break;
            case ENTRY_FYM_LADY:
                me->MonsterSay(SAY_LADY_NIGHTSWOOD_AGGRO, LANG_UNIVERSAL,
                        who->GetGUID());
                break;
            }
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) {
            if (spell->Id == SPELL_SOVEREIGN_ROD_TRIGGERED) {
                if (caster && caster->ToPlayer()) {
                    me->setDeathState(ALIVE);
                    me->setFaction(35);
                    me->DespawnOrUnsummon(4000);

                    switch (me->GetEntry()) {
                    case ENTRY_FYM_VILE:
                        me->MonsterSay(SAY_VILE_FREED, LANG_UNIVERSAL,
                                caster->GetGUID());
                        caster->ToPlayer()->KilledMonsterCredit(29845, 0);
                        break;
                    case ENTRY_FYM_LEAPER:
                        me->MonsterSay(SAY_THE_LEAPER_FREED, LANG_UNIVERSAL,
                                caster->GetGUID());
                        caster->ToPlayer()->KilledMonsterCredit(29847, 0);
                        break;
                    case ENTRY_FYM_LADY:
                        me->MonsterSay(SAY_LADY_NIGHTSWOOD_FREED,
                                LANG_UNIVERSAL, caster->GetGUID());
                        caster->ToPlayer()->KilledMonsterCredit(29846, 0);
                        break;
                    }
                }
            }
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!UpdateVictim())
                return;

            if (uiSpell1Entry_Timer <= uiDiff) {
                DoCastVictim(uiSpell1Entry);
                switch (me->GetEntry()) {
                case ENTRY_FYM_VILE:
                case ENTRY_FYM_LEAPER:
                    uiSpell1Entry_Timer = (urand(7000, 9000));
                    break;
                case ENTRY_FYM_LADY:
                    uiSpell1Entry_Timer = (urand(10000, 15000));
                    break;
                }
            } else
                uiSpell1Entry_Timer -= uiDiff;

            if (me->GetEntry() == ENTRY_FYM_VILE) {
                if (!Enraged && HealthBelowPct(30)) {
                    DoCast(me, uiSpell2Entry, true);
                    Enraged = true;
                }
            } else {
                if (uiSpell2Entry_Timer <= uiDiff) {
                    DoCastVictim(uiSpell2Entry);
                    uiSpell2Entry_Timer = (urand(8000, 10000));
                } else
                    uiSpell2Entry_Timer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

/*########
## npc_general_lightsbane
 #########*/

enum eGeneralLightsbaneSpells {
    SPELL_CLEAVE = 15284,
    SPELL_DEATH_AND_DECAY = 60160,
    SPELL_PLAGUE_STRIKE = 60186,
};

enum eNpcs {
    ENTRY_VILE = 29860, ENTRY_THE_LEAPER = 29859, ENTRY_LADY_NIGHTSWOOD = 29858,
};

class npc_general_lightsbane: public CreatureScript {
public:
    npc_general_lightsbane() :
            CreatureScript("npc_general_lightsbane") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_general_lightsbaneAI(creature);
    }

    struct npc_general_lightsbaneAI: public ScriptedAI {
        npc_general_lightsbaneAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        uint32 uiCleave_Timer;
        uint32 uiDeathDecay_Timer;
        uint32 uiPlagueStrike_Timer;
        uint32 uiSummonSupport_Timer;
        bool supportSummoned;

        void Reset() {
            uiCleave_Timer = urand(2000, 3000);
            uiDeathDecay_Timer = urand(15000, 20000);
            uiPlagueStrike_Timer = urand(5000, 10000);

            std::list<Creature*> TargetList;
            me->GetCreatureListWithEntryInGrid(TargetList, me->GetEntry(),
                    100.0f);
            if (TargetList.size() > 1) {
                me->DespawnOrUnsummon(1000);
            }

            uiSummonSupport_Timer = 5000;
            supportSummoned = false;
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!UpdateVictim())
                return;

            if (!supportSummoned)
                if (uiSummonSupport_Timer <= uiDiff) {
                    Creature* temp = DoSummon(ENTRY_VILE, me, 5, 20000,
                            TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                    temp->AI()->AttackStart(me);

                    temp = DoSummon(ENTRY_THE_LEAPER, me, 5, 20000,
                            TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                    temp->AI()->AttackStart(me);

                    temp = DoSummon(ENTRY_LADY_NIGHTSWOOD, me, 5, 20000,
                            TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                    temp->AI()->AttackStart(me);

                    uiSummonSupport_Timer = (urand(4000, 5000));
                    supportSummoned = true;
                } else
                    uiSummonSupport_Timer -= uiDiff;

            if (uiCleave_Timer <= uiDiff) {
                DoCastVictim(SPELL_CLEAVE);
                uiCleave_Timer = (urand(4000, 5000));
            } else
                uiCleave_Timer -= uiDiff;

            if (uiDeathDecay_Timer <= uiDiff) {
                DoCastVictim(SPELL_DEATH_AND_DECAY);
                uiDeathDecay_Timer = urand(15000, 20000);
            } else
                uiDeathDecay_Timer -= uiDiff;

            if (uiPlagueStrike_Timer <= uiDiff) {
                DoCastVictim(SPELL_PLAGUE_STRIKE);
                uiPlagueStrike_Timer = urand(5000, 10000);
            } else
                uiPlagueStrike_Timer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

/*########
## Saronite Mine Slave
 #########*/

enum eEntrysSlaveToSaronite {
    QUEST_SLAVES_TO_SARONITE_ALLIANCE = 13300,
    QUEST_SLAVES_TO_SARONITE_HORDE = 13302,

    ENTRY_SLAVE_QUEST_CREDIT = 31866,

    SPELL_SLAVE_ENRAGE = 8599,
    SPELL_HEAD_CRACK = 3148,

    ACTION_ENRAGED = 0,
    ACTION_INSANE = 1,
    ACTION_FREED = 2,
};

const Position FreedPos[2] = { { 7030.0f, 1862.0f, 533.2f, 0.0f }, { 6947.0f,
        2027.0f, 519.7f, 0.0f } };

#define GOSSIP_OPTION_FREE  "Go on, you're free. Get out of here!"

#define SAY_SLAVE_AGGRO_1 "AHAHAHAHA... you'll join us soon enough!"
#define SAY_SLAVE_AGGRO_2 "I don't want to leave! I want to stay here!"
#define SAY_SLAVE_AGGRO_3 "I won't leave!"
#define SAY_SLAVE_AGGRO_4 "NO! You're wrong! The voices in my head are beautiful!"

#define SAY_SLAVE_INSANE_1 "I must get further underground to where he is. I must jump!"
#define SAY_SLAVE_INSANE_2 "I'll never return. The whole reason for my existence awaits below!"
#define SAY_SLAVE_INSANE_3 "I'm coming, master!"
#define SAY_SLAVE_INSANE_4 "My life for you!"

class npc_saronite_mine_slave: public CreatureScript {
public:
    npc_saronite_mine_slave() :
            CreatureScript("npc_saronite_mine_slave") {
    }

    bool OnGossipHello(Player* player, Creature* creature) {
        if ((player->GetQuestStatus(QUEST_SLAVES_TO_SARONITE_HORDE)
                == QUEST_STATUS_INCOMPLETE)
                || (player->GetQuestStatus(QUEST_SLAVES_TO_SARONITE_ALLIANCE)
                        == QUEST_STATUS_INCOMPLETE))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_OPTION_FREE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/,
            uint32 uiAction) {
        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();

        if (uiAction == (GOSSIP_ACTION_INFO_DEF + 1)) {
            creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
            if (urand(0, 1) == 0) {
                creature->AI()->DoAction(ACTION_FREED);
                player->KilledMonsterCredit(ENTRY_SLAVE_QUEST_CREDIT, 0);
            } else {
                if (urand(0, 1) == 0) {
                    creature->AI()->DoAction(ACTION_ENRAGED);
                    creature->setFaction(16);
                    //FIXME creature->CastSpell(creature, SPELL_SLAVE_ENRAGE);
                    creature->AI()->AttackStart(player);
                } else
                    creature->AI()->DoAction(ACTION_INSANE);
            }
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_saronite_mine_slaveAI(creature);
    }

    struct npc_saronite_mine_slaveAI: public ScriptedAI {
        npc_saronite_mine_slaveAI(Creature* creature) :
                ScriptedAI(creature) {
            alreadyFreed = false;
            enraged = false;
        }

        bool enraged;
        bool alreadyFreed;

        uint32 uiHeadCrack_Timer;

        void DoAction(const int32 action) {
            switch (action) {
            case ACTION_ENRAGED:
                enraged = true;
                alreadyFreed = true;
                switch (urand(0, 3)) {
                case 0:
                    me->MonsterYell(SAY_SLAVE_AGGRO_1, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 1:
                    me->MonsterYell(SAY_SLAVE_AGGRO_2, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 2:
                    me->MonsterYell(SAY_SLAVE_AGGRO_3, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 3:
                    me->MonsterYell(SAY_SLAVE_AGGRO_4, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                }
                break;
            case ACTION_FREED:
                alreadyFreed = true;
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->GetMotionMaster()->MovePoint(0, FreedPos[0]);
                me->DespawnOrUnsummon(15000);
                break;
            case ACTION_INSANE:
                alreadyFreed = true;
                switch (urand(0, 3)) {
                case 0:
                    me->MonsterYell(SAY_SLAVE_INSANE_1, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 1:
                    me->MonsterYell(SAY_SLAVE_INSANE_2, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 2:
                    me->MonsterYell(SAY_SLAVE_INSANE_3, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                case 3:
                    me->MonsterYell(SAY_SLAVE_INSANE_4, LANG_UNIVERSAL,
                            me->GetGUID());
                    break;
                }
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->GetMotionMaster()->MovePoint(0, FreedPos[1]);
                me->DespawnOrUnsummon(15000);
                break;
            }
        }

        void MoveInLineOfSight(Unit* mover) {
            if (!enraged)
                return;

            ScriptedAI::MoveInLineOfSight(mover);
        }

        void AttackStart(Unit* attacker) {
            if (!enraged)
                return;

            ScriptedAI::AttackStart(attacker);
        }

        void Reset() {
            me->RestoreFaction();
            if (alreadyFreed) {
                alreadyFreed = false;
                me->DespawnOrUnsummon(10000);
            } else {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                alreadyFreed = false;
                enraged = false;
            }
        }

        void EnterCombat(Unit* who) {
            uiHeadCrack_Timer = urand(5000, 7000);
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!UpdateVictim())
                return;

            if (uiHeadCrack_Timer <= uiDiff) {
                DoCastVictim(SPELL_HEAD_CRACK);
                uiHeadCrack_Timer = (urand(7000, 9000));
            } else
                uiHeadCrack_Timer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

/*########
## spell_tournament_defend
 #########*/

class spell_tournament_defend: public SpellScriptLoader {
public:
    spell_tournament_defend() :
            SpellScriptLoader("spell_tournament_defend") {
    }

    class spell_tournament_defend_AuraScript: public AuraScript {
        PrepareAuraScript(spell_tournament_defend_AuraScript)
        ;

        void OnStackChange(AuraEffect const* aurEff,
                AuraEffectHandleModes mode) {
            if (GetCaster())
                if (Unit* caster = GetCaster()->GetCharmerOrOwnerOrSelf()) {
                    caster->RemoveAurasDueToSpell(63130);
                    caster->RemoveAurasDueToSpell(63131);
                    caster->RemoveAurasDueToSpell(63132);

                    switch (GetStackAmount()) {
                    case 1:
                        caster->CastSpell(caster, 63130, true, 0, aurEff);
                        break;
                    case 2:
                        caster->CastSpell(caster, 63131, true, 0, aurEff);
                        break;
                    case 3:
                        caster->CastSpell(caster, 63132, true, 0, aurEff);
                        break;
                    }
                }
        }

        void OnAuraRemoved(AuraEffect const* /*aurEff*/,
                AuraEffectHandleModes /*mode*/) {
            if (GetCaster())
                if (Unit* caster = GetCaster()->GetCharmerOrOwnerOrSelf()) {
                    caster->RemoveAurasDueToSpell(63130);
                    caster->RemoveAurasDueToSpell(63131);
                    caster->RemoveAurasDueToSpell(63132);
                }
        }

        void Register() {
            AfterEffectApply +=
                    AuraEffectApplyFn(spell_tournament_defend_AuraScript::OnStackChange, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT);
            AfterEffectApply +=
                    AuraEffectApplyFn(spell_tournament_defend_AuraScript::OnStackChange, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove +=
                    AuraEffectRemoveFn(spell_tournament_defend_AuraScript::OnAuraRemoved, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);

            AfterEffectApply +=
                    AuraEffectApplyFn(spell_tournament_defend_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT);
            AfterEffectApply +=
                    AuraEffectApplyFn(spell_tournament_defend_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove +=
                    AuraEffectRemoveFn(spell_tournament_defend_AuraScript::OnAuraRemoved, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const {
        return new spell_tournament_defend_AuraScript();
    }
};

/*######
## npc_vendor_tournament_fraction_champion
 ######*/

enum eAchievementsTournamentFactionChampion 
{
    ACHIEVEMENT_CHAMPION_OF_DARNASSUS = 2777,
    ACHIEVEMENT_CHAMPION_OF_EXODAR = 2778,
    ACHIEVEMENT_CHAMPION_OF_GNOMEREGAN = 2779,
    ACHIEVEMENT_CHAMPION_OF_IRONFORGE = 2780,
    ACHIEVEMENT_CHAMPION_OF_STORMWIND = 2781,
    ACHIEVEMENT_CHAMPION_OF_ORGRIMMAR = 2783,
    ACHIEVEMENT_CHAMPION_OF_DARKSPEARS = 2784,
    ACHIEVEMENT_CHAMPION_OF_SILVERMOON = 2785,
    ACHIEVEMENT_CHAMPION_OF_THUNDERBLUFF = 2786,
    ACHIEVEMENT_CHAMPION_OF_UNDERCITY = 2787,
};

enum eNPCVendorEntrys 
{
    ENTRY_EXODAR_VENDOR = 33657,
    ENTRY_GNOMEREGAN_VENDOR = 33650,
    ENTRY_DARNASSUS_VENDOR = 33653,
    ENTRY_STORMWIND_VENDOR = 33307,
    ENTRY_IRONFORGE_VENDOR = 33310,

    ENTRY_ORGRIMMAR_VENDOR = 33553,
    ENTRY_DARKSPEARS_VENDOR = 33554,
    ENTRY_SILVERMOON_VENDOR = 33557,
    ENTRY_THUNDERBLUFF_VENDOR = 33556,
    ENTRY_UNDERCITY_VENDOR = 33555,
};

class npc_vendor_tournament_fraction_champion: public CreatureScript 
{
public:
    npc_vendor_tournament_fraction_champion() :
            CreatureScript("npc_vendor_tournament_fraction_champion") {
    }

    bool OnGossipHello(Player* player, Creature* creature) {
        bool showVendor = false;

        switch (creature->GetEntry()) {
        case ENTRY_EXODAR_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_EXODAR);
            break;
        case ENTRY_GNOMEREGAN_VENDOR:
            showVendor = player->HasAchieved(
                    ACHIEVEMENT_CHAMPION_OF_GNOMEREGAN);
            break;
        case ENTRY_DARNASSUS_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_DARNASSUS);
            break;
        case ENTRY_STORMWIND_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_STORMWIND);
            break;
        case ENTRY_IRONFORGE_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_IRONFORGE);
            break;
        case ENTRY_ORGRIMMAR_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_ORGRIMMAR);
            break;
        case ENTRY_DARKSPEARS_VENDOR:
            showVendor = player->HasAchieved(
                    ACHIEVEMENT_CHAMPION_OF_DARKSPEARS);
            break;
        case ENTRY_SILVERMOON_VENDOR:
            showVendor = player->HasAchieved(
                    ACHIEVEMENT_CHAMPION_OF_SILVERMOON);
            break;
        case ENTRY_THUNDERBLUFF_VENDOR:
            showVendor = player->HasAchieved(
                    ACHIEVEMENT_CHAMPION_OF_THUNDERBLUFF);
            break;
        case ENTRY_UNDERCITY_VENDOR:
            showVendor = player->HasAchieved(ACHIEVEMENT_CHAMPION_OF_UNDERCITY);
            break;
        }

        if (showVendor) {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/,
            uint32 uiAction) {
        player->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());
        return true;
    }
};

/*######
## npc_maiden_of_drakmar
 ######*/

#define SAY_1       "Are those winter hyacinths? For me?"
#define SAY_2       "It\'s been so long since a traveler has come here bearing flowers."
#define SAY_3       "The lake has been too lonely these past years. The travelers stopped coming and evil came to these waters."
#define SAY_4       "Your gift is a rare kindness, traveler. Please take this blade with my gratitude. Long ago, another traveler left it here, but I have little use for it."

enum eEntrys {
    GO_DRAKMAR_LILY_PAD = 194239,
    GO_BLADE_OF_DRAKMAR = 194238,
    ENTRY_MAIDEN_OF_DRAKMAR = 33273,
};

static Position miscLocations[] = {
        { 4602.08f, -1600.22f, 156.657f, 0.128299f }, //GO_DRAKMAR_LILY_PAD
        { 4601.53f, -1600.47f, 156.986f, 1.944937f } //GO_BLADE_OF_DRAKMAR
};

class npc_maiden_of_drakmar: public CreatureScript {
public:
    npc_maiden_of_drakmar() :
            CreatureScript("npc_maiden_of_drakmar") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_maiden_of_drakmarAI(creature);
    }

    struct npc_maiden_of_drakmarAI: public ScriptedAI {
        npc_maiden_of_drakmarAI(Creature* creature) :
                ScriptedAI(creature) {
            me->SetCanFly(true); // cosmetic workaround
        }

        uint32 uiSayTimer;
        uint8 uiSayStep;

        void Reset() {
            uiSayStep = 0;
            uiSayTimer = 2000;
        }

        void UpdateAI(const uint32 uiDiff) {
            if (uiSayTimer <= uiDiff) {
                switch (uiSayStep) {
                case 0:
                    me->SummonGameObject(GO_DRAKMAR_LILY_PAD,
                            miscLocations[0].GetPositionX(),
                            miscLocations[0].GetPositionY(),
                            miscLocations[0].GetPositionZ(),
                            miscLocations[0].GetOrientation(), 0, 0, 0, 0,
                            30000);
                    me->MonsterSay(SAY_1, LANG_UNIVERSAL, 0);
                    uiSayTimer = 3000;
                    ++uiSayStep;
                    break;
                case 1:
                    me->MonsterSay(SAY_2, LANG_UNIVERSAL, 0);
                    uiSayTimer = 5000;
                    ++uiSayStep;
                    break;
                case 2:
                    me->MonsterSay(SAY_3, LANG_UNIVERSAL, 0);
                    uiSayTimer = 7000;
                    ++uiSayStep;
                    break;
                case 3:
                    me->SummonGameObject(GO_BLADE_OF_DRAKMAR,
                            miscLocations[1].GetPositionX(),
                            miscLocations[1].GetPositionY(),
                            miscLocations[1].GetPositionZ(),
                            miscLocations[1].GetOrientation(), 0, 0, 0, 0,
                            30000);
                    me->MonsterSay(SAY_4, LANG_UNIVERSAL, 0);
                    uiSayTimer = 20000;
                    ++uiSayStep;
                    break;
                }
            } else
                uiSayTimer -= uiDiff;
        }
    };
};

/*######
 npc_squire_cavin
 ######*/

enum eSquireCalvin {
    QUEST_THE_BLACK_KNIGHTS_FALL = 13664, NPC_BLACK_KNIGHT = 33785,
};

#define GOSSIP_SUMMON_BLACK_KNIGHT      "Ask Cavin to summon the Black Knight."

class npc_squire_cavin: public CreatureScript {
public:
    npc_squire_cavin() :
            CreatureScript("npc_squire_cavin") {
    }

    bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 uiSender,
            uint32 uiAction) {
        pPlayer->PlayerTalkClass->ClearMenus();
        if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) {
            Position pos;
            creature->GetPosition(&pos);
            {
                if (TempSummon* temp = creature->SummonCreature(NPC_BLACK_KNIGHT, pos, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                    temp->AI()->SetGUID(pPlayer->GetGUID());
            }
            pPlayer->CLOSE_GOSSIP_MENU();
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* creature) {
        if (pPlayer->HasAura(63663)
                && (pPlayer->GetQuestStatus(QUEST_THE_BLACK_KNIGHTS_FALL)
                        == QUEST_STATUS_INCOMPLETE)) {
            std::list<Creature*> checkList;
            creature->GetCreatureListWithEntryInGrid(checkList,
                    NPC_BLACK_KNIGHT, 100.0f);
            if (checkList.size() == 0)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SUMMON_BLACK_KNIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};



/*########
## npc_the_black_knight
 #########*/

enum eBlackKnight {
    SPELL_BK_CHARGE = 63003,
    //SPELL_SHIELD_BREAKER        = 65147,
    SPELL_DARK_SHIELD = 64505,
};

#define YELL_ATTACK_PHASE_1_END         "Get off that horse and fight me man-to-man!"
#define YELL_ATTACK_PHASE_2             "I will not fail you, master!"

class npc_the_black_knight: public CreatureScript {
public:
    npc_the_black_knight() :
            CreatureScript("npc_the_black_knight") {
    }

    struct npc_the_black_knightAI: public ScriptedAI {
        npc_the_black_knightAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        uint32 uiChargeTimer;
        uint32 uiShieldBreakerTimer;
        uint64 guidAttacker;
        uint32 uireattackTimer;

        bool chargeing;

        bool mountDuel;
        bool handDuel;

        void Reset() {
            uiChargeTimer = 7000;
            uiShieldBreakerTimer = 10000;
            uireattackTimer = 10000;

            me->setFaction(35);

            mountDuel = false;
            handDuel = false;
        }

        void SetGUID(uint64 guid, int32) {
            if (Player* plr = Player::GetPlayer(*me, guid)) {
                guidAttacker = guid;
                mountDuel = true;
                handDuel = false;
                me->setFaction(14);
                me->Mount(28652);
                AttackStart(plr);
                // Move Point

                me->SetMaxHealth(50000);
                me->SetHealth(50000);
            }
        }

        void JustDied(Unit* killer) {
            me->DespawnOrUnsummon(5000);
        }

        void EnterCombat(Unit* attacker) {
            DoCast(me, SPELL_DEFEND_AURA_PERIODIC, true);
            if (Aura* aur = me->AddAura(SPELL_DEFEND, me))
                aur->ModStackAmount(1);
        }

        void MovementInform(uint32 uiType, uint32 uiId) {
            if (uiType != POINT_MOTION_TYPE)
                return;

            if (uiId == 1) {
                chargeing = false;

                DoCastVictim(SPELL_BK_CHARGE);
                if (me->getVictim())
                    me->GetMotionMaster()->MoveChase(me->getVictim());
            } else if (uiId == 2) {
                if (Player* plr = Player::GetPlayer(*me, guidAttacker)) {
                    AttackStart(plr);
                }
            }
        }

        void DamageTaken(Unit* pDoneBy, uint32& uiDamage) {
            if (pDoneBy && pDoneBy->GetGUID() != guidAttacker)
                uiDamage = 0;

            if (handDuel)
                return;
            if (!mountDuel)
                return;

            if (uiDamage > me->GetHealth()
                    && pDoneBy->GetTypeId() == TYPEID_PLAYER) {
                uiDamage = 0;
                mountDuel = false;
                me->SetHealth(50000);
                me->Dismount();
                me->GetMotionMaster()->MoveIdle();
                me->RemoveAurasDueToSpell(SPELL_DEFEND_AURA_PERIODIC);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->MonsterYell(YELL_ATTACK_PHASE_1_END, LANG_UNIVERSAL,
                        guidAttacker);
                uireattackTimer = 10000;
            }
        }

        void UpdateAI(const uint32 uiDiff) {
            if (!UpdateVictim())
                return;

            if (mountDuel) {
                if (uiChargeTimer <= uiDiff) {
                    chargeing = true;
                    float x, y, z;
                    me->GetNearPoint(me, x, y, z, 1.0f, 15.0f,
                            float(M_PI * 2 * rand_norm()));
                    me->GetMotionMaster()->MovePoint(1, x, y, z);

                    uiChargeTimer = 7000;
                } else
                    uiChargeTimer -= uiDiff;

                if (uiShieldBreakerTimer <= uiDiff) {
                    DoCastVictim(SPELL_SHIELD_BREAKER);
                    uiShieldBreakerTimer = 10000;
                } else
                    uiShieldBreakerTimer -= uiDiff;

                if (me->isAttackReady()) {
                    DoCast(me->getVictim(), SPELL_THRUST, true);
                    me->resetAttackTimer();
                }
            } else if (handDuel) {
                if (uiShieldBreakerTimer <= uiDiff) {
                    DoCastVictim(SPELL_DARK_SHIELD);
                    uiShieldBreakerTimer = 30000;
                } else
                    uiShieldBreakerTimer -= uiDiff;

                DoMeleeAttackIfReady();
            } else {
                if (uireattackTimer <= uiDiff) {
                    handDuel = true;
                    if (me->getVictim())
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                    if (Player* plr = Player::GetPlayer(*me, guidAttacker))
                        plr->ExitVehicle();

                    me->SetMaxHealth(12500);
                    me->SetHealth(12500);
                    me->MonsterYell(YELL_ATTACK_PHASE_2, LANG_UNIVERSAL,
                            guidAttacker);
                    uireattackTimer = 99999999;
                } else
                    uireattackTimer -= uiDiff;
            }
        }
    };

    CreatureAI *GetAI(Creature* creature) const {
        return new npc_the_black_knightAI(creature);
    }
};

/*########
## npc_slain_tualiq_villager
 #########*/
// Maybe rewrite with SmartAI
class npc_slain_tualiq_villager: public CreatureScript {
public:
    npc_slain_tualiq_villager() :
            CreatureScript("npc_slain_tualiq_villager") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_slain_tualiq_villagerAI(creature);
    }

    struct npc_slain_tualiq_villagerAI: public ScriptedAI {
        npc_slain_tualiq_villagerAI(Creature* creature) :
                ScriptedAI(creature) {
        }

        bool credited;

        void Reset() {
            credited = false;
        }

        void SpellHit(Unit* caster, const SpellInfo* spellinfo) {
            if (credited)
                return;

            if (caster && caster->ToPlayer()) {
                if (spellinfo->Id == 66390) {
                    credited = true;
                    caster->ToPlayer()->KilledMonsterCredit(me->GetEntry(),
                            me->GetGUID());
                    me->DespawnOrUnsummon(3000);
                }
            }
        }

        void AttackStart(Unit* who) {
            return;
        }

        void MoveInLineOfSight(Unit *who) {
            return;
        }

        void UpdateAI(const uint32 uiDiff) {
            return;
        }
    };
};

/*######
## vehicle_black_knights_gryphon
 ######*/

const Position BlackKnightGryphonWaypoints[19] = { { 8522.41f, 582.23f, 552.29f,
        0.0f }, { 8502.92f, 610.34f, 550.01f, 0.0f }, { 8502.50f, 628.61f,
        547.38f, 0.0f }, { 8484.50f, 645.16f, 547.30f, 0.0f }, { 8454.49f,
        693.96f, 547.30f, 0.0f }, { 8403.00f, 742.34f, 547.30f, 0.0f }, {
        8374.00f, 798.35f, 547.93f, 0.0f },
        { 8376.43f, 858.33f, 548.00f, 0.0f },
        { 8388.22f, 868.56f, 547.78f, 0.0f },
        { 8465.58f, 871.45f, 547.30f, 0.0f }, { 8478.29f, 1014.63f, 547.30f,
                0.0f }, { 8530.86f, 1037.65f, 547.30f, 0.0f }, { 8537.69f,
                1078.33f, 554.80f, 0.0f },
        { 8537.69f, 1078.33f, 578.10f, 0.0f }, { 8740.47f, 1611.72f, 496.19f,
                0.0f }, { 9025.06f, 1799.67f, 171.54f, 0.0f }, { 9138.47f,
                2013.83f, 104.24f, 0.0f }, { 9081.39f, 2158.26f, 72.98f, 0.0f },
        { 9054.00f, 2124.85f, 57.13f, 0.0f } };

class vehicle_black_knights_gryphon: public CreatureScript {
public:
    vehicle_black_knights_gryphon() :
            CreatureScript("vehicle_black_knights_gryphon") {
    }

    CreatureAI* GetAI(Creature *_Creature) const {
        return new vehicle_black_knights_gryphonAI(_Creature);
    }

    struct vehicle_black_knights_gryphonAI: public VehicleAI {
        vehicle_black_knights_gryphonAI(Creature *c) :
                VehicleAI(c) {
            if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(3548)))
                vehSeat->m_flags |= VEHICLE_SEAT_FLAG_UNCONTROLLED;
        }

        bool isInUse;

        bool wp_reached;
        uint8 count;

        void Reset() {
            count = 0;
            wp_reached = false;
            isInUse = false;
        }

        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) {
            if (who && apply) {
                isInUse = apply;
                wp_reached = true;
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                me->SetSpeed(MOVE_RUN, 2.0f);
                me->SetSpeed(MOVE_FLIGHT, 3.5f);
            }
        }

        void MovementInform(uint32 type, uint32 id) {
            if (type != POINT_MOTION_TYPE || id != count)
                return;

            if (id < 18) {
                if (id > 11) {
                    me->SetUnitMovementFlags(MOVEMENTFLAG_DISABLE_GRAVITY);
                    me->SetSpeed(MOVE_RUN, 5.0f);
                    //me->SetFlying(true);
                }

                ++count;
                wp_reached = true;
            } else {
                Unit* player = me->GetVehicleKit()->GetPassenger(0);
                if (player && player->GetTypeId() == TYPEID_PLAYER) {
                    player->ToPlayer()->KilledMonsterCredit(me->GetEntry(),
                            me->GetGUID());
                    player->ExitVehicle();
                    me->DespawnOrUnsummon(5000);
                }
            }
        }

        void UpdateAI(const uint32 diff) {
            if (!me->IsVehicle())
                return;

            if (!isInUse)
                return;

            if (wp_reached) {
                wp_reached = false;
                me->GetMotionMaster()->MovePoint(count,
                        BlackKnightGryphonWaypoints[count]);
            }
        }
    };
};



/*########
## spell_flaming_spear
 #########*/

class spell_flaming_spear_targeting: public SpellScriptLoader {
public:
    spell_flaming_spear_targeting() :
            SpellScriptLoader("spell_flaming_spear_targeting") {
    }

    class spell_flaming_spear_targeting_SpellScript: public SpellScript {
        PrepareSpellScript(spell_flaming_spear_targeting_SpellScript)
        ;

        bool Load() {
            return GetCaster()->ToPlayer() != NULL;
        }

        void GetTargets(std::list<Unit*>& targetList) {
            targetList.clear();

            std::list<Creature*> pTargetList;
            GetCaster()->GetCreatureListWithEntryInGrid(pTargetList, 35092,
                    150.0f);
            GetCaster()->GetCreatureListWithEntryInGrid(pTargetList, 34925,
                    150.0f);

            if (pTargetList.empty())
                return;

            pTargetList.sort(Trinity::ObjectDistanceOrderPred(GetCaster()));

            std::list<Creature*>::iterator itr = pTargetList.begin();
            uint8 i = 3;
            for (std::list<Creature*>::const_iterator itr = pTargetList.begin();
                    itr != pTargetList.end(); ++itr) {
                if (i == 0)
                    break;
                if ((*itr)->isAlive()) {
                    targetList.push_back(*itr);
                    i--;
                }
            }
        }

        void Register() 
		{
            //OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_flaming_spear_targeting_SpellScript::GetTargets, EFFECT_0, TARGET_UNIT_TARGET_ANY);
        }
    };

    SpellScript* GetSpellScript() const {
        return new spell_flaming_spear_targeting_SpellScript();
    }
};

#define GOSSIP_TRADE        "Visit a trader."
#define GOSSIP_BANK         "Visit a bank."
#define GOSSIP_MAIL         "Visit a mailbox."

#define GOSSIP_ORC_PENNANT  "Orgrimmar Champion's Pennant"
#define GOSSIP_SEN_PENNANT  "Darkspear Champion's Pennant"
#define GOSSIP_UND_PENNANT  "Forsaken Champion's Pennant"
#define GOSSIP_SIL_PENNANT  "Silvermoon Champion's Pennant"
#define GOSSIP_TBL_PENNANT  "Thunder Bluff Champion's Pennant"

#define GOSSIP_STW_PENNANT  "Stormwind Champion's Pennant"
#define GOSSIP_IFR_PENNANT  "Ironforge Champion's Pennant"
#define GOSSIP_GNO_PENNANT  "Gnomeregan Champion's Pennant"
#define GOSSIP_DAR_PENNANT  "Darnassus Champion's Pennant"
#define GOSSIP_EXO_PENNANT  "Exodar Champion's Pennant"

/*######
## npc_argent_squire_gruntling
 ######*/

enum eSquireGruntling {
    SPELL_BANK_ERRAND_H = 68849,
    SPELL_POSTMAN_H = 68850,
    SPELL_SHOP_H = 68851,
    SPELL_TIRED_H = 68852,

    SPELL_BANK_ERRAND_A = 67368,
    SPELL_POSTMAN_A = 67376,
    SPELL_SHOP_A = 67377,
    SPELL_TIRED_A = 67401,

    SPELL_PEND_DAR = 63443,
    SPELL_PEND_GNO = 63442,
    SPELL_PEND_IRO = 63440,
    SPELL_PEND_ORG = 63444,
    SPELL_PEND_SEN = 63446,
    SPELL_PEND_SIL = 63438,
    SPELL_PEND_STO = 62727,
    SPELL_PEND_EXO = 63439,
    SPELL_PEND_UND = 63441,
    SPELL_PEND_THU = 63445,

    ACHIEVEMENT_CHAMP_DARNASSUS = 2777,
    ACHIEVEMENT_CHAMP_GNOMEREGAN = 2779,
    ACHIEVEMENT_CHAMP_IRONFORGE = 2780,
    ACHIEVEMENT_CHAMP_ORGRIMMAR = 2783,
    ACHIEVEMENT_CHAMP_SENJIN = 2784,
    ACHIEVEMENT_CHAMP_SILVERMOON = 2785,
    ACHIEVEMENT_CHAMP_STORMWIND = 2781,
    ACHIEVEMENT_CHAMP_EXODAR = 2778,
    ACHIEVEMENT_CHAMP_UNDERCITY = 2787,
    ACHIEVEMENT_CHAMP_THUNDERBLUFF = 2786,

    GOS_CHAMP_DAR = 1,
    GOS_CHAMP_GNO = 2,
    GOS_CHAMP_IRO = 3,
    GOS_CHAMP_ORG = 4,
    GOS_CHAMP_SEN = 5,
    GOS_CHAMP_SIL = 6,
    GOS_CHAMP_STO = 7,
    GOS_CHAMP_EXO = 8,
    GOS_CHAMP_UND = 9,
    GOS_CHAMP_THU = 10,

    ENTRY_SQUIRE = 33238,
    ENTRY_GRUNTLING = 33239,
};

class npc_argent_squire_gruntling: public CreatureScript {
public:
    npc_argent_squire_gruntling() :
            CreatureScript("npc_argent_squire_gruntling") {
    }

    bool canShowPostman(Creature* creature) 
	{
        if (creature->HasAura(SPELL_POSTMAN_H)
                || creature->HasAura(SPELL_POSTMAN_A))
            return true;

        if (creature->HasAura(SPELL_BANK_ERRAND_H)
                || creature->HasAura(SPELL_BANK_ERRAND_A))
            return false;

        if (creature->HasAura(SPELL_SHOP_H) || creature->HasAura(SPELL_SHOP_A))
            return false;

        if (creature->HasAura(SPELL_TIRED_H)
                || creature->HasAura(SPELL_TIRED_A))
            return false;

        return true;
    }

    bool canShowShop(Creature* creature) 
	{
        if (creature->HasAura(SPELL_POSTMAN_H)
                || creature->HasAura(SPELL_POSTMAN_A))
            return false;

        if (creature->HasAura(SPELL_BANK_ERRAND_H)
                || creature->HasAura(SPELL_BANK_ERRAND_A))
            return false;

        if (creature->HasAura(SPELL_SHOP_H) || creature->HasAura(SPELL_SHOP_A))
            return true;

        if (creature->HasAura(SPELL_TIRED_H)
                || creature->HasAura(SPELL_TIRED_A))
            return false;

        return true;
    }

    bool canShowBank(Creature* creature) 
	{
        if (creature->HasAura(SPELL_POSTMAN_H)
                || creature->HasAura(SPELL_POSTMAN_A))
            return false;

        if (creature->HasAura(SPELL_BANK_ERRAND_H)
                || creature->HasAura(SPELL_BANK_ERRAND_A))
            return true;

        if (creature->HasAura(SPELL_SHOP_H) || creature->HasAura(SPELL_SHOP_A))
            return false;

        if (creature->HasAura(SPELL_TIRED_H)
                || creature->HasAura(SPELL_TIRED_A))
            return false;

        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* creature) 
	{
        if (!pPlayer)
            return true;

        if (canShowBank(creature))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, GOSSIP_BANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_BANK);

        if (canShowShop(creature))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TRADE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        if (canShowPostman(creature))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_MAIL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_DARNASSUS))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DAR_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_DAR);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_GNOMEREGAN))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_GNO_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_GNO);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_IRONFORGE))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_IFR_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_IRO);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_ORGRIMMAR))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ORC_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_ORG);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_SENJIN))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_DAR_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_SEN);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_SILVERMOON))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SIL_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_SIL);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_STORMWIND))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_STO);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_EXODAR))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_EXO_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_EXO);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_UNDERCITY))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_UND_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_UND);

        if (pPlayer->HasAchieved(ACHIEVEMENT_CHAMP_THUNDERBLUFF))
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TBL_PENNANT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+GOS_CHAMP_THU);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    void cleanUpAllAuras(Creature* creature) 
	{
        creature->RemoveAurasDueToSpell(SPELL_PEND_DAR);
        creature->RemoveAurasDueToSpell(SPELL_PEND_GNO);
        creature->RemoveAurasDueToSpell(SPELL_PEND_IRO);
        creature->RemoveAurasDueToSpell(SPELL_PEND_ORG);
        creature->RemoveAurasDueToSpell(SPELL_PEND_SEN);
        creature->RemoveAurasDueToSpell(SPELL_PEND_SIL);
        creature->RemoveAurasDueToSpell(SPELL_PEND_STO);
        creature->RemoveAurasDueToSpell(SPELL_PEND_EXO);
        creature->RemoveAurasDueToSpell(SPELL_PEND_UND);
        creature->RemoveAurasDueToSpell(SPELL_PEND_THU);
    }

    bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 uiSender, uint32 uiAction) 
	{
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction) 
		{
			case GOSSIP_ACTION_BANK:
				pPlayer->GetSession()->SendShowBank(creature->GetGUID());
				break;
			case GOSSIP_ACTION_TRADE:
				pPlayer->GetSession()->SendListInventory(creature->GetGUID());
				break;
			case GOSSIP_ACTION_INFO_DEF:
				creature->MonsterSay("Nein, das kann ich nicht", LANG_UNIVERSAL, pPlayer->GetGUID());
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_DAR:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_DAR, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_GNO:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_GNO, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_IRO:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_IRO, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_ORG:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_ORG, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_SEN:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_SEN, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_SIL:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_SIL, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_STO:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_STO, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_EXO:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_EXO, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_UND:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_UND, true);
				break;
			case GOSSIP_ACTION_INFO_DEF + GOS_CHAMP_THU:
				cleanUpAllAuras(creature);
				creature->CastSpell(creature, SPELL_PEND_THU, true);
				break;
        }

        pPlayer->CLOSE_GOSSIP_MENU();
        return true;
    }
};

/*########
## npc_black_knights_grave
 #########*/

enum eGraveSpells {
    SPELL_COMPLETE_QUEST = 66785,

    ENTRY_CULT_ASSASSIN = 35127, ENTRY_CULT_SABOTEUR = 35116,
};

#define SAY_SABOTEUR_1              "What do you think you're doing?"
#define SAY_SABOTEUR_2              "You're too late to stop our plan."
#define SAY_SABOTEUR_3              "See to it that I'm not followed."
#define SAY_ASSASSIN_1              "Gladly."

class npc_black_knights_grave: public CreatureScript {
public:
    npc_black_knights_grave() :
            CreatureScript("npc_black_knights_grave") {
    }

    CreatureAI* GetAI(Creature* creature) const {
        return new npc_black_knights_graveAI(creature);
    }

    struct npc_black_knights_graveAI: public ScriptedAI {
        npc_black_knights_graveAI(Creature* creature) :
                ScriptedAI(creature), summons(creature) {
            me->setActive(true);
        }

        SummonList summons;
        bool bEventRunning;

        uint64 guidSaboteur;
        uint64 guidAssassin;

        uint32 uiSpeakTimer;
        uint32 uiSpeakPhase;

        void Reset() {
            bEventRunning = false;
            uiSpeakPhase = 0;
        }

        void StartEvent(uint64 attacker = 0) {
            if (bEventRunning)
                return;

            bEventRunning = true;

            Creature* cre = DoSummon(ENTRY_CULT_SABOTEUR, me, 5.0f, 30000,
                    TEMPSUMMON_TIMED_DESPAWN);
            if (cre) {
                cre->SetStandState(UNIT_STAND_STATE_KNEEL);
                cre->SetReactState(REACT_PASSIVE);
                cre->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                cre->setFaction(14);
                guidSaboteur = cre->GetGUID();
            }

            cre = DoSummon(ENTRY_CULT_ASSASSIN, me, 5.0f, 40000,
                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
            if (cre) {
                cre->SetReactState(REACT_PASSIVE);
                cre->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                cre->setFaction(14);
                guidAssassin = cre->GetGUID();
            }

            uiSpeakTimer = 7000;
            uiSpeakPhase = 1;
        }

        void StopEvent() {
            DoCastAOE(SPELL_COMPLETE_QUEST, true);
            bEventRunning = false;
            summons.DespawnAll();
            uiSpeakPhase = 0;
        }

        void JustSummoned(Creature* pSummoned) {
            summons.Summon(pSummoned);
        }

        void MoveInLineOfSight(Unit *who) {
            if (!who || !who->ToPlayer())
                return;

            if (who->IsWithinDist(me, 20.0f))
                if (who->ToPlayer()->GetQuestStatus(14016)
                        == QUEST_STATUS_INCOMPLETE)
                    StartEvent(who->GetGUID());

            return;
        }

        void UpdateAI(const uint32 uiDiff) {
            if (bEventRunning) {
                if (uiSpeakTimer <= uiDiff) {
                    switch (uiSpeakPhase) {
                    case 1:
                        if (Creature* cre = Creature::GetCreature(*me, guidSaboteur))
                            cre->MonsterSay(SAY_SABOTEUR_1, LANG_UNIVERSAL, 0);
                        break;
                    case 2:
                        if (Creature* cre = Creature::GetCreature(*me, guidSaboteur))
                            cre->MonsterSay(SAY_SABOTEUR_2, LANG_UNIVERSAL, 0);
                        break;
                    case 3:
                        if (Creature* cre = Creature::GetCreature(*me, guidSaboteur))
                            cre->MonsterSay(SAY_SABOTEUR_3, LANG_UNIVERSAL, 0);
                        break;
                    case 4:
                        if (Creature* cre = Creature::GetCreature(*me, guidAssassin)) {
                            cre->MonsterSay(SAY_ASSASSIN_1, LANG_UNIVERSAL, 0);
                            cre->SetReactState(REACT_AGGRESSIVE);
                            cre->RemoveFlag(UNIT_FIELD_FLAGS,
                                    UNIT_FLAG_NON_ATTACKABLE);
                            std::list<Player*> pList;
                            cre->SelectNearestPlayer(20.0f);
                            if (pList.size() > 0) {
                                std::list<Player*>::const_iterator itr =
                                        pList.begin();
                                if ((*itr))
                                    cre->AI()->AttackStart((*itr));
                            }
                            summons.DespawnEntry(ENTRY_CULT_SABOTEUR);
                        }
                        break;
                    }
                    uiSpeakPhase++;
                    uiSpeakTimer = 3000;
                } else
                    uiSpeakTimer -= uiDiff;

                if (Creature* cre = Creature::GetCreature(*me, guidAssassin)) {
                    if (cre->isDead()) {
                        StopEvent();
                    }
                } else {
                    bEventRunning = false;
                    summons.DespawnAll();
                }
            }
        }
    };
};

/*######
## Borrowed Technology - Id: 13291, The Solution Solution (daily) - Id: 13292, Volatility - Id: 13239, Volatiliy - Id: 13261 (daily)
######*/

enum BorrowedTechnologyAndVolatility
{
    // Spells
    SPELL_GRAB             = 59318,
    SPELL_PING_BUNNY       = 59375,
    SPELL_IMMOLATION       = 54690,
    SPELL_EXPLOSION        = 59335,
    SPELL_RIDE             = 56687,

    // Points
    POINT_GRAB_DECOY       = 1,
    POINT_FLY_AWAY         = 2,

    // Events
    EVENT_FLY_AWAY         = 1
};

class npc_frostbrood_skytalon : public CreatureScript
{
    public:
        npc_frostbrood_skytalon() : CreatureScript("npc_frostbrood_skytalon") { }

        struct npc_frostbrood_skytalonAI : public VehicleAI
        {
            npc_frostbrood_skytalonAI(Creature* creature) : VehicleAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                me->GetMotionMaster()->MovePoint(POINT_GRAB_DECOY, summoner->GetPositionX(), summoner->GetPositionY(), summoner->GetPositionZ());
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == POINT_GRAB_DECOY)
                    if (TempSummon* summon = me->ToTempSummon())
                        if (Unit* summoner = summon->GetSummoner())
                            DoCast(summoner, SPELL_GRAB); 
            }

            void UpdateAI(uint32 diff)
            {
                VehicleAI::UpdateAI(diff);
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    if (eventId == EVENT_FLY_AWAY)
                    {
                        Position randomPosOnRadius;
                        randomPosOnRadius.m_positionZ = (me->GetPositionZ() + 40.0f);
                        me->GetNearPoint2D(randomPosOnRadius.m_positionX, randomPosOnRadius.m_positionY, 40.0f, me->GetAngle(me));
                        me->GetMotionMaster()->MovePoint(POINT_FLY_AWAY, randomPosOnRadius);
                    }
                }
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_EXPLOSION:
                        DoCast(me, SPELL_IMMOLATION);
                        break;
                    case SPELL_RIDE:
                        DoCastAOE(SPELL_PING_BUNNY);
                        events.ScheduleEvent(EVENT_FLY_AWAY, 100);
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frostbrood_skytalonAI(creature);
        }
};

class spell_tournament_charge : public SpellScriptLoader
{
    public:
        spell_tournament_charge() : SpellScriptLoader("spell_tournament_charge") { }

        class spell_tournament_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tournament_charge_SpellScript);

            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Unit *caster = GetCaster())
                    {
                        caster->CastSpell(target, 74399, true);
                        caster->CastSpell(target, 68321, true);

                        if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->GetEntry() == 33272)
                        {
                            //Kill Credit
                            if (Unit *player = caster->GetCharmerOrOwner())
                                player->CastSpell(player, 62658, true);
                        }
                    }

                    if (target->GetAura(64100))
                        target->RemoveAuraFromStack(64100);
                    else if (target->GetAura(62552))
                        target->RemoveAuraFromStack(62552);
                    else if (target->GetAura(62719))
                        target->RemoveAuraFromStack(62719);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_tournament_charge_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_tournament_charge_SpellScript();
        }
};

class spell_tournament_shield_breaker : public SpellScriptLoader
{
    public:
        spell_tournament_shield_breaker() : SpellScriptLoader("spell_tournament_shield_breaker") { }

        class spell_tournament_shield_breaker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tournament_shield_breaker_SpellScript);

            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Unit *caster = GetCaster())
                    {
                        caster->CastSpell(target, 62626, true );

                        if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->GetEntry() == 33243)
                        {
                            //Kill Credit
                            if (Unit *player = caster->GetCharmerOrOwner())
                                player->CastSpell(player, 62673, true);
                        }
                    }

                    if (target->GetAura(64100))
                        target->RemoveAuraFromStack(64100);
                    else if (target->GetAura(62552))
                        target->RemoveAuraFromStack(62552);
                    else if (target->GetAura(62719))
                        target->RemoveAuraFromStack(62719);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_tournament_shield_breaker_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_tournament_shield_breaker_SpellScript();
        }
};

class spell_tournament_trust : public SpellScriptLoader
{
    public:
        spell_tournament_trust() : SpellScriptLoader("spell_tournament_trust") { }

        class spell_tournament_trust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tournament_trust_SpellScript);

            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (Unit *caster = GetCaster())
                    {
                        if (target->GetTypeId() == TYPEID_UNIT && target->ToCreature()->GetEntry() == 33229)
                        {
                            //Kill Credit
                            if (Unit *player = caster->GetCharmerOrOwner())
                                player->CastSpell(player, 62672, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_tournament_trust_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_tournament_trust_SpellScript();
        }
};

class spell_tournament_duel : public SpellScriptLoader
{
    public:
        spell_tournament_duel() : SpellScriptLoader("spell_tournament_duel") { }

        class spell_tournament_duel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tournament_duel_SpellScript);

            void HandleEffectScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    if (target->GetTypeId() != TYPEID_PLAYER)
                        return;

                    if (Unit *caster = GetCaster()->GetCharmerOrOwner())
                        caster->CastSpell(target, 62875, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_tournament_duel_SpellScript::HandleEffectScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_tournament_duel_SpellScript();
        }
};/*######
## The Flesh Giant Champion - Id: 13235
######*/
enum FleshGiant
{
    QUEST_FLESH_GIANT_CHAMPION = 13235,

    NPC_MORBIDUS = 30698,
    NPC_LICH_KING = 31301,
    NPC_OLAKIN = 31428,
    NPC_DHAKAR = 31306,

    FACTION_HOSTILE = 14,
    FACTION_BASIC = 2102,

    EVENT_INTRO = 1,
    EVENT_LK_SAY_1 = 2,
    EVENT_LK_SAY_2 = 3,
    EVENT_LK_SAY_3 = 4,
    EVENT_LK_SAY_4 = 5,
    EVENT_LK_SAY_5 = 6,
    EVENT_OUTRO = 7,
    EVENT_START = 8,

    SPELL_SIMPLE_TELEPORT = 64195,

    SAY_DHAKAR_START = 0,
    SAY_LK_1 = 0,
    SAY_LK_2 = 1,
    SAY_LK_3 = 2,
    SAY_LK_4 = 3,
    SAY_LK_5 = 4,
    SAY_OLAKIN_PAY = 0
};

class npc_margrave_dhakar : public CreatureScript
{
    public:
        npc_margrave_dhakar() : CreatureScript("npc_margrave_dhakar") { }

        struct npc_margrave_dhakarAI : public ScriptedAI
        {
            npc_margrave_dhakarAI(Creature* creature) : ScriptedAI(creature) , _summons(me), _lichKingGuid(0) { }

            void Reset()
            {
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);

                _events.Reset();
                _summons.DespawnAll();
            }

            void sGossipSelect(Player* player, uint32 sender, uint32 action)
            {
                if (player->GetQuestStatus(QUEST_FLESH_GIANT_CHAMPION) == QUEST_STATUS_INCOMPLETE && !player->isInCombat())
                {
                    if (me->GetCreatureTemplate()->GossipMenuId == sender && !action)
                    {
                        _events.ScheduleEvent(EVENT_INTRO, 1000);
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    }
                }
            }

            void UpdateAI(uint32 diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO:
                        {
                            Talk(SAY_DHAKAR_START);
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);

                            if (Creature* morbidus = me->FindNearestCreature(NPC_MORBIDUS, 50.0f, true))
                            {
                                if (Creature* lichKing = me->SummonCreature(NPC_LICH_KING, morbidus->GetPositionX() + 10.0f, morbidus->GetPositionY(), morbidus->GetPositionZ()))
                                {
                                    _lichKingGuid = lichKing->GetGUID();
                                    lichKing->SetFacingTo(morbidus->GetOrientation());
                                    lichKing->CastSpell(lichKing, SPELL_SIMPLE_TELEPORT, true);
                                }
                            }

                            _events.ScheduleEvent(EVENT_LK_SAY_1, 5000);
                            break;
                        }
                        case EVENT_LK_SAY_1:
                        {
                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->AI()->Talk(SAY_LK_1);
                            _events.ScheduleEvent(EVENT_LK_SAY_2, 5000);
                            break;
                        }
                        case EVENT_LK_SAY_2:
                        {
                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->AI()->Talk(SAY_LK_2);
                            _events.ScheduleEvent(EVENT_LK_SAY_3, 5000);
                            break;
                        }
                        case EVENT_LK_SAY_3:
                        {
                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->AI()->Talk(SAY_LK_3);
                            _events.ScheduleEvent(EVENT_LK_SAY_4, 5000);
                            break;
                        }
                        case EVENT_LK_SAY_4:
                        {
                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->AI()->Talk(SAY_LK_4);
                            _events.ScheduleEvent(EVENT_OUTRO, 12000);
                            break;
                        }
                        case EVENT_LK_SAY_5:
                        {
                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->AI()->Talk(SAY_LK_5);
                            _events.ScheduleEvent(EVENT_OUTRO, 8000);
                            break;
                        }
                        case EVENT_OUTRO:
                        {
                            if (Creature* olakin = me->FindNearestCreature(NPC_OLAKIN, 50.0f, true))
                                olakin->AI()->Talk(SAY_OLAKIN_PAY);

                            if (Creature* lichKing = Unit::GetCreature(*me, _lichKingGuid))
                                lichKing->DespawnOrUnsummon(0);

                            _events.ScheduleEvent(EVENT_START, 5000);
                            break;
                        }
                        case EVENT_START:
                        {
                            if (Creature* morbidus = me->FindNearestCreature(NPC_MORBIDUS, 50.0f, true))
                            {
                                morbidus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_DISABLE_MOVE);
                                morbidus->setFaction(FACTION_HOSTILE);
                            }

                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
            SummonList _summons;
            uint64 _lichKingGuid;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_margrave_dhakarAI(creature);
    }
};

class npc_morbidus : public CreatureScript
{
    public:
        npc_morbidus() : CreatureScript("npc_morbidus") { }

        struct npc_morbidusAI : public ScriptedAI
        {
            npc_morbidusAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                if (Creature* dhakar = me->FindNearestCreature(NPC_DHAKAR, 50.0f, true))
                    dhakar->AI()->Reset();

                // this will prevent the event to start without morbidus being alive
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                me->setFaction(FACTION_BASIC);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_morbidusAI(creature);
        }
};

void AddSC_icecrown()

{
    new npc_arete;
    new npc_guardian_pavilion;
    new npc_argent_squire_gruntling();
    new npc_vereth_the_cunning;
    new npc_tournament_training_dummy;
    new npc_alorah_and_grimmin();
    new spell_flaming_spear_targeting();
    new vehicle_black_knights_gryphon();
    new npc_black_knights_grave();
    new npc_maiden_of_drakmar();
    new npc_the_black_knight();
    new npc_slain_tualiq_villager();
    new npc_squire_cavin();
    new spell_tournament_defend();
    new npc_vendor_tournament_fraction_champion();
    new npc_faction_valiant_champion();
    new npc_general_lightsbane();
    new npc_free_your_mind();
    new npc_saronite_mine_slave();
    new npc_blessed_banner();
    new npc_webbed_crusader();
    new npc_captured_crusader();
    new npc_the_ocular();
    new spell_argent_cannon();
    new npc_frostbrood_skytalon();
    new npc_dame_evniki_kapsalis();
    new spell_tournament_charge;
    new spell_tournament_shield_breaker;
    new spell_tournament_trust;
    new spell_tournament_duel;
    new npc_margrave_dhakar();
    new npc_morbidus();
}