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


#ifndef MAPEXCEPTION_HH
#define MAPEXCEPTION_HH

//! die Oberklasse aller Exception die in Map auftreten k�nnen
class MapException: public std::exception {}
;

//! wird geworfen, wenn Bereichs�berschreitungen bei der Umrechnung von logischen Koordinaten in physikalische Indizes auftreten
class Exception_CoordinateOutOfRange: public MapException {}
;

class Exception_DifferentSizes: public MapException {}
;

class Exception_DifferentOffsets: public MapException {}
;

class Exception_MapToSmall: public MapException {}
;


class Exception_FileCorrupt: public MapException {}
;

#endif
