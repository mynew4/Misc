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
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/* ContentData
item_nether_wraith_beacon(i31742)   Summons creatures for quest Becoming a Spellfire Tailor (q10832)
item_flying_machine(i34060, i34061)  Engineering crafted flying machines
item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
item_only_for_flight                Items which should only useable while flying
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "Player.h"

/*#####
# item_only_for_flight
#####*/

enum eOnlyForFlight
{
    SPELL_ARCANE_CHARGES    = 45072
};

class item_only_for_flight : public ItemScript
{
public:
    item_only_for_flight() : ItemScript("item_only_for_flight") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        uint32 itemId = item->GetEntry();
        bool disabled = false;

        //for special scripts
        switch (itemId)
        {
           case 24538:
                if (player->GetAreaId() != 3628)
                    disabled = true;
                    break;
           case 34489:
                if (player->GetZoneId() != 4080)
                    disabled = true;
                    break;
           case 34475:
                if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_ARCANE_CHARGES))
                    Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_ON_GROUND);
                    break;
        }

        // allow use in flight only
        if (player->isInFlight() && !disabled)
            return false;

        // error
        player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

/*#####
# item_nether_wraith_beacon
#####*/

class item_nether_wraith_beacon : public ItemScript
{
public:
    item_nether_wraith_beacon() : ItemScript("item_nether_wraith_beacon") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/)
    {
        if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()+20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);

            if (Creature* nether = player->SummonCreature(22408, player->GetPositionX(), player->GetPositionY()-20, player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 180000))
                nether->AI()->AttackStart(player);
        }
        return false;
    }
};

/*#####
# item_gor_dreks_ointment
#####*/

class item_gor_dreks_ointment : public ItemScript
{
public:
    item_gor_dreks_ointment() : ItemScript("item_gor_dreks_ointment") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
    {
        if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT &&
            targets.GetUnitTarget()->GetEntry() == 20748 && !targets.GetUnitTarget()->HasAura(32578))
            return false;

        player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

/*#####
# item_incendiary_explosives
#####*/

class item_incendiary_explosives : public ItemScript
{
public:
    item_incendiary_explosives() : ItemScript("item_incendiary_explosives") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/)
    {
        if (player->FindNearestCreature(26248, 15) || player->FindNearestCreature(26249, 15))
            return false;
        else
        {
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
            return true;
        }
    }
};

/*#####
# item_mysterious_egg
#####*/

class item_mysterious_egg : public ItemScript
{
public:
    item_mysterious_egg() : ItemScript("item_mysterious_egg") { }

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/)
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 39883, 1); // Cracked Egg
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 39883, true, Item::GenerateItemRandomPropertyId(39883));

        return true;
    }
};

/*#####
# item_disgusting_jar
#####*/

class item_disgusting_jar : public ItemScript
{
public:
    item_disgusting_jar() : ItemScript("item_disgusting_jar") {}

    bool OnExpire(Player* player, ItemTemplate const* /*pItemProto*/)
    {
        ItemPosCountVec dest;
        uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 44718, 1); // Ripe Disgusting Jar
        if (msg == EQUIP_ERR_OK)
            player->StoreNewItem(dest, 44718, true, Item::GenerateItemRandomPropertyId(44718));

        return true;
    }
};

/*#####
# item_pile_fake_furs
#####*/

enum ePileFakeFur
{
    GO_CARIBOU_TRAP_1                                      = 187982,
    GO_CARIBOU_TRAP_2                                      = 187995,
    GO_CARIBOU_TRAP_3                                      = 187996,
    GO_CARIBOU_TRAP_4                                      = 187997,
    GO_CARIBOU_TRAP_5                                      = 187998,
    GO_CARIBOU_TRAP_6                                      = 187999,
    GO_CARIBOU_TRAP_7                                      = 188000,
    GO_CARIBOU_TRAP_8                                      = 188001,
    GO_CARIBOU_TRAP_9                                      = 188002,
    GO_CARIBOU_TRAP_10                                     = 188003,
    GO_CARIBOU_TRAP_11                                     = 188004,
    GO_CARIBOU_TRAP_12                                     = 188005,
    GO_CARIBOU_TRAP_13                                     = 188006,
    GO_CARIBOU_TRAP_14                                     = 188007,
    GO_CARIBOU_TRAP_15                                     = 188008,
    GO_HIGH_QUALITY_FUR                                    = 187983,
    NPC_NESINGWARY_TRAPPER                                 = 25835
};

