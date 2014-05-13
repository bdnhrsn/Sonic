/******************************************************************************
 * $Id:  $
 * $Name: $
 *
 * Author: Pete Goodliffe
 *
 * ----------------------------------------------------------------------------
 * Copyright 2002 Pete Goodliffe All rights reserved.
 *
 * ----------------------------------------------------------------------------
 * Purpose: Circular buffer test harness
 *
 * ----------------------------------------------------------------------------
 * History: See source control system log.
 *
 *****************************************************************************/

#include "circular.h"

#include <iostream>
#include <algorithm>
#include <iterator>

    /*
     * This really is a very poor set of examples of how the library
     * works, and a sort of noddy test-harness.
     */

typedef circular_buffer<int> cbuf_type;

void print_cbuf_contents(cbuf_type &cbuf)
{
    std::cout << "Printing cbuf size("
              <<cbuf.size()<<"/"<<cbuf.capacity()<<") contents...\n";
    for (size_t n = 0; n < cbuf.size(); ++n)
       std::cout << "  " << n << ": " << cbuf[n] << "\n";

    if (!cbuf.empty())
    {
        std::cout << "  front()=" << cbuf.front()
                  << ", back()=" << cbuf.back() << "\n";
    }
    else
    {
        std::cout << "  empty\n";
    }
}

int main()
{
    std::cout << "\nCreating circular_buffer...\n";
    cbuf_type cbuf(20);
    print_cbuf_contents(cbuf);

    std::cout << "\nAdding 10 even numbers...\n";
    for (int n = 0; n < 10; ++n) cbuf.push_back(n*2);
    print_cbuf_contents(cbuf);

    std::cout << "\nRemoving five numbers...\n";
    for (int n = 0; n < 5; ++n) cbuf.pop_front();
    print_cbuf_contents(cbuf);

    std::cout << "\nAdding 16 odd numbers...\n";
    for (int n = 0; n < 16; ++n) cbuf.push_back((n+1)*3);
    print_cbuf_contents(cbuf);

    std::cout << "\nTesting forward iterator...\n";
    cbuf_type::iterator i = cbuf.begin();
    while (i != cbuf.end())
    {
        std::cout << "  value " << *i << "\n";
        ++i;
    }

    std::cout << "\nTesting const forward iterator...\n";
    const cbuf_type &ccbuf(cbuf);
    cbuf_type::const_iterator ci = cbuf.begin();
    while (ci != cbuf.end())
    {
        std::cout << "  value " << *ci << "\n";
        // *ci = 5; // error
        ++ci;
    }

    std::cout << "\nUsing with std::copy...\n";
    std::copy(cbuf.begin(), cbuf.end(),
        std::ostream_iterator<int>(std::cout));

    std::cout << "\nTesting reverse iterator...\n";
    cbuf_type::reverse_iterator ri = cbuf.rbegin();
    while (ri != cbuf.rend())
    {
        std::cout << "  value " << *ri << "\n";
        ++ri;
    }

    cbuf_type cbuf2 = cbuf;
    if (cbuf == cbuf2)
    {
        std::cout << "operator== works\n";
    }
    else
    {
        std::cout << "operator== fails\n";
    }
    if (cbuf != cbuf2)
    {
        std::cout << "operator!== fails\n";
    }
    else
    {
        std::cout << "operator!= works\n";
    }
    cbuf2.push_back(3);
    if (cbuf == cbuf2)
    {
        std::cout << "operator== fails\n";
    }
    else
    {
        std::cout << "operator== works\n";
    }
    if (cbuf != cbuf2)
    {
        std::cout << "operator!= works\n";
    }
    else
    {
        std::cout << "operator!== fails\n";
    }

    std::cout << "Testing assignment...\n";
    cbuf_type cbuf3;
    cbuf3 = cbuf;
    if (cbuf == cbuf3)
    {
        std::cout << "assignment works\n";
    }
    else
    {
        std::cout << "assignment fails\n";
    }

    std::cout << "Testing reserve...\n";
    cbuf3.reserve(100);
    print_cbuf_contents(cbuf3);
    if (cbuf == cbuf3 && cbuf3.capacity() == 100)
    {
        std::cout << "reserve works\n";
    }
    else
    {
        std::cout << "reserve fails\n";
    }

    std::cout << "Testing copy constructor...\n";
    cbuf_type cbuf4(cbuf);
    if (cbuf == cbuf4)
    {
        std::cout << "copy ctor works\n";
    }
    else
    {
        std::cout << "copy ctor fails\n";
    }

    std::cout << "Testing template iterator constructor...\n";
    cbuf_type cbuf5(cbuf.begin(), cbuf.end());
    if (cbuf == cbuf5)
    {
        std::cout << "template iterator ctor works\n";
    }
    else
    {
        std::cout << "template iterator ctor fails\n";
    }
}
