#ifndef __XMLBUILDER_H__
#define __XMLBUILDER_H__

#include "typedef.h"

struct _XmlBuilder;
typedef struct _XmlBuilder XmlBuilder;

typedef void (*XmlBuilderOnStartElementFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnEndElementFunc)(XmlBuilder* thiz, const char* tag);
typedef void (*XmlBuilderOnTextFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnCommentFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnPiElementFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnErrorFunc)(XmlBuilder* thiz, int line, int row,const char* message);
typedef void (*XmlBuilderDestroyFunc)(XmlBuilder* thiz);

struct _XmlBuilder
{
	XmlBuilderOnStartElementFunc on_start_element; //解析器解析到一个起始标签时调用它
	XmlBuilderOnEndElementFunc on_end_element; //解析器解析到一个结束标签时调用它
	XmlBuilderOnTextFunc on_text;//解析器解析到一段文本时调用它
	XmlBuilderOnCommentFunc on_comment;//解析器解析到一个注释时调用它
	XmlBuilderOnPiElementFunc on_pi_element;//解析器解析到一个处理指令时调用它
	XmlBuilderOnErrorFunc on_error;//解析器遇到错误时调用它
	XmlBuilderDestroyFunc destory;//用于销毁Builder对象

	char priv[1];
};

/* Builder 接口封装 */
static inline void xml_builder_on_start_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	return_if_fail(thiz != NULL && thiz->on_start_element != NULL);
	thiz->on_start_element(thiz,tag, attrs);

	return;
}

static inline void xml_builder_on_end_element(XmlBuilder* thiz, const char* tag)
{
	return_if_fail(thiz != NULL && thiz->on_end_element != NULL);
	thiz->on_end_element(thiz, tag);

	return;
}

static inline void xml_builder_on_text(XmlBuilder* thiz, const char* text, size_t length)
{
	return_if_fail(thiz != NULL && thiz->on_text != NULL);
	thiz->on_text(thiz, text, length);
	return;
}

static inline void xml_builder_on_comment(XmlBuilder* thiz, const char* text, size_t length)
{
	return_if_fail(thiz != NULL && thiz->on_comment != NULL);
	thiz->on_comment(thiz, text, length);

	return;
}

static inline void xml_builder_on_pi_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	return_if_fail(thiz != NULL && thiz->on_pi_element != NULL);
	thiz->on_pi_element(thiz, tag, attrs);

	return;
}

static inline void xml_builder_on_error(XmlBuilder* thiz, int line, int row, const char* message)
{
	return_if_fail(thiz != NULL && thiz->on_error != NULL);
	thiz->on_error(thiz, line,row, message);

	return;
}

static inline void xml_builder_destory(XmlBuilder* thiz)
{
	return_if_fail(thiz != NULL && thiz->destory != NULL);
	thiz->destory(thiz);

	return;
}

#endif /*__XMLBUILDER_H__*/