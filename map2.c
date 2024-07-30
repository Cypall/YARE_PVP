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
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>

#include "core.h"
#include "mmo.h"
#include "grfio.h"
#include "npc.h"
#include "itemdb.h"
#include "skill_db.h"
#include "save.h"
#include "map2.h"
#include "party.h"
#include "guild.h"
// ADDED on 04/09/2003 --------------
#include "pet.h"
// ----------------------------------

const int packet_len_table[0x200]={
   10,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0, 55, 17,  3, 37,  46, -1, 23, -1,  3,108,  3,  2,
    3, 28, 19, 11,  3, -1,  9,  5,  52, 51, 56, 58, 41,  2,  6,  6,

    7,  3,  2,  2,  2,  5, 16, 12,  10,  7, 29, 23, -1, -1, -1,  0,
    7, 22, 28,  2,  6, 30, -1, -1,   3, -1, -1,  5,  9, 17, 17,  6,
   23,  6,  6, -1, -1, -1, -1,  8,   7,  6,  7,  4,  7,  0, -1,  6,
    8,  8,  3,  3, -1,  6,  6, -1,   7,  6,  2,  5,  6, 44,  5,  3,

    7,  2,  6,  8,  6,  7, -1, -1,  -1, -1,  3,  3,  6,  3,  2, 27,
    3,  4,  4,  2, -1, -1,  3, -1,   6, 14,  3, -1, 28, 29, -1, -1,
   30, 30, 26,  2,  6, 26,  3,  3,   8, 19,  5,  2,  3,  2,  2,  2,
    3,  2,  6,  8, 21,  8,  8,  2,   2, 26,  3, -1,  6, 27, 30, 10,


    2,  6,  6, 30, 79, 31, 10, 10,  -1, -1,  4,  6,  6,  2, 11, -1,
   10, 39,  4, 10, 31, 35, 10, 18,   2, 13, 15, 20, 68,  2,  3, 16,
    6, 14, -1, -1, 21,  8,  8,  8,   8,  8,  2,  2,  3,  4,  2, -1,
    6, 86,  6, -1, -1,  7, -1,  6,   3, 16,  4,  4,  4,  6, 24, 26,

   22, 14,  6, 10, 23, 19,  6, 39,   8,  9,  6, 27, -1,  2,  6,  6,
  110,  6, -1, -1, -1, -1, -1,  6,  -1, 54, 66, 54, 90, 42,  6, 42,
   -1, -1, -1, -1, -1, 30, -1,  3,  14,  3, 30, 10, 43, 14,186,182,
   14, 30, 10,  3, -1,  6,106, -1,   4,  5,  4, -1,  6,  7, -1, -1,

    6,  3,106, 10, 10, 34,  0,  6,   8,  4,  4,  4, 29, -1, 10,  6,
   90, 86, 24,  6, 30,102,  8,  4,   8,  4, 14, 10, -1,  6,  2,  6,
    3,  3, 35,  5, 11, 26, -1,  4,   4,  6, 10, 12,  6, -1,  4,  4,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

};

const unsigned short def_aspd[14][13] =
  {
    { 500, 650, 700,2000,2000,2000,2000, 650, 700, 800,2000,2000,2000},     /* Novice */
    { 400, 500, 550, 600, 650, 700,2000,2000, 650, 700, 740,2000,2000},     /* Swordman */
    { 500, 600,2000,2000,2000,2000,2000, 700,2000,2000,2000,2000,2000},     /* Mage */
    { 400, 600,2000,2000,2000,2000, 700,2000,2000,2000,2000,2000,2000},     /* Archer */
    { 350,2000,2000,2000,2000,2000,2000, 600, 600,2000,2000,2000,2000},     /* Acolyte */
    { 400, 600, 700,2000,2000,2000,2000,2000, 700, 700, 750,2000,2000},     /* Marchant */
    { 400, 500, 650,2000,2000,2000, 850,2000,2000, 800,2000,2000,2000},     /* Thief */
    { 400, 500, 500, 550, 600, 650,2000,2000, 650, 700, 740,2000,2000},     /* Night */
    { 350,2000,2000,2000,2000,2000,2000, 600, 600,2000,2000,2000, 600},     /* Priest */
    { 500, 600,2000,2000,2000,2000,2000, 700,2000,2000,2000,2000,2000},     /* Wizard */
    { 400, 600, 700,2000,2000,2000,2000,2000, 700, 700, 750,2000,2000},     /* Blacksmith */
    { 400, 600,2000,2000,2000,2000, 600,2000,2000,2000,2000,2000,2000},     /* Hunter */
    { 400, 500, 650,2000,2000,2000,2000,2000,2000, 800,2000, 500,2000},     /* Assasin */
    { 400, 600,2000,2000,2000,2000, 600,2000,2000,2000,2000,2000,2000},     /* kami */
};

int count=0;
int rank;
int char_fd;
int current_attack_m = 0;
char userid[24];
char passwd[24];
char char_ip_str[16];
int char_port;
char map_ip_str[16];
int map_ip;
int map_port;
int map_port_fd;
char mapmove_name[24]="-mapmove-";
char map[MAX_MAP_PER_SERVER][16]={"prontera.gat",""};
struct mmo_map_data map_data[MAX_MAP_PER_SERVER];
int users,users_global;
char *npc_txt[256];
int npc_txt_num=0;
struct skill_db skill_db[MAX_SKILL];//skill database information
struct mmo_chat *last_chat;
struct block_list *object[50000];
int first_free_object_id,last_object_id;
struct mons_data mons_data[4000];
long ExpData[101];//Lvup experience value data
long SkillExpData[3][101];//skill lv up experience data
int PVP_flag=0;
int check_connect_timer;
int send_users_tochar_timer;
int clear_flooritem_timer;
int loop_monstersearch_timer;
int heal_hp_timer;
int heal_sp_timer;
int map_number;


//macro declaration
#define NOM_ATTACK(atk1,atk2,def) ((atk1+rand()%atk2)-def);//normal attack
#define CRI_ATTACK(atk1,atk2,s_lv,s_type)	(atk1+atk2+s_lv*s_type);//critical attack
#define KAT_ATTACK(damage)	((damage/5)+1);//second katar attack
#define SKILL_ATTACK(matk1,matk2) (matk2+rand()%matk1);//skill attack
#define SKILL_HEAL(int_,blvl,lvl) ((blvl + int_)/8)*(12 * lvl);//( [ BLv + Int ] / 8 ) * ( 12 * SLv ) formula by Trihan 
//#define SKILL_HEAL(int_) (rand()%2000+int_);//heal amount
#define SKILL_BASH(atk1,atk2,lvl) ( (  (atk1+rand()%atk2) * (100+(30*lvl))/100 ) - 0  );//skilllvlbonus =  100% 130 160 depend on skill lvl
// Find char, By SilentAvenger 16/6/03
int find_char(char* cname)
{
	int i;
	for(i=0;i<FD_SETSIZE;i++)
	{
		if(session[i]!=NULL && session[i]->session_data!=NULL)
		{
			struct map_session_data *sd=NULL;
			sd = session[i]->session_data;
			if(strcmp(sd->status.name,cname)==0) return i;
		}
	}
	return -1;
}

// BugFix by -AL- 15/06/2003
void add_block(struct block_list *bl,int m,int x,int y)
{
  struct block_list * head;
  int blocknumber;

  if(!bl){
    printf("[Error] add_block : block_list is NULL\n");
    return;
  }
  if((m<0)||(m>(MAX_MAP_PER_SERVER-1))){
    printf("[Error] add_block : wrong map number (%d)\n",m);
    return;
  }
  if((x<0)||(y<0)){
    printf("[Error] add_block : x(%d) or y(%d) is negativ\n",x,y);
    return;
  }

  //printf("[Debug] add_block : add (%d,%d,%d)\n",m,x,y);
  
  blocknumber=x/BLOCK_SIZE+(y/BLOCK_SIZE)*map_data[m].bxs;
  head = &map_data[m].block[blocknumber];
  if (!head){
    printf("[Error] add_block : block %d doesn't exist in map %d\n",blocknumber,m);
    return;
  }
  
  bl->next = head->next;
  bl->prev = head;
  if(bl->next) bl->next->prev = bl;
  head->next = bl;
  return;
}


void del_block(struct block_list *bl)
{
  if(bl->prev==NULL){
    printf("del_block link error\n");
    exit(1);
  }
  if(bl->next) bl->next->prev = bl->prev;
  bl->prev->next = bl->next;
  bl->next = NULL;
  bl->prev = NULL;
}

int search_free_object_id(void)
{
  int i;
  if(first_free_object_id<2 || first_free_object_id>=50000)
    first_free_object_id=2;
  for(i=first_free_object_id;i<50000;i++)
    if(object[i]==NULL)
      break;
  if(i==50000){
    printf("no free object id\n");
    return 0;
  }
  first_free_object_id=i;
  if(last_object_id<i)
    last_object_id=i;
  return i;
}

int delete_object(int id)
{
  if(object[id]==NULL)
    return 0;

  del_block(object[id]);
  free(object[id]);
  object[id]=NULL;

  if(first_free_object_id>id)
    first_free_object_id=id;

  while(last_object_id>2 && object[last_object_id]==NULL)
    last_object_id--;

  return 0;
}

int set_pos(unsigned char *p,int x,int y)
{
  *p=x>>2;
  p[1]=(x<<6) | ((y>>4)&0x3f);
  p[2]=y<<4;
  return 0;
}

int set_2pos(unsigned char *p,int x0,int y0,int x1,int y1)
{
  *p=x0>>2;
  p[1]=(x0<<6) | ((y0>>4)&0x3f);
  p[2]=(y0<<4) | ((x1>>6)&0x0f);
  p[3]=(x1<<2) | ((y1>>8)&0x03);
  p[4]=y1;
  return 0;
}

int set_map(struct map_session_data *sd,char *mapname,int x,int y)
{
  int i;
  if(sd==NULL)
    return -1;
  sd->x = x;
  sd->y = y;
  memcpy(sd->mapname,mapname,16);
  for(i=0;map[i][0];i++)
    if(strcmp(map[i],mapname)==0)
      break;
	if(map[i][0]){
    sd->mapno=i;
//	printf("mapno %d \n",i);
	}
  else
    return 1; // must send 0092?
  if(sd->x <0 || sd->x >= map_data[i].xs)
    sd->x=0;
  if(sd->y <0 || sd->y >= map_data[i].ys)
    sd->y=0;
  add_block(&sd->block,i,sd->x,sd->y);
  map_data[i].users++;
  return 0;
}

int wait_close(int tid,unsigned int tick,int id,int data)
{
  if(session[id]==NULL)
    return 0;
  session[id]->eof=1;
  return 0;
}

int parse_tochar(int fd)
{
  struct map_session_data *sd=NULL;
  int i,j,fdc;

  if(session[fd]->eof){
    if(fd==char_fd)
      char_fd=-1;
    printf("parse_tochar close %d\n",fd);
    close(fd);
    delete_session(fd);
    return 0;
  }
  //printf("parse_tochar : %d %d %d\n",fd,RFIFOREST(fd),RFIFOW(fd,0));
  while(RFIFOREST(fd)>=2){
    switch(RFIFOW(fd,0)){
    case 0x2af9:
      if(RFIFOREST(fd)<3)
	return 0;
      if(RFIFOB(fd,2)){
	printf("connect char server error : %d\n",RFIFOB(fd,2));
	exit(1);
      }
      RFIFOSKIP(fd,3);
      WFIFOW(fd,0)=0x2afa;
      for(i=0;map[i][0];i++){
	memcpy(WFIFOP(fd,4+i*16),map[i],16);
      }
      WFIFOW(fd,2)=4+i*16;
      WFIFOSET(fd,WFIFOW(fd,2));
      break;
    case 0x2afb:
      if(RFIFOREST(fd)<3)
	return 0;
      if(RFIFOB(fd,2)!=0){
	printf("send map error :%d\n",RFIFOB(fd,2));
	exit(1);
      }
      RFIFOSKIP(fd,3);
      break;
    case 0x2afd:
      if(RFIFOREST(fd)<4 || RFIFOREST(fd)<RFIFOW(fd,2))
	return 0;
      for(i=0;i<FD_SETSIZE;i++)
	if(session[i] && (sd=session[i]->session_data) && sd->account_id==RFIFOL(fd,4))
	  break;
      for(j=i+1;j<FD_SETSIZE;j++){
	struct map_session_data *tmp_sd;
	if(session[j] && (tmp_sd=session[j]->session_data) && tmp_sd->account_id==RFIFOL(fd,4))
	  break;
      }
      if(j!=FD_SETSIZE){
	//after certification, both players are dropped on a double login
	//printf("Output 1 : %d / Output 2 : %d",session[i]->wdata_size,session[i]->wdata);

/* Testing fix by removing packets to null connections... and just raw closing the socket.. (hud) */
	if ((i<FD_SETSIZE) && session[i] && session[i]->session_data)
	{
	  WFIFOW(i,0)=0x81;
	  WFIFOB(i,2)=8;
	  WFIFOSET(i,3);
	  add_timer(gettick()+100,wait_close,i,0);
	}
	if ((j<FD_SETSIZE) && session[j] && session[j]->session_data)
	{
	  WFIFOW(j,0)=0x81;
	  WFIFOB(j,2)=8;
	  WFIFOSET(j,3);
	  add_timer(gettick()+100,wait_close,j,0);
	}
      } else if(i!=FD_SETSIZE){
	fdc=i;
	memcpy(&sd->status,RFIFOP(fd,12),sizeof(sd->status));
	set_map(sd,sd->status.last_point.map , sd->status.last_point.x , sd->status.last_point.y);
	sd->speed = DEFAULT_WALK_SPEED;
	sd->sitting=0;
	sd->dir=0;
	sd->head_dir=0;
	WFIFOW(fdc,0)=0x73;
	WFIFOL(fdc,2)=gettick();
	set_pos(WFIFOP(fdc,6),sd->x,sd->y);
	WFIFOB(fdc,9)=5;
	WFIFOB(fdc,10)=5;
	WFIFOSET(fdc,11);
	sd->state.auth=1;

	// ADDED on 04/09/2003 -------------
	//pet_init(sd);
	sd->status.pet.pet_id_as_npc = -1;
	//printf("PET_ID=%d, ACTIVITY=%d\n",sd->status.pet.pet_id_as_npc,sd->status.pet.activity);
	// ---------------------------------
      }
      RFIFOSKIP(fd,RFIFOW(fd,2));
      break;
    case 0x2afe:
      if(RFIFOREST(fd)<7)
	return 0;
      for(i=0;i<FD_SETSIZE;i++)
	if(session[i] && (sd=session[i]->session_data) && sd->account_id==RFIFOL(fd,4))
	  break;
      if(i!=FD_SETSIZE){
	close(i);
	session[i]->eof=1;
      }
      RFIFOSKIP(fd,7);
      break;
    case 0x2b00:
      if(RFIFOREST(fd)<6)
	return 0;
      users_global=RFIFOL(fd,2);
      RFIFOSKIP(fd,6);
      break;
    case 0x2b03:
      if(RFIFOREST(fd)<7)
	return 0;
      for(i=0;i<FD_SETSIZE;i++)
	if(session[i] && (sd=session[i]->session_data) && sd->account_id==RFIFOL(fd,2))
	  break;
      if(i!=FD_SETSIZE){
		WFIFOW(i,0)=0xb3;
		WFIFOB(i,2)=1;
		WFIFOSET(i,3);
      }
      RFIFOSKIP(fd,7);
      break;

    default:
      printf("parse_tochar3 close %d %d\n",fd,RFIFOW(fd,0));
      close(fd);
      session[fd]->eof=1;
      return 0;
    }
  }
  return 0;
}

int mmo_map_sendblock(int m,int bx,int by,char *dat,int len,int srcfd,int wos)
{
  struct block_list *bl;
  struct map_session_data *srcsd,*dstsd;
  int fd,c=0;

  if(bx<0 || bx>=map_data[m].bxs ||
     by<0 || by>=map_data[m].bys)
    return 0;
  bl=map_data[m].block[bx + by*map_data[m].bxs].next;
  if(session[srcfd])
    srcsd=session[srcfd]->session_data;
  else
    srcsd=NULL;
  for(;bl;bl=bl->next){
    if(bl->type==BL_PC){
      dstsd=(struct map_session_data *)bl;
      fd=dstsd->fd;
      if(wos && fd == srcfd)
	continue;
      if(wos==2 && dstsd->chatID)
	continue;
      if(wos==3 && srcsd && srcsd->chatID==dstsd->chatID)
	continue;
      memcpy(WFIFOP(fd,0),dat,len);
      WFIFOSET(fd,len);
      c++;
    }
  }
  return c;
}

// wos = without self  0 - include srcfd  1- exclude srcfd
// 2- exclude all in chat users 3- exclude same chat joined user
int mmo_map_sendarea(int srcfd,char *dat,int len,int wos)
{
  struct map_session_data *srcsd;
  int bx,by,i,j;

  srcsd=session[srcfd]->session_data;
  for(by=srcsd->y/BLOCK_SIZE-AREA_SIZE,i=0;i<(AREA_SIZE*2+1);by++,i++){
    if(by < 0 || by >= map_data[srcsd->mapno].bys)
      continue;
    for(bx=srcsd->x/BLOCK_SIZE-AREA_SIZE,j=0;j<(AREA_SIZE*2+1);bx++,j++){
      if(bx < 0 || bx >= map_data[srcsd->mapno].bxs)
	continue;
      mmo_map_sendblock(srcsd->mapno,bx,by,dat,len,srcfd,wos);
    }
  }
  return 0;
}

int mmo_map_sendchat(int srcfd,char *dat,int len,int wos)
{
  struct map_session_data *srcsd,*dstsd;
  struct mmo_chat* chat;
  int fd,i;

  srcsd=session[srcfd]->session_data;
  if((chat=(struct mmo_chat*)srcsd->chatID) == NULL)
    return 0;
  for(i=0;i<chat->users;i++){
    fd=chat->usersfd[i];
    if(!session[fd] || !(dstsd=session[fd]->session_data))
      continue;
    if(wos && fd==srcfd)
      continue;
    memcpy(WFIFOP(fd,0),dat,len);
    WFIFOSET(fd,len);
  }
  return 0;
}

int mmo_map_sendarea_mxy(int m,int x,int y,char *dat,int len)
{
  int bx,by,i,j;

  for(by=y/BLOCK_SIZE-AREA_SIZE,i=0;i<(AREA_SIZE*2+1);by++,i++){
    if(by < 0 || by >= map_data[m].bys)
      continue;
    for(bx=x/BLOCK_SIZE-AREA_SIZE,j=0;j<(AREA_SIZE*2+1);bx++,j++){
      if(bx < 0 || bx >= map_data[m].bxs)
	continue;
      mmo_map_sendblock(m,bx,by,dat,len,0,0);
    }
  }
  return 0;
}

int mmo_map_sendall(int srcfd,char *dat,int len,int wos)
{
  struct map_session_data *srcsd,*dstsd;
  int fd;

  srcsd=session[srcfd]->session_data;
  for(fd=0;fd<FD_SETSIZE;fd++){
    if(!session[fd] || !(dstsd=session[fd]->session_data))
      continue;
    if(wos && fd==srcfd)
      continue;
    memcpy(WFIFOP(fd,0),dat,len);
    WFIFOSET(fd,len);
  }
  return 0;
}

int mmo_map_set_frameinitem(int fd,unsigned char *buf,struct flooritem_data *fitem)
{
  if(fitem->item_data.nameid==0)
    return 0;
  //009d <ID>.l <name ID>.w <identify flag>.B <X>.w <Y>.w <amount>.w <subX>.B <subY>.B
  WBUFW(buf,0)=0x9d;
  WBUFL(buf,2)=fitem->id;
  WBUFW(buf,6)=fitem->item_data.nameid;
  WBUFB(buf,8)=fitem->item_data.identify;
  WBUFW(buf,9)=fitem->x;
  WBUFW(buf,11)=fitem->y;
  WBUFW(buf,13)=fitem->item_data.amount;
  WBUFB(buf,15)=fitem->subx;
  WBUFB(buf,16)=fitem->suby;
  return 17;
}

int mmo_map_set_dropitem(int fd,unsigned char *buf,struct flooritem_data *fitem)
{
  if(fitem->item_data.nameid==0)
    return 0;
  //009e <ID>.l <name ID>.w <identify flag>.B <X>.w <Y>.w <subX>.B <subY>.B <amount>.w
  WBUFW(buf,0)=0x9e;
  WBUFL(buf,2)=fitem->id;
  WBUFW(buf,6)=fitem->item_data.nameid;
  WBUFB(buf,8)=fitem->item_data.identify;
  WBUFW(buf,9)=fitem->x;
  WBUFW(buf,11)=fitem->y;
  WBUFB(buf,13)=fitem->subx;
  WBUFB(buf,14)=fitem->suby;
  WBUFW(buf,15)=fitem->item_data.amount;
  return 17;
}

int mmo_map_set_npc(int fd,unsigned char *buf,unsigned long id, unsigned long class, int x, int y,int dir)
{
  WBUFW(buf,0)=0x78;
  WBUFL(buf,2)=id;
  WBUFW(buf,6)=200;
  WBUFW(buf,8)=0;
  WBUFW(buf,10)=0;
  WBUFW(buf,12)=0;
  WBUFW(buf,14)=class;
  WBUFW(buf,16)=0;
  WBUFW(buf,18)=0;
  WBUFW(buf,20)=0;
  WBUFW(buf,22)=0;
  WBUFW(buf,24)=0;
  WBUFW(buf,26)=0;
  WBUFW(buf,28)=0;
  WBUFW(buf,30)=0;
  WBUFW(buf,32)=0;
  WBUFW(buf,34)=0;
  WBUFW(buf,36)=0;
  WBUFW(buf,38)=0;
  WBUFW(buf,40)=0;
  WBUFW(buf,42)=0;
  WBUFB(buf,44)=0;
  WBUFB(buf,45)=0;
  set_pos(WBUFP(buf,46),x,y);
  WBUFB(buf,48)|=dir&0x0f;
  WBUFB(buf,49)=5;
  WBUFB(buf,50)=5;
  WBUFB(buf,51)=0;
  return 52;
}

int mmo_map_set_npc007c(int fd,unsigned char *buf,unsigned long id, unsigned long class, int x, int y)
{
  WBUFW(buf,0)=0x7c;
  WBUFL(buf,2)=id;
  WBUFW(buf,6)=200;
  WBUFW(buf,8)=0;
  WBUFW(buf,10)=0;
  WBUFW(buf,12)=0;
  WBUFW(buf,14)=0;
  WBUFW(buf,16)=0;
  WBUFW(buf,18)=0;
  WBUFW(buf,20)=class;
  WBUFW(buf,22)=0;
  WBUFW(buf,24)=0;
  WBUFW(buf,26)=0;
  WBUFW(buf,28)=0;
  WBUFW(buf,30)=0;
  WBUFW(buf,32)=0;
  WBUFW(buf,34)=0;
  set_pos(WBUFP(buf,36),x,y);
  WBUFB(buf,39)=0;
  WBUFB(buf,40)=0;
  return 41;
}

int mmo_map_set00b1(int fd,int type,int val)
{
	WFIFOW(fd,0) = 0xb1;
	WFIFOW(fd,2) = type;
	WFIFOL(fd,4) = val;
	WFIFOSET(fd,8);
	return 0;
}
/*various character settings */
int mmo_map_set0078(int fd,unsigned char *buf)
{
  struct map_session_data *sd;

  sd=session[fd]->session_data;

  WBUFW(buf,0)=0x78;
  WBUFL(buf,2)=sd->account_id;
  WBUFW(buf,6)=sd->speed;
  WBUFW(buf,8)=0;
  WBUFW(buf,10)=0;
  WBUFW(buf,12)=sd->status.option;
  WBUFW(buf,14)=sd->status.class;
  WBUFW(buf,16)=sd->status.hair;
  WBUFW(buf,18)=sd->status.weapon;
  WBUFW(buf,20)=sd->status.head_bottom;
  WBUFW(buf,22)=sd->status.sheild;
  WBUFW(buf,24)=sd->status.head_top;
  WBUFW(buf,26)=sd->status.head_mid;
  WBUFW(buf,28)=sd->status.hair_color;
  WBUFW(buf,30)=sd->status.clothes_color;
  WBUFW(buf,32)=sd->head_dir;
  WBUFW(buf,34)=sd->status.guild_id;
  WBUFW(buf,36)=0;
  WBUFW(buf,38)=0;
  WBUFW(buf,40)=sd->status.manner;
  WBUFW(buf,42)=sd->status.karma;
  WBUFB(buf,44)=0;
  WBUFB(buf,45)=sd->sex;
  set_pos(WBUFP(buf,46),sd->x,sd->y);
  WBUFB(buf,48)|=sd->dir&0x0f;
  WBUFB(buf,49)=5;
  WBUFB(buf,50)=5;
  WBUFB(buf,51)=sd->sitting;
  return 0;
}

int mmo_map_set0079(int fd,unsigned char *buf)
{
  mmo_map_set0078(fd,buf);
  WBUFW(buf,0)=0x79;
  return 0;
}

int mmo_map_set007b(int fd,unsigned char *buf,unsigned long tick,int x0,int y0,int x1,int y1)
{
  struct map_session_data *sd;

  sd=session[fd]->session_data;

  WBUFW(buf,0)=0x7b;
  WBUFL(buf,2)=sd->account_id;
  WBUFW(buf,6)=sd->speed;
  WBUFW(buf,8)=0;
  WBUFW(buf,10)=0;
  WBUFW(buf,12)=sd->status.option;
  WBUFW(buf,14)=sd->status.class;
  WBUFW(buf,16)=sd->status.hair;
  WBUFW(buf,18)=sd->status.weapon;
  WBUFW(buf,20)=sd->status.head_bottom;
  WBUFL(buf,22)=tick;
  WBUFW(buf,26)=sd->status.sheild;
  WBUFW(buf,28)=sd->status.head_top;
  WBUFW(buf,30)=sd->status.head_mid;
  WBUFW(buf,32)=sd->status.hair_color;
  WBUFW(buf,34)=sd->status.clothes_color;
  WBUFW(buf,36)=sd->head_dir;
  WBUFW(buf,38)=sd->status.guild_id;
  WBUFW(buf,40)=0;
  WBUFW(buf,42)=0;
  WBUFW(buf,44)=sd->status.manner;
  WBUFW(buf,46)=sd->status.karma;
  WBUFB(buf,48)=0;
  WBUFB(buf,49)=sd->sex;
  set_2pos(WBUFP(buf,50),x0,y0,x1,y1);
  WBUFB(buf,55)=0;
  WBUFB(buf,56)=5;
  WBUFB(buf,57)=5;
  return 0;
}

/**********************************************
acquire name function
**********************************************/
int mmo_map_send0095(int fd,unsigned long id)
{
  	int i,char_id=0;
  	struct map_session_data *sd;
	sd = NULL;
	//printf("fd:%d,ID:%d\n",fd,id);
	//Fred Fix5 (Username Showing in Map)
	for(i=5;i<FD_SETSIZE;i++){
			if(session[i] && (sd=session[i]->session_data) &&  sd->account_id==id){
				char_id=sd->char_id;
				break;
				}
	}
	if(char_id){ // PC
		if(strcmp(sd->status.guild_name,"not post")!=0){//if guild post
			//printf("%s\n%s\n%s\n%s",sd->status.name,sd->status.party_name,sd->status.guild_name,sd->status.class_name);
			WFIFOW(fd,0)=0x195;//name packet
			WFIFOL(fd,2)=id;
			memcpy(WFIFOP(fd,6),sd->status.name,24);//increases as it sends name
			memcpy(WFIFOP(fd,30),sd->status.party_name,24);
			memcpy(WFIFOP(fd,54),sd->status.guild_name,24);
			memcpy(WFIFOP(fd,78),sd->status.class_name,24);
			WFIFOSET(fd,packet_len_table[0x195]);
		}else{
			WFIFOW(fd,0)=0x95;//name packet
			WFIFOL(fd,2)=id;
			memcpy(WFIFOP(fd,6),sd->status.name,24);//increases as it sends name
			WFIFOSET(fd,packet_len_table[0x95]);
		}

	return 0;
  	} else { // NPC
    sd=session[fd]->session_data;
    for(i=0;i<map_data[sd->mapno].npc_num;i++){
      if(map_data[sd->mapno].npc[i]->id==id){
	WFIFOW(fd,0)=0x95;
	WFIFOL(fd,2)=id;
	memcpy(WFIFOP(fd,6),map_data[sd->mapno].npc[i]->name,24);
	WFIFOSET(fd,30);
	return 0;
      }
    }
  }

  return -1;
}

void equip_check(int fd){
	int i;
	struct map_session_data *sd;
	sd=session[fd]->session_data;
	// equip check&modify
	for(i=0;i<100;i++){
	  if(sd->status.inventory[i].nameid &&
	     (sd->status.inventory[i].equip)){
			 mmo_map_calc_status(fd,sd->status.inventory[i].nameid,1);//status addition
		}
	}
}

int mmo_map_set_look(int fd,unsigned char *buf,int id,int type,int val)
{
  //printf("mmo_map_set_look : %d,%d,%d\n",id,type,val);
  WBUFW(buf,0)=0xc3;
  WBUFL(buf,2)=id;
  WBUFB(buf,6)=type;
  WBUFB(buf,7)=val;
  return 8;
}

int calc_need_status_point(struct map_session_data *sd,int type)
{
  int val;

  if(type<SP_STR || type>SP_LUK)
    return -1;
  val =
    type==SP_STR ? sd->status.str :
    type==SP_AGI ? sd->status.agi :
    type==SP_VIT ? sd->status.vit :
    type==SP_INT ? sd->status.int_:
    type==SP_DEX ? sd->status.dex : sd->status.luk;
  return (val+9)/10+1;
}

/*parameter setting*/
int mmo_map_set_param(int fd,unsigned char *buf,int type)
{

  int len=8;
  struct map_session_data *sd;

  sd=session[fd]->session_data;
  WBUFW(buf,0)=0xb0;
  WBUFW(buf,2)=type;
  switch(type){
  case SP_STATUSPOINT:
    WBUFL(buf,4)=sd->status.status_point;
    break;
  case SP_ASPD:
    WBUFL(buf,4)=sd->status.aspd;
    break;
  case SP_ATK1:
    WBUFW(buf,4)=sd->status.atk1;
    break;
  case SP_MATK1:
    WBUFL(buf,4)=sd->status.matk1;
    break;
  case SP_MATK2:
    WBUFL(buf,4)=sd->status.matk2;
    break;
  case SP_DEF2:
    WBUFW(buf,4) = sd->status.def2;
    break;
  case SP_MDEF2:
    WBUFW(buf,4) = sd->status.mdef2;
    break;
  case SP_HIT:
    WBUFL(buf,4) = sd->status.hit;
  case SP_FLEE1:
    WBUFW(buf,4) = sd->status.flee1;
    break;
  case SP_FLEE2:
    WBUFW(buf,4) = sd->status.flee2;
    break;
  case SP_SKILLPOINT:
    WBUFL(buf,4)=sd->status.skill_point;
    break;
  case SP_ZENY:
    WBUFW(buf,0)=0xb1;
    WBUFL(buf,4)=sd->status.zeny;
    break;
  case SP_WEIGHT:
    WBUFL(buf,4)=sd->weight;
    break;
  case SP_MAXWEIGHT:
    WBUFL(buf,4)=sd->max_weight;
    break;
  case SP_STR:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.str;
    WBUFL(buf,10)=0; // str bonus
    len=14;
    break;
  case SP_AGI:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.agi;
    WBUFL(buf,10)=0; // agi bonus
    len=14;
    break;
  case SP_VIT:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.vit;
    WBUFL(buf,10)=0; // vit bonus
    len=14;
    break;
  case SP_INT:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.int_;
    WBUFL(buf,10)=0; // int bonus
    len=14;
    break;
  case SP_DEX:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.dex;
    WBUFL(buf,10)=0; // dex bonus
    len=14;
    break;
  case SP_LUK:
    WBUFW(buf,0)=0x141;
    WBUFL(buf,2)=type;
    WBUFL(buf,6)=sd->status.luk;
    WBUFL(buf,10)=0; // luk bonus
    len=14;
    break;
  default:
    len=0;
    break;
  }
  return len;
}

int mmo_map_update_param(int fd,int type,int val)
{
 struct map_session_data *sd;
 int len;
 sd=session[fd]->session_data;
 switch(type){
 case SP_ZENY:
   sd->status.zeny=sd->status.zeny+val;
   break;
 case SP_STATUSPOINT:
   sd->status.status_point=sd->status.status_point+val;
   break;
 case SP_SKILLPOINT:
   sd->status.skill_point=sd->status.skill_point+val;
   break;
 default:
   break;
 }
 len=mmo_map_set_param(fd,WFIFOP(fd,0),type);
 if(len>0) WFIFOSET(fd,len);
 return 0;
}


//function for sigma operation
int mmo_map_calc_sigma(int k,double val)
{
	int i;
	double j=0;
	for(i=1;i<=k;i++){
		j = (double)(i*val) + j;
	}
	return (int)j;
}

