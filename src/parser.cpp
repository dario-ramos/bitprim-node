/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin-server.
 *
 * libbitcoin-server is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/node/parser.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <bitcoin/blockchain.hpp>
#include <bitcoin/network.hpp>
#include <bitcoin/node/p2p_node.hpp>
#include <bitcoin/node/settings.hpp>

BC_DECLARE_CONFIG_DEFAULT_PATH("libbitcoin" / "bn.cfg")

// TODO: localize descriptions.

namespace libbitcoin {
namespace node {
    
using namespace boost::filesystem;
using namespace boost::program_options;
using namespace bc::config;

options_metadata parser::load_options()
{
    options_metadata description("options");
    description.add_options()
    (
        BN_CONFIG_VARIABLE,
        value<path>(&settings.file),
        "The path to the configuration settings file."
    )
    (
        BN_HELP_VARIABLE ",h",
        value<bool>(&settings.help)->
            default_value(false)->zero_tokens(),
        "Get list of options for this command."
    )
    (
        "mainnet,m",
        value<bool>(&settings.main_network)->
            default_value(false)->zero_tokens(),
        "Initialize mainnet in the configured directory."
    )
    (
        BN_SETTINGS_VARIABLE ",s",
        value<bool>(&settings.settings)->
            default_value(false)->zero_tokens(),
        "Display the loaded configuration settings."
    )
    (
        "testnet,t",
        value<bool>(&settings.test_network)->
            default_value(false)->zero_tokens(),
        "Initialize testnet in the configured directory."
    )
    (
        BN_VERSION_VARIABLE ",v",
        value<bool>(&settings.version)->
            default_value(false)->zero_tokens(),
        "Get version information."
    );

    return description;
}

arguments_metadata parser::load_arguments()
{
    arguments_metadata description;
    return description
        .add(BN_CONFIG_VARIABLE, 1);
}

options_metadata parser::load_environment()
{
    options_metadata description("environment");
    description.add_options()
    (
        // For some reason po requires this to be a lower case name.
        // The case must match the other declarations for it to compose.
        // This composes with the cmdline options and inits to system path.
        BN_CONFIG_VARIABLE,
        value<path>(&settings.file)->composing()
            ->default_value(config_default_path()),
        "The path to the configuration settings file."
    );

    return description;
}

options_metadata parser::load_settings()
{
    options_metadata description("settings");
    description.add_options()
    /* [network] */
    (
        "network.threads",
        value<uint32_t>(&settings.network.threads)->
            default_value(network::settings::mainnet.threads),
        "The number of threads in the network threadpool, defaults to 4."
    )
    (
        "network.identifier",
        value<uint32_t>(&settings.network.identifier)->
            default_value(network::settings::mainnet.identifier),
        "The magic number for message headers, defaults to 3652501241."
    )
    (
        "network.inbound_port",
        value<uint16_t>(&settings.network.inbound_port)->
            default_value(network::settings::mainnet.inbound_port),
        "The port for incoming connections, defaults to 8333."
    )
    (
        "network.connection_limit",
        value<uint32_t>(&settings.network.connection_limit)->
            default_value(network::settings::mainnet.connection_limit),
        "The target number of total network connections, defaults to 16."
    )
    (
        "network.outbound_connections",
        value<uint32_t>(&settings.network.outbound_connections)->
            default_value(network::settings::mainnet.outbound_connections),
        "The target number of outgoing network connections, defaults to 8."
    )
    (
        "network.manual_retry_limit",
        value<uint32_t>(&settings.network.manual_retry_limit)->
            default_value(network::settings::mainnet.manual_retry_limit),
        "The retry limit for manual connection establishment, defaults to 0 (forever)."
    )
    (
        "network.connect_batch_size",
        value<uint32_t>(&settings.network.connect_batch_size)->
            default_value(network::settings::mainnet.connect_batch_size),
        "The number of concurrent attempts to estalish one connection, defaults to 5."
    )
    (
        "network.connect_timeout_seconds",
        value<uint32_t>(&settings.network.connect_timeout_seconds)->
            default_value(network::settings::mainnet.connect_timeout_seconds),
        "The time limit for connection establishment, defaults to 5."
    )
    (
        "network.channel_handshake_seconds",
        value<uint32_t>(&settings.network.channel_handshake_seconds)->
            default_value(network::settings::mainnet.channel_handshake_seconds),
        "The time limit to complete the connection handshake, defaults to 30."
    )
    (
        "network.channel_poll_seconds",
        value<uint32_t>(&settings.network.channel_poll_seconds)->
            default_value(network::settings::mainnet.channel_poll_seconds),
        "The polling interval for initial block download, defaults to 1."
    )
    (
        "network.channel_heartbeat_minutes",
        value<uint32_t>(&settings.network.channel_heartbeat_minutes)->
            default_value(network::settings::mainnet.channel_heartbeat_minutes),
        "The time between ping messages, defaults to 5."
    )
    (
        "network.channel_inactivity_minutes",
        value<uint32_t>(&settings.network.channel_inactivity_minutes)->
            default_value(network::settings::mainnet.channel_inactivity_minutes),
        "The inactivity time limit for any connection, defaults to 30."
    )
    (
        "network.channel_expiration_minutes",
        value<uint32_t>(&settings.network.channel_expiration_minutes)->
            default_value(network::settings::mainnet.channel_expiration_minutes),
        "The maximum age limit for an outbound connection, defaults to 90."
    )
    (
        "network.channel_germination_seconds",
        value<uint32_t>(&settings.network.channel_germination_seconds)->
            default_value(network::settings::mainnet.channel_germination_seconds),
        "The maximum time limit for obtaining seed addresses, defaults to 30."
    )
    (
        "network.host_pool_capacity",
        value<uint32_t>(&settings.network.host_pool_capacity)->
            default_value(network::settings::mainnet.host_pool_capacity),
        "The maximum number of peer hosts in the pool, defaults to 1000."
    )
    (
        "network.relay_transactions",
        value<bool>(&settings.network.relay_transactions)->
            default_value(network::settings::mainnet.relay_transactions),
        "Request that peers relay transactions, defaults to true."
    )
    (
        "network.hosts_file",
        value<path>(&settings.network.hosts_file)->
            default_value(network::settings::mainnet.hosts_file),
        "The peer hosts cache file path, defaults to 'hosts.cache'."
    )
    (
        "network.debug_file",
        value<path>(&settings.network.debug_file)->
            default_value(network::settings::mainnet.debug_file),
        "The debug log file path, defaults to 'debug.log'."
    )
    (
        "network.error_file",
        value<path>(&settings.network.error_file)->
            default_value(network::settings::mainnet.error_file),
        "The error log file path, defaults to 'error.log'."
    )
    (
        "network.self",
        value<config::authority>(&settings.network.self)->
            multitoken()->default_value(network::settings::mainnet.self),
        "The advertised public address of this node, defaults to none."
    )
    (
        "network.blacklist",
        value<config::authority::list>(&settings.network.blacklists)->
            multitoken()->default_value(network::settings::mainnet.blacklists),
        "IP address to disallow as a peer, multiple entries allowed."
    )
    (
        "network.seed",
        value<config::endpoint::list>(&settings.network.seeds)->
            multitoken()->default_value(network::settings::mainnet.seeds),
        "A seed node for initializing the host pool, multiple entries allowed."
    )

    /* [blockchain] */
    (
        "blockchain.threads",
        value<uint32_t>(&settings.chain.threads)->
            default_value(blockchain::settings::mainnet.threads),
        "The number of threads in the blockchain threadpool, defaults to 6."
    )
    (
        "blockchain.history_start_height",
        value<uint32_t>(&settings.chain.history_start_height)->
            default_value(blockchain::settings::mainnet.history_start_height),
        "The history index start height, defaults to 0."
    )
    (
        "blockchain.block_pool_capacity",
        value<uint32_t>(&settings.chain.block_pool_capacity)->
            default_value(blockchain::settings::mainnet.block_pool_capacity),
        "The maximum number of orphan blocks in the pool, defaults to 50."
    )
    (
        "blockchain.transaction_pool_capacity",
        value<uint32_t>(&settings.chain.transaction_pool_capacity)->
            default_value(blockchain::settings::mainnet.transaction_pool_capacity),
        "The maximum number of transactions in the pool, defaults to 2000."
    )
    (
        "blockchain.transaction_pool_consistency",
        value<bool>(&settings.chain.transaction_pool_consistency)->
            default_value(blockchain::settings::mainnet.transaction_pool_consistency),
        "Enforce consistency between the pool and the blockchain, defaults to false."
    )
    (
        "blockchain.use_testnet_rules",
        value<bool>(&settings.chain.use_testnet_rules)->
            default_value(blockchain::settings::mainnet.use_testnet_rules),
        "Use testnet rules for determination of work required, defaults to false."
    )
    (
        "blockchain.database_path",
        value<path>(&settings.chain.database_path)->
            default_value(blockchain::settings::mainnet.database_path),
        "The blockchain database directory, defaults to 'blockchain'."
    )
    (
        "blockchain.checkpoint",
        value<config::checkpoint::list>(&settings.chain.checkpoints)->
            multitoken()->default_value(blockchain::settings::mainnet.checkpoints),
        "A hash:height checkpoint, multiple entries allowed."
    )

    /* [node] */
    (
        "node.threads",
        value<uint32_t>(&settings.node.threads)->
            default_value(node::settings::defaults.threads),
        "The number of threads in the node threadpool, defaults to 4."
    )
    (
        "node.quorum",
        value<uint32_t>(&settings.node.quorum)->
            default_value(node::settings::defaults.quorum),
        "The number of peers to survey during sync, defaults to 8."
    )
    (
        "node.blocks_per_second",
        value<uint32_t>(&settings.node.blocks_per_second)->
            default_value(node::settings::defaults.blocks_per_second),
        "The minimum block rate required from a peer during block sync, defaults to 16."
    )
    (
        "node.headers_per_second",
        value<uint32_t>(&settings.node.headers_per_second)->
            default_value(node::settings::defaults.headers_per_second),
        "The minimum header rate required from a peer during header sync, defaults to 10000."
    )
    (
        "node.peer",
        value<config::endpoint::list>(&settings.node.peers)->
            multitoken()->default_value(node::settings::defaults.peers),
        "Persistent host:port to augment discovered hosts, multiple entries allowed."
    );

    return description;
}

bool parser::parse(std::string& out_error, int argc, const char* argv[])
{
    try
    {
        auto file = false;
        variables_map variables;
        load_command_variables(variables, argc, argv);
        load_environment_variables(variables, BN_ENVIRONMENT_VARIABLE_PREFIX);

        // Don't load the rest if any of these options are specified.
        if (!get_option(variables, BN_VERSION_VARIABLE) && 
            !get_option(variables, BN_SETTINGS_VARIABLE) &&
            !get_option(variables, BN_HELP_VARIABLE))
        {
            // Returns true if the settings were loaded from a file.
            file = load_configuration_variables(variables, BN_CONFIG_VARIABLE);
        }

        // Update bound variables in metadata.settings.
        notify(variables);

        // Clear the config file path if it wasn't used.
        if (!file)
            settings.file.clear();
    }
    catch (const boost::program_options::error& e)
    {
        // This is obtained from boost, which circumvents our localization.
        out_error = e.what();
        return false;
    }

    return true;
}

} // namespace node
} // namespace libbitcoin
