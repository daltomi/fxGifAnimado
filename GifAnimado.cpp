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

#include "GifAnimado.hpp"

/* extern Log.hpp */
FXApp * gApp = nullptr;


int main (int theArgc, char * theArgv [])
{
	if (theArgc <= 1)
	{
		Log(eLogIds::LOG_ERROR, "Error: Faltan argumentos.\nEjemplo: GifAnimado *.png");
		return EXIT_FAILURE;
	}
	
	FXApp aApp ("GifAnimado", "GifAnimado");
	
	gApp = & aApp;
	
	aApp.init (theArgc, theArgv);
	
	SetDefaultColors (aApp);
	
	MainWindow * aWindow = new MainWindow (& aApp);
	
	aWindow->SetListOfImages (ParseCmdLine (theArgc, theArgv));
	
	aApp.addSignal (SIGINT, aWindow, MainWindow::ID_QUIT);
	
	aApp.create ();
	
	return aApp.run ();
}

void SetDefaultColors (FXApp & theApp)
{
    // "Desert FOX" - ControlPanel
    theApp.setBaseColor		(FXRGB (214, 205, 187));
    theApp.setBorderColor 	(FXColors::Black);
    theApp.setBackColor 	(FXColors::Gray100);
    theApp.setForeColor 	(FXColors::Black);
    theApp.setHiliteColor 	(FXRGB (242, 240, 240));
    theApp.setShadowColor 	(FXRGB (143, 137, 125));
    theApp.setSelforeColor 	(FXColors::Gray100);
    theApp.setSelbackColor 	(FXRGB (128,0,0));
    theApp.setTipforeColor 	(FXColors::Black);
    theApp.setTipbackColor 	(FXRGB (255, 255, 225));
    theApp.setSelMenuTextColor 	(FXColors::Gray100);
    theApp.setSelMenuBackColor 	(FXRGB (128,0,0));
}


FXString & ParseCmdLine (FXint theArgc, FXchar * theArgv [])
{
	static FXString aCmdLine;

	for (int aIndex = 1; aIndex < theArgc; ++aIndex)
	{
	    aCmdLine += (theArgv [aIndex]);
	    aCmdLine += " ";
	}

    return aCmdLine;
}
