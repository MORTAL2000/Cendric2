#include "ScreenManager.h"

ScreenManager::ScreenManager(Screen *initialScreen) : m_isErrorScreen(false) {
	m_currentScreen = initialScreen;
	m_currentScreen->onEnter(nullptr);
}

ScreenManager::~ScreenManager() {
	if (m_currentScreen != nullptr) {
		m_currentScreen->onExit(nullptr);
		delete m_currentScreen->getCharacterCore();
		delete m_currentScreen;
	}
}

void ScreenManager::update(const sf::Time& frameTime) {
	m_currentScreen->update(frameTime);
	if (Screen* nextScreen = m_currentScreen->getNextScreen()) {
		m_currentScreen->onExit(nextScreen);
		nextScreen->onEnter(m_currentScreen);
		delete m_currentScreen;
		m_currentScreen = nextScreen;
	}
}

void ScreenManager::setErrorScreen() {
	if (m_isErrorScreen) return;
	m_currentScreen->onExit(nullptr);
	Screen* nextScreen = new ErrorScreen(m_currentScreen->getCharacterCore());
	nextScreen->onEnter(m_currentScreen);
	delete m_currentScreen;
	m_currentScreen = nextScreen;
	m_isErrorScreen = true;
}

void ScreenManager::render(sf::RenderTarget &renderTarget) {
	m_currentScreen->render(renderTarget);
}

Screen *ScreenManager::getCurrentScreen() const {
	return m_currentScreen;
}
