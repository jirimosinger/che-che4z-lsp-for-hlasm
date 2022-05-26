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

#include "utils/resource_location.h"

#include <filesystem>
#include <regex>

#include "network/uri/uri.hpp"

#include "utils/path.h"
#include "utils/platform.h"

namespace hlasm_plugin::utils::path {
namespace {
const std::string untitled = "untitled";
const std::string hlasm = "hlasm";
} // namespace

std::string uri_to_path(const std::string& uri)
{
    try
    {
        network::uri u(uri);

        // vscode sometimes sends us uri in form 'untitled:Untitled-1',
        // when user opens new file that is not saved to disk yet
        if (!u.scheme().compare(untitled))
            return uri;
        // There is a hlasm specific schema - don't convert to path
        if (!u.scheme().compare(hlasm))
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
    catch (const std::exception&)
    {
        return uri;
    }
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

resource_location::resource_location(std::string uri)
    : m_uri(std::move(uri))
{}

resource_location::resource_location(std::string_view uri)
    : resource_location(std::string(uri))
{}

resource_location::resource_location(const char* uri)
    : resource_location(std::string(uri))
{}

const std::string& resource_location::get_uri() const { return m_uri; }

std::string resource_location::get_path() const { return m_uri.size() != 0 ? uri_to_path(m_uri) : m_uri; }

bool resource_location::operator==(const resource_location& r) const { return m_uri == r.m_uri; }
bool resource_location::operator<(const resource_location& r) const { return m_uri < r.m_uri; }

std::size_t resource_location_hasher::operator()(const resource_location& r) const
{
    return std::hash<std::string> {}(r.get_uri());
}
} // namespace hlasm_plugin::utils::path