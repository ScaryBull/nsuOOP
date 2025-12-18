#ifndef CLONE_FACTORY_H
#define CLONE_FACTORY_H

#include <map>
#include <typeindex>
#include <exception>
#include <memory>
#include "Factory.h"

template
<
    class AbstractProduct,
    class ProductCreator = std::shared_ptr<AbstractProduct> (*)(const AbstractProduct*),
    template<typename, class>
    class FactoryErrorPolicy = DefaultFactoryError
>
    class CloneFactory
    : public FactoryErrorPolicy<std::type_index, AbstractProduct>
{
public:
    bool Register(const std::type_index& id, ProductCreator creator)
    {
        return associations_.insert(
            typename AssocMap::value_type(id, creator)).second;
    }
    bool Unregister(const std::type_index& id)
    {
        return associations_.erase(id) == 1;
    }
    std::shared_ptr<AbstractProduct> CreateObject(const AbstractProduct* model)
    {
        typename AssocMap::const_iterator i =
            associations_.find(std::type_index(typeid(*model)));
        if (i != associations_.end())
        {
            return (i->second)(model);
        }
        return this->OnUnknownType(std::type_index(typeid(*model)));
    }
private:
    typedef std::map<std::type_index, ProductCreator>
        AssocMap;
    AssocMap associations_;
};

#endif
