/*****************************************************************************
 * kde.cpp : KDE plugin for vlc
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: kde.cpp,v 1.1 2002/08/04 17:23:43 sam Exp $
 *
 * Authors: Andres Krapf <dae@chez.com> Sun Mar 25 2001
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

#include "common.h"

#include "interface.h"

#include <iostream>

#include <kaction.h>
#include <kapp.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kstdaction.h>
#include <qwidget.h>

/*****************************************************************************
 * The local class.
 *****************************************************************************/
class KInterface;
class KAboutData;

class KThread
{
    private:
        KThread ( KThread &thread ) { };
        KThread &operator= ( KThread &thread ) { return ( *this ); };

        intf_thread_t *p_intf;
        
    public:
        KThread(intf_thread_t *p_intf);
        ~KThread();

        // These methods get exported to the core
        static int     open    ( vlc_object_t * );
        static void    close   ( vlc_object_t * );
        static void    run     ( intf_thread_t * );
};

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin();
#ifdef WIN32
    int i = 90;
#else
    int i = getenv( "DISPLAY" ) == NULL ? 8 : 85;
#endif
    set_description( _("KDE interface module") );
    set_capability( "interface", i );
    set_program( "kvlc" );
    //set_callbacks( E_(Open), E_(Close) );
    set_callbacks( KThread::open, KThread::close );
vlc_module_end();

/*****************************************************************************
 * KThread::KThread: KDE interface constructor
 *****************************************************************************/
KThread::KThread(intf_thread_t *p_intf)
{
    this->p_intf = p_intf;

    p_intf->p_sys->p_about =
      new KAboutData( "VideoLAN Client", I18N_NOOP("Kvlc"), VERSION,
         _("This is the VideoLAN client, a DVD and MPEG player. It can play "
           "MPEG and MPEG 2 files from a file or from a network source."),
         KAboutData::License_GPL,
         _("(C) 1996, 1997, 1998, 1999, 2000, 2001, 2002 - the VideoLAN Team"),
         0, 0, "");

    char *authors[][2] = {
        { "the VideoLAN Team", "<videolan@videolan.org>" },
        { NULL, NULL },
    };

    for ( int i = 0; NULL != authors[i][0]; i++ ) {
        p_intf->p_sys->p_about->addAuthor( authors[i][0], 0, authors[i][1] );
    }

    int argc = 1;
    char *argv[] = { p_intf->p_vlc->psz_object_name, NULL };
    KCmdLineArgs::init( argc, argv, p_intf->p_sys->p_about );

    p_intf->p_sys->p_app = new KApplication();
    p_intf->p_sys->p_window = new KInterface(p_intf);
    p_intf->p_sys->p_window->setCaption( VOUT_TITLE " (KDE interface)" );

    p_intf->p_sys->p_input = NULL;
}

/*****************************************************************************
 * KThread::~KThread: KDE interface destructor
 *****************************************************************************/
KThread::~KThread()
{
    if( p_intf->p_sys->p_input )
    {
        vlc_object_release( p_intf->p_sys->p_input );
    }

    /* XXX: can be deleted if the user closed the window ! */
    //delete p_intf->p_sys->p_window;

    delete p_intf->p_sys->p_app;
    delete p_intf->p_sys->p_about;
}

/*****************************************************************************
 * KThread::open: initialize and create window
 *****************************************************************************/
int KThread::open(vlc_object_t *p_this)
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;

    /* Allocate instance and initialize some members */
    p_intf->p_sys = (intf_sys_t *)malloc( sizeof( intf_sys_t ) );
    if( p_intf->p_sys == NULL )
    {
        msg_Err( p_intf, "out of memory" );
        return( 1 );
    }

    p_intf->pf_run = KThread::run;

    p_intf->p_sys->p_thread = new KThread(p_intf);
    return ( 0 );
}

/*****************************************************************************
 * KThread::close: destroy interface window
 *****************************************************************************/
void KThread::close(vlc_object_t *p_this)
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;

    delete p_intf->p_sys->p_thread;
    free( p_intf->p_sys );
}

/*****************************************************************************
 * KThread::run: KDE thread
 *****************************************************************************
 * This part of the interface is in a separate thread so that we can call
 * exec() from within it without annoying the rest of the program.
 *****************************************************************************/
void KThread::run(intf_thread_t *p_intf)
{
    p_intf->p_sys->p_window->show();
    p_intf->p_sys->p_app->exec();
}

