#include "XmlBuilder.h"

//Builder接口的具体实现 implementation
//打印出传递 给builder的数据

typedef struct _PrivInfo
{
	FILE* fp;
}PrivInfo;


static void xml_builder_dump_on_start_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	int i = 0;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "<%s", tag);

	for (i = 0; attrs != NULL&& attrs[i] != NULL&&attrs[i+1]!= NULL ; i+=2)
	{
		fprintf(priv->fp, "%s=\"%s\"", attrs[i], attrs[i + 1]);
	}
	fprintf(priv->fp, ">");
	return;
}

static inline void xml_builder_dump_on_end_element(XmlBuilder* thiz, const char* tag)
{


	return;
}

static inline void xml_builder_dump_on_text(XmlBuilder* thiz, const char* text, size_t length)
{

	return;
}

static inline void xml_builder_dump_on_comment(XmlBuilder* thiz, const char* text, size_t length)
{


	return;
}

static inline void xml_builder_dump_on_pi_element(XmlBuilder* thiz, const char* tag, const char** attrs)
{
	int i = 0;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	fprintf(priv->fp, "<?%s", tag);

	for (i = 0; attrs != NULL && attrs[i] != NULL && attrs[i + 1] != NULL; i += 2)
	{
		fprintf(priv->fp, " %s=\"%s\"", attrs[i], attrs[i + 1]);
	}
	fprintf(priv->fp, "?>\n");

	return;
}

static inline void xml_builder_dump_on_error(XmlBuilder* thiz, int line, int row, const char* message)
{


	return;
}

static inline void xml_builder_dump_destory(XmlBuilder* thiz)
{


	return;
}

//初始化Builder接口
XmlBuilder* xml_builder_dump_create(FILE* fp)
{
	XmlBuilder* thiz = (XmlBuilder*)calloc(1,sizeof(XmlBuilder));

	if (thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
			
		thiz->on_start_element = xml_builder_dump_on_start_element;
		thiz->on_end_element = xml_builder_dump_on_end_element;
		thiz->on_text = xml_builder_dump_on_text;
		thiz->on_pi_element = xml_builder_dump_on_pi_element;
		thiz->on_comment = xml_builder_dump_on_comment;
		thiz->on_error = xml_builder_dump_on_error;
		thiz->destory = xml_builder_dump_destory;
	
		priv->fp = (fp != NULL ? fp : stdout);
	}

	return thiz;
}