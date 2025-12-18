#ifndef FACTORY_H
#define FACTORY_H

#include <map>
#include <exception>
#include <memory>

template <class IdentifierType, class ProductType>
class DefaultFactoryError
{
public:
    class Exception : public std::exception
    {
    public:
        Exception(const IdentifierType& unknownId)
            : unknownId_(unknownId)
        {
        }
        virtual const char* what() const noexcept override
        {
            return "Unknown object type passed to Factory.";
        }
        const IdentifierType GetId()
        {
            return unknownId_;
        };
    private:
        IdentifierType unknownId_;
    };
protected:
    static std::shared_ptr<ProductType> OnUnknownType(const IdentifierType& id)
    {
        throw Exception(id);
    }
};

template
<
    class AbstractProduct,
    typename IdentifierType,
    typename ProductCreator = std::shared_ptr<AbstractProduct> (*)(),
    template<typename, class>
    class FactoryErrorPolicy = DefaultFactoryError
>
class Factory
    : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
public:
    bool Register(const IdentifierType& id, ProductCreator creator)
    {
        return associations_.insert(
            typename AssocMap::value_type(id, creator)).second;
    }
    bool Unregister(const IdentifierType& id)
    {
        return associations_.erase(id) == 1;
    }
    std::shared_ptr<AbstractProduct> CreateObject(const IdentifierType& id)
    {
        typename AssocMap::const_iterator i =
            associations_.find(id);
        if (i != associations_.end())
        {
            return (i->second)();
        }
        return this->OnUnknownType(id);
    }
private:
    typedef std::map<IdentifierType, ProductCreator>
        AssocMap;
    AssocMap associations_;
};

#endif
