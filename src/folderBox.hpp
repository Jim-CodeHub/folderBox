
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
 *												FolderBox SHORTALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  FOLDERBOX_POLLER_BUFFER_SIZE		(sizeof(inotify_event) + 255)


/*------------------------------------------------------------------------------------------------------------------
 *
 *												FolderBox DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

typedef void (*pvFun)(const void *val, size_t _size);

class folderBox{
	public:	
		folderBox( const char *folderPath, int subLevel=0 );
		~folderBox() { inotify_rm_watch(queue_fd, watch_fd); close(queue_fd); delete [] buffer; }

		void Poll( pvFun Add, pvFun Del );

	protected:
		folderBox(){};
		void init( const char *folderPath, int subLevel=0 );

	private:
		int			queue_fd;
		int			watch_fd;
		char	   *buffer	; /**< Poller buffer for 'read()'. */
}; 


} /**< NS_FOLDERBOX */


#endif /*__FOLDERBOX_H__*/

