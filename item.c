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
// item.c - File for including special items, etc.
{
// searching for itemID
int search_item(int object){
	char file_name[40];
	char strgat[80];
	char gomi[40];
	FILE *fp;
	int scount=0,fcount=0,nameid=0;
	
	printf("object:%d\n",object);
	if(object==1){
		strcpy(file_name,"item_all.list");
		scount = (int)(((double)rand()/(double)RAND_MAX)*1061.0);
		printf("scount:%d\n",scount);
	}else if(object==2){
		strcpy(file_name,"item_equipment.list");
		scount = (int)(((double)rand()/(double)RAND_MAX)*490.0);
	}else if(object==3){
		strcpy(file_name,"item_card.list");
		scount = (int)(((double)rand()/(double)RAND_MAX)*149.0);
	}else if(object==4){
		strcpy(file_name,"item_present.list");
		scount = (int)(((double)rand()/(double)RAND_MAX)*77.0);
	}
	if((fp = fopen(file_name,"r"))==NULL){
		printf("Opened Successfully!\n");
	}
	for(fcount=0;fcount<scount;fcount++){//the file’s scount to susumu(?)(行目まですすむ)
		fgets(strgat,80,fp);
	}
	fclose(fp);
	sscanf(strgat,"%d%s",&nameid,gomi);
	printf("itemID:%d          item name:%s\n",nameid,gomi);
	return nameid;
}


// check to see if itemID exists
int is_item(int object){
	char str[80];
	FILE *fp;
	int item_id=0,result=0;
	
	if((fp = fopen("item_all.list","r"))==NULL){
		printf("Opened Successfully!\n");
	}
	while(fgets(str,80,fp)!=NULL){
		sscanf(str,"%d",&item_id);
		if(item_id==object){
			result=1;//found
				break;
		}
	}
	fclose(fp);
	return result;
}
/**********************************************************************************************/

// item use
     	int nameid=p->inventory[RFIFOW(fd,2)-2].nameid;
     	short option;
     	double bonus=0;//Vit recovery
     	item_num = itemdb_type(p->inventory[RFIFOW(fd,2)-2].nameid);//inventory baggage 
  		p=&sd->status;
     	if(item_num==0){//if recovery item
			WFIFOW(fd,0) = 0xa8;
			WFIFOW(fd,2) = RFIFOW(fd,2);
			WFIFOW(fd,4) = --p->inventory[RFIFOW(fd,2)-2].amount;
			WFIFOB(fd,6) = 01;
			WFIFOSET(fd,7);
			if(p->inventory[RFIFOW(fd,2)-2].amount <= 0){
				p->inventory[RFIFOW(fd,2)-2].nameid=0;
			}
			//recovery amount　=　（amount of recovery item）*（1 + VIT/100 + recovery skill level/10)
				if(p->skill[3].lv >= 1){
					bonus = ((double)p->skill[3].lv)/10.0;
				}
				bonus += ((double)(p->vit+100)/(double)100);
			//srand((unsigned)time(NULL));//random number
			if(nameid==501){//red potion
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 44)+30) * bonus;
			}else if(nameid==502){//orange potion
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 69)+50) * bonus;
			}else if(nameid==503){//yellow potion
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 104)+80) * bonus;
			}else if(nameid==504){//white potion
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 120)+149) * bonus;
			}else if(nameid==505){//blue potion
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 45)+53);
			}else if(nameid==506){//green potion(poison recovery)
				option = 0x0038;
				p->option = p->option & option;// turn off flag unless peco or cart
				WFIFOW(fd,0)=0x0119;
				WFIFOL(fd,2)=sd->account_id;
				WFIFOW(fd,6)=0;
				WFIFOW(fd,8)=0;
				WFIFOW(fd,10)=p->option;
			mmo_map_sendarea( fd, WFIFOP(fd,0),packet_len_table[0x0119], 0 );
			}else if(nameid==507){//red herb
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 12)+19) * bonus;
			}else if(nameid==508){//yellow herb
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 21)+29) * bonus;
			}else if(nameid==509){//white herb
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 50)+50) * bonus;
			}else if(nameid==510){//blue herb
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 10)+17);
			}else if(nameid==511){//green herb(poison recovery)
				option = 0x0038;
				p->option = p->option & option;//turn off flag unless peco or cart
				WFIFOW(fd,0)=0x0119;
				WFIFOL(fd,2)=sd->account_id;
				WFIFOW(fd,6)=0;
				WFIFOW(fd,8)=0;
				WFIFOW(fd,10)=p->option;
			mmo_map_sendarea( fd, WFIFOP(fd,0),packet_len_table[0x0119], 0 );
			}else if(nameid==512){//apple
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 12)+15) * bonus;
			}else if(nameid==513){//banana
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 11)+15) * bonus;
			}else if(nameid==514){//grape
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 8)+11);
			}else if(nameid==515){//carrot
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 13)+15) * bonus;
			}else if(nameid==516){//potato
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 10)+14) * bonus;
			}else if(nameid==517){//meat
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 41)+55) * bonus;
			}else if(nameid==518){//honey
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 46)+66) * bonus;
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 5)+9);
			}else if(nameid==519){//milk
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 15)+22) * bonus;
			}else if(nameid==520){//leaf of hinale
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 60)+22) * bonus;
			}else if(nameid==521){//leaf of aloe
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 150)+25) * bonus;
			}else if(nameid==522){//fruit of mastela
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 200)+200) * bonus;
			}else if(nameid==523){//holy water(zombie destroyer)
				option = 0x0038;
				p->option = p->option & option;//turn off flag unless peco or cart
				WFIFOW(fd,0)=0x0119;
				WFIFOL(fd,2)=sd->account_id;
				WFIFOW(fd,6)=0;
				WFIFOW(fd,8)=0;
				WFIFOW(fd,10)=p->option;
			mmo_map_sendarea( fd, WFIFOP(fd,0),packet_len_table[0x0119], 0 );
			}else if(nameid==525){//panacea(recover abnormal status) 
				option = 0x0038;
				p->option = p->option & option;//turn off flag unless peco or cart
				WFIFOW(fd,0)=0x0119;
				WFIFOL(fd,2)=sd->account_id;
				WFIFOW(fd,6)=0;
				WFIFOW(fd,8)=0;
				WFIFOW(fd,10)=p->option;
			mmo_map_sendarea( fd, WFIFOP(fd,0),packet_len_table[0x0119], 0 );
			}else if(nameid==526){//royal jelly(HP/SP recovery)
				p->sp = p->max_sp;
				p->hp = p->max_hp;
			}else if(nameid==528){//monster feed
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 41)+55) * bonus;
			}else if(nameid==529){//candy
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 30)+44) * bonus;
			}else if(nameid==530){//candy stripper
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 60)+88) * bonus;
			}else if(nameid==531){//apple juice
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 15)+22) * bonus;
			}else if(nameid==532){//banana juice
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 15)+22) * bonus;
			}else if(nameid==533){//grape juice
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 30)+22);
			}else if(nameid==534){//carrot juice
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 15)+22) * bonus;
			}else if(nameid==535){//pumpkin
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 12)+15) * bonus;
			}else if(nameid==536){//ice cream
				p->sp += (double)((((double)rand()/(double)RAND_MAX) * 150)+10) * bonus;
			}else if(nameid==538){//well baked cookie
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 500)+10) * bonus;
			}else if(nameid==539){//cake
				p->hp += (double)((((double)rand()/(double)RAND_MAX) * 1500)+10) * bonus;
			}

			if(p->hp > p->max_hp){//when max HP is reached
				p->hp = p->max_hp;//set to max HP
			}
			if(p->sp > p->max_sp){//when max SP is reached
				p->sp = p->max_sp;//set to max SP
			}
			
