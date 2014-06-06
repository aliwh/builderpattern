#ifndef _XMLPARSER_
#define _XMLPARSER_

#include "XmlBuilder.h"

#define  MAX_ATTR_NR 100
struct _XmlParser;
typedef struct _XmlParser XmlParser;


//XML���������캯��
XmlParser* xml_parser_create(void);

//ΪXmlParser����builder����
void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder);

//����XML
void xml_parser_parse(XmlParser* thiz, const char* xml);

//��������
void xml_parser_destory(XmlParser* thiz);

#endif // !_XMLPARSER_

