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
// skill_db.c - Skill database

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmo.h"
#include "skill_db.h"
struct skill_db ret_skill;

struct skill_db skill_database(int i_skill_id)
{
	FILE *fp;
	int skill_id,skill_range,skill_sp,skill_type_hit,skill_type_inf,skill_type_num,skill_type_nk;
	int skill_type_pl;
	int fcount,i;
	char line[1024];
	fcount = 0;
	i = 0;
	//item_db.txtのＩＤから見た目番号（２５番目）を取り出す
	
	fp = fopen("config/skill_info.txt","r");
	if(fp)
	{
		while(fgets(line,1024,fp)){
			if(sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d",&skill_id,&skill_range,&skill_sp,&skill_type_hit,
				&skill_type_inf,&skill_type_num,&skill_type_pl,&skill_type_nk) != 8)
				continue;
			if(skill_id == i_skill_id)
			{
			ret_skill.id = skill_id;
			ret_skill.sp = skill_sp;
			ret_skill.range = skill_range;
			ret_skill.type_hit = skill_type_hit;
			ret_skill.type_inf = skill_type_inf;
			ret_skill.type_num = skill_type_num;
			ret_skill.type_pl = skill_type_pl;
			ret_skill.type_nk = skill_type_nk;
			fclose(fp);
			return ret_skill;
			}
		}
	}
	ret_skill.id = i_skill_id;
	ret_skill.sp = 10;
	ret_skill.range = 18;
	ret_skill.type_hit = 8;
	ret_skill.type_inf = 1;
	ret_skill.type_num = 5;
	ret_skill.type_pl = 0;
	ret_skill.type_nk = 0;
	fclose(fp);
	return ret_skill;
}

/*****************************************
移動型タイプスキルのエフェクトの種類の指定

スキル効能地作成
type 7e:SW 7f:火壁 80:ポタ発動中 81:ポタ発動前 83:サンク 85:フニューマ
86:バーミリオン 8c:トーキーボックス発動時 8d:氷壁 8e:くあぐまいやー 91:あんくるすねあ
93:らんどまいん 97:?? 99:トーキーボックス発動前
他情報求む
*****************************************/
unsigned long search_placeskill(int skill_id)
{
	switch(skill_id)
	{
		//ＳＷ
	case 12:
		return 0x7e;
		break;
		//ファイアーウォール
	case 18:
		return 0x7f;
		//フーニューマ
	case 25:
		return 0x85;
		//ワープポータル
	case 27:
		return 0x80;
		//センチュアリ
	case 70:
		return 0x83;
		//バーミリオン
	case 86:
		return 0x86;
		//ランドマイン
	case 116:
		return 0x93;
		//アンクルスネア
	case 117:
		return 0x91;
		//トーキーボックス
	case 125:
		return 0x99;
	default:
		return 0;
		break;
	};
}
