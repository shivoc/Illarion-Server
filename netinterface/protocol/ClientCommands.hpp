//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _CLIENT_COMMANDS_HPP_
#define _CLIENT_COMMANDS_HPP_

#include "Player.hpp"
#include "World.hpp"
#include "Character.hpp"
#include <string>
#include "types.hpp"
#include "Logger.hpp"
#include "data/Data.hpp"
#include "data/MonsterTable.hpp"
#include <boost/shared_ptr.hpp>
#include "script/LuaNPCScript.hpp"
#include "script/LuaScript.hpp"
#include "script/LuaItemScript.hpp"
#include "script/LuaMagicScript.hpp"
#include "script/LuaMonsterScript.hpp"
#include "script/LuaTileScript.hpp"
#include "script/LuaLookAtPlayerScript.hpp"
#include "netinterface/protocol/ServerCommands.hpp"
#include "netinterface/BasicClientCommand.hpp"
#include "netinterface/protocol/BBIWIServerCommands.hpp"
#include <list>

extern MonsterTable *MonsterDescriptions;
extern std::shared_ptr<LuaLookAtPlayerScript>lookAtPlayerScript;

enum clientcommands {
    C_LOGIN_TS = 0x0D, /*<login*/
    C_SCREENSIZE_TS = 0xA0,
    C_CHARMOVE_TS = 0x10,
    C_IMOVERSTART_TS = 0x20,
    C_IMOVEREND_TS = 0x2A,
    C_PSPINRSTART_TS = 0x40,
    C_PSPINREND_TS = 0x49,
    C_LOOKATMAPITEM_TS = 0xFF,
    C_LOOKATCHARACTER_TS = 0x18,
    C_USE_TS = 0xFE,
    C_CAST_TS = 0xFD,
    C_ATTACKPLAYER_TS = 0xFA,
    C_INTRODUCE_TS = 0xF6,
    C_SAY_TS = 0xF5,
    C_SHOUT_TS = 0xF4,
    C_WHISPER_TS = 0xF3,
    C_REFRESH_TS = 0xF2,
    C_LOGOUT_TS = 0xF1,
    C_LOOKINTOCONTAINERONFIELD_TS = 0xEC,
    C_LOOKINTOINVENTORY_TS = 0xEB,
    C_LOOKINTOSHOWCASECONTAINER_TS = 0xEA,
    C_CLOSECONTAINERINSHOWCASE_TS = 0xE9,
    C_DROPITEMFROMSHOWCASEONMAP_TS = 0xE8,
    C_MOVEITEMBETWEENSHOWCASES_TS = 0xE7,
    C_MOVEITEMFROMMAPINTOSHOWCASE_TS = 0xE6,
    C_MOVEITEMFROMMAPTOPLAYER_TS = 0xE5,
    C_DROPITEMFROMPLAYERONMAP_TS = 0xE4,
    C_MOVEITEMINSIDEINVENTORY_TS = 0xE3,
    C_MOVEITEMFROMSHOWCASETOPLAYER_TS = 0xE2,
    C_MOVEITEMFROMPLAYERTOSHOWCASE_TS = 0xE1,
    C_LOOKATSHOWCASEITEM_TS = 0xE0,
    C_LOOKATINVENTORYITEM_TS = 0xDF,
    C_ATTACKSTOP_TS = 0xDE,
    C_REQUESTSKILLS_TS = 0xDD,
    C_KEEPALIVE_TS = 0xD8,
    C_REQUESTAPPEARANCE_TS = 0x0E,
    C_INPUTDIALOG_TS = 0x50,
    C_MESSAGEDIALOG_TS = 0x51,
    C_MERCHANTDIALOG_TS = 0x52,
    C_SELECTIONDIALOG_TS = 0x53,
    C_CRAFTINGDIALOG_TS = 0x54
};

class InputDialogTS : public BasicClientCommand {
private:
    unsigned int dialogId;
    bool success;
    std::string input;

public:
    InputDialogTS() : BasicClientCommand(C_INPUTDIALOG_TS) {
    }

    virtual ~InputDialogTS() {};

    virtual void decodeData() {
        dialogId = getIntFromBuffer();
        success = getUnsignedCharFromBuffer() > 0;
        input = getStringFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->executeInputDialog(dialogId, success, input);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new InputDialogTS());
        return cmd;
    }
};

class MessageDialogTS : public BasicClientCommand {
private:
    unsigned int dialogId;

public:
    MessageDialogTS() : BasicClientCommand(C_MESSAGEDIALOG_TS) {
    }

    virtual ~MessageDialogTS() {};

    virtual void decodeData() {
        dialogId = getIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->executeMessageDialog(dialogId);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MessageDialogTS());
        return cmd;
    }
};

class MerchantDialogTS : public BasicClientCommand {
private:
    unsigned int dialogId;
    uint8_t result;
    uint8_t purchaseIndex;
    uint16_t purchaseAmount;
    uint8_t saleLocation;
    uint16_t saleSlot;
    uint16_t saleAmount;

public:
    MerchantDialogTS() : BasicClientCommand(C_MERCHANTDIALOG_TS) {
    }

    virtual ~MerchantDialogTS() {};

    virtual void decodeData() {
        dialogId = getIntFromBuffer();
        result = getUnsignedCharFromBuffer();

        switch (result) {
        case 0:
            break;

        case 1:
            saleLocation = getUnsignedCharFromBuffer();
            saleSlot = getShortIntFromBuffer();
            saleAmount = getShortIntFromBuffer();
            break;

        case 2:
            purchaseIndex = getUnsignedCharFromBuffer();
            purchaseAmount = getShortIntFromBuffer();
            break;
        }
    }

    void performAction(Player *player) {
        time(&(player->lastaction));

        switch (result) {
        case 0:
            player->executeMerchantDialogAbort(dialogId);
            break;

        case 1:
            player->executeMerchantDialogSell(dialogId, saleLocation, saleSlot, saleAmount);
            break;

        case 2:
            player->executeMerchantDialogBuy(dialogId, purchaseIndex, purchaseAmount);
            break;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MerchantDialogTS());
        return cmd;
    }
};

class SelectionDialogTS : public BasicClientCommand {
private:
    unsigned int dialogId;
    bool success;
    uint8_t selectedIndex;

public:
    SelectionDialogTS() : BasicClientCommand(C_SELECTIONDIALOG_TS) {
    }

    virtual ~SelectionDialogTS() {};

    virtual void decodeData() {
        dialogId = getIntFromBuffer();
        success = getUnsignedCharFromBuffer() > 0;
        selectedIndex = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->executeSelectionDialog(dialogId, success, selectedIndex);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new SelectionDialogTS());
        return cmd;
    }
};

class CraftingDialogTS : public BasicClientCommand {
private:
    unsigned int dialogId;
    uint8_t result;
    uint8_t craftIndex;
    uint8_t craftAmount;
    uint8_t craftIngredient;

public:
    CraftingDialogTS() : BasicClientCommand(C_CRAFTINGDIALOG_TS) {
    }

    virtual ~CraftingDialogTS() {};