//functions for recalculating status�f
int mmo_map_calc_status(int fd,int item_num,int equip){
	struct item_db2 item_equip;
	struct map_session_data *sd;
	struct mmo_charstatus *p;
	short temp_str;
	short temp_agi;
	short temp_vit;
	short temp_int;
	short temp_dex;
	short temp_luk;
	if(item_num != 0)
		item_equip = item_database(item_num);
	sd=session[fd]->session_data;
	p=&sd->status;
	if(item_num != 0){
		//sword mastery wisc
		if(itemdb_hands(item_num)==1) item_equip.atk += ( sd->status.skill[2 -1].lv *5);
		if(itemdb_hands(item_num)==2) item_equip.atk += ( sd->status.skill[3 -1].lv *5);

		if(equip){
			p->str2 += item_equip.str;
			p->agi2 += item_equip.agi;
			p->dex2 += item_equip.dex;
			p->vit2 += item_equip.vit;
			p->int_2 += item_equip.int_;
			p->luk2 += item_equip.luk;
		}
		else
		{
			p->str2 -= item_equip.str;
			p->agi2 -= item_equip.agi;
			p->dex2 -= item_equip.dex;
			p->vit2 -= item_equip.vit;
			p->int_2 -= item_equip.int_;
			p->luk2 -= item_equip.luk;
		}
	}
	else
	{
		p->str2 = 0;
		p->agi2 = 0;
		p->dex2 = 0;
		p->vit2 = 0;
		p->int_2 = 0;
		p->luk2 = 0;
	}

	p->dex2 = p->skill[43-1].lv;	//correction on Owl�fs Eye skill

	temp_str=p->str + p->str2;
	temp_agi=p->agi + p->agi2;
	temp_vit=p->vit + p->vit2;
	temp_int=p->int_+ p->int_2;
	temp_dex=p->dex + p->dex2;
	temp_luk=p->luk + p->luk2;

	//HP calculation
	switch(p->class){
	case 0://novice
		p->max_hp = (double)(35 + p->base_level*5)*(double)(1+temp_vit/100);
		p->max_sp = p->base_level*2*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30;
		break;
	case 1://swordsman
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.7))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*2*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 800;
		break;
	case 2://mage
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.3))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*6*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 200;
		break;
	case 3://archer
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.5))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*2*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 600;
		break;
	case 4://acolyte
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.4))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*5*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 400;
		break;
	case 5://merchant
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.4))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*3*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 800;
		break;
	case 6://theif
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.5))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*2*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 400;
		break;
	case 7://knight
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,1.5))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*3*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 800;
		break;
	case 8://priest
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.75))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*7*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 600;
		break;
	case 9://wizard
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.55))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*9*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 400;
		break;
	case 10://blacksmith
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.9))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*4*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 1000;
		break;
	case 11://hunter
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,0.85))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*4*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 700;
		break;
	case 12://assassin
		p->max_hp = (double)(35 + p->base_level*5 + mmo_map_calc_sigma(p->base_level,1.1))*(double)(1+((double)temp_vit/100));
		p->max_sp = p->base_level*4*(double)(1+((double)temp_int/100));
		sd->max_weight = 2000 + p->str*30 + 400;
		break;
	}

	if(p->max_hp > 30000 || p->max_hp < 0){//max HP available is 30000
		p->max_hp = 30000;
	}

	//ATK,ASPD,MATK,DEF,MDEF,HIT,FLEE calculations
	if(item_num != 0){
		if(equip){
			p->atk1 = temp_str + (temp_str/10)*(temp_str/10) + (temp_dex/5);
			if(item_equip.atk != 0)
			p->atk2 += item_equip.atk;
			p->aspd = (long)((def_aspd[sd->status.class][0])*(float)(((250-(temp_agi)-((temp_dex)/4.0f)))/250.0f));
			p->matk1 = temp_int + (temp_int/5)*(temp_int/5);
			if(item_equip.matk != 0)
				p->matk1 += item_equip.matk;
			p->matk2 = temp_int + (temp_int/7)*(temp_int/7);
			p->def1 = temp_vit;
			if(item_equip.def != 0)
				p->def2 += item_equip.def;
			p->mdef1 = temp_int;
			if(item_equip.mdef != 0)
				p->mdef2 += item_equip.mdef;
			p->hit = sd->status.base_level + temp_dex + item_equip.hit;
			p->flee1 = sd->status.base_level + temp_agi + item_equip.flee;
			p->flee2 = (temp_luk/10)+1;
			p->critical = 1 + temp_luk/3 + item_equip.critical;
		}
		else
		{
			p->atk1 = temp_str + (temp_str/10)*(temp_str/10) + (temp_dex/5);
			if(item_equip.atk != 0)
				p->atk2 -= item_equip.atk;
			p->aspd = (long)((def_aspd[sd->status.class][0])*(float)(((250-(temp_agi)-((temp_dex)/4.0f)))/250.0f));
			p->matk1 = temp_int + (temp_int/5)*(temp_int/5);
			if(item_equip.matk != 0)
				p->matk1 -= item_equip.matk;
			p->matk2 = temp_int + (temp_int/7)*(temp_int/7);
			p->def1 = temp_vit;
			if(item_equip.def != 0)
				p->def2 -= item_equip.def;
			p->mdef1 = temp_int;
			if(item_equip.mdef != 0)
				p->mdef2 -= item_equip.mdef;
			p->hit = sd->status.base_level + temp_dex - item_equip.hit;
			p->flee1 = sd->status.base_level + temp_agi - item_equip.flee;
			p->flee2 = (temp_luk/10)+1;
			p->critical = 1 + temp_luk/3 - item_equip.critical;
		}
	}
	else
	{
			p->atk1 = temp_str + (temp_str/10)*(temp_str/10) + (temp_dex/5);
			p->atk2 = 1;
			p->aspd = (long)((def_aspd[sd->status.class][0])*(float)(((250-(temp_agi)-((temp_dex)/4.0f)))/250.0f));
			p->matk1 = temp_int + (temp_int/5)*(temp_int/5);
			p->matk2 = temp_int + (temp_int/7)*(temp_int/7);
			p->def1 = temp_vit;
			p->def2 = 1;
			p->mdef1 = temp_int;
			p->mdef2 = 1;
			p->hit = sd->status.base_level + temp_dex;
			p->flee1 = sd->status.base_level + temp_agi;
			p->flee2 = (temp_luk/10)+1;
			p->critical = 1 + temp_luk/3;
			equip_check(fd);
	}

	//MAX_HP renewal
	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x06;
	WFIFOL(fd,4) = p->max_hp;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x08;
	WFIFOL(fd,4) = p->max_sp;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x19;
	WFIFOL(fd,4) = sd->max_weight*10;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_STR;
	WFIFOL(fd,6) = p->str;
	WFIFOL(fd,10) = p->str2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_AGI;
	WFIFOL(fd,6) = p->agi;
	WFIFOL(fd,10) = p->agi2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_VIT;
	WFIFOL(fd,6) = p->vit;
	WFIFOL(fd,10) = p->vit2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_INT;
	WFIFOL(fd,6) = p->int_;
	WFIFOL(fd,10) = p->int_2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_DEX;
	WFIFOL(fd,6) = p->dex;
	WFIFOL(fd,10) = p->dex2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0x141;
	WFIFOL(fd,2) = SP_LUK;
	WFIFOL(fd,6) = p->luk;
	WFIFOL(fd,10) = p->luk2;
	WFIFOSET(fd,14);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x29;
	WFIFOL(fd,4) = p->atk1;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2a;
	WFIFOL(fd,4) = p->atk2;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2b;
	WFIFOL(fd,4) = p->matk1;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2c;
	WFIFOL(fd,4) = p->matk2;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2d;
	WFIFOL(fd,4) = p->def1;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2e;
	WFIFOL(fd,4) = p->def2;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x2f;
	WFIFOL(fd,4) = p->mdef1;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x30;
	WFIFOL(fd,4) = p->mdef2;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x31;
	WFIFOL(fd,4) = p->hit;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x32;
	WFIFOL(fd,4) = p->flee1;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x33;
	WFIFOL(fd,4) = p->flee2;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x34;
	WFIFOL(fd,4) = p->critical;
	WFIFOSET(fd,8);

	WFIFOW(fd,0) = 0xb0;
	WFIFOW(fd,2) = 0x35;
	WFIFOL(fd,4) = p->aspd;
	WFIFOSET(fd,8);

	return 0;
}

//skill lvl up proccessing
void mmo_map_skill_lv_up(int fd,int skill_id)
{
	int len;
	struct map_session_data *sd;
	skill_id--;
	sd=session[fd]->session_data;
	sd->status.skill[skill_id].lv++;
	WFIFOW(fd,0) = 0x10e;
	WFIFOW(fd,2) = skill_id;
	WFIFOW(fd,4) = sd->status.skill[skill_id].lv;
	WFIFOW(fd,6) = skill_db[skill_id].sp;
	WFIFOW(fd,8) = skill_db[skill_id].range;
	WFIFOB(fd,10) = 1;//skill point up
	WFIFOSET(fd,11);
	len = mmo_map_all_skill(fd,WFIFOP(fd,0),sd->status.skill_point,0);
    if(len>0) WFIFOSET(fd,len);
	mmo_map_update_param(fd,SP_SKILLPOINT,-1);
}

void mmo_map_job_lv_up(int fd,int val)
{
	int i,len;
//	int c;
//	struct skill *n_skill;
	struct map_session_data *sd;
	struct mmo_charstatus *m_cs;

	sd=session[fd]->session_data;
	m_cs=&sd->status;

	for(i=0;i<val;i++){
		m_cs->job_level++;
		//Lv up renewal
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x37;
		WFIFOL(fd,4) = m_cs->job_level;
		WFIFOSET(fd,8);

		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x0c;
		WFIFOL(fd,4) = val;
		WFIFOSET(fd,8);
	}
	len = mmo_map_all_skill(fd,WFIFOP(fd,0),sd->status.skill_point,0);
	 if(len>0) WFIFOSET(fd,len);
	/*
	WFIFOW(fd,0)=0x10f;
	for(i=c=0;i<MAX_SKILL;i++){
    n_skill=&sd->status.skill[i];
	 if(n_skill->id==0)
	 continue;
    WFIFOW(fd,4+c*37)= n_skill->id;	//ID 1 basic skills?
    WFIFOW(fd,4+c*37+2)=skill_db[i+1].type_inf;	//target type 0-passive 1-enemy 2-place 4-immidiatery 16-ally
    WFIFOW(fd,4+c*37+4)=0;	//???
    WFIFOW(fd,4+c*37+6)=n_skill->lv+1;	//lv
    WFIFOW(fd,4+c*37+8)=skill_db[i+1].sp;//sp
    WFIFOW(fd,4+c*37+10)=skill_db[i+1].range;//range
    memcpy(WFIFOP(fd,4+c*37+12),"",24);	//name
    WFIFOB(fd,4+c*37+36)=val;	//up
    c++;
  }
  */
}

//Lvl up processing
void mmo_map_level_up(int fd,int val)
{
	int i=0;
	struct map_session_data *sd;
	struct mmo_charstatus *m_cs;

	sd=session[fd]->session_data;
	m_cs=&sd->status;

	if(sd->status.base_level <= 99){
	for(i=0;i<val;i++){
		m_cs->base_level++;

		//status point acquisition
		if(m_cs->base_level<5){
			mmo_map_update_param(fd,SP_STATUSPOINT,3);
		}else if(m_cs->base_level<10){
			mmo_map_update_param(fd,SP_STATUSPOINT,4);
		}else if(m_cs->base_level<15){
			mmo_map_update_param(fd,SP_STATUSPOINT,5);
		}else if(m_cs->base_level<20){
			mmo_map_update_param(fd,SP_STATUSPOINT,6);
		}else if(m_cs->base_level<25){
			mmo_map_update_param(fd,SP_STATUSPOINT,7);
		}else if(m_cs->base_level<30){
			mmo_map_update_param(fd,SP_STATUSPOINT,8);
		}else if(m_cs->base_level<35){
			mmo_map_update_param(fd,SP_STATUSPOINT,9);
		}else if(m_cs->base_level<40){
			mmo_map_update_param(fd,SP_STATUSPOINT,10);
		}else if(m_cs->base_level<45){
			mmo_map_update_param(fd,SP_STATUSPOINT,11);
		}else if(m_cs->base_level<50){
			mmo_map_update_param(fd,SP_STATUSPOINT,12);
		}else if(m_cs->base_level<55){
			mmo_map_update_param(fd,SP_STATUSPOINT,13);
		}else if(m_cs->base_level<60){
			mmo_map_update_param(fd,SP_STATUSPOINT,14);
		}else if(m_cs->base_level<65){
			mmo_map_update_param(fd,SP_STATUSPOINT,15);
		}else if(m_cs->base_level<70){
			mmo_map_update_param(fd,SP_STATUSPOINT,16);
		}else if(m_cs->base_level<75){
			mmo_map_update_param(fd,SP_STATUSPOINT,17);
		}else if(m_cs->base_level<80){
			mmo_map_update_param(fd,SP_STATUSPOINT,18);
		}else if(m_cs->base_level<85){
			mmo_map_update_param(fd,SP_STATUSPOINT,19);
		}else if(m_cs->base_level<90){
			mmo_map_update_param(fd,SP_STATUSPOINT,20);
		}else if(m_cs->base_level<95){
			mmo_map_update_param(fd,SP_STATUSPOINT,21);
		}else if(m_cs->base_level<100){
			mmo_map_update_param(fd,SP_STATUSPOINT,22);
		}else{//Lv100 and above...
			mmo_map_update_param(fd,SP_STATUSPOINT,30);
			}
		}
		//Lv up renewal
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x0b;
		WFIFOL(fd,4) = m_cs->base_level;
		WFIFOSET(fd,8);
		mmo_map_calc_status(fd,0,0);//status recalculation
		//HP,SP maximum value modification
		m_cs->hp = m_cs->max_hp;
		m_cs->sp = m_cs->max_sp;

		//HP,MAXHP,SP,MAXSP renewal
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x05;
		WFIFOL(fd,4) = m_cs->hp;
		WFIFOSET(fd,8);
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x06;
		WFIFOL(fd,4) = m_cs->max_hp;
		WFIFOSET(fd,8);
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x07;
		WFIFOL(fd,4) = m_cs->sp;
		WFIFOSET(fd,8);
		WFIFOW(fd,0) = 0xb0;
		WFIFOW(fd,2) = 0x08;
		WFIFOL(fd,4) = m_cs->max_sp;
		WFIFOSET(fd,8);
	}
}


int mmo_map_item_get(int fd,unsigned char* buf, struct item* item)
{
  struct map_session_data *sd;
  struct mmo_charstatus *m_cs;
  struct item *n_item;
  int c=0,len;

  sd=session[fd]->session_data;
  m_cs=&sd->status;

  if(!itemdb_isequip(item->nameid)) {
    for(c=0;c<MAX_INVENTORY;c++) {
      n_item=&m_cs->inventory[c];
      if(n_item->nameid==item->nameid) {
	n_item->amount+=item->amount;
	//printf("already exist item add\n");
	break;
      }
    }
  }
  if(c==MAX_INVENTORY || itemdb_isequip(item->nameid)) {
    for(c=0;c<MAX_INVENTORY;c++) {
      n_item=&m_cs->inventory[c];
      if(n_item->nameid)
	continue;
      else {
	memcpy(n_item, item, sizeof(*item));
	//printf("equip or new item add\n");
 	break;
      }
    }
  }
  if(c==MAX_INVENTORY)
    return -1;
  if(sd->weight + itemdb_weight(item->nameid)*item->amount > sd->max_weight )
    return -2;

  sd->weight += itemdb_weight(item->nameid)*item->amount;

  //00a0 <index>.w <amount>.w <item ID>.w <identify flag>.B <attribute?>.B <refine>.B <card>.4w <equip type>.w <type>.B <fail>.B
  n_item=&m_cs->inventory[c];
  WBUFW(buf,0)=0xa0;
  WBUFW(buf,2)=c+2;
  WBUFW(buf,4)=item->amount;
  WBUFW(buf,6)=n_item->nameid;
  WBUFB(buf,8)=n_item->identify;
  WBUFB(buf,9)=n_item->attribute;
  WBUFB(buf,10)=n_item->refine;
  WBUFW(buf,11)=n_item->card[0];
  WBUFW(buf,13)=n_item->card[1];
  WBUFW(buf,15)=n_item->card[2];
  WBUFW(buf,17)=n_item->card[3];
  WBUFW(buf,19)=itemdb_equip_point(n_item->nameid,sd);
  WBUFB(buf,21)=itemdb_type(n_item->nameid);
  WBUFB(buf,22)=0;

  len=mmo_map_set_param(fd,WBUFP(buf,23),SP_WEIGHT);

  return len+23;
}

int mmo_map_item_lost(int fd,unsigned char* buf, short index, short amount)
{
  struct map_session_data *sd;
  struct mmo_charstatus *m_cs;
  struct item *n_item;
  int len;

  sd=session[fd]->session_data;
  m_cs=&sd->status;
  if(index>=MAX_INVENTORY+2 || index<2)
    return -1;
  n_item=&m_cs->inventory[index-2];
  if(!n_item->nameid)
    return -1;
  if(n_item->amount<amount)
    return -1;
  n_item->amount-=amount;
  if(n_item->amount==0)
    n_item->nameid=0;

  sd->weight-=itemdb_weight(n_item->nameid)*amount;
  len=mmo_map_set_param(fd,WBUFP(buf,0),SP_WEIGHT);
  WBUFW(buf,len+0)=0xaf;
  WBUFW(buf,len+2)=index;
  WBUFW(buf,len+4)=amount;

  return len+6;
}
int mmo_map_item_lost_silent(int fd,unsigned char* buf, short index, short amount)
{
 struct map_session_data *sd;
 struct mmo_charstatus *m_cs;
 struct item *n_item;
 int len;
 sd=session[fd]->session_data;
 m_cs=&sd->status;
 if(index>=MAX_INVENTORY+2 || index<2)
   return -1;
 n_item=&m_cs->inventory[index-2];
 if(!n_item->nameid)
   return -1;
 if(n_item->amount<amount)
   return -1;
 n_item->amount-=amount;
 if(n_item->amount==0)
   n_item->nameid=0;
 sd->weight-=itemdb_weight(n_item->nameid)*amount;
 len=mmo_map_set_param(fd,WBUFP(buf,0),SP_WEIGHT);
 return 0;
}
int mmo_map_set00d7(int fd,unsigned char *buf)
{
  struct mmo_chat* temp_chat;
  struct map_session_data *sd;

  if(session[fd]==NULL || (sd=session[fd]->session_data)==NULL)
    return 0;
  if((temp_chat=(struct mmo_chat*)(sd->chatID))==NULL)
    return 0;

  WBUFW(buf, 0) = 0xd7;
  WBUFW(buf, 2) = strlen(temp_chat->title)+17;	/* len */
  WBUFL(buf, 4) = temp_chat->ownID; /* ownID */
  WBUFL(buf, 8) = temp_chat->chatID;	/* chatID */
  WBUFW(buf,12) = temp_chat->limit;  /* limit */
  WBUFW(buf,14) = temp_chat->users;  /* users */
  WBUFB(buf,16) = temp_chat->pub;  /* pub */
  strcpy(WBUFP(buf,17),temp_chat->title);  /* title */
  return WBUFW(buf,2);
}

int mmo_map_getblockchar(int fd,int m,int bx,int by)
{
  struct block_list *bl;
  int c=0,len;

  if(bx<0 || bx>=map_data[m].bxs ||
     by<0 || by>=map_data[m].bys)
    return 0;
  //printf("getblockchar (%d,%d,%d)\n",m,bx,by);
  bl=map_data[m].block[bx + by*map_data[m].bxs].next;
  for(;bl;bl=bl->next){
    if(bl->type==BL_PC){
      struct map_session_data *srcsd;
      struct mmo_chat* temp_chat;

      srcsd=(struct map_session_data *)bl;
      if(fd == srcsd->fd)
	continue;
      //printf("send 0078 pc%08x\n",srcsd->account_id);
      mmo_map_set0078(srcsd->fd,WFIFOP(fd,0));
      WFIFOSET(fd,packet_len_table[0x78]);
      if((temp_chat=(struct mmo_chat*)srcsd->chatID) &&
	 temp_chat->ownID == srcsd->account_id){
	len=mmo_map_set00d7(srcsd->fd,WFIFOP(fd,0));
	if(len>0)
	  WFIFOSET(fd,len);
      }
      c++;
    } else if(bl->type==BL_NPC){
      struct npc_data *nd;
      nd=(struct npc_data*)bl;
      //printf("send 0078 npc%08x\n",nd->id);
      mmo_map_set_npc(fd,WFIFOP(fd,0),nd->id,nd->class,nd->x,nd->y,nd->dir);
      WFIFOSET(fd,packet_len_table[0x78]);
    } else if(bl->type==BL_ITEM){
      struct flooritem_data *fid;

      fid=(struct flooritem_data*)bl;
      len=mmo_map_set_frameinitem(fd,WFIFOP(fd,0),fid);
      if(len>0)
	WFIFOSET(fd,len);
    }
  }
  return c;
}

/* clear block character */
int mmo_map_clrblockchar(int fd,int m,int bx,int by)
{
  struct block_list *bl;
  int c=0;

  if(bx<0 || bx>=map_data[m].bxs ||
     by<0 || by>=map_data[m].bys)
    return 0;
  bl=map_data[m].block[bx + by*map_data[m].bxs].next;
  for(;bl;bl=bl->next){
    if(bl->type==BL_PC){//character
      struct map_session_data *srcsd;
      struct mmo_chat* temp_chat;
      srcsd=(struct map_session_data *)bl;
      if(fd == srcsd->fd)
	continue;
      WFIFOW(fd,0)=0x80;
      WFIFOL(fd,2)=srcsd->account_id;
      WFIFOB(fd,6)=0; // no effect?
      WFIFOSET(fd,packet_len_table[0x80]);
      if((temp_chat=(struct mmo_chat*)srcsd->chatID) &&
	 temp_chat->ownID == srcsd->account_id){
	WFIFOW(fd,0)=0xd8;
	WFIFOW(fd,2)=srcsd->chatID;
	WFIFOSET(fd,packet_len_table[0xd8]);
      }
      c++;
    } else if(bl->type==BL_NPC){//NPC
      struct npc_data *nd;
      nd=(struct npc_data*)bl;
      WFIFOW(fd,0)=0x80;
      WFIFOL(fd,2)=nd->id;
      WFIFOB(fd,6)=0; // no effect?
      WFIFOSET(fd,packet_len_table[0x80]);
	}else if(!bl->subtype){ //DJP Fix1
    }else{
       if(bl->subtype==MONS){//monster
    	  struct npc_data *nd;
          nd=(struct npc_data*)bl;
          WFIFOW(fd,0)=0x80;
          WFIFOL(fd,2)=nd->id;
          WFIFOB(fd,6)=0; //fadeout(keeps fading out)
          WFIFOSET(fd,packet_len_table[0x80]);
    }
   }     
  }
  return c;
}

int mmo_map_getareachar(int fd)
{
  int i,j,bx,by;
  struct map_session_data *srcsd;

  srcsd=session[fd]->session_data;
  for(by=srcsd->y/BLOCK_SIZE-AREA_SIZE,i=0;i<(AREA_SIZE*2+1);by++,i++){
    if(by < 0 || by >= map_data[srcsd->mapno].bys)
      continue;
    for(bx=srcsd->x/BLOCK_SIZE-AREA_SIZE,j=0;j<(AREA_SIZE*2+1);bx++,j++){
      if(bx < 0 || bx >= map_data[srcsd->mapno].bxs)
	continue;
      mmo_map_getblockchar(fd,srcsd->mapno,bx,by);
    }
  }

#if 0
  // --- skill effect test code ---
  if(strcmp(srcsd->mapname,"prontera.gat"))
    return 0;
  for(i=0;i<256;i++){
    len=mmo_map_set_skilleffectarea(fd,WFIFOP(fd,0),49999-i, srcsd->account_id, i, 159-(i/32*2), 191-(i%32*2));
    if(len>0) WFIFOSET(fd,len);
  }

#endif
#if 0
  // --- NPC test code ---
  if(strcmp(srcsd->mapname,"prontera.gat"))
    return 0;
  len=mmo_map_set_npc(fd,WFIFOP(fd,0),1, 0x70, 156, 195);
  WFIFOSET(fd,len);
  for(i=45;i<126;i++){
    if(i==0x68 || i==0x6f)
      continue;
    mmo_map_set_npc(fd,WFIFOP(fd,0),i-45+20, i, 159-((i-45)/20*2), 191-((i-45)%20*2));
    WFIFOSET(fd,packet_len_table[0x78]);
  }
  mmo_map_set_npc(fd,WFIFOP(fd,0),i-45+20, 0x2bc, 159-((i-45)/20*2), 191-((i-45)%20*2));
  WFIFOSET(fd,packet_len_table[0x78]);
  i++;
  mmo_map_set_npc(fd,WFIFOP(fd,0),i-45+20, 1039, 159-((i-45)/20*2), 191-((i-45)%20*2)); // Baphomet
  WFIFOSET(fd,packet_len_table[0x78]);
#endif
  return 0;
}

int mmo_map_sendwis(int fd)
{
  int i;
  struct map_session_data *srcsd,*dstsd;

  if(!session[fd] || !(srcsd=session[fd]->session_data))
    return -1;
  for(i=0;i<FD_SETSIZE;i++){
    if(!session[i] || fd==i || !(dstsd=session[i]->session_data))
      continue;
    if(strcmp(dstsd->status.name,RFIFOP(fd,4))==0){
      WFIFOW(i,0)=0x97;
      WFIFOW(i,2)=RFIFOW(fd,2);
      memcpy(WFIFOP(i,4),srcsd->status.name,24);
      memcpy(WFIFOP(i,28),RFIFOP(fd,28),RFIFOW(fd,2)-28);
      WFIFOSET(i,RFIFOW(fd,2));
      break;
    }
  }
  WFIFOW(fd,0)=0x98;
  WFIFOB(fd,2)= i==FD_SETSIZE;
  WFIFOSET(fd,3);
  return 0;
}

int mmo_map_all_nonequip(int fd,unsigned char *buf)
{
  int i,c;
  struct map_session_data *sd;
  struct item *n_item;

  sd=session[fd]->session_data;
  WBUFW(buf,0)=0xa3;
  for(i=c=0;i<MAX_INVENTORY;i++){
    n_item=&sd->status.inventory[i];
    if(n_item->nameid==0 || itemdb_isequip(n_item->nameid))
      continue;
    WBUFW(buf,4+c*10)=i+2;
    WBUFW(buf,4+c*10+2)=n_item->nameid;
    WBUFB(buf,4+c*10+4)=itemdb_type(n_item->nameid);
    WBUFB(buf,4+c*10+5)=n_item->identify;
    WBUFW(buf,4+c*10+6)=n_item->amount;
    WBUFB(buf,4+c*10+8)=0;
    WBUFB(buf,4+c*10+9)=0;	// while equipping arrow 0x80?
    c++;
  }
  if(c==0)
    return 0;
  WBUFW(buf,2)=4+c*10;
  return 4+c*10;
}

int mmo_map_all_equip(int fd,unsigned char *buf)
{
  int i,c;
  struct map_session_data *sd;
  struct item *n_item;

  sd=session[fd]->session_data;
  WBUFW(buf,0)=0xa4;
  for(i=c=0;i<MAX_INVENTORY;i++){
    n_item=&sd->status.inventory[i];
    if(n_item->nameid==0 || !itemdb_isequip(n_item->nameid))
      continue;
    WBUFW(buf,4+c*20)=i+2;
    WBUFW(buf,4+c*20+2)=n_item->nameid;
	//beginning equipment position somewhere?
    WBUFB(buf,4+c*20+4)=itemdb_type(n_item->nameid);
    WBUFB(buf,4+c*20+5)=n_item->identify;
    WBUFW(buf,4+c*20+6)=itemdb_equip_point(n_item->nameid,sd);
    WBUFW(buf,4+c*20+8)=n_item->equip;
    WBUFB(buf,4+c*20+10)=n_item->attribute;
    WBUFB(buf,4+c*20+11)=n_item->refine;
    WBUFW(buf,4+c*20+12)=n_item->card[0];
    WBUFW(buf,4+c*20+14)=n_item->card[1];
    WBUFW(buf,4+c*20+16)=n_item->card[2];
    WBUFW(buf,4+c*20+18)=n_item->card[3];
    c++;
  }
  if(c==0)
    return 0;
  WBUFW(buf,2)=4+c*20;
  return 4+c*20;
}



int mmo_map_set00bd(int fd,unsigned char *buf)
{
  struct map_session_data *sd;

  sd=session[fd]->session_data;

  //R 00bd <status point>.w <STR>.B <STRupP>.B <AGI>.B <AGIupP>.B <VIT>.B <VITupP>.B <INT>.B <INTupP>.B <DEX>.B <DEXupP>.B <LUK>.B <LUKupP>.B <ATK>.w <ATKbonus>.w <MATKmax>.w <MATKmin>.w <DEF>.w <DEFbonus>.w <MDEF>.w <MDEFbonus>.w <HIT>.w <FLEE>.w <FLEEbonus>.w <critical>.w ?.w
  memset(buf,0,packet_len_table[0xbd]);
  WBUFW(buf,0)=0xbd;
  WBUFW(buf,2)=sd->status.status_point;
  WBUFB(buf,4)=sd->status.str;
  WBUFB(buf,5)=calc_need_status_point(sd,SP_STR);
  WBUFB(buf,6)=sd->status.agi;
  WBUFB(buf,7)=calc_need_status_point(sd,SP_AGI);
  WBUFB(buf,8)=sd->status.vit;
  WBUFB(buf,9)=calc_need_status_point(sd,SP_VIT);
  WBUFB(buf,10)=sd->status.int_;
  WBUFB(buf,11)=calc_need_status_point(sd,SP_INT);
  WBUFB(buf,12)=sd->status.dex;
  WBUFB(buf,13)=calc_need_status_point(sd,SP_DEX);
  WBUFB(buf,14)=sd->status.luk;
  WBUFB(buf,15)=calc_need_status_point(sd,SP_LUK);

  WBUFW(buf,16) = sd->status.str + (sd->status.str%10)*(sd->status.str%10) + (sd->status.dex/5);
  WBUFW(buf,18) = 0; // attack bouns
  WBUFW(buf,20) = sd->status.int_ + (sd->status.int_/5)*(sd->status.int_/5);
  WBUFW(buf,22) = sd->status.int_ + (sd->status.int_/7)*(sd->status.int_/7);
  WBUFW(buf,24) = 0; // def
  WBUFW(buf,26) = sd->status.vit;
  WBUFW(buf,28) = 0; // mdef
  WBUFW(buf,30) = sd->status.int_;
  WBUFW(buf,32) = sd->status.base_level + sd->status.dex;
  WBUFW(buf,34) = sd->status.base_level + sd->status.agi;
  WBUFW(buf,36) = (sd->status.luk/10)+1;
  WBUFW(buf,38) = 1+(sd->status.luk/3);
  WBUFW(buf,40) = sd->status.karma;
  WBUFW(buf,42) = sd->status.manner;

  return packet_len_table[0xbd];
}

int mmo_send_selfdata(int fd,struct map_session_data *sd)
{
  int len,i;

  sd->max_weight=10000;
  sd->weight=0;
  sd->status.damage_atk = 0;
  for(i=0;i<MAX_INVENTORY;i++){
    struct item *n_item=&sd->status.inventory[i];
    if(n_item->nameid==0)
      continue;
    sd->weight+=itemdb_weight(n_item->nameid)*n_item->amount;
  }
  // each connection

  // party&guild
  // each load end
  // next exp , weight , skill point
  len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_MAXWEIGHT);
  if(len>0) WFIFOSET(fd,len);
  len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_WEIGHT);
  if(len>0) WFIFOSET(fd,len);
  // item
  len=mmo_map_all_nonequip(fd,WFIFOP(fd,0));
  if(len>0) WFIFOSET(fd,len);
  len=mmo_map_all_equip(fd,WFIFOP(fd,0));
  if(len>0) WFIFOSET(fd,len);
  // status
  len=mmo_map_set00bd(fd,WFIFOP(fd,0));
  if(len>0) WFIFOSET(fd,len);
  // ASPD
  len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_ASPD);
  if(len>0) WFIFOSET(fd,len);
  // skill
  //skill information and aquisition 
  len=mmo_map_all_skill(fd,WFIFOP(fd,0),sd->status.skill_point,0);
  if(len>0) WFIFOSET(fd,len);
  return 0;
}

/*reworking jobchange function (experimental)*/
int mmo_map_jobchange(int fd,int class)
{
  int len;
  struct map_session_data *sd;

  sd=session[fd]->session_data;
  //Fred JobLevel Reset ;)    
  sd->status.job_level=1;
  WFIFOW(fd,0) = 0xb0;
  WFIFOW(fd,2) = 0x37;
  WFIFOL(fd,4) = 1;
  WFIFOSET(fd,8);
  //Hud Skill Point Reset
  mmo_map_update_param(fd,SP_SKILLPOINT,-(sd->status.skill_point));
  sd->status.class=class;
  //the argument you see changes (3~)�@sd->account_id(apply ID)LOOK_BASE�isubstance�j,sd->status.class(type)
  len=mmo_map_set_look(fd,WFIFOP(fd,0),sd->account_id,LOOK_BASE,sd->status.class);
  if(len>0) mmo_map_sendarea(fd,WFIFOP(fd,0),len,0);
  mmo_map_calc_status(fd,0,0);
  len = mmo_map_all_skill(fd,WFIFOP(fd,0),sd->status.skill_point,0);
	 if(len>0) WFIFOSET(fd,len);
  return 0;
}

int mmo_map_make_flooritem(struct item *item_data,int amount,int m,int x,int y)
{
  int i,j,c,free_cell,r;
  int id;
  struct flooritem_data *fitem;
  unsigned char buf[64];
  int len;

  if((id=search_free_object_id())==0)
    return 0;

  // (x-1,y-1)-(x+1,y+1) possible cell searching among them. random drops?
  for(free_cell=0,i=-1;i<=1;i++){
    if(i+y<0 || i+y>=map_data[m].ys)
      continue;
    for(j=-1;j<=1;j++){
      if(j+x<0 || j+x>=map_data[m].xs)
	continue;
      if((c=map_data[m].gat[j+x+(i+y)*map_data[m].xs])==1 || c==5)
	continue;
      free_cell++;
    }
  }
  if(free_cell==0)
    return 0;
  r=rand();
  free_cell=r%free_cell;
  r=r>>4;
  for(i=-1;i<=1;i++){
    if(i+y<0 || i+y>=map_data[m].ys)
      continue;
    for(j=-1;j<=1;j++){
      if(j+x<0 || j+x>=map_data[m].xs)
	continue;
      if((c=map_data[m].gat[j+x+(i+y)*map_data[m].xs])==1 || c==5)
	continue;
      if(free_cell==0){
	x+=j;
	y+=i;
	i=3;
	break;
      }
      free_cell--;
    }
  }

  fitem=malloc(sizeof(*fitem));
  object[id]=&fitem->block;
  fitem->id=id;
  fitem->m=m;
  fitem->x=x;
  fitem->y=y;
  memcpy(&fitem->item_data,item_data,sizeof(*item_data));
  fitem->item_data.amount=amount;
  fitem->subx=(r&3)*3+3;
  fitem->suby=((r>>2)&3)*3+3;
  fitem->drop_tick=gettick();
  //printf("(%d,%d)+(%d,%d)\n",fitem->x,fitem->y,fitem->subx,fitem->suby);

  fitem->block.type=BL_ITEM;
  add_block(&fitem->block,fitem->m,fitem->x,fitem->y);

  len=mmo_map_set_dropitem(0,buf,fitem);
  if(len>0)
    mmo_map_sendarea_mxy(m,x,y,buf,len);

  return id;
}

