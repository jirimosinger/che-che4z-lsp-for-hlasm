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

#include "gtest/gtest.h"

#include "diagnostic.h"
#include "utils/platform.h"
#include "workspaces/file_manager_impl.h"
#include "workspaces/library_local.h"
#include "workspaces/workspace.h"

using namespace hlasm_plugin::parser_library::workspaces;
using namespace hlasm_plugin::utils::resource;

namespace {
const auto lib_loc =
    hlasm_plugin::utils::platform::is_windows() ? resource_location("lib\\") : resource_location("lib/");
const auto lib2_loc =
    hlasm_plugin::utils::platform::is_windows() ? resource_location("lib2\\") : resource_location("lib2/");
} // namespace

class file_manager_extension_mock : public file_manager_impl
{
    list_directory_result list_directory_files(const resource_location&) const override
    {
        return { { { "Mac.hlasm", resource_location::join(lib_loc, "Mac.hlasm") } },
            hlasm_plugin::utils::path::list_directory_rc::done };
    }
};

TEST(extension_handling_test, extension_removal)
{
    file_manager_extension_mock file_mngr;
    // file must end with hlasm
    library_local lib(file_mngr, lib_loc, { { ".hlasm" } });
    EXPECT_NE(lib.find_file("MAC"), nullptr);

    // file must end with hlasm
    library_local lib2(file_mngr, lib_loc, { { ".hlasm" } });
    EXPECT_NE(lib2.find_file("MAC"), nullptr);

    // file must end with asm
    library_local lib3(file_mngr, lib_loc, { { ".asm" } });
    EXPECT_EQ(lib3.find_file("MAC"), nullptr);

    // test multiple extensions
    library_local lib4(file_mngr, lib2_loc, { { ".hlasm", ".asm" } });
    EXPECT_NE(lib4.find_file("MAC"), nullptr);

    // test no extensions
    library_local lib5(file_mngr, lib2_loc, { {} });
    EXPECT_EQ(lib5.find_file("MAC"), nullptr);

    // test no extensions
    library_local lib6(file_mngr, lib2_loc, { { "" } });
    EXPECT_EQ(lib6.find_file("MAC"), nullptr);

    // tolerate missing dot
    library_local lib7(file_mngr, lib_loc, { { "hlasm", "asm" } });
    EXPECT_NE(lib7.find_file("MAC"), nullptr);
}

TEST(extension_handling_test, legacy_extension_selection)
{
    file_manager_extension_mock file_mngr;
    library_local lib(file_mngr, lib_loc, { { ".hlasm" }, true });
    EXPECT_NE(lib.find_file("MAC"), nullptr);
    lib.collect_diags();
    const auto& diags = lib.diags();
    EXPECT_TRUE(std::any_of(diags.begin(), diags.end(), [](const auto& d) { return d.code == "L0003"; }));
}

class file_manager_extension_mock2 : public file_manager_impl
{
    list_directory_result list_directory_files(const resource_location&) const override
    {
        return { { { "Mac.hlasm", resource_location::join(lib_loc, "Mac.hlasm") },
                     { "Mac", resource_location::join(lib_loc, "Mac") } },
            hlasm_plugin::utils::path::list_directory_rc::done };
    }
};

TEST(extension_handling_test, multiple_macro_definitions)
{
    file_manager_extension_mock2 file_mngr;
    library_local lib(file_mngr, lib_loc, { { ".hlasm", "" } });
    EXPECT_NE(lib.find_file("MAC"), nullptr);
    lib.collect_diags();
    const auto& diags = lib.diags();
    EXPECT_TRUE(std::any_of(diags.begin(), diags.end(), [](const auto& d) { return d.code == "L0004"; }));
}

TEST(extension_handling_test, no_multiple_macro_definitions)
{
    file_manager_extension_mock2 file_mngr;
    library_local lib(file_mngr, lib_loc, { { ".hlasm" } });
    EXPECT_NE(lib.find_file("MAC"), nullptr);
    lib.collect_diags();
    const auto& diags = lib.diags();
    EXPECT_TRUE(std::none_of(diags.begin(), diags.end(), [](const auto& d) { return d.code == "L0004"; }));
}

class file_manager_extension_mock_no_ext : public file_manager_impl
{
    list_directory_result list_directory_files(const resource_location&) const override
    {
        return { { { "Mac", resource_location::join(lib_loc, "Mac") } },
            hlasm_plugin::utils::path::list_directory_rc::done };
    }
};

TEST(extension_handling_test, legacy_extension_selection_file_without_ext)
{
    file_manager_extension_mock_no_ext file_mngr;
    library_local lib(file_mngr, lib_loc, { { ".hlasm" }, true });
    EXPECT_NE(lib.find_file("MAC"), nullptr);
    lib.collect_diags();
    const auto& diags = lib.diags();
    EXPECT_TRUE(std::none_of(diags.begin(), diags.end(), [](const auto& d) { return d.code == "L0003"; }));
}
