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

#ifndef HLASMPLUGIN_UTILS_CONTENT_LOADER_H
#define HLASMPLUGIN_UTILS_CONTENT_LOADER_H

#include <optional>
#include <string>
#include "resource_location.h"

namespace hlasm_plugin::utils::resource {

class content_loader
{
public:
    virtual ~content_loader() = default;
    virtual std::optional<std::string> load_text(const resource_location& res_loc) const = 0;
};

std::optional<std::string> load_text(const resource_location& res_loc);

} // namespace hlasm_plugin::utils::resource

#endif