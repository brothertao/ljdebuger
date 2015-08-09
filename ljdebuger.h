#ifndef PHP_LJDEBUGER_H
#define PHP_LJDEBUGER_H

#include "php.h"
#include "zend_extensions.h"
#include "ext/standard/php_smart_str.h"
#include "ext/json/php_json.h"

#define LJDEBUGER_NAME       "ljdebuger"
#define LJDEBUGER_VERSION    "0.0.1"
#define LJDEBUGER_AUTHOR     "moven"

#define LJDBUGER_LOG_FILE "/tmp/ljdebuger.log"
#define MOVEN_LOG(format,args...)   do {\
    FILE* f = fopen(LJDBUGER_LOG_FILE, "a");\
    if(f!= NULL) {\
        fprintf(f,"[%s:%d]" format "\n",__FILE__,__LINE__,##args);\
        fclose(f);\
    }\
} while(0)

#ifndef ZEND_EXT_API
#define ZEND_EXT_API    
#endif


#endif
