//  illarionserver - server for the game Illarion
//  Copyright 2011 Illarion e.V.
//
//  This file is part of illarionserver.
//
//  illarionserver is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  illarionserver is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//
//  You should have received a copy of the GNU Affero General Public License
//  along with illarionserver.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <string>

constexpr uint32_t DYNNPC_BASE = 0xFF800000;
constexpr uint32_t NPC_BASE = 0xFF000000;
constexpr uint32_t MONSTER_BASE = 0xFE000000;
constexpr auto MONSTERVIEWRANGE = 11;

constexpr auto MAXPOISONVALUE = 400;
constexpr auto MAXMANA = 10000;
constexpr auto MAXHPS = 10000;
constexpr auto MAXFOOD = 60000;
constexpr auto MAXATTRIB = 255;
constexpr auto MAXWEIGHT = 30000;

constexpr auto WAITINGVALIDATION = 1;
constexpr auto BANNED = 30;
constexpr auto BANNEDFORTIME = 31;

constexpr auto DEPOTITEM = 321;
constexpr auto DEPOTSIZE = 100;
constexpr auto BLOCKEDITEM = 228;

constexpr auto FLAG_WARPFIELD = 1;
constexpr auto FLAG_SPECIALITEM = 2;
constexpr auto FLAG_BLOCKPATH = 4;
constexpr auto FLAG_MAKEPASSABLE = 8;
constexpr auto FLAG_MONSTERONFIELD = 16;
constexpr auto FLAG_NPCONFIELD = 32;
constexpr auto FLAG_PLAYERONFIELD = 64;

// Verwendung siehe Tabelle:
// WERT|      tiles        |   tilesmoditems   |       flags        |
// ----+-------------------+-------------------+--------------------+
// 001 |                   |                   |FLAG_WARPFIELD      |
// ----+-------------------+-------------------+--------------------+
// 002 |                   |FLAG_SPECIALITEM   |FLAG_SPECIALITEM    |
// ----+-------------------+-------------------+--------------------+
// 004 |FLAG_BLOCKPATH     |FLAG_BLOCKPATH     |FLAG_BLOCKPATH      |
// ----+-------------------+-------------------+--------------------+
// 008 |                   |FLAG_MAKEPASSABLE  |FLAG_MAKEPASSABLE   |
// ----+-------------------+-------------------+--------------------+
// 016 |                   |                   |FLAG_MONSTERONFIELD |
// ----+-------------------+-------------------+--------------------+
// 032 |                   |                   |FLAG_NPCONFIELD     |
// ----+-------------------+-------------------+--------------------+
// 064 |                   |                   |FLAG_PLAYERONFIELD  |
// ----+-------------------+-------------------+--------------------+
// 128 |                   |                   |                    |
// ----+-------------------+-------------------+--------------------+

//! das Verzeichnis der Karte, relativ zum DEFAULTMUDDIR
const std::string MAPDIR("map/");

//! das Verzeichnis der Skripte, relativ zum DEFAULTMUDDIR
const std::string SCRIPTSDIR("scripts/");

//! Anzahl der maximal sichtbaren Ebenen nach Oben
constexpr auto RANGEUP = 0x02;

//! Anzahl der maximal sichtbaren Ebenen nach Unten
constexpr auto RANGEDOWN = 0x02;

//! Anzahl der Felder zwischen zwei Ebenen
constexpr auto LEVELDISTANCE = 0x03;

//! Typ der maximalen Anzahl von Item in einem Container
using MAXCOUNTTYPE = unsigned char;

//! Die maximale Anzahl von Item auf einem Feld
constexpr auto MAXITEMS = 250; // max 255 da oft als BYTE verwendet

//! die maximale Anzahl von Item am Grtel
constexpr auto MAX_BELT_SLOTS = 6;

//! Die maximale Anzahl von Item direkt am K�per
constexpr auto MAX_BODY_ITEMS = 12;

//! Rucksack
constexpr auto BACKPACK = 0;

//! Kopf
constexpr auto HEAD = 1;

//! Kopf-Flag
constexpr auto FLAG_HEAD = 1;

//! Hals
constexpr auto NECK = 2;

