/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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
#include "halls_of_reflection.h"
#include "ScriptedEscortAI.h"
#include "MapManager.h"
#include "Transport.h"

enum Yells
{
	SAY_JAINA_INTRO_1                             = -1668001,
	SAY_JAINA_INTRO_2                             = -1668002,
	SAY_JAINA_INTRO_3                             = -1668003,
	SAY_JAINA_INTRO_4                             = -1668004,
	SAY_UTHER_INTRO_A2_1                          = -1668005,
	SAY_JAINA_INTRO_5                             = -1668006,
	SAY_UTHER_INTRO_A2_2                          = -1668007,
	SAY_JAINA_INTRO_6                             = -1668008,
	SAY_UTHER_INTRO_A2_3                          = -1668009,
	SAY_JAINA_INTRO_7                             = -1668010,
	SAY_UTHER_INTRO_A2_4                          = -1668011,
	SAY_JAINA_INTRO_8                             = -1668012,
	SAY_UTHER_INTRO_A2_5                          = -1668013,
	SAY_JAINA_INTRO_9                             = -1668014,
	SAY_UTHER_INTRO_A2_6                          = -1668015,
	SAY_UTHER_INTRO_A2_7                          = -1668016,
	SAY_JAINA_INTRO_10                            = -1668017,
	SAY_UTHER_INTRO_A2_8                          = -1668018,
	SAY_JAINA_INTRO_11                            = -1668019,
	SAY_UTHER_INTRO_A2_9                          = -1668020,

	SAY_SYLVANAS_INTRO_1                          = -1668021,
	SAY_SYLVANAS_INTRO_2                          = -1668022,
	SAY_SYLVANAS_INTRO_3                          = -1668023,
	SAY_UTHER_INTRO_H2_1                          = -1668024,
	SAY_SYLVANAS_INTRO_4                          = -1668025,
	SAY_UTHER_INTRO_H2_2                          = -1668026,
	SAY_SYLVANAS_INTRO_5                          = -1668027,
	SAY_UTHER_INTRO_H2_3                          = -1668028,
	SAY_SYLVANAS_INTRO_6                          = -1668029,
	SAY_UTHER_INTRO_H2_4                          = -1668030,
	SAY_SYLVANAS_INTRO_7                          = -1668031,
	SAY_UTHER_INTRO_H2_5                          = -1668032,
	SAY_UTHER_INTRO_H2_6                          = -1668033,
	SAY_SYLVANAS_INTRO_8                          = -1668034,
	SAY_UTHER_INTRO_H2_7                          = -1668035,

	SAY_LK_INTRO_1                                = -1668036,
	SAY_LK_INTRO_2                                = -1668037,
	SAY_LK_INTRO_3                                = -1668038,
	SAY_FALRIC_INTRO_1                            = -1668039,
	SAY_MARWYN_INTRO_1                            = -1668040,
	SAY_FALRIC_INTRO_2                            = -1668041,

	SAY_JAINA_INTRO_END                           = -1668042,
	SAY_SYLVANAS_INTRO_END                        = -1668043,
	SAY_LK_JAINA_INTRO_END                        = -1594473,
	SAY_LK_SYLVANAS_INTRO_END                     = -1594474,
	
};

enum Events
{
	EVENT_NONE,

	EVENT_START_PREINTRO,
	EVENT_PREINTRO_1_A,
	EVENT_PREINTRO_1_H,
	EVENT_PREINTRO_2,

	EVENT_START_INTRO,
	EVENT_SKIP_INTRO,

	EVENT_INTRO_A2_1,
	EVENT_INTRO_A2_2,
	EVENT_INTRO_A2_3,
	EVENT_INTRO_A2_4,
	EVENT_INTRO_A2_5,
	EVENT_INTRO_A2_6,
	EVENT_INTRO_A2_7,
	EVENT_INTRO_A2_8,
	EVENT_INTRO_A2_9,
	EVENT_INTRO_A2_10,
	EVENT_INTRO_A2_11,
	EVENT_INTRO_A2_12,
	EVENT_INTRO_A2_13,
	EVENT_INTRO_A2_14,
	EVENT_INTRO_A2_15,
	EVENT_INTRO_A2_16,
	EVENT_INTRO_A2_17,
	EVENT_INTRO_A2_18,
	EVENT_INTRO_A2_19,

	EVENT_INTRO_H2_1,
	EVENT_INTRO_H2_2,
	EVENT_INTRO_H2_3,
	EVENT_INTRO_H2_4,
	EVENT_INTRO_H2_5,
	EVENT_INTRO_H2_6,
	EVENT_INTRO_H2_7,
	EVENT_INTRO_H2_8,
	EVENT_INTRO_H2_9,
	EVENT_INTRO_H2_10,
	EVENT_INTRO_H2_11,
	EVENT_INTRO_H2_12,
	EVENT_INTRO_H2_13,
	EVENT_INTRO_H2_14,
	EVENT_INTRO_H2_15,

	EVENT_INTRO_LK_1,
	EVENT_INTRO_LK_2,
	EVENT_INTRO_LK_3,
	EVENT_INTRO_LK_4,
	EVENT_INTRO_LK_5,
	EVENT_INTRO_LK_6,
	EVENT_INTRO_LK_7,
	EVENT_INTRO_LK_8,
	EVENT_INTRO_LK_9,
	EVENT_INTRO_LK_10,

	EVENT_INTRO_END,
	
	EVENT_OUTRO_1,
	EVENT_OUTRO_2,
	EVENT_OUTRO_3,
	EVENT_OUTRO_4,
	
	EVENT_QUEST_QUEL_1,
	EVENT_QUEST_QUEL_2,
	EVENT_QUEST_QUEL_3,
	EVENT_QUEST_QUEL_4,
	EVENT_QUEST_QUEL_5,
	EVENT_QUEST_QUEL_6,
	EVENT_QUEST_QUEL_7,
	EVENT_QUEST_QUEL_8,
	EVENT_QUEST_QUEL_9,
	EVENT_QUEST_QUEL_10,
	EVENT_QUEST_QUEL_11,
	EVENT_WHIRLWIND,
	EVENT_BLADESTORM,
	EVENT_MORTAL_STRIKE,
	EVENT_HEROIC_STRIKE,
	EVENT_ATTACK_START,
};

enum eEnum
{
	ACTION_START_PREINTRO,
	ACTION_START_INTRO,
	ACTION_SKIP_INTRO,
	ACTION_START_FLY,

	QUEST_DELIVRANCE_FROM_THE_PIT_A2              = 24710,
	QUEST_DELIVRANCE_FROM_THE_PIT_H2              = 24712,
	QUEST_WRATH_OF_THE_LICH_KING_A2               = 24500,
	QUEST_WRATH_OF_THE_LICH_KING_H2               = 24802,
};

enum Spells
{
	SPELL_CAST_VISUAL                  = 65633, //Jaina/Sylavana lo lanzan para invocar a uther
	SPELL_BOSS_SPAWN_AURA              = 72712, //Falric and Marwyn
	SPELL_UTHER_DESPAWN                = 70693,
	SPELL_TAKE_FROSTMOURNE             = 72729,
	SPELL_FROSTMOURNE_DESPAWN          = 72726,
	SPELL_FROSTMOURNE_VISUAL           = 73220,
	SPELL_FROSTMOURNE_SOUNDS           = 70667,
};

const Position HallsofReflectionLocs[]=
{
	{5283.234863f, 1990.946777f, 707.695679f, 0.929097f},   // 2 Loralen Follows
	{5408.031250f, 2102.918213f, 707.695251f, 0.792756f},   // 9 Sylvanas Follows
	{5401.866699f, 2110.837402f, 707.695251f, 0.800610f},   // 10 Loralen follows
};

const Position SpawnPos              = {5262.540527f, 1949.693726f, 707.695007f, 0.808736f}; // Jaina/Sylvanas Beginning Position
const Position MoveDoorPos           = {5268.254395f, 1955.522705f, 707.699585f, 1.277278f};
const Position MoveThronePos         = {5306.952148f, 1998.499023f, 709.341431f, 1.277278f}; // Jaina/Sylvanas walks to throne
const Position UtherSpawnPos         = {5308.310059f, 2003.857178f, 709.341431f, 4.650315f};
const Position LichKingSpawnPos      = {5362.917480f, 2062.307129f, 707.695374f, 3.945812f};
const Position LichKingMoveThronePos = {5312.080566f, 2009.172119f, 709.341431f, 3.973301f}; // Lich King walks to throne
const Position LichKingMoveAwayPos   = {5400.069824f, 2102.7131689f, 707.69525f, 0.843803f}; // Lich King walks away


//Function start motion of the ship
void StartFly(Transport* t)
{
	t->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
	t->SetGoState(GO_STATE_ACTIVE);
	t->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0x10830010); // Seen in sniffs
	t->SetFloatValue(GAMEOBJECT_PARENTROTATION + 3, 1.0f);

	Map* map = t->GetMap();
	std::set<uint32> mapsUsed;
	GameObjectTemplate const* goinfo = t->GetGOInfo();

	t->GenerateWaypoints(goinfo->moTransport.taxiPathId, mapsUsed);

	for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
	{
		if (Player* pPlayer = itr->getSource())
		{
			UpdateData transData;
			t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
			WorldPacket packet;
			transData.BuildPacket(&packet);
			pPlayer->SendDirectMessage(&packet);
		}
	}
}

