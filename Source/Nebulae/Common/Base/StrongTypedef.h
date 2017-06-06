// -*- C++ -*-
/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003-2008 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@gmail.com */

/** \file StrongTypedef.h \brief Contains macros used to create "strong
    typedefs", that is value types that are not mutually interoperable with
    each other or with builtin types for extra type safety. */

#ifndef _NEBULAE_STRONGTYPEDEF_H_
#define _NEBULAE_STRONGTYPEDEF_H_

namespace Nebulae {

inline int Value(int i)
{ return i; }

inline double Value(double d)
{ return d; }

inline std::size_t Value(std::size_t s)
{ return s; }

}

#define NE_MEMBER_BOOL_OP_SELF_TYPE(op, rhs_type)                    \
    inline bool operator op (rhs_type rhs) const                          \
    { return m_value op rhs.m_value; }

#define NE_MEMBER_BOOL_OP_OTHER_TYPE(op, rhs_type)                   \
    inline bool operator op (rhs_type rhs) const                          \
    { return m_value op Value(rhs); }

#define NE_MEMBER_NEG_INCR_DECR(this_type)                           \
    inline this_type& operator++()                                        \
    {                                                                     \
        ++m_value;                                                        \
        return *this;                                                     \
    }                                                                     \
    inline this_type& operator--()                                        \
    {                                                                     \
        --m_value;                                                        \
        return *this;                                                     \
    }                                                                     \
    inline this_type operator++(int)                                      \
    {                                                                     \
        this_type retval(m_value);                                        \
        ++m_value;                                                        \
        return retval;                                                    \
    }                                                                     \
    inline this_type operator--(int)                                      \
    {                                                                     \
        this_type retval(m_value);                                        \
        --m_value;                                                        \
        return retval;                                                    \
    }

#define NE_MEMBER_ASSIGN_OP_SELF_TYPE(op, rhs_type)                  \
    inline rhs_type& operator op (rhs_type rhs)                           \
    {                                                                     \
        m_value op rhs.m_value;                                           \
        return *this;                                                     \
    }

#define NE_MEMBER_ASSIGN_OP_OTHER_TYPE_DECL(op, self_type, rhs_type) \
    inline self_type& operator op ## = (rhs_type rhs)

#define NE_MEMBER_ASSIGN_OP_OTHER_TYPE(op, self_type, rhs_type)      \
    NE_MEMBER_ASSIGN_OP_OTHER_TYPE_DECL(op, self_type, rhs_type)     \
    {                                                                     \
        m_value = static_cast<self_type::value_type>(                     \
            m_value op Value(rhs)                                         \
        );                                                                \
        return *this;                                                     \
    }

#define NE_MEMBER_OP_OTHER_TYPE_DECL(op, self_type, rhs_type)        \
    inline self_type& operator op (rhs_type rhs) const

#define NE_MEMBER_OP_OTHER_TYPE(op, self_type, rhs_type)             \
    NE_MEMBER_OP_OTHER_TYPE_DECL(op, self_type, rhs_type)            \
    { return self_type(m_value op Value(rhs)); }

