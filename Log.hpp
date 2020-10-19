/*
   Copyright (c) 2016, daltomi <daltomi@disroot.org>

   This file is part of NiNo.

    NiNo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NiNo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NiNo.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdarg.h>
#include <fx.h>

extern FXApp * gApp;

static ssize_t GetSizeArgs (va_list theVarg, char const * theTemplate, ...)
{
    ssize_t aSize = 0;
    va_list aVarg;

    va_copy (aVarg, theVarg);
    aSize = vsnprintf (NULL, aSize, theTemplate, aVarg);
    va_end (aVarg);

    return aSize + 1; // +1 = '\0'
}


enum class eLogIds {
    SHOW_ERROR,
    SHOW_INFO,
    LOG_ERROR,
    LOG_INFO
};



static void Log(eLogIds theLogId, char const * theTemplate, ...)
{
    va_list aVarg;

    va_start (aVarg, theTemplate);

    int const aSize = GetSizeArgs (aVarg, theTemplate);

    char * aStr = (char *) malloc (aSize);

    vsnprintf (aStr, aSize, theTemplate, aVarg);

    va_end (aVarg);

    switch (theLogId)
    {
	case eLogIds::SHOW_ERROR:
	    FXMessageBox::error (gApp, MBOX_OK, "Nino - Error", aStr);
	    break;

	case eLogIds::LOG_ERROR:
	    fprintf(stderr,"\n%s\n\n", aStr);
	    break;

	default:
	    fprintf(stderr,"Log (), Error no soportado.");
    };

    free (aStr);
}


#define CheckIf(theValue, theAction, theMsgId, ...) ({ 	\
    if (! (theValue)) {              					\
		Log(theMsgId, __VA_ARGS__);         			\
		theAction;                   					\
    }                             						\
})