//Function stop motion of the ship
void StopFly(Transport* t)
{
	Map* map = t->GetMap();
	t->m_WayPoints.clear();
	//RelocateTransport(t);
	t->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
	t->SetGoState(GO_STATE_READY);

	for (Map::PlayerList::const_iterator itr = map->GetPlayers().begin(); itr != map->GetPlayers().end(); ++itr)
	{
		if (Player* pPlayer = itr->getSource())
		{
			UpdateData transData;
			t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
			WorldPacket packet;
			transData.BuildPacket(&packet);
			pPlayer->SendDirectMessage(&packet);
		}
	}
}


class npc_jaina_or_sylvanas_hor : public CreatureScript
{
private:
	bool m_isSylvana;

public:
	npc_jaina_or_sylvanas_hor(bool isSylvana, const char* name) : CreatureScript(name), m_isSylvana(isSylvana) { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (creature->isQuestGiver())
			player->PrepareQuestMenu(creature->GetGUID());

			player->ADD_GOSSIP_ITEM( 0, "Can you remove the sworld?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
	{
		player->PlayerTalkClass->ClearMenus();
		switch (uiAction)
		{
			case GOSSIP_ACTION_INFO_DEF+1:
				if (creature->AI())
					creature->AI()->DoAction(ACTION_START_INTRO);
				creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
				creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				player->CLOSE_GOSSIP_MENU();
				break;
		}

		return true;
	}



	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_jaina_or_sylvanas_horAI(creature);
	}

	// AI of Part1: handle the intro till start of gauntlet event.
	struct npc_jaina_or_sylvanas_horAI : public ScriptedAI
	{
		npc_jaina_or_sylvanas_horAI(Creature* creature) : ScriptedAI(creature)
		{
			pInstance = me->GetInstanceScript();
		}

		InstanceScript* pInstance;
		uint64 uiUther;
		uint64 uiLichKing;
		bool start;
		EventMap events;

		void Reset()
		{
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
			events.Reset();

			uiUther = 0;
			uiLichKing = 0;                
			me->SetStandState(UNIT_STAND_STATE_STAND);
			me->SetVisible(true);
			start = false;
		}

		void DoAction(const int32 actionId)
		{
			switch(actionId)
			{
				case ACTION_START_PREINTRO:
					events.ScheduleEvent(EVENT_START_PREINTRO, 3000);
					break;
				case ACTION_START_INTRO:
					events.ScheduleEvent(EVENT_START_INTRO, 1000);
					break;
			}
		}

		void MoveInLineOfSight(Unit* who)
		{
			if (!pInstance)
				return;

			if(start)
				return;

			if (who->GetTypeId() != TYPEID_PLAYER)
				return;

			me->AI()->DoAction(ACTION_START_PREINTRO);
			start = true;
		}

		void UpdateAI(const uint32 diff)
		{
			events.Update(diff);
			switch(events.ExecuteEvent())
			{
				case EVENT_START_PREINTRO:
					me->SetWalk(false);                    
					me->GetMotionMaster()->MovePoint(0, MoveDoorPos);
					
					if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
						events.ScheduleEvent(EVENT_PREINTRO_1_A, 0);
					else
						events.ScheduleEvent(EVENT_PREINTRO_1_H, 0);
					break;

				case EVENT_PREINTRO_1_A:
						DoScriptText(SAY_JAINA_INTRO_1, me);
						events.ScheduleEvent(EVENT_PREINTRO_2, 6000);
					break;

				case EVENT_PREINTRO_1_H:
						events.ScheduleEvent(EVENT_INTRO_H2_1, 8000);
					break;

				case EVENT_INTRO_H2_1:
						DoScriptText(SAY_SYLVANAS_INTRO_1, me);
						me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					break;

				case EVENT_PREINTRO_2:
						DoScriptText(SAY_JAINA_INTRO_2, me);
						me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
						me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					break;

				case EVENT_START_INTRO:
					me->SetWalk(false);                    
					me->GetMotionMaster()->MovePoint(0, MoveThronePos);
					// Begining of intro is differents between factions as the speech sequence and timers are differents.
					if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
						events.ScheduleEvent(EVENT_INTRO_A2_1, 0);
					else
						events.ScheduleEvent(EVENT_INTRO_H2_2, 0);
					break;

			// A2 Intro Events
				case EVENT_INTRO_A2_1:               
					DoScriptText(SAY_JAINA_INTRO_3, me);
					events.ScheduleEvent(EVENT_INTRO_A2_2, 5000);
					break;
				case EVENT_INTRO_A2_2:
					DoScriptText(SAY_JAINA_INTRO_4, me);
					events.ScheduleEvent(EVENT_INTRO_A2_3, 10000);
					break;
				case EVENT_INTRO_A2_3:
					DoCast(me, SPELL_CAST_VISUAL);
					me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
					pInstance->HandleGameObject(pInstance->GetData64(DATA_FROSTMOURNE), true);
					events.ScheduleEvent(EVENT_INTRO_A2_4, 10000);
					break;
				case EVENT_INTRO_A2_4:
					// spawn UTHER during speach 2
					if (Creature* pUther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
					{
						pUther->GetMotionMaster()->MoveIdle();
						pUther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
						uiUther = pUther->GetGUID();
						me->SetUInt64Value(UNIT_FIELD_TARGET, uiUther);
						pUther->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
					}
					events.ScheduleEvent(EVENT_INTRO_A2_5, 2000);
					break;
				case EVENT_INTRO_A2_5:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_1, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_6, 3000);
					break;
				case EVENT_INTRO_A2_6:
					DoScriptText(SAY_JAINA_INTRO_5, me);
					events.ScheduleEvent(EVENT_INTRO_A2_7, 6000);
					break;
				case EVENT_INTRO_A2_7:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_2, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_8, 6500);
					break;
				case EVENT_INTRO_A2_8:
					DoScriptText(SAY_JAINA_INTRO_6, me);
					events.ScheduleEvent(EVENT_INTRO_A2_9, 2000);
					break;
				case EVENT_INTRO_A2_9:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_3, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_10, 9000);
					break;
				case EVENT_INTRO_A2_10:
					DoScriptText(SAY_JAINA_INTRO_7, me);
					events.ScheduleEvent(EVENT_INTRO_A2_11, 5000);
					break;
				case EVENT_INTRO_A2_11:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_4, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_12, 11000);
					break;
				case EVENT_INTRO_A2_12:
					DoScriptText(SAY_JAINA_INTRO_8, me);
					events.ScheduleEvent(EVENT_INTRO_A2_13, 4000);
					break;
				case EVENT_INTRO_A2_13:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_5, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_14, 12500);
					break;
				case EVENT_INTRO_A2_14:
					DoScriptText(SAY_JAINA_INTRO_9, me);
					events.ScheduleEvent(EVENT_INTRO_A2_15, 10000);
					break;
				case EVENT_INTRO_A2_15:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_6, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_16, 22000);
					break;
				case EVENT_INTRO_A2_16:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_A2_7, pUther);
					events.ScheduleEvent(EVENT_INTRO_A2_17, 4000);
					break;
				case EVENT_INTRO_A2_17:
					DoScriptText(SAY_JAINA_INTRO_10, me);
					events.ScheduleEvent(EVENT_INTRO_A2_18, 2000);
					break;
				case EVENT_INTRO_A2_18:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
					{
						pUther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
						DoScriptText(SAY_UTHER_INTRO_A2_8, pUther);
					}
					events.ScheduleEvent(EVENT_INTRO_A2_19, 11000);
					break;
				case EVENT_INTRO_A2_19:
					DoScriptText(SAY_JAINA_INTRO_11, me);
					events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
					break;

			// H2 Intro Events
				case EVENT_INTRO_H2_2:
					DoScriptText(SAY_SYLVANAS_INTRO_2, me);
					events.ScheduleEvent(EVENT_INTRO_H2_3, 6000);
					break;
				case EVENT_INTRO_H2_3:
					DoScriptText(SAY_SYLVANAS_INTRO_3, me);
					DoCast(me, SPELL_CAST_VISUAL);
					pInstance->HandleGameObject(pInstance->GetData64(DATA_FROSTMOURNE), true);
					me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
					events.ScheduleEvent(EVENT_INTRO_H2_4, 6000);
					break;
				case EVENT_INTRO_H2_4:
					// spawn UTHER during speach 2
					if (Creature* pUther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
					{
						pUther->GetMotionMaster()->MoveIdle();
						pUther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
						uiUther = pUther->GetGUID();
						me->SetUInt64Value(UNIT_FIELD_TARGET, uiUther);
						pUther->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
					}
					events.ScheduleEvent(EVENT_INTRO_H2_5, 2000);
					break;
				case EVENT_INTRO_H2_5:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_H2_1, pUther);
					events.ScheduleEvent(EVENT_INTRO_H2_6, 11000);
					break;
				case EVENT_INTRO_H2_6:
					DoScriptText(SAY_SYLVANAS_INTRO_4, me);
					events.ScheduleEvent(EVENT_INTRO_H2_7, 3000);
					break;
				case EVENT_INTRO_H2_7:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_H2_2, pUther);
					events.ScheduleEvent(EVENT_INTRO_H2_8, 6000);
					break;
				case EVENT_INTRO_H2_8:
					DoScriptText(SAY_SYLVANAS_INTRO_5, me);
					events.ScheduleEvent(EVENT_INTRO_H2_9, 5000);
					break;
				case EVENT_INTRO_H2_9:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_H2_3, pUther);
					events.ScheduleEvent(EVENT_INTRO_H2_10, 19000);
					break;
				case EVENT_INTRO_H2_10:
					DoScriptText(SAY_SYLVANAS_INTRO_6, me);
					events.ScheduleEvent(EVENT_INTRO_H2_11, 1500);
					break;
				case EVENT_INTRO_H2_11:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_H2_4, pUther);
					events.ScheduleEvent(EVENT_INTRO_H2_12, 19500);
					break;
				case EVENT_INTRO_H2_12:
					DoScriptText(SAY_SYLVANAS_INTRO_7, me);
					events.ScheduleEvent(EVENT_INTRO_H2_13, 2000);
					break;
				case EVENT_INTRO_H2_13:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
					{
						pUther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
						DoScriptText(SAY_UTHER_INTRO_H2_5, pUther);
					}
					events.ScheduleEvent(EVENT_INTRO_H2_14, 12000);
					break;
				case EVENT_INTRO_H2_14:
					if (Creature* pUther = me->GetCreature(*me, uiUther))
						DoScriptText(SAY_UTHER_INTRO_H2_6, pUther);
					events.ScheduleEvent(EVENT_INTRO_H2_15, 8000);
					break;
				case EVENT_INTRO_H2_15:
					DoScriptText(SAY_SYLVANAS_INTRO_8, me);
					events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
					break;

			// Remaining Intro Events common for both faction
				case EVENT_INTRO_LK_1:
					// Spawn LK in front of door, and make him move to the sword.
					if (Creature* pLichKing = me->SummonCreature(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
					{
						pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
						pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
						pLichKing->SetReactState(REACT_PASSIVE);
						uiLichKing = pLichKing->GetGUID();
						pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

						if(GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_FROSTWORN_DOOR)))
							 pGate->SetGoState(GO_STATE_ACTIVE);

						me->SetUInt64Value(UNIT_FIELD_TARGET, uiLichKing);
						pLichKing->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
					}

					if (Creature* pUther = me->GetCreature(*me, uiUther))
					{
						pUther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);                        
						if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
							DoScriptText(SAY_UTHER_INTRO_A2_9, pUther);
						else
							DoScriptText(SAY_UTHER_INTRO_H2_7, pUther);
					}

					events.ScheduleEvent(EVENT_INTRO_LK_2, 11000);
					break;

				case EVENT_INTRO_LK_2:
					 if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
						 DoScriptText(SAY_LK_INTRO_1, pLichKing);
					if(GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_FROSTWORN_DOOR)))
						pGate->SetGoState(GO_STATE_READY);
					 events.ScheduleEvent(EVENT_INTRO_LK_3, 2000);
					 break;

				case EVENT_INTRO_LK_3:
					 // The Lich King banishes Uther to the abyss.
					 if (Creature* pUther = me->GetCreature(*me, uiUther))
					 {
						 pUther->CastSpell(pUther, SPELL_UTHER_DESPAWN, true);
						 pUther->DisappearAndDie();
						 uiUther = 0;
					 }
					 events.ScheduleEvent(EVENT_INTRO_LK_4, 5000);
					 break;

				case EVENT_INTRO_LK_4:
					  if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
						  DoScriptText(SAY_LK_INTRO_2, pLichKing);
					 // He steps forward and removes the runeblade from the heap of skulls.
					  if (GameObject *pFrostmourne = me->FindNearestGameObject(GO_FROSTMOURNE, 11.0f))
						 pFrostmourne->SetPhaseMask(0,true);

					  if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
					  {
						  pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, true);
						  pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, true);  
					  }

					  me->RemoveAllAuras();

					events.ScheduleEvent(EVENT_INTRO_LK_5, 10000);
					break;

				case EVENT_INTRO_LK_5:
					// summon Falric and Marwyn. then go back to the door
					if (Creature* pFalric = me->GetCreature(*me, pInstance->GetData64(DATA_FALRIC)))
					{
						pFalric->CastSpell(pFalric, SPELL_BOSS_SPAWN_AURA, true);
						pFalric->SetVisible(true);
						pFalric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
					}
					if (Creature* pMarwyn = me->GetCreature(*me, pInstance->GetData64(DATA_MARWYN)))
					{
						pMarwyn->CastSpell(pMarwyn, SPELL_BOSS_SPAWN_AURA, true);
						pMarwyn->SetVisible(true);
						pMarwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
					}

					if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
						DoScriptText(SAY_LK_INTRO_3, pLichKing);

					events.ScheduleEvent(EVENT_INTRO_LK_6, 8000);
					break;

				case EVENT_INTRO_LK_6:
					if (Creature* pFalric = me->GetCreature(*me, pInstance->GetData64(DATA_FALRIC)))
						DoScriptText(SAY_FALRIC_INTRO_1, pFalric);

					events.ScheduleEvent(EVENT_INTRO_LK_7, 2000);
					break;

				case EVENT_INTRO_LK_7:
					if (Creature* pMarwyn = me->GetCreature(*me, pInstance->GetData64(DATA_MARWYN)))
						DoScriptText(SAY_MARWYN_INTRO_1, pMarwyn);

					if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
					{
						if(GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_FROSTWORN_DOOR)))
							pGate->SetGoState(GO_STATE_ACTIVE);
						pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
						pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
					}

					events.ScheduleEvent(EVENT_INTRO_LK_8, 2000);
					break;

				case EVENT_INTRO_LK_8:
					if (Creature* pFalric = me->GetCreature(*me, pInstance->GetData64(DATA_FALRIC)))
					{
						pInstance->SetData(DATA_FALRIC_EVENT, SPECIAL);
						DoScriptText(SAY_FALRIC_INTRO_2, pFalric);
						pInstance->DoUpdateWorldState(WORLD_STATE_HOR, 1);
						pInstance->DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, 0);
					}
					events.ScheduleEvent(EVENT_INTRO_LK_9, 5000);
					break;

				case EVENT_INTRO_LK_9:
					if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
						DoScriptText(SAY_JAINA_INTRO_END, me);
					else
						DoScriptText(SAY_SYLVANAS_INTRO_END, me);

					me->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);

					if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
					{
						pLichKing->SetWalk(false);
						pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
					}

					events.ScheduleEvent(EVENT_INTRO_LK_10, 5000);
					break;

				case EVENT_INTRO_LK_10:
					 if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
						 if (pInstance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
							DoScriptText(SAY_LK_JAINA_INTRO_END, pLichKing);
						 else
							 DoScriptText(SAY_LK_SYLVANAS_INTRO_END, pLichKing);

					 events.ScheduleEvent(EVENT_INTRO_END, 7000);
					 break;

				case EVENT_INTRO_END:
					if (pInstance)
					{
						pInstance->SetData(DATA_INTRO_EVENT, DONE);
						pInstance->SetData(DATA_WAVE_COUNT, SPECIAL);   // start first wave
					}

					if(GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_FROSTWORN_DOOR)))
						pGate->SetGoState(GO_STATE_READY);

					if(GameObject* pGate = pInstance->instance->GetGameObject(pInstance->GetData64(DATA_FRONT_DOOR)))
						pGate->SetGoState(GO_STATE_READY);

					// Loralen or Koreln disappearAndDie()
					me->DisappearAndDie();
					if (Creature* pLichKing = me->GetCreature(*me, uiLichKing))
						pLichKing->DisappearAndDie();                

					break;

				case EVENT_SKIP_INTRO:
				   // Spawn LK in front of door, and make him move to the sword.
					if (Creature* pLichKing = me->SummonCreature(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
					{
						pLichKing->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
						pLichKing->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
						pLichKing->SetReactState(REACT_PASSIVE);
						uiLichKing = pLichKing->GetGUID();
						me->SetUInt64Value(UNIT_FIELD_TARGET, uiLichKing);
						pLichKing->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
					}

					me->SetWalk(false);
					me->GetMotionMaster()->MovePoint(0, MoveThronePos);

					events.ScheduleEvent(EVENT_INTRO_LK_4, 20000);
					break;
			}
		}
	};

};