int mmo_map_dropitem(int fd,int index,int amount)
{
  int len;
  struct map_session_data *sd;
  struct item item_data_tmp;

  if(index<2 || index>=MAX_INVENTORY+2)
    return -1;
  sd=session[fd]->session_data;
  memcpy(&item_data_tmp,&sd->status.inventory[index-2],sizeof(item_data_tmp));
  len=mmo_map_item_lost(fd,WFIFOP(fd,0),index,amount);
  if(len<=0)
    return -1;
  WFIFOSET(fd,len);
  mmo_map_make_flooritem(&item_data_tmp,amount,sd->mapno,sd->x,sd->y);
  return 0;
}

int mmo_map_takeitem(int fd,int item_id)
{
  int len;
  struct map_session_data *sd;
  struct flooritem_data *fitem;

  if(item_id<2 || item_id>=50000 || object[item_id]==NULL || object[item_id]->type!=BL_ITEM)
    return -1;

  sd=session[fd]->session_data;
  fitem=(struct flooritem_data*)object[item_id];

  len=mmo_map_item_get(fd,WFIFOP(fd,0),&fitem->item_data);
  if(len>0){
    WFIFOSET(fd,len);

    WFIFOW(fd,0)=0x8a;	// take motion
    WFIFOL(fd,2)=sd->account_id;
    WFIFOL(fd,6)=item_id;
    WFIFOL(fd,10)=gettick();
    WFIFOB(fd,26)=1;
    mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x8a],0);

    WFIFOW(fd,0)=0xa1;	// delete floor item
    WFIFOL(fd,2)=item_id;
    mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0xa1],0);

    delete_object(item_id);
  }

  return 0;
}

int clear_flooritem(int tid,unsigned int tick,int id,int data)
{
  int i;
  struct flooritem_data *fitem;
  unsigned char buf[64];

  for(i=2;i<=last_object_id;i++){
    if(object[i]==NULL || object[i]->type!=BL_ITEM)
      continue;
    fitem=(struct flooritem_data *)object[i];
    if(tick-fitem->drop_tick < LIFETIME_FLOORITEM*1000)
      continue;

    WBUFW(buf,0)=0xa1;	// delete floor item
    WBUFL(buf,2)=i;
    mmo_map_sendarea_mxy(fitem->m,fitem->x,fitem->y,buf,packet_len_table[0xa1]);

    delete_object(i);
  }
  return 0;
}

#define MAX_TMP_PATH (150)

int calc_cost(struct mmo_tmp_path *p,int x1,int y1)
{
  int xd,yd;
  xd=x1-p->x;
  if(xd<0) xd=-xd;
  yd=y1-p->y;
  if(yd<0) yd=-yd;
  return (xd+yd)*10+p->dist;
}

int add_path(struct mmo_tmp_path *path,int *wp,int x,int y,int dist,int dir,int before,int x1,int y1)
{
  int i;
  for(i=0;i<*wp;i++)
    if(path[i].x==x && path[i].y==y){
      if(path[i].dist>dist){
	path[i].dist=dist;
	path[i].dir=dir;
	path[i].before=before;
	path[i].cost=calc_cost(&path[i],x1,y1);
	path[i].flag=0;
      }
      return 0;
    }
  //printf("add_path %d %d,%d,%d,%d,%d\n",*wp,x,y,dist,dir,before);
  path[*wp].x=x;
  path[*wp].y=y;
  path[*wp].dist=dist;
  path[*wp].dir=dir;
  path[*wp].before=before;
  path[*wp].cost=calc_cost(&path[*wp],x1,y1);
  path[*wp].flag=0;
  *wp=*wp+1;
  return 0;
}

int can_move(struct mmo_map_data *m,int x0,int y0,int x1,int y1)
{
  int c;

  if(x0-x1<-1 || x0-x1>1 || y0-y1<-1 || y0-y1>1)
    return 0;
  if(x1<0 || y1<0 || x1>=m->xs || y1>=m->ys)
    return 0;
  if((c=m->gat[x0+y0*m->xs])==1 || c==5)
    return 0;
  if((c=m->gat[x1+y1*m->xs])==1 || c==5)
    return 0;
  if(x0==x1 || y0==y1)
    return 1;
  if(((c=m->gat[x0+y1*m->xs])==1 || c==5) ||
     ((c=m->gat[x1+y0*m->xs])==1 || c==5))
    return 0;
  return 1;
}

//FRED11
int search_path(struct map_session_data *sd,int mapno,int x0,int y0,int 
x1,int y1,int easy_only)
{
 struct mmo_tmp_path tmp_path[MAX_TMP_PATH];
 int rp,wp,i;
 int x,y,dx,dy;
 struct mmo_map_data *m;

// printf("search_path %s (%d,%d)-(%d,%d)n",map[mapno],x0,y0,x1,y1);
 if(!map[mapno][0])
   return -1;
 m=&map_data[mapno];
 if( (x0>0) && (y0>0)&& (x1>0) &&(y1>0) ) {
 // easy
 dx = (x1-x0<0) ? -1 : 1;
 dy = (y1-y0<0) ? -1 : 1;
 for(x=x0,y=y0,i=0;(x!=x1 || y!=y1) && (i<64);){
   
   if (x!=x1 && y!=y1) {
     if(!can_move(m,x,y,x+dx,y+dy))
      break;
   x+=dx;
   y+=dy;
   sd->walkpath[i++]=(dx<0) ? ((dy>0)? 1 : 3) : ((dy<0)? 5 : 7);
    } 
    
    else if ( x!=x1) {
         if(!can_move(m,x,y,x+dx,y   ))
         break;
     x+=dx;
     sd->walkpath[i++]=(dx<0) ? 2 : 6;
   } 
   else { // y!=y1
              if(!can_move(m,x,y,x   ,y+dy))
          break;
        y+=dy;
        sd->walkpath[i++]=(dy>0) ? 0 : 4;
    }
    
         if(x==x1 && y==y1){
             sd->walkpath_len=i;
             sd->walkpath_pos=0;
             return 0;
         }

}

 if(easy_only)
   return -1;

 // difficult
 wp=0;
 tmp_path[wp].x=x0;
 tmp_path[wp].y=y0;
 tmp_path[wp].dist=0;
 tmp_path[wp].dir=0;
 tmp_path[wp].before=0;
 tmp_path[wp].cost=calc_cost(&tmp_path[wp],x1,y1);
 tmp_path[wp++].flag=0;
 while(1){
   int min_cost,min_pos,max_dist;
   for(i=0,min_cost=65536,max_dist=-1,min_pos=-1;i<wp;i++){
     if(tmp_path[i].flag==0 && (tmp_path[i].cost<min_cost || 
(tmp_path[i].cost==min_cost && tmp_path[i].dist>max_dist))){
    min_pos=i;
    min_cost=tmp_path[i].cost;
    max_dist=tmp_path[i].dist;
     }
   }
   if(min_pos<0)
     return -1;
   rp=min_pos;
   x=tmp_path[rp].x;
   y=tmp_path[rp].y;
   if(x==x1 && y==y1){
     int len,j;
     for(len=0,i=rp;len<100 && i!=0;i=tmp_path[i].before,len++);
     if(len==100 || len>=sizeof(sd->walkpath))
    return -1;
     sd->walkpath_len=len;
     sd->walkpath_pos=0;
     for(i=rp,j=len-1;i!=0;i=tmp_path[i].before,j--)
    sd->walkpath[j]=tmp_path[i].dir;
     //printf("search path ok (%d,%d)-(%d,%d) ",x0,y0,x1,y1);
     //for(i=0;i<len;i++)
     //printf("%d",sd->walkpath[i]);
     //printf("n");
     return 0;
   }
   if(can_move(m,x,y,x+1,y-1))
     add_path(tmp_path,&wp,x+1,y-1,tmp_path[rp].dist+14,5,rp,x1,y1);
   if(can_move(m,x,y,x+1,y  ))
     add_path(tmp_path,&wp,x+1,y  ,tmp_path[rp].dist+10,6,rp,x1,y1);
   if(can_move(m,x,y,x+1,y+1))
     add_path(tmp_path,&wp,x+1,y+1,tmp_path[rp].dist+14,7,rp,x1,y1);
   if(can_move(m,x,y,x  ,y+1))
     add_path(tmp_path,&wp,x  ,y+1,tmp_path[rp].dist+10,0,rp,x1,y1);
   if(can_move(m,x,y,x-1,y+1))
     add_path(tmp_path,&wp,x-1,y+1,tmp_path[rp].dist+14,1,rp,x1,y1);
   if(can_move(m,x,y,x-1,y  ))
     add_path(tmp_path,&wp,x-1,y  ,tmp_path[rp].dist+10,2,rp,x1,y1);
   if(can_move(m,x,y,x-1,y-1))
     add_path(tmp_path,&wp,x-1,y-1,tmp_path[rp].dist+14,3,rp,x1,y1);
   if(can_move(m,x,y,x  ,y-1))
     add_path(tmp_path,&wp,x  ,y-1,tmp_path[rp].dist+10,4,rp,x1,y1);
   if(wp>=MAX_TMP_PATH-5)
     return -1;
   tmp_path[rp].flag=1;
 }
 return -1;
}
return -1;  
}

int calc_next_walk_step(int fd)
{
  struct map_session_data *sd;

  if(session[fd]==NULL || (sd=session[fd]->session_data)==NULL)
    return -1;
  if(sd->walkpath_pos>=sd->walkpath_len)
    return -1;
  if(sd->walkpath[sd->walkpath_pos]&1)
    return sd->speed*14/10;
  return sd->speed;
}

int mmo_map_changemap(int id,struct map_session_data *sd,char *mapname,int x,int y,int type)
{
	int m,j;
	m=sd->mapno;
	  //changing maps while escaping
	for(j=0;j<map_data[m].npc_num;j++){
		if(map_data[m].npc[j]->block.subtype==MONS){//if monster
			if(map_data[m].npc[j]->u.mons.target_id == sd->account_id){
				mmo_mons_attack_no(m,j,sd->account_id,gettick());
				}
	  		}
	}
  del_block(&sd->block);

	// pet processing
	if(sd->status.pet.activity == 1)
	{
		// changing maps on the same server
		WFIFOW(id,0) = 0x80;
		WFIFOL(id,2) = sd->status.pet.pet_id_as_npc;
		WFIFOB(id,6) = 0;
		WFIFOSET(id,7);

		del_block(&map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->block);
	}

  WFIFOW(id,0)=0x80;
  WFIFOL(id,2)=sd->account_id;
  WFIFOB(id,6)=type;
  mmo_map_sendarea(id,WFIFOP(id,0),packet_len_table[0x80], 1 );
  current_attack_m=0;
  map_data[sd->mapno].users--;
  set_map(sd,mapname,x,y);
  WFIFOW(id,0)=0x91; // in same server move
  memcpy(WFIFOP(id,2),mapname,16);
  WFIFOW(id,18)=x;
  WFIFOW(id,20)=y;
  WFIFOSET(id,22);
  return 0;
}

int walk_char(int tid,unsigned int tick,int id,int data)
{
  struct map_session_data *sd;
  int i,m,nx,ny,dx,dy;
  static int dirx[8]={0,-1,-1,-1,0,1,1,1};
  static int diry[8]={1,1,0,-1,-1,-1,0,1};
  // ADDED on 04/09/2003 ------------------
  char buf[256];
  // --------------------------------------

  //printf("walk_char %d %08x %d\n",id,tick,data);

  if(session[id]==NULL || (sd=session[id]->session_data)==NULL)
    return 0;
  if(sd->walktimer != tid){
    //printf("walk_char %d != %d\n",sd->walktimer,tid);
    return 0;
  }
  if(sd->walkpath_pos>=sd->walkpath_len || sd->walkpath_pos!=data || sd->walktimer<0){
    sd->walktimer=-1;
    return 0;
  }
  dx=dirx[(int)sd->walkpath[sd->walkpath_pos]];
  dy=diry[(int)sd->walkpath[sd->walkpath_pos]];
  nx=sd->x+dx;
  ny=sd->y+dy;
  if(nx/BLOCK_SIZE != sd->x/BLOCK_SIZE || ny/BLOCK_SIZE != sd->y/BLOCK_SIZE){
    unsigned char buf[256];
    int bx,by;
    bx=sd->x/BLOCK_SIZE;
    by=sd->y/BLOCK_SIZE;
    WBUFW(buf,0)=0x80;
    WBUFL(buf,2)=sd->account_id;
    WBUFB(buf,6)=0;	// no effect?
    if(nx/BLOCK_SIZE != sd->x/BLOCK_SIZE && bx-dx*AREA_SIZE>=0 && bx-dx*AREA_SIZE<map_data[sd->mapno].bxs){
      for(i=-AREA_SIZE;i<=AREA_SIZE;i++){
	if(by+i < 0 || by+i >= map_data[sd->mapno].bys)
	  continue;
	mmo_map_sendblock(sd->mapno,bx-dx*AREA_SIZE,by+i,buf,packet_len_table[0x80],id,1);
	mmo_map_clrblockchar(id,sd->mapno,bx-dx*AREA_SIZE,by+i);
      }
    }
    if(ny/BLOCK_SIZE != sd->y/BLOCK_SIZE && by-dy*AREA_SIZE>=0 && by-dy*AREA_SIZE<map_data[sd->mapno].bys){
      for(i=-AREA_SIZE;i<=AREA_SIZE;i++){
	if(bx+i < 0 || bx+i >= map_data[sd->mapno].bxs)
	  continue;
	mmo_map_sendblock(sd->mapno,bx+i,by-dy*AREA_SIZE,buf,packet_len_table[0x80],id,1);
	mmo_map_clrblockchar(id,sd->mapno,bx+i,by-dy*AREA_SIZE);
      }
    }
    del_block(&sd->block);
    add_block(&sd->block,sd->mapno,nx,ny);
    mmo_map_set007b(id,buf,tick,sd->x,sd->y,sd->to_x,sd->to_y);
    bx=nx/BLOCK_SIZE;
    by=ny/BLOCK_SIZE;
    if(nx/BLOCK_SIZE != sd->x/BLOCK_SIZE && bx+dx*AREA_SIZE>=0 && bx+dx*AREA_SIZE<map_data[sd->mapno].bxs){
      for(i=-AREA_SIZE;i<=AREA_SIZE;i++){
	if(by+i < 0 || by+i >= map_data[sd->mapno].bys)
	  continue;
	mmo_map_sendblock(sd->mapno,bx+dx*AREA_SIZE,by+i,buf,packet_len_table[0x7b],id,1);
	mmo_map_getblockchar(id,sd->mapno,bx+dx*AREA_SIZE,by+i);
      }
    }
    if(ny/BLOCK_SIZE != sd->y/BLOCK_SIZE && by+dy*AREA_SIZE>=0 && by+dy*AREA_SIZE<map_data[sd->mapno].bys){
      for(i=-AREA_SIZE;i<=AREA_SIZE;i++){
	if(bx+i < 0 || bx+i >= map_data[sd->mapno].bxs)
	  continue;
	mmo_map_sendblock(sd->mapno,bx+i,by+dy*AREA_SIZE,buf,packet_len_table[0x7b],id,1);
	mmo_map_getblockchar(id,sd->mapno,bx+i,by+dy*AREA_SIZE);
      }
    }
  }

  // ADDED on 04/09/2003 ------------
  if(sd->status.pet.activity == 1)
  {
		//mmo_map_set_npc(0,buf,sd->status.pet.pet_id_as_npc,BL_NPC,sd->x,sd->y,sd->dir);
		WBUFW(buf,0)=0x7b;
		WBUFL(buf,2)=sd->status.pet.pet_id_as_npc;
		WBUFW(buf,6)=200;
		WBUFW(buf,8)=0;
		WBUFW(buf,10)=0;
		WBUFW(buf,12)=0;
		WBUFW(buf,14)=sd->status.pet.pet_class;
		WBUFW(buf,16)=14;
		WBUFW(buf,18)=0;
		WBUFW(buf,20)=0;
		WBUFL(buf,22)=tick;
		WBUFW(buf,26)=0;
		WBUFW(buf,28)=0;
		WBUFW(buf,30)=0;
		WBUFW(buf,32)=0;
		WBUFW(buf,34)=0;
		WBUFW(buf,36)=0;
		WBUFW(buf,38)=0;
		WBUFW(buf,40)=0;
		WBUFW(buf,42)=0;
		WBUFW(buf,44)=0;
		WBUFW(buf,46)=0;
		WBUFB(buf,48)=0;
		WBUFB(buf,49)=0;
		//set_pos(WBUFP(buf,50),sd->x,sd->y);
		set_2pos(WBUFP(buf,50),map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y,sd->x,sd->y);
		//WBUFB(buf,48)|=sd->dir&0x0f;
		WBUFB(buf,55)=0;
		WBUFB(buf,56)=0;
		WBUFB(buf,57)=0;
		mmo_map_sendarea_mxy(sd->mapno,sd->x,sd->y,buf,packet_len_table[0x7b]);
	/*
		WFIFOW(sd->fd,0) = 0x1a4;
		WFIFOB(sd->fd,2) = 0;
		WFIFOL(sd->fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(sd->fd,7) = 0;
		WFIFOSET(sd->fd,11);

		WFIFOW(sd->fd,0) = 0x1a4;
		WFIFOB(sd->fd,2) = 5;
		WFIFOL(sd->fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(sd->fd,7) = 14;
		WFIFOSET(sd->fd,11);

		WFIFOW(sd->fd,0) = 0x1a2;
		memcpy(WFIFOP(sd->fd,2),sd->status.pet.pet_name,24);
		WFIFOB(sd->fd,26) = sd->status.pet.pet_name_flag;
		WFIFOW(sd->fd,27) = sd->status.pet.pet_level;
		WFIFOW(sd->fd,29) = sd->status.pet.pet_hungry;;
		WFIFOW(sd->fd,31) = sd->status.pet.pet_friendly;
		WFIFOW(sd->fd,33) = sd->status.pet.pet_accessory;
		WFIFOSET(sd->fd,35);
*/

		WFIFOW(sd->fd,0) = 0x1a4;
		WFIFOB(sd->fd,2) = 3;
		WFIFOL(sd->fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(sd->fd,7) = sd->status.pet.pet_accessory;
		WFIFOSET(sd->fd,11);

		WFIFOW(sd->fd,0) = 0x1a4;
		WFIFOB(sd->fd,2) = 5;
		WFIFOL(sd->fd,3) = sd->status.pet.pet_id_as_npc;
		WFIFOL(sd->fd,7) = 14;
		WFIFOSET(sd->fd,11);

		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x = sd->x;
		map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y = sd->y;
  }
  // --------------------------------


  sd->x+=dx;
  sd->y+=dy;
  //printf("walk_char %d (%d,%d) %d/%d %d\n",id,sd->x,sd->y,sd->walkpath_pos,sd->walkpath_len,sd->walkpath[sd->walkpath_pos]);
  sd->walkpath_pos++;
  if(sd->walkpath_pos>=sd->walkpath_len){
    sd->walkpath_pos=0;
    sd->walkpath_len=0;
    sd->walktimer=-1;
  }
  m=sd->mapno;
  if(!map[m][0])
    return -1;
  if(map_data[m].gat[sd->x+map_data[m].xs*sd->y]&0x80){
    int w;
    sd->walkpath_pos=0;
    sd->walkpath_len=0;
    sd->walktimer=-1;
    for(w=0;w<map_data[m].npc_num;w++){
      int xs,ys;
      if(map_data[m].npc[w]->block.subtype!=WARP)
	continue;
      xs=map_data[m].npc[w]->u.warp.xs;
      ys=map_data[m].npc[w]->u.warp.ys;
      if(sd->x >= map_data[m].npc[w]->x-xs/2 && sd->x <map_data[m].npc[w]->x-xs/2+xs &&
         sd->y >= map_data[m].npc[w]->y-ys/2 && sd->y <map_data[m].npc[w]->y-ys/2+ys){
	mmo_map_changemap(id,sd,map_data[m].npc[w]->u.warp.name,map_data[m].npc[w]->u.warp.x,map_data[m].npc[w]->u.warp.y,0);
	break;
      }
    }
  }
  if(sd->walkpath_pos>=sd->walkpath_len)
    return 0;
  if((i=calc_next_walk_step(id))>0){
    sd->walktimer=add_timer(tick+i,walk_char,id,sd->walkpath_pos);
    //printf("walktimer=%d\n",sd->walktimer);
  }

  return 0;
}

int mmo_map_status_up(int fd,int type)
{
  struct map_session_data *sd;
  int need,len,val=0;

  sd=session[fd]->session_data;

  need=calc_need_status_point(sd,type);
  if(type<SP_STR || type>SP_LUK || need<=0 || sd->status.status_point<need){
    WFIFOW(fd,0)=0xbc;
    WFIFOW(fd,2)=type;
    WFIFOB(fd,4)=0x00;
    WFIFOB(fd,5)=0;
    WFIFOSET(fd,6);
    return -1;
  }
  sd->status.status_point-=need;
  len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_STATUSPOINT);
  if(len>0) WFIFOSET(fd,len);
  switch(type){
  case SP_STR:
    val=++sd->status.str;
    break;
  case SP_AGI:
    val=++sd->status.agi;
    break;
  case SP_VIT:
    val=++sd->status.vit;
    break;
  case SP_INT:
    val=++sd->status.int_;
    break;
  case SP_DEX:
    val=++sd->status.dex;
    break;
  case SP_LUK:
    val=++sd->status.luk;
    break;
  }
  len=mmo_map_set_param(fd,WFIFOP(fd,0),type);
  if(len>0) WFIFOSET(fd,len);
  if(need!=calc_need_status_point(sd,type)){
    WFIFOW(fd,0)=0xbe;
    WFIFOW(fd,2)=type-SP_STR+SP_USTR;
    WFIFOB(fd,4)=calc_need_status_point(sd,type);
    WFIFOSET(fd,5);
  }
  WFIFOW(fd,0)=0xbc;
  WFIFOW(fd,2)=type;
  WFIFOB(fd,4)=0x01;
  WFIFOB(fd,5)=val;
  WFIFOSET(fd,6);
  //status renewal
  mmo_map_calc_status(fd,0,0);
  return 0;
}

/* log of message area */
void write_log(char *mes)
{
  FILE *log;
  log=fopen("logs/mes.log","a");
  if(log){
    fprintf(log,"%s\n",mes);
    fclose(log);
  }
}

/* chat room function's */
int mmo_map_createchat( struct mmo_chat* chat)
{
  if( 0 != last_chat ) {
    last_chat->next = chat;
  } else {
    chat->next = 0;
  }
  chat->prev = last_chat;
  last_chat = chat;
  return 0;
}

int mmo_map_delchat( struct mmo_chat* chat )
{
  struct mmo_chat* temp_chat;

  temp_chat = last_chat;
  for(;;) {
    if( 0 == temp_chat )
      break;

    if( temp_chat == chat ) {
      if( temp_chat == last_chat ) {
	last_chat = temp_chat->prev;
      }

      if( 0 != temp_chat->next )
	temp_chat->next->prev = temp_chat->prev;
      if( 0 != temp_chat->prev )
	temp_chat->prev->next = temp_chat->next;
      break;
    } else {
   // printf("DEBUG delchat %08x\n", chat );
      temp_chat = temp_chat->prev;
    }
  }
  return 0;
}
#if 0
struct mmo_chat* mmo_map_getchat(unsigned long id )
{
  struct mmo_chat* temp_chat;

  temp_chat = last_chat;
  //printf("DEBUG mmo_map_getchat ID:0x%08x\n",(unsigned long*)id);
  for(;;) {
    if( 0 == temp_chat )
      break;
    //printf("DEBUG mmo_map_getchat 0x%08x\n",(unsigned long*)temp_chat);
    if( 1 ) { 					/* if in the area */
      if(  temp_chat->chatID == id )
	return (temp_chat);
    }

    temp_chat = temp_chat->prev;
  }
  return (0);
}
#endif
int mmo_map_addchat( int fd,struct mmo_chat* chat,char *pass)
{
  int i,len;
  struct map_session_data *sd;

  if(session[fd]==NULL || (sd = session[fd]->session_data)== NULL || chat==NULL)
    return 0;

  if(chat->limit == chat ->users || (chat->pub==0 && strncmp(pass,chat->pass,8))){
    //if max capacity reached and password correct. report error
    WFIFOW(fd,0)=0xda;
    WFIFOB(fd,2)=1;
    WFIFOSET(fd,3);
    return 0;
  }
  chat->usersfd[chat->users] = fd;
  chat->usersID[chat->users] = sd->account_id;
  memcpy(chat->usersName[chat->users],sd->status.name,24);
  chat->users++;

  sd->chatID = chat->chatID;

  //new person joining gets list of users
  WFIFOW(fd,0)=0xdb;
  WFIFOW(fd,2)=8+(28*chat->users);
  WFIFOL(fd,4)=chat->chatID;
  for(i = 0;i < chat->users;i++){
    WFIFOL(fd,8+i*28) = i!=0;
    memcpy(WFIFOP(fd,8+i*28+4),chat->usersName[i],24);
  }
  WFIFOSET(fd,WFIFOW(fd,2));

  // person staying in the chat reports list of people
  WFIFOW(fd, 0) = 0x0dc;
  WFIFOW(fd, 2) = chat->users;
  memcpy(WFIFOP(fd, 4),sd->status.name,24);
  mmo_map_sendchat(fd,WFIFOP(fd,0),packet_len_table[0xdc],1);

  //change list when number of users changes. With this addition, chat member �eeveryone�f is excluded. (���͂̐l�ɂ͐l���ω��񍐁B�{�I�ł̓`���b�g�����o�[�S���������Ă���͗l �vwos�̃p�^�[���ǉ�)
  len=mmo_map_set00d7(chat->usersfd[0],WFIFOP(fd,0));
  mmo_map_sendarea(chat->usersfd[0],WFIFOP(fd,0),len,3);
  return 0;
}


int mmo_map_leavechat(int fd, struct mmo_chat* chat,char* leavecharname)
{
  int i,leavechar,leavechar_fd;
  unsigned char buf[256];
  struct map_session_data* sd;

  if(!chat) return -1;

  for(i = 0,leavechar=-1;i < chat->users;i++){
    if(strcmp(chat->usersName[i],leavecharname)==0){
      leavechar=i;
      break;
    }
  }
  if(leavechar<0)	// no such user
    return -1;

  leavechar_fd=chat->usersfd[leavechar];
  if(session[leavechar_fd]==NULL || (sd=session[leavechar_fd]->session_data)==NULL)
    return -1;

  if(leavechar==0 && chat->users>1){	// change owner
    WBUFW(buf,0)=0xe1;
    WBUFL(buf,2)=1;
    memcpy(WBUFP(buf,6),chat->usersName[0],24);
    WBUFW(buf,30)=0xe1;
    WBUFL(buf,32)=0;
    memcpy(WBUFP(buf,36),chat->usersName[1],24);
    mmo_map_sendchat(fd,buf,packet_len_table[0xe1]*2,0);

    WBUFW(buf,0)=0xd8;
    WBUFL(buf,2)=(unsigned long)chat;
    mmo_map_sendarea(chat->usersfd[0],buf,6,3);

    chat->ownID=chat->usersID[1];
  }

  WBUFW(buf,0)=0xdd;
  WBUFW(buf,2)=chat->users-1;
  memcpy(WBUFP(buf,4),chat->usersName[leavechar],24);
  WBUFB(buf,28)=0;
  mmo_map_sendchat(fd,buf,packet_len_table[0xdd],0);

  chat->users--;
  sd->chatID=0;

  if(chat->users == 0){		/* delete room */
    WBUFW(buf,0)=0xd8;
    WBUFL(buf,2)=(unsigned long)chat;
    mmo_map_sendarea(chat->usersfd[0],buf,6,0);
    mmo_map_delchat(chat);
    free(chat);
  } else {
    int len;
    for(i=leavechar;i < chat->users;i++){
      chat->usersfd[i] = chat->usersfd[i+1];
      chat->usersID[i] = chat->usersID[i+1];
      memcpy(chat->usersName[i],chat->usersName[i+1],24);
    }
    len=mmo_map_set00d7(chat->usersfd[0],buf);
    mmo_map_sendarea(chat->usersfd[0],buf,len,3);
  }
  return 0;
}

int mmo_map_changeowner(int fd,struct mmo_chat* chat,char *nextownername)
{
  int i,nextowner,len;
  unsigned char buf[256];

  if(!chat) return -1;

  for(i = 0,nextowner=-1;i < chat->users;i++){
    if(strcmp(chat->usersName[i],nextownername)==0){
      nextowner=i;
      break;
    }
  }
  if(nextowner<0)	// no such user
    return -1;

  WBUFW(buf,0)=0xe1;
  WBUFL(buf,2)=1;
  memcpy(WBUFP(buf,6),chat->usersName[0],24);
  WBUFW(buf,30)=0xe1;
  WBUFL(buf,32)=0;
  memcpy(WBUFP(buf,36),chat->usersName[nextowner],24);
  mmo_map_sendchat(fd,buf,packet_len_table[0xe1]*2,0);

  WBUFW(buf,0)=0xd8;
  WBUFL(buf,2)=(unsigned long)chat;
  mmo_map_sendarea(fd,buf,6,3);

  // swap position
  i=chat->usersfd[0];
  chat->usersfd[0]=chat->usersfd[nextowner];
  chat->usersfd[nextowner]=i;
  i=chat->usersID[0];
  chat->usersID[0]=chat->usersID[nextowner];
  chat->usersID[nextowner]=i;
  memcpy(buf,chat->usersName[0],24);
  memcpy(chat->usersName[0],chat->usersName[nextowner],24);
  memcpy(chat->usersName[nextowner],buf,24);

  chat->ownID=chat->usersID[0];

  len=mmo_map_set00d7(chat->usersfd[0],buf);
  mmo_map_sendarea(chat->usersfd[0],buf,len,3);

  return 0;
}
/* END */

int mmo_map_search_monster(int m,int id)
{
  int i;
  for(i=0;i<map_data[m].npc_num;i++){
    if(map_data[m].npc[i]->id==id){
      if(map_data[m].npc[i]->block.subtype!=MONS)
	return -1;
      return i;
    }
  }
  return -1;
}

struct delay_item_drop {
  int m,x,y;
  int nameid,amount;
};

int mmo_map_delay_item_drop(int tid,unsigned int tick,int id,int data)
{
  struct delay_item_drop *ditem;
  struct item temp_item;

  ditem=(struct delay_item_drop *)id;
  //printf("drop %d -> %d %d %d %d %d\n",tid,ditem->nameid,ditem->amount,ditem->m,ditem->x,ditem->y);

  memset(&temp_item,0,sizeof(temp_item));
  temp_item.nameid = ditem->nameid;
  temp_item.amount = ditem->amount;
  temp_item.identify = !itemdb_isequip(temp_item.nameid);
  mmo_map_make_flooritem(&temp_item,1,ditem->m,ditem->x,ditem->y);

  free(ditem);
  return 0;
}

/********************************************
skill information settings
*********************************************/

int mmo_map_all_skill(int fd,unsigned char *buf,int val,int type)
{
  int i,c;
  struct map_session_data *sd;
  struct skill *n_skill;

  sd=session[fd]->session_data;

  //skill information acquired

//  set_all_skilldata(fd);
  //skill information from every job acquired.
  if(type == 0)
	  set_job_skilldata(fd);

  if(val == 0)
	  val = 1;
  WBUFW(buf,0)=0x10f;
  //skill data is registered to the character
  for(i=c=0;i<MAX_SKILL;i++){
    n_skill=&sd->status.skill[i];
	 if(n_skill->id==0)
	 continue;
    WBUFW(buf,4+c*37)= n_skill->id;	//ID 1 basic skills?
    WBUFW(buf,4+c*37+2)=skill_db[i+1].type_inf;	//target type 0-passive 1-enemy 2-place 4-immidiatery 16-ally
    WBUFW(buf,4+c*37+4)=0;	//???
    WBUFW(buf,4+c*37+6)=n_skill->lv;	//lv
    WBUFW(buf,4+c*37+8)=skill_db[i+1].sp;//sp
    WBUFW(buf,4+c*37+10)=skill_db[i+1].range;//range
    memcpy(WBUFP(buf,4+c*37+12),"",24);	//name
	if(skill_db[i+1].type_lv < n_skill->lv+1)
	    WBUFB(buf,4+c*37+36)=0;	//up
	else
		WBUFB(buf,4+c*37+36)=val;
    c++;
  }
  if(c==0)
    return 0;
  WBUFW(buf,2)=4+c*37;
  return 4+c*37;
}

//*********************************************
//item drop function
//*********************************************
int mmo_map_item_drop(int m,int n)
{
	int i;
		for(i=0;i<16;i++)
				  {
		if(mons_data[map_data[m].npc[n]->class].dropitem[i].nameid==0)
		  continue;
		if(rand()%10000 < mons_data[map_data[m].npc[n]->class].dropitem[i].p)
					{
		  struct delay_item_drop *ditem;
		  ditem=malloc(sizeof(*ditem));
		  ditem->nameid=mons_data[map_data[m].npc[n]->class].dropitem[i].nameid;
		  ditem->amount=1;
	      ditem->m=m;
		  ditem->x=map_data[m].npc[n]->x;
		  ditem->y=map_data[m].npc[n]->y;
		  add_timer(gettick()+500+i,mmo_map_delay_item_drop,(int)ditem,0);
					}
				 }
		return 0;
}

