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
// map2.h - Map Server Include file

struct mmo_tmp_path { short x,y,dist,before,cost; char dir,flag;};

void mmo_open_storage(int fd);
void add_block(struct block_list *bl,int m,int x,int y);
void del_block(struct block_list *bl);
int search_free_object_id(void);
int delete_object(int id);
int set_pos(unsigned char *p,int x,int y);
int set_2pos(unsigned char *p,int x0,int y0,int x1,int y1);
int set_map(struct map_session_data *sd,char *mapname,int x,int y);
int wait_close(int tid,unsigned int tick,int id,int data);
int parse_tochar(int fd);
int mmo_map_sendblock(int m,int bx,int by,char *dat,int len,int srcfd,int wos);
int mmo_map_sendarea(int srcfd,char *dat,int len,int wos);
int mmo_map_sendchat(int srcfd,char *dat,int len,int wos);
int mmo_map_sendarea_mxy(int m,int x,int y,char *dat,int len);
int mmo_map_sendall(int srcfd,char *dat,int len,int wos);
int mmo_map_set_frameinitem(int fd,unsigned char *buf,struct flooritem_data *fitem);
int mmo_map_set_dropitem(int fd,unsigned char *buf,struct flooritem_data *fitem);
int mmo_map_set_npc(int fd,unsigned char *buf,unsigned long id, unsigned long class, int x, int y,int dir);
int mmo_map_set_npc007c(int fd,unsigned char *buf,unsigned long id, unsigned long class, int x, int y);
int mmo_map_set00b1(int fd,int type,int val);
int mmo_map_set0078(int fd,unsigned char *buf);
int mmo_map_set0079(int fd,unsigned char *buf);
int mmo_map_set007b(int fd,unsigned char *buf,unsigned long tick,int x0,int y0,int x1,int y1);
int mmo_map_send0095(int fd,unsigned long id);
void equip_check(int fd);
int mmo_map_set_look(int fd,unsigned char *buf,int id,int type,int val);
int calc_need_status_point(struct map_session_data *sd,int type);
int mmo_map_set_param(int fd,unsigned char *buf,int type);
int mmo_map_update_param(int fd,int type,int val);
int mmo_map_calc_sigma(int k,double val);
int mmo_map_calc_status(int fd,int item_num,int equip);
void mmo_map_skill_lv_up(int fd,int skill_id);
void mmo_map_job_lv_up(int fd,int val);
void mmo_map_level_up(int fd,int val);
int mmo_map_item_get(int fd,unsigned char* buf, struct item* item);
int mmo_map_item_lost(int fd,unsigned char* buf, short index, short amount);
int mmo_map_item_lost_silent(int fd,unsigned char* buf, short index, short amount);
int mmo_map_set00d7(int fd,unsigned char *buf);
int mmo_map_getblockchar(int fd,int m,int bx,int by);
int mmo_map_clrblockchar(int fd,int m,int bx,int by);
int mmo_map_getareachar(int fd);
int mmo_map_sendwis(int fd);
int mmo_map_all_nonequip(int fd,unsigned char *buf);
int mmo_map_all_equip(int fd,unsigned char *buf);
int mmo_map_set00bd(int fd,unsigned char *buf);
int mmo_send_selfdata(int fd,struct map_session_data *sd);
int mmo_map_jobchange(int fd,int class);
int mmo_map_make_flooritem(struct item *item_data,int amount,int m,int x,int y);
int mmo_map_dropitem(int fd,int index,int amount);
int mmo_map_takeitem(int fd,int item_id);
int clear_flooritem(int tid,unsigned int tick,int id,int data);
int calc_cost(struct mmo_tmp_path *p,int x1,int y1);
int add_path(struct mmo_tmp_path *path,int *wp,int x,int y,int dist,int dir,int before,int x1,int y1);
int can_move(struct mmo_map_data *m,int x0,int y0,int x1,int y1);
int search_path(struct map_session_data *sd,int mapno,int x0,int y0,int x1,int y1,int easy_only);
int calc_next_walk_step(int fd);
int mmo_map_changemap(int id,struct map_session_data *sd,char *mapname,int x,int y,int type);
int walk_char(int tid,unsigned int tick,int id,int data);
int mmo_map_status_up(int fd,int type);
void write_log(char *mes);
int mmo_map_createchat( struct mmo_chat* chat);
int mmo_map_delchat( struct mmo_chat* chat );
struct mmo_chat* mmo_map_getchat(unsigned long id );
int mmo_map_addchat( int fd,struct mmo_chat* chat,char *pass);
int mmo_map_leavechat(int fd, struct mmo_chat* chat,char* leavecharname);
int mmo_map_changeowner(int fd,struct mmo_chat* chat,char *nextownername);
int mmo_map_search_monster(int m,int id);
int mmo_map_delay_item_drop(int tid,unsigned int tick,int id,int data);
int mmo_map_all_skill(int fd,unsigned char *buf,int val,int type);
int mmo_map_item_drop(int m,int n);
//PVP DISABLED next line and next 4 line
int mmo_map_pvp_skill_attack(int fd,int target_id,int skill_num,int skill_lvl,unsigned long tick,int skill_x,int skill_y);
int mmo_map_skill_attack(int fd,int target_id,int skill_num,int skill_lvl,unsigned long tick,int skill_x,int skill_y);
int mmo_map_set_skilleffectarea(int fd,unsigned char *buf,unsigned long dst_id,unsigned long src_id,unsigned long type, int x, int y);
int mmo_map_pvp_attack(int fd,int target_id,unsigned long tick);
int mmo_map_level_mons(int fd,int m,int n);
int mmo_map_set010a(int fd,int item_id);
int mmo_map_set010b(int fd,int exp);
int mmo_map_mvp_do(int fd,int m,int n);
int mmo_map_once_attack(int fd,int target_id,unsigned long tick);
int mmo_mons_attack_no(int m,int n,int target_id,unsigned long tick);
int mmo_mons_once_attack_start(int m,int n,int fd,unsigned long tick);
int mmo_mons_once_attack(int m,int n,int target_id,unsigned long tick);
int mmo_mons_attack_continue(int tid,unsigned int tick,int id,int data);
int mmo_map_attack_continue(int tid,unsigned int tick,int id,int data);
int set_lvup_table();
int monster_search(int fd,unsigned int tick,int id,int data);
int loop_monster_search(int fd,unsigned int tick,int id,int data);
int test_monster(int fd,unsigned int tick,int id,int data);
void set_equip(int fd,int item_num,int item_view);
int parse_map(int fd);
int send_users_tochar(int tid,unsigned int tick,int id,int data);
int check_connect_char_server(int tid,unsigned int tick,int id,int data);
void mmo_map_allclose(void);
int read_mapdata(void);
int do_init(int argc,char **argv);
int set_all_skilldata(int fd);
int heal_hp(int tid,unsigned int tick,int id,int data);
int heal_sp(int tid,unsigned int tick,int id,int data);
int set_job_skilldata(int fd);
int mmo_map_card_017b(int fd,int item_loc);
int	mmo_map_card_017d(int fd,int id_card,int id_equipment);
int mmo_map_calc_card(int fd,int item_id,int inv_num,int type);
int mmo_map_set_0147(int fd,int skill_id,int type);
void reload_config();



