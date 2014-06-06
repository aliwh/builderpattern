#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//×Ô¶¯²âÊÔºê
#define return_if_fail(p)          if(!(p)) { printf("%s:%d "#p" failed.\n", __FUNCTION__, __LINE__); return;}
#define return_val_if_fail(p, val) if(!(p)) { printf("%s:%d "#p" failed.\n", __FUNCTION__, __LINE__); return (val);}

#endif /* _TYPEDEF_H */