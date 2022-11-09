#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <type_traits>
#include <Engine/Component/IComponent.hpp>
template <class T>
class Component : public IComponent
{
public: 
    template<typename U>
    bool isType<U>(){
       return (std::is_same<T, U>::value);
    }
};

#endif // !__COMPONENT_H__