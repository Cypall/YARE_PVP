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
// 01a7.c - Pet things

{
	
	#define add_block_npc(m,n) {add_block(&map_data[m].npc[n]->block,m,map_data[m].npc[n]->x,map_data[m].npc[n]->y); \
	map_data[m].npc[n]->block.type=BL_NPC;}


	if((sd->status.pet.activity == 0)&&(sd->status.pet.pet_id_as_npc != -1)&&(map_data[sd->mapno].npc_num < MAX_NPC_PER_MAP))
	{
		int npc,name_id;
		unsigned char buf[256];

		
		--p->inventory[RFIFOW(fd,2)-2].amount;
		// nameid‚Ítamago‚ÉŽ”‚¦‚éŽž‚Ì‚½‚ß‚É‚»‚Ì‚Ü‚Ü‚É‚µ‚Ä‚¨‚­(?? nameid obtaining egg.. enables something something??)
		//if(p->inventory[RFIFOW(fd,2)-2].amount <= 0){
		//	p->inventory[RFIFOW(fd,2)-2].nameid=0;
		//}

		name_id = sd->status.inventory[RFIFOW(fd,2)-2].nameid;
		switch(name_id)
		{
			case 9001: npc = 1002; strcpy(sd->status.pet.pet_name,"Poring"); break;
			case 9002: npc = 1113; strcpy(sd->status.pet.pet_name,"Drops"); break;
			case 9003: npc = 1031; strcpy(sd->status.pet.pet_name,"Poporing"); break;
			case 9004: npc = 1063; strcpy(sd->status.pet.pet_name,"Lunatic"); break;
			case 9005: npc = 1049; strcpy(sd->status.pet.pet_name,"Picky"); break;
			case 9006: npc = 1011; strcpy(sd->status.pet.pet_name,"Chonchon"); break;
			case 9007: npc = 1042; strcpy(sd->status.pet.pet_name,"Steel Chonchon"); break;
			case 9008: npc = 1035; strcpy(sd->status.pet.pet_name,"Hunter Fly"); break;
			case 9009: npc = 1067; strcpy(sd->status.pet.pet_name,"Cornutus"); break;
			case 9010: npc = 1107; strcpy(sd->status.pet.pet_name,"Baby Desert Wolf"); break;
			case 9011: npc = 1052; strcpy(sd->status.pet.pet_name,"Rockerh"); break;
			case 9012: npc = 1014; strcpy(sd->status.pet.pet_name,"Spore"); break;
			case 9013: npc = 1077; strcpy(sd->status.pet.pet_name,"Poison Spore"); break;
			case 9014: npc = 1019; strcpy(sd->status.pet.pet_name,"Peco Peco"); break;
			case 9015: npc = 1056; strcpy(sd->status.pet.pet_name,"Smokey"); break;
			case 9016: npc = 1057; strcpy(sd->status.pet.pet_name,"Yoyo"); break;
			case 9017: npc = 1023; strcpy(sd->status.pet.pet_name,"Orc Warrior"); break;
			case 9018: npc = 1026; strcpy(sd->status.pet.pet_name,"Munak"); break;
			case 9019: npc = 1110; strcpy(sd->status.pet.pet_name,"Dokebi"); break;
			case 9020: npc = 1170; strcpy(sd->status.pet.pet_name,"Sohee"); break;
			case 9021: npc = 1029; strcpy(sd->status.pet.pet_name,"Isis"); break;
			case 9022: npc = 1156; strcpy(sd->status.pet.pet_name,"Green Petite"); break;
			case 9023: npc = 1109; strcpy(sd->status.pet.pet_name,"Deviruchi"); break;
			case 9024: npc = 1101; strcpy(sd->status.pet.pet_name,"Baphomet Jr."); break;
			default: npc = 1002; strcpy(sd->status.pet.pet_name,"Poring"); break;
		}
		//printf("nameid=%d, case=%d\n",name_id,npc);


		if(sd->status.pet.pet_npc_id_on_map[sd->mapno] == -1)
		{
			sd->status.pet.pet_npc_id_on_map[sd->mapno] = map_data[sd->mapno].npc_num;
			map_data[sd->mapno].npc_num++;
			// Pet Status Initialization
			sd->status.pet.pet_name_flag = 0;
			sd->status.pet.pet_level = 1;
			sd->status.pet.pet_hungry = 0;
			sd->status.pet.pet_friendly = 0xff;
			sd->status.pet.pet_accessory = 0;
		}
		sd->status.pet.activity = 1;
		sd->status.pet.pet_class = npc;

		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]=malloc(sizeof(struct npc_data));
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->m = sd->mapno;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x = sd->x;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y = sd->y;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->u.mons.speed = 200;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->dir = 0;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->class=sd->status.pet.pet_class;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->id=sd->status.pet.pet_id_as_npc;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->block.subtype=SCRIPT;
		memcpy(map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->name,sd->status.pet.pet_name,24);
		//add_block_npc(sd->mapno,map_data[sd->mapno].npc_num);
		add_block_npc(sd->mapno,sd->status.pet.pet_npc_id_on_map[sd->mapno]);
		mmo_map_set_npc(0,buf,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->id,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->class,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y,0);
		mmo_map_sendarea_mxy(sd->mapno,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y,buf,packet_len_table[0x78]);
		//map_data[sd->mapno].npc_num++;


		//printf("CURRENT PET ID=%d MAP=%d\n",sd->status.pet.pet_npc_id_on_map[sd->mapno],sd->mapno);


		WFIFOW(fd,0) = 0x1a4;
		WFIFOB(fd,2) = 0;
		WFIFOL(fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(fd,7) = 0;
		WFIFOSET(fd,11);
		
		
		WFIFOW(fd,0) = 0x1a4;
		WFIFOB(fd,2) = 5;
		WFIFOL(fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(fd,7) = 14;
		WFIFOSET(fd,11);
		
		
		WFIFOW(fd,0) = 0x1a2;
		memcpy(WFIFOP(fd,2),sd->status.pet.pet_name,24);
		WFIFOB(fd,26) = sd->status.pet.pet_name_flag;
		WFIFOW(fd,27) = sd->status.pet.pet_level;
		WFIFOW(fd,29) = sd->status.pet.pet_hungry;;
		WFIFOW(fd,31) = sd->status.pet.pet_friendly;
		WFIFOW(fd,33) = sd->status.pet.pet_accessory;
		WFIFOSET(fd,35);

	}

}