    virtual void decodeData() {
        dialogId = getIntFromBuffer();
        result = getUnsignedCharFromBuffer();

        switch (result) {
        case 0:
            break;

        case 1:
            craftIndex = getUnsignedCharFromBuffer();
            craftAmount = getUnsignedCharFromBuffer();
            break;

        case 2:
            craftIndex = getUnsignedCharFromBuffer();
            break;

        case 3:
            craftIndex = getUnsignedCharFromBuffer();
            craftIngredient = getUnsignedCharFromBuffer();
            break;
        }
    }

    void performAction(Player *player) {
        time(&(player->lastaction));

        switch (result) {
        case 0:
            player->executeCraftingDialogAbort(dialogId);
            break;

        case 1:
            player->executeCraftingDialogCraft(dialogId, craftIndex, craftAmount);
            break;

        case 2:
            player->executeCraftingDialogLookAtCraftable(dialogId, craftIndex);
            break;

        case 3:
            player->executeCraftingDialogLookAtIngredient(dialogId, craftIndex, craftIngredient);
            break;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new CraftingDialogTS());
        return cmd;
    }
};

class RequestAppearanceTS : public BasicClientCommand {
public:
    RequestAppearanceTS() : BasicClientCommand(C_REQUESTAPPEARANCE_TS) {
    }

    virtual ~RequestAppearanceTS() {};

    virtual void decodeData() {
        id = getIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        Character *ch = World::get()->Players.findID(id);

        if (ch == NULL) {
            ch = World::get()->Monsters.findID(id);

            if (ch == NULL) {
                ch = World::get()->Npc.findID(id);
            }
        }

        //check if we found a character befor
        if (ch != NULL) {
            ch->updateAppearanceForPlayer(player, true);
        }

    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new RequestAppearanceTS());
        return cmd;
    }

    TYPE_OF_CHARACTER_ID id;
};

/**
*@ingroup Clientcommands
*player looks at a character
*/
class LookAtCharacterTS : public BasicClientCommand {
public:
    /**
    *a player attacked another char
    */
    LookAtCharacterTS() : BasicClientCommand(C_LOOKATCHARACTER_TS) {
        id = 0;
        mode = 0;
    }

    virtual ~LookAtCharacterTS() {

    }

    virtual void decodeData() {
        id = getIntFromBuffer();
        mode = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));

        if (id < MONSTER_BASE) {
            Player *pl = World::get()->Players.findID(id);

            if (pl) {
                if (lookAtPlayerScript) {
                    lookAtPlayerScript->lookAtPlayer(player, pl, mode);
                }
            }

            //code for player handling
        } else if (id >= MONSTER_BASE && id < NPC_BASE) {
            Monster *monster = World::get()->Monsters.findID(id);

            if (monster) {
                MonsterStruct mon;

                if (MonsterDescriptions->find(monster->getType(), mon)) {
                    if (mon.script && mon.script->existsEntrypoint("lookAtMonster")) {
                        mon.script->lookAtMonster(player, monster, mode);
                        return;
                    }

                    std::string german = "Das ist: " + mon.nameDe;
                    std::string english = "This is: " + mon.nameEn;

                    boost::shared_ptr<BasicServerCommand>cmd(new CharDescription(id, player->nls(german, english)));
                    player->Connection->addCommand(cmd);
                }
            }
        } else {
            NPC *npc = World::get()->Npc.findID(id);

            if (npc) {
                if (npc->getScript() && npc->getScript()->existsEntrypoint("lookAtNpc")) {
                    npc->getScript()->lookAtNpc(player, mode);
                    return;
                }

                std::string german = "Das ist: " + npc->name;
                std::string english = "This is: " + npc->name;

                boost::shared_ptr<BasicServerCommand>cmd(new CharDescription(id, player->nls(german, english)));
                player->Connection->addCommand(cmd);

            }
        }


    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookAtCharacterTS());
        return cmd;
    }

    TYPE_OF_CHARACTER_ID id;
    uint8_t mode;
};

/**
*@ingroup Clientcommands
*character casts something
*/
class CastTS : public BasicClientCommand {
public:
    CastTS() : BasicClientCommand(C_CAST_TS) {
        cid = 0;
        xc = 0;
        yc = 0;
        zc = 0;
        pos = 0;
        showcase = 0;

    }

    virtual ~CastTS() {
    };

    virtual void decodeData() {
        spellId = static_cast<unsigned long int>(getIntFromBuffer());
        cid = getUnsignedCharFromBuffer();

        switch (cid) {
        case UID_KOORD:
            xc = static_cast<short int>(getShortIntFromBuffer());
            yc = static_cast<short int>(getShortIntFromBuffer());
            zc = static_cast<short int>(getShortIntFromBuffer());
            break;

        case UID_SHOWC:
            showcase = getUnsignedCharFromBuffer();
            pos = getUnsignedCharFromBuffer();
            break;

        case UID_INV:
            pos = getUnsignedCharFromBuffer();
            break;

        case UID_MAGICWAND:
            break;
        }
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::Script) << player->name << " is casting!" << Log::end;

        bool paramOK = true;
        //CScript* skript = NULL;

        std::shared_ptr<LuaMagicScript> LuaMageScript;

        // berprfen, ob der Spieler die Runen beherrscht
        if ((spellId & player->magic.flags[ player->magic.type ]) == spellId) {
            Spell spell;
            spell.magicType = player->magic.type;
            spell.spellId = spellId;

            if (Data::Spells.exists(spell)) {
                LuaMageScript = Data::Spells.script(spell);
            }
        }

        Logger::info(LogFacility::Script) << player->name << " can't cast the spell: " << spellId << " , flags: " << player->magic.flags[ player->magic.type ] << Log::end;

        //Source des Castens zuweisen
        SouTar Source, Target;
        Source.character = dynamic_cast<Character *>(player);
        Source.pos = player->pos;
        Source.Type = LUA_CHARACTER;

