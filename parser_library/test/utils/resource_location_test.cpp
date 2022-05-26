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

#include "utils/platform.h"
#include "utils/resource_location.h"

using namespace hlasm_plugin;
using namespace hlasm_plugin::utils::platform;
using namespace hlasm_plugin::utils::path;

TEST(resource_location, uri_to_path)
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

TEST(resource_location, path_to_uri)
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

TEST(resource_location, empty_uri)
{
    resource_location res("");
    EXPECT_EQ(res.get_uri(), "");
    EXPECT_EQ(res.get_path(), "");
}

TEST(resource_location, invalid_uri)
{
    resource_location res("src/temp");
    EXPECT_EQ(res.get_uri(), "src/temp");
    EXPECT_EQ(res.get_path(), "src/temp");
}

TEST(resource_location, non_supported_uri)
{
    resource_location res("aaa://src/temp");
    EXPECT_EQ(res.get_uri(), "aaa://src/temp");
    EXPECT_EQ(res.get_path(), "");
}

TEST(resource_location, file_uri)
{
    if (is_windows())
    {
        resource_location res("file:///c%3A/Public");
        EXPECT_EQ(res.get_uri(), "file:///c%3A/Public");
        EXPECT_EQ(res.get_path(), "c:\\Public");
    }
    else
    {
        resource_location res("file:///home/user/somefile");
        EXPECT_EQ(res.get_uri(), "file:///home/user/somefile");
        EXPECT_EQ(res.get_path(), "/home/user/somefile");
    }
}

TEST(resource_location, hlasm_uri)
{
    resource_location res("hlasm://0/AINSERT:1");
    EXPECT_EQ(res.get_uri(), "hlasm://0/AINSERT:1");
    EXPECT_EQ(res.get_path(), "hlasm://0/AINSERT:1");
}

TEST(resource_location, untitled_uri)
{
    resource_location res("untitled:Untitled-1");
    EXPECT_EQ(res.get_uri(), "untitled:Untitled-1");
    EXPECT_EQ(res.get_path(), "untitled:Untitled-1");
}