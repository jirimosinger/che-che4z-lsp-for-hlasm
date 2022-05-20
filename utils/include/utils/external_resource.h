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

enum class uri_type
{
    ABSOLUTE_PATH,
    RELATIVE_PATH,
    URL,
    LOCAL,
    LOCAL_URI,
    LOCAL_ABSOLUTE,
    LOCAL_RELATIVE,
    NETWORK,
    UNTITLED,
    UNKNOWN,
    CORRUPT
};

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

    std::string get_absolute_path() const;

    const std::string& get_url() const;

    std::string get_content() const;

    uri_type get_type() const;

    bool operator==(const external_resource& r) const;
    bool operator!=(const external_resource& r) const;
    bool operator<(const external_resource& r) const;

private:
    std::string m_uri;
    uri_type m_type;
};

struct external_resource_hasher
{
    std::size_t operator()(const external_resource& r) const { return std::hash<std::string> {}(r.get_url()); }
};

struct external_resource_comp
{
    bool operator()(const external_resource& l, const external_resource& r) const { return l.get_url() < r.get_url(); }
};

// Converts URI (RFC3986) to common filesystem path.
std::string uri_to_path(const std::string& uri);

// Converts from filesystem path to URI
std::string path_to_uri(std::string_view path);

} // namespace hlasm_plugin::utils::path

#endif