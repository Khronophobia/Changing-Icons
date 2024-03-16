#pragma once

namespace changing_icons {
    template<typename T>
    class CIVariableRef : public cocos2d::CCObject {
    protected:
        T& m_varRef;
    public:
        CIVariableRef(T& varRef) : m_varRef(varRef) {}
        static CIVariableRef* create(T& varRef) {
            auto ret = new CIVariableRef(varRef);
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