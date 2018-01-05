#include "stdafx.h"

static size_t WriteData(char * ptr, size_t size, size_t nmemb, void * stream){
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

static unsigned int is_perform(CURLcode res, char * url){
	if (res != CURLE_OK){
		fprintf(stderr, "Curl curl_easy_perform failed! URL: %s\n\t%s\n",
		url,
		curl_easy_strerror(res));
		return 0;
	}
	return 1;
}

void GetSaveData(info * _info, char * file_name, char * url){
	CURL * curl = curl_easy_init();
	if (curl) {
		FILE * fp = fopen(_info->buff, "wb");
		if (fp){
			curl_easy_setopt(curl,	CURLOPT_URL,			url);
			curl_easy_setopt(curl,	CURLOPT_WRITEFUNCTION,	WriteData);
			curl_easy_setopt(curl,	CURLOPT_WRITEDATA,		fp);
			curl_easy_setopt(curl,	CURLOPT_USERAGENT,		_info->user_agent);
			CURLcode res = curl_easy_perform(curl);

			if (!is_perform(res, (char *) url)){
				unlink(_info->buff);
			}

			fclose(fp);
		}
		else perror((char*)_info->buff);
		curl_easy_cleanup(curl);
	}
	else
		fprintf(stderr, "%s\n", "Error!!! Can not start a libcurl easy session!");
}

void ReadConfiguration(info * _info){
	const xmlChar * header[] 				=	{
													(const xmlChar*) "user-agent",
													(const xmlChar*) "urls",
													(const xmlChar*) "dir-path"
												};

	void (* parse[])(info * _info)	=	{
													parse_UserAgent,
													parse_URLS,
													parse_DirPath
												};

	if (sizeof(parse)/sizeof(void(*)(info * _info)) >
		sizeof(header)/sizeof(const xmlChar *)){
		fprintf(stderr, "Error!!! Number sub-root fields less then number parse function!\n");
		cleanup(_info);
		exit(EXIT_FAILURE);
	}

	_info->cur = _info->cur->xmlChildrenNode;
	while (_info->cur != NULL) {
		for(size_t i = 0; i < sizeof(parse)/sizeof(void(*)(info * _info)); ++i)
			if (xmlStrEqual(_info->cur->name, header[i])){
				parse[i](_info);
				break;
			}
		_info->cur = _info->cur->next;
	}
}

void is_root_configuration(info * _info){
	if (xmlStrcmp(_info->cur->name, (const xmlChar *) "configuration")){
		fprintf(stderr,"Document of the wrong type, root node != configuration");
		cleanup(_info);
		exit(EXIT_FAILURE);
	}
}