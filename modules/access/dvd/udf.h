/*****************************************************************************
 * dvd_udf.h: structures for udf filesystem tools.
 *****************************************************************************
 * Copyright (C) 1998-2001 VideoLAN
 * $Id: udf.h,v 1.1 2002/08/04 17:23:41 sam Exp $
 *
 * Author: St�phane Borel <stef@via.ecp.fr>
 *
 * based on:
 *  - dvdudf by Christian Wolff <scarabaeus@convergence.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*
 * Fonctions in dvd_udf.c
 */
u32 DVDUDFFindFile( dvdcss_handle, char * );

