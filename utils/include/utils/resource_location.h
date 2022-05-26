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

#ifndef HLASMPLUGIN_UTILS_RESOURCE_LOCATION_H
#define HLASMPLUGIN_UTILS_RESOURCE_LOCATION_H

#include <optional>
#include <string>

namespace hlasm_plugin::utils::path {

// Converts URI (RFC3986) to common filesystem path.
std::string uri_to_path(const std::string& uri);

// Converts from filesystem path to URI
std::string path_to_uri(std::string_view path);

class resource_location
{
public:
    resource_location() = default;
    resource_location(std::string uri);
    resource_location(std::string_view uri);
    resource_location(const char* uri);
    resource_location(const resource_location&) = default;
    resource_location& operator=(const resource_location&) = default;

    const std::string& get_uri() const;
    std::string get_path() const;

    bool operator==(const resource_location& r) const;
    bool operator<(const resource_location& r) const;

private:
    std::string m_uri;
};

struct resource_location_hasher
{
    std::size_t operator()(const resource_location& r) const;
};

} // namespace hlasm_plugin::utils::path

#endif