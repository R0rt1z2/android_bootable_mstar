/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#ifndef _DRVLIST_H
#define _DRVLIST_H

#include "MsTypes.h"

/* list structure definition */
typedef struct list_head {
    struct list_head *next;
    struct list_head *prev;
}list_t;

#define DECLARE_LIST(mylist)  \
      struct list_head mylist = { &(mylist), &(mylist) }

/**
     * @brief             Initial list structure data member
     *
     * @param pInitList   pointer of the list to be initialized.
     *
     * @return            None
     */
static __inline__ void ms_list_init (struct list_head *pInitList)
{
	pInitList->next = pInitList;
	pInitList->prev = pInitList;
}

/**
     * @brief                           Insert list after the specified list
     *
     * @param pNew                New list to be added.
     *
     * @param pInsert_after_it  Insert list after this list.
     *
     * @return            None
     */
static __inline__ void ms_insert_list_after( struct list_head *pNew, struct list_head *pInsert_after_it )
{
    struct list_head *next;
    next = pInsert_after_it->next;
    pInsert_after_it->next = pNew;
    pNew->next = next;
    pNew->prev = pInsert_after_it;
    next->prev = pNew;
}

/**
     * @brief              check if  is empty list
     *
     * @param pList    list to be checked.
     *
     * @return            boolean
     */
static __inline__ int ms_is_empty_list( struct list_head *pList )
{
    if (pList == pList->next)
        return TRUE;
    else
        return FALSE;
}

/**
     * @brief                              Insert list before the specified list
     *
     * @param pNew                   New list to be added.
     *
     * @param pInsert_before_it   Insert list before this list.
     *
     * @return            None
     */
static __inline__ void ms_insert_list_before( struct list_head *pNew, struct list_head *pInsert_before_it )
{
    struct list_head *prev;
    prev = pInsert_before_it->prev;
    pNew->prev = prev;
    pNew->next = pInsert_before_it;
    pInsert_before_it->prev = pNew;
    prev->next = pNew;
}

/**
     * @brief                        Remove current entry from list
     *
     * @param pNew             Current entry to be removed.
     *
     * @return                      None
     */
static __inline__ void ms_list_remove( struct list_head *pList )
{
    pList->prev->next = pList->next;
    pList->next->prev = pList->prev;
}

/**
     * @brief                        Remove current entry from list and initial the entry
     *
     * @param pEntry            Current entry to be removed.
     *
     * @return                      None
     */
static __inline__ void ms_list_remove_and_init(struct list_head *pEntry)
{
    ms_list_remove(pEntry);
    ms_list_init(pEntry);
}

/**
     * @brief                       Join lists.
     *
     * @param pList             The list to be added.
     *
     * @param pHead           The place
     *
     * @return                     None
     */
static __inline__ void ms_list_join(struct list_head *pList, struct list_head *pHead)
{
  struct list_head *pFirst = pList->next;
  struct list_head *pLast = pList->prev;
  struct list_head *pNext = pHead->next;

  pFirst->prev = pHead;
  pHead->next = pFirst;

  pLast->next = pNext;
  pNext->prev = pLast;
}

/**
     * @brief                       cast a member item to the container structure.
     *
     * @param _item_          member item pointer.
     *
     * @param _struct_        The type definition of the container structure
     *
     * @param _member_     The name of the _item_ in the structure _struct_
     *
     * @return                     the container structure pointer
     */
#define entry_to_container( _item_, _struct_, _member_ ) \
    ((_struct_ *)((char *)(_item_)-(char *)(offsetof(_struct_,_member_))))

#define list_for_loop( _item_, _list_ )   \
    for ( (_item_)=(_list_)->next; (_item_)!=(_list_); (_item_)=(_item_)->next )

#define list_for_each_entry(_list_, _head_, _item_)                     \
for ((_list_) = entry_to_container((_head_)->next, typeof(*_list_), _item_); \
     &((_list_)->_item_) != (_head_);                                 \
     (_list_) = entry_to_container((_list_)->_item_.next, typeof(*_list_), _item_))

#define list_for_loop_ex(_item_, _temp_, _list_) \
  for (_item_=(_list_)->next, _temp_=_item_->next; _item_!=(_list_); _item_=_temp_, _temp_=_item_->next)

#if 0
#define list_for_each_prev(_item_, _list_) \
    or (_item_ = (_list_)->prev; _item_ != (_list_); _item_ = _item_->prev)
#endif

#define INIT_LIST_HEAD( _list_ )   ms_list_init( _list_)
#define list_add( _newlist_, _list_add_after_ )  ms_insert_list_after( _newlist_, _list_add_after_ )
#define list_empty( _list_ )  ms_is_empty_list( _list_)
#define list_add_tail( _newlist_, _list_add_before_ )  ms_insert_list_before( _newlist_, _list_add_before_ )
#define list_del( _list_ )   ms_list_remove( _list_)
#define list_del_init( _list_ )   ms_list_remove_and_init( _list_)
#define __list_splice( _list_, _head_ )   ms_list_join( _list_, _head_ )
#define list_entry( _list_, _struct_, _member_ ) entry_to_container( _list_, _struct_, _member_ )
#define list_for_each_safe(_item_, _temp_, _list_)  list_for_loop_ex(_item_, _temp_, _list_)
#define list_for_each( _item_, _list_ )   list_for_loop( _item_, _list_ )

#endif


