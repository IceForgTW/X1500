//********************* BEGIN LICENSE BLOCK **********************************
//
// INGENIC CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
// Copyright (c) Ingenic Semiconductor Co. Ltd 2009. All rights reserved.
// 
// This file, and the files included with this file, is distributed and made 
// available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
// EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
// INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
// 
// http://www.ingenic.cn 
//
//********************* END LICENSE BLOCK ************************************

#define  OS_GLOBALS                           /* Declare GLOBAL variables                              */
#include <minios.h>


#define  OS_MASTER_FILE                       /* Prevent the following files from including includes.h */
#include <os_core.c>
#include <os_flag.c>
#include <os_mbox.c>
#include <os_mem.c>
#include <os_mutex.c>
#include <os_q.c>
#include <os_sem.c>
#include <os_task.c>
#include <os_time.c>
#include <os_tmr.c>
