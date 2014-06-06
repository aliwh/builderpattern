// BuilderPatternTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "XmlParser.h"
#include "XmlDumpBuilder.h"


int _tmain(int argc, _TCHAR* argv[])
{
	const char* xml = "<?xml version=\"1.0\" encoding=\"utf - 8\" ?> <mime - type xmlns = \"http://www.freedesktop.org/standards/shared-mime-info\" type = \"all/all\"> <!--Created automatically by update - mime - database.DO NOT EDIT!--> <comment>all files and folders< / comment> </mime-type>";
	
	XmlBuilder* builder;
	XmlParser* parser;

	//创建Builder(初始化Builder接口)
	builder = xml_builder_dump_create(stdout);

	//创建Director
	parser = xml_parser_create();

	//为parser设置builder
	xml_parser_set_builder(parser, builder);

	//解析XML

	xml_parser_parse(parser,xml);

	//析构parser
	xml_parser_destory(parser);

	return 0;
}

