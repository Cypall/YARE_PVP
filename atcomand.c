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
//atcommand.c : Command Functions

{
/************************************************************************************/

// Check if the item ID that was asked for exists
int is_item(int object){
	char str[80];
	FILE *fp;
	int item_id=0,result=0;

	if((fp = fopen("item_all.list","r"))==NULL){
		printf("Open the item list!\n");
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
  	
      	char *message;
      	message = strchr(RFIFOP(fd,4)+strlen(sd->status.name),'!');
if((int*)message!=NULL){//when ! appears in a message
	  	char comand[40];	//enters system command strings
	  	char temp0[40];		//1st argument
	  	char temp1[24];		//2nd argument
//	  	char temp2[40];		//3rd argument
	  	char moji[200];		//long character string
      	int  x=0,y=0,z=0;	//coordinates
      	int i;

		struct mmo_charstatus *p;
   		sscanf(message,"%s",comand);
  		p=&sd->status;
 if(sd->account_id < 100000) { 
		sscanf(message,"%s",comand);
		if(strncmp(comand,"!save",5)==0){
					char moji[200];
			strcpy(p->last_point.map,sd->mapname);
			strcpy(p->save_point.map,sd->mapname);
			p->last_point.x = sd->x;
			p->save_point.x = sd->x;
			p->last_point.y = sd->y;
			p->save_point.y = sd->y;
			mmo_char_save(sd);
					strcpy(moji,"Location saved.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}
 }

 if(sd->account_id > 100000) { 
//!rura and !map commands
//!rura or !map <map filename [Çò, y cords]>
	  sscanf(message,"%s%s%d%d",comand,temp0,&x,&y);
      if( (strncmp(comand,"!rura",5)==0) || (strncmp(comand,"!map",4)==0))  {
      	if(x>0 && x<325 && y>0 && y<325){
					char moji[200];
      	    for(i=0;map[i][0];i++){
    			if(strcmp(map[i],temp0)==0){
					char moji[200];
          			mmo_map_changemap(fd,sd,temp0,x,y,2);
					strcpy(moji,"Warped!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
          			goto idou;//hide the !command text from other users
    				}
				}
					strcpy(moji,"Well..");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
          			goto idou;//hide the !command text from other users
      	}
	}


//!save command saves current location
//!save <no input>
		sscanf(message,"%s",comand);
		if(strncmp(comand,"!save",5)==0){
					char moji[200];
			strcpy(p->last_point.map,sd->mapname);
			strcpy(p->save_point.map,sd->mapname);
			p->last_point.x = sd->x;
			p->save_point.x = sd->x;
			p->last_point.y = sd->y;
			p->save_point.y = sd->y;
			mmo_char_save(sd);
					strcpy(moji,"Location saved.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}

//!load command returns to saved location
//!load <no input>
		sscanf(message,"%s",comand);
		if(strncmp(comand,"!load",5)==0){
					char moji[200];
			strcpy(sd->mapname,p->last_point.map);
			//strcpy(p->save_point.map,sd->mapname);
			sd->x = p->save_point.x;
			sd->y = p->save_point.y;
			mmo_map_changemap(fd,sd,sd->mapname,sd->x,sd->y,2);
					strcpy(moji,"Returning to saved location...");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}

//!speed command alters walking speed 
//!speed <value[1-1000]> (the lower the value, the faster the movement, but with jerky motions)
		sscanf(message,"%s%d",comand,&x);
		if(strncmp(comand,"!speed",6)==0){
			if(x>0 && x<1000){
					char moji[200];
				sd->speed=x;
				sd->walktimer=x;
					strcpy(moji,"Speed changed!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
			}
		}
// Storage code
// !storage command
        	sscanf(message,"%s%d",comand,&x);
        	if( strncmp(comand,"!storage",8)==0 ) {
            		mmo_open_storage(fd);
            		goto idou;
        	}

			 		// WHO! - added by silentavenger
 		
 		sscanf(message,"%s",comand);
 			if(strncmp(comand,"!who",4)==0){
				int i;
				char moji[200];
					strcpy(moji,"Chars currently online:");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					strcpy(moji,"");
 				for(i=0;i<FD_SETSIZE;i++)
 				{
 					if(session[i]!=NULL && session[i]->session_data!=NULL)
 					{
						struct map_session_data *sd=NULL;
						sd = session[i]->session_data;
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(sd->status.name);
					memcpy(WFIFOP(fd,4),sd->status.name,strlen(sd->status.name));
					WFIFOSET(fd,4 + strlen(sd->status.name));
 					}
 				}
 				goto idou;
			}
// Kill <nick> added by silentavenger
			sscanf(message,"%s%s",comand,temp1);
 			if(strncmp(comand,"!kill",5)==0){
				int cfd;
				char moji[200];
                cfd = find_char(temp1);
				if(cfd==-1)
				{
					strcpy(moji,"No such char.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
 				    goto idou;
				}
				else
				{
					struct map_session_data *sd=NULL;
					sd = session[cfd]->session_data;
					mmo_char_save(sd);
					if(cfd==char_fd)
						char_fd=-1;
					if(sd && sd->state.auth){
						if(sd->chatID)
							mmo_map_leavechat(cfd,(struct mmo_chat*)sd->chatID,sd->status.name);
						del_block(&sd->block);
						WFIFOW(cfd,0)=0x80;
						WFIFOL(cfd,2)=sd->account_id;
						WFIFOB(cfd,6)=2;	//logout?
						mmo_map_sendarea(cfd,WFIFOP(cfd,0),packet_len_table[0x80], 1 );
						map_data[sd->mapno].users--;
						if(char_fd>0){
							memcpy(sd->status.last_point.map,sd->mapname,16);
							sd->status.last_point.x = sd->x;
							sd->status.last_point.y = sd->y;
							
							WFIFOW(char_fd,0)=0x2b01;
							WFIFOW(char_fd,2)=sizeof(sd->status)+12;
							WFIFOL(char_fd,4)=sd->account_id;
							WFIFOL(char_fd,8)=sd->char_id;
							memcpy(WFIFOP(char_fd,12),&sd->status,sizeof(sd->status));
							WFIFOSET(char_fd,WFIFOW(char_fd,2));
						}
					}
					close(cfd);
					delete_session(cfd);
				}
				goto idou;
			}	
   // Warpto by SilentAvenger
			sscanf(message,"%s%s",comand,temp1);
			if(strncmp(comand,"!warpto",7)==0){
				char moji[200];
				int cfd = find_char(temp1);
				if(cfd==-1)
				{
					strcpy(moji,"No such char.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					goto idou;
				}
				else
				{
					struct map_session_data *csd=NULL;
					csd = session[cfd]->session_data;
					mmo_map_changemap(fd,sd,csd->mapname,csd->x,csd->y,2);
					strcpy(moji,"Warped!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					goto idou;//hide the !command text from other users
				}
			}

//status modification
//!option 1 20 15
/*
	Value descriptions:
	R 0119 <ID>.l <param1>.w <param2>.w <param3>.w ?.B
	Visual effects:
	param1=01 petrification?
	param1=02 frozen with frostdiver?
	param1=03 Ç“ÇÊÇÈ
	param1=04 sleep
	param1=06 darkness(you can still walk)

	param2=01 poison
	param2=02 back spirit?
	param2=04 silence
	param2=16 darkness
	param3=01 sight or ruwach?
	param3=02 hiding?
	param3=04 cloaking
	param3=08 cart attachment
	param3=16 ëÈïtÇ´ (???)
	param3=32 Peco Peco riding
	param3=64 è¡Ç¶ÇÈ (out?)
*/
		sscanf(message,"%s%d%d%d",comand,&x,&y,&z);
		if(strncmp(comand,"!option",7)==0){
					char moji[200];
			/*
			p->option consists of the following type of bitÅH
			pram1	0000 0000 0000 0000
			pram2	0000 0000 0000 0000
			pram3	0000 0000 0000 0000

			*/

			WFIFOW(fd,0)=0x0119;
			WFIFOL(fd,2)=sd->account_id;
			WFIFOW(fd,6)=x;
			WFIFOW(fd,8)=y;
			WFIFOW(fd,10)=z;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x0119], 0 );

			//x = x<<16;
			//y = y<<8;
			p->option=x|y|z;

					strcpy(moji,"Lots of changes!!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}
// !hide
sscanf(message,"%s",comand);
  if(strncmp(comand,"!hide",5)==0)
  {
    WFIFOW(fd,0)=0x0119;
    WFIFOL(fd,2)=sd->account_id;
    WFIFOW(fd,6)=00;
    WFIFOW(fd,8)=00;
  if(WFIFOW(fd,10)==04)
    WFIFOW(fd,10)=00;
    else
    WFIFOW(fd,10)=04;
    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x0119], 0 );
    goto idou;
  }
//!jobchange switches jobs
//!jobchange (ID[0-12])
/*
Job descriptions: 
ÇO novice
ÇP swordsman
ÇQ mage
ÇR archer
ÇS acolyte
ÇT merchant
ÇU theif
ÇV knight
ÇW priest
ÇX wizard
ÇPÇO blacksmith
ÇPÇP hunter
ÇPÇQ assassin

ÇPÇR
*/

		sscanf(message,"%s%d",comand,&x);
		if(strncmp(comand,"!jobchange",10)==0){
			if(x>=0 && x<=12){
					char moji[200];
			mmo_map_jobchange(fd,x);
					strcpy(moji,"Job changed!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
			}
		}

//suicide
//!die <no input>
		sscanf(message,"%s",comand);
		if(strncmp(comand,"!die",4)==0){
					char moji[200];
			p->hp = 0;
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0005;
			WFIFOL(fd,4) = p->hp;
			WFIFOSET(fd,8);
			WFIFOW(fd,0) = 0x80;
            WFIFOL(fd,2) = sd->account_id;
            WFIFOB(fd,6) =  1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
					strcpy(moji,"T-Bone!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}

//!alive reanimates character
//!alive <no input>
		sscanf(message,"%s",comand);
		if(strncmp(comand,"!alive",6)==0){
					char moji[200];
					p->hp = p->max_hp;
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0005;
					WFIFOL(fd,4) = p->hp;
					WFIFOSET(fd,8);

					WFIFOW(fd,0) = 0x148;
					WFIFOL(fd,2) = sd->account_id;
					mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x148], 0 );
					strcpy(moji,"Life! We have LIFE!!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			goto idou;//hide the !command text from other users
		}

//GM Chat (Voice of Heaven)
//!kami <text input>
      		sscanf(message,"%s%s",comand,moji);
		if(strncmp(comand,"!kami",5)==0){
			x=0;
			x=strlen(message)+4;
      WFIFOW(fd,0)=0x9a;
      WFIFOW(fd,2)=x;
      strcpy(WFIFOP(fd,4),moji);
      mmo_map_sendall(fd, WFIFOP(fd,0), WFIFOW(fd,2), 0 );
      goto idou;//hide the !command text from other users
      }

//HP,SP Recovery
//!heal <#(HP) #(SP)>
//if !heal is used with no input, HP/SP are fully healed
		x=y=0;
      		sscanf(message,"%s%d%d",comand,&x,&y);
		if(strncmp(comand,"!heal",5)==0){
					char moji[200];
			if(x==0 && y==0){//è»ó™ãLèqÇ≈ëSâÒïú
				WFIFOW(fd,0) = 0x13d;
				WFIFOW(fd,2) = 5;
				WFIFOW(fd,4) = p->max_hp - p->hp;
				WFIFOSET(fd,6);

				WFIFOW(fd,0) = 0x13d;
				WFIFOW(fd,2) = 7;
				WFIFOW(fd,4) = p->max_sp - p->sp;
				WFIFOSET(fd,6);
				p->hp = p->max_hp+1;
				p->sp = p->max_sp+1;

			}else if(x >= -10000 && x <= 10000 && y>= -10000 && y <= 10000){
				p->hp += x;
				p->sp += y;
				WFIFOW(fd,0) = 0x13d;
				WFIFOW(fd,2) = 5;
				WFIFOW(fd,4) = x;
				WFIFOSET(fd,6);

				WFIFOW(fd,0) = 0x13d;
				WFIFOW(fd,2) = 7;
				WFIFOW(fd,4) = y;
				WFIFOSET(fd,6);
			}
			//Once HP/SP reach their maximum values stop healing.
					if(p->hp > p->max_hp || p->hp < 0){
					p->hp = p->max_hp;
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0005;
					WFIFOL(fd,4) = p->hp;
					WFIFOSET(fd,8);
					}

					if(p->sp > p->max_sp || p->sp < 0){
					p->sp = p->max_sp;
					WFIFOW(fd,0) = 0xb0;
					WFIFOW(fd,2) = 0007;
					WFIFOL(fd,4) = p->sp;
					WFIFOSET(fd,8);
					}

					strcpy(moji,"HP and SP recovered.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					goto idou;//hide the !command text from other users
      }


//item spawn
//!item <itemID quantity>
			x=501;y=1;
      		sscanf(message,"%s%d%d",comand,&x,&y);
			if(strncmp(comand,"!item",5)==0){
				struct item tmp_item;

				if(is_item(x)==1){
					char moji[200];
      			memset(&tmp_item,0,sizeof(tmp_item));
      			tmp_item.nameid=x;
      			tmp_item.amount=y;
      			tmp_item.identify=1;
      			len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
      			if(len>0) WFIFOSET(fd,len);

					strcpy(moji,"Got item!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			      goto idou;//hide the !command text from other users
			    }else{
					char moji[200];
					strcpy(moji,"No item with that ID exists!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			    	goto idou;
			    }
      }

//èüéËÇ…é¿ëï!popee (used to be !popee?)
//Level Up! command (base)
//!Lvup <no input>
			sscanf(message,"%s%d",comand,&x);
			if(strncmp(comand,"!base_up",8)==0){
				if(p->base_level + x <= 99){
					char moji[200];
					mmo_map_level_up(fd,x);
					strcpy(moji,"Level UP!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
				goto idou;//hide the !command text from other users
				}
				else
				{
					strcpy(moji,"Max Baseup is level 99 or base_up 98 assuming level 1");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					goto idou;//hide the !command text from other users
				}
			}

//monster spawn
//!spawn <monster ID>
sscanf(message,"%s%d",comand,&x);
			if(strncmp(comand,"!spawn",6)==0){

			if( (44 < x) && (x < 140) )  {spawn_monster(x,sd->x,sd->y,sd->mapno,fd);}
			else if( (699 < x) && (x < 721) )  {spawn_monster(x,sd->x,sd->y,sd->mapno,fd);}
			else if( (1000 < x) && (x < 1253) )  {spawn_monster(x,sd->x,sd->y,sd->mapno,fd);}
			else {
					strcpy(moji,"Invalid monster ID!");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
			}
				goto idou;
}



//Level Up! command (job)
//!Lvup <no input>
			sscanf(message,"%s%d",comand,&x);
			if(strncmp(comand,"!job_up",7)==0){
				if(p->job_level + x <= 50){
					char moji[200];
					mmo_map_job_lv_up(fd,x);
					mmo_map_update_param(fd,SP_SKILLPOINT,x);
					strcpy(moji,"Your job level has been raised.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
				goto idou;
				}
				else
				{
					strcpy(moji,"Job level 50 is the maximum.");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					goto idou;
				}
			}



//!h help command explains !commands
//!h <no input>
			sscanf(message,"%s",comand);
			if(strncmp(comand,"!h",2)==0 || strncmp(comand,"!help",5)==0 ){
					char moji[400];
					FILE *file;
					file = fopen("config/help.txt","r");

					sprintf(moji,"Help Commands");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
					while(fgets(moji,380,file)!=NULL){
						strcat(moji," \n");
						WFIFOW(fd,0) = 0x8e;
						WFIFOW(fd,2) = 4+strlen(moji);
						memcpy(WFIFOP(fd,4),moji,strlen(moji));
						WFIFOSET(fd,4 + strlen(moji));
					}
					fclose(file);
				goto idou;//hide the !command text from other users
			}

//!GM enables GM sprite.Å@Still doesnÅft work correctly. Requires relogin.
//!GM <no input>
/*
	R 006b <len>.w <character select data>.106B*
	Character select connection success & character data
	<character select data> = <character ID>.l <base exp>.l <zeny>.l <job exp>.l <job level>.l ?.8B <option>.l <karma>.l <manner>.l ?.2B <HP>.w <MaxHP>.w <SP>.w <MaxSP>.w <speed>.w <class>.w <hair>.w ?.2w <base level>.w <skill point>.w ?.w <sheild>.w <head option top>.w <head option mid>.w <hair color>.w ?.w <name>.24B <STR>.B <AGI>.B <VIT>.B <INT>.B <DEX>.B <LUK>.B <character number>.B ?.B
*/
			sscanf(message,"%s",comand);
			if(strncmp(comand,"!GM",3)==0){
					char moji[200];
				int next_id;
				next_id = mmo_char_GM(sd);
				WFIFOW(char_fd,0)=0x3001;
				WFIFOL(char_fd,2)=fd;
				WFIFOSET(char_fd,6);
				sd->account_id=next_id;
				sd->status.account_id=next_id;
					strcpy(moji,"Want to be a game master?");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
				goto idou;//hide the !command text from other users
			}
//PVP cancellation (PVP must be disabled on every map it was enabled)
//!pvpoff <no input>
          		sscanf(message,"%s",comand);
	         	if(strncmp(comand,"!pvpoff",7) == 0){
               		  char moji[200];
				WFIFOW(fd,0)=0x199;
				WFIFOW(fd,2)=1;
				WFIFOSET(fd,4);
					strcpy(moji,"Player Versus Player : DISABLED");
                 	  WFIFOW(fd,0) = 0x8e;
                    	  WFIFOW(fd,2) = 4 + strlen(moji);
                   	  memcpy(WFIFOP(fd,4),moji,strlen(moji));
               	          WFIFOSET(fd,4 + strlen(moji));
                          PVP_flag=0;
            		  goto idou;
	                }
//PVP enable
//!pvp <no input>
	               sscanf(message,"%s",comand);
       		       if(strncmp(comand,"!pvpon",6)==0){
              		 char moji[200];
				WFIFOW(fd,0)=0x199;
				WFIFOW(fd,2)=1;
				WFIFOSET(fd,4);
			//	mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x199], 0 );
				WFIFOW(fd,0) = 0x19a;
				WFIFOL(fd,2) = sd->account_id;
				WFIFOL(fd,6) = rank;
				WFIFOL(fd,10) = users_global;
				mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x19a], 0 );
					strcpy(moji,"Player Versus Player : ENABLED");
                  	 WFIFOW(fd,0) = 0x8e;
                	 WFIFOW(fd,2) = 4 + strlen(moji);
             	         memcpy(WFIFOP(fd,4),moji,strlen(moji));
                    	 WFIFOSET(fd,4 + strlen(moji));
                    	 PVP_flag=1;
                	 goto idou;
			}


//!model modifies hair type, hair color, and clothing color
//!model <x y z> (when 2hq is used, it causes an error...(ÅLÑDÅM))
/*	ExampleÅF!model 15 4 0

x [0Å`17]Hair type
y [0Å`8]Hair color
z [0Å`4]Clothing color

*/
			sscanf(message,"%s%d%d%d",comand,&x,&y,&z);
			if(strncmp(comand,"!model",6)==0){
			char moji[200];
				if(x >= 0 && x < 18 && y >=0 && y < 9 && z >= 0 && z <= 4){
	            //ïûÇÃêFïœçX
	            if (sd->status.class == 12){
	                //ÉAÉTÉVÉìÇÕïûÇÃêFñ¢é¿ëï
	                	strcpy(moji,"Assassin isn't working yet...");
						WFIFOW(fd,0) = 0x8e;
						WFIFOW(fd,2) = 4 + strlen(moji);
						memcpy(WFIFOP(fd,4),moji,strlen(moji));
						WFIFOSET(fd,4 + strlen(moji));
	              	}else{
	            	sd->status.hair = x;
	            	sd->status.hair_color = y;
	            	sd->status.clothes_color = z;

	            	WFIFOW(fd,0) = 0xc3;
	            	WFIFOL(fd,2) = sd->account_id;
	            	WFIFOB(fd,6) = 1;
	            	WFIFOB(fd,7) = x;
	            	WFIFOSET(fd,8);

	            	WFIFOW(fd,0) = 0xc3;
	            	WFIFOL(fd,2) = sd->account_id;
	            	WFIFOB(fd,6) = 6;
	            	WFIFOB(fd,7) = y;
	            	WFIFOSET(fd,8);

	            	WFIFOW(fd,0) = 0xc3;
	            	WFIFOL(fd,2) = sd->account_id;
	            	WFIFOB(fd,6) = 7;
	            	WFIFOB(fd,7) = z;
	            	WFIFOSET(fd,8);
	            }
						strcpy(moji,"Amazing!");
						WFIFOW(fd,0) = 0x8e;
						WFIFOW(fd,2) = 4 + strlen(moji);
						memcpy(WFIFOP(fd,4),moji,strlen(moji));
						WFIFOSET(fd,4 + strlen(moji));
					goto idou;
				}else{
						strcpy(moji,"Invalid values. Cancelled!");
						WFIFOW(fd,0) = 0x8e;
						WFIFOW(fd,2) = 4 + strlen(moji);
						memcpy(WFIFOP(fd,4),moji,strlen(moji));
						WFIFOSET(fd,4 + strlen(moji));
				}
			}










//!test command for debugging. (please do not make changes)
//!test <no input>
//			R 00fb <len>.w <party name>.24B {<ID>.l <nick>.24B <map name>.16B <leader>.B <offline>.B}.46B*
			sscanf(message,"%s",comand);
			if(strncmp(comand,"!test",5)==0){
				WFIFOW(fd,0) = 0xfb;
				WFIFOW(fd,2) = 74;
				strcpy(WFIFOP(fd,4),"Horseman group");
				WFIFOL(fd,28) = 100006;
				strcpy(WFIFOP(fd,32),"Applying~");
				strcpy(WFIFOP(fd,56),"Where?");
				WFIFOB(fd,72)=1;
				WFIFOB(fd,73)=1;
				WFIFOSET(fd,74);
				goto idou;
			}


		sscanf(message,"%s",comand);
		if(strncmp(comand,"!reload_config",5)==0){
			reload_config();
			goto idou;
		}




//!test command for debugging. (please do not make changes)
//!test <no input>
/*
R 0196 <type>.w <ID>.l
	(éÂÇ…?)ÉvÉää÷åWÇÃëùã≠Ç‡ÇÃêFÅXÅBIDÇÕtargetÇ∆évÇÌÇÍÇÈÇ™é©ï™ëäéËÇÃÇ›ÇµÇ©óàÇ»Ç¢?
	Mainly a reinforcement pre-relationship. (???)
	ID might be the target, but it only works on other players? (???)
	type=02 IMPOSITIO grant
	type=03 IMPOSITIO cancel
	type=07 ASPERSIO grant
	type=08 ASPERSIO cancel
	type=0b KYRIE grant
	type=0c KYRIE cancel
*/
			sscanf(message,"%s%d",comand,&x);
			if(strncmp(comand,"!debug",6)==0){
				WFIFOW(fd,0) = 0x196;
				WFIFOW(fd,2) = x;
				WFIFOL(fd,4) = sd->account_id;
				WFIFOSET(fd,8);
				goto idou;
			}


//ADDED
//!rura revised
//!go <value[1-7]>
/*
	0=Prontera
	1=Morroc
	2=Geffen
	3=Payon
	4=Alberta
	5=Izlude
	6=Al de Baron
	7=Lutie
*/
			sscanf(message,"%s%d",comand,&x);
			if(strncmp(comand,"!go",3)==0)
			{
      			if(x==0)
      			{
             		mmo_map_changemap(fd,sd,"prontera.gat",156,191,2);
          			goto idou;
				}
				else if(x==1)
				{
             		mmo_map_changemap(fd,sd,"morocc.gat",156,96,2);
          			goto idou;
				}
				else if(x==2)
				{
             		mmo_map_changemap(fd,sd,"geffen.gat",119,60,2);
          			goto idou;
				}
				else if(x==3)
				{
             		mmo_map_changemap(fd,sd,"payon.gat",89,122,2);
          			goto idou;
				}
				else if(x==4)
				{
             		mmo_map_changemap(fd,sd,"alberta.gat",192,147,2);
          			goto idou;
				}
				else if(x==5)
				{
             		mmo_map_changemap(fd,sd,"izlude.gat",128,114,2);
          			goto idou;
				}
				else if(x==6)
				{
             		mmo_map_changemap(fd,sd,"aldebaran.gat",140,131,2);
          			goto idou;
				}
				else if(x==7)
				{
             		mmo_map_changemap(fd,sd,"xmas.gat",147,134,2);
          			goto idou;
				}
				else
				{
					char *moji;
					moji="Error in map destination...";
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					strcpy(WFIFOP(fd,4),moji);
					WFIFOSET(fd,4 + strlen(moji));
          			goto idou;//hide the !command text from other users
				}
			}



	}
}



/************************************************************************************/
}
