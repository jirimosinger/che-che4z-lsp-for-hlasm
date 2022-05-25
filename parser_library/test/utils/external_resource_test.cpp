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

#include "gtest/gtest.h"

#include "utils/external_resource.h"
#include "utils/platform.h"

using namespace hlasm_plugin;
using namespace hlasm_plugin::utils::platform;
using namespace hlasm_plugin::utils::path;

std::pair<std::string, std::string> path_1 = is_windows()
    ? std::make_pair("file://czprfs50/Public", "\\\\czprfs50\\Public")
    : std::make_pair("file:///home/user/somefile", "/home/user/somefile");

std::pair<std::string, std::string> path_2 = is_windows() ? std::make_pair("file:///C%3A/Public", "c:\\Public")
                                                          : std::make_pair("file:///C%3A/Public", "/C:/Public");

TEST(external_resource, uri_to_path)
{
    if (is_windows())
    {
        EXPECT_EQ(uri_to_path("file://czprfs50/Public"), "\\\\czprfs50\\Public");
        EXPECT_EQ(uri_to_path("file:///C%3A/Public"), "c:\\Public");
    }
    else
    {
        EXPECT_EQ(uri_to_path("file:///home/user/somefile"), "/home/user/somefile");
        EXPECT_EQ(uri_to_path("file:///C%3A/Public"), "/C:/Public");
    }
}

TEST(external_resource, path_to_uri)
{
    if (is_windows())
    {
        EXPECT_EQ(path_to_uri("\\\\czprfs50\\Public"), "file://czprfs50/Public");
        EXPECT_EQ(path_to_uri("c:\\Public"), "file:///c%3A/Public");
    }
    else
    {
        EXPECT_EQ(path_to_uri("/home/user/somefile"), "file:///home/user/somefile");
        EXPECT_EQ(path_to_uri("/C:/Public"), "file:///C%3A/Public");
    }
}

 // todo write tests