//*********************************************
//skills for PVP
//*********************************************
int mmo_map_pvp_skill_attack(int fd,int target_id,int skill_num,int skill_lvl,unsigned long tick,int skill_x,int skill_y)
{
	int damage,i,heal_point;
	struct map_session_data *sd;
	struct map_session_data *target_sd;
	int target_fd = 0;

	sd=session[fd]->session_data;
	target_sd = NULL;
/*	if(skill_db[skill_num].type_inf != 4 && skill_db[skill_num].type_inf != 16
		&& skill_db[skill_num].type_inf != 2)
		return 1;
*/
	//Fred Fix6 (PVP)
        for(i=5;i<FD_SETSIZE;i++)
	{
		if(session[i] && (target_sd=session[i]->session_data)){
				if(target_sd->account_id == target_id){
				target_fd=i;
				break;
				}
			}
	 }

	if(skill_db[skill_num].type_inf == 1 || skill_db[skill_num].type_inf == 4
			|| skill_db[skill_num].type_inf == 16){
	switch(skill_db[skill_num].type_nk)
	{
	case 0:
	if(PVP_flag)
	{
	//perminate skill
	//R 013e <src ID>.l <dst ID>.l <X>.w <Y>.w <lv?>.w ?.w <wait>.l
	WFIFOW(fd,0) = 0x13e;
	WFIFOL(fd,2) = sd->account_id;
	WFIFOL(fd,6) = target_id;
	WFIFOW(fd,10) = 0;
	WFIFOW(fd,12) = 0;
	WFIFOW(fd,14) = skill_num;//magical perminate skill
	WFIFOL(fd,16) = 0;//attribute�H
	WFIFOL(fd,20) = 100;//perminate skill time 
	WFIFOSET(fd,24);
	// calc damage
	damage = SKILL_ATTACK(sd->status.matk1,sd->status.matk2);
	//attack system skill effect
		WFIFOW(fd,0) = 0x114;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOL(fd,8) = target_id;
		WFIFOL(fd,12) = tick;
		WFIFOL(fd,16) = sd->speed;//src_speed
		WFIFOL(fd,20) = 250;
		WFIFOW(fd,24) = damage;//total damage
		WFIFOW(fd,26) = 10;//LEVEL
		WFIFOW(fd,28) = skill_db[skill_num].type_num;//when single(6), add number of partitions(�P�����ƌŒ�,���̂ق�������)
		WFIFOB(fd,30) = skill_db[skill_num].type_hit;//6�Fsingle 8�Fdouble(rapid)
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x114], 0 );
	    target_sd->status.hp -= damage;
		 if(target_sd->status.hp < 0)
			 target_sd->status.hp = 0;
		 WFIFOW(target_fd,0) = 0xb0;
		 WFIFOW(target_fd,2) = 0005;
		 WFIFOL(target_fd,4) = target_sd->status.hp;
		 WFIFOSET(target_fd,8);
	    if(target_sd->status.hp <= 0 ){
			WFIFOW(fd,0) = 0x80;//transmits death packet
			WFIFOL(fd,2) = target_id;
			WFIFOB(fd,6) = 1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
			}
		}
		break;
	case 1:
		//immediate motion skill
		//R 011a <skill ID>.w <val>.w <dst ID>.w <src ID>.w <fail>.B
		heal_point=SKILL_HEAL(sd->status.int_,sd->status.base_level,skill_lvl);
		WFIFOW(fd,0) = 0x11a;
		WFIFOW(fd,2) = skill_num;
		WFIFOW(fd,4) = heal_point;
		WFIFOL(fd,6) = target_id;
		WFIFOL(fd,10) = sd->account_id;
		WFIFOB(fd,14) = 1;
//		WFIFOSET(fd,11);
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11a], 0 );
		switch(skill_num){
		//heal
			case 28:
			//printf("targetID:%d\nheal_point:%d\naccount_id:%d\n",target_id,heal_point,sd->account_id);
				target_sd->status.hp += heal_point;
				if(target_sd->status.hp > target_sd->status.max_hp)
					target_sd->status.hp=target_sd->status.max_hp;
					WFIFOW(target_fd,0) = 0xb0;
					WFIFOW(target_fd,2) = 0005;
					WFIFOL(target_fd,4) = target_sd->status.hp;
					WFIFOSET(target_fd,8);
				break;
		//ressurection
			case 54:
					target_sd->status.hp++;
					if(target_sd->status.hp > target_sd->status.max_hp)target_sd->status.hp=target_sd->status.max_hp;
					WFIFOW(target_fd,0) = 0xb0;
					WFIFOW(target_fd,2) = 0005;
					WFIFOL(target_fd,4) = target_sd->status.hp;
					WFIFOSET(target_fd,8);

					WFIFOW(fd,0) = 0x148;
					WFIFOL(fd,2) = target_id;
					mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x148], 0 );
					break;

			default:
				break;
		}
		//printf("SKILLNUM = %d,%d,%d\n",skill_num,target_id,skill_db[skill_num].type_hit);
		break;
	//repelling/throwing skill
	//R 0115 <skill ID>.w <src ID>.l <dst ID>.l <server tick>.l <src speed>.l <dst speed>.l <X>.w <Y>.w <param1>.w <param2>.w <param3>.w <type>.B
	case 2:
		if(PVP_flag)
		{
		// calc damage
		damage = SKILL_ATTACK(sd->status.matk1,sd->status.matk2);
		WFIFOW(fd,0) = 0x115;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOL(fd,8) = target_id;
		WFIFOL(fd,12) = tick;
		WFIFOL(fd,16) = 100;//sd->speed;//src_speed
		WFIFOL(fd,20) = 250;//dst_speed
		WFIFOW(fd,24) = sd->x+3;//x
		WFIFOW(fd,26) = sd->y+3;//y
		WFIFOW(fd,28) = damage;//total damage
		WFIFOW(fd,30) = skill_lvl;//LEVEL
		WFIFOW(fd,32) = skill_db[skill_num].type_num;//number of partitions?
		WFIFOB(fd,34) = skill_db[skill_num].type_hit;//type=05 damage & repelling/throwing. param1 total damage�Aparam2 level�Aparam3 number of partitions expected
		 //type=06 zero power area? param1 gomi pattern
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x115], 0 );
		//printf("SKILLNUM = %d,%d,%d\n",skill_num,damage,skill_db[skill_num].type_num);
		 target_sd->status.hp-=damage;
		  if(target_sd->status.hp < 0)
			 target_sd->status.hp = 0;
					WFIFOW(target_fd,0) = 0xb0;
					WFIFOW(target_fd,2) = 0005;
					WFIFOL(target_fd,4) = target_sd->status.hp;
					WFIFOSET(target_fd,8);
	    if(target_sd->status.hp <= 0 ){
			WFIFOW(fd,0) = 0x80;//transmits death packet
			WFIFOL(fd,2) = target_id;
			WFIFOB(fd,6) = 1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
			}
		break;
		}
			default:
				break;
		}
	}
	else if(skill_db[skill_num].type_inf == 2)
		{
		int skill_type;
		skill_type = search_placeskill(skill_num);
	//perminate skills
	//R 013e <src ID>.l <dst ID>.l <X>.w <Y>.w <lv?>.w ?.w <wait>.l
		WFIFOW(fd,0) = 0x13e;
		WFIFOL(fd,2) = sd->account_id;
		WFIFOL(fd,6) =  0;
		WFIFOW(fd,10) = skill_x;
		WFIFOW(fd,12) = skill_y;
		WFIFOW(fd,14) = skill_num;//perminate magical skill
		WFIFOL(fd,16) = 0;//attribute�H
		WFIFOL(fd,20) = 3000;//perminate skill time 
		WFIFOSET(fd,24);
	//skill effect of partner
	//<skill ID>.w <src ID>.l <val>.w <X>.w <Y>.w <server tick>.l
		WFIFOW(fd,0) = 0x117;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOW(fd,8) = 100;
		WFIFOW(fd,10) = skill_x;//X coordinate
		WFIFOW(fd,12) = skill_y;//Y coordinate
		WFIFOL(fd,14) = tick;
	    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x117], 0 );
		WFIFOW(fd,0) = 0x11f;
		WFIFOL(fd,2) = sd->account_id;//target_id;
		WFIFOL(fd,6) = target_id;//sd->account_id;
		WFIFOW(fd,10)= skill_x;
		WFIFOW(fd,12)= skill_y;
		WFIFOB(fd,14)= skill_type;
		WFIFOB(fd,15)= 1;
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
		}
	return 0;
}
/*********************************************************************
skill attacks
*********************************************************************/

int mmo_map_skill_attack(int fd,int target_id,int skill_num,int skill_lvl,unsigned long tick,int skill_x,int skill_y)
{
	int damage,i,heal_point;
	int skill_type;
	int m,n;
	int mvp_fd = 0,mvp_damage=0;
	struct map_session_data *sd;
	struct map_session_data *target_sd;

if(session[fd] != 0) { // edit Lemming
	int target_fd = 0;
	sd=session[fd]->session_data;
	target_sd = NULL;
	m=sd->mapno;
	n=mmo_map_search_monster(m,target_id);

	//PVP processing
//	if(n<0 && skill_db[skill_num].type_inf != 2 && skill_db[skill_num].type_inf != 4 &&
//		skill_db[skill_num].type_inf != 16)
	if(n<0)
	{
		mmo_map_pvp_skill_attack(fd,target_id,skill_num,skill_lvl,tick,skill_x,skill_y);
		return 0;
	}

/*	if(skill_db[skill_num].type_inf != 4 && skill_db[skill_num].type_inf != 16
		&& skill_db[skill_num].type_inf != 2)
		return 1;
*/
	//acquire target information. Target fumbles(?)(����Ń^�[�Q�b�g���������)
	for(i=6;i<FD_SETSIZE;i++)
	{
			if(session[i]&&(target_sd=session[i]->session_data))
				if(target_sd->account_id==target_id){
				target_fd=i;
				break;
			}
	}

//	if(map_data[m].npc[n]->u.mons.hp<=0)
		//  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
	//printf("AFTER SKILL_INF = %d\n",skill_db[skill_num].type_inf);

	if(skill_db[skill_num].type_inf == 1 || skill_db[skill_num].type_inf == 4
			|| skill_db[skill_num].type_inf == 16){
	//creates skill packets
	//printf("DO SKILL! %d\n",skill_num);

	switch(skill_db[skill_num].type_nk)
	{
	case 0:
	if(map_data[m].npc[n]->u.mons.hp<=0)
		  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
	if(map_data[m].npc[n]->u.mons.hp<=0){
		  map_data[m].npc[n]->u.mons.hp = 1000;
	  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
	 }
	//perminate skill
	//R 013e <src ID>.l <dst ID>.l <X>.w <Y>.w <lv?>.w ?.w <wait>.l
	WFIFOW(fd,0) = 0x13e;
	WFIFOL(fd,2) = sd->account_id;
	WFIFOL(fd,6) = target_id;
	WFIFOW(fd,10) = 0;
	WFIFOW(fd,12) = 0;
	WFIFOW(fd,14) = skill_num;//perminate magical skill
	WFIFOL(fd,16) = 0;//attributes�H
	WFIFOL(fd,20) = 100;//perminate skill time
	WFIFOSET(fd,24);
	// calc damage
	//damage=sd->status.matk2+rand()%sd->status.matk1;
	switch (skill_num) {    //skill_update

		case 5:            //BASH case
			damage = SKILL_BASH(sd->status.atk1,sd->status.atk2,skill_lvl); // do we have to subtract def of m
			// mons_data[map_data[m].npc[n]->class].def1   mons_data[map_data[m].npc[n]->class].def2
		break;

		default:        //all other skills
			damage = SKILL_ATTACK(sd->status.matk1,sd->status.matk2);
		break;
	}    //switch skill_num
	//attack system skill effect
		WFIFOW(fd,0) = 0x114;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOL(fd,8) = target_id;
		WFIFOL(fd,12) = tick;
		WFIFOL(fd,16) = sd->speed;//src_speed
		WFIFOL(fd,20) = 250;
		WFIFOW(fd,24) = damage*skill_db[skill_num].type_num;//total damage
		WFIFOW(fd,26) = 10;//LEVEL
		WFIFOW(fd,28) = skill_db[skill_num].type_num;// when single(6), add number of partitions(�P�����ƌŒ�,���̂ق�������)
		WFIFOB(fd,30) = skill_db[skill_num].type_hit;//6�Fsingle 8�Fdouble (rapid)
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x114], 0 );
		//printf("SKILLNUM = %d,%d,%d,%d,%d\n",skill_num,skill_db[skill_num].type_num,skill_db[skill_num].type_hit,damage,map_data[m].npc[n]->u.mons.hp);
	 //if(map_data[m].npc[n]->u.mons.hp == mons_data[map_data[m].npc[n]->class].max_hp){
	 if(map_data[m].npc[n]->u.mons.target_id < 1){
			 map_data[m].npc[n]->u.mons.target_id = sd->account_id;
			 map_data[m].npc[n]->u.mons.target_fd = fd;
			 mmo_mons_once_attack_start(m,n,fd,tick);
		 }
		 map_data[m].npc[n]->u.mons.hp-= (damage*skill_db[skill_num].type_num);
		 sd->status.damage_atk += (damage*skill_db[skill_num].type_num);
		 //targeting of enemy
		// map_data[m].npc[n]->u.mons.speed = -1;//stop monster movement
         if(map_data[m].npc[n]->u.mons.hp <= 0 ){
			 WFIFOW(fd,0) = 0x80;
			 WFIFOL(fd,2) = target_id;
			 WFIFOB(fd,6) = 1;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
      // stop attack of everyone targeting this enemy
		 for(i=0;i<FD_SETSIZE;i++){
			struct map_session_data *temp_sd;
				if(session[i] && (temp_sd=session[i]->session_data )
					&&(temp_sd->attacktarget==target_id)){
						if(temp_sd->status.damage_atk >= mvp_damage){
							  mvp_damage = temp_sd->status.damage_atk;
							  mvp_fd = i;
						}
					  mmo_map_level_mons(i,m,n);
					  delete_timer(temp_sd->attacktimer,mmo_map_attack_continue);
					  temp_sd->attacktimer=-1;
						}
				   }
	  mmo_map_mvp_do(mvp_fd,m,n);
	  // item drop
      mmo_map_item_drop(m,n);
	  if(map_data[m].npc[n]->u.mons.attacktimer>=0){
	  delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
		  map_data[m].npc[n]->u.mons.target_id = -1;
		  map_data[m].npc[n]->u.mons.current_attack_m = 0;
      map_data[m].npc[n]->u.mons.attacktimer = -1;
	  if(map_data[m].npc[n]->u.mons.speed<=0)
		 map_data[m].npc[n]->u.mons.speed = 200;
			}
      respawn_mons(m,n);
	  return 0;
    }
		break;
	case 1:
		//immediate motion skill
		//R 011a <skill ID>.w <val>.w <dst ID>.w <src ID>.w <fail>.B
//		heal_point=rand()%2000+sd->status.int_;
		heal_point=SKILL_HEAL(sd->status.int_,sd->status.base_level,skill_lvl);
		WFIFOW(fd,0) = 0x11a;
		WFIFOW(fd,2) = skill_num;
		WFIFOW(fd,4) = heal_point;
		WFIFOL(fd,6) = target_id;
		WFIFOL(fd,10) = sd->account_id;
		WFIFOB(fd,14) = 1;
//		WFIFOSET(fd,11);
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11a], 0 );
		switch(skill_num){
		//heal
			case 28:
			//printf("targetID:%d\nheal_point:%d\naccount_id:%d\n",target_id,heal_point,sd->account_id);
					map_data[m].npc[n]->u.mons.hp += heal_point;
					if(map_data[m].npc[n]->u.mons.hp > mons_data[map_data[m].npc[n]->class].max_hp){
						  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
				 	}
				break;

			default:
				break;
		}
		//printf("SKILLNUM = %d,%d,%d\n",skill_num,target_id,skill_db[skill_num].type_hit);
		break;
	//throwing skill
	//R 0115 <skill ID>.w <src ID>.l <dst ID>.l <server tick>.l <src speed>.l <dst speed>.l <X>.w <Y>.w <param1>.w <param2>.w <param3>.w <type>.B
	case 2:
		//magnumbreak arrow_rain bowlingbash .. what else?
		// calc damage
		if(map_data[m].npc[n]->u.mons.hp<=0)
		  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
		if(map_data[m].npc[n]->u.mons.hp<=0){
		  map_data[m].npc[n]->u.mons.hp = 1000;
		  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
		 }
		//damage=sd->status.matk2+rand()%sd->status.matk1;
		damage = SKILL_BASH(sd->status.atk1,sd->status.atk2,skill_lvl);    //skill_update
		WFIFOW(fd,0) = 0x115;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOL(fd,8) = target_id;
		WFIFOL(fd,12) = tick;
		WFIFOL(fd,16) = 100;//sd->speed;//src_speed
		WFIFOL(fd,20) = 250;//dst_speed
		WFIFOW(fd,24) = sd->x+3;//x
		WFIFOW(fd,26) = sd->y+3;//y
		WFIFOW(fd,28) = damage;//damage total
		WFIFOW(fd,30) = skill_lvl;//LEVEL
		WFIFOW(fd,32) = skill_db[skill_num].type_num;//number of partitions?
		WFIFOB(fd,34) = skill_db[skill_num].type_hit;//type=05 damage & repelling/throwing. param1 total damage�Aparam2 level�Aparam3 number of partitions expected.
		 //type=06 zero power area? param1 gomi pattern (�S�~�̖͗l)
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x115], 0 );
		//printf("SKILLNUM = %d,%d,%d\n",skill_num,damage,skill_db[skill_num].type_num);
		 //printf("%d - %d : %d\n",m,n,map_data[m].npc[n]->u.mons.hp);
    		 //if(map_data[m].npc[n]->u.mons.hp == mons_data[map_data[m].npc[n]->class].max_hp){
    		 if(map_data[m].npc[n]->u.mons.target_id < 1){
			 map_data[m].npc[n]->u.mons.target_id = sd->account_id;
			 map_data[m].npc[n]->u.mons.target_fd = fd;
			 mmo_mons_once_attack_start(m,n,fd,tick);
		 }
		 map_data[m].npc[n]->u.mons.hp-=damage;
		 //targeting enemy
		 sd->status.damage_atk += damage;
         if(map_data[m].npc[n]->u.mons.hp <= 0 ){
			 WFIFOW(fd,0) = 0x80;
			 WFIFOL(fd,2) = target_id;
			 WFIFOB(fd,6) = 1;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
//		 mmo_map_mvp_do(fd,m,n);
      //stop attack of everyone targeting enemy
		 for(i=0;i<FD_SETSIZE;i++){
			struct map_session_data *temp_sd;
				if(session[i] && (temp_sd=session[i]->session_data )
					&& (temp_sd->attacktarget==target_id)){
						if(temp_sd->status.damage_atk >= mvp_damage){
							  mvp_damage = temp_sd->status.damage_atk;
							  mvp_fd = i;
						}
					  mmo_map_level_mons(i,m,n);
					  delete_timer(temp_sd->attacktimer,mmo_map_attack_continue);
					  temp_sd->attacktimer=-1;
						}
				   }
	  mmo_map_mvp_do(mvp_fd,m,n);
		   // item drop
			mmo_map_item_drop(m,n);
	  if(map_data[m].npc[n]->u.mons.attacktimer>=0){
	  delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
		  map_data[m].npc[n]->u.mons.target_id = -1;
		  map_data[m].npc[n]->u.mons.current_attack_m = 0;
      map_data[m].npc[n]->u.mons.attacktimer = -1;
	  if(map_data[m].npc[n]->u.mons.speed<=0)
		 map_data[m].npc[n]->u.mons.speed = 200;
			}
      respawn_mons(m,n);
    }
		break;
			default:
				break;
		}
	}
	else if(skill_db[skill_num].type_inf == 2)
		{
		//printf("PLACE SKILL 2 ! = %d\n",skill_db[skill_num].type_pl);
		skill_type = search_placeskill(skill_num);
	//perminate skill
	//R 013e <src ID>.l <dst ID>.l <X>.w <Y>.w <lv?>.w ?.w <wait>.l
		WFIFOW(fd,0) = 0x13e;
		WFIFOL(fd,2) = sd->account_id;
		WFIFOL(fd,6) =  0;
		WFIFOW(fd,10) = skill_x;
		WFIFOW(fd,12) = skill_y;
		WFIFOW(fd,14) = skill_num;//perminate magical skill
		WFIFOL(fd,16) = 0;//attribute�H
		WFIFOL(fd,20) = 3000;//perminate skill time
		WFIFOSET(fd,24);
	//skill effect on partner
	//<skill ID>.w <src ID>.l <val>.w <X>.w <Y>.w <server tick>.l
		WFIFOW(fd,0) = 0x117;
		WFIFOW(fd,2) = skill_num;
		WFIFOL(fd,4) = sd->account_id;
		WFIFOW(fd,8) = 100;
		WFIFOW(fd,10) = skill_x;//X coordinate
		WFIFOW(fd,12) = skill_y;//Y coordinate
		WFIFOL(fd,14) = tick;
	    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x117], 0 );
	//	int x,y,obj_count;
	//	int j;
		/*
		struct npc_data Obj_ID[11*11];
		//printf("skill_num %d\n",skill_num);
		damage=rand()%2000+sd->status.int_;
		x=0;
		y=0;
		obj_count=0;
		switch(skill_num){
			//meteor storm processing
		case 83:
			for(x=0;x < 5;x++)
			{
						WFIFOW(fd,0) = 0x117;
				WFIFOW(fd,2) = skill_num;
				WFIFOL(fd,4) = sd->account_id;
				WFIFOW(fd,8) = 100;
				WFIFOW(fd,10) = skill_x+x;//X coordinates 
				WFIFOW(fd,12) = skill_y;//Y coordinates
				WFIFOL(fd,14) = tick+1000*(11*x+y);
			    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x117], 0 );
				Obj_ID[11*x + y].id =obj_count;
				Obj_ID[11*x + y].x = skill_x+x;
				Obj_ID[11*x + y].y = skill_y;
				WFIFOW(fd,0) = 0x11f;
				WFIFOL(fd,2) = sd->account_id;//target_id;
				WFIFOL(fd,6) = Obj_ID[11*x + y].id;//sd->account_id;
				WFIFOW(fd,10)= skill_x + x;
				WFIFOW(fd,12)= skill_y;
				WFIFOB(fd,14)= skill_type;
				WFIFOB(fd,15)= 1;
				mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
				obj_count++;
				for(y=0;y < 5;y++)
				{
						WFIFOW(fd,0) = 0x117;
						WFIFOW(fd,2) = skill_num;
						WFIFOL(fd,4) = sd->account_id;
						WFIFOW(fd,8) = 100;
						WFIFOW(fd,10) = skill_x;//X coordinates
						WFIFOW(fd,12) = skill_y+y;//Y coordinates
						WFIFOL(fd,14) = tick+ 1000 * (11*x+y);
						  mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x117], 0 );
						Obj_ID[11*x + y].id=obj_count;
						Obj_ID[11*x + y].x = skill_x;
						Obj_ID[11*x + y].y = skill_y+y;
						WFIFOW(fd,0) = 0x11f;
						WFIFOL(fd,2) = sd->account_id;//target_id;
						WFIFOL(fd,6) = Obj_ID[11 * x + y].id;//map_data[sd->mapno].npc[j]->id;//sd->account_id;
						WFIFOW(fd,10)= skill_x;
						WFIFOW(fd,12)= skill_y + y;
						WFIFOB(fd,14)= skill_type;
						WFIFOB(fd,15)= 1;
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
						obj_count++;
				}
				y=0;
			}
			break;
			//lord of vermillion processing
		case 85:
			for(x=0;x < 11;x++)
			{
				for(j=0;j<map_data[sd->mapno].npc_num;j++)
		  		if(map_data[sd->mapno].npc[j]->block.subtype==MONS)
					if(map_data[sd->mapno].npc[j]->x == (skill_x + x))
					{
						Obj_ID[11*x + y].id =obj_count;
						Obj_ID[11*x + y].x = skill_x+x;
						Obj_ID[11*x + y].y = skill_y;
						WFIFOW(fd,0) = 0x11f;
						WFIFOL(fd,2) = sd->account_id;//target_id;
						WFIFOL(fd,6) = Obj_ID[11*x + y].id;//sd->account_id;
						WFIFOW(fd,10)= skill_x + x;
						WFIFOW(fd,12)= skill_y;
						WFIFOB(fd,14)= skill_type;
						WFIFOB(fd,15)= 1;
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
						obj_count++;
						WFIFOW(fd,0) = 0x114;
						WFIFOW(fd,2) = skill_num;
						WFIFOL(fd,4) = sd->account_id;
						WFIFOL(fd,8) = map_data[sd->mapno].npc[j]->id;
						WFIFOL(fd,12) = tick;
						WFIFOL(fd,16) = sd->speed;//src_speed
						WFIFOL(fd,20) = 250;
						WFIFOW(fd,24) = damage;//total damage
						WFIFOW(fd,26) = 10;//LEVEL
						WFIFOW(fd,28) = 10;//when single(6), add number of partitions(�P�����ƌŒ�,���̂ق�������)
						WFIFOB(fd,30) = 8;//6�Fsingle 8�Fdouble (rapid)
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x114], 0 );
					}
				for(y=1;y < 12;y++)
				{
					for(j=0;j<map_data[sd->mapno].npc_num;j++)
				  		if(map_data[sd->mapno].npc[j]->block.subtype==MONS)
							if(map_data[sd->mapno].npc[j]->y == (skill_y + y)){
						Obj_ID[11*x + y].id=obj_count;
						Obj_ID[11*x + y].x = skill_x;
						Obj_ID[11*x + y].y = skill_y+y;
						WFIFOW(fd,0) = 0x11f;
						WFIFOL(fd,2) = sd->account_id;//target_id;
						WFIFOL(fd,6) = Obj_ID[11 * x + y].id;//map_data[sd->mapno].npc[j]->id;//sd->account_id;
						WFIFOW(fd,10)= skill_x;
						WFIFOW(fd,12)= skill_y + y;
						WFIFOB(fd,14)= skill_type;
						WFIFOB(fd,15)= 1;
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
						WFIFOW(fd,0) = 0x114;
						WFIFOW(fd,2) = skill_num;
						WFIFOL(fd,4) = sd->account_id;
						WFIFOL(fd,8) = map_data[sd->mapno].npc[j]->id;
						WFIFOL(fd,12) = tick;
						WFIFOL(fd,16) = sd->speed;//src_speed
						WFIFOL(fd,20) = 250;
						WFIFOW(fd,24) = damage;//total damage
						WFIFOW(fd,26) = 10;//LEVEL
						WFIFOW(fd,28) = 10;// when single(6), add number of partitions(�P�����ƌŒ�,���̂ق�������)
						WFIFOB(fd,30) = 8;//6�Fsingle 8�Fdouble(rapid)
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x114], 0 );
							}
						obj_count++;
				}
				y=0;
			}

			for(j=0;j<12;j++)
			{
				struct block_list *bl;
				for(;bl;bl=bl->next){
				bl=map_data[m].block[Obj_ID[j].x/BLOCK_SIZE + (Obj_ID[j].y/BLOCK_SIZE)*map_data[m].bxs].next;
				//printf("OBJ %d %d\n",Obj_ID[j].x,Obj_ID[j].y);
				if(bl->type==BL_NPC){
						//printf("MONS %d\n",bl->type);
						 struct npc_data *nd;
						 nd=(struct npc_data*)bl;
						 //printf("MONS DATA %d\n",nd->id);
						 //printf("MONS HP %d\n",nd->u.mons.hp);
						WFIFOW(fd,0) = 0x114;
						WFIFOW(fd,2) = skill_num;
						WFIFOL(fd,4) = sd->account_id;
						WFIFOL(fd,8) = nd->id;
						WFIFOL(fd,12) = tick;
						WFIFOL(fd,16) = sd->speed;//src_speed
						WFIFOL(fd,20) = 250;
						WFIFOW(fd,24) = damage;//total damage
						WFIFOW(fd,26) = 10;//LEVEL
						WFIFOW(fd,28) = 10;//when single(6), add number of partitions(�P�����ƌŒ�,���̂ق�������)
						WFIFOB(fd,30) = 8;//6�Fsingle 8�Fdouble(rapid)
						//mmo_map_sendarea_mxy(m,Obj_ID[j].x,Obj_ID[j].y,packet_len_table[0x114],0);
						mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x114], 0 );
						nd->u.mons.hp -= damage;
							    if(nd->u.mons.hp <= 0 )
						{
					   WFIFOW(fd,0) = 0x80;
					   WFIFOL(fd,2) = nd->id;
					   WFIFOB(fd,6) = 1;
					   mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
					   // item drop
					   //mmo_map_item_drop(m,n);
						  // respawn_mons(m,n);
						}
					}
				}
			}

			break;
		default:
		WFIFOW(fd,0) = 0x11f;
		WFIFOL(fd,2) = sd->account_id;//target_id;
		WFIFOL(fd,6) = target_id;//sd->account_id;
		WFIFOW(fd,10)= skill_x;
		WFIFOW(fd,12)= skill_y;
		WFIFOB(fd,14)= skill_type;
		WFIFOB(fd,15)= 1;
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x11f], 0 );
		break;
		}
		*/
	}
} // edit Lemming
  return 0;
}

//**********************************************
//area effect skill function
//********************************************
int mmo_map_set_skilleffectarea(int fd,unsigned char *buf,unsigned long dst_id,unsigned long src_id,unsigned long type, int x, int y)
{
  WBUFW(buf,0)=0x11f;
  WBUFL(buf,2)=dst_id;
  WBUFL(buf,6)=src_id;
  WBUFW(buf,10)=x;
  WBUFW(buf,12)=y;
  WBUFB(buf,14)=0x7e;//type;
  WBUFB(buf,15)=1;
  return 16;
}

//*********************************************
//PVP attacking 
//*********************************************
int mmo_map_pvp_attack(int fd,int target_id,unsigned long tick)
{
  if(PVP_flag)
  {
  int damage,hit,i;
  int damage2 = 0;
  struct map_session_data *sd;
  struct map_session_data *target_sd;
  int target_fd = 0;
  int critical;
  char s_lv=0, s_type=0;
  sd=session[fd]->session_data;
  target_sd = NULL;
  //Fred fix7 (pvp)
  for(i=5;i<FD_SETSIZE;i++)
	{
	  if(session[i] && (target_sd=session[i]->session_data))
			{
			if(target_sd->account_id == target_id){
				target_fd=i;
				break;
				}
			}
	}
	WFIFOW(fd,0) = 0x13a;
	if(sd->status.weapon != 11)
		WFIFOW(fd,2) = 1;
	else
		WFIFOW(fd,2) = 11;
	WFIFOSET(fd,4);

	hit = (sd->status.hit - target_sd->status.flee1)*5;
  if(hit <= 0)
	  hit = 20;
  else if(hit >= 100)
	  hit = 95;

 if(sd->status.weapon == 16)
	 critical = sd->status.critical*2;
	 else
		critical = sd->status.critical;

	 if(rand()%100 >= critical)
		 {

		  if(rand()%100 >= hit){
				  //attack if missed
				damage = -1;
				// make packet
				WFIFOW(fd,0) = 0x8a;
				WFIFOL(fd,2) = sd->account_id;
				WFIFOL(fd,6) = target_id;
				WFIFOL(fd,10) = tick;
				WFIFOL(fd,14) = sd->status.aspd;
				WFIFOL(fd,18) = target_sd->status.aspd;
				WFIFOW(fd,22) = 0;
				WFIFOW(fd,24) = 0;
				WFIFOB(fd,26) = 0;
				WFIFOW(fd,27) = 0;
				//Fred stuff1
				//printf("%d",WFIFOP(fd,0));
				mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
			  } else {

				 //atk2(attack bonus)�{30(variation) random value
				 //damage=sd->status.atk1 + rand()%sd->status.atk2;
				 //damage -= target_sd->status.vit;

				//	recalculate damage with skill

				 if(sd->status.weapon== 4 || sd->status.weapon== 5){
					 s_type = 5;	//��
					 s_lv = sd->status.skill[55-1].lv;	//skill level
				 }
				 else if(sd->status.weapon==2 || sd->status.weapon==3){
					 s_type = 4;		//1hs and 2hs
					 s_lv = sd->status.weapon==2 ?
						 sd->status.skill[2-1].lv : sd->status.skill[3-1].lv;	//�X�L�����x��
				 }
				 else if(sd->status.weapon== 16 || sd->status.weapon== 8){
					 s_type = 3;	//katar, mace
					 s_lv = sd->status.weapon==16 ?
						 sd->status.skill[134-1].lv : sd->status.skill[65-1].lv;	//skill level
				 }
				 else{

					 s_lv = s_type = 0;
				 }

				 damage = NOM_ATTACK(sd->status.atk1,sd->status.atk2,target_sd->status.vit);


				 if(damage <= 0)
					 damage = 1;

				//recalculate damage with skill
				 damage += s_lv*s_type;
				 s_lv = s_type = 0;

				 // make packet
				 WFIFOW(fd,0) = 0x8a;
				 WFIFOL(fd,2) = sd->account_id;
				 WFIFOL(fd,6) = target_id;
				 WFIFOL(fd,10) = tick;
				 WFIFOL(fd,14) = sd->status.aspd;//250;//speed
				 WFIFOL(fd,18) = target_sd->status.aspd;//250;//enemy attack speed
				 WFIFOW(fd,22) = damage;//damage
				 if(sd->status.weapon == 16)
					{
					 damage2 = KAT_ATTACK(damage);
					 WFIFOW(fd,24) = 2;
					 WFIFOB(fd,26) = 0;
					 WFIFOW(fd,27) = damage2;
					 }
				 else{
					 WFIFOW(fd,24) = 1;
					 WFIFOB(fd,26) = 0;
					 WFIFOW(fd,27) = 0;
					 }
				 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
				 }
		 }
		 //critical attack
		 else
		 {
			 //damage=sd->status.atk1 + sd->status.atk2;

			 //recalculate damage with skill

				 if(sd->status.weapon== 4 || sd->status.weapon== 5){
					 s_type = 5;	//��
					 s_lv = sd->status.skill[55-1].lv;	//skill level
				 }
				 else if(sd->status.weapon==2 || sd->status.weapon==3){
					 s_type = 4;		//1hs and 2hs
					 s_lv = sd->status.weapon==2 ?
						 sd->status.skill[2-1].lv : sd->status.skill[3-1].lv;	//skill level
				 }
				 else if(sd->status.weapon== 16 || sd->status.weapon== 8){
					 s_type = 3;	//katar, mace
					 s_lv = sd->status.weapon==16 ?
						 sd->status.skill[134-1].lv : sd->status.skill[65-1].lv;	//skill level
				 }
				 else{

					 s_lv = s_type = 0;
				 }

			 damage=CRI_ATTACK(sd->status.atk1,sd->status.atk2,s_lv,s_type);

			 s_lv = s_type = 0;

			 // make packet
			 WFIFOW(fd,0) = 0x8a;
			 WFIFOL(fd,2) = sd->account_id;
			 WFIFOL(fd,6) = target_id;
			 WFIFOL(fd,10) = tick;
			 WFIFOL(fd,14) = sd->status.aspd;//250;//speed
			 WFIFOL(fd,18) = target_sd->status.aspd;//250;//enemy attack speed
			 WFIFOW(fd,22) = damage;//damage
			 if(sd->status.weapon == 16){
				 damage2 = KAT_ATTACK(damage);
				 WFIFOW(fd,24) = 2;
				 WFIFOB(fd,26) = 0x0a;
				 WFIFOW(fd,27) = damage2;
			 }
			 else{
				 WFIFOW(fd,24) = 1;
				 WFIFOB(fd,26) = 0x0a;
				 WFIFOW(fd,27) = 0;
			 }
			 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
		 }

		 if(damage > 0){
		 target_sd->status.hp  -= (damage + damage2);
 		 if(target_sd->status.hp < 0)
			 target_sd->status.hp = 0;
		 WFIFOW(target_fd,0) = 0xb0;
		 WFIFOW(target_fd,2) = 0005;
		 WFIFOL(target_fd,4) = target_sd->status.hp;
		 WFIFOSET(target_fd,8);
	    if(target_sd->status.hp <= 0 ){
			WFIFOW(fd,0) = 0x80;//transmits death packet
			WFIFOL(fd,2) = target_id;
			WFIFOB(fd,6) = 1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
			delete_timer(target_sd->attacktimer,mmo_map_attack_continue);
			target_sd->attacktimer=-1;
			 for(i=0;i<FD_SETSIZE;i++){
			struct map_session_data *temp_sd;
				if(session[i] && (temp_sd=session[i]->session_data )
					&& (temp_sd->attacktimer>=0 )&&
					(temp_sd->attacktarget==target_id)){
					  delete_timer(temp_sd->attacktimer,mmo_map_attack_continue);
					  temp_sd->attacktimer=-1;
						}
				}
			}
		}
	}
		return 0;
}

