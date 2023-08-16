#include "UI.h"

UI::UI()
{
	AppMode = DESIGN;	//Design Mode is the startup mode

	//Initilaize interface colors
	DrawColor = BLACK;
	SelectColor = BLUE;
	ConnColor = RED;
	MsgColor = BLUE;
	BkGrndColor = WHITE;
	
	//Create the drawing window
	pWind = new window(width, height, wx, wy);	


	ChangeTitle("Logic Simulator Project");

	CreateDesignToolBar();	//Create the desgin toolbar
	CreateStatusBar();		//Create Status bar
}


int UI::getCompWidth() const
{
	return COMP_WIDTH;
}

int UI::getCompHeight() const
{
	return COMP_HEIGHT;
}

//======================================================================================//
//								Input Functions 										//
//======================================================================================//

void UI::GetPointClicked(int &x, int &y)
{
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}

string UI::GetSrting()
{
	//Reads a complete string from the user until the user presses "ENTER".
	//If the user presses "ESCAPE". This function should return an empty string.
	//"BACKSPACE" is also supported
	//User should see what he is typing at the status bar


	string userInput;
	char Key;
	while(1)
	{
		pWind->WaitKeyPress(Key);

		switch(Key)
		{
		case 27: //ESCAPE key is pressed
			PrintMsg("");
			return ""; //returns nothing as user has cancelled the input
		
		case 13:		//ENTER key is pressed
			return userInput;
		
		case 8:		//BackSpace is pressed
			if(userInput.size() > 0)
				userInput.resize(userInput.size() -1 );
			break;
		
		default:
			userInput+= Key;
		};
		
		PrintMsg(userInput);
	}

}

