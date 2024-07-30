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
// itemdb.h - Include file for item database usage 

int itemdb_type(int nameid);
int itemdb_sellvalue(int nameid);
int itemdb_weight(int nameid);
int itemdb_isequip(int named);
int itemdb_stype(int nameid);
int itemdb_equip_point(int named,struct map_session_data *sd);
int itemdb_init(void);
int itemdb_view_point(int nameid);
struct item_db2 item_database(int item_id);
int itemdb_hands(int nameid);
int itemdb_can_equipt_weapon(int nameid,short class);
