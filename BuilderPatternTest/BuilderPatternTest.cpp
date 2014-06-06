// BuilderPatternTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "XmlParser.h"
#include "XmlDumpBuilder.h"


int _tmain(int argc, _TCHAR* argv[])
{
	const char* xml = "<?xml version=\"1.0\" encoding=\"utf - 8\" ?> <mime - type xmlns = \"http://www.freedesktop.org/standards/shared-mime-info\" type = \"all/all\"> <!--Created automatically by update - mime - database.DO NOT EDIT!--> <comment>all files and folders< / comment> </mime-type>";
	
	XmlBuilder* builder;
	XmlParser* parser;

	//����Builder(��ʼ��Builder�ӿ�)
	builder = xml_builder_dump_create(stdout);

	//����Director
	parser = xml_parser_create();

	//Ϊparser����builder
	xml_parser_set_builder(parser, builder);

	//����XML

	xml_parser_parse(parser,xml);

	//����parser
	xml_parser_destory(parser);

	return 0;
}

