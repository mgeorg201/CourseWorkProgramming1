/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();	

	theAreaClicked = { 0, 0 };
	// Store the textures





	textureName = { "space", "energy", "ship","enemy","theBackground", "OpeningScreen", "ClosingScreen", "HScoreScreen"};
	texturesToUse = { "Images/Sprites/space64x64.png", "Images/Sprites/energy64x64.png", "Images/Sprites/shipGreen64x64.png", "Images/Sprites/shipRed64x64.png","Images/Bkg/Bkgnd.png", "Images/Bkg/OpeningScreenF.png", "Images/Bkg/ClosingScreenF.jpg","Images/Bkg/BkgndHS.jpg" };
	
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	tempTextTexture = theTextureMgr->getTexture("space");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 228, 213, 238, 255 };
	// Store the textures
	
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn", "hs_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png", "Images/Buttons/button_hscore.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 }, { 400, 500 } };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;
	// Create textures for Game Dialogue (text)







	fontList = { "pirate", "skeleton", "amazon", "spaceage"};
	fontsToUse = { "Fonts/BlackPearl.ttf", "Fonts/SkeletonCloset.ttf", "Fonts/KleinsAmazon.ttf", "Fonts/spaceage.ttf" };

	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		if (fonts == 0)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
		}
		else
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
		}
	}
	// Create text Textures







	gameTextNames = { "TitleTxt", "CollectTxt", "InstructTxt", "ThanksTxt", "SeeYouTxt","BottleCount","HSTable","HScore"};
	gameTextList = { "Space WA(sd)R", "Collec 10 Energy to win!", "Use the arrows to navigate space.", "Thanks for playing!", "See you again soon!", "Points: ", "", "High Score"};
	
	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size()-1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("amazon")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("spaceage")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
		}
	}











	// Load game sounds
	soundList = { "theme", "click", "shiphit" };
	soundTypes = { soundType::music, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/Theme/396231__romariogrande__tentacle-wedding.mp3", "Audio/SFX/ClickOn.wav", "Audio/SFX/253903__jagadamba__alien-space-sound-03.mp3" };
	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());

	theTileMap.setMapStartXY({ 150, 100 });

	bottlesCollected = 0;
	strScore = gameTextList[5];
	strScore += to_string(bottlesCollected).c_str();
	theTextureMgr->deleteTexture("BottleCount");

	numTableItems = 0;
	theHSTable.loadFromFile("Data/HighScore1.dat");
	

	theHighScoreTable = gameTextList[6];
	
	theHSTableSize = theHSTable.getTableSize();
	highScoreTextures = { "score1","score2","score3","score4","score5","score6","score7","score8","score9","score10" };









	for (int item = 0; item < theHSTableSize; item++)
	{
		string entry = "";
		entry += theHSTable.getItem(item).Name + " " + to_string(theHSTable.getItem(item).score);
		theTextureMgr->addTexture(highScoreTextures[item], theFontMgr->getFont("spaceage")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
 	}








	gameOver = false;

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 350, 125 , tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("CollectTxt");
		pos = { 175, 275, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("InstructTxt");
		pos = { 50, 350, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 475});
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("hs_btn")->setSpritePos({ 435,525 });
		theButtonMgr->getBtn("hs_btn")->render(theRenderer, &theButtonMgr->getBtn("hs_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("hs_btn")->getSpritePos(), theButtonMgr->getBtn("hs_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 450, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	 
	}
	break;
	case gameState::playing:
	{
		if (resetScore)
		{
			theTextureMgr->deleteTexture("BottleCount");
			strScore = gameTextList[5];
			strScore += to_string(bottlesCollected).c_str();
			theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("amazon")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
			resetScore = false;
		}

		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("amazon")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("BottleCount");
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour);
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 850, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		if (bottlesCollected == 10) {
			theGameState = gameState::end;
		}
	}
	break;
	case gameState::end:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("ClosingScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("ClosingScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		if (bottlesCollected == 10) {
			theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("amazon")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 0,188,212,255 }, { 0, 0, 0, 0 }));
			tempTextTexture = theTextureMgr->getTexture("BottleCount");
			pos = { 400, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			resetScore = true;
			 
		}
	}
	break;
	case gameState::highscore:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("HScore");
		pos = { 220, 125, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		pos = { 220, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };

		for (int item = 0; item < theHSTableSize; item++)
		{
			tempTextTexture = theTextureMgr->getTexture(highScoreTextures[item]);
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			pos = { 220, 200 + (50 * item), tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		}

		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 900, 375 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		
		bottlesCollected = 0;

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 900, 425 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 900, 475 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// CHeck Button clicked and change state
	if (theGameState == gameState::menu || theGameState == gameState::end)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);
		 
	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::end, theAreaClicked);
	}
	if (theGameState == gameState::highscore)
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("HScoreScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("HScoreScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);

		gameOver = false;
		if (theGameState == gameState::playing && gameOver == false)
		{
			theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
			theShip.setMapPosition(spriteRandom(gen), spriteRandom(gen));
			theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
			// Lab Code goes here
			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			// Lab Code goes here
			bottlesCollected = 0;
		 
		}

		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	}

	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		gameOver = false;
		if (theGameState == gameState::playing && gameOver == false)
		{
			theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
			theShip.setMapPosition(spriteRandom(gen), spriteRandom(gen));
			theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
			// Lab Code goes here
			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			// Lab Code goes here
			bottlesCollected = 0;
		}
	}

	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	theGameState = theButtonMgr->getBtn("hs_btn")->update(theGameState, gameState::highscore, theAreaClicked);

	if (theGameState == gameState::playing)
	{
		
		// Check if ship has collided with the bottle
		if (theShip.getMapPosition() == theBottle.getMapPosition())
		{
			bottlesCollected++;
			theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
			theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			//Create Updated bottle count
			theSoundMgr->getSnd("shiphit")->play(0);
			strScore = gameTextList[5];
			strScore += to_string(bottlesCollected).c_str();
			theTextureMgr->deleteTexture("BottleCount");
		}

		// Check if Pirate has collided with the ship
		// Lab Code goes here

		if (gameOver)
		{
			theGameState = gameState::end;
		}
		
	}
	
		
	
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };






					theSoundMgr->getSnd("click")->play(0);






					//if (theGameState == gameState::playing)
					//{
					//	theTilePicker.update(theAreaClicked);
					//	if (theTilePicker.getTilePicked() > -1)
					//	{
					//		dragTile.setSpritePos(theAreaClicked);
					//		dragTile.setTexture(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1]));
					//		dragTile.setSpriteDimensions(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTWidth(), theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTHeight());
					//	}
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					//if (theGameState == gameState::playing)
					//{
					//	theAreaClicked = { event.motion.x, event.motion.y };
					//	theTileMap.update(theAreaClicked, theTilePicker.getTilePicked());
					//	theTilePicker.setTilePicked(-1);
					//}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				dragTile.setSpritePos({ event.motion.x, event.motion.y });
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().R < 9)
					{
						theShip.setShipRotation(0.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R + 1);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;

				case SDLK_UP:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().R > 0)
					{
						theShip.setShipRotation(180.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R - 1);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;
				case SDLK_RIGHT:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().C < 9)
					{
						theShip.setShipRotation(270.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C + 1, theShip.getMapPosition().R);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;

				case SDLK_LEFT:
				{
					if (theGameState == gameState::playing && theShip.getMapPosition().C > 0)
					{
						theShip.setShipRotation(90.0f);
						theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
						theShip.update(theShip.getMapPosition().C - 1, theShip.getMapPosition().R);
						theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
						// Lab Code goes here
					}
				}
				break;
				case SDLK_SPACE:
				{
				}
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

