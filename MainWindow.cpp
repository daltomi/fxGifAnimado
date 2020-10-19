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

#include "MainWindow.hpp"

FXDEFMAP(MainWindow) MainWindowMap [] = {
	FXMAPFUNC(SEL_COMMAND,	MainWindow::ID_QUIT,	MainWindow::OnQuit),
	FXMAPFUNC(SEL_SIGNAL,	MainWindow::ID_QUIT,	MainWindow::OnQuit),
	FXMAPFUNC(SEL_CLOSE,	MainWindow::ID_QUIT,	MainWindow::OnQuit),
	FXMAPFUNC( SEL_UPDATE,	MainWindow::ID_EXEC, MainWindow::OnExecute),
	FXMAPFUNC(SEL_COMMAND,	MainWindow::ID_EXEC, MainWindow::OnExecute),
	FXMAPFUNC(SEL_CHANGED,	MainWindow::ID_FILE_NAME,	MainWindow::OnKeyPress),
	FXMAPFUNC(SEL_IO_READ,	MainWindow::ID_PIPE_EVENT,  MainWindow::OnPipe)
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))



//~ FXbool * MainWindow::mIsValidToExecute;


MainWindow::MainWindow ()
{ }


MainWindow::MainWindow (FXApp * theApp) :
		FXMainWindow (theApp, "GifAnimado", nullptr, nullptr,
		FX::DECOR_ALL,
		0, 0, 320, 200)

{
	InitDefaultValues ();

	mLoopTarget.connect (mLoop);
	mDelayTarget.connect (mDelay);

	new FXToolTip (getApp ());

	getApp ()->setToolTipTime (4000000000);

	setTarget (this);

	setSelector (ID_TITLE);


	NewIcon (aceptar_gif, mIconOk);
	NewIcon (cancelar_gif, mIconCancel);


	BuildLayout ();
}


void MainWindow::InitDefaultValues ()
{
	MapSharedMemory ();

	//XXX: para que? intentar sin esto
//~ 	MapSharedCustomMemory (FXbool*, mIsValidToExecute, sizeof(FXbool*));
//~ 	* mIsValidToExecute = true;

	FXString const gifName = "GifAnimado_";
	FXString const extension = ".gif";
	mIsValidToExecute = true;
	mGifName += gifName;
	mGifName +=FXSystem::localTime("%H:%M:%S");
	mGifName += extension;
	mLoop = LOOP_VAL;
	mDelay = DELAY_VAL;
}


void MainWindow::BuildLayout ()
{
	FXVerticalFrame		* aTopFrame;
	FXHorizontalFrame	* aHFrame;
	FXGroupBox		* aGroup;
	FXFont			* aFontSize8;

	aFontSize8 = MakeFont ("Sans", 8);

	getApp ()->setNormalFont (aFontSize8);

	aTopFrame = new FXVerticalFrame (this, LAYOUT_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y ,0,0,0,0,15,15,5,5,0,5);

	{ // Archivo

		FXTextField	* aText;

		aGroup = new FXGroupBox (aTopFrame, "Archivo", FRAME_THICK | LAYOUT_FILL_X);

		aHFrame = new FXHorizontalFrame (aGroup, LAYOUT_FILL_X, 0,0,0,0,20);

		new FXLabel (aHFrame, "Nombre:", NULL, LAYOUT_LEFT);

		aText= new FXTextField (aHFrame, 1,  this, ID_FILE_NAME, FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_CENTER_Y| LAYOUT_FILL_COLUMN);
		aText->setJustify (JUSTIFY_CENTER_X);
		aText->setText (mGifName);

	} // Archivo


	{ // Opciones

		FXString aTitle;
		FXString aTip;
		FXMatrix * aMatrix;
		FXSpinner * aSpinner;

		aGroup = new FXGroupBox (aTopFrame, "Opciones", FRAME_THICK | LAYOUT_FILL_X);

		aMatrix = new FXMatrix(aGroup,3, MATRIX_BY_COLUMNS | LAYOUT_FILL_X | LAYOUT_FILL_Y,0,0,0,0,20,0,0,0,0,10);

		aTip.format (DELAY_TIP, DELAY_MIN, DELAY_MAX);
		aTitle = "Retraso:\t" + aTip;

		new FXLabel (aMatrix, aTitle);

		aSpinner = new FXSpinner (aMatrix, 4,  & mDelayTarget, FXDataTarget::ID_VALUE, FRAME_SUNKEN | FRAME_THICK | SPIN_NOMAX);
		aSpinner->setRange (DELAY_MIN, DELAY_MAX);

		new FXLabel (aMatrix, "ms.");

		aTip.format (LOOP_TIP, LOOP_MIN, LOOP_MAX);
		aTitle = "Repetir:\t" + aTip;

		new FXLabel (aMatrix, aTitle);

		aSpinner = new FXSpinner (aMatrix, 4,  & mLoopTarget, FXDataTarget::ID_VALUE,  FRAME_SUNKEN | FRAME_THICK | SPIN_NOMAX);
		aSpinner->setRange (LOOP_MIN, LOOP_MAX);

		new FXLabel (aMatrix, "veces.");

	} // Opciones

	{ // Botones

		FXButton * aButton;

		aHFrame = new FXHorizontalFrame (aTopFrame, PACK_UNIFORM_WIDTH | LAYOUT_CENTER_X | LAYOUT_FILL_Y,0,0,0,0,5,5,10,5,20,5);

		aButton = new FXButton (aHFrame, "&Aceptar", mIconOk, this, ID_EXEC, TEXT_AFTER_ICON | FRAME_RAISED|FRAME_THICK | LAYOUT_CENTER_X);
		aButton->show ();

		aButton = new FXButton (aHFrame, "&Cancelar", mIconCancel, this, ID_QUIT, TEXT_AFTER_ICON | FRAME_RAISED|FRAME_THICK | LAYOUT_CENTER_X);
		aButton->show ();

//~ 		aButton = new FXButton (aHFrame, "&Acerca de...", nullptr, nullptr, 0,FRAME_RAISED | FRAME_THICK | LAYOUT_CENTER_X);
//~ 		aButton->show ();

	} // Botones
}


