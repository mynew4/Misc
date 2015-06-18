/*
* Copyright (C) 2011-2012 ProjectStudioMirage <http://www.studio-mirage.fr/>
* Copyright (C) 2011-2012 https://github.com/Asardial
*/

#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "the_lost_city_of_the_tol_vir.h"
#include "UnitAI.h"

/*
NPC 44711 - Spawned before shockwave - targets.
NPC 44712 - Shockwave NPC.
*/

enum Spells
{
    SPELL_BAD_INTENTIONS = 83113,
    SPELL_SHOCKWAVE = 83445,
    SPELL_HAMMER_FIST = 83655,
	
    SPELL_MYSTIC_TRAP_PLANT = 83646,
    SPELL_MYSTIC_TRAP_EXPLODE = 83171,
    SPELL_DETONATE_TRAPS = 91263,
    SPELL_MYSTIC_TRAP_EXPLODE_EFFECT = 83110,
	
    SPELL_SHOCKWAVE_SUMMON_EFFECT = 83128,
    SPELL_SHOCKWAVE_VISUAL_TRIGGER = 83127,
    SPELL_SHOCKWAVE_TARGET_EST = 83133,
    SPELL_SHOCKWAVE_TARGET_OUEST = 83134,
    SPELL_SHOCKWAVE_TARGET_NORD = 83131,
    SPELL_SHOCKWAVE_TARGET_SUD = 83132,
    SPELL_SHOWKWAVE_VISUAL_DAMAGE = 83130,
};

enum Yells
{
    SAY_AGGRO = -1755001,
    SAY_SHOCKWAVE = -1755002,
    SAY_DETONATE = -1755003,
    SAY_DEATH = -1755004,
};

enum Actions
{
    ACTION_DETONATE
};

enum Creatures
{
	CREATURE_HUSAM = 44577,
    CREATURE_LAND_MINE = 44840,
};

/******************
**  General Hussam
*******************/
class boss_general_husam : public CreatureScript
{
public:
    boss_general_husam() : CreatureScript("boss_general_husam") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_general_husamAI (pCreature);
    }

    struct boss_general_husamAI : public ScriptedAI
    {
        InstanceScript* m_pInstance;
        
        boss_general_husamAI(Creature *c) : ScriptedAI(c)
        {
            m_pInstance = c->GetInstanceScript();
        }

        uint32 BadIntentionsTimer;
        uint32 ShockwaveTimer;
        uint32 HammerFistTimer;
        uint32 PlantTimer;
        uint32 DetonateTimer;

        void JustDied(Unit* /*Killer*/)
        {
            Talk(SAY_DEATH);
        }

        void Reset()
        {
            BadIntentionsTimer = 8000;
            ShockwaveTimer = 18000;
            HammerFistTimer = 5000;
            PlantTimer = 3000;
            DetonateTimer = 23000;
        }

        void EnterCombat(Unit* pWho)
        {
            Talk(SAY_AGGRO);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if(BadIntentionsTimer <= diff)
            {
                if(Unit* pTar = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTar,SPELL_BAD_INTENTIONS);
                BadIntentionsTimer = 15000;
            }
            else
                BadIntentionsTimer -= diff;

            if(ShockwaveTimer <= diff)
            {
                Creature* Target1 = me->SummonCreature(44711, me->GetPositionX()+10,me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                Creature* Target2 = me->SummonCreature(44711, me->GetPositionX()-10,me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                Creature* Target3 = me->SummonCreature(44711, me->GetPositionX(),me->GetPositionY()+10,me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                Creature* Target4 = me->SummonCreature(44711, me->GetPositionX(),me->GetPositionY()-10,me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000);
                Talk(SAY_SHOCKWAVE);
                DoCastAOE(SPELL_SHOCKWAVE);
                DoCast(Target1, 83130);
                DoCast(Target2, 83130);
                DoCast(Target3, 83130);
                DoCast(Target4, 83130);
                if (Creature* Shockwave1 = me->SummonCreature(44712, me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000))
                    Shockwave1->GetMotionMaster()->MoveChase(Target1);
                if (Creature* Shockwave2 = me->SummonCreature(44712, me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000))
                    Shockwave2->GetMotionMaster()->MoveChase(Target2);
                if (Creature* Shockwave3 = me->SummonCreature(44712, me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000))
                    Shockwave3->GetMotionMaster()->MoveChase(Target3);
                if (Creature* Shockwave4 = me->SummonCreature(44712, me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000))
                    Shockwave4->GetMotionMaster()->MoveChase(Target4);
                ShockwaveTimer = urand(20000,40000);
            }
            else
                ShockwaveTimer -= diff;

            if (HammerFistTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_HAMMER_FIST);
                HammerFistTimer = 12000;
            }
            else
                HammerFistTimer -= diff;

            if (PlantTimer <= diff)
            {
                if(Unit* pTar = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    pTar->CastSpell(pTar, SPELL_MYSTIC_TRAP_PLANT, true);
                PlantTimer = 3000;
            }
            else
                PlantTimer -= diff;

            if (DetonateTimer <= diff && IsHeroic())
            {
                DoCast(me, SPELL_DETONATE_TRAPS);
                Talk(SAY_DETONATE);
                DetonateTimer = 20000;
            }
            else
                DetonateTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/****************
** Mobs Shockwave
*****************/
class mob_shokwave_spike : public CreatureScript
{
public:
    mob_shokwave_spike() : CreatureScript("mob_shokwave_spike") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_shokwave_spikeAI (pCreature);
    }

    struct mob_shokwave_spikeAI : public ScriptedAI
    {
        mob_shokwave_spikeAI(Creature *c) : ScriptedAI(c)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            DoCast(me, SPELL_SHOWKWAVE_VISUAL_DAMAGE);
            DoCast(me, SPELL_SHOCKWAVE);
            me->DespawnOrUnsummon(10000);
        }
    };
};

/***********************
** Mobs Shockwave Visual
************************/
class mob_shokwave_spike_visual : public CreatureScript
{
public:
    mob_shokwave_spike_visual() : CreatureScript("mob_shokwave_spike_visual") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_shokwave_spike_visualAI (pCreature);
    }

    struct mob_shokwave_spike_visualAI : public ScriptedAI
    {
        mob_shokwave_spike_visualAI(Creature *c) : ScriptedAI(c)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            me->AddAura(SPELL_SHOCKWAVE_VISUAL_TRIGGER, me);
            me->DespawnOrUnsummon(20000);
        }
    };
};

/************
**  Mine Trap
*************/
class npc_trap : public CreatureScript
{
public:
    npc_trap() : CreatureScript("npc_trap") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_trapAI(pCreature);
    }

    struct npc_trapAI : public ScriptedAI
    {
        npc_trapAI(Creature *pCreature) : ScriptedAI(pCreature) 
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            DoCastAOE(SPELL_MYSTIC_TRAP_EXPLODE);
            me->AddAura(SPELL_MYSTIC_TRAP_EXPLODE_EFFECT, me);
        }
    };
};

void AddSC_boss_ussam()
{
    new boss_general_husam();
    new npc_trap();
    new mob_shokwave_spike();
    new mob_shokwave_spike_visual();
}