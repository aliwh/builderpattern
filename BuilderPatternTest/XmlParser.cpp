#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "XmlParser.h"


//XML解析器的对象，封装到.c文件中
struct _XmlParser
{
	const char* read_ptr;

	int   attrs_nr;
	char* attrs[2 * MAX_ATTR_NR + 1];

	char* buffer;//动态数组，存放属性名和属性值
	int buffer_used;
	int buffer_total;

	XmlBuilder* builder;
};

static const char* strtrim(char* str)
{
	char* p = NULL;

	p = str + strlen(str) - 1;

	while (p != str && isspace(*p))
	{
		*p = '\0';
		p--;
	}

	p = str;
	while (*p != '\0' && isspace(*p)) p++;

	if (p != str)
	{
		char* s = p;
		char* d = str;
		while (*s != '\0')
		{
			*d = *s;
			d++;
			s++;
		}
		*d = '\0';
	}

	return str;
}

static void xml_parser_reset_buffer(XmlParser* thiz)
{

	thiz->buffer_used = 0;
	thiz->attrs_nr = 0;
	thiz->attrs[0] = NULL;

	return;
}

static int xml_parser_strdup(XmlParser* thiz, const char* start, int length)
{
	int offset = -1;

	if ((thiz->buffer_used + length) >= thiz->buffer_total)
	{
		size_t length = thiz->buffer_total + (thiz->buffer_total >> 1) + 128;
		char* buffer = (char*)realloc(thiz->buffer, length);
		if (buffer != NULL)
		{
			thiz->buffer = buffer;
			thiz->buffer_total = length;
		}
	}

	if ((thiz->buffer_used + length) >= thiz->buffer_total)
	{
		return offset;
	}

	offset = thiz->buffer_used;
	strncpy(thiz->buffer + offset, start, length);
	thiz->buffer[offset + length] = '\0';
	strtrim(thiz->buffer + offset); //字符串的整理
	thiz->buffer_used += length + 1;

	return offset;
}

//解析属性值
static void xml_parser_parse_attrs(XmlParser* thiz, char end_char)
{
	int i = 0;
	enum _State{
		STAT_PRE_KEY,
		STAT_KEY,
		STAT_PRE_VALUE,
		STAT_VALUE,
		STAT_END,
	}state = STAT_PRE_KEY;

	char value_end = '\"';
	const char* start = thiz->read_ptr;

	thiz->attrs_nr = 0;

	for (; *thiz->read_ptr != '\0'&& thiz->attrs_nr < MAX_ATTR_NR; thiz->read_ptr++)
	{
		char c = *thiz->read_ptr;
		
		switch (state)
		{
		case STAT_PRE_KEY:
			if (c == end_char || c == '>')
			{
				state = STAT_END;
			}
			else if (!isspace(c))
			{

				state = STAT_KEY;
				start = thiz->read_ptr;
			}
		case STAT_KEY:
			if (c == '=')
			{
				thiz->attrs[thiz->attrs_nr++] =
					(char*)xml_parser_strdup(thiz, start, thiz->read_ptr - start);
				state = STAT_PRE_VALUE;
			}
			break;
		case STAT_PRE_VALUE:
			if (c == '\"' || c == '\'')//单引号和双引号都能用于界定属性值
			{
				state = STAT_VALUE;
				value_end = c;
				start = thiz->read_ptr + 1;
			}
			break;
		case STAT_VALUE:
			if (c == value_end)
			{
				thiz->attrs[thiz->attrs_nr++] =
					(char*) xml_parser_strdup(thiz, start, thiz->read_ptr - start);
				state = STAT_PRE_KEY;
			}
			break;
		default:
			break;
		}

		if (state == STAT_END) //“结束”状态
		{
			break;
		}
	}

	for ( i = 0; i < thiz->attrs_nr; i++)
	{
		thiz->attrs[i] = thiz->buffer + (size_t)(thiz->attrs[i]);
	}
	thiz->attrs[thiz->attrs_nr] = NULL;

	return;
}

static void xml_parser_parse_start_tag(XmlParser* thiz)
{

	enum _State{
		STAT_NAME,
		STAT_ATTR,
		STAT_END,
	}state = STAT_NAME;

	char* tag_name = NULL;
	const char* start = thiz->read_ptr - 1;

	for (; *thiz->read_ptr != '\0'; thiz->read_ptr++)
	{
		char c = *thiz->read_ptr;

		switch (state)
		{
		case STAT_NAME:
			if (isspace(c) || c == '>' || c == '/')
			{
				state = (c != '>' && c != '/') ? STAT_ATTR : STAT_END;
			}
			break;
		case STAT_ATTR:
			xml_parser_parse_attrs(thiz, '/');
			state = STAT_END;
			break;
		default:
			break;
		}

		if (state == STAT_END)
		{
			break;
		}
	}

	for (; *thiz->read_ptr != '>' && *thiz->read_ptr != '\0'; *thiz->read_ptr++)
	{
		;
	}

	return;
}

static void xml_parser_parse_end_tag(XmlParser* thiz)
{
	char* tag_name = NULL;
	const char* start = thiz->read_ptr;
	for (; *thiz->read_ptr != '\0'; thiz->read_ptr++)
	{
		if (*thiz->read_ptr == '>')
		{
			tag_name = thiz->buffer + xml_parser_strdup(thiz, start, thiz->read_ptr - start);
			xml_builder_on_end_element(thiz->builder, tag_name);

			break;
		}
	}

	return;

}

static void xml_parser_parse_entity(XmlParser* thiz)
{
	/*TODO*/

	return;
}

