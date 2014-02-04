//------------------------------------------------------------------------------
/*
    This file is part of rippled: https://github.com/ripple/rippled
    Copyright (c) 2012, 2013 Ripple Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include "beast/beast/make_unique.h"

#include "../api/Manager.h"

#include "DoPrint.h"

namespace ripple {
namespace RPC {

class ManagerImp : public Manager
{
public:
    typedef std::unordered_map <std::string, handler_type> Map;

    Journal m_journal;
    Map m_map;

    ManagerImp (Journal journal)
        : m_journal (journal)
    {
    }

    void add (std::string const& method, handler_type&& handler)
    {
        std::pair <Map::iterator, bool> result (m_map.emplace (
            std::piecewise_construct, std::make_tuple (method),
                std::make_tuple (std::move (handler))));
    }

    bool dispatch (Request& req)
    {
        Map::const_iterator const iter (m_map.find (req.method));
        if (iter == m_map.end())
            return false;
        iter->second (req);
        return true;
    }
};

//------------------------------------------------------------------------------

Manager::~Manager ()
{
}

std::unique_ptr <Manager> make_Manager (Journal journal)
{
    std::unique_ptr <Manager> m (std::make_unique <ManagerImp> (journal));

    m->add <DoPrint> ("print");

    return m;
}

}
}
