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
##                 Automatic image conversion file                        ##
##------------------------------------------------------------------------##
## This file is intended for users to automate music conversion from      ##
## original files (like Arkos Tracker .aks) into data arrays.             ##
############################################################################

##
## NEW MACROS
##

# Default values

#$(eval $(call AKS2DATA, SET_OUTPUTS  , h s  )) { bin, h, hs, s }
#$(eval $(call AKS2DATA, SET_SFXONLY  , no   )) { yes, no       }
#$(eval $(call AKS2DATA, SET_EXTRAPAR ,      ))
#$(eval $(call AKS2DATA, CONVERT      , music.aks , array , mem_address ))

# Conversion

#$(eval $(call AKS2DATA, SET_FOLDER   , src/assets/sound/arkos1 ))
#$(eval $(call AKS2DATA, CONVERT      , ../assets/sound/arkos1/song1.aks , song1 , 0x42 ))
