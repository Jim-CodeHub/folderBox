
/**-----------------------------------------------------------------------------------------------------------------
 * @file	folderBox.hpp
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __FOLDERBOX_H__
#define __FOLDERBOX_H__


/*------------------------------------------------------------------------------------------------------------------
 *
 *												FolderBox INCLUDES
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <errno.h>
#include <sys/inotify.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>


namespace NS_FOLDERBOX{

/*------------------------------------------------------------------------------------------------------------------
 *
 *												FolderBox DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

class folderBox{
	public:	
		folderBox( const char *folderPath );
		~folderBox() { inotify_rm_watch(queue_fd, watch_fd); close(queue_fd); }

		void Poller( void );

	private:
		int			queue_fd;
		int			watch_fd;
}; 


} /**< NS_FOLDERBOX */


#endif /*__FOLDERBOX_H__*/

