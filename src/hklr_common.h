#ifndef HKLR_OBJECT_MANAGER_H
#define HKLR_OBJECT_MANAGER_H

#include "hklr.h"

HklObject* hklr_create(HklTypes type, HklFlags flags, void* value);

void hklr_reference(HklObject* object, HklObject* reference);

void hklr_member_insert(HklObject* object, HklObject* name, HklObject* value);

#endif // HKLR_OBJECT_MANAGER_H