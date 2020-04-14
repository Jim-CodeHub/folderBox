
/**-----------------------------------------------------------------------------------------------------------------
 * @file	folderBox.cpp
 * @brief	Easy hotfolder library 
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <folderBox.hpp>

using namespace NS_FOLDERBOX;


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS PROTOTYPES
*
--------------------------------------------------------------------------------------------------------------------
*/


/*
--------------------------------------------------------------------------------------------------------------------
*
*			                                  FUNCTIONS IMPLEMENT
*
--------------------------------------------------------------------------------------------------------------------
*/
/**
 *	@brief	    Init folderBox
 *	@param[in]  folderPath	- folder path which to be monitored
 *	@param[in]  subLevel	- -1 : Monitor all sub directory recursively 
 *							-  0 : Do not monitor sub directory
 *							- >0 : Monitor N level of sub directory 
 *	@param[out] None
 *	@return		None	
 *	@note		The function work only if param 'folderPath' is a directory
 **/
folderBox::folderBox( const char *folderPath, int subLevel )
{
	if ( -1 == (queue_fd=inotify_init()) ) { throw(errno); exit(-1); } 

	watch_fd = inotify_add_watch( queue_fd, folderPath, IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO );

	if ( -1 == watch_fd ) { throw(errno); exit(-1); }

	this->buffer = new char[ FOLDERBOX_POLLER_BUFFER_SIZE ];

	return;
}

/**
 *	@brief	    Init folderBox
 *	@param[in]  folderPath	- folder path which to be monitored
 *	@param[in]  subLevel	- -1 : Monitor all sub directory recursively 
 *							-  0 : Do not monitor sub directory
 *							- >0 : Monitor N level of sub directory 
 *	@param[out] None
 *	@return		None	
 *	@note		The function work only if param 'folderPath' is a directory
 **/
void folderBox::init( const char *folderPath, int subLevel )
{
	if ( -1 == (queue_fd=inotify_init()) ) { throw(errno); exit(-1); } 

	watch_fd = inotify_add_watch( queue_fd, folderPath, IN_CREATE|IN_DELETE|IN_MOVED_FROM|IN_MOVED_TO );

	if ( -1 == watch_fd ) { throw(errno); exit(-1); }

	this->buffer = new char[ FOLDERBOX_POLLER_BUFFER_SIZE ];

	return;
}

/**
 *	@brief	    Start folderBox (monitor) 
 *	@param[in]  Add		-	callback function for add event 
 *	@param[in]  Del		-	callback function for delete event 
 *	@param[out] None
 *	@return		None	
 *	@note		The function perform a block style
 **/
void folderBox::Poll( pvFun Add, pvFun Del )
{
	const struct	inotify_event *event;
	struct			pollfd fds = { .fd = queue_fd, .events = POLLIN };

	while ( true )
	{
		switch ( poll( &fds, 1, -1 ) )
		{
			case -1: /**< Error */
				if ( EINTR != errno ) { throw(errno); exit(-1); }
				break;
			case 0 : /**< Nothing to be done. */ 
				break;
			default: /**< Get ev */
				if ( fds.revents & POLLIN ) /**< Inotify events are available. */
				{
					int len = read( queue_fd, this->buffer, FOLDERBOX_POLLER_BUFFER_SIZE );

					if ( (-1 == len) && (EAGAIN != errno) ) { throw(errno); exit(-1); }

					if ( !(len <= 0) )
					{
						event = (const struct inotify_event *) buffer;

						if ( (event->mask & IN_Q_OVERFLOW) || (event->mask & IN_UNMOUNT) ) { throw(errno); exit(-1); }

						if ( (event->mask & IN_CREATE) || (event->mask & IN_MOVED_TO  ) )
						{
							if ( NULL != Add ) { Add(event->name, event->len); }
						}

						if ( (event->mask & IN_DELETE) || (event->mask & IN_MOVED_FROM) )
						{
							if ( NULL != Del ) { Del(event->name, event->len); }
						}
					}
				}
		}
	}

	return;
}