#define CaribouTrapsNum 15
const uint32 CaribouTraps[CaribouTrapsNum] =
{
    GO_CARIBOU_TRAP_1, GO_CARIBOU_TRAP_2, GO_CARIBOU_TRAP_3, GO_CARIBOU_TRAP_4, GO_CARIBOU_TRAP_5,
    GO_CARIBOU_TRAP_6, GO_CARIBOU_TRAP_7, GO_CARIBOU_TRAP_8, GO_CARIBOU_TRAP_9, GO_CARIBOU_TRAP_10,
    GO_CARIBOU_TRAP_11, GO_CARIBOU_TRAP_12, GO_CARIBOU_TRAP_13, GO_CARIBOU_TRAP_14, GO_CARIBOU_TRAP_15,
};

class item_pile_fake_furs : public ItemScript
{
public:
    item_pile_fake_furs() : ItemScript("item_pile_fake_furs") { }

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const & /*targets*/)
    {
        GameObject* go = NULL;
        for (uint8 i = 0; i < CaribouTrapsNum; ++i)
        {
            go = player->FindNearestGameObject(CaribouTraps[i], 5.0f);
            if (go)
                break;
        }

        if (!go)
            return false;

        if (go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, true) || go->FindNearestCreature(NPC_NESINGWARY_TRAPPER, 10.0f, false) || go->FindNearestGameObject(GO_HIGH_QUALITY_FUR, 2.0f))
            return true;

        float x, y, z;
        go->GetClosePoint(x, y, z, go->GetObjectSize() / 3, 7.0f);
        go->SummonGameObject(GO_HIGH_QUALITY_FUR, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 0, 0, 0, 0, 0, 1000);
        if (TempSummon* summon = player->SummonCreature(NPC_NESINGWARY_TRAPPER, x, y, z, go->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 1000))
        {
            summon->SetVisible(false);
            summon->SetReactState(REACT_PASSIVE);
            summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
        }
        return false;
    }
};

/*#####
# item_petrov_cluster_bombs
#####*/

enum ePetrovClusterBombs
{
    SPELL_PETROV_BOMB           = 42406,
    AREA_ID_SHATTERED_STRAITS   = 4064,
    ZONE_ID_HOWLING             = 495
};

class item_petrov_cluster_bombs : public ItemScript
{
public:
    item_petrov_cluster_bombs() : ItemScript("item_petrov_cluster_bombs") { }

