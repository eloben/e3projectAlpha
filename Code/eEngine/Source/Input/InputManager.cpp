/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Engine

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------*/

// Create 14-Aug-2014 by Elías Lozada-Benavente
// Original 04-Dec-2011 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file InputManager.cpp
This file defines the InputManager class.
*/

#include <EnginePch.h>
#include "InputManager.h"

using namespace E;

/*----------------------------------------------------------------------------------------------------------------------
Public initialization / finalization
------------------------------------------------------------------------------------------------------------------------*/

Input::InputManager::InputManager()
  : mKeys()
  , mLastKeys() {}

/*----------------------------------------------------------------------------------------------------------------------
IInputManager Accessors
------------------------------------------------------------------------------------------------------------------------*/

const Vector2i& Input::InputManager::GetCursonPosition() const
{
	return mCursorPosition;
}

bool Input::InputManager::IsKeyDown(U8 v) const
{
	if (mKeys[v] & 0x80) return true;
	return false;
}

bool Input::InputManager::IsKeyReleased(U8 v) const
{
	if (!(mKeys[v] & 0x80) && mLastKeys[v] & 0x80)	return true;
	return false;
}

void Input::InputManager::SetCurrentViewport(Graphics::IViewportInstance viewport)
{
  mViewportDescriptor = viewport->GetDescriptor();
}

/*----------------------------------------------------------------------------------------------------------------------
Public Methods
------------------------------------------------------------------------------------------------------------------------*/

void Input::InputManager::CenterCursor()
{
	// Retrieve the windows location
	RECT rc;
	::GetWindowRect(static_cast<HWND>(mViewportDescriptor.windowHandle), &rc);

	mCursorPosition.x = rc.left + mViewportDescriptor.width / 2;  
	mCursorPosition.y = rc.top + mViewportDescriptor.height / 2; 

	::SetCursorPos(mCursorPosition.x , mCursorPosition.y);  
}

void Input::InputManager::Update()
{
	// Update last keys
  Memory::Copy(&mLastKeys[0], &mKeys[0], kKeyBufferSize);

	// Retrieve current keys
	if (!::GetKeyboardState(mKeys)) return;

	// Retrieve the cursor position
	POINT cursorPosition;
	::GetCursorPos(&cursorPosition);

	// Retrieve windows location
	RECT rc;
	::GetWindowRect(static_cast<HWND>(mViewportDescriptor.windowHandle), &rc);

	if (cursorPosition.x >= rc.left && cursorPosition.x <= rc.right && cursorPosition.y >= rc.top && cursorPosition.y <= rc.bottom)
	{
		mCursorPosition.x = cursorPosition.x;
		mCursorPosition.y = cursorPosition.y;
	}
}
