/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/node/configuration.hpp>

#include <cstddef>
#include <bitcoin/blockchain.hpp>
#include <bitcoin/network.hpp>

namespace libbitcoin {
namespace node {

// Construct with defaults derived from given context.
configuration::configuration(config::settings context)
  : help(false),

#if !defined(WITH_REMOTE_BLOCKCHAIN) && !defined(WITH_REMOTE_DATABASE)
    initchain(false),
#endif // !defined(WITH_REMOTE_BLOCKCHAIN) && !defined(WITH_REMOTE_DATABASE)    

    settings(false),
    version(false),
    node(context),
    chain(context),
    database(context),
    network(context)
{
}

//OLD MERGE MAY 2017 TODO:this code no longer exits...
//// Copy constructor.
//configuration::configuration(const configuration& other)
//  : help(other.help),
//
//#if !defined(WITH_REMOTE_BLOCKCHAIN) && !defined(WITH_REMOTE_DATABASE)
//    initchain(other.initchain),
//#endif // !defined(WITH_REMOTE_BLOCKCHAIN) && !defined(WITH_REMOTE_DATABASE)
//
//    settings(other.settings),
//    version(other.version),
//    file(other.file),
//    node(other.node),
//    chain(other.chain),
//    database(other.database),
//    network(other.network)
//{
//}

} // namespace node
} // namespace libbitcoin