enum TrashSpells
{
	// Ghostly Priest
	SPELL_SHADOW_WORD_PAIN                        = 72318,
	SPELL_CIRCLE_OF_DESTRUCTION                   = 72320,
	SPELL_COWER_IN_FEAR                           = 72321,
	SPELL_DARK_MENDING                            = 72322,

	// Phantom Mage
	SPELL_FIREBALL                                = 72163,
	SPELL_FLAMESTRIKE                             = 72169,
	SPELL_FROSTBOLT                               = 72166,
	SPELL_CHAINS_OF_ICE                           = 72121,
	SPELL_HALLUCINATION                           = 72342,

	// Phantom Hallucination (same as phantom mage + HALLUCINATION_2 when dies)
	SPELL_HALLUCINATION_2                         = 72344,

	// Shadowy Mercenary
	SPELL_SHADOW_STEP                             = 72326,
	SPELL_DEADLY_POISON                           = 72329,
	SPELL_ENVENOMED_DAGGER_THROW                  = 72333,
	SPELL_KIDNEY_SHOT                             = 72335,

	// Spectral Footman
	SPELL_SPECTRAL_STRIKE                         = 72198,
	SPELL_SHIELD_BASH                             = 72194,
	SPELL_TORTURED_ENRAGE                         = 72203,

