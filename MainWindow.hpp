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

#pragma once


#include <fx.h>
#include <limits.h>

#include "Resources/Resources.h"
#include "Scripts/Scripts.hpp"
#include "Log.hpp"
#include "Files.hpp"
#include "FileScript.hpp"
#include "Shell.hpp"

/* Los valores máximo y mínimo deberán corresponder
    a valores permitidos por el programa 'convert (ImageMagick)'
    pero no encontré documentación al respecto.
*/
#define LOOP_MIN 0
#define LOOP_MAX 100
#define LOOP_VAL 0	// Implica infinito.
#define LOOP_TIP "Veces que debe repertir la secuencia animada.\n" \
		 "0 (cero) implica infinitamente.\nMin: %d, Max: %d"


#define DELAY_MIN 10
#define DELAY_MAX 1000
#define DELAY_VAL 100	// En ms.
#define DELAY_TIP "Tiempo de retraso de cada cuadro de imágen.\n" \
		    "Medido en milisegundos.\nMin: %d, Max: %d"


extern FXApp * gApp;


class MainWindow : public FXMainWindow
{
	FXDECLARE(MainWindow);
	
	public:
		
		MainWindow (FXApp * theApp);
		virtual ~MainWindow ();
		
		long OnQuit 	(FXObject * theObj, FXSelector theSelector, void * theData);
		long OnPipe 	(FXObject * theObj, FXSelector theSelector, void * theData);
		long OnExecute 	(FXObject * theObj, FXSelector theSelector, void * theData);
//~ 		long OnCancel 	(FXObject * theObj, FXSelector theSelector, void * theData);
		long OnKeyPress (FXObject * theObj, FXSelector theSelector, void * theData);
		
		inline void NewIcon (unsigned char const * theData, FXPNGIcon * & theIcon)
		{
		    theIcon = new FXPNGIcon (getApp(), theData);
		}
		
		
		inline void NewIcon (unsigned char const * theData, FXGIFIcon * & theIcon)
		{
		    theIcon = new FXGIFIcon (getApp(), theData);
		}
				
		
		void SetListOfImages (FXString & theImages)
		{
			mListOfImages = theImages;
		}
		
		enum {
			ID_TITLE = FXMainWindow::ID_LAST,
			ID_QUIT,
			ID_EXEC,
			ID_FILE_NAME,
			ID_PIPE_EVENT
		};
		
		
		
	protected:
		
		MainWindow ();
		virtual void create ();
			
	private:
		
		FXString  	mGifName;
		FXuint		mLoop;
		FXuint		mDelay;
		FXString 	mListOfImages;
//~ 		static FXbool  * 	mIsValidToExecute; // shared
		FXbool		mIsValidToExecute;

		
		FXDataTarget	mLoopTarget;
		FXDataTarget	mDelayTarget;
		
		FXGIFIcon	* mIconOk;
		FXGIFIcon	* mIconCancel;
		
		FXFont * MakeFont(const FXival theSize, const FXival theWeight);
		FXFont * MakeFont(const FXString & theFontName, FXival theSize);
		FXFont * MakeFont(const FXFontDesc & theFontDesc);
		
		void InitDefaultValues ();
		
		void BuildLayout ();
		
		void Execute ();
};