/*
R 00b0 <type>.w <val>.l
	Below are the renewal of various ability values. type: corresponds with numeric values
	0000:speed 0003:evildoing value 0004:manner points 0005:HP 0006:MaxHP
	0007:SP 0008:MaxSP 0009:status point 000b:base level
	000c:skill point 0018:weight(10x input)
	0019:maximum weight(10x input)
	0029:ATK(before) 002a:ATK(after) 002b:MATK(before) 002c:MATK(after)
	002d:DEF(before) 002e:DEF(after) 002f:MDEF(before) 0030:MDEF(after)
	0031:HIT 0032:FLEE(before) 0033:FLEE(after) 0034:critical
	0035:ASPD(2ms?) 0037:job level
	0082:unsure...ATK(same as after)?
*/
				WFIFOW(fd,0) = 0xb0;
				WFIFOW(fd,2) = 0005;
				WFIFOL(fd,4) = p->hp;
				WFIFOSET(fd,8);
				
				WFIFOW(fd,0) = 0xb0;
				WFIFOW(fd,2) = 0007;
				WFIFOL(fd,4) = p->sp;
				WFIFOSET(fd,8);
				
     }else if(item_num==2){//if item has special effect
     	 	WFIFOW(fd,0) = 0xa8;
			WFIFOW(fd,2) = RFIFOW(fd,2);
			WFIFOW(fd,4) = --p->inventory[RFIFOW(fd,2)-2].amount;
			WFIFOB(fd,6) = 01;
			WFIFOSET(fd,7);
			if(p->inventory[RFIFOW(fd,2)-2].amount <= 0){
				p->inventory[RFIFOW(fd,2)-2].nameid=0;
			}
			if(nameid==602){//wing of butterfly
				mmo_map_changemap(fd,sd,p->save_point.map,p->save_point.x,p->save_point.y,2);
			}else if(nameid==601){//ヘing of fly
				do {
				sd->x=rand()%(map_data[sd->mapno].xs-2)+1;
				sd->y=rand()%(map_data[sd->mapno].ys-2)+1;
				} while(map_data[sd->mapno].gat[sd->x+sd->y*map_data[sd->mapno].xs]==1 || map_data[sd->mapno].gat[sd->x+sd->y*map_data[sd->mapno].xs]==5);
				mmo_map_changemap(fd,sd,sd->mapname,sd->x,sd->y,2);
			}else if(nameid==611){//magnifying glass
				int i,c;
				WFIFOW(fd,0) = 0x177;
				for(i=c=0;i<100;i++)
					//if item hasn’t been appraised
					if(sd->status.inventory[i].identify != 1)
					{
						WFIFOW(fd,4+c*2) = i+2;//sd->status.inventory[i].nameid;
						c++;
					}
				WFIFOW(fd,2) = 4+c*2;
				WFIFOSET(fd,4+c*2);
			}else if(nameid==604){//dead branch(summons random enemy)
				/*
				int mapno,npc;
				unsigned char buf[256];
				mapno = sd->mapno;
				npc = (double)(((double)rand()/(double)RAND_MAX) * 250) + 1001;
				map_data[sd->mapno].npc[npc]->x = sd->x;
				map_data[sd->mapno].npc[npc]->y = sd->y;
				map_data[mapno].npc[map_data[mapno].npc_num]=malloc(sizeof(struct npc_data));
			    map_data[mapno].npc[map_data[mapno].npc_num]->u.mons.speed=200;
				map_data[mapno].npc[map_data[mapno].npc_num]->class=npc;
				map_data[mapno].npc[map_data[mapno].npc_num]->id=npc;
				map_data[mapno].npc[map_data[mapno].npc_num]->block.subtype=MONS;
				map_data[mapno].npc[map_data[mapno].npc_num]->u.mons.hp=10;
				map_data[mapno].npc[map_data[mapno].npc_num]->u.mons.script = 1;
				memcpy(map_data[mapno].npc[map_data[mapno].npc_num]->name,npc,24);
				respawn_mons2(mapno,map_data[mapno].npc_num);
				map_data[mapno].npc_num++;
				*/
			}else if(nameid==605){//anodyne
				WFIFOW(fd,0) = 0x147;
				WFIFOW(fd,2) = 8;
				WFIFOW(fd,4) = skill_db[8].type_inf;
				WFIFOW(fd,6) = 0;
				WFIFOW(fd,8) = 1;
				WFIFOW(fd,10) = skill_db[8].sp;
				WFIFOW(fd,12) = skill_db[8].range;
				memcpy(WFIFOP(fd,14),"Anodyne",24);
				WFIFOB(fd,38) = 0;
				mmo_map_sendarea(fd,WFIFOP(fd,0),39,0);
			}else if(nameid==606){//aloebera(full SP recovery)
				p->sp = p->max_sp;
				WFIFOW(fd,0) = 0xb0;
				WFIFOW(fd,2) = 0007;
				WFIFOL(fd,4) = p->sp;
				WFIFOSET(fd,8);
			}else if(nameid==607){//yggdrasilberry(full HP/SP recovery)
					p->hp = p->max_hp;
					p->sp = p->max_sp;
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0005;
					WFIFOL(fd,4) = p->hp;
					WFIFOSET(fd,8);
				
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0007;
					WFIFOL(fd,4) = p->sp;
					WFIFOSET(fd,8);
			}else if(nameid==608){//seed of yggdrasil(semi HP/SP recovery) 
					p->hp += p->max_hp/2;
					p->sp += p->max_sp/2;
					if(p->hp > p->max_hp)p->hp = p->max_hp;
					if(p->sp > p->max_sp)p->sp = p->max_sp;
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0005;
					WFIFOL(fd,4) = p->hp;
					WFIFOSET(fd,8);
				
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0007;
					WFIFOL(fd,4) = p->sp;
					WFIFOSET(fd,8);
			}else if(nameid==609){//amulet
				
			}else if(nameid==610){//leaf of yggdrasil(resurrect w/1hp)
				WFIFOW(fd,0) = 0x147;
				WFIFOW(fd,2) = 54;
				WFIFOW(fd,4) = skill_db[54].type_inf;
				WFIFOW(fd,6) = 0;
				WFIFOW(fd,8) = 1;
				WFIFOW(fd,10) = skill_db[54].sp;
				WFIFOW(fd,12) = skill_db[54].range;
				memcpy(WFIFOP(fd,14),"Leaf of Yggdrasil",24);
				WFIFOB(fd,38) = 0;
				mmo_map_sendarea(fd,WFIFOP(fd,0),39,0);
			}else if(nameid==603){//old blue box(random item generation)
					int i1=502,i2=1,len=0;
					struct item tmp_item;
				i1 = search_item(1);
				if(i1>7000 || (619 <= i1 && i1 <= 643) || 
					(1256 <= i1 && i1<=1261)){i1=644;}//becomes unstable after 7000
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=i1;
      			tmp_item.amount=i2;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);
			}else if(nameid==616){//old card (random card generation)
					int i1=502,i2=1,len=0;
					struct item tmp_item;
				i1 = search_item(3);
				if(i1>7000 || (619 <= i1 && i1 <= 643) || 
					(1256 <= i1 && i1<=1261)){i1=644;}//becomes unstable after 7000
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=i1;
      			tmp_item.amount=i2;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);
			}else if(nameid==617){//old purple box(random equipment generation)
					int i1=502,i2=1,len=0;
					struct item tmp_item;
				i1 = search_item(2);
				if(i1>7000 || (619 <= i1 && i1 <= 643) || 
					(1256 <= i1 && i1<=1261)){i1=644;}//becomes unstable after 7000
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=i1;
      			tmp_item.amount=i2;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);
			}else if(nameid==618){//古い巻物
					int i1=502,i2=1,len=0;
					struct item tmp_item;
				i1 = search_item(3);
				if(i1>7000 || (619 <= i1 && i1 <= 643) || 
					(1256 <= i1 && i1<=1261)){i1=644;}//becomes unstable after 7000
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=i1;
      			tmp_item.amount=i2;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);
			}else if(nameid==644){//present box(random card、equipment、consumable generation)
					int i1=502,i2=1,len=0;
					struct item tmp_item;
				i1 = search_item(4);
				if(i1>7000 || (619 <= i1 && i1 <= 643) || 
					(1256 <= i1 && i1<=1261)){i1=644;}//becomes unstable after 7000
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=i1;
      			tmp_item.amount=i2;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);
			}else if(nameid == 643){// ADDED on 04/09/2003 --------------
				int i,c;
				WFIFOW(fd,0) = 0x1a6;
				for(i=c=0;i<100;i++)
					//if egg
					if((sd->status.inventory[i].nameid >= 9001)&&(sd->status.inventory[i].nameid <=9024))
					{
						WFIFOW(fd,4+c*2) = i+2;//sd->status.inventory[i].nameid;
						c++;
					}
				WFIFOW(fd,2) = 4+c*2;
				WFIFOSET(fd,4+c*2);


				//WFIFOW(fd,0) = 0x1a6;
				//WFIFOW(fd,2) = 6;
				//WFIFOW(fd,4) = RFIFOW(fd,2);
				//WFIFOSET(fd,6);
				//printf("index=%d\n",RFIFOW(fd,2));
				// --------------------------------------------------
			}
     }else{
     	 	if(nameid==714){//emperium (to start a guild)
     	 		
     	 	}
     }

}
/********************************************************************************************/
