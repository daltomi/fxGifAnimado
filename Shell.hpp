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

#include <fx.h>

#include "Log.hpp"
#include "Files.hpp"


static const char * const gShellToken = "@\n";

enum eShellProto {
    PROTO_ERR_Q=0,
    PROTO_ERR_I,
    PROTO_ERR_W,
    PROTO_INFO,
    PROTO_WARN,
    PROTO_QUIT,
    PROTO_LEN
};

enum eShellReply {
    YES=0,
    NO,
    UNKNOWN,
    REPLY_LEN
};



template <typename T>
struct ShellType
{
    T mID;
    char const * mMsg;
};


typedef ShellType<eShellProto> ShellProto;
typedef ShellType<eShellReply> ShellReply;



static ShellProto gShellProto [eShellProto::PROTO_LEN] = {
    {
	PROTO_ERR_Q,
	"ERR_Q"
    },
    {
	PROTO_ERR_I,
	"ERR_I"
    },
    {
	PROTO_ERR_W,
	"ERR_W"
    },
    {
	PROTO_INFO,
	"INFO"
    },
    {
	PROTO_WARN,
	"WARN"
    },
    {
	PROTO_QUIT,
	"QUIT"
    }
};


static ShellReply gShellReply [eShellReply::REPLY_LEN] = {
    {
	eShellReply::YES,
	"Y"
    },
    {
	eShellReply::NO,
	"N"
    },
    {
	eShellReply::UNKNOWN,
	"?"
    }
};


struct ShellFormat {
    char * mProtocol;
    char * mMessageA;
    char * mMessageB;

    explicit ShellFormat (char * theData) {
		mProtocol  	= strtok (theData, gShellToken);
		mMessageA 	= strtok (NULL, gShellToken);
		mMessageB  	= strtok (NULL, gShellToken);
    }
};


static bool ProcessReply (FXuint theMsgId)
{
    char const * aReply = nullptr;

    bool aReturn = false;

    switch (theMsgId)
    {
	case MBOX_CLICKED_YES:
	case MBOX_CLICKED_OK:
	    aReply  = gShellReply [eShellReply::YES].mMsg;
	    aReturn = true;
	    break;

	case MBOX_CLICKED_NO:
	case MBOX_CLICKED_CANCEL:
	    aReply  = gShellReply [eShellReply::NO].mMsg;
	    aReturn = false;
	    break;
    }

    FifoWrite (aReply,  strlen(aReply));

    return aReturn;
}


struct ProcessProtocolResponse {
    bool mWait;
    bool mQuit;
};


static ProcessProtocolResponse ProcessErrorQuestion (ShellFormat const & theFormat)
{
    FXuint aReply = FX::FXMessageBox::question (gApp, MBOX_YES_NO, theFormat.mMessageA, theFormat.mMessageB);

    return ProcessProtocolResponse {.mWait = ProcessReply (aReply), .mQuit = false };
}


static ProcessProtocolResponse ProcessErrorInformation (ShellFormat const & theFormat)
{
    FXuint aReply = FX::FXMessageBox::information (gApp, MBOX_YES_NO, theFormat.mMessageA, theFormat.mMessageB);

    return ProcessProtocolResponse {.mWait = ProcessReply (aReply), .mQuit = false };
}


static ProcessProtocolResponse ProcessErrorWarning (ShellFormat const & theFormat)
{
    FXuint aReply = FX::FXMessageBox::warning (gApp, MBOX_YES_NO, theFormat.mMessageA, theFormat.mMessageB);

    return ProcessProtocolResponse {.mWait = ProcessReply (aReply), .mQuit = false };
}


static ProcessProtocolResponse ProcessInformation (ShellFormat const & theFormat)
{
    FXuint aReply = FX::FXMessageBox::information (gApp, MBOX_OK, theFormat.mMessageA, theFormat.mMessageB);

    return ProcessProtocolResponse { .mWait = ProcessReply (aReply), .mQuit = false };
}


static ProcessProtocolResponse ProcessWarning (ShellFormat const & theFormat)
{
    FXuint aReply = FX::FXMessageBox::warning (gApp, MBOX_OK, theFormat.mMessageA, theFormat.mMessageB);

    return ProcessProtocolResponse { .mWait = ProcessReply (aReply), .mQuit = false };
}


static ProcessProtocolResponse ProcessQuit (ShellFormat const & theFormat)
{
    bool aReturn = false;

    if ( 0 == strcasecmp (theFormat.mMessageA, gShellReply [eShellReply::YES].mMsg))
    {
		aReturn = true;
    }

    return ProcessProtocolResponse { .mWait = false, .mQuit = aReturn };
}


static ProcessProtocolResponse const ProcessProtocol (ShellFormat const & theFormat )
{
    ProcessProtocolResponse aResponse {false, false};

    if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_ERR_Q].mMsg))
    {
		aResponse = ProcessErrorQuestion (theFormat);
    }
    else if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_ERR_I].mMsg))
    {
		aResponse = ProcessErrorInformation (theFormat);
    }
    else if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_ERR_W].mMsg))
    {
		aResponse = ProcessErrorWarning (theFormat);
    }
    else if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_INFO].mMsg))
    {
		aResponse = ProcessInformation (theFormat);
    }
    else if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_WARN].mMsg))
    {
		aResponse = ProcessWarning (theFormat);
    }
    else if ( 0 == strcasecmp (theFormat.mProtocol, gShellProto[eShellProto::PROTO_QUIT].mMsg))
    {
		aResponse = ProcessQuit (theFormat);
    }
    else
    {
		Log(eLogIds::LOG_ERROR, "ERROR: ProcessProtocol %s, A=%s, B=%S",
			theFormat.mProtocol,
			theFormat.mMessageA,
			theFormat.mMessageB );
    }
			
    return aResponse;
}

