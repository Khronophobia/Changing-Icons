#pragma once

namespace changing_icons {
    template<typename T>
    class CCVariableRef : public cocos2d::CCObject {
    protected:
        T& m_varRef;
    public:
        CCVariableRef(T& varRef) : m_varRef(varRef) {}
        static CCVariableRef* create(T& varRef) {
            auto ret = new CCVariableRef(varRef);
            if (ret) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        T& getVarRef() {
            return m_varRef;
        }
    };
}