    bool OnUse(Player* player, Item* /*item*/, const SpellCastTargets & /*targets*/)
    {
        if (player->GetZoneId() != ZONE_ID_HOWLING)
            return false;

        if (!player->GetTransport() || player->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
        {
            if (const SpellInfo* spellInfo = sSpellMgr->GetSpellInfo(SPELL_PETROV_BOMB))
                Spell::SendCastResult(player, spellInfo, 1, SPELL_FAILED_NOT_HERE);

            return true;
        }

        return false;
    }
};

/*######
# item_dehta_trap_smasher
# For quest 11876, Help Those That Cannot Help Themselves
######*/
enum eHelpThemselves
{
    QUEST_CANNOT_HELP_THEMSELVES                  =  11876,
    NPC_TRAPPED_MAMMOTH_CALF                      =  25850,
    GO_MAMMOTH_TRAP_1                             = 188022,
    GO_MAMMOTH_TRAP_2                             = 188024,
    GO_MAMMOTH_TRAP_3                             = 188025,
    GO_MAMMOTH_TRAP_4                             = 188026,
    GO_MAMMOTH_TRAP_5                             = 188027,
    GO_MAMMOTH_TRAP_6                             = 188028,
    GO_MAMMOTH_TRAP_7                             = 188029,
    GO_MAMMOTH_TRAP_8                             = 188030,
    GO_MAMMOTH_TRAP_9                             = 188031,
    GO_MAMMOTH_TRAP_10                            = 188032,
    GO_MAMMOTH_TRAP_11                            = 188033,
    GO_MAMMOTH_TRAP_12                            = 188034,
    GO_MAMMOTH_TRAP_13                            = 188035,
    GO_MAMMOTH_TRAP_14                            = 188036,
    GO_MAMMOTH_TRAP_15                            = 188037,
    GO_MAMMOTH_TRAP_16                            = 188038,
    GO_MAMMOTH_TRAP_17                            = 188039,
    GO_MAMMOTH_TRAP_18                            = 188040,
    GO_MAMMOTH_TRAP_19                            = 188041,
    GO_MAMMOTH_TRAP_20                            = 188042,
    GO_MAMMOTH_TRAP_21                            = 188043,
    GO_MAMMOTH_TRAP_22                            = 188044,
};

#define MammothTrapsNum 22
const uint32 MammothTraps[MammothTrapsNum] =
{
    GO_MAMMOTH_TRAP_1, GO_MAMMOTH_TRAP_2, GO_MAMMOTH_TRAP_3, GO_MAMMOTH_TRAP_4, GO_MAMMOTH_TRAP_5,
    GO_MAMMOTH_TRAP_6, GO_MAMMOTH_TRAP_7, GO_MAMMOTH_TRAP_8, GO_MAMMOTH_TRAP_9, GO_MAMMOTH_TRAP_10,
    GO_MAMMOTH_TRAP_11, GO_MAMMOTH_TRAP_12, GO_MAMMOTH_TRAP_13, GO_MAMMOTH_TRAP_14, GO_MAMMOTH_TRAP_15,
    GO_MAMMOTH_TRAP_16, GO_MAMMOTH_TRAP_17, GO_MAMMOTH_TRAP_18, GO_MAMMOTH_TRAP_19, GO_MAMMOTH_TRAP_20,
    GO_MAMMOTH_TRAP_21, GO_MAMMOTH_TRAP_22
};

class item_dehta_trap_smasher : public ItemScript
{
public:
    item_dehta_trap_smasher() : ItemScript("item_dehta_trap_smasher") { }

    bool OnUse(Player* player, Item* /*item*/, const SpellCastTargets & /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_CANNOT_HELP_THEMSELVES) != QUEST_STATUS_INCOMPLETE)
            return false;

        Creature* pMammoth = player->FindNearestCreature(NPC_TRAPPED_MAMMOTH_CALF, 5.0f);
        if (!pMammoth)
            return false;

        GameObject* pTrap = NULL;
        for (uint8 i = 0; i < MammothTrapsNum; ++i)
        {
            pTrap = player->FindNearestGameObject(MammothTraps[i], 11.0f);
            if (pTrap)
            {
                pMammoth->AI()->DoAction(1);
                pTrap->SetGoState(GO_STATE_READY);
                player->KilledMonsterCredit(NPC_TRAPPED_MAMMOTH_CALF, 0);
                return true;
            }
        }
        return false;
    }
};

enum TheEmissary
{
    QUEST_THE_EMISSARY      =   11626,
    NPC_LEVIROTH            =   26452
};

class item_trident_of_nazjan : public ItemScript
{
public:
    item_trident_of_nazjan() : ItemScript("item_Trident_of_Nazjan") { }

    bool OnUse(Player* player, Item* item, const SpellCastTargets & /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_THE_EMISSARY) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* pLeviroth = player->FindNearestCreature(NPC_LEVIROTH, 10.0f)) // spell range
            {
                pLeviroth->AI()->AttackStart(player);
                return false;
            } else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        } else
            player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};

enum eCapturedFrog
{
    QUEST_THE_PERFECT_SPIES      = 25444,
    NPC_VANIRAS_SENTRY_TOTEM     = 40187
};

class item_captured_frog : public ItemScript
{
public:
    item_captured_frog() : ItemScript("item_captured_frog") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_THE_PERFECT_SPIES) == QUEST_STATUS_INCOMPLETE)
        {
            if (player->FindNearestCreature(NPC_VANIRAS_SENTRY_TOTEM, 10.0f))
                return false;
            else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        else
            player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, item, NULL);
        return true;
    }
};
/*#####
# item_draenei_fishing_net
 #####*/

class item_draenei_fishing_net : public ItemScript
{
public:
    item_draenei_fishing_net () :
            ItemScript("item_draenei_fishing_net")
    {
    }

