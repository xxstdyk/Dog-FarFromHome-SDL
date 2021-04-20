#include "InteractableObject.h"

void InteractableObject::IncrementTimer() { this->m_timer++; }

// Setters
void InteractableObject::SetEnabled(const bool _status) { this->m_enabled = _status; }
void InteractableObject::ResetTimer() { m_timer = 0; }

// Getters
bool InteractableObject::GetEnabled() const { return this->m_enabled; }
int InteractableObject::GetTimer() const { return this->m_timer; }

InteractableObject::operator bool() const { return GetEnabled(); }
