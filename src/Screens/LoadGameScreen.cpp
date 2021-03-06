#include "Screens/LoadGameScreen.h"
#include "Screens/MenuScreen.h"

using namespace std;

LoadGameScreen::LoadGameScreen(CharacterCore* core) : Screen(core) {
}

void LoadGameScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		onBack();
		return;
	}
	else if (m_loadGame) {
		setNextScreen(new LoadingScreen(m_characterCore));
		return;
	}
	updateObjects(GameObjectType::_Window, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	updateTooltipText(frameTime);
	if (!getObjects(GameObjectType::_Form)->empty()) return;
	if (m_saveGameWindow->isChosen()) {
		onLoadSaveGame();
	}
}

void LoadGameScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderTooltipText(renderTarget);
	renderObjects(GameObjectType::_Window, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
}

void LoadGameScreen::execOnEnter(const Screen *previousScreen) {
	// text
	m_title = new BitmapText(g_textProvider->getText("LoadGame"));
	m_title->setCharacterSize(24);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getBounds().width) / 2.f, 25.f));

	float buttonWidth = 200.f;
	float buttonHeight = 50.f;
	float marginX = 60.f;
	float marginY = WINDOW_HEIGHT - 100.f;
	float buttonSpaceWidth = WINDOW_WIDTH - 2 * marginX;
	float buttonSpacing = (buttonSpaceWidth - 3 * buttonWidth) / 2.f;

	// add buttons
	Button* button = new Button(sf::FloatRect(marginX, marginY, buttonWidth, buttonHeight));
	button->setText("Back");
	button->setOnClick(std::bind(&LoadGameScreen::onBack, this));
	addObject(button);

	m_deleteSaveGameButton = new Button(sf::FloatRect(marginX + buttonWidth + buttonSpacing, marginY, buttonWidth, buttonHeight));
	m_deleteSaveGameButton->setText("Delete");
	m_deleteSaveGameButton->setOnClick(std::bind(&LoadGameScreen::onDeleteSaveGame, this));
	addObject(m_deleteSaveGameButton);

	m_loadSaveGameButton = new Button(sf::FloatRect(marginX + 2 * (buttonWidth + buttonSpacing), marginY, buttonWidth, buttonHeight));
	m_loadSaveGameButton->setText("Load");
	m_loadSaveGameButton->setOnClick(std::bind(&LoadGameScreen::onLoadSaveGame, this));
	addObject(m_loadSaveGameButton);

	// savegame window
	m_saveGameWindow = new SaveGameWindow();
	setAllButtonsEnabled(true);
	addObject(m_saveGameWindow);
}

void LoadGameScreen::execOnExit(const Screen *nextScreen) {
	delete m_title;
	delete m_newCharacterCore;
}

void LoadGameScreen::setAllButtonsEnabled(bool value) {
	Screen::setAllButtonsEnabled(value);
	bool empty = m_saveGameWindow->getChosenFilename().empty();
	m_loadSaveGameButton->setEnabled(value && !empty);
	m_deleteSaveGameButton->setEnabled(value && !empty);
	m_saveGameWindow->setEnabled(value);
}

// <<< agents for yes or no form >>>

void LoadGameScreen::onNo() {
	m_yesOrNoForm = nullptr;
	delete m_newCharacterCore;
	m_newCharacterCore = nullptr;
	setAllButtonsEnabled(true);
}

void LoadGameScreen::onLoadGame() {
	m_yesOrNoForm = nullptr;
	delete m_characterCore;
	m_characterCore = m_newCharacterCore;
	m_newCharacterCore = nullptr;
	m_loadGame = true;
}

void LoadGameScreen::onYesDeleteSaveGame() {
	m_yesOrNoForm = nullptr;
	if (remove(m_saveGameWindow->getChosenFilename().c_str()) == 0) {
		setTooltipText("SavegameDeleted", COLOR_LIGHT_PURPLE, true);
	}
	else {
		g_logger->logError("SaveGameScreen", "Savegame could not be deleted");
		setTooltipText("OperationFailed", COLOR_BAD, true);
	}
	m_saveGameWindow->reload();
	setAllButtonsEnabled(true);
}

void LoadGameScreen::onDeleteSaveGame() {
	m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
	m_yesOrNoForm->setMessage("QuestionDeleteSaveGame");
	m_yesOrNoForm->setOnNoClicked(std::bind(&LoadGameScreen::onNo, this));
	m_yesOrNoForm->setOnYesClicked(std::bind(&LoadGameScreen::onYesDeleteSaveGame, this));
	addObject(m_yesOrNoForm);
	setAllButtonsEnabled(false);
}

void LoadGameScreen::onLoadSaveGame() {
	if (m_characterCore == nullptr) {
		// load a savegame
		m_characterCore = new CharacterCore();
		if (!(m_characterCore->load(m_saveGameWindow->getChosenFilename()))) {
			string errormsg = string(m_saveGameWindow->getChosenFilename()) + ": save file corrupted!";
			g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		}
		setNextScreen(new LoadingScreen(m_characterCore));
		return;
	}
	else {
		m_newCharacterCore = new CharacterCore();
		if (!(m_newCharacterCore->load(m_saveGameWindow->getChosenFilename()))) {
			string errormsg = string(m_saveGameWindow->getChosenFilename()) + ": save file corrupted!";
			g_resourceManager->setError(ErrorID::Error_dataCorrupted, errormsg);
		}
		m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
		m_yesOrNoForm->setMessage("QuestionLoadGame");
		m_yesOrNoForm->setOnNoClicked(std::bind(&LoadGameScreen::onNo, this));
		m_yesOrNoForm->setOnYesClicked(std::bind(&LoadGameScreen::onLoadGame, this));
		addObject(m_yesOrNoForm);
		setAllButtonsEnabled(false);
	}
}

void LoadGameScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}