        switch (cid) {
        case UID_KOORD:
            Logger::info(LogFacility::Script) << player->name << " trys to cast on a coordinate pos(" << xc << "," << yc << "," << zc << ")" << Log::end;

            if (LuaMageScript) {
                Field *temp;

                if (!World::get()->GetPToCFieldAt(temp, xc, yc, zc)) {
                    Logger::error(LogFacility::Script) << "cant find field for casting at pos(" << xc << "," << yc << "," << zc << ") !" << Log::end;
                    paramOK = false;
                } else {
                    // Feld gefunden
                    if (temp->IsPlayerOnField() || temp->IsMonsterOnField() || temp->IsNPCOnField()) {
                        Character *tmpCharacter = World::get()->findCharacterOnField(xc, yc, zc);

                        if (tmpCharacter != NULL) {
                            //Nothing to do here
                        } else {
                            Logger::debug(LogFacility::Script) << "Character found at target field!" << Log::end;
                        }

                        // Character auf Feld ist ein Spieler
                        if ((tmpCharacter->character == Character::player) && (LuaMageScript)) {
                            Logger::debug(LogFacility::Script) << "Target Character: player" << Log::end;
                            //Lua Script zuweisung
                            Target.character = tmpCharacter;
                            Target.pos = tmpCharacter->pos;
                            Target.Type = LUA_CHARACTER;
                        }
                        // Character auf Feld ist ein NPC
                        else if ((tmpCharacter->character == Character::npc) && (LuaMageScript)) {
                            Logger::debug(LogFacility::Script) << "Target Character: NPC" << Log::end;
                            //Lua Script zuweisung
                            Target.character = tmpCharacter;
                            Target.pos = tmpCharacter->pos;
                            Target.Type = LUA_CHARACTER;
                        } else if ((tmpCharacter->character == Character::monster) && (LuaMageScript)) {
                            Logger::debug(LogFacility::Script) << "Target Character: monster" << Log::end;
                            //Lua Script zuweisung
                            Target.character = tmpCharacter;
                            Target.pos = tmpCharacter->pos;
                            Target.Type = LUA_CHARACTER;
                        }
                    } else {
                        Item it;

                        if (temp->ViewTopItem(it)) {
                            Logger::debug(LogFacility::Script) << "Item found at target field!" << Log::end;

                            if (LuaMageScript) {
                                //Lua Script zuweisung
                                Target.pos = position(xc, yc, zc); //Position des TargetItems
                                Target.Type = LUA_ITEM;
                                Target.item = it;
                                Target.item.type = ScriptItem::it_field;
                                Target.item.pos = position(xc, yc, zc); //Position des TargetItems
                                Target.item.owner = player;
                            }
                        } else {
                            Logger::debug(LogFacility::Script) << "empty field!" << Log::end;

                            if (LuaMageScript) {
                                //Lua Script zuweisung
                                Target.pos = position(xc, yc, zc); //Position des TargetItems
                                Target.Type = LUA_FIELD;
                            }
                        } // Item oder leer ?
                    } // character ?
                } // Feld vorhanden ?
            } // skript != NULL ?
            else {
                std::cerr<<"LuaMageScript false, paramOK = false!"<<std::endl;
                paramOK = false;
            }

            break;

        case UID_SHOWC:

#ifdef World_DEBUG
            std::cout << "showcase: " << (int) showcase << " pos: " << (int) pos << std::endl;
#endif

            Logger::info(LogFacility::Script) << player->name << " is casting in showcas: " << showcase << " pos " << pos << Log::end;

            if (LuaMageScript) {
                if (player->isShowcaseOpen(showcase)) {
                    Container *ps = player->getShowcaseContainer(showcase);

                    if (ps != NULL) {
#ifdef World_DEBUG
                        std::cout << "Container gefunden" << std::endl;
#endif
                        ScriptItem tempi;
                        Container *tempc;

                        if (ps->viewItemNr(pos, tempi, tempc)) {
#ifdef World_DEBUG
                            std::cout << "pos gefunden" << std::endl;
#endif

                            if (LuaMageScript) {
                                Target.Type = LUA_ITEM;
                                ps->viewItemNr(pos, Target.item, tempc);
                                Target.item.pos = position(xc, yc, zc);
                                Target.item.type = ScriptItem::it_container;
                                Target.item.itempos = pos;
                                Target.item.owner = player;
                                Target.item.inside = ps;
                                Target.pos = position(xc, yc, zc);
                            }
                        } else {
                            std::cerr<<"ps->viewItemNr false, paramOK = false!"<<std::endl;
                            paramOK = false;
                        }
                    } else {
                        std::cerr<<"ps!=Null false, paramOK = false!"<<std::endl;
                        paramOK = false;
                    }
                } else {
                    std::cerr << "showcase < MAXSHOWCASE false, paramOK = false!"<<std::endl;
                    paramOK = false;
                }
            } // LuaMageScript == NULL ?

            break;

        case UID_MAGICWAND:

            //UID_MAGICWAND wird immer gesandt wenn kein Ziel gewaehlt wird.
#ifdef World_DEBUG
            std::cout << "UID_MAGICWAND" << std::endl;
#endif
            Logger::debug(LogFacility::Script) << "Cast with Wand" << Log::end;

            if ((player->attackmode) && (player->enemyid != 0) && (LuaMageScript)) {
                bool zauberstab=false;

                if ((player->characterItems[ LEFT_TOOL ].getId() != 0) && (player->characterItems[ LEFT_TOOL ].getId() != BLOCKEDITEM)) {
                    const auto weaponId = player->characterItems[LEFT_TOOL].getId();

                    if (Data::WeaponItems.exists(weaponId)) {
                        if (Data::WeaponItems[weaponId].WeaponType == 13) {
                            zauberstab = true;
                            std::cout << "Zauberstab in der Hand -> OK" << std::endl;
                        }
                    }
                }

                if ((player->characterItems[ RIGHT_TOOL ].getId() != 0) && (player->characterItems[ RIGHT_TOOL ].getId() != BLOCKEDITEM)) {
                    const auto weaponId = player->characterItems[RIGHT_TOOL].getId();

                    if (Data::WeaponItems.exists(weaponId)) {
                        if (Data::WeaponItems[weaponId].WeaponType == 13) {
                            zauberstab = true;
                            std::cout << "Zauberstab in der Hand -> OK" << std::endl;
                        }
                    }
                }


                if (zauberstab) {
                    switch (player->enemytype) {

                        //Muss spaeter angepasst werden wenn es nur noch einen Charactervektor gibt.

                    case Character::player:
#ifdef World_DEBUG
                        std::cout << "Gegner ist ein Spieler" << std::endl;
#endif

                        if (LuaMageScript) {
                            Target.Type = LUA_CHARACTER;
                            Target.character = World::get()->findCharacter(player->enemyid);

                            if (Target.character != NULL) {
                                Target.pos = Target.character->pos;
                            } else {
                                paramOK = false;
                                std::cerr << "Kein geeignetes Ziel fr Zauberstab gefunden (Target.Character == NULL)!" << std::endl;
                            }
                        }

                        break;

                    case Character::npc:
#ifdef World_DEBUG
                        std::cout << "Gegner ist ein NPC" << std::endl;
#endif

                        if (LuaMageScript) {
                            Target.Type = LUA_CHARACTER;
                            Target.character = World::get()->findCharacter(player->enemyid);

                            if (Target.character != NULL) {
                                Target.pos = Target.character->pos;
                            } else {
                                paramOK = false;
                                std::cerr << "Kein geeignetes Ziel fr Zauberstab gefunden (Target.Character == NULL)!" << std::endl;
                            }
                        }

                        break;

                    case Character::monster:
#ifdef World_DEBUG
                        std::cout << "Gegner ist ein Monster" << std::endl;
#endif

                        if (LuaMageScript) {
                            Target.Type = LUA_CHARACTER;
                            Target.character = World::get()->findCharacter(player->enemyid);

                            if (Target.character != NULL) {
                                Target.pos = Target.character->pos;
                            } else {
                                paramOK = false;
                                std::cerr << "Kein geeignetes Ziel fr Zauberstab gefunden (Target.Character == NULL)!" << std::endl;
                            }
                        }

                        break;
                    } // switch
                } // zauberstab
                else {
                    std::cout<<"Zauberstab = false, paramOK = false!"<<std::endl;
                    paramOK = false;
                }

            } // enemyid != 0
            else {

                if (!LuaMageScript) {
                    std::cout<<"LuaMageScript nicht gesetzt, paramOK = false!"<<std::endl;
                    paramOK = false;
                }
            }

            break;

        case UID_INV:

#ifdef World_DEBUG
            std::cout << "cast mit Inv" << std::endl;
#endif
            Logger::debug(LogFacility::Script) << "cast in inventory" << Log::end;

            if (LuaMageScript) {
                if (pos < (MAX_BELT_SLOTS + MAX_BODY_ITEMS)) {
#ifdef World_DEBUG
                    std::cout << "gltiger Wert" << std::endl;
#endif

                    if (player->characterItems[ pos ].getId() != 0) {
#ifdef World_DEBUG
                        std::cout << "Position " << (int) pos << " am Koerper ist besetzt" << std::endl;
#endif

                        if (LuaMageScript) {
                            Target.Type = LUA_ITEM;
                            Target.item = (ScriptItem)player->characterItems[ pos ];
                            Target.item.pos = player->pos;

                            if (pos < MAX_BODY_ITEMS) {
                                Target.item.type = ScriptItem::it_inventory;
                            } else {
                                Target.item.type = ScriptItem::it_belt;
                            }

                            Target.item.itempos = pos;
                            Target.item.owner = player;
                            Target.pos = player->pos;
                        }
                    } else {
                        std::cerr<<"cp->characterItems[pos].getId() != 0 false, paramOK = false!"<<std::endl;
                        paramOK = false;
                    }
                } else {
                    std::cerr<<"pos < (MAX_BELT_SLOTS + MAX_BODY_ITEMS), paramOK = false!"<<std::endl;
                    paramOK = false;
                }
            } // skript != NULL ?

            break;

        } // end of switch ID

