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

#ifndef CONTEXT_SECTION_H
#define CONTEXT_SECTION_H

#include <memory>
#include <vector>

#include "../id_index.h"

namespace hlasm_plugin::parser_library::context {

class location_counter;

enum class section_kind
{
    DUMMY,
    COMMON,
    EXECUTABLE,
    READONLY,
    EXTERNAL,
    WEAK_EXTERNAL,
};


// class representing section (CSECT/DSECT ...)
class section
{
    // location counter assigned to this section
    std::vector<std::unique_ptr<location_counter>> loctrs_;
    location_counter* curr_loctr_;

public:
    // unique identifier
    const id_index name;
    const section_kind kind;

    // access list of location counters
    const std::vector<std::unique_ptr<location_counter>>& location_counters() const;

    section(id_index name, section_kind kind);

    // sets current location counter
    void set_location_counter(id_index loctr_name);

    // checker method whether the location counter with provided name already exists in the section
    bool counter_defined(id_index name);

    // access current location counter
    location_counter& current_location_counter() const;
};

} // namespace hlasm_plugin::parser_library::context
#endif
