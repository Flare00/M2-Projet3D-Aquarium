#ifndef __I_COMPONENT_HPP__
#define __I_COMPONENT_HPP__

class IComponent
{
public: 
    template<typename U>
    bool isType<U>();
};

#endif // !__COMPONENT_H__