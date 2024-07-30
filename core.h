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
// core.h - basic Commands include File

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define BASE_TICK 5

#define RFIFOP(fd,pos) (session[fd]->rdata+session[fd]->rdata_pos+pos)
#define RFIFOB(fd,pos) (*(unsigned char*)(session[fd]->rdata+session[fd]->rdata_pos+pos))
#define RFIFOW(fd,pos) (*(unsigned short*)(session[fd]->rdata+session[fd]->rdata_pos+pos))
#define RFIFOL(fd,pos) (*(unsigned int*)(session[fd]->rdata+session[fd]->rdata_pos+pos))
#define RFIFOSKIP(fd,len) (session[fd]->rdata_pos+=len)
#define RFIFOREST(fd) (session[fd]->rdata_size-session[fd]->rdata_pos)
#define RFIFOFLUSH(fd) (memmove(session[fd]->rdata,RFIFOP(fd,0),RFIFOREST(fd)),session[fd]->rdata_size=RFIFOREST(fd),session[fd]->rdata_pos=0)
#define RFIFOSPACE(fd) (session[fd]->max_rdata-session[fd]->rdata_size)
#define RBUFP(p,pos) (((unsigned char*)(p))+pos)
#define RBUFB(p,pos) (*(unsigned char*)RBUFP(p,pos))
#define RBUFW(p,pos) (*(unsigned short*)RBUFP(p,pos))
#define RBUFL(p,pos) (*(unsigned int*)RBUFP(p,pos))

#define WFIFOSPACE(fd) (session[fd]->max_wdata-session[fd]->wdata_size)
#define WFIFOP(fd,pos) (session[fd]->wdata+session[fd]->wdata_size+pos)
#define WFIFOB(fd,pos) (*(unsigned char*)(session[fd]->wdata+session[fd]->wdata_size+pos))
#define WFIFOW(fd,pos) (*(unsigned short*)(session[fd]->wdata+session[fd]->wdata_size+pos))
#define WFIFOL(fd,pos) (*(unsigned int*)(session[fd]->wdata+session[fd]->wdata_size+pos))
#define WFIFOSET(fd,len) (session[fd]->wdata_size = (session[fd]->wdata_size+len+2048 < session[fd]->max_wdata) ? session[fd]->wdata_size+len : session[fd]->wdata_size)
#define WBUFP(p,pos) (((unsigned char*)(p))+pos)
#define WBUFB(p,pos) (*(unsigned char*)WBUFP(p,pos))
#define WBUFW(p,pos) (*(unsigned short*)WBUFP(p,pos))
#define WBUFL(p,pos) (*(unsigned int*)WBUFP(p,pos))

#define TIMER_ONCE_AUTODEL 0
#define TIMER_ONCE 1
#define TIMER_INTERVAL 2
struct TimerData {
  unsigned int tick;
  int (*func)(int,unsigned int,int,int);
  int id;
  int data;
  int type;
  int interval;
} **timer_data;

struct socket_data{
  int eof;
  unsigned char *rdata,*wdata;
  int max_rdata,max_wdata;
  int rdata_size,wdata_size;
  int rdata_pos;
  struct sockaddr_in client_addr;
  int (*func_recv)(int);
  int (*func_send)(int);
  int (*func_parse)(int);
  void* session_data;
} *session[FD_SETSIZE];

int rfifo_size,wfifo_size;
int (*default_func_parse)(int);
void (*term_func)(void);

int do_init(int,char**);
int do_exit(void);
int make_listen_port(int);
int make_connection(long,int);
int delete_session(int);
unsigned int gettick(void);
int add_timer(unsigned int,int (*)(int,unsigned int,int,int),int,int);
int delete_timer(int,int (*)(int,unsigned int,int,int));
int add_basic_loop_func(int (*)(int));
int del_basic_loop_func(int);

