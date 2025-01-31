/*
 * Copyright (c) 2019 Broadcom.
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

// Pimpl implementations of workspace manager. Also converts C-like parameters
// into C++ ones.

#include "workspace_manager.h"

#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "nlohmann/json.hpp"
#include "utils/resource_location.h"
#include "workspace_manager_impl.h"
#include "workspaces/file_manager_impl.h"
#include "workspaces/workspace.h"

namespace hlasm_plugin::parser_library {

workspace_manager::workspace_manager(std::atomic<bool>* cancel)
    : impl_(new impl(cancel))
{}

workspace_manager::workspace_manager(workspace_manager&& ws_mngr) noexcept
    : impl_(ws_mngr.impl_)
{
    ws_mngr.impl_ = nullptr;
}
workspace_manager& workspace_manager::operator=(workspace_manager&& ws_mngr) noexcept
{
    std::swap(impl_, ws_mngr.impl_);
    return *this;
}

workspace_manager::~workspace_manager() { delete impl_; }

size_t workspace_manager::get_workspaces(ws_id* workspaces, size_t max_size)
{
    return impl_->get_workspaces(workspaces, max_size);
}

size_t workspace_manager::get_workspaces_count() { return impl_->get_workspaces_count(); }

void workspace_manager::add_workspace(const char* name, const char* uri) { impl_->add_workspace(name, uri); }

ws_id workspace_manager::find_workspace(const char* document_uri) { return impl_->find_workspace(document_uri); }

void workspace_manager::remove_workspace(const char* uri) { impl_->remove_workspace(uri); }

void workspace_manager::did_change_watched_files(sequence<fs_change> fs_changes)
{
    std::vector<utils::resource::resource_location> paths_s;
    paths_s.reserve(fs_changes.size());
    std::transform(fs_changes.begin(), fs_changes.end(), std::back_inserter(paths_s), [](const auto& change) {
        return utils::resource::resource_location(static_cast<std::string_view>(change.uri));
    });
    impl_->did_change_watched_files(std::move(paths_s));
}

void workspace_manager::did_open_file(const char* document_uri, version_t version, const char* text, size_t text_size)
{
    impl_->did_open_file(utils::resource::resource_location(document_uri), version, std::string(text, text_size));
}
void workspace_manager::did_change_file(
    const char* document_uri, version_t version, const document_change* changes, size_t ch_size)
{
    impl_->did_change_file(utils::resource::resource_location(document_uri), version, changes, ch_size);
}

void workspace_manager::did_close_file(const char* document_uri)
{
    impl_->did_close_file(utils::resource::resource_location(document_uri));
}

void workspace_manager::configuration_changed(const lib_config& new_config, const char* whole_settings)
{
    impl_->configuration_changed(
        new_config, std::make_shared<const nlohmann::json>(nlohmann::json::parse(std::string_view(whole_settings))));
}

void workspace_manager::register_diagnostics_consumer(diagnostics_consumer* consumer)
{
    impl_->register_diagnostics_consumer(consumer);
}

void workspace_manager::unregister_diagnostics_consumer(diagnostics_consumer* consumer)
{
    impl_->unregister_diagnostics_consumer(consumer);
}

void workspace_manager::register_parsing_metadata_consumer(parsing_metadata_consumer* consumer)
{
    impl_->register_parsing_metadata_consumer(consumer);
}

void workspace_manager::unregister_parsing_metadata_consumer(parsing_metadata_consumer* consumer)
{
    impl_->unregister_parsing_metadata_consumer(consumer);
}

void workspace_manager::set_message_consumer(message_consumer* consumer) { impl_->set_message_consumer(consumer); }

position_uri workspace_manager::definition(const char* document_uri, const position pos)
{
    return impl_->definition(document_uri, pos);
}

position_uri_list workspace_manager::references(const char* document_uri, const position pos)
{
    return impl_->references(document_uri, pos);
}

sequence<char> workspace_manager::hover(const char* document_uri, const position pos)
{
    return sequence<char>(impl_->hover(document_uri, pos));
}

completion_list workspace_manager::completion(
    const char* document_uri, const position pos, const char trigger_char, completion_trigger_kind trigger_kind)
{
    return impl_->completion(document_uri, pos, trigger_char, trigger_kind);
}

sequence<token_info> workspace_manager::semantic_tokens(const char* document_uri)
{
    return sequence<token_info>(impl_->semantic_tokens(document_uri));
}

document_symbol_list workspace_manager::document_symbol(const char* document_uri, long long limit)
{
    return impl_->document_symbol(document_uri, limit);
}


continuous_sequence<char> workspace_manager::get_virtual_file_content(unsigned long long id) const
{
    return impl_->get_virtual_file_content(id);
}

} // namespace hlasm_plugin::parser_library
