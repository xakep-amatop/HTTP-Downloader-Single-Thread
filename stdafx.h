#ifndef __STDAFX_H__
#define __STDAFX_H__

	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	#include <curl/curl.h>

	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>

	#include <libxml/xmlmemory.h>
	#include <libxml/parser.h>

	typedef struct{
		xmlChar **	urls;
		xmlChar *	path;
		xmlChar *	user_agent;

		char *		buff;

		xmlDocPtr	doc;
		xmlNodePtr	cur;

		size_t 		num_urls;
	} info;

	void cleanup(info * _info);

	#include "getXmlFields.h"
	#include "fpath.h"
	#include "download.h"

#endif