    //This is just a hack and should be removed from here.
    //Creature/Item are in fact created before spell are sucessfully casted, without any checks at all to ensure proper/expected behavior.
    bool OnUse (Player* pPlayer, Item* /*pItem*/, SpellCastTargets const& /*targets*/)
    {
        if (pPlayer->GetQuestStatus(9452) == QUEST_STATUS_INCOMPLETE)
        {
            if (urand(0, 99) < 35)
            {
                Creature *Murloc = pPlayer->SummonCreature(17102, pPlayer->GetPositionX(), pPlayer->GetPositionY() + 20, pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                if (Murloc)
                    Murloc->AI()->AttackStart(pPlayer);
            }
            else
            {
                ItemPosCountVec dest;
                uint32 itemId = 23614;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1);
                if (msg == EQUIP_ERR_OK)
                {
                    if (Item* item = pPlayer->StoreNewItem(dest, itemId, true))
                        pPlayer->SendNewItem(item, 1, false, true);
                }
                else
                    pPlayer->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, NULL, NULL, itemId);
            }
        }
        return false;
    }
};

/*#####
# item_flying_machine
 #####*/

class item_flying_machine : public ItemScript
{
public:
    item_flying_machine () :
            ItemScript("item_flying_machine")
    {
    }

    bool OnUse (Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
    {
        uint32 itemId = pItem->GetEntry();
        if (itemId == 34060)
            if (pPlayer->GetBaseSkillValue(SKILL_RIDING) >= 225)
                return false;

        if (itemId == 34061)
            if (pPlayer->GetBaseSkillValue(SKILL_RIDING) >= 300)
                return false;

        sLog->outDebug(LOG_FILTER_TSCR, "TSCR: Player attempt to use item %u, but did not meet riding requirement", itemId);
        pPlayer->SendEquipError(EQUIP_ERR_CANT_EQUIP_SKILL, pItem, NULL);
        return true;
    }
};

/*#####
# item_tiki_torch
 #####*/

class item_tiki_torch : public ItemScript
{
public:
    item_tiki_torch () :
            ItemScript("item_tiki_torch")
    {
    }

    bool OnUse (Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(26357) == QUEST_STATUS_INCOMPLETE)

            if (pPlayer->FindNearestCreature(42704, 15))
            {
                pPlayer->CastSpell(pPlayer, 79513, true, NULL);
                pPlayer->KilledMonsterCredit(42704, 0);
                return true;
            }
        return false;
    }
};
/*#####
# item_harvesters_gift
 #####*/
#define GHOULS 28845

/*class item_harvesters_gift : public ItemScript
{
public:
    item_harvesters_gift () :
            ItemScript("item_harvesters_gift")
    {
    }

    bool OnUse (Player* pPlayer, Item* /*pItem*, SpellCastTargets const& /*targets*)
    {
        std::list<Unit*> MinionList;
        pPlayer->GetAllMinionsByEntry(MinionList, GHOULS);

        if (pPlayer->GetQuestStatus(12698) == QUEST_STATUS_INCOMPLETE)
        {
            if (!MinionList.empty())
            {
                if (MinionList.size() < 5)
                    return false;
                else
                {
                    //This should be sent to the player as red text.
                    pPlayer->Say("You have created enough ghouls. Return to Gothik the Harvester at Death's Breach.", LANG_UNIVERSAL);
                    return true;
                }
            }
            else
                return false;
        }
        return true;
    }
};*/

/*#####
# item_kromgar_flame_thrower
#####*/

enum KromgarFlameThrower
{
    SPELL_FLAMETHROWER          = 78141,
    QUEST_ASHES_TO_ASHES        = 26010,
    GO_NORTHWATCH_TENT          = 203431,
    KC                          = 41936
};

