#ifndef _XMLPARSER_
#define _XMLPARSER_

#include "XmlBuilder.h"

#define  MAX_ATTR_NR 100
struct _XmlParser;
typedef struct _XmlParser XmlParser;


//XML解析器构造函数
XmlParser* xml_parser_create(void);

//为XmlParser设置builder对象
void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder);

//解析XML
void xml_parser_parse(XmlParser* thiz, const char* xml);

//析构函数
void xml_parser_destory(XmlParser* thiz);

#endif // !_XMLPARSER_

