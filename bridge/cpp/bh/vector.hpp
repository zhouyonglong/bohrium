/*
This file is part of Bohrium and copyright (c) 2012 the Bohrium team:
http://bohrium.bitbucket.org

Bohrium is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 
of the License, or (at your option) any later version.

Bohrium is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the 
GNU Lesser General Public License along with Bohrium. 

If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>

#include "traits.hpp"       // Traits for assigning type to constants and arrays.
#include "cppb.hpp"
#include "runtime.hpp"

namespace bh {

template <typename T>
Operand<T>::Operand() : key(keys++), is_temp(false)
{
    storage.insert(key, new bh_array);
    assign_array_type<T>( &storage[this->key] );
}

template <typename T>
Operand<T>::~Operand()
{
    std::cout << "De-allocating=" << this->key << std::endl;
    Runtime::instance()->enqueue((bh_opcode)BH_FREE, *this);
    Runtime::instance()->enqueue((bh_opcode)BH_DISCARD, *this);
}

template <typename T>
int Operand<T>::getKey() const
{
    return this->key;
}

template <typename T>
bool Operand<T>::isTemp() const
{
    return is_temp;
}

template <typename T>
void Operand<T>::setTemp(bool is_temp)
{
    is_temp = is_temp;
}

template <typename T>
typename Operand<T>::iterator Operand<T>::begin()
{
    Runtime::instance()->enqueue( BH_SYNC, *this );
    Runtime::instance()->flush();

    return Operand<T>::iterator( storage[this->key] );
}

template <typename T>
typename Operand<T>::iterator Operand<T>::end()
{
    return Operand<T>::iterator();
}

template <typename T>
Vector<T>::Vector( Vector<T> const& vector )
{
    storage[this->key].base        = NULL;
    storage[this->key].ndim        = storage[vector.getKey()].ndim;
    storage[this->key].start       = storage[vector.getKey()].start;
    for(bh_index i=0; i< storage[vector.getKey()].ndim; i++) {
        storage[this->key].shape[i] = storage[vector.getKey()].shape[i];
    }
    for(bh_index i=0; i< storage[vector.getKey()].ndim; i++) {
        storage[this->key].stride[i] = storage[vector.getKey()].stride[i];
    }
    storage[this->key].data        = NULL;
}

template <typename T>
Vector<T>::Vector( int d0 )
{
    storage[this->key].base        = NULL;
    storage[this->key].ndim        = 1;
    storage[this->key].start       = 0;
    storage[this->key].shape[0]    = d0;
    storage[this->key].stride[0]   = 1;
    storage[this->key].data        = NULL;
}

template <typename T>
Vector<T>::Vector( int d0, int d1 )
{
    storage[this->key].base        = NULL;
    storage[this->key].ndim        = 2;
    storage[this->key].start       = 0;
    storage[this->key].shape[0]    = d0;
    storage[this->key].stride[0]   = d1;
    storage[this->key].shape[1]    = d1;
    storage[this->key].stride[1]   = 1;
    storage[this->key].data        = NULL;
}

template <typename T>
Vector<T>& Vector<T>::operator++()
{
    Runtime::instance()->enqueue( (bh_opcode)BH_ADD, *this, *this, (T)1 );
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator++(int)
{
    Runtime::instance()->enqueue( (bh_opcode)BH_ADD, *this, *this, (T)1 );
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator--()
{
    Runtime::instance()->enqueue( (bh_opcode)BH_SUBTRACT, *this, *this, (T)1 );
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator--(int)
{
    Runtime::instance()->enqueue( (bh_opcode)BH_SUBTRACT, *this, *this, (T)1 );
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator+=( const T rhs )
{
    Runtime::instance()->enqueue( (bh_opcode)BH_ADD, *this, *this, rhs );
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator+=( Vector & rhs )
{
    Runtime::instance()->enqueue( (bh_opcode)BH_ADD, *this, *this, rhs );
    return *this;
}

/*

This might be fun to do an initializer on the form:

x = 1,2,3,
    4,5,6,
    7,8,8;

*/
template <typename T>
Vector<T>& operator, ( Vector<T>& lhs, T rhs )
{
    std::cout << "[" << lhs.getKey() << "," << rhs << "]" << std::endl;
    return lhs;
}


}