//*********************************************
// Lv up processing function
//*********************************************
int mmo_map_level_mons(int fd,int m,int n)
{
    struct map_session_data *sd;
	int per_damage = 1;
	 long next_job_exp;
    sd=session[fd]->session_data;
	//Lv up processing
	if(mons_data[map_data[m].npc[n]->class].max_hp >= 0)
	per_damage = (sd->status.damage_atk / mons_data[map_data[m].npc[n]->class].max_hp )*100;
	//	printf("damage_atk %d per_damage %d\n",sd->status.damage_atk,per_damage);
	sd->status.damage_atk = 0;
	if(per_damage >= 100)
		per_damage = 100;
	else
		if(per_damage <= 0)
			per_damage = 1;
//	sd->status.base_exp += mons_data[map_data[m].npc[n]->class].base_exp;
	sd->status.base_exp += (mons_data[map_data[m].npc[n]->class].base_exp*per_damage)/100;
//	printf("base_exp = %d\n",sd->status.base_exp);
//	printf("Next base = %ld\n",ExpData[sd->status.base_level]);
	if(sd->status.base_exp >= ExpData[sd->status.base_level] && (sd->status.base_level <= 100)){
		 mmo_map_level_up(fd,1);
		 WFIFOW(fd,0) = 0x19b;
		 WFIFOL(fd,2) = sd->account_id;
		 WFIFOL(fd,6) = 0;
		 mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x19b],0);
		 sd->status.base_exp = 0;
	 }
	 mmo_map_set00b1(fd,0x01,sd->status.base_exp);
	 mmo_map_set00b1(fd,0x16,ExpData[sd->status.base_level]);
//	 sd->status.job_exp += mons_data[map_data[m].npc[n]->class].job_exp;
	 sd->status.job_exp += (mons_data[map_data[m].npc[n]->class].job_exp*per_damage)/100;
	// printf("job_exp = %d\n",sd->status.job_exp);
	next_job_exp = 0;
/* If class is novice, pull from the 2nd number in exp.txt table.. */
	 if(sd->status.class == 0)
		 next_job_exp = SkillExpData[0][sd->status.job_level];
/* If class = 1-6 (First Class) pull from 3rd number in exp.txt table.. */
	 else if(sd->status.class > 0 && sd->status.class < 7)
		 next_job_exp = SkillExpData[1][sd->status.job_level];
/* If class = 7-12 (Second Class) pull from 4th number in exp.txt table.. */
	 else if(sd->status.class > 6 && sd->status.class < 13)
		 next_job_exp = SkillExpData[2][sd->status.job_level];
	 if(sd->status.job_exp >= next_job_exp && sd->status.job_level <= 50)
	 {
		 mmo_map_job_lv_up(fd,1);
		 WFIFOW(fd,0) = 0x19b;
		 WFIFOL(fd,2) = sd->account_id;
		 WFIFOL(fd,6) = 1;
		 mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x19b],0);
		 mmo_map_update_param(fd,SP_SKILLPOINT,1);
		 sd->status.job_exp = 0;
	 }
	 mmo_map_set00b1(fd,0x02,sd->status.job_exp);
	 if(sd->status.class == 0)
		 mmo_map_set00b1(fd,0x17,SkillExpData[0][sd->status.job_level]);
	 else if(sd->status.class > 0 || sd->status.class < 7)
		 mmo_map_set00b1(fd,0x17,SkillExpData[1][sd->status.job_level]);
	 else if(sd->status.class > 6 || sd->status.class < 13)
		 mmo_map_set00b1(fd,0x17,SkillExpData[2][sd->status.job_level]);
	 return 0;
}

int mmo_map_set010a(int fd,int item_id)
{
	struct item tmp_item;
	int len;
	WFIFOW(fd,0) = 0x10a;
	WFIFOW(fd,2) = item_id;
	WFIFOSET(fd,4);
	tmp_item.nameid=item_id;
    tmp_item.amount=1;
    tmp_item.identify=1;
    len=mmo_map_item_get(fd,WFIFOP(fd,0),&tmp_item);
    if(len>0) WFIFOSET(fd,len);
	return 0;
}

int mmo_map_set010b(int fd,int exp)
{
	struct map_session_data *sd;
	sd=session[fd]->session_data;

	WFIFOW(fd,0) = 0x10b;
	WFIFOL(fd,2) = 10000;
	WFIFOSET(fd,6);
	sd->status.base_exp += 10000;
	if(sd->status.base_exp >= ExpData[sd->status.base_level] && (sd->status.base_level <= 100)){
	 mmo_map_level_up(fd,1);
	 sd->status.base_exp = 0;
	 }
	 mmo_map_set00b1(fd,0x01,sd->status.base_exp);
	 mmo_map_set00b1(fd,0x16,ExpData[sd->status.base_level]);
	 return 0;
}

//*********************************************
// MVP processing function
//*********************************************

int mmo_map_mvp_do(int fd,int m,int n){

	struct map_session_data *sd;
	int	real_luc;
	sd=session[fd]->session_data;

	if( map_data[m].npc[n]->class != 1038 &&
		map_data[m].npc[n]->class != 1039 &&
		map_data[m].npc[n]->class != 1046 &&
		map_data[m].npc[n]->class != 1086 &&
		map_data[m].npc[n]->class != 1087 &&
		map_data[m].npc[n]->class != 1112 &&
		map_data[m].npc[n]->class != 1115 &&
		map_data[m].npc[n]->class != 1150 &&
		map_data[m].npc[n]->class != 1059 &&
		map_data[m].npc[n]->class != 1056 &&
		map_data[m].npc[n]->class != 1159 &&
		map_data[m].npc[n]->class != 1147 &&
		map_data[m].npc[n]->class != 1190)
	return 0;
	else
	{
		WFIFOW(fd,0) = 0x10c;
		WFIFOL(fd,2) = sd->account_id;
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x10c], 0 );
		real_luc = rand()%100;
		switch(map_data[m].npc[n]->class)
		{
		//osiris
		case 1038:
		if(real_luc < 40)
			{
				//emperium
				if(real_luc > 20)
					mmo_map_set010a(fd,714);
				else if(real_luc >= 10)
				//eye of dullahan
				mmo_map_set010a(fd,2614);
				else if(real_luc >= 0)
				//crown
				mmo_map_set010a(fd,2235);
				break;
			}
			else
				mmo_map_set010b(fd,15420);
			break;
		//baphomet
		case 1039:
			if(real_luc < 40)
			{
				//emperium
				if(real_luc > 20)
					mmo_map_set010a(fd,714);
				else if(real_luc >= 10)
				//gungnir
				mmo_map_set010a(fd,1413);
				else if(real_luc >= 0)
				//magestic goat
				mmo_map_set010a(fd,2256);
				break;
			}
			else
				mmo_map_set010b(fd,26000);
			break;
		//doppleganger
		case 1046:
		if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 25)
				mmo_map_set010a(fd,985);
				else if(real_luc > 10)
				//oridecon
				mmo_map_set010a(fd,984);
				else if(real_luc > 0)
				//safety ring
				mmo_map_set010a(fd,2615);
				break;
			}
			else
				mmo_map_set010b(fd,10680);
			break;
		//mistress
		case 1059:
		if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 25)
					mmo_map_set010a(fd,985);
				else if(real_luc > 10)
				//oridecon
				mmo_map_set010a(fd,984);
				else if(real_luc > 0)
				//Coronet
				mmo_map_set010a(fd,2249);
				break;
			}
			else
				mmo_map_set010b(fd,1284);
			break;
		//gold thief bug
		case 1086:
		if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 15)
					mmo_map_set010a(fd,985);
				else if(real_luc > 0)
				//golden gear
				mmo_map_set010a(fd,2246);
				break;
			}
			else
				mmo_map_set010b(fd,25);
			break;
		//orc hero
		case 1087:
		if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 25)
					mmo_map_set010a(fd,985);
				else if(real_luc > 5)
				//brood axe
				mmo_map_set010a(fd,1363);
				else if(real_luc > 0)
				//right epsilon
				mmo_map_set010a(fd,1366);
				break;
			}
			else
				mmo_map_set010b(fd,4300 + rand()%200);
			break;
			//ord lord
		case 1090:
			if(real_luc < 40)
			{
				//clip
				mmo_map_set010a(fd,2607);
				break;
			}
			else
				mmo_map_set010b(fd,25600 + rand()%200);
			break;
		//drakee
		case 1112:
			if(real_luc < 40)
			{
				//corsair
				if(real_luc > 30)
					mmo_map_set010a(fd,5019);
				else if(real_luc >= 0)
				//oridecon
				mmo_map_set010a(fd,984);
				break;
			}
			else
				mmo_map_set010b(fd,8600);
			break;
		//eddga
		case 1115:
			if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 20)
					mmo_map_set010a(fd,985);
				else if(real_luc >= 0)
				//oridecon
				mmo_map_set010a(fd,984);
				break;
			}
			else
				mmo_map_set010b(fd,3400);
			break;
		//moonlight
		case 1150:
			if(real_luc < 40)
			{
				//emperium
				if(real_luc > 20)
					mmo_map_set010a(fd,714);
				else if(real_luc >= 10)
				//oridecon
				mmo_map_set010a(fd,984);
				else if(real_luc >= 0)
				//hae dong gum
				mmo_map_set010a(fd,1128);
				break;
			}
			else
				mmo_map_set010b(fd,4000);
			break;
		//pheeroni
		case 1159:
			if(real_luc < 40)
			{
				//eluminum
				if(real_luc > 25)
					mmo_map_set010a(fd,985);
				else if(real_luc > 10)
				//oridecon
				mmo_map_set010a(fd,984);
				else if(real_luc > 0)
				//mr scream
				mmo_map_set010a(fd,2288);
				break;
			}
			else
				mmo_map_set010b(fd,3400);
			break;
		//maya
		case 1147:
			if(real_luc < 40)
			{
				//tiara
				mmo_map_set010a(fd,2234);
				break;
			}
			else
				mmo_map_set010b(fd,11550);
			break;
		default:
			return 0;
		}
	}
	return 0;
}

/* attack decisions�@*/
int mmo_map_once_attack(int fd,int target_id,unsigned long tick)
{
  int critical;
  int damage,hit,i;
  int damage2 = 0;
  int m,n;
  int mvp_fd = 0,mvp_damage=0;
  char s_lv=0, s_type=0;
  struct map_session_data *sd;
if(session[fd] != 0) { // edit Lemming
  sd=session[fd]->session_data;
  m=sd->mapno;
  n=mmo_map_search_monster(m,target_id);

  //PVP mode routine
  if(n<0)
  {
	   mmo_map_pvp_attack(fd,target_id,tick);
	   return 0;
  }
	//R 0139 <ID>.l <X>.w <Y>.w <X2>.w <Y2>.w <range>.w
	//enemy (X,Y) follows your coords (X2,Y2) but cannot reach
	//until within range?
	//R 013a <val>.w
	//unknown. Possible attack distance?
  // check in attack range
  /*
	WFIFOW(fd,0) = 0x139;
	WFIFOL(fd,2) = target_id;
	WFIFOW(fd,6) = map_data[m].npc[n]->x;//enemy coordinates
	WFIFOW(fd,8) = map_data[m].npc[n]->y;
	WFIFOW(fd,10) = sd->x;//your coordinates
	WFIFOW(fd,12) = sd->y;//��
	*/
  if(sd->status.hp <= 0)
  {
	  delete_timer(sd->attacktimer,mmo_map_attack_continue);
	  sd->attacktimer=-1;
	  return 0;
  }

	WFIFOW(fd,0) = 0x13a;
	if(sd->status.weapon != 11)
		WFIFOW(fd,2) = 1;
	else
		WFIFOW(fd,2) = 15;
//	printf("sd->status.weapon = %d\n",sd->status.weapon);
	WFIFOSET(fd,4);
  // check hit
//  hit=rand()%30 !=0;
	//thought to come from data
  hit = (sd->status.hit - mons_data[map_data[m].npc[n]->class].flee)*5 + 55;
  if(hit < 50)
	  hit = 60;
  //else if(hit >= 100)
  //	  hit = 95;
		 if(sd->status.weapon == 16)
			 critical = sd->status.critical*2;
		 else
			critical = sd->status.critical;
		 if(rand()%100 >= critical)
		 {
			   if(rand()%100 >= hit){
				  //attack if missed
				damage = -1;
				// make packet
				WFIFOW(fd,0) = 0x8a;
				WFIFOL(fd,2) = sd->account_id;
				WFIFOL(fd,6) = target_id;
				WFIFOL(fd,10) = tick;
				WFIFOL(fd,14) = sd->status.aspd;
				WFIFOL(fd,18) = mons_data[map_data[m].npc[n]->class].speed;
				WFIFOW(fd,22) = 0;
				WFIFOW(fd,24) = 0;
				WFIFOB(fd,26) = 0;
				WFIFOW(fd,27) = 0;
				mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
			  } else {
				  //when hit
				 if(map_data[m].npc[n]->u.mons.hp<=0)
					  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
				 if(map_data[m].npc[n]->u.mons.hp<=0){
					  map_data[m].npc[n]->u.mons.hp = 1000;
					  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
				  }
				 //atk2(attack bonus)�{30(variation) random value 
			//	 damage=sd->status.atk1 + rand() %sd->status.atk2;
			//	 damage -= mons_data[map_data[m].npc[n]->class].def1;

				//recalculate damage with skill

				 if(sd->status.weapon== 4 || sd->status.weapon== 5){
					 s_type = 5;	//spear
					 s_lv = sd->status.skill[55-1].lv;	//skill level
				 }
				 else if(sd->status.weapon==2 || sd->status.weapon==3){
					 s_type = 4;		//1hs and 2hs
					 s_lv = sd->status.weapon==2 ?
						 sd->status.skill[2-1].lv : sd->status.skill[3-1].lv;	//skill level
				 }
				 else if(sd->status.weapon== 16 || sd->status.weapon== 8){
					 s_type = 3;	//katar, mace
					 s_lv = sd->status.weapon==16 ?
						 sd->status.skill[134-1].lv : sd->status.skill[65-1].lv;	//skill level
				 }
				 else{

					 s_lv = s_type = 0;
				 }

				 damage = NOM_ATTACK(sd->status.atk1,sd->status.atk2,mons_data[map_data[m].npc[n]->class].def1);


				 if(damage <= 0)
					 damage = 1;

				//recalculate damage with skill
				 damage += s_lv*s_type;
				 s_lv = s_type = 0;

				 // make packet
				 WFIFOW(fd,0) = 0x8a;
				 WFIFOL(fd,2) = sd->account_id;
				 WFIFOL(fd,6) = target_id;
				 WFIFOL(fd,10) = tick;
				 WFIFOL(fd,14) = sd->status.aspd;//250;//speed
				 WFIFOL(fd,18) = mons_data[map_data[m].npc[n]->class].speed;//250;//enemy attack speed
				 WFIFOW(fd,22) = damage;//damage
				 if(sd->status.weapon == 16)
					{

					 //damage2 += ((damage/5) + 1);
					 damage2 = KAT_ATTACK(damage);
					 WFIFOW(fd,24) = 2;
					 WFIFOB(fd,26) = 0;
					 WFIFOW(fd,27) = damage2;
					 }
				 else{
					 WFIFOW(fd,24) = 1;
					 WFIFOB(fd,26) = 0;
					 WFIFOW(fd,27) = 0;
					 }
				 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
				 }
		 }
		 //critical attack
		 else
		 {
			 if(map_data[m].npc[n]->u.mons.hp<=0)
				  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
			 if(map_data[m].npc[n]->u.mons.hp<=0){
				  map_data[m].npc[n]->u.mons.hp = 1000;
				  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
				  }
			 //damage=sd->status.atk1 + sd->status.atk2;


			 //recalculate damage with skill
				 if(sd->status.weapon== 4 || sd->status.weapon== 5){
					 s_type = 5;	//spear
					 s_lv = sd->status.skill[55-1].lv;	//skill level
				 }
				 else if(sd->status.weapon==2 || sd->status.weapon==3){
					 s_type = 4;		//1hs and 2hs
					 s_lv = sd->status.weapon==2 ?
						 sd->status.skill[2-1].lv : sd->status.skill[3-1].lv;	//skill level
				 }
				 else if(sd->status.weapon== 16 || sd->status.weapon== 8){
					 s_type = 3;	//katar, mace
					 s_lv = sd->status.weapon==16 ?
						 sd->status.skill[134-1].lv : sd->status.skill[65-1].lv;	//skill level
				 }
				 else{

					 s_lv = s_type = 0;
				 }

			 damage=CRI_ATTACK(sd->status.atk1,sd->status.atk2,s_lv,s_type);
			 s_lv = s_type = 0;

			 // make packet
			 WFIFOW(fd,0) = 0x8a;
			 WFIFOL(fd,2) = sd->account_id;
			 WFIFOL(fd,6) = target_id;
			 WFIFOL(fd,10) = tick;
			 WFIFOL(fd,14) = sd->status.aspd;//250;//speed
			 WFIFOL(fd,18) = mons_data[map_data[m].npc[n]->class].speed;//250;//enemy attack speed
			 WFIFOW(fd,22) = damage;//damage
			 if(sd->status.weapon == 16){
				 //damage2 += ((sd->status.atk1 + sd->status.atk2)/5 + 1);
				 damage2 = KAT_ATTACK(damage);
				 WFIFOW(fd,24) = 2;
				 WFIFOB(fd,26) = 0x0a;
				 WFIFOW(fd,27) = damage2;
			 }
			 else{
				 WFIFOW(fd,24) = 1;
				 WFIFOB(fd,26) = 0x0a;
				 WFIFOW(fd,27) = 0;
			 }
			 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
		 }

		 if(damage > 0){
			  //enemy targeting
			 //if(map_data[m].npc[n]->u.mons.hp == mons_data[map_data[m].npc[n]->class].max_hp){
			 if(map_data[m].npc[n]->u.mons.target_id < 1){
				 map_data[m].npc[n]->u.mons.target_id = sd->account_id;
				 map_data[m].npc[n]->u.mons.target_fd = fd;
				 mmo_mons_once_attack_start(m,n,fd,tick);

				 }
			 map_data[m].npc[n]->u.mons.hp-=(damage + damage2);
			 sd->status.damage_atk += (damage + damage2);
			 map_data[m].npc[n]->u.mons.speed = -1;//stop monster movement
			 if(map_data[m].npc[n]->u.mons.hp <= 0 ){
				 WFIFOW(fd,0) = 0x80;
				 WFIFOL(fd,2) = target_id;
				 WFIFOB(fd,6) = 1;
			 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
	//		 mmo_map_mvp_do(fd,m,n);
		  //stop everyone attacking target
			   for(i=0;i<FD_SETSIZE;i++){
				struct map_session_data *temp_sd;
					if(session[i] && (temp_sd=session[i]->session_data )
						&& (temp_sd->attacktimer>=0 )&&
						(temp_sd->attacktarget==target_id)){
							if(temp_sd->status.damage_atk >= mvp_damage){
								  mvp_damage = temp_sd->status.damage_atk;
								  mvp_fd = i;
							}
							//experience value partition function
						  mmo_map_level_mons(i,m,n);
						  delete_timer(temp_sd->attacktimer,mmo_map_attack_continue);
						  temp_sd->attacktimer=-1;
							}
						   }
		  mmo_map_mvp_do(mvp_fd,m,n);
		  // item drop
		  mmo_map_item_drop(m,n);
		  delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
		  map_data[m].npc[n]->u.mons.attacktimer = -1;
		  map_data[m].npc[n]->u.mons.target_id = -1;
		  map_data[m].npc[n]->u.mons.current_attack_m = 0;
		  map_data[m].npc[n]->u.mons.speed = 200;
		  respawn_mons(m,n);
		  return 0;
		}
		else if(sd->status.hp <= 0)
		{
			delete_timer(sd->attacktimer,mmo_map_attack_continue);
			sd->attacktimer=-1;
			return 0;
		}

	}

} // edit Lemming

  return 0;
}

int mmo_mons_attack_no(int m,int n,int target_id,unsigned long tick)
{
	delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
	map_data[m].npc[n]->u.mons.attacktimer=-1;
	map_data[m].npc[n]->u.mons.target_id = -1;
    map_data[m].npc[n]->u.mons.current_attack_m = 0;
	map_data[m].npc[n]->u.mons.timer=
	add_timer(tick+rand()%5000+5000,mons_walk,m,n);
	return 0;
}

int mmo_mons_once_attack(int m,int n,int target_id,unsigned long tick)
{
  int damage,avoid,i;
  int fd=0;
  struct map_session_data *sd;

  int target_atc_x,target_atc_y;
// ADDED JUNE 7
// DISABLE IDLE MONSTER ATTACK
if(mons_data[map_data[m].npc[n]->class].aggressive==2)
    return 0;
  for(i=6;i<FD_SETSIZE;i++){
		if(session[i] && (sd=session[i]->session_data)){
			if(sd->account_id == target_id){
				fd=i;
				break;
				}
			}
		 //game ending (when it escapes)
		else
		{
		 mmo_mons_attack_no(m,n,target_id,tick);
		 return 0;
		}
  }
  m=sd->mapno;
	target_atc_x = map_data[m].npc[n]->x - sd->to_x;
	target_atc_y = map_data[m].npc[n]->y - sd->to_y;
  //when it can escape
  if((target_atc_x <= -3 || target_atc_x >= 3) && (target_atc_y <= -3 || target_atc_y >= 3))
  {
	  mmo_mons_attack_no(m,n,target_id,tick);
	  return 0;
  }

  if(sd->status.hp <= 0 ){
	//    printf("DIEEND m=%d n=%d\n",current_attack_m,n);
		mmo_mons_attack_no(m,n,target_id,tick);
		return 0;
		}

  if(n<0)
    return 1;

  if(map_data[m].npc[n]->u.mons.hp<=0)
			  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
		 if(map_data[m].npc[n]->u.mons.hp<=0){
			  map_data[m].npc[n]->u.mons.hp = 1000;
			  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
		  }
  // check in attack range

  // range of enemy is not the packet (?)(�G�̎˒��̓p�P�b�g�ł͂Ȃ�������ōl����K�v����)

  // check hit
  avoid=10 + sd->status.flee1 - mons_data[map_data[m].npc[n]->class].hit;
  if(avoid >= 100)
	  avoid = 95;
  if(rand()%100 <= avoid){
	  //attack if missed
    // make packet
    WFIFOW(fd,0) = 0x8a;
    WFIFOL(fd,2) = map_data[m].npc[n]->id;
    WFIFOL(fd,6) = sd->account_id;
    WFIFOL(fd,10) = tick;
    WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;
    WFIFOL(fd,18) = sd->status.aspd;
    WFIFOW(fd,22) = 0;
    WFIFOW(fd,24) = 0;
    WFIFOB(fd,26) = 0;
    WFIFOW(fd,27) = 0;
    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
  } else {
		  //when hit
		 // calc damage
	  if(rand()%100 <= sd->status.critical){
		 WFIFOW(fd,0) = 0x8a;
		 WFIFOL(fd,2) = map_data[m].npc[n]->id;
		 WFIFOL(fd,6) = sd->account_id;
		 WFIFOL(fd,10) = tick;
		 WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;//6000;//speed
		 WFIFOL(fd,18) = sd->status.aspd;//250;//enemy attack speed
		 WFIFOW(fd,22) = 0;//damage
		 WFIFOW(fd,24) = 0;
		 WFIFOB(fd,26) = 0x0b;
		 WFIFOW(fd,27) = 0;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
	  }
	  else{
		 if ( mons_data[map_data[m].npc[n]->class].atk1 != 0 )
		 damage=rand()%mons_data[map_data[m].npc[n]->class].atk1+mons_data[map_data[m].npc[n]->class].atk2;
		 else damage = mons_data[map_data[m].npc[n]->class].atk2;
		 damage -= sd->status.def1;
		 if(damage < 0)
			 damage = 1;
		 // make packet
		 WFIFOW(fd,0) = 0x8a;
		 WFIFOL(fd,2) = map_data[m].npc[n]->id;
		 WFIFOL(fd,6) = sd->account_id;
		 WFIFOL(fd,10) = tick;
		 WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;//6000;//speed
		 WFIFOL(fd,18) = sd->status.aspd;//250;//enemy attack speed
		 WFIFOW(fd,22) = damage;//damage
		 WFIFOW(fd,24) = 1;
		 WFIFOB(fd,26) = 0;
		 WFIFOW(fd,27) = 0;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
//		 printf("sd->status.hp = %d,damage = %d\n",sd->status.hp,damage);
		 sd->status.hp = sd->status.hp - damage;
		 if(sd->status.hp < 0)
			 sd->status.hp = 0;
		 WFIFOW(fd,0) = 0xb0;
		 WFIFOW(fd,2) = 0x05;
		 WFIFOL(fd,4) = sd->status.hp;
		 WFIFOSET(fd,8);
		   // ���S����������`
		   if(sd->status.hp <= 0 ){
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0x05;
			WFIFOL(fd,4) = sd->status.hp;
			WFIFOSET(fd,8);
			WFIFOW(fd,0) = 0x80;//transmits death packet
			WFIFOL(fd,2) = sd->account_id;
			WFIFOB(fd,6) = 1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
			mmo_mons_attack_no(m,n,target_id,tick);
			return 0;
		}//if(sd->status.hp terminal place
	  }
  }

  if(map_data[m].npc[n]->u.mons.attacktimer>= 0)
  {
	  delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
	  map_data[m].npc[n]->u.mons.attacktimer=-1;
  }

  if(map_data[m].npc[n]->u.mons.hp > 0){
	  if(mons_data[map_data[m].npc[n]->class].adelay<=0)
		  mons_data[map_data[m].npc[n]->class].adelay = 1000;
	//  printf("MONSTER ONCE ATTACK!!!\n");
	//  current_attack_m = m;
	  map_data[m].npc[n]->u.mons.current_attack_m = m;
	  map_data[m].npc[n]->u.mons.attacktimer
			= add_timer(tick+mons_data[map_data[m].npc[n]->class].adelay,mmo_mons_attack_continue,map_data[m].npc[n]->id,m);
  }

  return 0;
}


int mmo_mons_once_attack_start(int m,int n,int fd,unsigned long tick)
{
  int damage,avoid,target_id;
  struct map_session_data *sd;
  int target_atc_x,target_atc_y;
  
  if(session[fd]==NULL)
  {
	  mmo_mons_attack_no(m,n,-1,tick);
	  return 0;
  }  

  sd=session[fd]->session_data;
  target_id = sd->account_id;
// ADDED JUNE 7
// DISABLE IDLE MONSTER ATTACK
if(mons_data[map_data[m].npc[n]->class].aggressive==2)
    return 0;
    m=sd->mapno;
    target_atc_x = map_data[m].npc[n]->x - sd->to_x;
    target_atc_y = map_data[m].npc[n]->y - sd->to_y;
  if((target_atc_x <= -3 || target_atc_x >= 3) && (target_atc_y <= -3 || target_atc_y >= 3))
  {
	  mmo_mons_attack_no(m,n,target_id,tick);
	  return 0;
  }

  if(sd->status.hp <= 0 ){
		//mmo_mons_attack_no(m,n,target_id,tick);
		mmo_mons_attack_no(m,n,target_id,tick);
		return 0;
		}

  if(n<0)
    return 1;

  if(map_data[m].npc[n]->u.mons.hp<=0)
			  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
		 if(map_data[m].npc[n]->u.mons.hp<=0){
			  map_data[m].npc[n]->u.mons.hp = 1000;
			  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
		  }
  // check in attack range
  // check hit
  avoid=10 + sd->status.flee1 - mons_data[map_data[m].npc[n]->class].hit;
  if(avoid >= 100)
	  avoid = 95;
  if(rand()%100 <= avoid){
    // make packet
    WFIFOW(fd,0) = 0x8a;
    WFIFOL(fd,2) = map_data[m].npc[n]->id;
    WFIFOL(fd,6) = sd->account_id;
    WFIFOL(fd,10) = tick;
    WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;
    WFIFOL(fd,18) = sd->status.aspd;
    WFIFOW(fd,22) = 0;
    WFIFOW(fd,24) = 0;
    WFIFOB(fd,26) = 0;
    WFIFOW(fd,27) = 0;
    mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
  } else {
		  //�~�X����Ȃ��ꍇ
		 // calc damage
	  if(rand()%100 <= sd->status.critical){
		 WFIFOW(fd,0) = 0x8a;
		 WFIFOL(fd,2) = map_data[m].npc[n]->id;
		 WFIFOL(fd,6) = sd->account_id;
		 WFIFOL(fd,10) = tick;
		 WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;//6000;//�X�s�[�h
		 WFIFOL(fd,18) = sd->status.aspd;//250;//�G�̍U���X�s�[�h
		 WFIFOW(fd,22) = 0;//�_���[�W
		 WFIFOW(fd,24) = 0;
		 WFIFOB(fd,26) = 0x0b;
		 WFIFOW(fd,27) = 0;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
	  }
	  else{
		 if(mons_data[map_data[m].npc[n]->class].atk1 != 0)
		 damage=rand()%mons_data[map_data[m].npc[n]->class].atk1+mons_data[map_data[m].npc[n]->class].atk2;
		 else damage = mons_data[map_data[m].npc[n]->class].atk2; //DJP FIX2
		 damage -= sd->status.def1;
		 if(damage < 0)
			 damage = 1;
		 // make packet
		 WFIFOW(fd,0) = 0x8a;
		 WFIFOL(fd,2) = map_data[m].npc[n]->id;
		 WFIFOL(fd,6) = sd->account_id;
		 WFIFOL(fd,10) = tick;
		 WFIFOL(fd,14) = mons_data[map_data[m].npc[n]->class].speed;//6000;//�X�s�[�h
		 WFIFOL(fd,18) = sd->status.aspd;//250;//�G�̍U���X�s�[�h
		 WFIFOW(fd,22) = damage;//�_���[�W
		 WFIFOW(fd,24) = 1;
		 WFIFOB(fd,26) = 0;
		 WFIFOW(fd,27) = 0;
		 mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
		 //printf("sd->status.hp = %d,damage = %d\n",sd->status.hp,damage);
		 sd->status.hp = sd->status.hp - damage;
		 if(sd->status.hp < 0)
			 sd->status.hp = 0;
		 WFIFOW(fd,0) = 0xb0;
		 WFIFOW(fd,2) = 0x05;
		 WFIFOL(fd,4) = sd->status.hp;
		 WFIFOSET(fd,8);
		   // ���S����������`
		   if(sd->status.hp <= 0 ){
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0x05;
			WFIFOL(fd,4) = sd->status.hp;
			WFIFOSET(fd,8);
			WFIFOW(fd,0) = 0x80;//���S�p�P�b�g�𑗐M
			WFIFOL(fd,2) = sd->account_id;
			WFIFOB(fd,6) = 1;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );
			mmo_mons_attack_no(m,n,target_id,tick);
			return 0;
		}//if(sd->status.hp�I�[�ӏ�
	  }
  }

  if(map_data[m].npc[n]->u.mons.attacktimer>= 0)
  {
	  delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);
	  map_data[m].npc[n]->u.mons.attacktimer=-1;
  }

  if(map_data[m].npc[n]->u.mons.hp > 0){
	  if(mons_data[map_data[m].npc[n]->class].adelay<=0)
		  mons_data[map_data[m].npc[n]->class].adelay = 1000;
	//  printf("MONSTER ONCE ATTACK!!!\n");
	//  current_attack_m = m;
	  map_data[m].npc[n]->u.mons.current_attack_m = m;
	  map_data[m].npc[n]->u.mons.attacktimer
			= add_timer(tick+mons_data[map_data[m].npc[n]->class].adelay,mmo_mons_attack_continue,map_data[m].npc[n]->id,m);
  }

  return 0;
}