	// Tortured Rifleman
	SPELL_SHOOT                                   = 72208,
	SPELL_CURSED_ARROW                            = 72222,
	SPELL_FROST_TRAP                              = 72215,
	SPELL_ICE_SHOT                                = 72268,

	// Quel'Delar Event
	SPELL_QUELDELAR_AURA                          = 70013,
};

enum TrashEvents
{
	EVENT_TRASH_NONE,

	// Ghostly Priest
	EVENT_SHADOW_WORD_PAIN,
	EVENT_CIRCLE_OF_DESTRUCTION,
	EVENT_COWER_IN_FEAR,
	EVENT_DARK_MENDING,

	// Phantom Mage
	EVENT_FIREBALL,
	EVENT_FLAMESTRIKE,
	EVENT_FROSTBOLT,
	EVENT_CHAINS_OF_ICE,
	EVENT_HALLUCINATION,

	// Shadowy Mercenary
	EVENT_SHADOW_STEP,
	EVENT_DEADLY_POISON,
	EVENT_ENVENOMED_DAGGER_THROW,
	EVENT_KIDNEY_SHOT,

	// Spectral Footman
	EVENT_SPECTRAL_STRIKE,
	EVENT_SHIELD_BASH,
	EVENT_TORTURED_ENRAGE,

	// Tortured Rifleman
	EVENT_SHOOT,
	EVENT_CURSED_ARROW,
	EVENT_FROST_TRAP,
	EVENT_ICE_SHOT,
};

class npc_ghostly_priest : public CreatureScript
{
public:
	npc_ghostly_priest() : CreatureScript("npc_ghostly_priest") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_ghostly_priestAI(creature);
	}

	struct npc_ghostly_priestAI: public ScriptedAI
	{
		npc_ghostly_priestAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000); // TODO: adjust timers
			events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
			events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
			events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					case EVENT_SHADOW_WORD_PAIN:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_SHADOW_WORD_PAIN);
						events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000);
						return;
					case EVENT_CIRCLE_OF_DESTRUCTION:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_CIRCLE_OF_DESTRUCTION);
						events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
						return;
					case EVENT_COWER_IN_FEAR:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_COWER_IN_FEAR);
						events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
						return;
					case EVENT_DARK_MENDING:
						// find an ally with missing HP
						if (Unit* target = DoSelectLowestHpFriendly(40, DUNGEON_MODE(30000, 50000)))
						{
							DoCast(target, SPELL_DARK_MENDING);
							events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
						}
						else
						{
							// no friendly unit with missing hp. re-check in just 5 sec.
							events.ScheduleEvent(EVENT_DARK_MENDING, 5000);
						}
						return;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

};

class npc_phantom_mage : public CreatureScript
{
public:
	npc_phantom_mage() : CreatureScript("npc_phantom_mage") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_phantom_mageAI(creature);
	}

	struct npc_phantom_mageAI: public ScriptedAI
	{
		npc_phantom_mageAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_FIREBALL, 3000); // TODO: adjust timers
			events.ScheduleEvent(EVENT_FLAMESTRIKE, 6000);
			events.ScheduleEvent(EVENT_FROSTBOLT, 9000);
			events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 12000);
			events.ScheduleEvent(EVENT_HALLUCINATION, 40000);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					case EVENT_FIREBALL:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_FIREBALL);
						events.ScheduleEvent(EVENT_FIREBALL, 15000);
						return;
					case EVENT_FLAMESTRIKE:
						DoCast(SPELL_FLAMESTRIKE);
						events.ScheduleEvent(EVENT_FLAMESTRIKE, 15000);
						return;
					case EVENT_FROSTBOLT:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_FROSTBOLT);
						events.ScheduleEvent(EVENT_FROSTBOLT, 15000);
						return;
					case EVENT_CHAINS_OF_ICE:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_CHAINS_OF_ICE);
						events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 15000);
						return;
					case EVENT_HALLUCINATION:
						DoCast(SPELL_HALLUCINATION);
						return;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

};

class npc_phantom_hallucination : public CreatureScript
{
public:
	npc_phantom_hallucination() : CreatureScript("npc_phantom_hallucination") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_phantom_hallucinationAI(creature);
	}

	struct npc_phantom_hallucinationAI : public npc_phantom_mage::npc_phantom_mageAI
	{
		npc_phantom_hallucinationAI(Creature* creature) : npc_phantom_mage::npc_phantom_mageAI(creature)
		{
		}

		void JustDied(Unit* /*who*/)
		{
			DoCast(SPELL_HALLUCINATION_2);
		}
	};

};

class npc_shadowy_mercenary : public CreatureScript
{
public:
	npc_shadowy_mercenary() : CreatureScript("npc_shadowy_mercenary") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_shadowy_mercenaryAI(creature);
	}

	struct npc_shadowy_mercenaryAI: public ScriptedAI
	{
		npc_shadowy_mercenaryAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_SHADOW_STEP, 8000); // TODO: adjust timers
			events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
			events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
			events.ScheduleEvent(EVENT_KIDNEY_SHOT, 12000);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					case EVENT_SHADOW_STEP:
						DoCast(SPELL_SHADOW_STEP);
						events.ScheduleEvent(EVENT_SHADOW_STEP, 8000);
						return;
					case EVENT_DEADLY_POISON:
						DoCast(me->getVictim(), SPELL_DEADLY_POISON);
						events.ScheduleEvent(EVENT_DEADLY_POISON, 10000);
						return;
					case EVENT_ENVENOMED_DAGGER_THROW:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_ENVENOMED_DAGGER_THROW);
						events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
						return;
					case EVENT_KIDNEY_SHOT:
						DoCast(me->getVictim(), SPELL_KIDNEY_SHOT);
						events.ScheduleEvent(EVENT_KIDNEY_SHOT, 10000);
						return;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

};

class npc_spectral_footman : public CreatureScript
{
public:
	npc_spectral_footman() : CreatureScript("npc_spectral_footman") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_spectral_footmanAI(creature);
	}

	struct npc_spectral_footmanAI: public ScriptedAI
	{
		npc_spectral_footmanAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000); // TODO: adjust timers
			events.ScheduleEvent(EVENT_SHIELD_BASH, 10000);
			events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					case EVENT_SPECTRAL_STRIKE:
						DoCast(me->getVictim(), SPELL_SPECTRAL_STRIKE);
						events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000);
						return;
					case EVENT_SHIELD_BASH:
						DoCast(me->getVictim(), SPELL_SHIELD_BASH);
						events.ScheduleEvent(EVENT_SHIELD_BASH, 5000);
						return;
					case EVENT_TORTURED_ENRAGE:
						DoCast(SPELL_TORTURED_ENRAGE);
						events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
						return;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

};

class npc_tortured_rifleman : public CreatureScript
{
public:
	npc_tortured_rifleman() : CreatureScript("npc_tortured_rifleman") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_tortured_riflemanAI(creature);
	}

	struct npc_tortured_riflemanAI  : public ScriptedAI
	{
		npc_tortured_riflemanAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap events;

		void Reset()
		{
			events.Reset();
		}

		void EnterCombat(Unit* /*who*/)
		{
			events.ScheduleEvent(EVENT_SHOOT, 2000); // TODO: adjust timers
			events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
			events.ScheduleEvent(EVENT_FROST_TRAP, 1000);
			events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch(eventId)
				{
					case EVENT_SHOOT:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_SHOOT);
						events.ScheduleEvent(EVENT_SHOOT, 2000);
						return;
					case EVENT_CURSED_ARROW:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_CURSED_ARROW);
						events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
						return;
					case EVENT_FROST_TRAP:
						DoCast(SPELL_FROST_TRAP);
						events.ScheduleEvent(EVENT_FROST_TRAP, 30000);
						return;
					case EVENT_ICE_SHOT:
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
							DoCast(target, SPELL_ICE_SHOT);
						events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
						return;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

};

enum GENERAL_EVENT
{
	SAY_AGGRO                    = -1594519,
	SAY_DEATH                    = -1594520,

	SPELL_SHIELD_THROWN          = 69222,
	H_SPELL_SHIELD_THROWN        = 73076,
	SPELL_SPIKE                  = 69184,
	H_SPELL_SPIKE                = 70399,
	SPELL_CLONE_NAME             = 57507,
	SPELL_CLONE_MODEL            = 45204,

	// Reflection'Spells
	SPELL_BALEFUL_STRIKE         = 69933,
	SPELL_SPIRIT_BURST           = 69900,
	H_SPELL_BALEFUL_STRIKE       = 70400,
	H_SPELL_SPIRIT_BURST         = 73046,
};

