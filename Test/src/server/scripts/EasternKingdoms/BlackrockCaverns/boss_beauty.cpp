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
#include "blackrock_caverns.h"
#include "Unit.h"



#define spell_magma 76031
#define spell_cassure 76032
#define spell_cassure_H 93583

#define spell_charge 74367
#define spell_charge_H 93580

#define spell_fear 76028
#define spell_fear_H 93586

#define spell_berserk 82395

#define SAY_AGGRO "!!!"

#define SAY_DIED "..."



class boss_beauty: public CreatureScript

{

public:
   
 boss_beauty() : CreatureScript("boss_beauty") { }
    struct boss_beautyAI : public ScriptedAI

    {
	public :
        boss_beautyAI(Creature *c) : ScriptedAI(c) {}


	uint32 charge;

        uint32 magma;

        uint32 cassure;

	uint32 fear;

        uint32 Berserk_Timer;


        void Reset()

        {

           magma = 10000;

           cassure = 12000;

	   charge = 8000;

	   fear = 15000;
		   
	   Berserk_Timer = 300000; //5 minutes
        }
        void EnterCombat(Unit* /*who*/)
 
       {

	    me->MonsterYell(SAY_AGGRO, LANG_UNIVERSAL, NULL);

        }
        void JustDied(Unit* /*killer*/)

        {
			
	    me->MonsterYell(SAY_DIED, LANG_UNIVERSAL, NULL);

        }


        void UpdateAI(const uint32 diff)

        {

            if (!UpdateVictim())

                return;


            if (magma<= diff)

            {

                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))

                    DoCast(pTarget, spell_magma);

                magma   = 10000;
 
           }

            else

                magma -= diff;
 
           
if (cassure <= diff)

            {

                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))

                    DoCast(pTarget, IsHeroic() ? spell_cassure_H : spell_cassure);

                cassure = 12000;

            }

            else

                cassure -= diff;

	    if (charge<= diff)

            {

                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))

                    DoCast(pTarget, IsHeroic() ? spell_charge_H : spell_charge);

                charge   = 8000;
 
           }

            else

                charge -= diff;
 
	    if (fear<= diff)

            {

                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))

                    DoCast(pTarget, IsHeroic() ? spell_fear_H : spell_fear);

                fear   = 15000;
 
           }

            else

                fear -= diff;
 

	    if (!UpdateVictim())

                    return;

	    if (Berserk_Timer <= diff)

            {

                DoCast(me->getVictim(), spell_berserk, true);


                Berserk_Timer = 300000;

            } 
	    else Berserk_Timer -= diff;

	    DoMeleeAttackIfReady();

        }

    };


    CreatureAI* GetAI(Creature* pCreature) const

    {

        return new boss_beautyAI (pCreature);

    }


};


void AddSC_boss_beauty() 

{

    new boss_beauty();

}