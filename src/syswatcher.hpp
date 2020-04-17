
/**-----------------------------------------------------------------------------------------------------------------
 * @file	syswatcher.hpp
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/


#ifndef __SYSWATCHER_H__
#define __SYSWATCHER_H__


/*------------------------------------------------------------------------------------------------------------------
 *
 *												SYSWATCHER INCLUDES
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#include <errno.h>
#include <sys/inotify.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <vector>
#include <list>
#include <iostream>


using namespace std;

namespace NS_SYSWATCHER{


/*------------------------------------------------------------------------------------------------------------------
 *
 *												SYSWATCHER SHORTALIAS 
 *
 *------------------------------------------------------------------------------------------------------------------
*/
#define  SYSWATCHER_POLLER_BUFFER_SIZE		(sizeof(inotify_event) + 255)


/*------------------------------------------------------------------------------------------------------------------
 *
 *												SYSWATCHER DATABLOCK 
 *
 *------------------------------------------------------------------------------------------------------------------
*/

struct _watched_src{
	int			watch_fd; /**< watched filedescriptor. */
	uint32_t	mask_reg; /**< watched events. */
	uint32_t	mask_get; /**< get mask from read(). */
	string		pathname; /**< regists pathname + name from read(). */
};

typedef std::function<void( const struct _watched_src &src )> FB_CGI_T;

class syswatcher{
	public:	
		syswatcher( );
		~syswatcher();

		int	 reg ( const char *pathname, int mask			 );
		void del ( const char *pathname, int wd				 );
		void poll( FB_CGI_T evFun, char *buff, ssize_t _size );

		struct _reg_info{ /**< Regists info. */
			int			watch_fd;
			uint32_t	mask_reg;
			string		pathname;
		};

	private:
		int						 queue_fd;
		list<struct _reg_info *> resource;
}; 


} /**< NS_SYSWATCHER */


#endif /*__SYSWATCHER_H__*/

