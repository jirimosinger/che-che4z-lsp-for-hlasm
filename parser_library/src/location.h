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

#ifndef HLASMPLUGIN_PARSERLIBRARY_LOCATION_H
#define HLASMPLUGIN_PARSERLIBRARY_LOCATION_H

#include <string>
#include <vector>

#include "range.h"
#include "utils/resource_location.h"

namespace hlasm_plugin::parser_library {

struct location : public utils::path::resource_location
{
    location() = default;
    location(position pos, utils::path::resource_location file)
        : utils::path::resource_location(std::move(file))
        , pos(pos)
    {}
    bool operator==(const location& oth) const { return pos == oth.pos && get_uri() == oth.get_uri(); }
    position pos;
};

using location_list = std::vector<location>;

} // namespace hlasm_plugin::parser_library

#endif // !HLASMPLUGIN_PARSERLIBRARY_LOCATION_H