        if (LuaMageScript) {
            Logger::debug(LogFacility::Script) << "try to call magic script" << Log::end;
            player->ltAction->setLastAction(LuaMageScript, Source, Target, LongTimeAction::ACTION_MAGIC);
            //std::string msg;
#ifdef World_DEBUG
            std::cout<<"paramOK: "<<paramOK<<std::endl;
#endif

            if ((paramOK) && player->IsAlive() && (player->GetStatus() < 10)) {
                switch (Target.Type) {
                case LUA_NONE:
                    LuaMageScript->CastMagic(player, static_cast<unsigned char>(LTS_NOLTACTION));
                    //msg = "Casted spell: " + Logger::toString(spellId);
                    break;

                case LUA_FIELD:
                    LuaMageScript->CastMagicOnField(player, Target.pos, static_cast<unsigned char>(LTS_NOLTACTION));
                    //msg = "Casted spell: " + Logger::toString(spellId) + " on field at pos(" + Logger::toString(Target.pos.x) + "," + Logger::toString(Target.pos.y) + "," + Logger::toString(Target.pos.z) + ")";
                    break;

                case LUA_CHARACTER:
                    LuaMageScript->CastMagicOnCharacter(player, Target.character, static_cast<unsigned char>(LTS_NOLTACTION));

                    if (Target.character->character == Character::monster) {
                        MonsterStruct monStruct;
                        Monster *temp = dynamic_cast<Monster *>(Target.character);

                        if (MonsterDescriptions->find(temp->getType(), monStruct)) {
                            if (monStruct.script) {
                                monStruct.script->onCasted(temp,player);
                            }
                        } else {
                            std::cerr<<"Didn't finde Monster Description for: "<< temp->getType() << " can't call onCasted!"<<std::endl;
                        }

                    }

                    //msg = "Casted spell: " + Logger::toString(spellId) + " on character: " + Target.character->name + "(" + Logger::toString(Target.character->id) + ")";
                    break;

                case LUA_ITEM:
                    LuaMageScript->CastMagicOnItem(player, Target.item, static_cast<unsigned char>(LTS_NOLTACTION));
                    //msg = "Casted spell: " + Logger::toString(spellId) + " on item: " + Logger::toString(Target.item.getId());
                    break;

                default:
                    LuaMageScript->CastMagic(player, static_cast<unsigned char>(LTS_NOLTACTION));
                    //msg = "Casted spell: " + Logger::toString(spellId) + " on item: " + Logger::toString(Target.item.getId());
                    break;
                } //Ende Switch

                //monitoringClientList->sendCommand( new SendActionTS( player->id, player->name, 2, msg));
            } //ENde if player->IsAlive

            Logger::debug(LogFacility::Script) << "all succeeded" << Log::end;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new CastTS());
        return cmd;

    }

    unsigned char showcase;
    unsigned char pos;
    short int xc,yc,zc;
    unsigned char cid;
    unsigned long int spellId;
};


/**
*@ingroup Clientcommands
*character uses something
*/
class UseTS : public BasicClientCommand {
public:
    UseTS() : BasicClientCommand(C_USE_TS) {
    }

    virtual ~UseTS() {
    };

