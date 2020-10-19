/*
   Copyright (c) 2016, daltomi <daltomi@disroot.org>

   This file is part of GifAnimado.

    GifAnimado is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GifAnimado is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GifAnimado.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FileScript.hpp"


char * ParseScript (va_list theVarg, char const * theTemplate, ...)
{
    ssize_t aSize = GetSizeArgs (theVarg, theTemplate);

    CheckIf(aSize > 0, return NULL, eLogIds::LOG_ERROR, "GetSizeArgs <= 0");

    char * aStr =  (char *) malloc (aSize);

    CheckIf(NULL != aStr, return NULL, eLogIds::LOG_ERROR, "Malloc failed");

    aSize = vsnprintf (aStr, aSize, theTemplate, theVarg);

    CheckIf(aSize > 0, free (aStr); aStr = NULL,eLogIds::LOG_ERROR, "aSize <= 0");

    return aStr;
}


int WriteFileScript (char * theId, char const * theTemplate, ...)
{
    va_list aVarg;

    va_start (aVarg, theTemplate);

    char * aScript  = ParseScript (aVarg, theTemplate);

    va_end (aVarg);

    CheckIf (NULL != aScript, return -1, eLogIds::LOG_ERROR, "aScript == NULL");

    MakeTempFile (theId);

    WriteFile (theId, aScript);

    free (aScript);

    return 0;
}
