#pragma once

#define DEFINE_BUILDER_MEMBER(type, name)                   \
public:                                                     \
    constexpr auto& name(type const& x)                     \
    {                                                       \
        m_##name = x;                                       \
        return *this;                                       \
    }                                                       \
    constexpr type const& name() const { return m_##name; } \
                                                            \
private:                                                    \
    type m_##name