    virtual void decodeData() {
        useId = getUnsignedCharFromBuffer();

        switch (useId) {
        case UID_KOORD:
            xc = getShortIntFromBuffer();
            yc = getShortIntFromBuffer();
            zc = getShortIntFromBuffer();
            break;

        case UID_SHOWC:
            showcase = getUnsignedCharFromBuffer();
            pos = getUnsignedCharFromBuffer();
            break;

        case UID_INV:
            pos = getUnsignedCharFromBuffer();
            break;
        }
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();

        Logger::debug(LogFacility::Script) << player->name << " uses something" << Log::end;

        bool paramOK = true;

        std::shared_ptr<LuaItemScript> LuaScript;
        std::shared_ptr<LuaNPCScript> LuaNPCScript;
        std::shared_ptr<LuaMonsterScript> LuaMonsterScript;
        std::shared_ptr<LuaTileScript> LuaTileScript;
        SouTar Source, Target;
        CommonStruct com;

        switch (useId) {
        case UID_KOORD:

            Field *temp;

            Logger::debug(LogFacility::Script) << "UID_KOORD" << Log::end;
            Logger::debug(LogFacility::Script) << "xc: " << static_cast<int>(xc) << " yc: " << static_cast<int>(yc) << " zc: " << static_cast<int>(zc) << Log::end;

            if (!World::get()->GetPToCFieldAt(temp, xc, yc, zc)) {
                Logger::debug(LogFacility::Script) << "Use UID_KOORD field not found!" << Log::end;
                Logger::debug(LogFacility::Script) << "Use UID_KOORD field not found at pos ( " << static_cast<int>(xc) << "," << static_cast<int>(yc) << "," << static_cast<int>(zc) << ")" << Log::end;
                paramOK = false;
            } else {
                // Feld gefunden
                //Prfen ob sich irgendeine art Char auf dem Feld befindet (Spaeter nur noch IsCharOnField vorerst noch alle Arten pruefen
                if (temp->IsPlayerOnField() || temp->IsNPCOnField() || temp->IsMonsterOnField()) {
                    Logger::debug(LogFacility::Script) << "Character on field found!" << Log::end;
                    Character *tmpCharacter = World::get()->findCharacterOnField(xc, yc, zc);

                    if (tmpCharacter != NULL) {
                        if (tmpCharacter->character == Character::player) {
                            Logger::debug(LogFacility::Script) << "Character is a player!" << Log::end;
                        } else if (tmpCharacter->character == Character::npc) {
                            Logger::debug(LogFacility::Script) << "Character is a NPC!" << Log::end;

                            NPC *scriptNPC = dynamic_cast<NPC *>(tmpCharacter);
                            LuaNPCScript = scriptNPC->getScript();

                            if (LuaNPCScript) {
                                Source.pos = scriptNPC->pos;
                                Source.character = scriptNPC;
                                Source.Type = LUA_CHARACTER;
                            }
                        } else if (tmpCharacter->character == Character::monster) {
                            Logger::debug(LogFacility::Script) << "Character is a monster!" << Log::end;

                            Monster *scriptMonster = dynamic_cast<Monster *>(tmpCharacter);
                            MonsterStruct monStruct;

                            if (MonsterDescriptions->find(scriptMonster->getType(),monStruct)) {
                                LuaMonsterScript = monStruct.script;
                            } else {
                                Logger::error(LogFacility::Script) << "try to use Monster but id: " << scriptMonster->getType() << " not found in database!" << Log::end;
                            }

                            if (LuaMonsterScript) {
                                Source.pos = scriptMonster->pos;
                                Source.character = scriptMonster;
                                Source.Type = LUA_CHARACTER;
                            }
                        }
                    } else {
                        Logger::error(LogFacility::Script) << "Character on field (" << xc << "," << yc << "," << zc << ") not found!" << Log::end;
                    }
                } else {
                    Logger::debug(LogFacility::Script) << "no character on field!" << Log::end;
                    Item it;

                    if (temp->ViewTopItem(it)) {
                        Logger::debug(LogFacility::Script) << "Item on field" << Log::end;

                        LuaScript = Data::CommonItems.script(it.getId());

                        if (LuaScript) {
                            Source.Type = LUA_ITEM;
                            temp->ViewTopItem(Source.item);
                            Source.item.pos = position(xc, yc, zc); //Position des SourceItems
                            Source.item.type = ScriptItem::it_field; //Position des SourceItems
                            Source.item.owner = player; //Owner des Items
                            Source.pos = position(xc, yc, zc);
                        }
                    } else {
                        Logger::debug(LogFacility::Script) << "empty field!" << Log::end;

                        auto &script = Data::Tiles.script(temp->getTileId());

                        if (script) {
                            LuaTileScript = script;
                            Source.Type = LUA_FIELD;
                            Source.pos = position(xc, yc, zc);
                        }
                    }
                }
            }

            break;

        case UID_SHOWC:
            Logger::debug(LogFacility::Script) << "showcase: " << static_cast<int>(showcase) << " pos: " << static_cast<int>(pos) << Log::end;

            if (player->isShowcaseOpen(showcase)) {
                Container *ps = player->getShowcaseContainer(showcase);

                if (ps != NULL) {
                    Logger::debug(LogFacility::Script) << "Container gefunden!" << Log::end;
                    ScriptItem tempi;
                    Container *tempc;

                    if (ps->viewItemNr(pos, tempi, tempc)) {
                        Logger::debug(LogFacility::Script) << "pos found item id: " << tempi.getId() << Log::end;

                        LuaScript = Data::CommonItems.script(tempi.getId());

                        if (LuaScript) {
                            Source.Type = LUA_ITEM;
                            ps->viewItemNr(pos, Source.item, tempc);
                            Source.item.pos = position(xc, yc, zc);
                            Source.item.type = ScriptItem::it_container;
                            Source.item.itempos = pos;
                            Source.item.owner = player;
                            Source.item.inside = ps;
                            Source.pos = position(xc, yc, zc);
                        }
                    } else {
                        paramOK = false;
                    }
                } else {
                    paramOK = false;
                }
            } else {
                paramOK = false;
            }

            break;

        case UID_INV:

            if (pos < (MAX_BELT_SLOTS + MAX_BODY_ITEMS)) {
                Logger::debug(LogFacility::Script) << "position approved!" << Log::end;

                if (player->characterItems[ pos ].getId() != 0) {
                    Logger::debug(LogFacility::Script) << "at position " << static_cast<int>(pos) << " on body, is an item with id: " << player->characterItems[ pos ].getId() << Log::end;

                    LuaScript = Data::CommonItems.script(player->characterItems[ pos ].getId()) ;

                    if (LuaScript) {
                        Source.Type = LUA_ITEM;
                        Source.item = (ScriptItem)player->characterItems[ pos ];
                        Source.item.pos = player->pos;

                        if (pos < MAX_BODY_ITEMS) {
                            Source.item.type = ScriptItem::it_inventory;
                        } else {
                            Source.item.type = ScriptItem::it_belt;
                        }

                        Source.item.itempos = pos;
                        Source.item.owner = player;
                        Source.pos = player->pos;
                    }
                } else {
                    paramOK = false;
                }
            } else {
                paramOK = false;
            }

            break;

        default:
            paramOK = false;
            break;
        }

        Logger::debug(LogFacility::Script) << "=========Use Script Start=============" << Log::end;
        Logger::debug(LogFacility::Script) << "Source pos (" << Source.pos.x << "," << Source.pos.y << "," << Source.pos.z << ")" << Log::end;
        Logger::debug(LogFacility::Script) << "Source type: " << Source.Type << Log::end;
        Logger::debug(LogFacility::Script) << "Source Character: " << *Source.character << Log::end;
        Logger::debug(LogFacility::Script) << "Target pos (" << Target.pos.x << "," << Target.pos.y << "," << Target.pos.z << ")" << Log::end;
        Logger::debug(LogFacility::Script) << "Target Type: " << Target.Type << Log::end;
        Logger::debug(LogFacility::Script) << "Target Character: " << *Target.character << Log::end;
        Logger::debug(LogFacility::Script) << "==========Use Script End=============" << Log::end;

        std::string msg;

        if (LuaScript) {
            player->ltAction->setLastAction(LuaScript, Source, Target, LongTimeAction::ACTION_USE);

            if ((paramOK) && player->IsAlive()) {

                if (Source.Type == LUA_ITEM) {
                    LuaScript->UseItem(player, Source.item, static_cast<unsigned char>(LTS_NOLTACTION));
                    msg = "Used Item: " + boost::lexical_cast<std::string>(Source.item.getId()) + " with item: " + boost::lexical_cast<std::string>(Target.item.getId());
                }
            }
        } else if (LuaNPCScript) {
            player->ltAction->setLastAction(LuaNPCScript, Source, Target, LongTimeAction::ACTION_USE);

            if ((paramOK) && player->IsAlive()) {
                if (Source.Type == LUA_CHARACTER && (Target.Type == LUA_NONE)) {
                    LuaNPCScript->useNPC(player, static_cast<unsigned char>(LTS_NOLTACTION));
                    msg = "Used NPC: " + Source.character->name + "(" + boost::lexical_cast<std::string>(Source.character->id) + ")";
                }
            }

        } else if (LuaMonsterScript) {
            player->ltAction->setLastAction(LuaMonsterScript, Source, Target, LongTimeAction::ACTION_USE);

            if ((paramOK) && player->IsAlive()) {
                if (Source.Type == LUA_CHARACTER && (Target.Type == LUA_NONE)) {
                    LuaMonsterScript->useMonster(Source.character, player, static_cast<unsigned char>(LTS_NOLTACTION));
                    msg = "Used Monster: " + Source.character->name + "(" + boost::lexical_cast<std::string>(Source.character->id) + ")";
                }
            }
        } else if (LuaTileScript) {
            player->ltAction->setLastAction(LuaTileScript, Source, Target, LongTimeAction::ACTION_USE);

            if ((paramOK) && player->IsAlive()) {
                if (Source.Type == LUA_FIELD && Target.Type == LUA_NONE) {
                    LuaTileScript->useTile(player, Source.pos, static_cast<unsigned char>(LTS_NOLTACTION));
                }
            }
        }

        boost::shared_ptr<BasicServerCommand>cmd(new BBSendActionTC(player->id, player->name, 3,msg));
        World::get()->monitoringClientList->sendCommand(cmd);

    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new UseTS());
        return cmd;
    }

    unsigned char useId;
    unsigned char showcase;
    unsigned char pos;
    short int xc;
    short int yc;
    short int zc;
};

