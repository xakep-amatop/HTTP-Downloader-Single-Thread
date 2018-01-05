#include "stdafx.h"

void PrintHelp(char * name, FILE * _file);

int main(int argc, char * argv[]){
	if(argc < 2){
		fprintf(stderr, "Error!!! Pass the XML configuration file as an argument to the program!\n");
		PrintHelp(argv[0], stderr);
		return EXIT_FAILURE;
	}
	else{
		info _info = {0};

		_info.doc = xmlParseFile(argv[1]);
		if (_info.doc == NULL ) {
			fprintf(stderr,"Document not parsed successfully. \n");
			cleanup(& _info);
			return EXIT_FAILURE;
		}

		_info.cur = xmlDocGetRootElement(_info.doc);
		if (_info.cur == NULL) {
			fprintf(stderr,"empty document\n");
			cleanup(& _info);
			return EXIT_FAILURE;
		}

		is_root_configuration(& _info);

		ReadConfiguration(& _info);

		if (_info.urls && _info.user_agent){
			size_t	len_path	= 0;

			if(_info.path){
				len_path	= GetPath(&_info);
			}

			for (size_t i = 0; i < _info.num_urls; ++i){

				char * file_name	= strrchr((char *)_info.urls[i], '/');

				if (file_name){
					size_t len_url	= strlen((char*)_info.urls[i]);
					char * url		= (char*)malloc(++len_url * sizeof(char));

					memcpy(url, _info.urls[i], len_url);
					char * start = NULL;
					while (file_name != start && !*(file_name+1)){
						*file_name		= '\0';
						start			= strchr ((char *)url, '/');
						file_name		= strrchr((char *)url, '/');
					}
					free(url);
				}

				char * name			= (file_name ? ++file_name : (char *) _info.urls[i]);

				if (!len_path){
					if(_info.buff)
						free(_info.buff);
					_info.buff	= (char * )malloc(sizeof(char) * (strlen(name) + 1));
				}
				else{
					size_t buff_len	= strlen(_info.buff);
					size_t full_len	= buff_len + strlen(name) + 1;
					char * tmp_buff	= (char*)malloc(sizeof(char) * full_len);
					strncpy(tmp_buff, _info.buff, buff_len);
					free(_info.buff);
					_info.buff		= tmp_buff;
				}

				strncpy(_info.buff + len_path, name, strlen(name)+1);

				GetSaveData(& _info, file_name, (char *) _info.urls[i]);
			}
		}
		cleanup(& _info);
	}
	return EXIT_SUCCESS;
}

void PrintHelp(char * name, FILE * _file){
	char * _n = strrchr(name, '/');
	fprintf(_file,
		"\tUsage: %s <name xml configuration file>\n\n",
		_n ? ++_n : name
	);
}

void cleanup(info * _info){
	freeXmlKeys(_info->urls, _info->num_urls);
	if (_info->user_agent)	free(_info->user_agent);
	if (_info->buff)		free(_info->buff);
	if (_info->path)		xmlFree(_info->path);
	if (_info->doc)			xmlFreeDoc(_info->doc);
	xmlCleanupParser();
}