MainWindow::~MainWindow ()
{
    UnmapSharedMemory ();
//~ 	UnmapSharedCustomMemory (FXbool*, mIsValidToExecute, sizeof(FXbool*));
    delete mIconCancel;
    delete mIconOk;
}


void MainWindow::create ()
{
    FXMainWindow::create ();

    mIconCancel->create ();
    mIconOk->create ();

    show (PLACEMENT_SCREEN);
}


long MainWindow::OnQuit (FXObject * theObj, FXSelector theSelector, void * theData)
{
    gApp->exit (0);

    return 1;
}


long MainWindow::OnPipe (FXObject * theObj, FXSelector theSelector, void * theData)
{
    ProcessProtocolResponse aResponse;

    bzero (gBuffer, gBufferSize);

    ssize_t aLen = FifoRead ();

    CheckIf((aLen > 0 && aLen < SSIZE_MAX), goto END, eLogIds::LOG_ERROR, "Error OnPipeEvent, falló read.");

    {
		gBuffer [aLen] = '\0';

		ShellFormat const aFormat (gBuffer);

		aResponse = ProcessProtocol (aFormat);

		if (aResponse.mWait == true)
		{
			// No cerrar la comunicación todavía.
			return 1;
		}
    }

END:

    mIsValidToExecute = true;

    gApp->removeInput (*gFifoIn, INPUT_READ);

    CloseAllFiles ();

    if (aResponse.mQuit == true)
    {
	OnQuit (NULL, 0, NULL);
    }

    return 1;
}


long MainWindow::OnExecute (FXObject * theObj, FXSelector theSelector, void * theData)
{
    switch (FXSELTYPE (theSelector))
    {
	case SEL_UPDATE:
	{
	    theObj->handle (this,  mIsValidToExecute ?
		    FXSEL(SEL_COMMAND,ID_ENABLE) : FXSEL(SEL_COMMAND,ID_DISABLE), NULL);

	    break;
	}
	case SEL_COMMAND:
	{
	    Execute ();
	    break;
	}
    };

    return 1;
}


void MainWindow::Execute ()
{
    mIsValidToExecute = false;

    char * aCmd;
    char * aDelay;
    char * aLoop;
    char * aFifoName 	= strdup (gFifoName);
    char * aScript	= strdup (gifanimado_id);

    asprintf (& aDelay, "%d", mDelay);
    asprintf (& aLoop, "%d", mLoop);

    MakeTempFile (aFifoName);

    MakeFifo (aFifoName);

    WriteFileScript (aScript,
		    gifanimado_sh,
		    gShellReply [eShellReply::YES].mMsg,
		    gShellReply [eShellReply::NO].mMsg,
		    gShellProto [eShellProto::PROTO_ERR_Q].mMsg,
		    gShellProto [eShellProto::PROTO_ERR_I].mMsg,
		    gShellProto [eShellProto::PROTO_ERR_W].mMsg,
		    gShellProto [eShellProto::PROTO_INFO].mMsg,
		    gShellProto [eShellProto::PROTO_WARN].mMsg,
		    gShellProto [eShellProto::PROTO_QUIT].mMsg,
		    gShellToken,
		    aFifoName,
		    aDelay,
		    aLoop,
		    mGifName.text (),
		    mListOfImages.text () ,
		    (void*)0);

    asprintf (& aCmd, "/usr/bin/bash %s", aScript);

    *gFifoIn = FifoOpenForRead (aFifoName);

    *gFifoOut = FifoOpenForWrite (aFifoName);

    gFilePipe = PipeOpen (aCmd);

    free (aScript);
    free (aFifoName);
    free (aDelay);
    free (aLoop);
    free (aCmd);

    gApp->addInput(this, ID_PIPE_EVENT, *gFifoIn, INPUT_READ, NULL);
}


//~ long MainWindow::OnCancel (FXObject * theObj, FXSelector theSelector, void * theData)
//~ {
//~ 	return 1;
//~ }


// Crea una fuente basada en el tipo de fuente normal.
FXFont * MainWindow::MakeFont (FXival const theSize, FXival const theWeight)
{
    FXFontDesc aFontDesc = getApp ()->getNormalFont ()->getFontDesc ();
    aFontDesc.size = (aFontDesc.size * theSize) / 10;
    aFontDesc.weight = theWeight;
    return new FXFont (getApp (), aFontDesc);
}


FXFont * MainWindow::MakeFont (FXString const & theFontName, FXival theSize)
{
    FXFont * aFont  = new FXFont (getApp(), theFontName, theSize, 0);
    aFont->create ();
    return aFont;
}


FXFont * MainWindow::MakeFont (FXFontDesc const & theFontDesc)
{
    FXFont * aFont = new FXFont (getApp(), theFontDesc);
    aFont->create ();
    return aFont;
}


long MainWindow::OnKeyPress (FXObject * theObj, FXSelector theSelector, void * theData)
{
	FXTextField * const aTextField = (FXTextField *) theObj;

	mGifName = aTextField->getText ();

	mIsValidToExecute = false;

	if (mGifName.right (4) != ".gif")
	{
	    mGifName.append (".gif");

	    aTextField->setText (mGifName);
	}

	if (mGifName.length () > 4 and (not mGifName.before (' ').empty ()) )
	{
	    mIsValidToExecute = true;
	}

	return 1;
}