class item_kromgar_flame_thrower : public ItemScript
{
public:
    item_kromgar_flame_thrower() : ItemScript("item_kromgar_flame_thrower") {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const & /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_ASHES_TO_ASHES) == QUEST_STATUS_INCOMPLETE)
        {
            if (GameObject* go = player->FindNearestGameObject(GO_NORTHWATCH_TENT, 15.0f))
            {
                player->CastSpell(player, SPELL_FLAMETHROWER, true, NULL);
                go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                player->KilledMonsterCredit(KC, 0);
                return true;
            }
        }
        return false;
    }
};enum eOcMote
{
    NPC_PLAGE_DISEMINATOR              = 45475,
    QUEST_POSTPONING_THE_INEVITABLE    = 27420,
    ITEM_PLAGE_DISEMINATOR_RUNE        = 61037,
};
class item_overcharged_mote : public ItemScript
{
public:
    item_overcharged_mote() : ItemScript("item_overcharged_mote") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_POSTPONING_THE_INEVITABLE) == QUEST_STATUS_INCOMPLETE)
        {
            if(player->HasItemCount(ITEM_PLAGE_DISEMINATOR_RUNE,1))
            {
                if(Creature* cr = player->FindNearestCreature(NPC_PLAGE_DISEMINATOR,5.0f))
                {
                        player->CastSpell(cr, 84973, true, NULL);
                        player->TalkedToCreature(cr->GetEntry(),cr->GetGUID());
                        cr->DisappearAndDie();
                        return true;
                }
                else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
            }
            else
                player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND_2, item, NULL);
        }
        return false;
    }
};

enum ePromoDelicacy
{
    NPC_DUNEMAUL_OGRE                = 45475,
    MAUL_EM_WITH_KIDNESS            = 24963,
};
class item_promotional_delicacy : public ItemScript
{
public:
    item_promotional_delicacy() : ItemScript("item_promotional_delicacy") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if (player->GetQuestStatus(MAUL_EM_WITH_KIDNESS) == QUEST_STATUS_INCOMPLETE)
        {
                if(Creature* cr = player->FindNearestCreature(NPC_DUNEMAUL_OGRE,10.0f))
                {
                        player->TalkedToCreature(cr->GetEntry(),cr->GetGUID());
                        cr->Attack(player,true);
                        return true;
                }
                else
                player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        return false;
    }
};

enum eAdanaTorch
{
    NPC_FIRST            = 36727,
    NPC_SECOND            = 37155,
    NPC_THIRD            = 37156,
    QUEST_FEED_OF_EVIL    = 14461,
};
class item_adana_torch : public ItemScript
{
public:
    item_adana_torch() : ItemScript("item_adana_torch") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if (player->GetQuestStatus(QUEST_FEED_OF_EVIL) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* go = player->FindNearestCreature(NPC_FIRST,8.0f))
            {
                player->KilledMonsterCredit(go->GetEntry(),go->GetGUID());
                return true;
            }
            else if(Creature* go = player->FindNearestCreature(NPC_SECOND,8.0f))
            {
                player->KilledMonsterCredit(go->GetEntry(),go->GetGUID());
                return true;
            }
            else if(Creature* go = player->FindNearestCreature(NPC_THIRD,8.0f))
            {
                player->KilledMonsterCredit(go->GetEntry(),go->GetGUID());
                return true;
            }
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        return false;
    }
};

#define NPC_STICKBONE_BERSEKER 44329
#define QUEST_THE_ENDLESS_FLOW 26922
#define ANIMUS_BUNNY 44360
class item_hand_grenade : public ItemScript
{
public:
    item_hand_grenade() : ItemScript("item_hand_grenade") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if(player->hasQuest(QUEST_THE_ENDLESS_FLOW) && player->GetQuestStatus(QUEST_THE_ENDLESS_FLOW) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* cr = player->FindNearestCreature(NPC_STICKBONE_BERSEKER,20.0f))
            {
                player->Kill(cr->ToUnit(),false);
                player->KilledMonsterCredit(cr->GetEntry(),cr->GetGUID());
                return true;
            }
            else if(Creature* cr2 = player->FindNearestCreature(ANIMUS_BUNNY,20.0f))
            {
                player->KilledMonsterCredit(cr2->GetEntry(),cr2->GetGUID());
                return true;
            }
            else
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        return false;
    }
};

class item_kyle_lunch : public ItemScript
{
public:
    item_kyle_lunch() : ItemScript("item_kyle_lunch") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if(player->hasQuest(11129) && player->GetQuestStatus(11129) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* cr = player->FindNearestCreature(23616,30.0f))
            {
                GameObject* gob = player->SummonGameObject(186265,player->GetPositionX(),player->GetPositionY(),player->GetPositionZ()+3.0,0.0f,0,0,0,0,200);
                cr->GetMotionMaster()->MoveFollow(gob->ToUnit(),0.2f,3);
                player->CompleteQuest(11129);
                gob->RemoveFromWorld();
                cr->GetMotionMaster()->MoveIdle();
                return true;
            }
            else
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        return false;
    }
};

