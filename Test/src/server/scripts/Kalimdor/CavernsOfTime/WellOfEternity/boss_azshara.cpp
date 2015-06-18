/*
 * Copyright (C) 2005 - 2011 MaNGOS <http://www.getmangos.org/>
 *
 * Copyright (C) 2008 - 2011 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2011 TrilliumEMU <http://www.arkania.net/>
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
#include "well_of_eternity.h"


enum Spells

{
SP_FROSTBOLT				= 28478,
SP_FIREBALL				= 41484,
SP_SHADOWBOLT				= 56405,
SP_FROSTBOLT_VOLLEY     		= 55807,
SP_SHADOWBOLT_VOLLEY    		= 55638,
SP_POISON				= 64152,
SP_DISEASE				= 64153,
SP_MAGIC				= 64156,
SP_HEAL					= 62892,
SP_BARRIER				= 36815,
SP_SHADOW_BLAST				= 41078,
SP_SOFT_ENRAGE				= 64473
};

class boss_azshara : public CreatureScript
{
public:
    boss_azshara() : CreatureScript("boss_azshara") { }
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_azsharaAI (creature);
    }
	struct boss_azsharaAI : public ScriptedAI
	{
		boss_azsharaAI(Creature *Creature) : ScriptedAI(Creature)
		{
			Reset();
		}
		uint32 ShieldThreshold;
		uint32 SoftEnrageTimer;
		uint32 GCDTimer;
		double SoftEnrageStack;
		void Reset()
		{
			GCDTimer = 2000;
			SoftEnrageTimer = 15 * 60000; //15 minutes
			SoftEnrageStack = SoftEnrageTimer;
			ShieldThreshold = 19;
		}
		void Aggro(Unit *who)
		{
			if(who->GetTypeId() == TYPEID_PLAYER)
			{
				if( ((Player *) who)->GetTeam() == ALLIANCE)
					me->MonsterTextEmote(-1037000, NULL);
				else
					me->MonsterTextEmote(-1037001, NULL);
			};
			DoCast(SP_HEAL);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!me->SelectNearestTarget() || !me->getVictim())
				return;
			if(me->GetPositionZ() > 220.0f)
				EnterEvadeMode();
			if(SoftEnrageTimer < diff)
			{
				DoCastVictim(SP_SOFT_ENRAGE);
				SoftEnrageStack *= 0.35f;
				SoftEnrageTimer = (SoftEnrageStack > 1500.0f) ? SoftEnrageStack : 1500;
			}
			else SoftEnrageTimer -= diff;
			if(me->GetHealth() < me->GetMaxHealth() * ShieldThreshold / 20)
			{
				--ShieldThreshold;
				DoCast(SP_BARRIER);
			}  
			if(GCDTimer < diff)
			{
				if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
				{
					switch(urand(0,8))
					{
						case 0: DoCastVictim(SP_SHADOW_BLAST); break;
						case 1: DoCastVictim(SP_FROSTBOLT); break;
						case 2: DoCastVictim(SP_SHADOWBOLT); break;
						case 3: DoCastVictim(SP_FIREBALL); break;
						case 4: DoCastVictim(SP_POISON); break;
						case 5: DoCastVictim(SP_MAGIC); break;
						case 6: DoCastVictim(SP_DISEASE); break;
						case 7: DoCastVictim(SP_FROSTBOLT_VOLLEY); break;
						case 8: DoCastVictim(SP_SHADOWBOLT_VOLLEY); break;
					}
				}
				GCDTimer = 2000;
			}
			else GCDTimer -= diff;
			DoMeleeAttackIfReady();
		}
	};
};

void AddSC_boss_azshara()
{
    new boss_azshara;
}