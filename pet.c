//
// PvPGN YARE MOD V1.0 (Yeat Another Ragnarok Emulator) - (Server)
// Copyright (c) Project-YARE & PvPGN 2003
// www.project-yare.com
// forum.project-yare.net
// www.pvpgn.org
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// pet.c - Pet Code

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#include "core.h"
#include "mmo.h"
#include "npc.h"
#include "pet.h"

static int *init_npc_id;

int pet_init(struct map_session_data *sd)
{
	int *id,i;

	if(sd->status.pet.pet_id_as_npc == -1)
	{
		id = return_npc_current_id();
		if((*id - *init_npc_id) < MAX_NPC_PER_MAP)
		{
			sd->status.pet.pet_id_as_npc = *id;
//			*id++;
			// pet_npc_id_on_mapは初期段階では-1
			// （マップ上にまだ一度も表示されていないの意味)
			// （マップが変わればまた-1にする)
			for(i=0;i<MAX_MAP_PER_SERVER;i++)
				sd->status.pet.pet_npc_id_on_map[i] = -1;
		}
		else
		{
			// もしNPC用idが確保できなかった場合は
			// pet_id_as_npc = -2
			sd->status.pet.pet_id_as_npc = -2;
			printf("Could not give npc id for pet\n");
		}
		sd->status.pet.activity = 0;
	}

	return 0;
}

int pet_store_init_npc_id(int *id)
{
	init_npc_id = id;
	printf("INITIAL ID=%d\n",*init_npc_id);

	return 0;
}

