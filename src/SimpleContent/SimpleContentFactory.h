#pragma once
#include "SimpleContent.h"
enum class SimpleContentType
{
	DEFAULT,
	CIRCLE,
};

SimpleContent getSimpleContent(SimpleContentType type);
