#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include "visualiser.hpp"

// #define DEMO_MODE

#define MYINT_DEF(name, value) Microscope<int32_t> name = Microscope<int32_t>(value)

template <class T>
class Microscope {
    public:
        T GetData() const {
            return m_data;
        }

        std::string GetName() const {
            return m_name;
        }

        void SetDotName(const std::string& newName) {
            m_dotName = newName;
        }

        std::string GetDotName() const {
            return m_dotName;
        }

        std::string GetColor() const {
            return m_isTmp ? RED : GREEN;
        }
        
        static std::string GetTypename() {
            return m_typename;
        }

        static uint64_t GetTmpCounter() {
            return tmpCounter;
        }

        Microscope<T>(const T& t, const std::string& name = "") :
            m_data(t),
        #ifdef DEMO_MODE
            m_demoCounter(0),
        #endif
            m_name(name),
            m_dotName(""),
            m_isTmp(false),
            m_log("")
        {
            LOG_FUNC;
            CheckIfNameNull();

            m_dotName = Visualiser::GetInstance().CreateObjectNode(*this);
            Visualiser::GetInstance().ProcessContruction(t, *this, TransferType::Copy);
        }

        Microscope<T>(T&& t, const std::string& name = "") :
            m_data(std::move(t)),
        #ifdef DEMO_MODE
            m_demoCounter(0),
        #endif
            m_name(name),
            m_dotName(""),
            m_isTmp(false),
            m_log("")
        {
            LOG_FUNC;
            CheckIfNameNull();

            m_dotName = Visualiser::GetInstance().CreateObjectNode(*this);
            Visualiser::GetInstance().ProcessContruction(m_data, *this, TransferType::Move);
        }

        Microscope<T>(const Microscope<T>& tToCopy, const std::string& name = "")  :
            m_data(tToCopy.m_data),
        #ifdef DEMO_MODE
            m_demoCounter(0),
        #endif
            m_name(name),
            m_dotName(""),
            m_isTmp(false),
            m_log("")
        {
            LOG_FUNC;
            CheckIfNameNull();

            m_dotName = Visualiser::GetInstance().CreateObjectNode(*this);
            Visualiser::GetInstance().ProcessContruction(tToCopy, *this, TransferType::Copy);
        }

        Microscope<T>(Microscope<T>&& tToCopy, const std::string& name = "")  :
            m_data(std::move(tToCopy.m_data)),
        #ifdef DEMO_MODE
            m_demoCounter(0),
        #endif
            m_name(name),
            m_dotName(""),
            m_isTmp(false),
            m_log("")
        {
            LOG_FUNC;
            std::swap(m_data, tToCopy.m_data);
            CheckIfNameNull();

            m_dotName = Visualiser::GetInstance().CreateObjectNode(*this);
            Visualiser::GetInstance().ProcessContruction(tToCopy, *this, TransferType::Move);
        }

    #ifndef DEMO_MODE
        #define REF_OPERATOR(op)                                                        \
            Microscope<T>& operator op (const Microscope<T>& t) {                       \
                LOG_FUNC;                                                               \
                m_data op t.m_data;                                                     \
                Visualiser::GetInstance().ProcessChangingOperator(*this, t, #op);       \
                                                                                        \
                return *this;                                                           \
            }
    #else
        #define REF_OPERATOR(op)                                                        \
            Microscope<T>& operator op (const Microscope<T>& t) {                       \
                LOG_FUNC;                                                               \
                if (CheckIfDemo()) {                                                    \
                    return *this;                                                       \
                }                                                                       \
                                                                                        \
                m_data op t.m_data;                                                     \
                Visualiser::GetInstance().ProcessChangingOperator(*this, t, #op);       \
                                                                                        \
                return *this;                                                           \
            }
    #endif
        REF_OPERATOR(+=)
        REF_OPERATOR(-=)
        REF_OPERATOR(*=)
        REF_OPERATOR(/=)
        REF_OPERATOR(%=)
        REF_OPERATOR(&=)
        REF_OPERATOR(|=)
        REF_OPERATOR(^=)
        REF_OPERATOR(<<=)
        REF_OPERATOR(>>=)
    #undef REF_OPERATOR

        Microscope<T>& operator++() {
            LOG_FUNC;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return *this;
            }
        #endif /*DEMO_MODE*/

            ++m_data;
            return *this;
        }

        Microscope<T> operator++(int) {
            LOG_FUNC;
            Microscope<T> tmp = *this;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return tmp;
            }
        #endif /*DEMO_MODE*/

            tmp.m_data++;

            return tmp;
        }

        Microscope<T>& operator--() {
            LOG_FUNC;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return *this;
            }
        #endif /*DEMO_MODE*/