class npc_frostworn_general : public CreatureScript
{
public:
	npc_frostworn_general() : CreatureScript("npc_frostworn_general") { }

	struct npc_frostworn_generalAI : public ScriptedAI
	{
		npc_frostworn_generalAI(Creature *creature) : ScriptedAI(creature)
		{
			pInstance = (InstanceScript*)creature->GetInstanceScript();
			Reset();
		}

		InstanceScript* pInstance;

		uint32 uiShieldTimer;
		uint32 uiSpikeTimer;
		uint32 uiCloneTimer;

		void Reset()
		{
			if (!pInstance)
				return;
			uiShieldTimer = 5000;
			uiSpikeTimer = 14000;
			uiCloneTimer = 22000;
			pInstance->SetData(DATA_FROSWORN_EVENT, NOT_STARTED);
			me->SetReactState(REACT_AGGRESSIVE);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void JustDied(Unit* pKiller)
		{
			if (!pInstance)
				return;
			DoScriptText(SAY_DEATH, me);
			pInstance->SetData(DATA_FROSWORN_EVENT, DONE);
		}

		void MoveInLineOfSight(Unit* who)
		{
			if (!pInstance)
				return;

			if (me->getVictim())
				return;

			if (who->GetTypeId() != TYPEID_PLAYER
				|| pInstance->GetData(DATA_MARWYN_EVENT) != DONE
				|| !me->IsWithinDistInMap(who, 20.0f)
				) return;

			if (Player* player = (Player*)who)
				if (player->isGameMaster())
					return;

			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

			AttackStart(who);
		}

		void EnterCombat(Unit* victim)
		{
			if (!pInstance)
				return;
			DoScriptText(SAY_AGGRO, me);
			pInstance->SetData(DATA_FROSWORN_EVENT, IN_PROGRESS);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
		}

		void UpdateAI(const uint32 uiDiff)
		{
			if(!UpdateVictim())
				return;

			if(uiShieldTimer < uiDiff)
			{
				if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
				   DoCast(target, SPELL_SHIELD_THROWN);
				uiShieldTimer = urand(8000, 12000);
			}
			else
				uiShieldTimer -= uiDiff;

			if (uiSpikeTimer < uiDiff)
			{
				if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
					DoCast(target, SPELL_SPIKE);
				uiSpikeTimer = urand(15000, 20000);
			}
			else
				uiSpikeTimer -= uiDiff;

			if (uiCloneTimer < uiDiff)
			{
				SummonClones();
				uiCloneTimer = 60000;
			}
			else
				uiCloneTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}

		void SummonClones()
		{
			std::list<Unit *> playerList;
			SelectTargetList(playerList, 5, SELECT_TARGET_TOPAGGRO, 0, true);
			for (std::list<Unit*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
			{
				Unit* pTemp = (*itr);
				Creature* pReflection = me->SummonCreature(NPC_REFLECTION, pTemp->GetPositionX(), pTemp->GetPositionY(), pTemp->GetPositionZ(), pTemp->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
				pReflection->SetName(pTemp->GetName());
				pTemp->CastSpell(pReflection, SPELL_CLONE_NAME, true);
				pTemp->CastSpell(pReflection, SPELL_CLONE_MODEL, true);
				pReflection->setFaction(me->getFaction());
				pReflection->AI()->AttackStart(pTemp);
			}

		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_frostworn_generalAI(creature);
	}
};

class npc_spiritual_reflection : public CreatureScript
{
public:
	npc_spiritual_reflection() : CreatureScript("npc_spiritual_reflection") { }

	struct npc_spiritual_reflectionAI : public ScriptedAI
	{
		npc_spiritual_reflectionAI(Creature *creature) : ScriptedAI(creature)
		{
			Reset();
		}

		InstanceScript* pInstance;    
		uint32 uiStrikeTimer;

		void Reset()
		{
			uiStrikeTimer = urand(1000,3000);    
		}

		void JustDied(Unit* pKiller)
		{
			DoCast(pKiller, SPELL_SPIRIT_BURST);            
		}
		
		void UpdateAI(const uint32 uiDiff)
		{
			if(!UpdateVictim())
				return;

			if(uiStrikeTimer < uiDiff)
			{
				if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
				   DoCast(target, SPELL_BALEFUL_STRIKE);
				uiStrikeTimer = urand(3000, 8000);
			}
			else
				uiStrikeTimer -= uiDiff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_spiritual_reflectionAI(creature);
	}
};

enum Defs
{
	/*INTRO - Pre Escape*/
	SAY_LICH_KING_AGGRO_A              = -1594477,
	SAY_LICH_KING_AGGRO_H              = -1594478,
	SAY_JAINA_AGGRO                    = -1594479,
	SAY_SYLVANA_AGGRO                  = -1594480,

	/*ESCAPE*/
	SAY_JAINA_WALL_01                  = -1594487,
	SAY_SYLVANA_WALL_01                = -1594488,
	SAY_JAINA_WALL_02                  = -1594489,
	SAY_SYLVANA_WALL_02                = -1594490,
	SAY_LICH_KING_WALL_02              = -1594491,
	SAY_LICH_KING_WALL_03              = -1594492,
	SAY_LICH_KING_WALL_04              = -1594493,
	SAY_JAINA_WALL_03                  = -1594494,
	SAY_JAINA_WALL_04                  = -1594495,
	SAY_SYLVANA_WALL_03                = -1594496,
	SAY_SYLVANA_WALL_04                = -1594497,
	SAY_JAINA_ESCAPE_01                = -1594498,
	SAY_JAINA_ESCAPE_02                = -1594499,
	SAY_SYLVANA_ESCAPE_01              = -1594500,
	SAY_SYLVANA_ESCAPE_02              = -1594501,
	SAY_JAINA_TRAP                     = -1594502,
	SAY_SYLVANA_TRAP                   = -1594503,
	SAY_MATHEAS_JAINA                  = -1594505,
	SAY_JAINA_FIRE                     = -1594526,
	SAY_SYLVANA_FIRE                   = -1594522,
	SAY_SYLVANA_FINAL_1                = -1594523,
	SAY_SYLVANA_FINAL_2                = -1594521,
	SAY_JAINA_FINAL_1                  = -1594527,
	SAY_JAINA_FINAL_2                  = -1594524,
	SAY_JAINA_FINAL_3                  = -1594525,
	SAY_LICH_KING_END_01               = -1594506,
	SAY_LICH_KING_END_02               = -1594507,
	SAY_LICH_KING_END_03               = -1594508,

	/*SPELLS AND VISUAL EFFECTS*/
	SPELL_WINTER                       = 69780,
	SPELL_FURY_OF_FROSTMOURNE          = 70063,
	SPELL_ICE_PRISON_VISUAL            = 69708,
	SPELL_DARK_ARROW                   = 70194,
	SPELL_ICE_BARRIER                  = 69787,
	SPELL_DESTROY_ICE_WALL_01          = 69784, //Jaina
	SPELL_DESTROY_ICE_WALL_03          = 70225, //Sylvana
	SPELL_SYLVANA_JUMP                 = 68339,
	SPELL_SYLVANA_STEP                 = 69087,
	SPELL_FIRE_CANNON                  = 67461,

	FACTION                            = 2076,
};

const Position CannonSpawns[4] = 
{
	{5215.912109f, 1658.796997f, 802.862976f, 0.00f},
	{5229.619141f, 1646.045166f, 802.862976f, 0.00f},
	{5244.881348f, 1626.080688f, 802.862976f, 0.00f},
	{5256.842285f, 1617.717041f, 802.051331f, 0.00f},
};

class npc_jaina_and_sylvana_hor_part2 : public CreatureScript
{
public:
	npc_jaina_and_sylvana_hor_part2() : CreatureScript("npc_jaina_and_sylvana_hor_part2") { }

	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
	{
		InstanceScript* m_pInstance = (InstanceScript*)creature->GetInstanceScript();
		switch (uiAction)
		{
			case GOSSIP_ACTION_INFO_DEF+1:
				player->CLOSE_GOSSIP_MENU();
				((npc_jaina_and_sylvana_hor_part2AI*)creature->AI())->Start(false,true);
				creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
				creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
				creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
				creature->setActive(true);

				if(m_pInstance)
				{
					m_pInstance->SetData64(DATA_ESCAPE_LIDER, creature->GetGUID());
					m_pInstance->SetData(DATA_LICHKING_EVENT, IN_PROGRESS);
				}
				return true;
			default:
				return false;
		}
	}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		InstanceScript*   m_pInstance = (InstanceScript*)creature->GetInstanceScript();

		if(!m_pInstance)
			return false;

		if(m_pInstance->GetData(DATA_LICHKING_EVENT) == DONE)
			return false;

		if(creature->isQuestGiver())
		   player->PrepareQuestMenu( creature->GetGUID());

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We run away!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

		player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

		return true;
	}

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_jaina_and_sylvana_hor_part2AI(creature);
	}

	struct npc_jaina_and_sylvana_hor_part2AI : public npc_escortAI
	{
		npc_jaina_and_sylvana_hor_part2AI(Creature *creature) : npc_escortAI(creature)
		{
			m_pInstance = (InstanceScript*)creature->GetInstanceScript();
			Reset();
		}

		InstanceScript* m_pInstance;

		uint32 CastTimer;
		uint32 StepTimer;
		uint32 Step;
		int32 HoldTimer;
		uint32 Count;
		bool Fight;
		bool Event;
		bool PreFight;
		bool WallCast;
		uint64 m_uiLichKingGUID;
		uint64 m_uiLiderGUID;
		uint64 m_uiIceWallGUID;
		uint64 m_uipWallTargetGUID;
		uint64 uiCaptain;
		Creature* pLichKing;
		uint32 m_chestID;

		void Reset()
		{
			if(!m_pInstance)
				return;

			if(m_pInstance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
				return;

			Step = 0;
			StepTimer = 500;
			Fight = true;
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
			m_uipWallTargetGUID = 0;

			if(me->GetEntry() == NPC_JAINA_OUTRO)
			{
				me->CastSpell(me, SPELL_ICE_BARRIER, false);
				me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
			}
			if(m_pInstance->GetData(DATA_LICHKING_EVENT) == DONE)
				me->SetVisible(false);

		}

		void AttackStart(Unit* who)
		{
			if(!who)
				return;

			if(me->GetEntry() != NPC_SYLVANA_OUTRO)
				return;

			if(m_pInstance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS || Fight != true)
				return;

			npc_escortAI::AttackStart(who);

		}

		void JustDied(Unit* pKiller)
		{
			if(!m_pInstance)
				return;
			m_pInstance->SetData(DATA_LICHKING_EVENT, FAIL);
		}

		void WaypointReached(uint32 i)
		{
			switch(i)
			{
				case 3:
					m_pInstance->SetData(DATA_ICE_WALL_1, IN_PROGRESS);
					if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_1)))
					{
						pGate->SetGoState(GO_STATE_READY);
						m_uiIceWallGUID = pGate->GetGUID();
					}
					break;
				case 4:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_WALL_01, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_WALL_01, me);
					CastTimer = 1000;
					HoldTimer = 30000;
					SetEscortPaused(true);
					if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
					{
						m_uipWallTargetGUID = pWallTarget->GetGUID();
						pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
						if(me->GetEntry() == NPC_JAINA_OUTRO)
							me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
					}
					WallCast = true;
					break;
				case 6:
					m_pInstance->SetData(DATA_ICE_WALL_2, IN_PROGRESS);
					if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
					{
						if(pWallTarget->isAlive())
						{
							pWallTarget->ForcedDespawn();
							m_uipWallTargetGUID = 0;
						}
					}
					break;
				case 8:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_WALL_02, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_WALL_02, me);
					CastTimer = 1000;
					HoldTimer = 30000;
					SetEscortPaused(true);
					if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
					{
						m_uipWallTargetGUID = pWallTarget->GetGUID();
						pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
						if(me->GetEntry() == NPC_JAINA_OUTRO)
							me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
					}
					WallCast = true;
					break;
				case 9:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_ESCAPE_01, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_ESCAPE_01, me);
					break;
				case 11:
					m_pInstance->SetData(DATA_ICE_WALL_3, IN_PROGRESS);
					if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
					{
						if(pWallTarget->isAlive())
						{
							pWallTarget->ForcedDespawn();
							m_uipWallTargetGUID = 0;
						}
					}
					break;
				case 12:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_WALL_03, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_WALL_03, me);
					CastTimer = 1000;
					HoldTimer = 30000;
					SetEscortPaused(true);
					if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
					{
						m_uipWallTargetGUID = pWallTarget->GetGUID();
						pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
						if(me->GetEntry() == NPC_JAINA_OUTRO)
							me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
					}
					WallCast = true;
					break;
				case 13:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_ESCAPE_02, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_ESCAPE_02, me);
					break;
				case 15:
					m_pInstance->SetData(DATA_ICE_WALL_4, IN_PROGRESS);
					if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
					{
						if(pWallTarget->isAlive())
						{
							pWallTarget->ForcedDespawn();
							m_uipWallTargetGUID = 0;
						}
					}
					break;
				case 16:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_WALL_04, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_WALL_04, me);
					CastTimer = 1000;
					HoldTimer = 30000;
					SetEscortPaused(true);
					if (Creature *pWallTarget = me->SummonCreature(NPC_ICE_WALL,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000))
					{
						m_uipWallTargetGUID = pWallTarget->GetGUID();
						pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
						if(me->GetEntry() == NPC_JAINA_OUTRO)
							me->CastSpell(me, SPELL_DESTROY_ICE_WALL_01, false);
					}
					WallCast = true;
					break;
				case 19:
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						DoScriptText(SAY_JAINA_TRAP, me);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_TRAP, me);
					break;
				case 20:
					if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
					{
						if(pWallTarget->isAlive())
						{
							pWallTarget->ForcedDespawn();
							m_uipWallTargetGUID = 0;
						}
					}
					SetEscortPaused(true);
					if(me->GetEntry() == NPC_JAINA_OUTRO)
						me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
					break;
			}
		}

		void MoveInLineOfSight(Unit* who)
		{
			if(!who || !m_pInstance)
				return;

			if(who->GetTypeId() != TYPEID_PLAYER)
				return;

			Player* player = (Player *)who;

			if(player->GetTeam() == ALLIANCE && me->GetEntry() == NPC_SYLVANA_OUTRO)
				return;

			if(player->GetTeam() == HORDE && me->GetEntry() == NPC_JAINA_OUTRO)
				return;

			if(me->IsWithinDistInMap(who, 50.0f)
				&& m_pInstance->GetData(DATA_FROSWORN_EVENT) == DONE
				&& m_pInstance->GetData(DATA_PHASE) == 3)
			{
				player = (Player *)who;
				Event = true;
				me->setFaction(FACTION);
				m_pInstance->SetData(DATA_PHASE, 4);
			}
		}

		void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
		{
			if(!m_pInstance)
				return;

			if(m_pInstance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
			{
				uiDamage = 0;
				return;
			}

			if(m_pInstance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS && WallCast == true)
			{
				HoldTimer = HoldTimer + 100;
				return;
			}
		}

		void JumpNextStep(uint32 Time)
		{
			StepTimer = Time;
			Step++;
		}

		void Intro()
		{
			switch(Step)
			{
				case 0:
					me->SetWalk(false);
					m_uiLichKingGUID = m_pInstance->GetData64(DATA_LICHKING);
					pLichKing = m_pInstance->instance->GetCreature(m_uiLichKingGUID);
					me->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
					JumpNextStep(100);
					break;
				case 1:
					HoRQuestComplete(me->GetEntry());
					if(pLichKing)
					{
						if(me->GetEntry() == NPC_JAINA_OUTRO)
							DoScriptText(SAY_LICH_KING_AGGRO_A, pLichKing);
						if(me->GetEntry() == NPC_SYLVANA_OUTRO)
							DoScriptText(SAY_LICH_KING_AGGRO_H, pLichKing);
						pLichKing->AI()->AttackStart(me);
						me->AI()->AttackStart(pLichKing);
					}
					JumpNextStep(3000);
					break;
				case 2:
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
					{
						Fight = false;
						if(pLichKing)
							me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
						JumpNextStep(3000);
					}
					else
						JumpNextStep(100);
					break;
				case 3:
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						Fight = true;
					JumpNextStep(100);
					break;
				case 4:
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
					{
						if(pLichKing)
							me->CastSpell(pLichKing, SPELL_SYLVANA_STEP, false);
						JumpNextStep(3000);
					}
					else
						JumpNextStep(100);
					break;
				case 5:
					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
					{
						Fight = false;
						if(pLichKing)
							me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
						JumpNextStep(3000);
					}
					else
						JumpNextStep(12000);
					break;
				case 6:
					Fight = true;

					if(pLichKing)
					{
						if(me->GetEntry() == NPC_SYLVANA_OUTRO)
							DoCast(SPELL_SYLVANA_JUMP);
						DoCast(pLichKing, me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, false);
						pLichKing->AttackStop();
					}
					JumpNextStep(1480);
					break;
				case 7:
					me->RemoveAllAuras();

					if(pLichKing && !pLichKing->HasAura(SPELL_ICE_PRISON_VISUAL))
					{
						pLichKing->AddAura(me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, pLichKing);
						pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					}

					me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
					me->AttackStop();

					if(me->GetEntry() == NPC_JAINA_OUTRO)
					{
						me->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);
						DoScriptText(SAY_JAINA_AGGRO, me);
					}

					if(me->GetEntry() == NPC_SYLVANA_OUTRO)
						DoScriptText(SAY_SYLVANA_AGGRO, me);

					JumpNextStep(3000);
					break;
				case 8:
					me->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
					if(pLichKing && !pLichKing->HasAura(SPELL_ICE_PRISON_VISUAL))
					{
						pLichKing->AddAura(me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, pLichKing);
						me->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
					}
					JumpNextStep(10000);
					break;
				case 9:
					if(pLichKing && (!pLichKing->HasAura(SPELL_ICE_PRISON_VISUAL) || !pLichKing->HasAura(SPELL_DARK_ARROW)))
					{
						pLichKing->AddAura(me->GetEntry() == NPC_JAINA_OUTRO ? SPELL_ICE_PRISON_VISUAL : SPELL_DARK_ARROW, pLichKing);
						me->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
					}
					me->RemoveAllAuras();
					me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
					Count = 1;
					JumpNextStep(100);
					break;
			}
		}

		void Outro()
		{
			switch(Step)
			{
				case 10:
					JumpNextStep(5000);
					break;
				case 11:
					if (Creature* pCaptain = ObjectAccessor::GetCreature(*me, m_pInstance->GetData64(DATA_CAPTAIN)))
					{
						pCaptain->AI()->DoAction(ACTION_START_FLY);
					}
					JumpNextStep(10000);
					break;
				case 12:
					if(GameObject* pCave = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CAVE)))
					{
						Creature* pCaveTarget = me->SummonCreature(NPC_ICE_WALL,pCave->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation(),TEMPSUMMON_MANUAL_DESPAWN,720000);
						pCaveTarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 4);

						for(int8 i = 0; i<4; i++)
						{
							if(Creature* pCannoner = me->SummonCreature(NPC_ICE_WALL, CannonSpawns[i],TEMPSUMMON_MANUAL_DESPAWN,720000))
							{
								pCannoner->SetPower(POWER_ENERGY, 100);
								pCannoner->setFaction(me->getFaction());
								pCannoner->CastSpell(pCaveTarget, SPELL_FIRE_CANNON, true);
							}
						}
					}        
					JumpNextStep(4000);
					break;
				case 13:
					me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
					if(GameObject* pCave = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_CAVE)))
						pCave->SetGoState(GO_STATE_READY);
					me->RemoveAllAuras();

					if (pLichKing)
						pLichKing->DespawnOrUnsummon();

					HoRQuestComplete(38211);
					JumpNextStep(10000);
					break;
				case 14:
					DoScriptText(me->GetEntry() == NPC_JAINA_OUTRO ? SAY_JAINA_FINAL_2 : SAY_SYLVANA_FINAL_2, me);
					JumpNextStep(20000);
					break;
				case 15:
					DoScriptText(me->GetEntry() == NPC_JAINA_OUTRO ? SAY_JAINA_FINAL_3 : 0, me);
					JumpNextStep(5000);
					break;
				case 16:
					me->SetOrientation(0.68f);
					me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
					me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
					JumpNextStep(5000);
					break;
			}
		}

		void HoRQuestComplete(uint32 killCredit)
		{
			if(m_pInstance)
			{
				Map::PlayerList const &PlayerList = m_pInstance->instance->GetPlayers();
				for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
					i->getSource()->KilledMonsterCredit(killCredit, 0);
			}
		}

		void UpdateEscortAI(const uint32 diff)
		{
			if(!m_pInstance || !Event)
				return;

			DoMeleeAttackIfReady();

			if(m_pInstance->GetData(DATA_PHASE) == 4 && m_pInstance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
			{
				if(StepTimer < diff)
					Intro();
				else
					StepTimer -= diff;
			}

			if(m_pInstance->GetData(DATA_LICHKING_EVENT) == SPECIAL
				&& m_pInstance->GetData(DATA_PHASE) != 6)       //End Cinematic
			{
				m_pInstance->SetData(DATA_PHASE, 6);
				Step = 10;
			}

			if (m_pInstance->GetData(DATA_PHASE) == 6)
			{
				if(StepTimer < diff)
					Outro();
				else
					StepTimer -= diff;
				return;
			}

			if(WallCast == true && CastTimer < diff)
			{
				if(me->GetEntry() == NPC_SYLVANA_OUTRO)
				{
				   if (Creature* pWallTarget = m_pInstance->instance->GetCreature(m_uipWallTargetGUID))
						me->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_03, false);
					CastTimer = 1000;
				}
			}
			else
				CastTimer -= diff;

			if (WallCast == true && HoldTimer < 10000 && ( m_pInstance->GetData(DATA_SUMMONS) == 0 || !me->isInCombat()))
			{
				WallCast = false;
				me->InterruptNonMeleeSpells(false);
				SetEscortPaused(false);
				if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiIceWallGUID))
					pGate->SetGoState(GO_STATE_ACTIVE);
				++Count;
				switch(Count)
				{
					case 2:
						if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_2)))
						{
							pGate->SetGoState(GO_STATE_READY);
							if(pLichKing && pLichKing->isAlive())
								DoScriptText(SAY_LICH_KING_WALL_02, pLichKing);
							m_uiIceWallGUID = pGate->GetGUID();
						}
						break;
					case 3:
						if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_3)))
						{
							pGate->SetGoState(GO_STATE_READY);
							if(pLichKing && pLichKing->isAlive())
								DoScriptText(SAY_LICH_KING_WALL_03, pLichKing);
							m_uiIceWallGUID = pGate->GetGUID();
						}
						break;
					case 4:
						if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_ICE_WALL_4)))
						{
							pGate->SetGoState(GO_STATE_READY);
							if(pLichKing && pLichKing->isAlive())
								DoScriptText(SAY_LICH_KING_WALL_04, pLichKing);
							m_uiIceWallGUID = pGate->GetGUID();
						}
						break;
					case 5:
						if(pLichKing && pLichKing->isAlive())
						{
							pLichKing->RemoveAurasDueToSpell(SPELL_WINTER);
							pLichKing->SetSpeed(MOVE_WALK, 2.5f, true);
							Step = 0;
						}
						break;
				}
			}
			else
			{
				HoldTimer -= diff;
				if (HoldTimer <= 0)
					HoldTimer = 0;
			}

			return;
		}
	};
};

