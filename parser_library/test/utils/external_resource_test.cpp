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

TEST(external_resource, empty_uri)
{
    external_resource res("");
    EXPECT_EQ(res.get_uri(), "");
    EXPECT_EQ(res.get_path(), "");
}

TEST(external_resource, invalid_uri)
{
    external_resource res("src/temp");
    EXPECT_EQ(res.get_uri(), "src/temp");
    EXPECT_EQ(res.get_path(), "src/temp");
}

TEST(external_resource, non_supported_uri)
{
    external_resource res("aaa://src/temp");
    EXPECT_EQ(res.get_uri(), "aaa://src/temp");
    EXPECT_EQ(res.get_path(), "");
}

TEST(external_resource, file_uri)
{
    if (is_windows())
    {
        external_resource res("file:///c%3A/Public");
        EXPECT_EQ(res.get_uri(), "file:///c%3A/Public");
        EXPECT_EQ(res.get_path(), "c:\\Public");
    }
    else
    {
        external_resource res("file:///home/user/somefile");
        EXPECT_EQ(res.get_uri(), "file:///home/user/somefile");
        EXPECT_EQ(res.get_path(), "/home/user/somefile");
    }
}

TEST(external_resource, hlasm_uri)
{
    external_resource res("hlasm://0/AINSERT:1");
    EXPECT_EQ(res.get_uri(), "hlasm://0/AINSERT:1");
    EXPECT_EQ(res.get_path(), "hlasm://0/AINSERT:1");
}

TEST(external_resource, untitled_uri)
{
    external_resource res("untitled:Untitled-1");
    EXPECT_EQ(res.get_uri(), "untitled:Untitled-1");
    EXPECT_EQ(res.get_path(), "untitled:Untitled-1");
}