//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Jarlac
//  Copyright (C) 2018 Retrobytes Productions
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include "exomizer.h"

void decompress(u8* source, u8* target) __z88dk_callee __naked{

	__asm

		pop af
		pop hl
		pop de

		push af
		push ix

		call _deexo

		pop ix

		ret

	__endasm;

}