class item_sturdy_manacles : public ItemScript
{
public:
    item_sturdy_manacles() : ItemScript("item_sturdy_manacles") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if(player->hasQuest(26118) && player->GetQuestStatus(26118) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* cr = player->FindNearestCreature(42146,20.0f))
            {
                float x = cr->GetPositionX();
                float y = cr->GetPositionY();
                float z = cr->GetPositionZ();
                cr->DisappearAndDie();
                player->SummonCreature(42153,x,y,z,0.0f);
                player->CompleteQuest(26118);
                return true;
            }
            else
            player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, item, NULL);
        }
        return false;
    }
};

class item_tabetha_torch : public ItemScript
{
public:
    item_tabetha_torch() : ItemScript("item_tabetha_torch") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(27429) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(27296) == QUEST_STATUS_INCOMPLETE)
        {
            if (pPlayer->FindNearestCreature(23751,15))
            {
                pPlayer->CastSpell(pPlayer, 42356, true, NULL);
                pPlayer->KilledMonsterCredit(23751, 0);
               return true;
            }
            if (pPlayer->FindNearestCreature(23752,15))
            {
                pPlayer->CastSpell(pPlayer, 42356, true, NULL);
                pPlayer->KilledMonsterCredit(23752, 0);
               return true;
            }
            if (pPlayer->FindNearestCreature(23753,15))
            {
                pPlayer->CastSpell(pPlayer, 42356, true, NULL);
                pPlayer->KilledMonsterCredit(23753, 0);
               return true;
            }
        }
        return false;
    }
};

class item_reinforced_canister : public ItemScript
{
public:
    item_reinforced_canister() : ItemScript("item_reinforced_canister") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(13730) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(33736,15))
            {
                pPlayer->CastSpell(pPlayer, 63565, true, NULL);
                pPlayer->KilledMonsterCredit(33736, 0);
                cr->DisappearAndDie();
               return true;
            }
            else
                pPlayer->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
        return false;
    }
};

class item_butcherbot_control_gizmo : public ItemScript
{
public:
    item_butcherbot_control_gizmo() : ItemScript("item_butcherbot_control_gizmo") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(25111) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(5429,15,false))
                if(cr && cr->isDead())
                {
                    pPlayer->CastSpell(pPlayer, 74175, true, NULL);
                    float x = cr->GetPositionX();
                    float y = cr->GetPositionY();
                    float z = cr->GetPositionZ();
                    pPlayer->SummonCreature(39696,x,y,z,0.0f);
                    pPlayer->KilledMonsterCredit(40507, 0);
                    cr->DisappearAndDie();
                    return true;
                }

        if (pPlayer->GetQuestStatus(25112) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(5419,15,false))
                if(cr && cr->isDead())
                {
                    pPlayer->CastSpell(pPlayer, 74175, true, NULL);
                    float x = cr->GetPositionX();
                    float y = cr->GetPositionY();
                    float z = cr->GetPositionZ();
                    pPlayer->SummonCreature(39696,x,y,z,0.0f);
                    pPlayer->KilledMonsterCredit(39702, 0);
                    cr->DisappearAndDie();
                    return true;
                }

        if (pPlayer->GetQuestStatus(25115) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(5426,15,false))
                if(cr && cr->isDead())
                {
                    pPlayer->CastSpell(pPlayer, 74175, true, NULL);
                    float x = cr->GetPositionX();
                    float y = cr->GetPositionY();
                    float z = cr->GetPositionZ();
                    pPlayer->SummonCreature(39696,x,y,z,0.0f);
                    pPlayer->KilledMonsterCredit(40509, 0);
                    cr->DisappearAndDie();
                    return true;
                }

        return false;
    }
};

class item_enchanted_imp_sack : public ItemScript
{
public:
    item_enchanted_imp_sack() : ItemScript("item_enchanted_imp_sack") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(28000) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(47339,5))
            {
                pPlayer->KilledMonsterCredit(47365, 0);
                cr->DisappearAndDie();
               return true;
            }
        return false;
    }
};