/**
*@ingroup Clientcommands
*client gives signal that he is ok
*/
class KeepAliveTS : public BasicClientCommand {
public:
    KeepAliveTS() : BasicClientCommand(C_KEEPALIVE_TS) {
    }

    virtual ~KeepAliveTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        Logger::debug(LogFacility::Player) << "KEEPALIVE_TS from player " << player->name << Log::end;
        time(&(player->lastkeepalive));
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new KeepAliveTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*client asked to send his skills
*/
class RequestSkillsTS : public BasicClientCommand {
public:
    RequestSkillsTS() : BasicClientCommand(C_REQUESTSKILLS_TS) {
    }

    virtual ~RequestSkillsTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->sendAllSkills();
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new RequestSkillsTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char stopps an attack
*/
class AttackStopTS : public BasicClientCommand {
public:
    AttackStopTS() : BasicClientCommand(C_ATTACKSTOP_TS) {
    }

    virtual ~AttackStopTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        player->attackmode = false;
        //hast to be changed to a command
        boost::shared_ptr<BasicServerCommand>cmd(new TargetLostTC());
        player->Connection->addCommand(cmd);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new AttackStopTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char looks at an item in his inventory
*/
class LookAtInventoryItemTS : public BasicClientCommand {
public:
    LookAtInventoryItemTS() : BasicClientCommand(C_LOOKATINVENTORYITEM_TS) {
    }

    virtual ~LookAtInventoryItemTS() {};

    virtual void decodeData() {
        pos = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        Logger::debug(LogFacility::World) << player->name << " looks at an item in the inventory." << Log::end;
        time(&(player->lastaction));

        if (player->IsAlive()) {
            World::get()->lookAtInventoryItem(player, pos);
            player->actionPoints -= P_LOOK_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookAtInventoryItemTS());
        return cmd;
    }
    unsigned char pos;
};

/**
*@ingroup Clientcommands
*char looks at an item in his showcase
*/
class LookAtShowCaseItemTS : public BasicClientCommand {
public:
    LookAtShowCaseItemTS() : BasicClientCommand(C_LOOKATSHOWCASEITEM_TS) {
    }

    virtual ~LookAtShowCaseItemTS() {};

    virtual void decodeData() {
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        Logger::debug(LogFacility::World) << player->name << " looks at an item in a container." << Log::end;
        time(&(player->lastaction));

        if (player->IsAlive()) {
            World::get()->lookAtShowcaseItem(player, showcase, pos);
            player->actionPoints -= P_LOOK_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookAtShowCaseItemTS());
        return cmd;
    }
    unsigned char showcase;
    unsigned char pos;
};

/**
*@ingroup Clientcommands
*char moves item from inventory to a showcase
*/
class MoveItemFromPlayerToShowCaseTS : public BasicClientCommand {
public:
    MoveItemFromPlayerToShowCaseTS() : BasicClientCommand(C_MOVEITEMFROMPLAYERTOSHOWCASE_TS) {
    }

    virtual ~MoveItemFromPlayerToShowCaseTS() {};

    virtual void decodeData() {
        cpos = getUnsignedCharFromBuffer();
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << "moves an item from the inventory to showcase!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemFromPlayerIntoShowcase(player, cpos, showcase, pos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemFromPlayerToShowCaseTS());
        return cmd;
    }
    unsigned char showcase;
    unsigned char pos;
    unsigned char cpos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char moves item from showcase to his inventory
*/
class MoveItemFromShowCaseToPlayerTS : public BasicClientCommand {
public:
    MoveItemFromShowCaseToPlayerTS() : BasicClientCommand(C_MOVEITEMFROMSHOWCASETOPLAYER_TS) {
    }

    virtual ~MoveItemFromShowCaseToPlayerTS() {};

    virtual void decodeData() {
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
        cpos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " moves an item from the shocase to the inventory!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemFromShowcaseToPlayer(player, showcase, pos, cpos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemFromShowCaseToPlayerTS());
        return cmd;
    }
    unsigned char showcase;
    unsigned char pos;
    unsigned char cpos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char moves items inside his inventory
*/
class MoveItemInsideInventoryTS : public BasicClientCommand {
public:
    MoveItemInsideInventoryTS() : BasicClientCommand(C_MOVEITEMINSIDEINVENTORY_TS) {
    }

    virtual ~MoveItemInsideInventoryTS() {};

    virtual void decodeData() {
        opos = getUnsignedCharFromBuffer();
        npos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << "moves an item inside the inventory!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemBetweenBodyParts(player, opos, npos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemInsideInventoryTS());
        return cmd;
    }
    unsigned char opos;
    unsigned char npos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char droppes a item from his inventory to the map
*/
class DropItemFromInventoryOnMapTS : public BasicClientCommand {
public:
    DropItemFromInventoryOnMapTS() : BasicClientCommand(C_DROPITEMFROMPLAYERONMAP_TS) {
    }

    virtual ~DropItemFromInventoryOnMapTS() {};

    virtual void decodeData() {
        pos = getUnsignedCharFromBuffer();
        xc = static_cast<short int>(getShortIntFromBuffer());
        yc = static_cast<short int>(getShortIntFromBuffer());
        zc = static_cast<short int>(getShortIntFromBuffer());
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " throws an item from inventory on the map!" << Log::end;
        World::get()->dropItemFromPlayerOnMap(player, pos, xc, yc, zc, count);
        player->actionPoints -= P_ITEMMOVE_COST;
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new DropItemFromInventoryOnMapTS());
        return cmd;
    }
    unsigned char pos;
    short int xc,yc,zc;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char moves item from map to his inventory
*/
class MoveItemFromMapToPlayerTS : public BasicClientCommand {
public:
    MoveItemFromMapToPlayerTS() : BasicClientCommand(C_MOVEITEMFROMMAPTOPLAYER_TS) {
    }

    virtual ~MoveItemFromMapToPlayerTS() {};

    virtual void decodeData() {
        dir = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " moves an Item from the map to the inventory!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemFromMapToPlayer(player, dir, pos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemFromMapToPlayerTS());
        return cmd;
    }
    unsigned char dir;
    unsigned char pos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char moves item from map into a open showcase
*/
class MoveItemFromMapIntoShowCaseTS : public BasicClientCommand {
public:
    MoveItemFromMapIntoShowCaseTS() : BasicClientCommand(C_MOVEITEMFROMMAPINTOSHOWCASE_TS) {
    }

    virtual ~MoveItemFromMapIntoShowCaseTS() {};

    virtual void decodeData() {
        dir = getUnsignedCharFromBuffer();
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " moves an item from the map to the showcase!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemFromMapIntoShowcase(player, dir, showcase, pos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemFromMapIntoShowCaseTS());
        return cmd;
    }
    unsigned char dir;
    unsigned char showcase;
    unsigned char pos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char moves item between two showcases
*/
class MoveItemBetweenShowCasesTS : public BasicClientCommand {
public:
    MoveItemBetweenShowCasesTS() : BasicClientCommand(C_MOVEITEMBETWEENSHOWCASES_TS) {
    }