class at_hor_waves_restarter : public AreaTriggerScript
{
	public:
		at_hor_waves_restarter() : AreaTriggerScript("at_hor_waves_restarter") {}

		bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
		{
			InstanceScript* pInstance = player->GetInstanceScript();

			if (player->isGameMaster())
				return true;

			if(pInstance->GetData(DATA_WAVE_COUNT) == SPECIAL)
				return true;

			if (pInstance->GetData(DATA_INTRO_EVENT) == DONE && pInstance->GetData(DATA_MARWYN_EVENT) != DONE)
			{
				pInstance->SetData(DATA_WAVE_COUNT, SPECIAL);

				if (Creature* pFalric = player->GetCreature(*player, pInstance->GetData64(DATA_FALRIC)))
				{
					pFalric->CastSpell(pFalric, SPELL_BOSS_SPAWN_AURA, true);
					pFalric->SetVisible(true);
				}
				if (Creature* pMarwyn = player->GetCreature(*player, pInstance->GetData64(DATA_MARWYN)))
				{
					pMarwyn->CastSpell(pMarwyn, SPELL_BOSS_SPAWN_AURA, true);
					pMarwyn->SetVisible(true);
				}
			}
			return true;
		}
};

class npc_capitan_gunship: public CreatureScript
{
	public:
		npc_capitan_gunship() : CreatureScript("npc_capitan_gunship") { }