            --m_data;
            return *this;
        }

        Microscope<T> operator--(int) {
            LOG_FUNC;
            Microscope<T> tmp = *this;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return tmp;
            }
        #endif /*DEMO_MODE*/

            tmp.m_data--;

            return tmp;
        }

        Microscope<T> operator!() const {
            LOG_FUNC;
            Microscope<T> tmp = *this;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return tmp;
            }
        #endif

            tmp.m_data = !tmp.m_data;

            return tmp;
        }

        Microscope<T> operator~() const {
            LOG_FUNC;
            Microscope<T> tmp = *this;
        #ifdef DEMO_MODE
            if (CheckIfDemo()) {
                return tmp;
            }
        #endif

            tmp.m_data = ~tmp.m_data;

            return tmp;
        }

        Microscope<T>& operator=(const Microscope<T>& t) {
            LOG_FUNC;
            m_data = t.m_data;

            Visualiser::GetInstance().ProcessEqOperator(*this, t, TransferType::Copy);
            return *this;
        }

        Microscope<T>& operator=(Microscope<T>&& t) {
            LOG_FUNC;
            std::swap(m_data, t.m_data);

            Visualiser::GetInstance().ProcessEqOperator(*this, t, TransferType::Move);
            return *this;
        }

    #ifndef DEMO_MODE
        #define OBJ_CONST_OPERATOR(op)                                                  \
            Microscope<T> operator op (const Microscope<T>& t) const {                  \
                LOG_FUNC;                                                               \
                Visualiser::GetInstance().DisableLogging();                             \
                Microscope<T> tmp = *this;                                              \
                                                                                        \
                tmp op##= t;                                                            \
                Visualiser::GetInstance().EnableLogging();                              \
                                                                                        \
                tmp.SetDotName(Visualiser::GetInstance().CreateObjectNode(tmp));        \
                Visualiser::GetInstance().ProcessBinaryOperator(*this, t, tmp, #op);    \
                                                                                        \
                return tmp;                                                             \
            }
    #else
        #define OBJ_CONST_OPERATOR(op)                                                  \
            Microscope<T> operator op (const Microscope<T>& t) const {                  \
                LOG_FUNC;                                                               \
                Microscope<T> tmp = *this;                                              \
                if (CheckIfDemo()) {                                                    \
                    return tmp;                                                         \
                }                                                                       \
                tmp op##= t;                                                            \
                Visualiser::GetInstance().ProcessBinaryOperator(*this, t, tmp, #op);    \
                                                                                        \
                return tmp;                                                             \
            }
    #endif
        OBJ_CONST_OPERATOR(+)
        OBJ_CONST_OPERATOR(-)
        OBJ_CONST_OPERATOR(*)
        OBJ_CONST_OPERATOR(/)
        OBJ_CONST_OPERATOR(%)
        OBJ_CONST_OPERATOR(&)
        OBJ_CONST_OPERATOR(|)
        OBJ_CONST_OPERATOR(^)
        OBJ_CONST_OPERATOR(<<)
        OBJ_CONST_OPERATOR(>>)
    #undef OBJ_CONST_OPERATOR

    #ifndef DEMO_MODE
        #define BOOL_CONST_OPERATOR(op)                                         \
            bool operator op (const Microscope<T>& t) const {                   \
                return m_data op t.m_data;                                      \
            }

    #else
        #define BOOL_CONST_OPERATOR(op)                                         \
            bool operator op (const Microscope<T>& t) const {                   \
                if (CheckIfDemo()) {                                            \
                    return false;                                               \
                }                                                               \
                                                                                \
                return m_data op t.m_data;                                      \
            }   
    #endif

        BOOL_CONST_OPERATOR(==)
        BOOL_CONST_OPERATOR(!=)
        BOOL_CONST_OPERATOR(>)
        BOOL_CONST_OPERATOR(<)
        BOOL_CONST_OPERATOR(>=)
        BOOL_CONST_OPERATOR(<=)
    #undef BOOL_CONST_OPERATOR

    private:
        void CheckIfNameNull() {
            if (m_name == "") {
                m_name = "tmp" + std::to_string(tmpCounter++);
                m_isTmp = true;
            }
        }

    #ifdef DEMO_MODE
        bool CheckIfDemo() const {
            if (m_demoCounter > MaxOperationsAmount) {
                std::cerr << "Operations amount is reached. Please buy license for next usage" << m_name << " otherwise, operations on this integer won't be accepted" << std::endl;

                return true;
            }
    
            const_cast<Microscope<T>*>(this)->m_demoCounter++;
            return false;
        }
    #endif

        T m_data;
    #ifdef DEMO_MODE
        uint32_t m_demoCounter;
        const static uint64_t MaxOperationsAmount = 20;
    #endif
        std::string m_name;
        std::string m_dotName;

        bool m_isTmp;
        
        const static std::string m_typename;

        static uint64_t tmpCounter;

        std::string m_log;
};