    virtual ~MoveItemBetweenShowCasesTS() {};

    virtual void decodeData() {
        source = getUnsignedCharFromBuffer();
        spos = getUnsignedCharFromBuffer();
        dest = getUnsignedCharFromBuffer();
        dpos = getUnsignedCharFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " moves an item between showcases!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItemBetweenShowcases(player, source, spos, dest, dpos, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new MoveItemBetweenShowCasesTS());
        return cmd;
    }
    unsigned char source;
    unsigned char spos;
    unsigned char dest;
    unsigned char dpos;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char droppes a item from a showcase to the map
*/
class DropItemFromShowCaseOnMapTS : public BasicClientCommand {
public:
    DropItemFromShowCaseOnMapTS() : BasicClientCommand(C_DROPITEMFROMSHOWCASEONMAP_TS) {
    }

    virtual ~DropItemFromShowCaseOnMapTS() {};

    virtual void decodeData() {
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
        xc = static_cast<short int>(getShortIntFromBuffer());
        yc = static_cast<short int>(getShortIntFromBuffer());
        zc = static_cast<short int>(getShortIntFromBuffer());
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " moves an item from showcase to the map!" << Log::end;
        World::get()->dropItemFromShowcaseOnMap(player, showcase, pos, xc, yc, zc, count);
        player->actionPoints -= P_ITEMMOVE_COST;
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new DropItemFromShowCaseOnMapTS());
        return cmd;
    }
    unsigned char showcase;
    unsigned char pos;
    short int xc,yc,zc;
    unsigned short count;
};

/**
*@ingroup Clientcommands
*char closes a showcase container
*/
class CloseContainerInShowCaseTS : public BasicClientCommand {
public:
    CloseContainerInShowCaseTS() : BasicClientCommand(C_CLOSECONTAINERINSHOWCASE_TS) {
    }

    virtual ~CloseContainerInShowCaseTS() {};

    virtual void decodeData() {
        showcase = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " closes a container in the showcase" << Log::end;

        if (player->IsAlive()) {
            World::get()->closeContainerInShowcase(player, showcase);
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new CloseContainerInShowCaseTS());
        return cmd;
    }
    unsigned char showcase;
};

/**
*@ingroup Clientcommands
*char looks into a showcase container
*/
class LookIntoShowCaseContainerTS : public BasicClientCommand {
public:
    LookIntoShowCaseContainerTS() : BasicClientCommand(C_LOOKINTOSHOWCASECONTAINER_TS) {
    }

    virtual ~LookIntoShowCaseContainerTS() {};

    virtual void decodeData() {
        showcase = getUnsignedCharFromBuffer();
        pos = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " looks into a container in a showcase!" << Log::end;
        World::get()->lookIntoShowcaseContainer(player, showcase, pos);
        player->actionPoints -= P_LOOK_COST;
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookIntoShowCaseContainerTS());
        return cmd;
    }
    unsigned char showcase;
    unsigned char pos;
};

/**
*@ingroup Clientcommands
*char looks into the inventory
*/
class LookIntoInventoryTS : public BasicClientCommand {
public:
    LookIntoInventoryTS() : BasicClientCommand(C_LOOKINTOINVENTORY_TS) {
    }

    virtual ~LookIntoInventoryTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " looks into his backpack" << Log::end;
        World::get()->lookIntoBackPack(player);
        player->actionPoints -= P_LOOK_COST;
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookIntoInventoryTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char looks into a container on a map field
*/
class LookIntoContainerOnFieldTS : public BasicClientCommand {
public:
    LookIntoContainerOnFieldTS() : BasicClientCommand(C_LOOKINTOCONTAINERONFIELD_TS) {
    }

    virtual ~LookIntoContainerOnFieldTS() {};

    virtual void decodeData() {
        direction = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " looks into a container on the map" << Log::end;

        if (player->IsAlive()) {
            World::get()->lookIntoContainerOnField(player, direction);
            player->actionPoints -= P_LOOK_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookIntoContainerOnFieldTS());
        return cmd;
    }
    unsigned char direction;
};

/**
*@ingroup Clientcommands
*char logged out
*/
class LogOutTS : public BasicClientCommand {
public:
    /**
    *a client logged out
    */
    LogOutTS() : BasicClientCommand(C_LOGOUT_TS) {
    }

    virtual ~LogOutTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        player->ltAction->abortAction();
        Logger::info(LogFacility::Player) << player->name << " logt aus" << Log::end;
        player->Connection->closeConnection();
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LogOutTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char talks to another one
*/
class WhisperTS : public BasicClientCommand {
public:
    /**
    *a player talked
    */
    WhisperTS() : BasicClientCommand(C_WHISPER_TS) {
    }

    virtual ~WhisperTS() {};

    virtual void decodeData() {
        text = getStringFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        Logger::debug(LogFacility::World) << player->name << " whispers something!" << Log::end;
        player->talk(Character::tt_whisper, text);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new WhisperTS());
        return cmd;
    }
    std::string text;
};

/**
*@ingroup Clientcommands
*char talks to another one
*/
class ShoutTS : public BasicClientCommand {
public:
    /**
    *a player talked
    */
    ShoutTS() : BasicClientCommand(C_SHOUT_TS) {
    }

    virtual ~ShoutTS() {};

    virtual void decodeData() {
        text = getStringFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->talk(Character::tt_yell, text);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new ShoutTS());
        return cmd;
    }
    std::string text;
};

/**
*@ingroup Clientcommands
*char talks to another one
*/
class SayTS : public BasicClientCommand {
public:
    /**
    *a player talked
    */
    SayTS() : BasicClientCommand(C_SAY_TS) {
    }

    virtual ~SayTS() {};

    virtual void decodeData() {
        text = getStringFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        Logger::debug(LogFacility::World) << player->name << " whispers something!" << Log::end;

        if (!World::get()->parseGMCommands(player, text)) {
            if (!World::get()->parsePlayerCommands(player, text)) {    // did we issue a player command?
                player->talk(Character::tt_say, text);
            }
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new SayTS());
        return cmd;
    }
    std::string text;
};

/**
*@ingroup Clientcommands
*char wants a clientupdate for the map
*/
class RefreshTS : public BasicClientCommand {
public:
    /**
    *a clientupdate should be sendet to the client
    */
    RefreshTS() : BasicClientCommand(C_REFRESH_TS) {
    }

    virtual ~RefreshTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        Logger::debug(LogFacility::World) << player->name << " want sended a refresh_ts, sending map!" << Log::end;
        //update the current mapview
        player->sendFullMap();
        World::get()->sendAllVisibleCharactersToPlayer(player, true);
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new RefreshTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char introduces to another one
*/
class IntroduceTS : public BasicClientCommand {
public:
    /**
    *a player attacked another char
    */
    IntroduceTS() : BasicClientCommand(C_INTRODUCE_TS) {
    }

    virtual ~IntroduceTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        Logger::debug(LogFacility::World) << player->name << " introduces himself!" << Log::end;

