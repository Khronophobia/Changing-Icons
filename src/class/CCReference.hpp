#pragma once

namespace khronos {

template<typename T>
requires (std::is_object_v<T>)
class CCReference : public cocos2d::CCObject {
public:
    static CCReference* create(T& ref) {
        return new (std::nothrow) CCReference(ref);
    }
    T& get() const {
        return *m_ref;
    }
    void reseat(T& newRef) {
        m_ref = &newRef;
    }
protected:
    CCReference(T& ref) : m_ref(&ref) {
        autorelease();
    }
protected:
    T* m_ref;
};

} // namespace khronos