		struct npc_capitan_gunshipAI : public ScriptedAI
		{
			npc_capitan_gunshipAI(Creature* creature) : ScriptedAI(creature)
			{
				instance = creature->GetInstanceScript();
			}

			void Reset()
			{
				gunship = me->GetTransport();
			}

			void DoAction(int32 const action)
			{
				switch (action)
				{
					case ACTION_START_FLY:
						StartFly(gunship);
						events.ScheduleEvent(EVENT_OUTRO_1, 6000);
						break;
				}
			}

			void UpdateAI(const uint32 diff)
			{
				events.Update(diff);
				
				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
						case EVENT_OUTRO_1:
							if(instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
							{
								DoScriptText(SAY_JAINA_FIRE, me);
								events.ScheduleEvent(EVENT_OUTRO_2, 5500);
							} else
							{
								DoScriptText(SAY_SYLVANA_FIRE, me);
								events.ScheduleEvent(EVENT_OUTRO_2, 5500);
							}
							
							StopFly(gunship);
							break;
						case EVENT_OUTRO_2:
							StartFly(gunship);
							events.ScheduleEvent(EVENT_OUTRO_3, 4000);
							break;
						case EVENT_OUTRO_3:
							StopFly(gunship);
							instance->SetData(DATA_LICHKING_EVENT, DONE);
							events.ScheduleEvent(EVENT_OUTRO_4, 7000);
							if(instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
							{
								DoScriptText(SAY_JAINA_FINAL_1, me);
								me->SummonGameObject(GO_STAIRS_SKYBREAKER, 5229.25f, 1641.51f, 784.304f, 5.46113f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
								me->SummonGameObject(GO_STAIRS_SKYBREAKER, 5214.03f, 1627.29f, 797.229f, 5.46113f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
							} else
							{
								DoScriptText(SAY_SYLVANA_FINAL_1, me);
								me->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 524.15f, 1624.48f, 784.302f, 5.88208f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
								me->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 5237.84f, 1608.19f, 797.127f, 5.88208f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
								me->SummonGameObject(GO_STAIRS_ORGRIM_HAMMER, 5227.70f, 1589.14f, 807.735f, 5.88208f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
							}
							break;
						case EVENT_OUTRO_4:
							if(instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
							{
								me->SummonGameObject(GO_PORTAL, 5211.70496f, 1595.272065f, 807.46669f, 0, 0, 0, 0, 0, 100000);
								me->SummonGameObject(DUNGEON_MODE(GO_CAPTAIN_CHEST_2, GO_CAPTAIN_CHEST_4), 5196.40496f, 1608.379865f, 806.4349f, 0, 0, 0, 0, 0, 100000);
							} else
							{
								me->SummonGameObject(GO_PORTAL, 5219.33296f, 1569.029065f, 819.36969f, 0, 0, 0, 0, 0, 100000);
								me->SummonGameObject(DUNGEON_MODE(GO_CAPTAIN_CHEST_1, GO_CAPTAIN_CHEST_3), 5206.70996f, 1570.794065f, 818.77069f, 0, 0, 0, 0, 0, 100000);
							}
							break;
					}
				}
							
			}

			private:
				Transport* gunship;
				EventMap events;
				InstanceScript* instance;
		};

		CreatureAI* GetAI(Creature* pCreature) const
		{
			return new npc_capitan_gunshipAI(pCreature);
		}
};

enum SayS_Quest
{
	SAY_QUEST_QUEL = -1652000,
	SAY_QUEST_QUEL1 = -1652001,
	SAY_QUEST_QUEL2 = -1652002,
	SAY_QUEST_QUEL3 = -1652003,
	SAY_QUEST_QUEL4 = -1652004,
	SAY_QUEST_QUEL5 = -1652006,
	SAY_QUEST_QUEL6 = -1652007,
	SAY_QUEST_QUEL7 = -1652008,
};

class npc_quest_quel_dellar : public CreatureScript
{
public:
	npc_quest_quel_dellar() : CreatureScript("npc_quest_quel_dellar") { }

	struct npc_quest_quel_dellarAI  : public ScriptedAI
	{
		npc_quest_quel_dellarAI(Creature* pCreature) : ScriptedAI(pCreature)
		{
		}
		
		void Reset()
		{
			me->SetVisible(true);
			StartEvent = false;
			Uther = NULL;
		}
		
		 void MoveInLineOfSight(Unit* pWho)
		{
		   if(!StartEvent)
		   {
			if (pWho->GetTypeId() != TYPEID_PLAYER || pWho->ToPlayer()->GetQuestStatus(24480) != QUEST_STATUS_INCOMPLETE && pWho->ToPlayer()->GetQuestStatus(24561) != QUEST_STATUS_INCOMPLETE)
			{
				me->SetVisible(false);
				return;
			}
		
			pWho->ToPlayer()->KilledMonsterCredit(37158, 0);
			me->SetVisible(true);
			events.ScheduleEvent(EVENT_QUEST_QUEL_1, 100);
			StartEvent = true;
		   }
		}

		void UpdateAI(const uint32 diff)
		{

			events.Update(diff);
		
			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_QUEST_QUEL_1:
					DoCast(me, SPELL_CAST_VISUAL);
					if(Creature* pUther = me->SummonCreature(37225,5297.84f,1985.44f,707.696f,3.93116f,TEMPSUMMON_MANUAL_DESPAWN))
					{
						Uther = pUther;
						Uther->SetVisible(true);
						DoScriptText(SAY_QUEST_QUEL, Uther);
					}
					events.ScheduleEvent(EVENT_QUEST_QUEL_2, 8000);
					break;
				case EVENT_QUEST_QUEL_2:
					DoScriptText(SAY_QUEST_QUEL1,me);
					DoCast(me, 70300);
					events.ScheduleEvent(EVENT_QUEST_QUEL_3, 3000);
					break;
				case EVENT_QUEST_QUEL_3:
					if(Uther)
						DoScriptText(SAY_QUEST_QUEL2, Uther);
					events.ScheduleEvent(EVENT_QUEST_QUEL_4, 7000);
					break;
				case EVENT_QUEST_QUEL_4:
					if(Uther)
					{
						Uther->SetWalk(false);
						Uther->GetMotionMaster()->MovePoint(0, 5335.5f, 1981.74f, 709.32f);
					}
					events.ScheduleEvent(EVENT_QUEST_QUEL_5, 6000);
					break;
				case EVENT_QUEST_QUEL_5:
					if(Uther)
						Uther->SetFacingToObject(me);
					DoScriptText(SAY_QUEST_QUEL3, me);
					events.ScheduleEvent(EVENT_ATTACK_START, 3000);
					break;
				case EVENT_ATTACK_START:
					me->RemoveAurasDueToSpell(70300);
					me->SetReactState(REACT_AGGRESSIVE);
						if (Uther)
							Uther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SIT);
					if (Unit* target = SelectTarget(SELECT_TARGET_NEAREST,0, 100.0f, true))
					{
						AttackStart(target);
						me->GetMotionMaster()->MoveChase(target);
					}
					events.ScheduleEvent(EVENT_BLADESTORM, 2000);
					events.ScheduleEvent(EVENT_WHIRLWIND, 10000);
					events.ScheduleEvent(EVENT_MORTAL_STRIKE, 14000);
					events.ScheduleEvent(EVENT_HEROIC_STRIKE, 8000);
					break;
				case EVENT_WHIRLWIND:
					if (UpdateVictim())
					{
						DoCastVictim(67716);
						events.ScheduleEvent(EVENT_WHIRLWIND, 10000);
					}
					break;
				case EVENT_BLADESTORM:
					if (UpdateVictim())
					{
						DoCastVictim(67541);
						events.ScheduleEvent(EVENT_BLADESTORM, 8000);
					}
					break;
				case EVENT_MORTAL_STRIKE:
					if (UpdateVictim())
					{
						DoCastVictim(16856);
						events.ScheduleEvent(EVENT_MORTAL_STRIKE, 14000);
					}
					break;
				case EVENT_HEROIC_STRIKE:
					if (UpdateVictim())
					{
						DoCastVictim(29426);
						events.ScheduleEvent(EVENT_HEROIC_STRIKE, 8000);
					}
					break;
				}

				DoMeleeAttackIfReady();
			}
		}
		
		void JustDied(Unit* /*pKiller*/)
		{
			if(Creature* pUther = GetClosestCreatureWithEntry(me, 37225, 100.0f))
				pUther->AI()->DoAction(1);
		}
		
		private:
			EventMap events;
			bool StartEvent;
			Creature* Uther;
			
	};
	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_quest_quel_dellarAI(pCreature);
	}
};

