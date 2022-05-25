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

#ifndef HLASMPLUGIN_UTILS_URI_H
#define HLASMPLUGIN_UTILS_URI_H

#include <optional>
#include <string>

namespace hlasm_plugin::utils::path {

class external_resource
{
public:
    external_resource() = default;
    external_resource(std::string uri);
    external_resource(std::string_view uri);
    external_resource(const char* uri);

    // external_resource(external_resource&) = default;
    // external_resource& operator=(external_resource&) = default;
    external_resource(const external_resource&);
    external_resource& operator=(const external_resource&) = default;
    external_resource(external_resource&&) = default;
    external_resource& operator=(external_resource&&) = default;

    // template<class T>
    // external_resource(T) = delete;

    // template<class T>
    // external_resource(T, uri_type) = delete;

    const std::string& get_uri() const;
    std::string get_path() const;

    bool operator==(const external_resource& r) const;
    bool operator!=(const external_resource& r) const;
    bool operator<(const external_resource& r) const;

private:
    std::string m_uri;
};

struct external_resource_hasher
{
    std::size_t operator()(const external_resource& r) const;
};

struct external_resource_comp
{
    bool operator()(const external_resource& l, const external_resource& r) const;
};

// Converts URI (RFC3986) to common filesystem path.
std::string uri_to_path(const std::string& uri);

// Converts from filesystem path to URI
std::string path_to_uri(std::string_view path);

} // namespace hlasm_plugin::utils::path

#endif