static void xml_parser_parse_text(XmlParser* thiz)
{
	const char* start = thiz->read_ptr - 1;
	for (; *thiz->read_ptr != '\0'; thiz->read_ptr++)
	{
		char c = *thiz->read_ptr;

		if (c == '<')
		{
			if (thiz->read_ptr > start)
			{
				xml_builder_on_text(thiz->builder, start, thiz->read_ptr - start);
			}
			thiz->read_ptr--;
			return;
		}
		else if (c == '&')
		{
			xml_parser_parse_entity(thiz);
		}
	}

	return;

}

static void xml_parser_parse_pi(XmlParser* thiz)
{
	enum _State{
		STAT_NAME,
		STAT_ATTR,
		STAT_END,
	}state = STAT_NAME;

	char* tag_name = NULL;
	const char* start = thiz->read_ptr;

	for (; *thiz->read_ptr != '\0'; thiz->read_ptr++)
	{
		char c = *thiz->read_ptr;

		switch (state)
		{
		case STAT_NAME:
			if (isspace(c) || c == '>')
			{
				tag_name = (char*)xml_parser_strdup(thiz, start, thiz->read_ptr - start);
				state = (c != '>') ? STAT_ATTR : STAT_END;
			}
			break;
		case STAT_ATTR:
			xml_parser_parse_attrs(thiz, '?');
			state = STAT_END;
			break;
		default:
			break;
		}

		if (state == STAT_END)
		{
			break;
		}
	}

	tag_name = thiz->buffer + (size_t)tag_name;
	xml_builder_on_pi_element(thiz->builder, tag_name, (const char**)thiz->attrs);

//	for (; *thiz->read_ptr != '>' && *thiz->read_ptr != '\0'; *thiz->read_ptr++) ;

	return;

}

static void xml_parser_parse_comment(XmlParser* thiz)
{
	enum _State
	{
		STAT_COMMENT,
		STAT_MINUS1,
		STAT_MINUS2,
	}state = STAT_COMMENT;

	const char* start = ++thiz->read_ptr;
	for (; *thiz->read_ptr != '\0'; thiz->read_ptr++)
	{
		char c = *thiz->read_ptr;

		switch (state)
		{
		case STAT_COMMENT:
		{
			 if (c == '-')
			 {
				 state = STAT_MINUS1;
			 }
			break;
		}
		case STAT_MINUS1:
		{
			if (c == '-')
			{
				state = STAT_MINUS2;
			}
			else
			{
				state = STAT_COMMENT;
			}
			break;
		}
		case STAT_MINUS2:
		{
			if (c == '>')
			{
				xml_builder_on_comment(thiz->builder, start, thiz->read_ptr - start - 2);
				return;
			}
		}
		default:break;
		}
	}

	return;

}

//XML解析器构造函数
XmlParser* xml_parser_create(void)
{
	XmlParser* parser = (XmlParser*)calloc(1, sizeof(XmlParser));
	if (parser != NULL)
	{
		
	}

	return parser;
}

//为XmlParser设置builder对象
void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder)
{
	return_if_fail(thiz != NULL && builder != NULL);

	thiz->builder = builder;

	return;
}

//解析XML
void xml_parser_parse(XmlParser* thiz, const char* xml)
{
	/* 定义状态的枚举值 */
	enum _State 
	{
		STAT_NONE,
		STAT_AFTER_LT,
		STAT_START_TAG,
		STAT_END_TAG,
		STAT_TEXT,
		STAT_PRE_COMMENT1,
		STAT_PRE_COMMENT2,
		STAT_COMMENT,
		STAT_PROCESS_INSTRUCTION,
	}state = STAT_NONE;

	thiz->read_ptr = xml;

	for (; thiz->read_ptr != '\0';thiz->read_ptr++)
	{
		char c = thiz->read_ptr[0];
		switch (state)
		{
		case STAT_NONE:
			if (c == '<')
			{
				xml_parser_reset_buffer(thiz);
				state = STAT_AFTER_LT;
			}
			else if (!isspace(c))
			{
				state = STAT_TEXT;
			}
			break;
		case STAT_AFTER_LT:
			if (c == '!')
			{
				state = STAT_PRE_COMMENT1;
			}
			else if (c == '?')
			{
				state = STAT_PROCESS_INSTRUCTION;
			}
			else if (c == '/')
			{
				state = STAT_END_TAG;
			}
			else if (isalpha(c) || c == '_')
			{
				state = STAT_START_TAG;
			}
			else
			{

			}
			break;
		case STAT_START_TAG:
			/*进入子状态*/
			xml_parser_parse_start_tag(thiz);
			state = STAT_NONE;
			break;
		case STAT_END_TAG:
			/*进入子状态*/
			xml_parser_parse_end_tag(thiz);
			state = STAT_NONE;
			break;
		case STAT_TEXT:
			/*进入子状态*/
			xml_parser_parse_text(thiz);
			state = STAT_NONE;
			break;
		case STAT_PRE_COMMENT1:
			if (c == '-')
			{
				state = STAT_PRE_COMMENT2;
			}
			break;
		case STAT_PRE_COMMENT2:
			if (c == '-')
			{
				state = STAT_COMMENT;
			}
			break;
		case STAT_COMMENT:
			/*进入子状态*/
			xml_parser_parse_comment(thiz);
			state = STAT_NONE;
			break;
		case STAT_PROCESS_INSTRUCTION:
			/*进入子状态*/
			xml_parser_parse_pi(thiz);
			state = STAT_NONE;
			break;
		default:
			break;
		}
	}
}

//析构函数
void xml_parser_destory(XmlParser* thiz)
{
	if (thiz != NULL)
	{
		free(thiz->buffer);
		free(thiz);
	}

	return;
}