class npc_quest_quel_uther : public CreatureScript
{
public:
	npc_quest_quel_uther() : CreatureScript("npc_quest_quel_uther") { }

	struct npc_quest_quel_utherAI  : public ScriptedAI
	{
		npc_quest_quel_utherAI(Creature* pCreature) : ScriptedAI(pCreature)
		{
		}
		
		void Reset()
		{
		}
		

		void DoAction(const int32 actionId)
		{
			if (actionId == 1)
			{
				events.ScheduleEvent(EVENT_QUEST_QUEL_6, 100);
			}
		}
			
		void UpdateAI(const uint32 diff)
		{
			events.Update(diff);
		
			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_QUEST_QUEL_6:
					me->HandleEmoteCommand(EMOTE_STATE_STAND);
					me->SetWalk(true);
					me->GetMotionMaster()->MovePoint(0, 5309.5f, 1991.74f, 707.32f);
					events.ScheduleEvent(EVENT_QUEST_QUEL_7, 5000);
					break;
				case EVENT_QUEST_QUEL_7:
					DoScriptText(SAY_QUEST_QUEL4,me);
					events.ScheduleEvent(EVENT_QUEST_QUEL_8, 10000);
					break;
				case EVENT_QUEST_QUEL_8:
					DoScriptText(SAY_QUEST_QUEL5,me);
					events.ScheduleEvent(EVENT_QUEST_QUEL_9, 10000);
					break;
				case EVENT_QUEST_QUEL_9:
					DoScriptText(SAY_QUEST_QUEL6,me);
					events.ScheduleEvent(EVENT_QUEST_QUEL_10, 10000);
					break;    
				case EVENT_QUEST_QUEL_10:
					DoScriptText(SAY_QUEST_QUEL7,me);
					events.ScheduleEvent(EVENT_QUEST_QUEL_11, 5000);
					break;
				case EVENT_QUEST_QUEL_11:
					me->CastSpell(me, SPELL_UTHER_DESPAWN, false);
					me->DespawnOrUnsummon(2000);
					break;
				}
			}
		}
		
		private:
			EventMap events;

	};
	CreatureAI* GetAI(Creature* pCreature) const
	{
		return new npc_quest_quel_utherAI(pCreature);
	}
};

void AddSC_halls_of_reflection()
{
	new npc_jaina_or_sylvanas_hor(true, "npc_sylvanas_hor_part1");
	new npc_jaina_or_sylvanas_hor(false, "npc_jaina_hor_part1");
	new npc_jaina_and_sylvana_hor_part2();
	new npc_ghostly_priest();
	new npc_phantom_mage();
	new npc_phantom_hallucination();
	new npc_shadowy_mercenary();
	new npc_spectral_footman();
	new npc_tortured_rifleman();
	new at_hor_waves_restarter();
	new npc_frostworn_general();
	new npc_spiritual_reflection();
	new npc_capitan_gunship();
	new npc_quest_quel_dellar();
	new npc_quest_quel_uther();
}
