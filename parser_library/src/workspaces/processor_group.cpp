/*
 * Copyright (c) 2021 Broadcom.
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

#include "processor_group.h"

namespace hlasm_plugin::parser_library::workspaces {

namespace {
struct translate_pp_options
{
    preprocessor_options operator()(const config::db2_preprocessor& opt) const
    {
        return db2_preprocessor_options(opt.version, opt.conditional);
    }
    preprocessor_options operator()(const config::cics_preprocessor& opt) const
    {
        return cics_preprocessor_options(opt.prolog, opt.epilog, opt.leasm);
    }
};

std::vector<preprocessor_options> translate_pp_configs(const std::vector<config::preprocessor_options>& pp)
{
    std::vector<preprocessor_options> result;
    result.reserve(pp.size());
    std::transform(pp.begin(), pp.end(), std::back_inserter(result), [](const auto& p) {
        return std::visit(translate_pp_options {}, p.options);
    });
    return result;
}
} // namespace

processor_group::processor_group(const std::string& pg_name,
    const config::assembler_options& asm_options,
    const std::vector<config::preprocessor_options>& pp)
    : m_pg_name(pg_name)
    , m_asm_opts(asm_options)
    , m_prep_opts(translate_pp_configs(pp))
{}

void processor_group::update_asm_options(asm_option& opts) const { m_asm_opts.apply(opts); }

void processor_group::collect_diags() const
{
    for (auto&& lib : m_libs)
    {
        collect_diags_from_child(*lib);
    }
}

void processor_group::add_library(std::unique_ptr<library> library) { m_libs.push_back(std::move(library)); }

} // namespace hlasm_plugin::parser_library::workspaces
