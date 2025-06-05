##-----------------------------LICENSE NOTICE------------------------------------
##  This file is part of CPCtelera: An Amstrad CPC Game Engine 
##  Copyright (C) 2018 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
##
##  This program is free software: you can redistribute it and/or modify
##  it under the terms of the GNU Lesser General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Lesser General Public License for more details.
##
##  You should have received a copy of the GNU Lesser General Public License
##  along with this program.  If not, see <http://www.gnu.org/licenses/>.
##------------------------------------------------------------------------------
############################################################################
##                        CPCTELERA ENGINE                                ##
##                 Automatic compression utilities                        ##
##------------------------------------------------------------------------##
## This file is intended for users to automate the generation of          ##
## compressed files and their inclusion in users' projects.               ##
############################################################################

## COMPRESSION EXAMPLE (Uncomment lines to use)
##

## First 3 calls to ADD2PACK_ZX7B add enemy, hero and background
## graphics (previously converted to binary data) into the 
## compressed group 'mygraphics'. After that, call to PACKZX7B
## compresses all the data and generates an array with the result
## that is placed in src/mygraphics.c & src/mygraphics.h, ready
## to be included and used by other modules.
##
#$(eval $(call ADD2PACK_ZX7B,mygraphics,gfx/enemy.bin))
#$(eval $(call ADD2PACK_ZX7B,mygraphics,gfx/hero.bin))
#$(eval $(call ADD2PACK_ZX7B,mygraphics,gfx/background.bin))
#$(eval $(call PACKZX7B,mygraphics,src/))



########################
# BINARY TO COMPRESSED #
########################


# cpctelera only
# ###

#$(eval $(call ADD2PACK_ZX7B,titlescreenfull_cpctelera,src/assets/binary_cpctelera/titlescreenfull_cpctelera.bin ))
#$(eval $(call PACKZX7B,titlescreenfull_cpctelera,src/assets/binary_compressed_cpctelera ))


# both cpctelera and wincpctelera
# ###

# images

#$(eval $(call ADD2PACK_ZX7B,menu_professor,src/assets/binary/menu_professor.bin ))
#$(eval $(call PACKZX7B,menu_professor,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,menu_text,src/assets/binary/menu_text.bin ))
$(eval $(call PACKZX7B,menu_text,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,ludic_talk_00,src/assets/binary/ludic_talk_00.bin ))
$(eval $(call PACKZX7B,ludic_talk_00,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,ludic_talk_01,src/assets/binary/ludic_talk_01.bin ))
$(eval $(call PACKZX7B,ludic_talk_01,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,ai_talk_00,src/assets/binary/ai_talk_00.bin ))
$(eval $(call PACKZX7B,ai_talk_00,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,ai_talk_01,src/assets/binary/ai_talk_01.bin ))
$(eval $(call PACKZX7B,ai_talk_01,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_0,src/assets/binary/level_local_0.bin ))
$(eval $(call PACKZX7B,level_local_0,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_1,src/assets/binary/level_local_1.bin ))
$(eval $(call PACKZX7B,level_local_1,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_2,src/assets/binary/level_local_2.bin ))
$(eval $(call PACKZX7B,level_local_2,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_3,src/assets/binary/level_local_3.bin ))
$(eval $(call PACKZX7B,level_local_3,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_4,src/assets/binary/level_local_4.bin ))
$(eval $(call PACKZX7B,level_local_4,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_5,src/assets/binary/level_local_5.bin ))
$(eval $(call PACKZX7B,level_local_5,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_6,src/assets/binary/level_local_6.bin ))
$(eval $(call PACKZX7B,level_local_6,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_7,src/assets/binary/level_local_7.bin ))
$(eval $(call PACKZX7B,level_local_7,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_8,src/assets/binary/level_local_8.bin ))
$(eval $(call PACKZX7B,level_local_8,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_9,src/assets/binary/level_local_9.bin ))
$(eval $(call PACKZX7B,level_local_9,src/assets/binary_compressed ))

$(eval $(call ADD2PACK_ZX7B,level_local_10,src/assets/binary/level_local_10.bin ))
$(eval $(call PACKZX7B,level_local_10,src/assets/binary_compressed ))


# texts

$(eval $(call ADD2PACK_EXO2,texts_levels,src/assets/binary/texts_levels.bin ))
$(eval $(call PACKEXO2,texts_levels,src/assets/binary_compressed ))


############################################################################
##              DETAILED INSTRUCTIONS AND PARAMETERS                      ##
##------------------------------------------------------------------------##
##                                                                        ##
## Macros used for compression are ADD2PACK_ZX7B and PACKZX7B:                 ##
##                                                                        ##
##	ADD2PACK_ZX7B: Adds files to packed (compressed) groups. Each call to this ##
##  		  macro will add a file to a named compressed group.          ##
##  PACKZX7B: Compresses all files in a group into a single binary and    ##
##            generates a C-array and a header to comfortably use it from ##
##            inside your code.                                           ##
##                                                                        ##
##------------------------------------------------------------------------##
##                                                                        ##
##  $(eval $(call ADD2PACK_ZX7B,<packname>,<file>))                            ##
##                                                                        ##
##		Sequentially adds <file> to compressed group <packname>. Each     ##
## call to this macro adds a new <file> after the latest one added.       ##
## packname could be any valid C identifier.                              ##
##                                                                        ##
##  Parameters:                                                           ##
##  (packname): Name of the compressed group where the file will be added ##
##  (file)    : File to be added at the end of the compressed group       ##
##                                                                        ##
##------------------------------------------------------------------------##
##                                                                        ##
##  $(eval $(call PACKZX7B,<packname>,<dest_path>))                       ##
##                                                                        ##
##		Compresses all files in the <packname> group using ZX7B algorithm ##
## and generates 2 files: <packname>.c and <packname>.h that contain a    ##
## C-array with the compressed data and a header file for declarations.   ##
## Generated files are moved to the folder <dest_path>.                   ##
##                                                                        ##
##  Parameters:                                                           ##
##  (packname) : Name of the compressed group to use for packing          ##
##  (dest_path): Destination path for generated output files              ##
##                                                                        ##
############################################################################
##                                                                        ##
## Important:                                                             ##
##  * Do NOT separate macro parameters with spaces, blanks or other chars.##
##    ANY character you put into a macro parameter will be passed to the  ##
##    macro. Therefore ...,src/sprites,... will represent "src/sprites"   ##
##    folder, whereas ...,  src/sprites,... means "  src/sprites" folder. ##
##  * You can omit parameters by leaving them empty.                      ##
##  * Parameters (4) and (5) are optional and generally not required.     ##
############################################################################
