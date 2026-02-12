#ifndef ITEM_FACTORIES_H
#define ITEM_FACTORIES_H

#include "Factory.h"
#include "CloneFactory.h"
#include "Item.h"
#include "Singleton.h"
#include <string>

typedef Factory<Item, std::string, std::shared_ptr<Item> (*)()> ItemFactoryType;
typedef Singleton<ItemFactoryType> ItemFactory;

typedef CloneFactory<Item, std::shared_ptr<Item> (*)(const Item*)> ItemCloneFactoryType;
typedef Singleton<ItemCloneFactoryType> ItemCloneFactory;

#endif