int mmo_mons_attack_continue(int tid,unsigned int tick,int id,int data)
{

//	id,sd->attacktarget,tick��fd, enemy�h�c�C(?)������
    int m,n;


	m=data;
	n=mmo_map_search_monster(m,id);
// ADDED JUNE 7
// DISABLE IDLE MONSTER ATTACK
if(mons_data[map_data[m].npc[n]->class].aggressive==2)
    return 0;
if(map_data[m].npc[n] != 0) {   //edit
	if(map_data[m].npc[n]->u.mons.hp<=0)
			  map_data[m].npc[n]->u.mons.hp = mons_data[map_data[m].npc[n]->class].max_hp;
	 if(map_data[m].npc[n]->u.mons.hp<=0){
			  map_data[m].npc[n]->u.mons.hp = 1000;
			  mons_data[map_data[m].npc[n]->class].max_hp = 1000;
		  }
	if(map_data[m].npc[n]->u.mons.hp > 0) {
		//mmo_mons_once_attack(m,n,map_data[m].npc[n]->u.mons.target_id,tick);
		mmo_mons_once_attack_start(m,n,map_data[m].npc[n]->u.mons.target_fd,tick);
		}
	else
		delete_timer(map_data[m].npc[n]->u.mons.attacktimer,mmo_mons_attack_continue);


} //->Lemming
	return 0;
}

/* constant attacking */
int mmo_map_attack_continue(int tid,unsigned int tick,int id,int data)
{
  struct map_session_data *sd;
  if(session[id]==NULL || (sd=session[id]->session_data)==NULL)
    return 0;
  mmo_map_once_attack(id,sd->attacktarget,tick);
  if((sd->attacktimer>=0) && (data!=1) )
    //Fred Fix1 (atack Speedy Function)
    sd->attacktimer=add_timer(tick+1500 - (sd->status.aspd * 2 - 100 * 2),mmo_map_attack_continue,id,0);
  return 0;
}

int set_lvup_table()
{
	FILE *fp;
	char line[2040];
	int i = 1;

	fp = fopen("config/exp.txt","r");
	if(fp)
	{
	  long tExpData[101];
	  long tSkillExpData[3][101];
	  while(fgets(line,1020,fp))
		{
	  if(sscanf(line,"%ld,%ld,%ld,%ld",
		  &tExpData[i],&tSkillExpData[0][i],//base experience value�Anovice skill up
		  &tSkillExpData[1][i],//primary job skill up
		  &tSkillExpData[2][i]) //secondary job skill up
		  != 4)
		  continue;
	  ExpData[i] = tExpData[i];
	  SkillExpData[0][i] = tSkillExpData[0][i];
	  SkillExpData[1][i] = tSkillExpData[1][i];
	  SkillExpData[2][i] = tSkillExpData[2][i];
	  i++;
	  if(i >= 110)//sense
		  break;
		  }
	  	fclose(fp);
	}
	return 0;
}

//periodic backup
int timer_backup(int tid,unsigned int tick,int id,int data)
{
	int i;
	struct map_session_data *sd;
	//Fred Fix2 (Dunno Yet but needed this change)
	for(i=5;i<=users_global+5;i++){//number of people loop
		if(!session[i] || !(sd=session[i]->session_data))//while deciding sd=substitution
				continue;
			strcpy(sd->status.last_point.map,sd->mapname);
			sd->status.last_point.x = sd->x;
			sd->status.last_point.y = sd->y;
		mmo_char_save(sd);
	}
	printf("You created a periodic backup.\n");
	return 0;
}

//HP recovery stability
int heal_hp(int tid,unsigned int tick,int id,int data)
{
//june-10-2003 Wisc aka Shen_long___
#define HP_TIME_T 1000        // Time since last called
#define HP_TIME_S 500        // Sitting bonus 
#define HP_TIME_R 3000        // Recovery time 1 hp
    int i, hp;
    struct map_session_data *sd;
    //Fred Fix3 (Hp/Sp Heal)
	for(i=5;i<=users_global+5;i++){//number of people loop
        if(!session[i] || !(sd=session[i]->session_data))
                continue;
        if(sd->status.hp < sd->status.max_hp && sd->status.hp != 0 && sd->walktimer<=0){
        //hp lower max and not walking
        //printf("heal_hp: %d %dn",sd->status.heal_time,sd->status.heal_time_skill);

            double bonus= ((double)(sd->status.vit +100)/(double)100);//Vit Bonus
            sd->status.heal_time += HP_TIME_T * bonus;
		    if(sd->sitting == 2)//when sitting
                sd->status.heal_time += HP_TIME_S * bonus;
            if( sd->status.heal_time >= HP_TIME_R ){
                sd->status.heal_time -= HP_TIME_R;

                sd->status.hp++;
                WFIFOW(i,0) = 0xb0;
                WFIFOW(i,2) = 0x05;
                WFIFOL(i,4) = sd->status.hp;
                WFIFOSET(i,8);

            }
            if(sd->status.skill[4-1].lv > 0){
                sd->status.heal_time_skill += HP_TIME_T;
                if(sd->status.heal_time_skill >= 10000){
                    sd->status.heal_time_skill=0;

                    hp = (sd->status.skill[4-1].lv * 5) * bonus;
                    if((hp + sd->status.hp) > sd->status.max_hp)
                    hp = (sd->status.max_hp - sd->status.hp);
                    sd->status.hp = sd->status.hp + hp;

                    WFIFOW(i,0) = 0x13d;
                    WFIFOW(i,2) = 5;
                    WFIFOL(i,4) = hp;
                    WFIFOSET(i,6);
                }
            }
        } else {    
            //walking or hp full
            sd->status.heal_time = 0; 
            sd->status.heal_time_skill = 0; 
        }


    }
    return 0;
}

//SP recovery stability
int heal_sp(int tid,unsigned int tick,int id,int data)
{
//june-10-2003 Wisc aka Shen_long___
#define SP_TIME_T 1000        // Time since last called
#define SP_TIME_S 500        // Sitting bonus 
#define SP_TIME_R 5000        // Recovery time 1 sp
    int i, sp;
    struct map_session_data *sd;
    double bonus;
    //Fred Fix3 (Hp/Sp Heal)
	for(i=5;i<=users_global+5;i++){//number of people loop
        if(!session[i] || !(sd=session[i]->session_data))
                continue;
        if(sd->status.sp < sd->status.max_sp && sd->walktimer<=0){  // -> && sd->status.sp != 0  regen also if 0 sp left
        //sp lower max and not walking
        //printf("heal_sp: %d %dn",sd->status.sp_time,sd->status.sp_time_skill);

            bonus= ((double)(sd->status.int_ +100)/(double)100);//Int Bonus
            sd->status.sp_time += SP_TIME_T * bonus;
		    if(sd->sitting == 2)//when sitting
                sd->status.sp_time += SP_TIME_S * bonus;
            if( sd->status.sp_time >= SP_TIME_R ){
                sd->status.sp_time -= SP_TIME_R;

                sd->status.sp++;
                WFIFOW(i,0) = 0xb0;
                WFIFOW(i,2) = 0x07;
                WFIFOL(i,4) = sd->status.sp;
                WFIFOSET(i,8);
            }
            if(sd->status.skill[9-1].lv > 0){
                sd->status.sp_time_skill += SP_TIME_T;
                if(sd->status.sp_time_skill >= 10000){
                    sd->status.sp_time_skill=0;

                    sp = (sd->status.skill[9-1].lv * 3) * bonus;
                    if((sp + sd->status.sp) > sd->status.max_sp)
                    sp = (sd->status.max_sp - sd->status.sp);
                    sd->status.sp = sd->status.sp + sp;

                    WFIFOW(i,0) = 0x13d;
                    WFIFOW(i,2) = 7;
                    WFIFOL(i,4) = sp;
                    WFIFOSET(i,6);
                }
            }
        } else {    
            //walking or sp full
            sd->status.sp_time = 0; 
            sd->status.sp_time_skill = 0; 
        }
    }
    return 0;
}
int monster_search(int fd,unsigned int tick,int id,int data){
	int i,j,m;
	int player_fd;
	struct map_session_data *sd;
//	tick=gettick();
	//session information (person has logged in) request
	for(i=6;i<users_global+6;i++){//number of people loop
		if(session[i] && (sd=session[i]->session_data)){
			player_fd=i;
			m=sd->mapno;
			for(j=0;j<map_data[m].npc_num;j++){
		  		if(map_data[m].npc[j]->block.subtype==MONS){//if monster
					if(map_data[m].npc[j]->u.mons.target_id <= 0){
						test_monster(player_fd,tick,map_data[m].npc[j]->id,m);//player fd transmits map number and monster information
					}
				}
		  	}
		}
	}
	return 0;
}

int loop_monster_search(int fd,unsigned int tick,int id,int data){
	int i,j,m;
	int player_fd;
	struct map_session_data *sd;
//	tick=gettick();
	//session information (person has logged in) request
	for(i=1;i<users_global+6;i++){//number of people loop
		if(session[i] && (sd=session[i]->session_data)){
			player_fd=i;
			m=sd->mapno;
			for(j=0;j<map_data[m].npc_num;j++){
		  		if(map_data[m].npc[j]->block.subtype==MONS){//If monster
					if(map_data[m].npc[j]->u.mons.target_id <= 0){
						if(mons_data[map_data[m].npc[j]->class].aggressive == 1 ||
						   mons_data[map_data[m].npc[j]->class].aggressive == 3 ||
						   mons_data[map_data[m].npc[j]->class].aggressive == 4) {
						test_monster(player_fd,tick,map_data[m].npc[j]->id,m);//player fd transmits map number and monster information 
					}	}
				}
		  	}
		}
	}
	return 0;
}


//every monster search routine
int test_monster(int fd,unsigned int tick,int id,int data){
	int dx,dy;
	int ret;
//	int i;
	struct map_session_data *sd;
//	int m=id;
//	int n=data;
	int m=data;
	int n;
	int target_sub_x,target_sub_y;
	n = mmo_map_search_monster(m,id);
	sd = session[fd]->session_data;
	target_sub_x = sd->x - map_data[m].npc[n]->x;
	target_sub_y = sd->y - map_data[m].npc[n]->y;
	if(sd->status.hp > 0){
		if(target_sub_x>=-12 && target_sub_x <= 12 && target_sub_y>=-12 && target_sub_y <= 12)
		{
			int x,y;
			if(map_data[m].npc[n]->u.mons.timer>=0)
			 delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
			/*****************  setting portable coordinate *****************/
			x=map_data[m].npc[n]->x;
			y=map_data[m].npc[n]->y;
			dx= map_data[m].npc[n]->u.mons.to_x - x;//���݂̈ʒu�Ǝ��̍��W�̍�
			dy= map_data[m].npc[n]->u.mons.to_y - y;
			//coordinate correction
			if(dx || dy){
				int target_atc_x,target_atc_y;
				// step
				if(dx<0) dx=-1;
				else if(dx>0) dx=1;
				if(dy<0) dy=-1;
				else if(dy>0) dy=1;
				map_data[m].npc[n]->x+=dx;
				map_data[m].npc[n]->y+=dy;

				dx= map_data[m].npc[n]->u.mons.to_x - map_data[m].npc[n]->x;
				dy= map_data[m].npc[n]->u.mons.to_y - map_data[m].npc[n]->y;
				 target_atc_x = map_data[m].npc[n]->x - sd->x;
				 target_atc_y = map_data[m].npc[n]->y - sd->y;
				 if((target_atc_x >= -3 && target_atc_x <= 3) && (target_atc_y >= -3 && target_atc_y <= 3)){
			//	 printf("MONSTER ATTACK! %d\n",sd->account_id);
			//		 delete_timer(map_data[m].npc[n]->u.mons.targettimer,test_monster);
			//	 delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
					 map_data[m].npc[n]->u.mons.target_id = sd->account_id;
					 map_data[m].npc[n]->u.mons.target_fd = fd;
					 map_data[m].npc[n]->u.mons.attacktimer
						= add_timer(tick+10,mmo_mons_attack_continue,map_data[m].npc[n]->id,m);
						//printf("hallo\n");
			//		 mmo_mons_once_attack(m,n,sd->account_id,tick);
					}
			//	 else{
				//	delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
		/*		 map_data[m].npc[n]->u.mons.targettimer=
					  add_timer(tick+map_data[m].npc[n]->u.mons.speed/2*((dx&&dy)?14:10)/10,test_monster,
											map_data[m].npc[n]->id,m);*/
				//	 printf("#1\n");
		//		 }
				return 0;
			}
			/*****************�@setting the following coordinate�@*****************/
		//	for(i=0;i<5;i++){
			if(target_sub_x>0) dx = target_sub_x-2;
			else if(target_sub_x<0) dx = target_sub_x+2;
			if(target_sub_y>0) dy = target_sub_y-2;
			else if(target_sub_y<0) dy = target_sub_y+2;
	//			dx = rand()%(abs(target_sub_x)*2+1)-abs(target_sub_x);
	//			dy = rand()%(abs(target_sub_y)*2+1)-abs(target_sub_y);
				ret=search_path(sd,m,map_data[m].npc[n]->x,map_data[m].npc[n]->y,map_data[m].npc[n]->x+dx,map_data[m].npc[n]->y+dy,1);
	//			if(ret == 0)
	//				break;
	//		}
			if(ret==0) {
			    unsigned char buf[256];
			    int target_atc_x,target_atc_y;
			    memset(buf,0,256);
			    target_atc_x = map_data[m].npc[n]->x - sd->to_x;
    			    target_atc_y = map_data[m].npc[n]->y - sd->to_y;
    			    if(mons_data[map_data[m].npc[n]->class].aggressive==3) {
				if((target_atc_x >= -6 && target_atc_x <= 6) && (target_atc_y >= -6 && target_atc_y <= 6)) {
				    map_data[m].npc[n]->u.mons.target_id = sd->account_id;
            			    map_data[m].npc[n]->u.mons.target_fd = fd;
            			    map_data[m].npc[n]->u.mons.attacktimer = add_timer(tick+10,mmo_mons_attack_continue,map_data[m].npc[n]->id,m);     
            			    mmo_mons_once_attack_start(m,n,fd,tick);
            			    return 0;
            			} else {
				    mmo_mons_attack_no(m,n,fd,tick);
                		    return 0;
            			}
        			return 0;
        		    }
        		if(mons_data[map_data[m].npc[n]->class].aggressive==4) {
            		    if((target_atc_x >= -2 && target_atc_x <= 2) && (target_atc_y >= -2 && target_atc_y <= 2)) {
				map_data[m].npc[n]->u.mons.target_id = sd->account_id;
            			map_data[m].npc[n]->u.mons.target_fd = fd;
            			map_data[m].npc[n]->u.mons.attacktimer = add_timer(tick+10,mmo_mons_attack_continue,map_data[m].npc[n]->id,m);
            			mmo_mons_once_attack_start(m,n,fd,tick);
            			return 0;
            		    } else {
				mmo_mons_attack_no(m,n,fd,tick);
                		return 0;
            		    }
        		    return 0;
        		}

		 //�ړ��惂���X�^�[�L�����N�^�[�ݒ�
		 WBUFW(buf,0)=0x7b;
		 WBUFL(buf,2)=map_data[m].npc[n]->id;
		 WBUFW(buf,6)=map_data[m].npc[n]->u.mons.speed;
		 WBUFW(buf,14)=map_data[m].npc[n]->class;
		 WBUFL(buf,22)=tick+map_data[m].npc[n]->u.mons.speed*((dx&&dy)?14:10)/10;
		 set_2pos(WBUFP(buf,50),map_data[m].npc[n]->x,map_data[m].npc[n]->y,map_data[m].npc[n]->x+dx,map_data[m].npc[n]->y+dy);
		 WBUFB(buf,55)=0;
		 WBUFB(buf,56)=5;
		 WBUFB(buf,57)=5;
		 mmo_map_sendarea_mxy(m,map_data[m].npc[n]->x,map_data[m].npc[n]->y,buf,packet_len_table[0x7b]);
		 map_data[m].npc[n]->u.mons.to_x=map_data[m].npc[n]->x+dx;
		 map_data[m].npc[n]->u.mons.to_y=map_data[m].npc[n]->y+dy;
		 if(map_data[m].npc[n]->u.mons.timer>=0)
			 delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
	//	 if(map_data[m].npc[n]->u.mons.targettimer>=0)
	//		 delete_timer(map_data[m].npc[n]->u.mons.targettimer,test_monster);
/*		 map_data[m].npc[n]->u.mons.targettimer=
			  add_timer(tick+map_data[m].npc[n]->u.mons.speed/2*((dx&&dy)?14:10)/10,
					  test_monster,map_data[m].npc[n]->id,m);*/
		 return 0;
		 /*
		 int target_atc_x,target_atc_y;
		 target_atc_x = map_data[m].npc[n]->u.mons.to_x - sd->x;
		 target_atc_y = map_data[m].npc[n]->u.mons.to_y - sd->y;
		 if((target_atc_x >= -1 && target_atc_x <= 1) && (target_atc_y >= -1 && target_atc_y <= 1)){
	//		 printf("MONSTER ATTACK! %d\n",sd->account_id);
			 delete_timer(map_data[m].npc[n]->targettimer);
			 delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
			 map_data[m].npc[n]->u.mons.target_id = sd->account_id;
			 mmo_mons_once_attack(m,n,sd->account_id,tick);
				 }

		 else{
	     map_data[m].npc[n]->targettimer=
			  add_timer(tick+map_data[m].npc[n]->u.mons.speed/2*((dx&&dy)?14:10)/10,test_monster,
									map_data[m].npc[n]->id,m);
			}
			*/
			}
	/*		else
			{
				if(map_data[m].npc[n]->u.mons.timer>=0)
				delete_timer(map_data[m].npc[n]->u.mons.timer,mons_walk);
				map_data[m].npc[n]->u.mons.timer=
					add_timer(tick+rand()%5000+5000,mons_walk,m,n);
				return 0;
			}*/
		}
	}
	return 0;
}

void set_equip(int fd,int item_num,int item_view)
{
  struct map_session_data *sd;
  sd=session[fd]->session_data;
 switch(item_num)
	{
	case 2:
		sd->status.weapon = item_view;
		break;
	case 3:
		sd->status.head_bottom = item_view;
		break;
	case 4:
		sd->status.head_top = item_view;
		break;
	case 5:
		sd->status.head_mid = item_view;
		break;
	case 8:
		sd->status.sheild = item_view;
		break;
	default:
		break;
	}
}

int parse_map(int fd)
{
  int i,x,y,parsing_packet_len,len,ret;
  int ep;
  int item_num = 0;
  int item_type = 0;
  int item_view = 0;
  int trade_i;
  int skill_lvl;            //skill_update
  struct map_session_data *sd;
  struct map_session_data *target_sd;
  struct mmo_charstatus *p;
  struct mmo_chat* temp_chat;
  struct item_db2 item_db;
  unsigned long tick;
  int skill_num = 0;
  char moji[256];
  //if no character data
  if(char_fd<0)
    session[fd]->eof=1;
  sd=session[fd]->session_data;
	p=&sd->status;
  //present location and acquisition information?
  //if(session[fd]->eof){
//hud test fix for players.txt wipe (sanity check)
  if((session[fd]->eof) && (char_fd == fd))  
    char_fd = -1; 
    else if (session[fd]->eof)
  { 
 	mmo_char_save(sd);
    if(fd==char_fd)
      char_fd=-1;
    if(sd && sd->state.auth){
      if(sd->chatID)
	mmo_map_leavechat(fd,(struct mmo_chat*)sd->chatID,sd->status.name);
      del_block(&sd->block);
      WFIFOW(fd,0)=0x80;
      WFIFOL(fd,2)=sd->account_id;
      WFIFOB(fd,6)=2;	//logout?
      mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x80], 1 );
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
    close(fd);
    delete_session(fd);
    return 0;
  }
  //acquisition end

  tick=gettick();

  //packet acquisition loop
  if(RFIFOREST(fd)>=2){
    if(RFIFOW(fd,0)>=0x200 || packet_len_table[RFIFOW(fd,0)]==0 ||
       ((!sd || (sd && sd->state.auth==0)) && RFIFOW(fd,0)!=0x72)){
      close(fd);
      session[fd]->eof=1;
      return 0;
    }
    parsing_packet_len=packet_len_table[RFIFOW(fd,0)];
    if(parsing_packet_len==-1){
      if(RFIFOREST(fd)<4)
	return 0;
      parsing_packet_len=RFIFOW(fd,2);
    }

    if(RFIFOREST(fd)<parsing_packet_len)
      return 0;
    //printf("%x\n",RFIFOW(fd,0)); // THIS
    switch(RFIFOW(fd,0)){
    case 0x72: // want to connection
      if(sd==NULL){
	sd=session[fd]->session_data=malloc(sizeof(*sd));
	memset(sd,0,sizeof(*sd));
      }
      WFIFOW(char_fd,0)=0x2afc;
      sd->account_id=WFIFOL(char_fd,2)=RFIFOL(fd,2);
      sd->char_id=WFIFOL(char_fd,6)=RFIFOL(fd,6);
      sd->login_id1=WFIFOL(char_fd,10)=RFIFOL(fd,10);
      sd->client_tick=RFIFOL(fd,14);
      sd->sex=RFIFOB(fd,18);
      sd->state.auth=0;
      sd->fd=fd;
      sd->block.type=BL_PC;
	  // ADDED on 06/14/2003 by SilentAvenger ---------
      sd->status.talking_to_npc=0;
      // ----------------------------------------------
      WFIFOSET(char_fd,14);
      WFIFOL(fd,0)=RFIFOL(fd,2);
      WFIFOSET(fd,4);
      break;
    case 0x7e: // tick send
      sd->client_tick=RFIFOL(fd,2);
      WFIFOL(fd,0)=0x7f;
      sd->server_tick=WFIFOL(fd,2)=tick;
      WFIFOSET(fd,6);
      break;
    case 0x7d: // load end ack
      mmo_send_selfdata(fd,sd);
	  mmo_map_calc_status(fd,0,0);
	  mmo_map_calc_card(fd,0,0,1);
  	  set_lvup_table();
      mmo_map_getareachar(fd);
      mmo_map_set0079(fd,WFIFOP(fd,0));
      mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x79], 1 );

			// ADDED on 04/09/2003 -------------
			pet_init(sd);
			if(sd->status.pet.activity == 1)
			{
				char buf[256];
				#define add_block_npc(m,n) {add_block(&map_data[m].npc[n]->block,m,map_data[m].npc[n]->x,map_data[m].npc[n]->y); \
				map_data[m].npc[n]->block.type=BL_NPC;}

				if(sd->status.pet.pet_npc_id_on_map[sd->mapno] == -1)
				{
						sd->status.pet.pet_npc_id_on_map[sd->mapno] = map_data[sd->mapno].npc_num;
						map_data[sd->mapno].npc_num++;
				}

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
				add_block_npc(sd->mapno,sd->status.pet.pet_npc_id_on_map[sd->mapno]);
				mmo_map_set_npc(0,buf,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->id,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->class,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y,0);
				mmo_map_sendarea_mxy(sd->mapno,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->x,map_data[sd->mapno].npc[sd->status.pet.pet_npc_id_on_map[sd->mapno]]->y,buf,packet_len_table[0x78]);

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
			//printf("PET_ID=%d, ACTIVITY=%d\n",sd->status.pet.pet_id_as_npc,sd->status.pet.activity);
			// ---------------------------------
      break;
    case 0x85: // walk to (x,y)
      if(sd->attacktimer>=0){
	delete_timer(sd->attacktimer,mmo_map_attack_continue);
	sd->attacktimer=-1;
	sd->status.talking_to_npc=0;
      }
	  //position acquisition?(���݈ʒu�擾�H)
      x=RFIFOB(fd,2)*4+(RFIFOB(fd,3)>>6);
      y=((RFIFOB(fd,3)&0x3f)<<4)+(RFIFOB(fd,4)>>4);

	//printf("x=%d y=%d, to_x=%d to_y=%d\n",x,y,sd->x,sd->y);

      if(search_path(sd,sd->mapno,sd->x,sd->y,x,y,0)==0){
	sd->to_x=x;
	sd->to_y=y;
	//movement response
	WFIFOW(fd,0)=0x87;
	WFIFOL(fd,2)=tick;
	set_2pos(WFIFOP(fd,6),sd->x,sd->y,x,y);
	WFIFOB(fd,11)=0;
	WFIFOSET(fd,12);
	mmo_map_set007b(fd,WFIFOP(fd,0),tick,sd->x,sd->y,x,y);
	mmo_map_sendarea(fd,WFIFOP(fd,0),packet_len_table[0x7b],1);
	if(sd->walktimer>=0){
	  delete_timer(sd->walktimer,walk_char);
	  sd->walktimer=-1;
	}
	if((i=calc_next_walk_step(fd))>0){
	  sd->walktimer=add_timer(tick+i/2,walk_char,fd,0);
	  //printf("walktimer=%d\n",sd->walktimer);
	}
      }
      break;
    case 0x0089: /* attack sitting request */
      if(sd->walktimer>=0){	// stop walking
	delete_timer(sd->walktimer,walk_char);
	sd->walktimer=-1;
      }
    switch(RFIFOB(fd,6)) {
      case 0x0007:  /* ctrl-click attack */
	if(sd->attacktimer>0){
	  delete_timer(sd->attacktimer,mmo_map_attack_continue);
	  sd->attacktimer=-1;
	}
	sd->attacktarget=RFIFOL(fd,2);
	sd->attacktimer=add_timer(tick+10,mmo_map_attack_continue,fd,0);
	break;
      case 0x0000:  /* once attack */

	if(sd->attacktimer>0){
	  delete_timer(sd->attacktimer,mmo_map_attack_continue);
	  sd->attacktimer=-1;
	}
	sd->attacktarget = RFIFOL(fd,2);
    sd->attacktimer=add_timer(tick+10,mmo_map_attack_continue,fd,1);
	break;
      case 0x0002: /* sit down */
	if (sd->status.skill[0].lv >= 3) {
	WFIFOW(fd,0) = 0x8a;
	WFIFOL(fd,2) = sd->account_id;
	WFIFOB(fd,26) = 2;
	mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
	sd->sitting=2;	// 1 makes corpse visible when person logs in (?)(1�ɂ����login�����l����͎��̂Ɍ�����)
	} else {
		strcpy(moji,"You must obtain basic skill level 3 in order to sit.");
		WFIFOW(fd,0) = 0x8e;
		WFIFOW(fd,2) = 4 + strlen(moji);
		memcpy(WFIFOP(fd,4),moji,strlen(moji));
		WFIFOSET(fd,4 + strlen(moji));
	}
	break;
      case 0x0003: /* stand up */
	WFIFOW(fd,0) = 0x8a;
	WFIFOL(fd,2) = sd->account_id;
	WFIFOB(fd,26) = 3;
	mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x8a], 0 );
	sd->sitting=0;
	break;
      }
      break;
    case 0x118:	// stop repeat attack
      if(sd->attacktimer>=0){
	delete_timer(sd->attacktimer,mmo_map_attack_continue);
	sd->attacktimer=-1;
      }
      break;
	  //skill up
	case 0x112:
		skill_num=RFIFOW(fd,2);
		mmo_map_skill_lv_up(fd,skill_num);
		//sword mastery changes atk of weapon
		if ((skill_num == 2)||(skill_num == 3)) mmo_map_calc_status(fd,0,0);
		break;
    //designated target attack and skill place***********************************
	case 0x113:
		sd->attacktarget = RFIFOL(fd,6);//target ID
		skill_num = RFIFOW(fd,4);//perminate magical skill
		skill_lvl = RFIFOW(fd,2);        //skill_update
		if(sd->status.sp < skill_db[skill_num].sp){
			/*
			R 0110 <skill ID>.w <basic type>.w ?.w <fail>.B <type>.B
			fail=00 skill failure?
			type 00:basic type 01:insufficient SP 
			*/
			WFIFOW(fd,0) = 0x110;
			WFIFOW(fd,2) = RFIFOW(fd,2);
			WFIFOW(fd,4) = 05;
			WFIFOW(fd,6) = 0;
			WFIFOB(fd,8) = 00;
			WFIFOB(fd,9) = 01;//insufficient sp? 
			WFIFOSET(fd,10);
			break;
		}else{
			sd->status.sp -= skill_db[skill_num].sp;
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0007;
			WFIFOL(fd,4) = sd->status.sp;
			WFIFOSET(fd,8);
			//printf("0x113\n");
			mmo_map_skill_attack(fd,RFIFOL(fd,6),skill_num,skill_lvl,tick,0,0);
			break;
		}
	//****************************************************************
	//partner skill reaction************************************
	case 0x116:
		skill_num = RFIFOW(fd,4);//skill ID
		skill_lvl = RFIFOW(fd,2);        //skill_update
		if(sd->status.sp < skill_db[skill_num].sp){
			/*
			R 0110 <skill ID>.w <basic type>.w ?.w <fail>.B <type>.B
			fail=00 skill failure?
			type 00:basic type 01:insufficient SP
			*/
			WFIFOW(fd,0) = 0x110;
			WFIFOW(fd,2) = RFIFOW(fd,2);
			WFIFOW(fd,4) = 05;
			WFIFOW(fd,6) = 0;
			WFIFOB(fd,8) = 00;
			WFIFOB(fd,9) = 01;//insufficient sp?
			WFIFOSET(fd,10);
			break;
		}else{
			int skill_x,skill_y;
			sd->status.sp -= skill_db[skill_num].sp;
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0007;
			WFIFOL(fd,4) = sd->status.sp;
			WFIFOSET(fd,8);
			//printf("0x116\n");
			skill_x = RFIFOW(fd,6);
			skill_y = RFIFOW(fd,8);
			mmo_map_skill_attack(fd,RFIFOL(fd,6),skill_num,skill_lvl,tick,0,0);
			break;
		}
	//****************************************************************

    case 0x8c: // global message
#include "atcomand.c"
      WFIFOW(fd,0)=0x8d;
      WFIFOW(fd,2)=RFIFOW(fd,2)+4;
      WFIFOL(fd,4)=sd->account_id;
      memcpy(WFIFOP(fd,8),RFIFOP(fd,4),RFIFOW(fd,2)-4);
      if(sd->chatID) mmo_map_sendchat(fd,WFIFOP(fd,0),WFIFOW(fd,2),1);
      else mmo_map_sendarea(fd,WFIFOP(fd,0),WFIFOW(fd,2),2);

      memcpy(WFIFOP(fd,0),RFIFOP(fd,0),RFIFOW(fd,2));
      WFIFOW(fd,0)=0x8e;
      WFIFOSET(fd,WFIFOW(fd,2));
idou:
      break;
	  //character name inquiry
	case 0x193:
		mmo_map_send0095(fd,RFIFOL(fd,2));
		break;
    case 0x94: // get char name req
      mmo_map_send0095(fd,RFIFOL(fd,2));
      break;
    case 0x96: // wis
      sprintf(WFIFOP(fd,0),"wis to %s %s",RFIFOP(fd,4),RFIFOP(fd,28));
      write_log(WFIFOP(fd,0));
      if(strcmp(RFIFOP(fd,4),mapmove_name)==0){
	char mapname[32];
	int x,y;
	if(sscanf(RFIFOP(fd,28),"%s%d%d",mapname,&x,&y)!=3)
	  break;
	for(i=0;map[i][0];i++)
	  if(strcmp(map[i],mapname)==0){
	    mmo_map_changemap(fd,sd,mapname,x,y,2);
	    break;
	  }
	if(map[i][0]==0){
	  WFIFOW(fd,0)=0x98;
	  WFIFOB(fd,2)=2;
	  WFIFOSET(fd,3);
	}
      }else
		  mmo_map_sendwis(fd);
      break;
    case 0x99: // GM message
      if(sd->account_id > 100000) {
        write_log(RFIFOP(fd,4));
        memcpy(WFIFOP(fd,0),RFIFOP(fd,0),RFIFOW(fd,2));
        WFIFOW(fd,0)=0x9a;
        mmo_map_sendall(fd, WFIFOP(fd,0), WFIFOW(fd,2), 0 );
      } else {
	char moji[200];
	strcpy(moji, "You are not a GM!");
	WFIFOW(fd,0) = 0x8e;
	WFIFOW(fd,2) = 4 + strlen(moji);
	memcpy(WFIFOP(fd,4),moji,strlen(moji));
	WFIFOSET(fd,4 + strlen(moji));
      }
      break;
    case 0x9b: // change dir
      WFIFOW(fd,0)=0x9c;
      WFIFOL(fd,2)=sd->account_id;
      WFIFOW(fd,6)=RFIFOW(fd,2);
      WFIFOB(fd,8)=RFIFOW(fd,4);
      mmo_map_sendarea(fd,WFIFOP(fd,0),9,1);
      sd->head_dir=RFIFOW(fd,2);
      sd->dir=RFIFOB(fd,4);
      break;
    case 0xb2:
      switch(RFIFOB(fd,2)) {
		case 0x00: // restart
      			sd->status.hp=2;//HP = 1
			WFIFOW(fd,0) = 0xb0;
			WFIFOW(fd,2) = 0x05;
			WFIFOL(fd,4) = sd->status.hp;
			WFIFOSET(fd,8);

  			p=&sd->status;
      		strcpy(sd->mapname,p->save_point.map);
		//strcpy(sd->mapname,p->save_point.map);

			sd->x = p->save_point.x;
			sd->y = p->save_point.y;
			//printf("X: %d\nY: %d",sd->x,sd->y,sd->mapname);
			mmo_map_changemap(fd,sd,sd->mapname,sd->x,sd->y,2);
			mmo_char_save(sd);//save
			break;
      	case 0x01: // char sel
			if(sd->status.hp==0)
         {
	         sd->status.hp=1;
	      }
         else
         {
            p=&sd->status;
	         strcpy(p->last_point.map,sd->mapname);
				p->last_point.x = sd->x;
				p->last_point.y = sd->y;
	      }
	      	mmo_char_save(sd);//save

			WFIFOW(fd,0) = 0x80;
			WFIFOL(fd,2) = sd->account_id;
			WFIFOB(fd,6) = 2;
			mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );

			WFIFOW(fd,0)=0xb3;//character select image 
			WFIFOB(fd,2)=1;
			WFIFOSET(fd,3);

			WFIFOW(char_fd,0)=0x2b02;//memory information in character server
			WFIFOL(char_fd,2)=sd->account_id;
			WFIFOL(char_fd,6)=sd->login_id1;
			 sd->account_id = -1;
			WFIFOSET(char_fd,10);
			break;
      }
      break;
    case 0xbf: // emotion
      WFIFOW(fd,0)=0xc0;
      WFIFOL(fd,2)=sd->account_id;
      WFIFOB(fd,6)=RFIFOB(fd,2);
      mmo_map_sendarea(fd,WFIFOP(fd,0),7,0);
      break;
    case 0xc1: // how many connections
      WFIFOW(fd,0)=0xc2;
      WFIFOL(fd,2)=users_global;
      WFIFOSET(fd,6);
      break;
    case 0x18a: // quit game
      if(sd->status.hp==0){//if death status after logout
      	sd->status.hp=1;
      }
      p=&sd->status;
		strcpy(p->save_point.map,sd->mapname);//renewal of position information
		p->save_point.x = sd->x;
		p->save_point.y = sd->y;
		mmo_char_save(sd);//save

		WFIFOW(fd,0) = 0x80;
		WFIFOL(fd,2) = sd->account_id;
		WFIFOB(fd,6) = 2;
		mmo_map_sendarea( fd, WFIFOP(fd,0), packet_len_table[0x80], 0 );

		WFIFOW(fd,0)=0x18b;
		WFIFOW(fd,2)=0;
		sd->account_id = -1;
		WFIFOSET(fd,4);
		break;
