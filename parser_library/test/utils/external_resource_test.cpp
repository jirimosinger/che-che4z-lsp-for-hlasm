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

//TEST(external_resource, external_resource_local_relative)
//{
//    std::string uri = is_windows() ? "folder" : "user/somefile";
//
//    external_resource res(uri, uri_type::UNKNOWN);
//    EXPECT_EQ(res.get_type(), uri_type::LOCAL);
//}
//
//TEST(external_resource, external_resource_local_absolute)
//{
//    std::string uri = is_windows() ? "c:\\Public" : "/home/user/somefile";
//
//    external_resource res(uri, uri_type::UNKNOWN);
//    EXPECT_EQ(res.get_type(), uri_type::LOCAL);
//}

TEST(external_resource, external_resource_local_absolute_schema)
{
    std::string uri = is_windows() ? "file:///C%3A/Public" : "file:///home/user/somefile";

    external_resource res(uri, uri_type::UNKNOWN);
    EXPECT_EQ(res.get_type(), uri_type::LOCAL);
}

TEST(external_resource, external_resource_network)
{
    std::string uri = "file://czprfs50/Public";

    external_resource res(uri, uri_type::UNKNOWN);
    EXPECT_EQ(res.get_type(), uri_type::NETWORK);
}

TEST(external_resource, external_resource_untitled)
{
    std::string uri = "untitled:Untitled-1";

    external_resource res(uri, uri_type::UNKNOWN);
    EXPECT_EQ(res.get_type(), uri_type::UNTITLED);
}