//    OpenVPN -- An application to securely tunnel IP networks
//               over a single port, with support for SSL/TLS-based
//               session authentication and key exchange,
//               packet encryption, packet authentication, and
//               packet compression.
//
//    Copyright (C) 2013-2014 OpenVPN Technologies, Inc.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License Version 3
//    as published by the Free Software Foundation.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program in the COPYING file.
//    If not, see <http://www.gnu.org/licenses/>.

#ifndef OPENVPN_OPTIONS_SERVPUSH_H
#define OPENVPN_OPTIONS_SERVPUSH_H

#include <string>
#include <vector>
#include <utility> // for std::move

#include <openvpn/common/exception.hpp>
#include <openvpn/common/options.hpp>

namespace openvpn {
  class ServerPushList : public std::vector<std::string>
  {
  public:
    void parse(const OptionList& opt)
    {
      const OptionList::IndexList* push = opt.get_index_ptr("push");
      if (push)
	{
	    reserve(push->size());
	    for (OptionList::IndexList::const_iterator i = push->begin(); i != push->end(); ++i)
	      {
		const Option& o = opt[*i];
		o.touch();
		push_back(o.get(1, 512));
	      }
	}
    }

    void output(std::ostream& push) const
    {
      for (const_iterator i = begin(); i != end(); ++i)
	{
	  const std::string& e = *i;
	  const bool must_quote = (e.find_first_of(',') != std::string::npos);
	  push << ',';
	  Option::escape_string(push, e, must_quote);
	}
    }
  };
}

#endif