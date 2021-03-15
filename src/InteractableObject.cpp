#include "InteractableObject.h"

void InteractableObject::SetEnabled(const bool _status) { this->m_enabled = _status; }
bool InteractableObject::GetEnabled() const { return this->m_enabled; }

InteractableObject::operator bool() const 
{ 
	return GetEnabled();
}