class item_irontree_oilcan : public ItemScript
{
public:
    item_irontree_oilcan() : ItemScript("item_irontree_oilcan") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & targets)
    {
        if (pPlayer->GetQuestStatus(28333) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(48259,5))
            {
                if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT && targets.GetUnitTarget()->GetEntry() == 48259 && !targets.GetUnitTarget()->HasAura(89829))
                {
                    pPlayer->CastSpell(pPlayer, 89829, true, NULL);
                    pPlayer->KilledMonsterCredit(48330, 0);
                    return true;
                }
                else
                    pPlayer->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, pItem, NULL);
            }
        return false;
    }
};

class item_delaris_prism : public ItemScript
{
public:
    item_delaris_prism() : ItemScript("item_delaris_prism") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(28288) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(47369,15))
            {
                pPlayer->CastSpell(pPlayer, 89622, true, NULL);
                pPlayer->KilledMonsterCredit(47369, 0);
                cr->DisappearAndDie();
               return true;
            }
        return false;
    }
};

class item_helculars_rod : public ItemScript
{
public:
    item_helculars_rod() : ItemScript("item_helculars_rod") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & targets)
    {
        if (pPlayer->GetQuestStatus(28331) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(48269,5))
            {
                if (targets.GetUnitTarget() && targets.GetUnitTarget()->GetTypeId() == TYPEID_UNIT && targets.GetUnitTarget()->GetEntry() == 48269 && !targets.GetUnitTarget()->HasAura(89824))
                {
                    pPlayer->CastSpell(pPlayer, 89824, true, NULL);
                    pPlayer->KilledMonsterCredit(48269, 0);
                    return true;
                }
                else
                    pPlayer->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, pItem, NULL);
            }
        return false;
    }
};

class item_bloodtalon_whistle : public ItemScript
{
public:
    item_bloodtalon_whistle() : ItemScript("item_bloodtalon_whistle") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(24623) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(39157,15))
            {
                pPlayer->CastSpell(pPlayer, 70874, true, NULL);
                pPlayer->KilledMonsterCredit(39157, 0);
                cr->DisappearAndDie();
               return true;
            }
        return false;
    }
};

class item_medicated_salve : public ItemScript
{
public:
    item_medicated_salve() : ItemScript("item_medicated_salve") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(13613) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(33266,5))
            {
                pPlayer->CastSpell(pPlayer, 62644, true, NULL);
                pPlayer->KilledMonsterCredit(33266, 0);
                return true;
            }
        return false;
    }
};

class item_flameseer_staff : public ItemScript
{
public:
    item_flameseer_staff() : ItemScript("item_flameseer_staff") { }

    bool OnUse(Player* pPlayer, Item* pItem, SpellCastTargets const& /*targets*/)
    {
        if(pPlayer->GetQuestStatus(25323) == QUEST_STATUS_INCOMPLETE)
        {
            if(Creature* cr = pPlayer->FindNearestCreature(38896,20))
            {
                cr->DisappearAndDie();
                pPlayer->CastSpell(pPlayer, 74723, true, NULL);
                return true;
            }
            else
            pPlayer->SendEquipError(EQUIP_ERR_OUT_OF_RANGE, pItem, NULL);
        }
        return false;
    }
};

class item_sullah_pygmy_pen : public ItemScript
{
public:
    item_sullah_pygmy_pen() : ItemScript("item_sullah_pygmy_pen") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(28351) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(51217,5))
            {
                pPlayer->CastSpell(pPlayer, 93753, true, NULL);
                pPlayer->KilledMonsterCredit(51188, 0);
                cr->DisappearAndDie();
                return true;
            }
        return false;
    }
};

class item_milly_fire_extinguisher : public ItemScript
{
public:
    item_milly_fire_extinguisher() : ItemScript("item_milly_fire_extinguisher") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(26391) == QUEST_STATUS_INCOMPLETE)
            if (Creature* cr = pPlayer->FindNearestCreature(42940,5))
            {
                pPlayer->CastSpell(pPlayer, 80199, true, NULL);
                pPlayer->KilledMonsterCredit(42940, 0);
                cr->DisappearAndDie();
                return true;
            }
        return false;
    }
};