        if (player->IsAlive()) {
            World::get()->introduceMyself(player);
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new IntroduceTS());
        return cmd;
    }
};

/**
*@ingroup Clientcommands
*char attacks another char
*/
class AttackPlayerTS : public BasicClientCommand {
public:
    /**
    *a player attacked another char
    */
    AttackPlayerTS() : BasicClientCommand(C_ATTACKPLAYER_TS) {

    }

    virtual ~AttackPlayerTS() {};

    virtual void decodeData() {
        enemyid = getIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));

        if (player->IsAlive() && player->GetStatus() < 10) {
            player->ltAction->abortAction();

            if (player->IsAlive()) {
                player->enemyid = enemyid;
                player->attackmode = true;
                player->enemytype = Character::player;

                if (player->enemyid >= MONSTER_BASE) {
                    player->enemytype = Character::monster;
                }

                if (player->enemyid >= NPC_BASE) {
                    player->enemytype = Character::npc;
                }

                boost::shared_ptr<BasicServerCommand>cmd(new AttackAcknowledgedTC());
                player->Connection->addCommand(cmd);
                //monitoringClientList->sendCommand( new SendActionTS(player->id, player->name, 0, "Starts an attack: " + Logger::toString(player->enemyid) ) );
                World::get()->characterAttacks(player);
            } else {
                player->attackmode = false;
            }
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new AttackPlayerTS());
        return cmd;
    }
    uint32_t enemyid;

};

/**
*@ingroup Clientcommands
*char looks at a map item
*/
class LookAtMapItemTS : public BasicClientCommand {
public:
    /**
    *a spin occured special handling to get direction which is encoded in the definition byte
    */
    LookAtMapItemTS() : BasicClientCommand(C_LOOKATMAPITEM_TS) {
    }

    virtual ~LookAtMapItemTS() {};

    virtual void decodeData() {
        x = getShortIntFromBuffer();
        y = getShortIntFromBuffer();
        z = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        Logger::debug(LogFacility::World) << player->name << " looks at a map item." << Log::end;

        if (player->IsAlive()) {
            World::get()->lookAtMapItem(player, x, y, z);
            player->actionPoints -= P_LOOK_COST;
        }

    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LookAtMapItemTS());
        return cmd;
    }
    short int x,y,z;

};

/**
*@ingroup Clientcommands
*char starts to spin around
*/
class PSpinActionTS : public BasicClientCommand {
public:
    /**
    *a spin occured special handling to get direction which is encoded in the definition byte
    */
    PSpinActionTS(uint8_t dir) : BasicClientCommand(C_PSPINRSTART_TS) {
        direction = dir;
    }

    virtual ~PSpinActionTS() {};

    virtual void decodeData() {
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << "Player changes his dircetion: " << player->name << " temp: " << direction << Log::end;

        if (World::get()->spinPlayer(player, direction)) {
            player->actionPoints -= P_SPIN_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new PSpinActionTS(direction));
        return cmd;
    }
    uint8_t direction;
};


/**
*@ingroup Clientcommands
*a char move has occured
*/
class CharMoveTS : public BasicClientCommand {
public:
    /**
    *a character has started moving
    */
    CharMoveTS() : BasicClientCommand(C_CHARMOVE_TS) {
    }

    virtual ~CharMoveTS() {};

    virtual void decodeData() {
        charid = getIntFromBuffer();
        direction = getUnsignedCharFromBuffer();
        mode = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));

        if (charid == player->id && (mode == NORMALMOVE || mode == RUNNING)) {
            player->ltAction->abortAction();
            Logger::debug(LogFacility::World) << "Playermove from Player: " << player->name << Log::end;

            if (player->getTurtleActive() && player->hasGMRight(gmr_settiles) && mode == NORMALMOVE) {
                World::get()->setNextTile(player, player->getTurtleTile());
                Logger::info(LogFacility::World) << "Turtle was active, new tile set at pos: " << player->pos.x << "," << player->pos.y << "," << player->pos.z << " tile: " << player->getTurtleTile() << Log::end;
            }

            if (player->move(static_cast<Character::direction>(direction), mode)) {
                player->closeAllShowcasesOfMapContainers();
            }
        } else if (mode == PUSH) {
            player->ltAction->abortAction();
            Logger::debug(LogFacility::World) << "Player pushes another: " << player->name << Log::end;

            if (player->IsAlive()) {
                if (World::get()->pushCharacter(player, charid, direction)) {
                    player->actionPoints -= P_PUSH_COST;
                }
            }
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new CharMoveTS());
        return cmd;
    }

    TYPE_OF_CHARACTER_ID charid;
    unsigned char direction;
    unsigned char mode;
};


/**
*@ingroup Clientcommands
*char starts to move a item
*/
class IMoverActionTS : public BasicClientCommand {
public:
    /**
    *a item move occured special handling to get direction which is encoded in the definition byte
    */
    IMoverActionTS(uint8_t dir) : BasicClientCommand(C_IMOVERSTART_TS) {
        direction = dir;
    }

    virtual ~IMoverActionTS() {};

    virtual void decodeData() {
        xc = getShortIntFromBuffer();
        yc = getShortIntFromBuffer();
        zc = getShortIntFromBuffer();
        count = getShortIntFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
        player->ltAction->abortAction();
        Logger::debug(LogFacility::World) << player->name << " tryes to move an Item!" << Log::end;

        if (player->IsAlive()) {
            World::get()->moveItem(player, direction, xc, yc, zc, count);
            player->actionPoints -= P_ITEMMOVE_COST;
        }
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new IMoverActionTS(direction));
        return cmd;
    }
    int16_t xc, yc, zc;
    uint16_t count;
    uint8_t direction;
};


//=========================================Login Command=========================
/**
*@ingroup Clientcommands
*a player tries to login
*/
class LoginCommandTS : public BasicClientCommand {
public:
    LoginCommandTS() : BasicClientCommand(C_LOGIN_TS) {
    }

    virtual ~LoginCommandTS() {};

    virtual void decodeData() {
        clientVersion = getUnsignedCharFromBuffer();
        loginName = getStringFromBuffer();
        passwort = getStringFromBuffer();
    }

    void performAction(Player *player) {
        time(&(player->lastaction));
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new LoginCommandTS());
        return cmd;
    }

    unsigned short clientVersion;
    std::string loginName;
    std::string passwort;
};
//==============================End Login Command================================

//=========================================Screen Size Command=========================
/**
*@ingroup Clientcommands
*a player tries to login
*/
class ScreenSizeCommandTS : public BasicClientCommand {
public:
    ScreenSizeCommandTS() : BasicClientCommand(C_SCREENSIZE_TS) {
    }

    virtual ~ScreenSizeCommandTS() {};

    virtual void decodeData() {
        width = getUnsignedCharFromBuffer();
        height = getUnsignedCharFromBuffer();
    }

    void performAction(Player *player) {
        // Set screen width and screen height
        player->screenwidth = width;
        player->screenheight = height;
        player->sendFullMap();
        player->sendCharacters();
    }

    boost::shared_ptr<BasicClientCommand> clone() {
        boost::shared_ptr<BasicClientCommand>cmd(new ScreenSizeCommandTS());
        return cmd;
    }

    uint8_t width;
    uint8_t height;
};
//==============================End Screen Size Command================================

#endif
