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

#include "assembler_options.h"

#include "compiler_options.h"
#include "nlohmann/json.hpp"

namespace hlasm_plugin::parser_library::config {


namespace {
bool optable_valid(std::string_view optable) noexcept
{
#ifdef __cpp_lib_ranges
    return optable.size() == 0
        || std::ranges::any_of(instr_set_version_equivalents, [optable](auto item) { return optable == item.first; });
#else
    return optable.size() == 0
        || std::any_of(std::begin(instr_set_version_equivalents),
            std::end(instr_set_version_equivalents),
            [optable](auto item) { return optable == item.first; });
#endif
}
} // namespace

bool assembler_options::valid() const noexcept
{
    if (sysparm.has_value() && sysparm.value().size() >= 256)
        return false;
    if (optable.has_value() && !optable_valid(optable.value()))
        return false;
    return true;
}

namespace {
std::optional<instruction_set_version> find_instruction_set(std::string_view optable)
{
#ifdef __cpp_lib_ranges
    auto it = std::ranges::lower_bound(
        instr_set_version_equivalents, optable, {}, [](const auto& instr) { return instr.first; });
#else
    auto it = std::lower_bound(std::begin(instr_set_version_equivalents),
        std::end(instr_set_version_equivalents),
        optable,
        [](const auto& l, const auto& r) { return l.first < r; });
#endif

    if (it == std::end(instr_set_version_equivalents) || it->first != optable)
        return std::nullopt;

    return it->second;
}
} // namespace

void assembler_options::apply(asm_option& opts) const
{
    if (sysparm.has_value())
        opts.sysparm = sysparm.value();
    if (profile.has_value())
        opts.profile = profile.value();
    if (optable.has_value())
    {
        if (auto translated = find_instruction_set(optable.value()); translated.has_value())
        {
            opts.instr_set = translated.value();
        }
    }
    if (system_id.has_value())
        opts.system_id = system_id.value();
    if (goff.has_value())
        opts.sysopt_xobject = goff.value();
}

bool assembler_options::has_value() const noexcept
{
    return sysparm.has_value() || profile.has_value() || optable.has_value() || system_id.has_value()
        || goff.has_value();
}

template<typename T>
void optional_to_json(nlohmann::json& j, std::string name, const std::optional<T>& o)
{
    if (o.has_value())
        j[name] = o.value();
}

template<typename T>
void optional_from_json(const nlohmann::json& j, std::optional<T>& o)
{
    if (o.has_value())
        throw nlohmann::json::other_error::create(501, "Parameter redefined", j);

    o = j.get<T>();
}

void to_json(nlohmann::json& j, const assembler_options& p)
{
    j = nlohmann::json::object();
    optional_to_json(j, "GOFF", p.goff); // GOFF and XOBJECTS are synonyms
    optional_to_json(j, "OPTABLE", p.optable);
    optional_to_json(j, "PROFILE", p.profile);
    optional_to_json(j, "SYSPARM", p.sysparm);
    optional_to_json(j, "SYSTEM_ID", p.system_id);
}

namespace {
using from_json_helper_function = std::function<void(const nlohmann::json&, assembler_options&)>;

const auto profile_json = from_json_helper_function(
    [](const nlohmann::json& val, assembler_options& p) { optional_from_json(val, p.profile); });

const auto sysparm_json = from_json_helper_function(
    [](const nlohmann::json& val, assembler_options& p) { optional_from_json(val, p.sysparm); });

const auto system_id_json = from_json_helper_function(
    [](const nlohmann::json& val, assembler_options& p) { optional_from_json(val, p.system_id); });

const auto optable_json = from_json_helper_function(
    [](const nlohmann::json& val, assembler_options& p) { optional_from_json(val, p.optable); });

const auto goff_json =
    from_json_helper_function([](const nlohmann::json& val, assembler_options& p) { optional_from_json(val, p.goff); });

struct string_hash
{
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const
    {
        std::hash<std::string_view> hasher;
        return hasher(sv);
    }
};

const std::unordered_map<std::string, from_json_helper_function, string_hash, std::equal_to<>> from_json_map = {
    { "PROFILE", profile_json },
    { "SYSPARM", sysparm_json },
    { "SYSTEM_ID", system_id_json },
    { "GOFF", goff_json },
    { "XOBJECT", goff_json },
    { "OPTABLE", optable_json },
    { "MACHINE", optable_json }
};
} // namespace

void from_json(const nlohmann::json& j, assembler_options& p)
{
    if (!j.is_object())
        throw nlohmann::json::other_error::create(501, "asm_options must be an object.", j);

    for (const auto& item : j.items())
    {
        const auto& key = item.key();

        if (auto it = from_json_map.find(key); it != from_json_map.end())
        {
            it->second(item.value(), p);
        }
    }
}
} // namespace hlasm_plugin::parser_library::config
