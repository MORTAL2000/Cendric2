#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item equipment light bean, directly from the database
struct ItemEquipmentLightBean : DatabaseBean {
	std::string item_id;
	sf::Vector2f light_offset;
	sf::Vector2f light_radius;
	float brightness;
};