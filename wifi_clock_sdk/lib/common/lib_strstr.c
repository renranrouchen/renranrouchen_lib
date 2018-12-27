/*=============================================================================+
|                                                                              |
| Copyright 2012                                                               |
| Montage Inc. All right reserved.                                             |
|                                                                              |
+=============================================================================*/
/*! 
*   \file lib_strstr.c
*   \brief 
*   \author Montage
*/

/*=============================================================================+
| Included Files							       																					 |
+=============================================================================*/
#include <string.h>

/*=============================================================================+
| Functions  						   	 	       																					 |
+=============================================================================*/
char * strstr(const char *searchee, const char *lookfor)
{
  if (searchee==0)
    return (char *) NULL;

  if (*searchee == 0)
    {
      if (*lookfor)
	return (char *) NULL;
      return (char *) searchee;
    }

  while (*searchee)
    {
      size_t i;
      i = 0;

      while (1)
	{
	  if (lookfor[i] == 0)
	    {
	      return (char *) searchee;
	    }

	  if (lookfor[i] != searchee[i])
	    {
	      break;
	    }
	  i++;
	}
      searchee++;
    }

  return (char *) NULL;
}