class item_potion_of_shrouding : public ItemScript
{
public:
    item_potion_of_shrouding() : ItemScript("item_potion_of_shrouding") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(26290) == QUEST_STATUS_INCOMPLETE)
            if (pPlayer->FindNearestCreature(42655,20))
            {
                pPlayer->CastSpell(pPlayer, 79528, true, NULL);
                pPlayer->CompleteQuest(26290);
                return true;
            }
        return false;
    }
};

class item_horn_of_challenge : public ItemScript
{
public:
    item_horn_of_challenge() : ItemScript("item_horn_of_challenge") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
    {
        if(player->GetQuestStatus(25044) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(24534) == QUEST_STATUS_INCOMPLETE)
        {
            float x = player->GetPositionX();
            float y = player->GetPositionY();
            float z = player->GetPositionZ();
            player->SummonCreature(37513,x,y,z,0.0f);
            return true;
        }
        return false;
    }
};

class item_soothing_seeds : public ItemScript
{
public:
    item_soothing_seeds() : ItemScript("item_soothing_seeds") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(24566) == QUEST_STATUS_INCOMPLETE)
            if (pPlayer->FindNearestCreature(37090,30) || pPlayer->FindNearestCreature(37091,30) )
            {
                pPlayer->CastSpell(pPlayer, 70458, true, NULL);
                pPlayer->KilledMonsterCredit(37726, 0);
                return true;
            }
        return false;
    }
};

class item_orb_of_elune : public ItemScript
{
public:
    item_orb_of_elune() : ItemScript("item_orb_of_elune") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(13523) == QUEST_STATUS_INCOMPLETE)
            if (pPlayer->FindNearestCreature(32890,20))
            {
                pPlayer->CastSpell(pPlayer, 62518, true, NULL);
                pPlayer->KilledMonsterCredit(32937, 0);
               return true;
            }
        return false;
    }
};

class item_sentinel_torch : public ItemScript
{
public:
    item_sentinel_torch() : ItemScript("item_sentinel_torch") { }

    bool OnUse(Player *pPlayer, Item *pItem, SpellCastTargets const & /*targets*/)
    {
        if (pPlayer->GetQuestStatus(13509) == QUEST_STATUS_INCOMPLETE)
        {
            if (pPlayer->FindNearestGameObject(194102,15))
            {
                pPlayer->CastSpell(pPlayer, 62624, true, NULL);
                pPlayer->KilledMonsterCredit(33056, 0);
               return true;
            }
            if (pPlayer->FindNearestGameObject(194103,15))
            {
                pPlayer->CastSpell(pPlayer, 62624, true, NULL);
                pPlayer->KilledMonsterCredit(33056, 0);
               return true;
            }
            if (pPlayer->FindNearestGameObject(194104,15))
            {
                pPlayer->CastSpell(pPlayer, 62624, true, NULL);
                pPlayer->KilledMonsterCredit(33056, 0);
               return true;
            }
        }
        return false;
    }
};



void AddSC_item_scripts()

{

    new item_captured_frog();
    new item_dehta_trap_smasher();
    new item_disgusting_jar();
    new item_draenei_fishing_net;
    new item_flying_machine;
    new item_gor_dreks_ointment();
    //new item_harvesters_gift;
    new item_incendiary_explosives();
    new item_kromgar_flame_thrower();
    new item_mysterious_egg();
    new item_nether_wraith_beacon();
    new item_only_for_flight();
    new item_petrov_cluster_bombs();
    new item_pile_fake_furs();
    new item_tiki_torch();
    new item_trident_of_nazjan();
    new item_overcharged_mote();
    new item_promotional_delicacy();
    new item_adana_torch();
    new item_hand_grenade();
    new item_kyle_lunch();
    new item_sturdy_manacles();
    new item_tabetha_torch();
    new item_reinforced_canister();
    new item_butcherbot_control_gizmo();
    new item_enchanted_imp_sack();
    new item_irontree_oilcan();
    new item_delaris_prism();
    new item_helculars_rod();
    new item_bloodtalon_whistle();
    new item_medicated_salve();
    new item_flameseer_staff();
    new item_sullah_pygmy_pen();
    new item_milly_fire_extinguisher();
    new item_potion_of_shrouding();
    new item_horn_of_challenge();
    new item_soothing_seeds();
    new item_orb_of_elune();
    new item_sentinel_torch();
}