#define NE_NONMEMBER_OP_SELF_TYPE(op, self_type)                     \
    inline self_type operator op (self_type lhs, self_type rhs)           \
    { return lhs op ## = rhs; }

#define NE_NONMEMBER_OP_OTHER_TYPE(op, self_type, rhs_type)          \
    inline self_type operator op (self_type lhs, rhs_type rhs)            \
    { return lhs op ## = Value(rhs); }

#define NE_MEMBER_SELF_COMPARATORS(self_type)                        \
    NE_MEMBER_BOOL_OP_SELF_TYPE(==, self_type);                      \
    NE_MEMBER_BOOL_OP_SELF_TYPE(!=, self_type);                      \
    NE_MEMBER_BOOL_OP_SELF_TYPE(<, self_type);                       \
    NE_MEMBER_BOOL_OP_SELF_TYPE(>, self_type);                       \
    NE_MEMBER_BOOL_OP_SELF_TYPE(<=, self_type);                      \
    NE_MEMBER_BOOL_OP_SELF_TYPE(>=, self_type);

#define NE_MEMBER_OTHER_COMPARATORS(rhs_type)                        \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(==, rhs_type);                      \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(!=, rhs_type);                      \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(<, rhs_type);                       \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(>, rhs_type);                       \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(<=, rhs_type);                      \
    NE_MEMBER_BOOL_OP_OTHER_TYPE(>=, rhs_type);

#define NE_MEMBER_ARITH_ASSIGN_OPS_SELF_TYPE(rhs_type)               \
    NE_MEMBER_ASSIGN_OP_SELF_TYPE(+=, rhs_type);                     \
    NE_MEMBER_ASSIGN_OP_SELF_TYPE(-=, rhs_type);                     \
    NE_MEMBER_ASSIGN_OP_SELF_TYPE(*=, rhs_type);                     \
    NE_MEMBER_ASSIGN_OP_SELF_TYPE(/=, rhs_type);

#define NE_MEMBER_ARITH_ASSIGN_OPS_OTHER_TYPE(self_type, rhs_type)   \
    NE_MEMBER_ASSIGN_OP_OTHER_TYPE(+, self_type, rhs_type);          \
    NE_MEMBER_ASSIGN_OP_OTHER_TYPE(-, self_type, rhs_type);          \
    NE_MEMBER_ASSIGN_OP_OTHER_TYPE(*, self_type, rhs_type);          \
    NE_MEMBER_ASSIGN_OP_OTHER_TYPE(/, self_type, rhs_type);

#define NE_NONMEMBER_ARITH_OPS_SELF_TYPE(self_type)                  \
    NE_NONMEMBER_OP_SELF_TYPE(+, self_type);                         \
    NE_NONMEMBER_OP_SELF_TYPE(-, self_type);                         \
    NE_NONMEMBER_OP_SELF_TYPE(*, self_type);                         \
    NE_NONMEMBER_OP_SELF_TYPE(/, self_type);

#define NE_NONMEMBER_ARITH_OPS_OTHER_TYPE(self_type, rhs_type)       \
    NE_NONMEMBER_OP_OTHER_TYPE(+, self_type, rhs_type);              \
    NE_NONMEMBER_OP_OTHER_TYPE(-, self_type, rhs_type);              \
    NE_NONMEMBER_OP_OTHER_TYPE(*, self_type, rhs_type);              \
    NE_NONMEMBER_OP_OTHER_TYPE(/, self_type, rhs_type);

#define NE_NONMEMBER_REVERSED_BOOL_OP_SET(lhs_type, self_type)       \
    inline bool operator==(lhs_type x, self_type y)                       \
    { return y == x; }                                                    \
    inline bool operator!=(lhs_type x, self_type y)                       \
    { return y != x; }                                                    \
    inline bool operator<(lhs_type x, self_type y)                        \
    { return !(y < x || y == x); }                                        \
    inline bool operator>(lhs_type x, self_type y)                        \
    { return !(y > x || y == x); }                                        \
    inline bool operator<=(lhs_type x, self_type y)                       \
    { return !(y < x); }                                                  \
    inline bool operator>=(lhs_type x, self_type y)                       \
    { return !(y > x); }

#define NE_NONMEMBER_REVERSED_ARITH_OP_SET(lhs_type, self_type)      \
    inline self_type operator+(lhs_type x, self_type y)                   \
    { return y += x; }                                                    \
    inline self_type operator*(lhs_type x, self_type y)                   \
    { return y *= x; }


/** Creates a new type \a name, based on underlying type std::size_t, which is
    not interconvertible with any other numeric type.  The resulting type has
    most of the operations of std::size_t.  Specifically, the type is totally
    ordered, incrementable, decrementable, and arithmetic.  The type is also
    interarithemtic with and comparable to objects of type std::size_t. */
#define NE_STRONG_SIZE_TYPEDEF(name)                                      \
    class name;                                                           \
    std::size_t Value(name x);                                            \
                                                                          \
    class name                                                            \
    {                                                                     \
    private:                                                              \
        struct ConvertibleToBoolDummy {int _;};                           \
                                                                          \
    public:                                                               \
        typedef std::size_t value_type;                                   \
                                                                          \
        name() : m_value(0) {}                                            \
        explicit name(std::size_t t) : m_value(t) {}                      \
                                                                          \
        NE_MEMBER_SELF_COMPARATORS(name);                                 \
                                                                          \
        NE_MEMBER_OTHER_COMPARATORS(std::size_t);                         \
                                                                          \
        operator int ConvertibleToBoolDummy::* () const                   \
        { return m_value ? &ConvertibleToBoolDummy::_ : 0; }              \
                                                                          \
        NE_MEMBER_NEG_INCR_DECR(name);                                    \
                                                                          \
        NE_MEMBER_ARITH_ASSIGN_OPS_SELF_TYPE(name);                       \
        NE_MEMBER_ASSIGN_OP_SELF_TYPE(%=, name);                          \
                                                                          \
        NE_MEMBER_ARITH_ASSIGN_OPS_OTHER_TYPE(name, std::size_t);         \
        NE_MEMBER_ASSIGN_OP_OTHER_TYPE(%, name, std::size_t);             \
                                                                          \
    private:                                                              \
        std::size_t m_value;                                              \
                                                                          \
        friend class name ## _d;                                          \
        friend std::size_t Value(name x);                                 \
    };                                                                    \
                                                                          \
    NE_NONMEMBER_ARITH_OPS_SELF_TYPE(name);                               \
    NE_NONMEMBER_OP_SELF_TYPE(%, name);                                   \
                                                                          \
    NE_NONMEMBER_ARITH_OPS_OTHER_TYPE(name, std::size_t);                 \
    NE_NONMEMBER_OP_OTHER_TYPE(%, name, std::size_t);                     \
                                                                          \
    NE_NONMEMBER_REVERSED_BOOL_OP_SET(std::size_t, name);                 \
                                                                          \
    NE_NONMEMBER_REVERSED_ARITH_OP_SET(std::size_t, name);                \
                                                                          \
    inline std::size_t Value(name x)                                      \
    { return x.m_value; }                                                 \
                                                                          \
    inline std::ostream& operator<<(std::ostream& os, name x)             \
    { os << Value(x); return os; }                                        \
                                                                          \
    inline std::istream& operator>>(std::istream& os, name& x)            \
    {                                                                     \
        std::size_t t;                                                    \
        os >> t;                                                          \
        x = name(t);                                                      \
        return os;                                                        \
    }                                                                     \
                                                                          \
    void dummy_function_to_force_semicolon()

#endif