//This function reads the position where the user clicks to determine the desired action
ActionType UI::GetUserAction() const
{	
	int x,y;
	pWind->WaitMouseClick(x, y);	//Get the coordinates of the user click

	if(AppMode == DESIGN )	//application is in design mode
	{
		//[1] If user clicks on the Toolbar
		if ( y >= 0 && y < ToolBarHeight)
		{	
			//Check whick Menu item was clicked
			//==> This assumes that menu items are lined up horizontally <==
			int ClickedItemOrder = (x / ToolItemWidth);
			//Divide x coord of the point clicked by the menu item width (int division)
			//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on

			switch (ClickedItemOrder)
			{
			case ITM_RES:	return ADD_RESISTOR;
			case ITM_SWITCH:	return ADD_SWITCH;
			case ITM_LAMP:	return ADD_LAMP;
			case ITM_CONNECTION:	return ADD_CONNECTION;
			case ITM_BATTARY:	return ADD_BATTARY;
			case ITM_GROUND:	return ADD_GROUND;
			case ITM_FUSE:	return ADD_FUSE;
			case ITM_BUZZER:	return ADD_BUZZER;
			case ITM_SAVE: return SAVE;
			case ITM_LOAD: return LOAD;
			case ITM_Delete: return DEL;
			case ITM_UNDO: return UNDO;
			case ITM_REDO: return REDO;
			case ITM_EXIT:	return EXIT;	
			
			default: return DSN_TOOL;	//A click on empty place in desgin toolbar
			}
		}
	
		//[2] User clicks on the drawing area
		if ( y >= ToolBarHeight && y < height - StatusBarHeight)
		{
			return SELECT;	//user want to select/unselect a statement in the flowchart
		}
		
		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	else	//Application is in Simulation mode
	{
		return SIM_MODE;	//This should be changed after creating the compelete simulation bar 
	}

}



//======================================================================================//
//								Output Functions										//
//======================================================================================//

//////////////////////////////////////////////////////////////////////////////////
void UI::ChangeTitle(string Title) const
{
	pWind->ChangeTitle(Title);
}
//////////////////////////////////////////////////////////////////////////////////
void UI::CreateStatusBar() const
{
	pWind->SetPen(RED,3);
	pWind->DrawLine(0, height-StatusBarHeight, width, height-StatusBarHeight);
}
//////////////////////////////////////////////////////////////////////////////////
void UI::PrintMsg(string msg) const
{
	ClearStatusBar();	//Clear Status bar to print message on it
	// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = StatusBarHeight - 10;

	// Print the Message
    pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial"); 
	pWind->SetPen(MsgColor); 
	pWind->DrawString(MsgX, height - MsgY, msg);
}
//////////////////////////////////////////////////////////////////////////////////
void UI::ClearStatusBar()const
{
	// Set the Message offset from the Status Bar
	int MsgX = 25;
	int MsgY = StatusBarHeight - 10;

	//Overwrite using bachground color to erase the message
	pWind->SetPen(BkGrndColor);
	pWind->SetBrush(BkGrndColor);
	pWind->DrawRectangle(MsgX, height - MsgY, width, height);
}
//////////////////////////////////////////////////////////////////////////////////////////
//Clears the drawing (degin) area
void UI::ClearDrawingArea() const
{
	pWind->SetPen(RED, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, ToolBarHeight, width, height - StatusBarHeight);
	
}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the Design mode
void UI::CreateDesignToolBar() 
{
	AppMode = DESIGN;	//Design Mode

	//You can draw the tool bar icons in any way you want.

	//First prepare List of images for each menu item
	string MenuItemImages[ITM_DSN_CNT];
	MenuItemImages[ITM_RES] = "Photos\\Menu\\Menu_Resistor.jpg";
	MenuItemImages[ITM_SWITCH] = "Photos\\Menu\\Menu_Switch.jpg";
	MenuItemImages[ITM_LAMP] = "Photos\\Menu\\Menu_Bulb.jpg";
	MenuItemImages[ITM_CONNECTION] = "Photos\\Menu\\Menu_Connection.jpg";
	MenuItemImages[ITM_BATTARY] = "Photos\\Menu\\Menu_Battery.jpg";
	MenuItemImages[ITM_GROUND] = "Photos\\Menu\\Menu_Ground.jpg";
	MenuItemImages[ITM_FUSE] = "Photos\\Menu\\Menu_Fuse.jpg";
	MenuItemImages[ITM_BUZZER] = "Photos\\Menu\\Menu_Buzzer.jpg";
	MenuItemImages[ITM_UNDO] = "Photos\\Menu\\Menu_Undo.jpg";
	MenuItemImages[ITM_LOAD] = "Photos\\Menu\\Menu_Load.jpg";
	MenuItemImages[ITM_SAVE] = "Photos\\Menu\\Menu_Save.jpg";
	MenuItemImages[ITM_DELETE] = "Photos\\Menu\\Menu_Delete.jpg";
	MenuItemImages[ITM_EXIT] = "Photos\\Menu\\Menu_Exit.jpg";

	//TODO: Prepare image for each menu item and add it to the list

	//Draw menu item one image at a time
	for(int i=0; i<ITM_DSN_CNT; i++)
		pWind->DrawImage(MenuItemImages[i],i*ToolItemWidth,0,ToolItemWidth, ToolBarHeight);


	//Draw a line under the toolbar
	pWind->SetPen(RED,3);
	pWind->DrawLine(0, ToolBarHeight, width, ToolBarHeight);	

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the simulation mode
void UI::CreateSimulationToolBar()
{
	AppMode = SIMULATION;	//Simulation Mode

	//TODO: Write code to draw the simualtion toolbar (similar to that of design toolbar drawing)
	string MenuItemImages[ITM_CIRC_SIM];
	MenuItemImages[ITM_VOLT] = "Photos\\Menu\\Menu_Voltmeter.jpg";
	MenuItemImages[ITM_AMPARE] = "Photos\\Menu\\Menu_Ammeter.jpg";


	//Draw menu item one image at a time
	for (int i = 0; i < ITM_DSN_CNT; i++)
		pWind->DrawImage(MenuItemImages[i], i * ToolItemWidth, 0, ToolItemWidth, ToolBarHeight);


	//Draw a line under the toolbar
	pWind->SetPen(RED, 3);
	pWind->DrawLine(0, ToolBarHeight, width, ToolBarHeight);

}

//======================================================================================//
//								Components Drawing Functions							//
//======================================================================================//

void UI::DrawResistor(const GraphicsInfo &r_GfxInfo, bool selected) const
{
	string ResImage;
	if(selected)	
		ResImage ="Photos\\Comp\\Resistor_HI.jpg";	//use image of highlighted resistor
	else  
		ResImage = "Photos\\Comp\\Resistor.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(ResImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawLAMP(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string LAMPImage;
	if (selected)
		LAMPImage = "Photos\\Comp\\Pulb_HI.jpg";	//use image of highlighted resistor
	else
		LAMPImage = "Photos\\Comp\\Pulb.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(LAMPImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawSwitch(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string SwitchImage;
	if (selected)
		SwitchImage = "Photos\\Comp\\Switch_HI.jpg";	//use image of highlighted resistor
	else
		SwitchImage = "Photos\\Comp\\Switch.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(SwitchImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawBattary(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string BattaryImage;
	if (selected)
		BattaryImage = "Photos\\Comp\\Battary_HI.jpg";	//use image of highlighted resistor
	else
		BattaryImage = "Photos\\Comp\\Battary.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(BattaryImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawGround(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string GroundImage;
	if (selected)
		GroundImage = "Photos\\Comp\\Ground_HI.jpg";	//use image of highlighted resistor
	else
		GroundImage = "Photos\\Comp\\Ground.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(GroundImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawBuzzer(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string BuzzImage;
	if (selected)
		BuzzImage = "Photos\\Comp\\Buzzer_HI.jpg";	//use image of highlighted resistor
	else
		BuzzImage = "Photos\\Comp\\Buzzer.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(BuzzImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawFuse(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string FuseImage;
	if (selected)
		FuseImage = "Photos\\Comp\\Fuse_HI.jpg";	//use image of highlighted resistor
	else
		FuseImage = "Photos\\Comp\\Fuse.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(FuseImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawExit(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string FuseImage;
	if (selected)
		FuseImage = "Photos\\Comp\\Fuse_HI.jpg";	//use image of highlighted resistor
	else
		FuseImage = "Photos\\Comp\\Fuse.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(FuseImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawDelete(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string FuseImage;
	if (selected)
		FuseImage = "Photos\\Comp\\Fuse_HI.jpg";	//use image of highlighted resistor
	else
		FuseImage = "Photos\\Comp\\Fuse.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(FuseImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawUndo(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string FuseImage;
	if (selected)
		FuseImage = "Photos\\Comp\\Fuse_HI.jpg";	//use image of highlighted resistor
	else
		FuseImage = "Photos\\Comp\\Fuse.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(FuseImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

void UI::DrawRedo(const GraphicsInfo& r_GfxInfo, bool selected) const
{
	string FuseImage;
	if (selected)
		FuseImage = "Photos\\Comp\\Fuse_HI.jpg";	//use image of highlighted resistor
	else
		FuseImage = "Photos\\Comp\\Fuse.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(FuseImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}

//TODO: Add similar functions to draw all other components




void UI::DrawConnection(const GraphicsInfo &r_GfxInfo, bool selected) const
{
	//TODO: Add code to draw connection
	string ConImage;
	if (selected)
		ConImage = "Photos\\Menu\\Resistor_HI.jpg";	//use image of highlighted resistor
	else
		ConImage = "Photos\\Comp\\Resistor.jpg";	//use image of the normal resistor

	//Draw Resistor at Gfx_Info (1st corner)
	pWind->DrawImage(ConImage, r_GfxInfo.PointsList[0].x, r_GfxInfo.PointsList[0].y, COMP_WIDTH, COMP_HEIGHT);
}


UI::~UI()
{
	delete pWind;
}