//! Hals-Flag
constexpr auto FLAG_NECK = 2;

//! Brustkorb
constexpr auto BREAST = 3;

//! Brustkorb-Flag
constexpr auto FLAG_BREAST = 4;

//! H�de (fr Handschuhe)
constexpr auto HANDS = 4;

//! H�de-Flag
constexpr auto FLAG_HANDS = 8;

//! Werkzeug / Waffe in der linken Hand
constexpr auto LEFT_TOOL = 5;

//! Werkzeug / Waffe in der rechten Hand
constexpr auto RIGHT_TOOL = 6;

//! Finger der linken Hand
constexpr auto FINGER_LEFT_HAND = 7;

//! Finger der rechten Hand
constexpr auto FINGER_RIGHT_HAND = 8;

//! Finger-Flag
constexpr auto FLAG_FINGER = 32;

//! Beine
constexpr auto LEGS = 9;

//! Beine-Flag
constexpr auto FLAG_LEGS = 64;

//! F�
constexpr auto FEET = 10;

//! F�-Flag
constexpr auto FLAG_FEET = 128;

//! Umhang
constexpr auto COAT = 11;

constexpr auto LAST_WEARABLE = 11;

//! Coat-Flag
constexpr auto FLAG_COAT = 16;

constexpr auto MAXSHOWCASES = 100;
constexpr auto MAX_DEPOT_SHOWCASE = 9;

//! Code fr "kein Feld"
constexpr auto NOFIELD = 0xFFFF;

//-------------- Client to Server ---------------------

//! folgender Wert ist relative x und y Koordinaten eines Items/Bodenplatte/Charakters
constexpr auto UID_KOORD = 0x01;

//! folgender Wert ist Showcasenummer+showcaseposition
constexpr auto UID_SHOWC = 0x02;

//! folgender Wert ist Inventory Position
constexpr auto UID_INV = 0x03;

//! Eine Person wird benutzt
constexpr auto UID_PERSON = 0x05;

constexpr auto UID_MAGICWAND = 0x06;

//-------------- Server to Client ---------------------

constexpr auto STILLMOVING = 0x09;
constexpr auto NOMOVE = 0x0A;
constexpr auto NORMALMOVE = 0x0B;
constexpr auto PUSH = 0x0C;
constexpr auto RUNNING = 0x0D;

//! Grund fr Verbindungsabbruch: Client logt aus
constexpr auto NORMALLOGOUT = 0x00;

//! Grund fr Verbindungsabbruch: zu alter Client
constexpr auto OLDCLIENT = 0x01;

//! Grund fr Verbindungsabbruch: Spieler ist schon online
constexpr auto DOUBLEPLAYER = 0x02;

//! Grund fr Verbindungsabbruch: Falsches Pa�ort
constexpr auto WRONGPWD = 0x03;

//! Grund fr Verbindungsabbruch: Servershutdown
constexpr auto SERVERSHUTDOWN = 0x04;

//! Grund fr Verbindungsabbruch: durch Gamemaster entfernt
constexpr auto BYGAMEMASTER = 0x05;

//! Grund fr Verbindungsabbruch: zum Erstellen eines neuen Player
constexpr auto FORCREATE = 0x06;

//! Grund fr Verbindungsabbruch: kein Platz fr den Player
constexpr auto NOPLACE = 0x07;

//! Grund fr Verbindungsabbruch: angegebener Spieler nicht gefunden
constexpr auto NOCHARACTERFOUND = 0x08;

//! Grund fr Verbindungsabbruch: Spieler wurde erstellt
constexpr auto PLAYERCREATED = 0x09; // string name

//! Grund fr Verbindungsabbruch: UNSTABLECONNECTION
constexpr auto UNSTABLECONNECTION = 0x0A; // string name

//! Reason for Connection shutdown: player has no account
constexpr auto NOACCOUNT = 0x0B;

//! Grund fr Verbindungsabbruch: no skill package chosen
constexpr auto NOSKILLS = 0x0C;

//! Grund fuer Verbindungsabbruch: Spielerdaten korrupt
constexpr auto CORRUPTDATA = 0x0D;

#endif
