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
	XmlBuilderOnStartElementFunc on_start_element; //������������һ����ʼ��ǩʱ������
	XmlBuilderOnEndElementFunc on_end_element; //������������һ��������ǩʱ������
	XmlBuilderOnTextFunc on_text;//������������һ���ı�ʱ������
	XmlBuilderOnCommentFunc on_comment;//������������һ��ע��ʱ������
	XmlBuilderOnPiElementFunc on_pi_element;//������������һ������ָ��ʱ������
	XmlBuilderOnErrorFunc on_error;//��������������ʱ������
	XmlBuilderDestroyFunc destory;//��������Builder����

	char priv[1];
};

/* Builder �ӿڷ�װ */
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