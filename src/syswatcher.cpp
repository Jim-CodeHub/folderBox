
/**-----------------------------------------------------------------------------------------------------------------
 * @file	syswatcher.cpp
 * @brief	system inode monitor	
 *
 * Copyright (c) 2020-2020 Jim Zhang 303683086@qq.com
 *------------------------------------------------------------------------------------------------------------------
*/

#include <syswatcher.hpp>

using namespace NS_SYSWATCHER;


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
 *	@brief	    Constructure, init syswatcher
 *	@param[in]  None	
 *	@param[out] None
 *	@return		None	
 **/
syswatcher::syswatcher( ) 
{
	if ( -1 == (queue_fd = inotify_init()) ) { throw(errno); exit(-1); } ;
}

/**
 *	@brief	    Register, change and alter events for certain 'pathname'
 *	@param[in]	pathname	-	file or directory to be monitored 
 *	@param[in]	mask		-	events, IN_XXX	
 *	@param[out] None
 *	@return		watched filedescriptor	
 *	@note		1. use or-ed to create combined events 
 *				2. Or with IN_MASK_ADD to append instead of cover exsits events for the same pathname 
 **/
int syswatcher::reg( const char *pathname, int mask )
{
	int watch_fd = inotify_add_watch( queue_fd, pathname, mask );

	if ( -1 == watch_fd ) { throw(errno); exit(-1); }

	struct _reg_info *pReg = new struct _reg_info; 

	pReg->watch_fd = watch_fd;
	pReg->pathname = pathname;
	pReg->mask_reg = (mask & IN_MASK_ADD)?(pReg->mask_reg | mask):mask;

	list <struct _reg_info *>::iterator _big = resource.begin(),
										_end = resource.end(  );

	while ( _big != _end )
	{
		if ( (*_big)->pathname == pathname ) { 
			*_big = pReg; goto _EXIT;
		}

		_big++;
	}

	resource.push_back( pReg );

_EXIT:
	return watch_fd;
}

/**
 *	@brief	    Delete watch resource by pathname   
 *	@param[in]	pathname	-	file or directory to be monitored 
 *	@param[in]	wd			-	watched descriptor
 *	@param[out] None
 *	@return		None	
 *	@note		1. param 'wd' will be used if 'pathname' is NULL, otherwise pathname
 **/
void syswatcher::del( const char *pathname, int wd )
{
	list <struct _reg_info *>::iterator _big = resource.begin(),
										_end = resource.end(  );

	while ( _big != _end )
	{
		if ( ((NULL != pathname) && ((*_big)->pathname == pathname )) || ((*_big)->watch_fd == wd) )
		{
			inotify_rm_watch( queue_fd, (*_big)->watch_fd );

			delete *_big; this->resource.erase( _big ); break;
		}

		_big++;
	}

	return;
}

/**
 *	@brief	    Poller of events that montored 
 *	@param[in]  buff		-	events read buffer	
 *	@param[in]  _size		-	sizeof buff	
 *	@param[out] evFun		-	callback function, work when events occur
 *	@return		None	
 *	@note		1. The function perform a block style, hence the way to start a process or thread to handler callback function MAYBE used.	
 *				2. Set enough size for buffer to suitable
 **/
void syswatcher::poll( FB_CGI_T evFun, char *buff, ssize_t _size )
{
	struct _watched_src						 src;

	while ( true ) 
	{
		ssize_t len = read( queue_fd, buff, _size );

		if (-1 == len) { throw(errno); exit(-1); }

		int i = 0;
		while ( i < len ) 
		{
			struct inotify_event *pEvent = (struct inotify_event *)&buff[i];

			if ( (pEvent->mask & IN_Q_OVERFLOW) || (pEvent->mask & IN_UNMOUNT) ) { throw(errno); exit(-1); }

			list <struct _reg_info*>::const_iterator _big = resource.begin(),
													 _end = resource.end(  );

			while ( _big != _end )
			{
				if ( (*_big)->watch_fd == pEvent->wd )
				{
					src.pathname  = (*_big)->pathname;
					src.pathname += ((*_big)->pathname.at((*_big)->pathname.length()-1) == '/')?"":"/";
					src.pathname +=	pEvent->name;

					src.mask_reg  = (*_big)->mask_reg;
					src.mask_get  = pEvent->mask;
					src.watch_fd  = pEvent->wd;

					break;
				}

				_big++;
			}

			evFun( src );

			i += sizeof(struct inotify_event) + pEvent->len; 
		}
	}

	return;
}

/**
 *	@brief	    Destructure, close quque fd and release resource
 *	@param[in]	None
 *	@param[out] None
 *	@return		None	
 **/
syswatcher::~syswatcher( )
{
	close( queue_fd );

	list <struct _reg_info *>::const_iterator _big = resource.begin(),
											  _end = resource.end(  );

	while ( _big != _end ) { delete *_big; _big++; }

	this->resource.clear();
}

