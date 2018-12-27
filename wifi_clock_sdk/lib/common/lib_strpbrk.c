/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file lib_strbrk.c
*   \brief 
*   \author Montage
*/

/*=============================================================================+
| Included Files							       																					 |
+=============================================================================*/
#include <string.h>
#include <stddef.h>         

/*=============================================================================+
| Functions  						   	 	       																					 |
+=============================================================================*/
char *strpbrk( const char *s1, const char *s2 )
{
    const char *c;


    if (!*s1)
    {
        return (char *) NULL;
    } // if
    
    do
    {
        for (c = s2; *c; c++)
        {
            if (*s1 == *c)
                break;
        }
        if (*c)
            break;
        s1++;
    }
    while (*s1);
    
    if (*c == '\0')
        s1 = NULL;
    
    
    return (char *) s1;
} // __strpbrk()
