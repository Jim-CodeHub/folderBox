
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

#include <iostream>
using namespace std;

/**
 *	@brief	    Init folderBox
 *	@param[in]  folderPath	- folder path which to be monitored
 *	@param[out] None
 *	@return		None	
 *	@note		The function work only if param 'folderPath' is a directory
 **/
folderBox::folderBox( const char *folderPath )
{ 
	if ( -1 == (queue_fd=inotify_init()) ) { throw(errno); exit(-1); } 

	//watch_fd = inotify_add_watch( queue_fd, folderPath, IN_ONLYDIR|IN_ACCESS );
	//watch_fd = inotify_add_watch( queue_fd, folderPath, IN_ALL_EVENTS);
	//watch_fd = inotify_add_watch( queue_fd, folderPath, IN_MOVED_FROM);
	//watch_fd = inotify_add_watch( queue_fd, folderPath, IN_MOVED_TO);
	watch_fd = inotify_add_watch( queue_fd, folderPath, IN_CREATE);

	if ( -1 == watch_fd ) { throw(errno); exit(-1); }
}

/**
 *	@brief	    Start folderBox (monitor) 
 *	@param[in]  None 
 *	@param[out] None
 *	@return		None	
 *	@note		The function perform a block style
 **/
void folderBox::Poller( void )
{
	struct pollfd fds = { .fd = queue_fd, .events = POLLIN };

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
				char buff[4096];

				const struct inotify_event *event;

				int len;

				while ( len=read(queue_fd, buff, sizeof(buff)), ((len > 0) || (-1 == len)) )
				{
					if ( (-1 == len) && (EAGAIN != errno) ) { throw(errno); exit(-1); }

					/* Loop over all events in the buffer */
					for ( char *ptr = buff; ptr < buff + len; ptr += sizeof(struct inotify_event) + event->len )
					{
						event = (const struct inotify_event *) ptr;

						if (event->mask & IN_ACCESS)
						{
							cout << "access" <<endl;
						}

						if (event->len)
							cout << event->name << endl;
					}
				}
			}
	}
}