case 0xf3: //sendStorageAdd
//RFIFOW(fd,2) index inventory
//RFIFOL(fd,4) amount
if( (sd->status.storage_amount <= MAX_STORAGE) && (sd->status.storage_status == 1) ) { // storage not full & storage open
if(RFIFOW(fd,2)>=2 && RFIFOW(fd,2)<MAX_INVENTORY+2) { // valid index
    if( (RFIFOL(fd,4) <= sd->status.inventory[RFIFOW(fd,2)-2].amount) && (RFIFOL(fd,4) > 0) ) { //valid amount

     ep=0; // used for a check
          
     for(i=0;i<MAX_STORAGE;i++) {
        if( 
         (sd->status.storage[i].nameid == sd->status.inventory[RFIFOW(fd,2)-2].nameid) &&
         (sd->status.storage[i].identify == sd->status.inventory[RFIFOW(fd,2)-2].identify) &&
         (sd->status.storage[i].attribute == sd->status.inventory[RFIFOW(fd,2)-2].attribute) &&
         (sd->status.storage[i].refine == sd->status.inventory[RFIFOW(fd,2)-2].refine) &&
         (sd->status.storage[i].card[0] == sd->status.inventory[RFIFOW(fd,2)-2].card[0]) &&
         (sd->status.storage[i].card[1] == sd->status.inventory[RFIFOW(fd,2)-2].card[1]) &&
         (sd->status.storage[i].card[2] == sd->status.inventory[RFIFOW(fd,2)-2].card[2]) &&
         (sd->status.storage[i].card[3] == sd->status.inventory[RFIFOW(fd,2)-2].card[3])
         ) {    break;    }
        else if(sd->status.storage[i].amount==0) {break;    }
     }
    
    // add item to storage
    if(sd->status.storage[i].amount < 1) {
    sd->status.storage[i] = sd->status.inventory[RFIFOW(fd,2)-2];
    sd->status.storage[i].amount = RFIFOL(fd,4);
    }
    else {
    sd->status.storage[i].amount += RFIFOL(fd,4);
    ep=1; // item type was already in storage -> dont raise storage amount
    }
    // remove item from inventory
     len=mmo_map_item_lost(fd,WFIFOP(fd,0),RFIFOW(fd,2),RFIFOL(fd,4));
    if(len>0) WFIFOSET(fd,len);
    
    // send packet
    WFIFOW(fd,0) = 0xf4; // Storage item added
    WFIFOL(fd,2) = i+1; // index
    WFIFOL(fd,4) = RFIFOL(fd,4); // amount
    WFIFOL(fd,8) = sd->status.storage[i].nameid; // id
    WFIFOB(fd,10) = sd->status.storage[i].identify; //identify flag
    WFIFOB(fd,12) = sd->status.storage[i].attribute; // attribute
    WFIFOB(fd,14) = sd->status.storage[i].refine; //refine
    WFIFOW(fd,16) = sd->status.storage[i].card[0]; //card (4w)
    WFIFOW(fd,17) = sd->status.storage[i].card[1]; //card (4w)
    WFIFOW(fd,18) = sd->status.storage[i].card[2]; //card (4w)
    WFIFOW(fd,19) = sd->status.storage[i].card[3]; //card (4w)
    WFIFOSET(fd,21);
    if(ep!=1) { // item type was not already in storage
    // update vars
    sd->status.storage_amount++;
    // update storage amount
    WFIFOW(fd,0) = 0xf2;
    WFIFOL(fd,2) = sd->status.storage_amount;  //items
    WFIFOL(fd,4) = MAX_STORAGE; //items max
    WFIFOSET(fd,6);
    
    ep=0;
    }
    mmo_char_save(sd);
 } // valid amount
}// valid index
}// storage not full & storage open

break;

case 0xf5: //sendStorageGet

//RFIFOW(fd,2) index storage
//RFIFOL(fd,4) amount


if(sd->status.storage_status == 1) { //  storage open
if(RFIFOW(fd,2)>=1 && RFIFOW(fd,2)<MAX_STORAGE+1) { // valid index
    if( (RFIFOL(fd,4) <= sd->status.storage[RFIFOW(fd,2)-1].amount) && (RFIFOL(fd,4) > 0) ) { //valid amount
        
     sd->status.storage[RFIFOW(fd,2)-1].amount = sd->status.storage[RFIFOW(fd,2)-1].amount-RFIFOL(fd,4); // new amount of item in storage
     ep=sd->status.storage[RFIFOW(fd,2)-1].amount; // save the new amount
     sd->status.storage[RFIFOW(fd,2)-1].amount=RFIFOL(fd,4); // set amount to amount which goes to inventory (to avoid an item buffer)
     len=mmo_map_item_get(fd,WFIFOP(fd,0),&sd->status.storage[RFIFOW(fd,2)-1]);
      if(len>0) WFIFOSET(fd,len);

    sd->status.storage[RFIFOW(fd,2)-1].amount=ep; // backup the storage amount 
        
    // send packet
    WFIFOW(fd,0) = 0xf6; // Storage item removed
    WFIFOL(fd,2) = RFIFOW(fd,2); // index
    WFIFOL(fd,4) = RFIFOL(fd,4); // amount
    WFIFOSET(fd,8);
    
    if(sd->status.storage[RFIFOW(fd,2)-1].amount < 1) { // no item left in storage
    // update vars
    sd->status.storage[RFIFOW(fd,2)-1].nameid=0;
    sd->status.storage_amount--;
    // update storage amount
    WFIFOW(fd,0) = 0xf2;
    WFIFOL(fd,2) = sd->status.storage_amount;  //items
    WFIFOL(fd,4) = MAX_STORAGE; //items max
    WFIFOSET(fd,6);
    }
    ep=0; // reset ep
    mmo_char_save(sd);
 } // valid amount
}// valid index
}// storage open


break;

case 0xf7: //sendStorageClose
sd->status.storage_status=0;
WFIFOW(fd,0) = 0xf8; // Storage Closed
WFIFOSET(fd,2);
break;

    case 0x00d5: /* create chat room */
      temp_chat = (struct mmo_chat* )malloc(sizeof(struct mmo_chat));
      memset(temp_chat,0,sizeof(struct mmo_chat));
      temp_chat->len = RFIFOW(fd,2);
      temp_chat->limit = RFIFOW(fd,4);
      temp_chat->pub = RFIFOB(fd,6);
      memset(temp_chat->pass,0,8);
      memcpy(temp_chat->pass,RFIFOP(fd,7),8);
      memset(temp_chat->title,0,62);
      memcpy(temp_chat->title,RFIFOP(fd,15),(temp_chat->len - 15));
      temp_chat->ownID = sd->account_id;
      temp_chat->usersfd[0] = fd;
      temp_chat->usersID[0] = temp_chat->ownID; /* join users ID */
      temp_chat->chatID = (unsigned long)temp_chat;
      sd->chatID = temp_chat->chatID;
      memcpy(temp_chat->usersName[0],sd->status.name,24);
      temp_chat->users = 1;
      mmo_map_createchat(temp_chat);

      WFIFOW(fd,0) = 0x0d6;
      WFIFOB(fd,2) = 0;
      WFIFOSET( fd, 3);

      i=mmo_map_set00d7(fd,WFIFOP(fd,0));
      mmo_map_sendarea( fd, WFIFOP(fd, 0),i,1);
      break;
    case 0x00d9: /* chat add member */
      mmo_map_addchat( fd,(struct mmo_chat*)RFIFOL(fd, 2),RFIFOP(fd, 6));
      break;
    case 0x00de: /* chat room status change */
      if(0) break; // check owner
      temp_chat = (struct mmo_chat*)sd->chatID;
      temp_chat->len = RFIFOW(fd, 2);
      temp_chat->limit = RFIFOW(fd, 4);
      temp_chat->pub = RFIFOB(fd, 6);
      memset(temp_chat->pass,0,8);
      memcpy(temp_chat->pass,RFIFOP(fd, 7),8);
      memset(temp_chat->title,0,62);
      memcpy(temp_chat->title,RFIFOP(fd,15),(temp_chat->len - 15));

      WFIFOW(fd, 0) = 0xdf;
      WFIFOW(fd, 2) = strlen(temp_chat->title)+1+17;	/* len */
      WFIFOL(fd, 4) = temp_chat->ownID; /* ownID */
      WFIFOL(fd, 8) = temp_chat->chatID;	/* chatID */
      WFIFOW(fd,12) = temp_chat->limit;  /* limit */
      WFIFOW(fd,14) = temp_chat->users;  /* users */
      WFIFOB(fd,16) = temp_chat->pub;  /* pub */
      strcpy(WFIFOP(fd,17),temp_chat->title);	/* title */
      mmo_map_sendchat( fd, WFIFOP(fd, 0),WFIFOW(fd,2),0);

      i=mmo_map_set00d7(fd,WFIFOP(fd,0));
      mmo_map_sendarea(temp_chat->usersfd[0], WFIFOP(fd, 0),i,3);

      break;
    case 0x00e0: // change owner
      if(0) break; // check owner
      mmo_map_changeowner(fd,(struct mmo_chat*)sd->chatID,RFIFOP(fd,6));
      break;
    case 0x00e2: /* Kick from chat  */
      if(0) break; // check owner
      mmo_map_leavechat(fd,(struct mmo_chat*)sd->chatID,RFIFOP(fd,2));
      break;
    case 0x00e3: /* chat leave */
      mmo_map_leavechat(fd,(struct mmo_chat*)sd->chatID,sd->status.name);
      break;
    case 0xbb: // status up
      mmo_map_status_up(fd,RFIFOW(fd,2));
      break;
    case 0x90: // NPC clicked
      npc_click(fd,RFIFOL(fd,2));
      break;
      // for script NPC
    case 0xb8: // NPC select menu
      npc_menu_select(fd,RFIFOL(fd,2),RFIFOB(fd,6));
      break;
    case 0xb9: // NPC next clicked
      npc_next_click(fd,RFIFOL(fd,2));
      sd->status.talking_to_npc=0;
      break;
    case 0x143: // NPC amount input
      npc_amount_input(fd,RFIFOL(fd,2),RFIFOL(fd,6));
      break;
    case 0x146: // NPC close clicked
      npc_close_click(fd,RFIFOL(fd,2));
      sd->status.talking_to_npc=0;
      break;
      // for shop NPC
    case 0xc5: // NPC buy or sell selected
      npc_buysell_selected(fd,RFIFOL(fd,2),RFIFOB(fd,6));
	  // ADDED on 06/14/2003 by SilentAvenger ---------
      sd->status.talking_to_npc=0;
      // ----------------------------------------------
      break;
    case 0xc8: // NPC buy list send
      npc_buy_selected(fd,RFIFOP(fd,4),(RFIFOW(fd,2)-4)/4);
	  // ADDED on 06/14/2003 by SilentAvenger ---------
      sd->status.talking_to_npc=0;
      // ----------------------------------------------
      break;
    case 0xc9: // NPC sell list send
      npc_sell_selected(fd,RFIFOP(fd,4),(RFIFOW(fd,2)-4)/4);
	  // ADDED on 06/14/2003 by SilentAvenger ---------
      sd->status.talking_to_npc=0;
      // ----------------------------------------------
      break;
      // floor item
    case 0xa2: // drop item
      mmo_map_dropitem(fd,RFIFOW(fd,2),RFIFOW(fd,4));
      sd->status.talking_to_npc=0;
      break;
    case 0x9f: // take item
      mmo_map_takeitem(fd,RFIFOL(fd,2));
      sd->status.talking_to_npc=0;
      break;
      //item equipment S 00a9 <index>.w <equip type>.w
    case 0xa9:
	//search item equipment points for item number sd->status.inventory[RFIFOW(fd,2)-2].nameid
		//ADDED on 04/10/2003 -------------------
		#include "00a9.c"
		// --------------------------------------

     ep=itemdb_equip_point(sd->status.inventory[RFIFOW(fd,2)-2].nameid,sd);
     item_num = itemdb_stype(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
     item_type = RFIFOW(fd,4);
	 item_view = itemdb_view_point(sd->status.inventory[RFIFOW(fd,2)-2].nameid);

			//printf("eq=%d item_type=%d\n",ep,item_type);
		//lvl equipt check wisc
		item_db = item_database(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
		if(sd->status.base_level < item_db.elv) {
			WFIFOW(fd,0)=0xaa;
			WFIFOW(fd,2)=RFIFOW(fd,2);
			WFIFOB(fd,6)=0;    // cannot equip
			WFIFOSET(fd,7);
		break;
		}
		//can equip weapon huh? :D wisc
		if(!itemdb_can_equipt_weapon(sd->status.inventory[RFIFOW(fd,2)-2].nameid,sd->status.class)) {
			WFIFOW(fd,0)=0xaa;
			WFIFOW(fd,2)=RFIFOW(fd,2);
			WFIFOB(fd,6)=0;    // cannot equip
			WFIFOSET(fd,7);
		break;
		}  

     if((ep&RFIFOW(fd,4))==0){
		 //item equipment response<index>.w <equip point>.w <type>.B
		 //bare hand equipment R 00aa <index>.w <equip point>.w <type>.B
		WFIFOW(fd,0)=0xaa;
		WFIFOW(fd,2)=RFIFOW(fd,2);
		WFIFOB(fd,6)=0;
		WFIFOSET(fd,7);
      } else
	 	 //accessory processing
	 if(ep == 0x88)
	 {
   	 int j = 0;
		//check for what�fs already equipped
		 for(i=0;i < 100;i++)
		 {
			 //the right opening (?)(�E�������Ă�)
			 if(sd->status.inventory[i].nameid && (sd->status.inventory[i].equip&0x80)){
				 ep = ep & 0x08;
				 j = i;
			 }
			 else if(sd->status.inventory[i].nameid && (sd->status.inventory[i].equip&0x08))
				ep = ep & 0x80;
		 }
		 //both being buried(?)(�������܂��Ă�)
		 if(ep == 0)
		 {
			WFIFOW(fd,0)=0xac;
			WFIFOW(fd,2)=j;
			WFIFOW(fd,4)=sd->status.inventory[j].equip;
			sd->status.inventory[j].equip=0;
			WFIFOB(fd,6)=1;
			WFIFOSET(fd,7);
			mmo_map_calc_status(fd,sd->status.inventory[j].nameid,0);
			mmo_map_calc_card(fd,sd->status.inventory[j].nameid,j,0);
			ep = 0x80;
		 }
		 //both opening(?)(���������Ă�)
		 else if(ep == 0x88)
		 {
			ep = 0x08;
		 }
		WFIFOW(fd,0)=0xaa;
		WFIFOW(fd,2)=RFIFOW(fd,2);
		WFIFOW(fd,4)=ep;
		WFIFOB(fd,6)=1;
		WFIFOSET(fd,7);
		sd->status.inventory[RFIFOW(fd,2)-2].equip=ep;
		mmo_map_calc_status(fd,sd->status.inventory[RFIFOW(fd,2)-2].nameid,1);
		mmo_map_calc_card(fd,0,0,1);
	 }
	 else {
				 //check equipment place
			for(i=0;i<100;i++){
				//if already equipped, then cancel equip
				//	R 00ac <index>.w <equip point>.w <type>.B
			  if(sd->status.inventory[i].nameid &&
				 (sd->status.inventory[i].equip&ep)){
				WFIFOW(fd,0)=0xac;
				WFIFOW(fd,2)=i+2;
				WFIFOW(fd,4)=sd->status.inventory[i].equip;
				WFIFOB(fd,6)=1;
				WFIFOSET(fd,7);
				sd->status.inventory[i].equip=0;
				mmo_map_calc_status(fd,sd->status.inventory[i].nameid,0);
				mmo_map_calc_card(fd,sd->status.inventory[i].nameid,i,0);
		//		set_equip(fd,item_num,item_view);
				}
			}
			item_db = item_database(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
		/*	if equipment success R 00aa <index>.w <equip point>.w <type>.B
			if it�fs modified after equipping, then it should change
			mmo_map_set_look function is the modification you saw 

			(�����ڕύX��mmo_map_set_look�֐�) */
			WFIFOW(fd,0)=0xaa;
			//index
			WFIFOW(fd,2)=RFIFOW(fd,2);
			//equipment point
			WFIFOW(fd,4)=ep;
			//proprietary equipment decision
			WFIFOB(fd,6)=1;
			WFIFOSET(fd,7);
			sd->status.inventory[RFIFOW(fd,2)-2].equip=ep;
			//item number sd->status.inventory[RFIFOW(fd,2)-2].nameid;
			//the argument changes(3?) sd->account_id(applied ID),
			//equipment,sd->status.class(type)
			//type00 is reguarding equipment details (when changing jobs), 02 weapon,
			//03 head(lower), 04 head(top), 05 head(mid), 08 shield
			len=mmo_map_set_look(fd,WFIFOP(fd,0),sd->account_id,item_num,
			item_view);
			if(len>0) mmo_map_sendall(fd,WFIFOP(fd,0),len,0);
			mmo_map_calc_status(fd,sd->status.inventory[RFIFOW(fd,2)-2].nameid,1);
			set_equip(fd,item_num,item_view);//item is set
			mmo_map_calc_card(fd,0,0,1);
			  }
			  //else end
		//printf("%d %x %d %d %d %d %d %d\n",item_num,ep,item_view,sd->status.weapon,sd->status.head_bottom,sd->status.head_top,sd->status.head_mid,sd->status.sheild);
		break;
	  //equipment cancellation
    case 0xab:
	item_db = item_database(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
	item_num = itemdb_stype(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
      if(sd->status.inventory[RFIFOW(fd,2)-2].equip){
	WFIFOW(fd,0)=0xac;
	WFIFOW(fd,2)=RFIFOW(fd,2);
	WFIFOW(fd,4)=sd->status.inventory[RFIFOW(fd,2)-2].equip;
	WFIFOW(fd,6)=1;
	WFIFOSET(fd,7);
	sd->status.inventory[RFIFOW(fd,2)-2].equip=0;
      }
	  else {
	WFIFOW(fd,0)=0xac;
	WFIFOW(fd,2)=RFIFOW(fd,2);
	WFIFOW(fd,6)=0;
	WFIFOSET(fd,7);
      }
	len=mmo_map_set_look(fd,WFIFOP(fd,0),sd->account_id,item_num,0);
	if(len>0) mmo_map_sendall(fd,WFIFOP(fd,0),len,0);
	mmo_map_calc_status(fd,sd->status.inventory[RFIFOW(fd,2)-2].nameid,0);
	mmo_map_calc_card(fd,sd->status.inventory[RFIFOW(fd,2)-2].nameid,RFIFOW(fd,2)-2,0);
	set_equip(fd,item_num,0);
	 //printf("%d %d %d %d %d %d %d\n",item_num,item_view,sd->status.weapon,sd->status.head_bottom,sd->status.head_top,sd->status.head_mid,sd->status.sheild);
      break;
//item use
case 0xa7:
#include "item.c"
	break;
case 0xe4://transaction request sent

if(sd->status.skill[0].lv >= 1) {
ep=0;
    for(i=5;i < users_global+5;i++){
        if(session[i] && (target_sd=session[i]->session_data)){
            if(target_sd->account_id == RFIFOL(fd,2)){
            ep=1;
            if(target_sd->status.skill[0].lv >= 1) {
                    if((target_sd->status.trade_partner !=0) || (sd->status.trade_partner !=0)){
                    WFIFOW(fd,0) = 0xe7;
                    //type=0 too far | type=1 character does not exist
                    //type=2 person is in another trade
                    //type=3 allowed the trade | type=4 trade canceled
                    WFIFOB(fd,2) = 2;
                    WFIFOSET(fd,3);
                    }
                    else {

                        if(sd->mapno != target_sd->mapno) {
                        WFIFOW(fd,0) = 0xe7;
                        //type=0 too far | type=1 character does not exist
                        //type=2 person is in another trade
                        //type=3 allowed the trade | type=4 trade canceled
                        WFIFOB(fd,2) = 0;
                        WFIFOSET(fd,3);
                        }
                         else if((sd->x - target_sd->x <= -5 || sd->x - target_sd->x >= 5) || (sd->y - target_sd->y <= -5 || sd->y - target_sd->y >= 5)) {
                        WFIFOW(fd,0) = 0xe7;
                        //type=0 too far | type=1 character does not exist
                        //type=2 person is in another trade
                        //type=3 allowed the trade | type=4 trade canceled
                        WFIFOB(fd,2) = 0;
                        WFIFOSET(fd,3);
                        }
                        else {
                        // Trade Code
                        target_sd->status.trade_partner = sd->account_id;
                        sd->status.trade_partner = target_sd->account_id;
                        //
                        WFIFOW(i,0) = 0xe5; // Send  'Requests a Deal' to Client
                        strcpy(WFIFOP(i,2),sd->status.name);
                        WFIFOSET(i,26);
                        }
                      }//else  by if(target_sd->status.trade_partner !=0)
            } // if target player is lower base_skill lvl 1
            else {
            WFIFOW(fd,0) = 0xe7;
            //type=0 too far | type=1 character does not exist
            //type=2 person is in another trade
            //type=3 allowed the trade | type=4 trade canceled
            WFIFOB(fd,2) = 4;
            WFIFOSET(fd,3);
            }

            } // if(target_sd->account_id == RFIFOL(fd,2))
        } // if(session[i] && (target_sd=session[i]->session_data))
    } //for(i=5;i < users_global+5;i++)

if(ep==0) {
WFIFOW(fd,0) = 0xe7;
//type=0 too far | type=1 character does not exist
//type=2 person is in another trade
//type=3 allowed the trade | type=4 trade canceled
WFIFOB(fd,2) = 1;
WFIFOSET(fd,3);
}
ep=0;

}// if target player is >= lvl 1
else {
WFIFOW(fd,0) = 0x110;
WFIFOW(fd,2) = 1;
WFIFOW(fd,4) = 0;
WFIFOW(fd,6) = 0;
WFIFOW(fd,8) = 0;
WFIFOB(fd,10) =0;
WFIFOSET(fd,10);
}

break;

case 0xe6:// ClientsendDealAccept
    for(i=5;i < users_global+5;i++){
    if(session[i] && (target_sd=session[i]->session_data)){
        if(target_sd->account_id == sd->status.trade_partner){

            //type=0 too far | type=1 character does not exist
            //type=2 person is in another trade
            //type=3 allowed the trade | type=4 trade canceled

            if(RFIFOB(fd,2)==3){ // Ok
                WFIFOW(i,0) = 0xe7; // Engaged Deal
                WFIFOB(i,2) = 3; // type
                WFIFOSET(i,3);
                WFIFOW(fd,0) = 0xe7; // Engaged Deal
                WFIFOB(fd,2) = 3; // type
                WFIFOSET(fd,3);


            } else if(RFIFOB(fd,2)==4){ // Cancel
                WFIFOW(i,0) = 0xe7; // Engaged Deal
                WFIFOB(i,2) = 4; // type
                WFIFOSET(i,3);
                WFIFOW(fd,0) = 0xe7; // Engaged Deal
                WFIFOB(fd,2) = 4; // type
                WFIFOSET(fd,3);
                sd->status.deal_locked =0;
                sd->status.trade_partner=0;
                target_sd->status.deal_locked=0;
                target_sd->status.trade_partner=0;
            }
        }
    }
}
break;

/*
S 00e8 <index>.w <amount>.l
	Item addition. index=0 is zeny addition. zeny only trade is before 00eb (?)(���Kclient�ł�zeny��00eb�̒��O�̂�)
R 00e9 <amount>.l <type ID>.w <identify flag>.B <attribute?>.B <refine>.B <card>.4w
	item addition from others
R 00ea <index>.w <fail>.B
	fail=00 item addition success
	fail=01 addition falure. return weight
*/
case 0xe8: // ClientsendDealAddItem
	//argument pulls item information from <index> and sends it
	//decreases the item added, to cancel afterwards, you must use backup
ep=0;

    for(i=5;i < users_global+5;i++){
        if(session[i] && (target_sd=session[i]->session_data)){

            if( (target_sd->account_id == sd->status.trade_partner) && (sd->status.deal_locked < 1) ){

                if(RFIFOW(fd,2)<2 || RFIFOW(fd,2)>=MAX_INVENTORY+2) {
                    if( (RFIFOW(fd,2) == 0) && (RFIFOL(fd,4) > 0) && (RFIFOL(fd,4)<= sd->status.zeny) ) {
                    //Part A1
                    sd->status.deal_zeny=RFIFOL(fd,4);

                    //Part B2
                    WFIFOW(i,0) = 0xe9; // Send partner 'Added item' to Client (if item id==0,item=zeny)
                    WFIFOL(i,2) = RFIFOL(fd,4);//amount
                    WFIFOW(i,6) = RFIFOW(fd,2); // type id
                    WFIFOB(i,8) = 0; //identify flag
                    WFIFOB(i,10) = 0; // attribute
                    WFIFOB(i,12) = 0; //refine
                    WFIFOW(i,14) = 0; //card (4w)
                    WFIFOW(i,16) = 0; //card (4w)
                    WFIFOW(i,18) = 0; //card (4w)
                    WFIFOW(i,20) = 0; //card (4w)
                    WFIFOSET(i,packet_len_table[0xe9]);
                    }
                }
            else if( (RFIFOL(fd,4) <= sd->status.inventory[RFIFOW(fd,2)-2].amount) && (RFIFOL(fd,4) > 0) ) {

                //Part A1
                    ep=0;
                     for(trade_i=0; trade_i<10;trade_i++) {
                          if(sd->status.deal_inventory[trade_i].amount == 0) { ep=1; break;    }
                    }

                if(ep==1) {

                if(target_sd->weight + itemdb_weight(sd->status.inventory[RFIFOW(fd,2)-2].nameid)*sd->status.inventory[RFIFOW(fd,2)-2].amount > target_sd->max_weight) {
                WFIFOW(fd,0) = 0xea; // Send you've 'Added item' to Client (if item id==0,item=zeny)
                WFIFOW(fd,2) = RFIFOW(fd,2); //RFIFOW(fd,2) index - RFIFOL(fd,4) amount
                WFIFOB(fd,4) = 1; //fail=00    success to add item / fail=01 fail to add item-the player was over weighted.
                WFIFOSET(fd,5);
                ep=0;
                }

                 else {
                 sd->status.deal_inventory[trade_i] = sd->status.inventory[RFIFOW(fd,2)-2];
                 sd->status.deal_inventory[trade_i].amount=RFIFOL(fd,4);
                 sd->status.deal_item[trade_i] = RFIFOW(fd,2);

                 //Part B1
                WFIFOW(fd,0) = 0xea; // Send you've 'Added item' to Client (if item id==0,item=zeny)
                WFIFOW(fd,2) = RFIFOW(fd,2); //RFIFOW(fd,2) index - RFIFOL(fd,4) amount
                WFIFOB(fd,4) = 0; //fail=00    success to add item / fail=01 fail to add item-the player was over weighted.
                WFIFOSET(fd,5);
                //Part B2
                //item_db = item_database(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
                //item_num = itemdb_stype(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
                WFIFOW(i,0) = 0xe9; // Send partner 'Added item' to Client (if item id==0,item=zeny)
                WFIFOL(i,2) = RFIFOL(fd,4);//amount
                WFIFOW(i,6) = sd->status.inventory[RFIFOW(fd,2)-2].nameid; // type id
                WFIFOB(i,8) = sd->status.inventory[RFIFOW(fd,2)-2].identify; //identify flag
                WFIFOB(i,10) = sd->status.inventory[RFIFOW(fd,2)-2].attribute; // attribute
                WFIFOB(i,12) = sd->status.inventory[RFIFOW(fd,2)-2].refine; //refine
                WFIFOW(i,14) = sd->status.inventory[RFIFOW(fd,2)-2].card[0]; //card (4w)
                WFIFOW(i,16) = sd->status.inventory[RFIFOW(fd,2)-2].card[1]; //card (4w)
                   WFIFOW(i,18) = sd->status.inventory[RFIFOW(fd,2)-2].card[2]; //card (4w)
                  WFIFOW(i,20) = sd->status.inventory[RFIFOW(fd,2)-2].card[3]; //card (4w)
                WFIFOSET(i,packet_len_table[0xe9]);
                ep=0;
                }
            }
        }
    }
     }
     }
break;

case 0xeb: // ClientsendDealOK
    for(i=5;i < users_global+5;i++){
    if(session[i] && (target_sd=session[i]->session_data)){
    if(target_sd->account_id == sd->status.trade_partner){
    sd->status.deal_locked =1;
    WFIFOW(fd,0) = 0xea; // Send you've 'Added item' to Client (if item id==0,item=zeny)
    WFIFOW(fd,2) = 0; //RFIFOW(fd,2) index - RFIFOL(fd,4) amount
    WFIFOB(fd,4) = 0; //fail=00    success to add item / fail=01 fail to add item-the player was over weighted.
    WFIFOSET(fd,5);
    WFIFOW(fd,0) = 0xec; // finalized the deal
    WFIFOL(fd,2) = 0; // 0=you 1=the other person
    WFIFOSET(fd,3);
    WFIFOW(i,0) = 0xec; // finalized the deal
    WFIFOL(i,2) = 1; // 0=you 1=the other person
    WFIFOSET(i,3);

    }
    }
    }
break;

case 0xed: // ClientsendCurrentDealCancel
    for(i=5;i < users_global+5;i++){
        if(session[i] && (target_sd=session[i]->session_data)){
        if(target_sd->account_id == sd->status.trade_partner){


   //give items back (only virtual)

    for(trade_i=0; trade_i<10;trade_i++) {
     if(sd->status.deal_inventory[trade_i].amount != 0) {
     
    WFIFOW(fd,0)=0xa0;
    WFIFOW(fd,2)=sd->status.deal_item[trade_i];
    WFIFOW(fd,4)=sd->status.deal_inventory[trade_i].amount;
    WFIFOW(fd,6)=sd->status.deal_inventory[trade_i].nameid;
    WFIFOB(fd,8)=sd->status.deal_inventory[trade_i].identify;
    WFIFOB(fd,9)=sd->status.deal_inventory[trade_i].attribute;
    WFIFOB(fd,10)=sd->status.deal_inventory[trade_i].refine;
    WFIFOW(fd,11)=sd->status.deal_inventory[trade_i].card[0];
    WFIFOW(fd,13)=sd->status.deal_inventory[trade_i].card[1];
    WFIFOW(fd,15)=sd->status.deal_inventory[trade_i].card[2];
    WFIFOW(fd,17)=sd->status.deal_inventory[trade_i].card[3];
    WFIFOW(fd,19)=itemdb_equip_point(sd->status.deal_inventory[trade_i].nameid,sd);
    WFIFOB(fd,21)=itemdb_type(sd->status.deal_inventory[trade_i].nameid);
    WFIFOB(fd,22)=0;
    WFIFOSET(fd,23);
      
     sd->status.deal_inventory[trade_i].amount=0;
     sd->status.deal_item[trade_i]=0;
     }    }

    for(trade_i=0; trade_i<10;trade_i++) {
    if(target_sd->status.deal_inventory[trade_i].amount != 0) {

    WFIFOW(i,0)=0xa0;
    WFIFOW(i,2)=target_sd->status.deal_item[trade_i];
    WFIFOW(i,4)=target_sd->status.deal_inventory[trade_i].amount;
    WFIFOW(i,6)=target_sd->status.deal_inventory[trade_i].nameid;
    WFIFOB(i,8)=target_sd->status.deal_inventory[trade_i].identify;
    WFIFOB(i,9)=target_sd->status.deal_inventory[trade_i].attribute;
    WFIFOB(i,10)=target_sd->status.deal_inventory[trade_i].refine;
    WFIFOW(i,11)=target_sd->status.deal_inventory[trade_i].card[0];
    WFIFOW(i,13)=target_sd->status.deal_inventory[trade_i].card[1];
    WFIFOW(i,15)=target_sd->status.deal_inventory[trade_i].card[2];
    WFIFOW(i,17)=target_sd->status.deal_inventory[trade_i].card[3];
    WFIFOW(i,19)=itemdb_equip_point(target_sd->status.deal_inventory[trade_i].nameid,sd);
    WFIFOB(i,21)=itemdb_type(target_sd->status.deal_inventory[trade_i].nameid);
    WFIFOB(i,22)=0;
    WFIFOSET(i,23);

     target_sd->status.deal_inventory[trade_i].amount=0;
     target_sd->status.deal_item[trade_i]=0;
     }    }


    if(sd->status.deal_zeny) {
    len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_ZENY);
    if(len>0) WFIFOSET(fd,len);;
    sd->status.deal_zeny=0;
    }

    if(target_sd->status.deal_zeny) {
    len=mmo_map_set_param(i,WFIFOP(i,0),SP_ZENY);
    if(len>0) WFIFOSET(i,len);
    target_sd->status.deal_zeny=0;
    }


    //reset vars
    sd->status.deal_locked =0;
    sd->status.trade_partner=0;
    target_sd->status.deal_locked=0;
    target_sd->status.trade_partner=0;
    //send 'cancelled'
    WFIFOW(i,0) = 0xee; // Deal Cancelled
    WFIFOSET(i,2);

    WFIFOW(fd,0) = 0xee; // Deal Cancelled
    WFIFOSET(fd,2);
    }
    }
    }
break;

case 0xef: //ClientsendDealTrade

for(i=5;i < users_global+5;i++){
    if(session[i] && (target_sd=session[i]->session_data)){
    if(target_sd->account_id == sd->status.trade_partner) { // trade partner?
    if( (sd->status.deal_locked >=1) && (target_sd->status.deal_locked >=1) ){ // both have pressed 'ok'
            if(sd->status.deal_locked < 2) {sd->status.deal_locked=2;} // set locked to 2
    if(target_sd->status.deal_locked==2) { // the other one pressed 'trade' too


                         for(trade_i=0; trade_i<10;trade_i++) {
                         if(sd->status.deal_inventory[trade_i].amount != 0) {

                         
mmo_map_item_lost_silent(fd,WFIFOP(fd,0),sd->status.deal_item[trade_i],sd->status.deal_inventory[trade_i].amount);


                         len=mmo_map_item_get(i,WFIFOP(i,0),&sd->status.deal_inventory[trade_i]);
                         if(len>0) WFIFOSET(i,len);
                         sd->status.deal_inventory[trade_i].amount=0;
                         sd->status.deal_item[trade_i]=0;

                         }    }


                         for(trade_i=0; trade_i<10;trade_i++) {
                         if(target_sd->status.deal_inventory[trade_i].amount != 0) {


                         
mmo_map_item_lost_silent(i,WFIFOP(i,0),target_sd->status.deal_item[trade_i],target_sd->status.deal_inventory[trade_i].amount);


                         len=mmo_map_item_get(fd,WFIFOP(fd,0),&target_sd->status.deal_inventory[trade_i]);
                         if(len>0) WFIFOSET(fd,len);
                         target_sd->status.deal_inventory[trade_i].amount=0;
                         target_sd->status.deal_item[trade_i]=0;
                         }    }

                    //give zeny
                    if(sd->status.deal_zeny) {

                    sd->status.zeny = sd->status.zeny - sd->status.deal_zeny;
                    len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_ZENY);
                    if(len>0) WFIFOSET(fd,len);

                    target_sd->status.zeny = target_sd->status.zeny + sd->status.deal_zeny;
                    len=mmo_map_set_param(i,WFIFOP(i,0),SP_ZENY);
                    if(len>0) WFIFOSET(i,len);

                    sd->status.deal_zeny=0;
                    }


                    if(target_sd->status.deal_zeny) {

                    target_sd->status.zeny = target_sd->status.zeny - target_sd->status.deal_zeny;
                    len=mmo_map_set_param(i,WFIFOP(i,0),SP_ZENY);
                    if(len>0) WFIFOSET(i,len);

                    sd->status.zeny = sd->status.zeny + target_sd->status.deal_zeny;
                    len=mmo_map_set_param(fd,WFIFOP(fd,0),SP_ZENY);
                    if(len>0) WFIFOSET(fd,len);

                    target_sd->status.deal_zeny=0;
                    }

                    //reset vars
                    sd->status.deal_locked =0;
                    sd->status.trade_partner=0;
                    target_sd->status.deal_locked=0;
                    target_sd->status.trade_partner=0;

                    //send 'complete';
                    WFIFOW(i,0) = 0xf0; // Deal Complete
                    WFIFOL(i,2) = 0; // 1=failed
                    WFIFOSET(i,3);
                    WFIFOW(fd,0) = 0xf0; // Deal Complete
                    WFIFOL(fd,2) = 0; // 1=failed
                    WFIFOSET(fd,3);


    }
    }
    }
    }
}

break;



/*
S 0149 <ID>.l <type>.B
	Manner point is given to the ID. type=00 plus type=01 minus
R 014a <fail>.l
	Result of giving manner point. fail=0 success fail=1 failure
R 014b <type>.B <nick>.24B
	manner point received. type=00 plus type=01 minus
*/
case 0x149://manner point is recieved
	WFIFOW(fd,0) = 0x14a;//success decision: first time restriction is not given(?)(����Ɉ�x�����^�����Ȃ��Ȃǂ̐����̂���)�@it later becomes required
	WFIFOB(fd,2) = 0;//should restrict the degree given (?)(���͉��x�ł��^������@�Ƃ������Ƃɂ��Ă����A������͐������ׂ�)
	WFIFOSET(fd,3);

	WFIFOW(fd,0) = 0x14b;//packet receives manner point
	if(RFIFOB(fd,6)==0){//if you receive plus
		WFIFOB(fd,2)=0;//plus
		sd->status.manner++;
	}else if(RFIFOB(fd,6)==1){//you receive negative
		WFIFOB(fd,2)=1;//minus
		sd->status.manner--;
	}
	WFIFOSET(fd,packet_len_table[0x14b]);

	WFIFOW(fd,0) = 0x14a;
	WFIFOL(fd,2) = 0;//success(failure processing not implemented yet)
	WFIFOSET(fd,packet_len_table[0x14a]);
	break;

/*
S 00f9 <party name>.24B
	party creation
R 00fa <fail>.B
	fail=00 creation success
R 00fb <len>.w <party name>.24B {<ID>.l <nick>.24B <map name>.16B <leader>.B <offline>.B}.46B*
	collecting and sending party information
S 00fc <ID>.l
	your created party
R 00fd <nick>.24B <fail>.B
	fail=00 player already in party
	fail=01 denied by player
	fail=02 success
R 00fe <ID>.l <party name>.24B
	invited to party
S 00ff <ID>.l <fail>.l
	answer while being invited. fail=1 answer ok?
R 0100
	something related to party?
S 0101 <exp>.w <item?>.w
	modifying party settings
R 0102 <exp>.w <item?>.w
	party�fs present setting? exp=2 exp share  failure?
R 0104 <ID>.l ?.l <X>.w <Y>.w <offline>.B <party name>.24B <nick>.24B <map name>.16B
	renewal of party player information 
R 0105 <ID>.l <nick>.24B <fail>.B
	player name secedes party
R 0106 <ID>,l <HP>.w <MaxHP>.w
	Renew party member�fs HP
R 0107 <ID>.l <X>.w <Y>.w
	Renewal party member�fs position
S 0108 <len>.w <message>.?B
	party chat
R 0109 <len>.w <ID>.l <message>.?B
	accepting party chat
*/
case 0xf9://party creation
		ret = create_party(fd,RFIFOP(fd,2));
		if(ret==1){//creation success value
			WFIFOW(fd,0) = 0x00fa;
			WFIFOB(fd,2) = 0;
			WFIFOSET(fd,3);
		}else{//failure
					strcpy(moji,"Failed to create party.\n");
					WFIFOW(fd,0) = 0x8e;
					WFIFOW(fd,2) = 4 + strlen(moji);
					memcpy(WFIFOP(fd,4),moji,strlen(moji));
					WFIFOSET(fd,4 + strlen(moji));
		}
		break;
case 0xfc://your party
		break;
case 0xff://while being invited answer

		break;
case 0x101://changing party settings
		break;
case 0x108://party chat
	//for statement repeats so everyone in party receives party messages
	WFIFOW(fd,0) = 0x109;
	WFIFOW(fd,2) = RFIFOW(fd,2)+4;
	WFIFOL(fd,4) = fd;
	strcpy(WFIFOP(fd,8),RFIFOP(fd,4));
	WFIFOSET(fd,RFIFOW(fd,2)+4);
		break;
case 0x11d://current location memo
	WFIFOW(fd,0) = 0x11e;
	WFIFOB(fd,2) = 0;//memo success(loose it when defeated?(?)���s���Ă���̂��ȁH)
	WFIFOSET(fd,3);
	break;
case 0x126://item inserted in cart
	//ADDED ON 04/07/2003 ------------------------------------------

	p=&sd->status;

	//printf("index=%d, amount=%d\n",RFIFOW(fd,2),RFIFOL(fd,3));
	WFIFOW(fd,0) = 0x124;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	WFIFOL(fd,4) = RFIFOL(fd,3)/256;
	WFIFOW(fd,8) = p->inventory[RFIFOW(fd,2)-2].nameid;
	WFIFOB(fd,10) = 1;
	WFIFOB(fd,11) = 0;
	WFIFOB(fd,12) = 0;
	WFIFOW(fd,13) = 0;
	WFIFOW(fd,15) = 0;
	WFIFOW(fd,17) = 0;
	WFIFOW(fd,19) = 0;
	WFIFOSET(fd,21);

	p->inventory[RFIFOW(fd,2)-2].amount -= RFIFOL(fd,3)/256;
	WFIFOW(fd,0) = 0xaf;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	WFIFOW(fd,4) = (int)(RFIFOL(fd,3)/256);
	WFIFOSET(fd,6);
	// --------------------------------------------------------------

	break;
case 0x127://item removed from cart

	// ADDED ON 04/07/2003 ---------------------------------
	printf("amount=%d\n",RFIFOL(fd,4));
	p=&sd->status;

	WFIFOW(fd,0) = 0x125;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	WFIFOL(fd,4) = RFIFOL(fd,4);
	WFIFOSET(fd,8);

	p->inventory[RFIFOW(fd,2)-2].amount += RFIFOL(fd,4);
	WFIFOW(fd,0) = 0xaf;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	WFIFOW(fd,4) = -1 * (int)RFIFOL(fd,4);
	WFIFOSET(fd,6);
	// ------------------------------------

	break;
case 0x128://remove item from cart during vending
	break;
case 0x12e://close vending
	break;
case 0x12f://open vending

	// ADDED ON 04/07/2003 -------------------------
	//printf("len=%d mes=%s index=%d amount=%d value=%d\n",RFIFOW(fd,2),RFIFOP(fd,4),RFIFOW(fd,84),RFIFOW(fd,86),RFIFOW(fd,88));
	// -------------------------------

	break;
case 0x130://vending item list required
	break;
case 0x134://vending purchase request
	break;
case 0x14d://guild creation information�H
	break;
case 0x14f://guild creation tab
	break;
case 0x151://emblem request
	break;
case 0x159://guild secession
	break;
case 0x15b://guild exile
	break;
case 0x165://guild creation
	break;
case 0x168://guild join request
	break;
case 0x16b://guild join request reply
	break;
case 0x16e://guild notification setting
	break;
case 0x170://alliance join request
	break;
case 0x172://alliance join request reply
	break;

case 0x17e://guild chat
	WFIFOW(fd,0) = 0x17f;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	strcpy(WFIFOP(fd,4),RFIFOP(fd,4));
	WFIFOSET(fd,RFIFOW(fd,2));
	break;
case 0x178://item apraisal
	WFIFOW(fd,0) = 0x179;
	WFIFOW(fd,2) = RFIFOW(fd,2);
	WFIFOB(fd,4) = 0;
	WFIFOSET(fd,5);
	sd->status.inventory[RFIFOW(fd,2)-2].identify = 1;
	break;
case 0x17a://when using card, use packet
	item_db = item_database(sd->status.inventory[RFIFOW(fd,2)-2].nameid);
	//printf("%x\n",item_db.loc);
	mmo_map_card_017b(fd,item_db.loc);
	break;
case 0x17c://when using card and equipment selection, end packet
	//printf("sour = %d\n",sd->status.inventory[RFIFOW(fd,2)-2].nameid);
	//printf("desc = %d\n",sd->status.inventory[RFIFOW(fd,4)-2].nameid);
	mmo_map_card_017d(fd,RFIFOW(fd,2),RFIFOW(fd,4));
	break;
case 0x1a1:// pet is reset to egg ADDED on 04/10/2003 --------------
	#include "01a1.c"
// ------------------------------------------------------------------
	break;
case 0x1a5:// pet name modification
	if(sd->status.pet.pet_name_flag == 0)
	{
		WFIFOW(fd,0) = 0x95;
		WFIFOL(fd,2) = sd->status.pet.pet_id_as_npc;
		memcpy(WFIFOP(fd,6),RFIFOP(fd,2),24);
		WFIFOSET(fd,30);
	}
	break;
case 0x1a7://show make of pets ADDED on 04/09/2003 ----------------
	#include "01a7.c"
// ------------------------------------------------------------------
	break;
case 0x1a9:// pet armor ADDED on 04/10/2003 ------------------------
	//printf("PET EMO=%d\n",RFIFOL(fd,2));
	WFIFOW(fd,0) = 0x1aa;
	WFIFOL(fd,2) = sd->status.pet.pet_id_as_npc;
	WFIFOL(fd,6) = RFIFOL(fd,2);
	WFIFOSET(fd,10);
// ------------------------------------------------------------------
	break;
default://default processing
	//printf("parse_map : %d %d %x\n",fd,RFIFOREST(fd),RFIFOW(fd,0));
    break;
    }
    RFIFOSKIP(fd,parsing_packet_len);
  }
  return 0;
}

