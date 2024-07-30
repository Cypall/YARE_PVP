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
// npc.h - Include file for npcs

// for map2.c
int npc_click(int fd,int npc_id);
int npc_menu_select(int fd,int npc_id,int sel);
int npc_next_click(int fd,int npc_id);
int npc_amount_input(int fd,int npc_id,int val);
int npc_close_click(int fd,int npc_id);
int npc_buysell_selected(int fd,int npc_id,int sell);
int npc_buy_selected(int fd,void *list,int num);
int npc_sell_selected(int fd,void *list,int num);
int read_npcdata(void);
int mons_walk(int tid,unsigned int tick,int m,int n);
void respawn_mons(int m,int n);
void respawn_mons2(int m,int n);
// ADDED on 04/09/2003
int* return_npc_current_id(void);


// for script.c
int mmo_map_npc_say(int fd, unsigned char* buf,unsigned long id, char *string);
int mmo_map_npc_next(int fd, unsigned char* buf, unsigned long id);
int mmo_map_npc_close(int fd, unsigned char* buf, unsigned long id);
int mmo_map_npc_select(int fd, unsigned char* buf, unsigned long id, char *string);
int mmo_map_npc_amount_request(int fd, unsigned char* buf, unsigned long id);
int spawn_monster(int class,int x,int y,int m,int fd);
