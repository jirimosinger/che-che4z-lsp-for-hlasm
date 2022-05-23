/*
 * Copyright (c) 2022 Broadcom.
 * The term "Broadcom" refers to Broadcom Inc. and/or its subsidiaries.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Broadcom, Inc. - initial API and implementation
 */

#include "utils/external_resource.h"

#include <filesystem>
#include <regex>

#include "network/uri/uri.hpp"

#include "utils/path.h"
#include "utils/platform.h"


namespace hlasm_plugin::utils::path {

// namespace {

const std::string untitled = "untitled";

std::string uri_to_path(const std::string& uri)
{
    network::uri u(uri);

    // vscode sometimes sends us uri in form 'untitled:Untitled-1',
    // when user opens new file that is not saved to disk yet
    if (!u.scheme().compare(untitled))
        return uri;

    if (u.scheme().compare("file"))
        return "";
    if (!u.has_path())
        return "";

    std::string auth_path;
    if (u.has_authority() && u.authority().to_string() != "")
    {
        auth_path = u.authority().to_string() + u.path().to_string();
        if (utils::platform::is_windows())
        {
            // handle remote locations correctly, like \\server\path
            auth_path = "//" + auth_path;
        }
    }
    else
    {
        network::string_view path = u.path();

        if (utils::platform::is_windows())
        {
            // we get path always beginning with / on windows, e.g. /c:/Users/path
            path.remove_prefix(1);
        }
        auth_path = path.to_string();

        if (utils::platform::is_windows())
        {
            auth_path[0] = (char)tolower((unsigned char)auth_path[0]);
        }
    }

    return utils::path::lexically_normal(network::detail::decode(auth_path)).string();
}

// one letter schemas are valid, but Windows paths collide
const std::regex uri_like("^[A-Za-z][A-Za-z0-9+\\-.]+:");

std::string path_to_uri(std::string_view path)
{
    if (std::regex_search(path.begin(), path.end(), uri_like))
        return std::string(path);

    // network::detail::encode_path(uri) ignores @, which is incompatible with VS Code
    std::string uri;
    auto out = std::back_inserter(uri);

    for (char c : path)
    {
        if (c == '\\')
            c = '/';
        network::detail::encode_char(c, out, "/.%;=");
    }

    if (utils::platform::is_windows())
    {
        // in case of remote address such as \\server\path\to\file
        if (uri.size() >= 2 && uri[0] == '/' && uri[1] == '/')
            uri.insert(0, "file:");
        else
            uri.insert(0, "file:///");
    }
    else
    {
        uri.insert(0, "file://");
    }

    return uri;
}

uri_type get_uri_type(const std::string& uri)
{
    try
    {
        network::uri u(uri);

        // vscode sometimes sends us uri in form 'untitled:Untitled-1',
        // when user opens new file that is not saved to disk yet
        if (!u.scheme().compare(untitled))
            return uri_type::UNTITLED;

        if (u.scheme().compare("file"))
            return uri_type::UNKNOWN;
        if (!u.has_path())
            return uri_type::UNKNOWN;

        if (u.has_authority() && u.authority().to_string() != "")
        {
            return uri_type::NETWORK;
        }
        else
        {
            return uri_type::LOCAL_URI;
        }
    }
    catch (const std::exception&)
    {
        return uri_type::CORRUPT;

    }
}

uri_type transform_uri_by_resource_type(std::string& uri)
{
    try
    {
        network::uri u(uri);

        // vscode sometimes sends us uri in form 'untitled:Untitled-1',
        // when user opens new file that is not saved to disk yet
        if (!u.scheme().compare(untitled))
            return uri_type::UNTITLED;

        if (u.scheme().compare("file"))
            return uri_type::UNKNOWN;
        if (!u.has_path())
            return uri_type::UNKNOWN;

        uri_type ret_val = uri_type::UNKNOWN;

        std::string auth_path;
        if (u.has_authority() && u.authority().to_string() != "")
        {
            auth_path = u.authority().to_string() + u.path().to_string();
            if (utils::platform::is_windows())
            {
                // handle remote locations correctly, like \\server\path
                auth_path = "//" + auth_path;
            }

            ret_val = uri_type::NETWORK;
        }
        else
        {
            network::string_view path = u.path();

            if (utils::platform::is_windows())
            {
                // we get path always beginning with / on windows, e.g. /c:/Users/path
                path.remove_prefix(1);
            }
            auth_path = path.to_string();

            if (utils::platform::is_windows())
            {
                auth_path[0] = (char)tolower((unsigned char)auth_path[0]);
            }

            ret_val = uri_type::LOCAL_URI;
        }

        uri = utils::path::lexically_normal(network::detail::decode(auth_path)).string();
        return ret_val;
    }
    catch (const std::exception&)
    {
        return uri_type::CORRUPT;
    }
}

std::optional<std::string> get_path(const std::string& uri, uri_type uri_type)
{
    switch (uri_type)
    {
        case uri_type::LOCAL_ABSOLUTE:
            return uri;
        case uri_type::LOCAL_RELATIVE:
            return uri; // todo
        case uri_type::LOCAL_URI:
            return uri_to_path(uri);
        default:
            return std::nullopt;
    }
}

//} // namespace

external_resource::external_resource(std::string uri)
    : m_uri(std::move(uri))
    , m_type("" == m_uri ? uri_type::UNKNOWN : get_uri_type(m_uri))
{
    // if (m_type == uri_type::CORRUPT)
    //     while (1)
}

external_resource::external_resource(std::string_view uri)
    : external_resource(std::string(uri))
{}

external_resource::external_resource(const char* uri)
    : external_resource(std::string(uri))
{}

external_resource::external_resource(const external_resource& r)
    : m_uri(r.m_uri)
    , m_type(r.m_type)
{}

const std::string& external_resource::get_url() const { return m_uri; }

std::string external_resource::get_absolute_path() const
{
    if (uri_type::LOCAL_URI == m_type)
        return uri_to_path(m_uri);

    return m_uri;
}

std::string external_resource::get_content() const { return std::string(); }

uri_type external_resource::get_type() const { return m_type; }


// bool external_resource::operator==(const external_resource& r) const { return m_uri == r.m_uri && m_type == r.m_type;
// }
bool external_resource::operator==(const external_resource& r) const { return m_uri == r.m_uri; }
bool external_resource::operator!=(const external_resource& r) const { return !operator==(r); }
bool external_resource::operator<(const external_resource& r) const { return m_uri < r.m_uri; }

} // namespace hlasm_plugin::utils::path