int send_users_tochar(int tid,unsigned int tick,int id,int data)
{
  int users=0,i;
  struct map_session_data *sd;

  for(i=0;i<FD_SETSIZE;i++){
    if(session[i] && (sd=session[i]->session_data) && sd->state.auth)
      users++;
  }
  if(char_fd>0 && session[char_fd]){
    WFIFOW(char_fd,0)=0x2aff;
    WFIFOL(char_fd,2)=users;
    WFIFOSET(char_fd,6);
  }
  return 0;
}

int check_connect_char_server(int tid,unsigned int tick,int id,int data)
{
  if(char_fd<=0 || session[char_fd]==NULL){
    char_fd=make_connection(inet_addr(char_ip_str),char_port);
    session[char_fd]->func_parse=parse_tochar;
    WFIFOW(char_fd,0)=0x2af8;
    memcpy(WFIFOP(char_fd,2),userid,24);
    memcpy(WFIFOP(char_fd,26),passwd,24);
    WFIFOL(char_fd,50)=0;
    WFIFOL(char_fd,54)=map_ip;
    WFIFOL(char_fd,58)=map_port;
    WFIFOSET(char_fd,60);
  }
  return 0;
}

void mmo_map_allclose(void)
{
  int fd;
  struct map_session_data *sd;

  for(fd=0;fd<FD_SETSIZE;fd++){
    if(session[fd] && (sd=session[fd]->session_data) && sd->state.auth){
      if(char_fd>0 && session[char_fd]){
	memcpy(sd->status.last_point.map,sd->mapname,16);
	sd->status.last_point.x = sd->x;
	sd->status.last_point.y = sd->y;

	WFIFOW(char_fd,0)=0x2b01;
	WFIFOW(char_fd,2)=sizeof(sd->status)+12;
	WFIFOL(char_fd,4)=sd->account_id;
	WFIFOL(char_fd,8)=sd->char_id;
	memcpy(WFIFOP(char_fd,12),&sd->status,sizeof(sd->status));
	write(char_fd,WFIFOP(char_fd,0),WFIFOW(char_fd,2));
      }
    }
    close(fd);
    delete_session(fd);
  }
  if(char_fd>0){
    close(char_fd);
    delete_session(char_fd);
    char_fd=-1;
  }
  return;
}

int read_mapdata(void)
{
  int i;
 map_number=0;
printf("Loading maps wait a moment please....\n");

  // read&convert *.gat files
  for(i=0;map[i][0];i++){
    char fn[256];
    unsigned char *gat;
    int s;

    if(strstr(map[i],".gat")==NULL)
      continue;
    sprintf(fn,"data\\%s",map[i]);
    gat=grfio_read(fn);
    if(gat){
      int x,y,xs,ys;
      struct gat_1cell {float high[4]; int type;} *p;

      xs=map_data[i].xs=*(int*)(gat+6);
      ys=map_data[i].ys=*(int*)(gat+10);
      map_data[i].gat=malloc(s=map_data[i].xs*map_data[i].ys);
      map_data[i].npc_num=0;
      map_data[i].users=0;
      for(y=0;y<ys;y++){
	p=(struct gat_1cell*)(gat+y*xs*20+14);
	for(x=0;x<xs;x++){
	  if(p->type==0){
	    map_data[i].gat[x+y*xs]=(p->high[0]<0 || p->high[1]<0 || p->high[2]<0 || p->high[3]<0) ? 3 : 0;
	  } else {
	    map_data[i].gat[x+y*xs]=p->type;
	  }
	  p++;
	}
      }
      free(gat);
      map_data[i].bxs=(xs+BLOCK_SIZE-1)/BLOCK_SIZE;
      map_data[i].bys=(ys+BLOCK_SIZE-1)/BLOCK_SIZE;
      map_data[i].block=malloc(map_data[i].bxs*map_data[i].bys*sizeof(struct block_list));
      for(y=0;y<map_data[i].bys;y++){
	for(x=0;x<map_data[i].bxs;x++){
	  map_data[i].block[x+y*map_data[i].bxs].next=NULL;
	  map_data[i].block[x+y*map_data[i].bxs].prev=NULL;
	  map_data[i].block[x+y*map_data[i].bxs].type=BL_NUL;
	}
      }
      //printf("%s read done\n",fn);
	map_number++;
    }
  }
	printf("OK!! ->> %d MAPS got loaded into memory! Starting...\n",map_number);
  return 0;
}

void unload_mapdata(void)
{ int i,j;

  if (map_number!=0)
  {
    for (i=0;i<map_number;i++)
    {
	if (map_data[i].gat)   free((void *)map_data[i].gat);
	for (j=0;j<map_data[i].npc_num;j++)
	{
          if (map_data[i].npc[j]->block.subtype==SCRIPT)
	    if (map_data[i].npc[j]->u.script)
		free((void *)map_data[i].npc[j]->u.script);
	  if (map_data[i].npc[j]->block.subtype==SHOP)
	    if (map_data[i].npc[j]->u.shop_item)
		free((void *)map_data[i].npc[j]->u.shop_item);
/*	  if (map_data[i].npc[j]->block.subtype==MONS)
	  {
	    if (map_data[i].npc[j]->u.mons.timer)
		delete_timer(map_data[i].npc[j]->u.mons.timer,mons_walk);
	    if (map_data[i].npc[j]->u.mons.targettimer)
		delete_timer(map_data[i].npc[j]->u.mons.targettimer,test_monster);
	    if ((map_data[i].npc[j]->u.mons.attacktimer!=0) && (map_data[i].npc[j]->u.mons.attacktimer!=-1))
		delete_timer(map_data[i].npc[j]->u.mons.attacktimer,mmo_map_attack_continue);
	  } */
	  if (map_data[i].npc[j]) free((void *)map_data[i].npc[j]);
	}

	if (map_data[i].block) free((void *)map_data[i].block);
    }

  }
}

int do_init(int argc,char **argv)
{
  int i,map_num=0;
  FILE *irc = fopen("data/status.txt","w");
  
  printf("PvPGN YARE MOD V1.1.0 \n(c) Project YARE & PvPGN \nwww.project-yare.net\n\n");
 
  
  if(irc)
    fprintf(irc,"[status]\nserver=1\n");
  fclose(irc);

  if(1){
    char line[1024],w1[1024],w2[1024];
    //FILE *fp=fopen(argv[1],"r");
	FILE *fp=fopen("map-server-config.txt","r");
    if(fp==NULL){
      printf("file not found: map_server_config.txt\n");
      exit(1);
    }
    while(fgets(line,1020,fp)){
      i=sscanf(line,"%[^:]:%s",w1,w2);
      if(i!=2)
	continue;
      if(strcmp(w1,"userid")==0){//SySCall Test
	memcpy(userid,w2,24);
	memset(userid+strlen(userid),0,24-strlen(userid));
      } else if(strcmp(w1,"passwd")==0){
	memcpy(passwd,w2,24);
	memset(passwd+strlen(passwd),0,24-strlen(passwd));
      } else if(strcmp(w1,"char_ip")==0){
	memcpy(char_ip_str,w2,16);
      } else if(strcmp(w1,"char_port")==0){
	char_port=atoi(w2);
      } else if(strcmp(w1,"map_ip")==0){
	memcpy(map_ip_str,w2,16);
      } else if(strcmp(w1,"map_port")==0){
	map_port=atoi(w2);
      } else if(strcmp(w1,"map")==0){
	strncpy(map[map_num],w2,16);
	map_num++;
	map[map_num+1][0]=0;
      } else if(strcmp(w1,"npc")==0){
	npc_txt[npc_txt_num]=malloc(strlen(w2)+1);
	strcpy(npc_txt[npc_txt_num++],w2);
      } else if(strcmp(w1,"mapmove")==0){
	strcpy(mapmove_name,w2);
      }
    }
    fclose(fp);
  }
  map_ip=inet_addr(map_ip_str);

  default_func_parse=parse_map;

  term_func=mmo_map_allclose;

  grfio_init();//grf data reading
  itemdb_init();//item database initialization
  read_mapdata();//reading map data
  read_npcdata();//NPC data initialization
  mmo_party_do_init();//party initialization

  make_listen_port(map_port);
  
  i=add_timer(gettick()+10,check_connect_char_server,0,0);
  timer_data[i]->type=TIMER_INTERVAL;
  timer_data[i]->interval=10*1000;

  i=add_timer(gettick()+10,send_users_tochar,0,0);
  timer_data[i]->type=TIMER_INTERVAL;
  timer_data[i]->interval=5*1000;

  i=add_timer(gettick()+10,clear_flooritem,0,0);//clears floor items
  timer_data[i]->type=TIMER_INTERVAL;
  timer_data[i]->interval=1000;

 i=add_timer(gettick()+10,loop_monster_search,0,0);//monster search
 timer_data[i]->type=TIMER_INTERVAL;
 timer_data[i]->interval=500;

  i=add_timer(gettick()+10,heal_hp,0,0);//natural HP recovery
  timer_data[i]->type=TIMER_INTERVAL;
  timer_data[i]->interval=1000;

  i=add_timer(gettick()+20,heal_sp,0,0);//natural SP recovery
  timer_data[i]->type=TIMER_INTERVAL;
  timer_data[i]->interval=900;

  //i=add_timer(gettick()+300000,timer_backup,0,0);
  //timer_data[i]->type=TIMER_INTERVAL;
  //timer_data[i]->interval=30 * 60 * 1000;
printf("Map-Server is online and ready!.\n");
irc=fopen("data/status.txt","w");
  if(irc)
    fprintf(irc,"[status]\nserver=0\n");
  fclose(irc);
  return 0;
}

int do_exit()
{
 delete_timer(heal_sp_timer,heal_sp);
 delete_timer(heal_hp_timer,heal_hp);
 delete_timer(loop_monstersearch_timer,loop_monster_search);
 delete_timer(clear_flooritem_timer,clear_flooritem);
 delete_timer(send_users_tochar_timer,send_users_tochar);
 delete_timer(check_connect_timer,check_connect_char_server);
 delete_session(map_port_fd);
 unload_mapdata();
 mmo_party_exit();
 return 0;
}

int set_all_skilldata(int fd)
{
	FILE *fp;
	int skill_id,skill_range,skill_sp,skill_type_hit,skill_type_inf,skill_type_num,skill_type_nk;
	int skill_type_pl;
	int i,j;
	struct map_session_data *sd;
	char line[1024];
	sd=session[fd]->session_data;
	i = 0;
	j = 1;
	//removes ID from item_db.txt(25th)

	fp = fopen("config/skill_info.txt","r");
	if(fp)
	{
		while(fgets(line,1024,fp)){
			if(sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d",&skill_id,&skill_range,&skill_sp,&skill_type_hit,
				&skill_type_inf,&skill_type_num,&skill_type_pl,&skill_type_nk) != 8)
				continue;
			skill_db[j].id = skill_id;
			skill_db[j].sp = skill_sp;
			skill_db[j].range = skill_range;
			skill_db[j].type_hit = skill_type_hit;
			skill_db[j].type_inf = skill_type_inf;
			skill_db[j].type_num = skill_type_num;
			skill_db[j].type_pl = skill_type_pl;
			skill_db[j].type_nk = skill_type_nk;
			sd->status.skill[i].id = j;
			j++;
			i++;
		}
	}
	fclose(fp);
	return 0;
}

//skills are set per job
int set_job_skilldata(int fd)
{
	FILE *fp;
	int skill_id,skill_range,skill_sp,skill_type_hit,skill_type_inf,skill_type_num,skill_type_nk;
	int skill_type_pl,skill_type_job,skill_type_job2,skill_type_job3,skill_lv_max;
	char skill_require[128];
	int i,j;
	struct map_session_data *sd;
	char line[1024],*p;
	int c;
	sd=session[fd]->session_data;
	i = 0;
	j = 1;
	//removes ID from item_db.txt(25th)
	fp = fopen("config/skill_info2.txt","r");
	if(fp)
	{
		while(fgets(line,1024,fp)){
			if(sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%[^t]",&skill_id,&skill_range,&skill_sp,&skill_type_hit,
				&skill_type_inf,&skill_type_num,&skill_type_pl,&skill_type_nk
				,&skill_type_job,&skill_type_job2,&skill_type_job3,&skill_lv_max,skill_require) != 13)
				continue;
			skill_db[j].id = skill_id;
			skill_db[j].sp = skill_sp;
			skill_db[j].range = skill_range;
			skill_db[j].type_hit = skill_type_hit;
			skill_db[j].type_inf = skill_type_inf;
			skill_db[j].type_num = skill_type_num;
			skill_db[j].type_pl = skill_type_pl;
			skill_db[j].type_nk = skill_type_nk;
			skill_db[j].type_lv = skill_lv_max;
			if(skill_require != NULL)
				strcpy(skill_db[j].type_require,skill_require);
			//skill settings are adjusted to job by skill lvl
			//don�ft remember if novice
		//	printf("stj %d %d %d %d\n",sd->status.class,skill_type_job,skill_type_job2,skill_type_job3);
			if(sd->status.class == 0)
			{
				if(j == 1 && i ==0)
				sd->status.skill[i].id = j;
				else
					sd->status.skill[i].id = 0;
				j++;
				i++;
				continue;
			}

			if(sd->status.class == skill_type_job || sd->status.class == skill_type_job2 || sd->status.class == skill_type_job3 )
			{
				if(sd->status.class == skill_type_job && sd->status.class != skill_type_job2 && j >= 54)
					sd->status.skill[i].id = 0;
				else
					sd->status.skill[i].id = j;
			}
			else if(skill_type_job == 0 && skill_type_job2 == 0 && skill_type_job3 == 0)
				sd->status.skill[i].id = j;
			else
				sd->status.skill[i].id = 0;
			j++;
			i++;
		}
	}

	//skill adjustment

	for(j=1;j<=MAX_SKILL;j++){
		if(sd->status.skill[j-1].id != 0){
			c=0;
			for(p=skill_db[j].type_require,i=0;p && i<16;i++,p=strchr(p,',')){
				int r_skill_id,r_skill_lv;
				if(*p==',') p++;
				if(sscanf(p,"%d:%d",&r_skill_id,&r_skill_lv)!=2)
				  break;
				if(sd->status.skill[r_skill_id-1].id != 0 && sd->status.skill[r_skill_id-1].lv >= r_skill_lv)
					c++;
			//	printf("%d %d\n",r_skill_id,r_skill_lv);
					}
			if(c == i)
				sd->status.skill[j-1].id = j;
			else if(p == NULL)
				sd->status.skill[j-1].id = 0;
			else
				sd->status.skill[j-1].id = j;
		}
	}

	fclose(fp);
	return 0;
}

int mmo_map_card_017b(int fd,int item_loc)
{
	struct map_session_data *sd;
	struct item_db2 item_db;
	int i,j,c,count_slot=0;
	sd=session[fd]->session_data;
	WFIFOW(fd,0) = 0x17b;
	for(i=c=0;i<100;i++){
		//if matches item equipment number
		if((sd->status.inventory[i].nameid>=501 && sd->status.inventory[i].nameid<=1099)
			|| (sd->status.inventory[i].nameid >= 4001 && sd->status.inventory[i].nameid<=4148)
			|| (sd->status.inventory[i].nameid >= 7001 && sd->status.inventory[i].nameid<=10019)
			|| (sd->status.inventory[i].nameid == 0))
			continue;
		item_db = item_database(sd->status.inventory[i].nameid);
		if(item_db.loc == item_loc && item_db.slot != 0)
		{
			for(j=0;j<item_db.slot;j++){
				if(sd->status.inventory[i].card[j] != 0){
					count_slot++;
				}
			}
			if(count_slot != item_db.slot){
				WFIFOW(fd,4+c*2) = i+2;
				c++;
			}
		}
		count_slot=0;
	}
	WFIFOW(fd,2) = 4+c*2;
	WFIFOSET(fd,4+c*2);
	return 0;
}

int	mmo_map_card_017d(int fd,int id_card,int id_equipment){
	int i;
	struct map_session_data *sd;
	sd=session[fd]->session_data;
	for(i=0;i<5;i++)
	{
		if(sd->status.inventory[id_equipment-2].card[i] == 0){
		sd->status.inventory[id_equipment-2].card[i] = sd->status.inventory[id_card-2].nameid;
		break;
		}
	}
	WFIFOW(fd,0) = 0x17d;
	WFIFOW(fd,2) = id_equipment;
	WFIFOW(fd,4) = id_card;
	WFIFOB(fd,6) = 0;
	WFIFOSET(fd,7);
	sd->status.inventory[id_card-2].nameid = 0;
	return 0;
}


//type=1 equipment�@type=0 cancellation
int mmo_map_calc_card(int fd,int item_id,int inv_num,int type)
{
	int i,j,len;
	struct item_db2 item_db;
	struct item_db2 item_cd;
	struct map_session_data *sd;

	sd=session[fd]->session_data;

	if(type == 1){
		for(i=0;i<100;i++){
		  if(sd->status.inventory[i].nameid && sd->status.inventory[i].equip){
				 //equipment exists
				 item_db = item_database(sd->status.inventory[i].nameid);
				 //check if card is already in slot
				 if(item_db.slot != 0)
				 {
					for(j=0;j<item_db.slot;j++){
						if(sd->status.inventory[i].card[j] != 0){
						//card has stuck
					//			//printf("S_SKILL_ID %d\n",item_db.skill_id);
								item_cd = item_database(sd->status.inventory[i].card[j]);
								if(item_cd.skill_id == 0)
									mmo_map_calc_status(fd,sd->status.inventory[i].card[j],1);
								else{
									//mmo_map_set_0147(fd,item_cd.skill_id,1);
									sd->status.skill[item_cd.skill_id-1].id = item_cd.skill_id;
									sd->status.skill[item_cd.skill_id-1].lv = 1;
									len = mmo_map_all_skill(fd,WFIFOP(fd,0),0,1);
									if(len>0) WFIFOSET(fd,len);
								}
						}
					}
				}
			}
		}
		return 0;
	}
	//card cancellation
	else if(type == 0)
	{
		item_db = item_database(item_id);
		if(item_db.slot != 0){
			for(j=0;j<item_db.slot;j++){
				if(sd->status.inventory[inv_num].card[j] != 0){
					item_cd = item_database(sd->status.inventory[inv_num].card[j]);
					if(item_cd.skill_id == 0)
						mmo_map_calc_status(fd,sd->status.inventory[inv_num].card[j],0);
					else{
					//	mmo_map_set_0147(fd,item_cd.skill_id,0);
						sd->status.skill[item_cd.skill_id-1].id = 0;
						len = mmo_map_all_skill(fd,WFIFOP(fd,0),0,1);
						if(len>0) WFIFOSET(fd,len);
						}
					}
				}
			}
		return 0;
	}
	return 0;
}

//when using skills, this function is used(?)(�g�p���Ɉꎞ�I��skill�𗘗p���鎞�Ɏg���֐�)
//type=1 set type=0 reset
int mmo_map_set_0147(int fd,int skill_id,int type)
{
	struct map_session_data *sd;
	sd=session[fd]->session_data;

	if(type == 1)
	{
		//printf("SKILLID %d\n",skill_id);
		WFIFOW(fd,0) = 0x147;
		WFIFOW(fd,2) = skill_id;
		WFIFOW(fd,4) = skill_db[skill_id].type_inf;
		WFIFOW(fd,6) = 0;
		WFIFOW(fd,8) = 1;
		WFIFOW(fd,10) = skill_db[skill_id].sp;
		WFIFOW(fd,12) = skill_db[skill_id].range;
		memcpy(WFIFOP(fd,14),"",24);
		WFIFOB(fd,38) = 0;
		WFIFOSET(fd,39);
		return 0;
	}
	else if(type == 0)
	{
		WFIFOW(fd,0) = 0x147;
		WFIFOW(fd,2) = skill_id;
		WFIFOW(fd,4) = skill_db[skill_id].type_inf;
		WFIFOW(fd,6) = 0;
		WFIFOW(fd,8) = 0;
		WFIFOW(fd,10) = skill_db[skill_id].sp;
		WFIFOW(fd,12) = skill_db[skill_id].range;
		memcpy(WFIFOP(fd,14),"",24);
		WFIFOB(fd,38)=0;
		WFIFOSET(fd,39);
		return 0;
	}
	return 0;
}


void reload_config() {

}

void mmo_open_storage(int fd) {

int i;
struct map_session_data *sd;
sd=session[fd]->session_data;

    if(session[fd] && (sd=session[fd]->session_data)){
            sd->status.storage_status=1;
            WFIFOW(fd,0) = 0xa5;
            
            for(i=0;i<MAX_STORAGE;i++) {
                if(sd->status.storage[i].amount==0) {break;    }

             WFIFOW(fd,4+i*10) = i+1; //index
            WFIFOW(fd,4+i*10+2) = sd->status.storage[i].nameid; // id
            WFIFOB(fd,4+i*10+4) = itemdb_type(sd->status.storage[i].nameid); // item type
            WFIFOB(fd,4+i*10+5) = sd->status.storage[i].identify; // identify
            WFIFOW(fd,4+i*10+6) = sd->status.storage[i].amount; // amount
            WFIFOB(fd,4+i*10+8) = 0; // ?
            WFIFOB(fd,4+i*10+9) = 0; // ?    
            }
            
            WFIFOW(fd,2) = 4+i*10; // packet size of this packet
            WFIFOSET(fd,4+i*10);

            WFIFOW(fd,0) = 0xf2;
            WFIFOL(fd,2) = sd->status.storage_amount;  //items
            WFIFOL(fd,4) = MAX_STORAGE; //items max
            WFIFOSET(fd,6);
    }
}


