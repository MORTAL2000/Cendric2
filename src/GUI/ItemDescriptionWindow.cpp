#include <iomanip>
#include <sstream>

#include "GUI/ItemDescriptionWindow.h"

using namespace std;

const float ItemDescriptionWindow::WIDTH = 340.f;

inline std::string toStrMaxDecimals(float value, int decimals) {
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	std::string s = ss.str();
	if (decimals > 0 && s[s.find_last_not_of('0')] == '.') {
		s.erase(s.size() - decimals + 1);
	}
	return std::string(s.begin(), s.end());
}

ItemDescriptionWindow::ItemDescriptionWindow() : Window(
	sf::FloatRect(0.f, 0.f, WIDTH, WIDTH),
	WindowOrnamentStyle::FANCY,
	GUIConstants::MAIN_COLOR,
	GUIConstants::BACK_COLOR,
	GUIConstants::ORNAMENT_COLOR) {
	m_titleText.setCharacterSize(GUIConstants::CHARACTER_SIZE_M);
	m_titleText.setColor(COLOR_WHITE);

	m_descriptionText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_descriptionText.setColor(COLOR_LIGHT_GREY);

	m_statsText.setCharacterSize(GUIConstants::CHARACTER_SIZE_S);
	m_statsText.setColor(COLOR_WHITE);
}

std::string ItemDescriptionWindow::getGoldText(const Item& item) const {
	std::string goldText;
	goldText.append(g_textProvider->getText("GoldValue"));
	goldText.append(": ");
	goldText.append(item.getValue() < 0 ? g_textProvider->getText("Unsalable") : to_string(item.getValue()));
	return goldText;
}

void ItemDescriptionWindow::setPosition(const sf::Vector2f& position) {
	Window::setPosition(position);
	float y = position.y + GUIConstants::TEXT_OFFSET;
	m_titleText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	m_descriptionText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
	y += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	if (m_statsText.getLocalBounds().height == GUIConstants::CHARACTER_SIZE_S) {
		y -= GUIConstants::TEXT_OFFSET - GUIConstants::CHARACTER_SIZE_S;
	}
	m_statsText.setPosition(position.x + GUIConstants::TEXT_OFFSET, y);
}

void ItemDescriptionWindow::load(const Item& item) {
	m_titleText.setString(g_textProvider->getText(item.getID(), "item"));
	m_descriptionText.setString(g_textProvider->getCroppedText(item.getID(), "item_desc", GUIConstants::CHARACTER_SIZE_S, static_cast<int>(WIDTH - 2 * GUIConstants::TEXT_OFFSET)));

	string stats = "\n";

	if (item.getType() == ItemType::Permanent) {
		m_statsText.setColor(COLOR_GOOD);
		stats.append(g_textProvider->getText("Permanent"));
		stats.append("\n\n");
	}
	else {
		m_statsText.setColor(COLOR_WHITE);
	}

	const AttributeData& attr = item.getAttributes();
	AttributeData::appendAttributes(stats, attr);
	stats.append("\n");

	if (item.getFoodDuration() > sf::Time::Zero) {
		stats.append(g_textProvider->getText("Duration"));
		stats.append(": ");
		stats.append(to_string(static_cast<int>(floor(item.getFoodDuration().asSeconds()))));
		stats.append(" s\n");
	}

	stats.append(getGoldText(item));

	if (item.getType() == ItemType::Equipment_weapon && item.isWeapon()) {
		Weapon weapon(item.getID());
		stats.append("\n\n");
		stats.append(g_textProvider->getText("WeaponDamage"));
		stats.append(": ");
		stats.append(to_string(weapon.getWeaponChopDamage()));
		stats.append("\n");

		stats.append(g_textProvider->getText("Cooldown"));
		stats.append(": ");
		stats.append(toStrMaxDecimals(weapon.getWeaponCooldown().asSeconds(), 1));
		stats.append("s\n");

		if (weapon.getWeaponSlots().size() > 0) {
			stats.append("\n");
			stats.append("<<< " + g_textProvider->getText("SpellSlots") + " >>>\n");
			for (auto& it : weapon.getWeaponSlots()) {
				stats.append(g_textProvider->getText(EnumNames::getSpellTypeName(it.spellSlot.spellType)));
				stats.append(" - " + g_textProvider->getText("GemSockets") + ": ");
				stats.append(to_string(it.spellModifiers.size()) + "\n");
			}
		}
	}

	m_statsText.setString(stats);

	float height = 2 * GUIConstants::TEXT_OFFSET;
	height += m_titleText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S * 2;
	height += m_descriptionText.getLocalBounds().height + GUIConstants::CHARACTER_SIZE_S;
	height += m_statsText.getLocalBounds().height;
	setHeight(height);
}

void ItemDescriptionWindow::render(sf::RenderTarget& renderTarget) {
	if (!m_isVisible) return;
	Window::render(renderTarget);
	renderTarget.draw(m_titleText);
	renderTarget.draw(m_descriptionText);
	renderTarget.draw(m_statsText);
}

void ItemDescriptionWindow::show() {
	m_isVisible = true;
}

void ItemDescriptionWindow::hide() {
	m_isVisible = false;
}

bool ItemDescriptionWindow::isVisible() const {
	return m_isVisible;
}


