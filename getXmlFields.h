#ifndef __GETXMLFIELDS_H__
#define __GETXMLFIELDS_H__

	void parse_UserAgent(info * _info);
	void parse_URLS		(info * _info);
	void parse_DirPath	(info * _info);

	void freeXmlKeys(xmlChar ** keys, size_t max_count);

#endif