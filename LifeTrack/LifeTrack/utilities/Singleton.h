#ifndef LIFETRACK_UTILS_TTSINGLETONT_H
#define LIFETRACK_UTILS_TTSINGLETONT_H

#include <memory>
#include <mutex>
#include <assert.h>

namespace utils
{
    template<typename T>
    class Singleton {
    public:
        template<typename ...Args>
        static T* GetInstance(Args&&... args) {
            if (nullptr == m_pSington) {
                std::call_once(m_OnceFlag, [&] {
                    static T instance(std::forward<Args>(args)...);
                    m_pSington = &instance;
                    });
            }
            assert(m_pSington != nullptr);
            return m_pSington;
        }

        static void DesInstance() {
            if (m_pSington) {
                delete m_pSington;
                m_pSington = nullptr;
            }
        }
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    private:
        Singleton(const Singleton&) = delete;
        Singleton(const Singleton&&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(const Singleton&&) = delete;
    private:
        static T* m_pSington;
        static std::once_flag m_OnceFlag;
    };

    template<typename T>
    T* Singleton<T>::m_pSington = nullptr;

    template<typename T>
    std::once_flag Singleton<T>::m_OnceFlag;
}

#endif