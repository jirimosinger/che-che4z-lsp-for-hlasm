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

#ifndef HLASMPLUGIN_PARSERLIBRARY_DEBUGGING_MACRO_PARAM_VARIABLE_H
#define HLASMPLUGIN_PARSERLIBRARY_DEBUGGING_MACRO_PARAM_VARIABLE_H

#include "context/variables/macro_param.h"
#include "variable.h"

namespace hlasm_plugin::parser_library::debugging {

// Implementation of variable interface that adapts macro parameters
// representation from context to DAP variable.
class macro_param_variable : public variable
{
public:
    macro_param_variable(const context::macro_param_base& param, std::vector<size_t> index);

    const std::string& get_name() const override;
    const std::string& get_value() const override;

    set_type type() const override;

    bool is_scalar() const override;

    std::vector<variable_ptr> values() const override;
    size_t size() const override;

private:
    const context::macro_param_base& macro_param_;
    std::vector<size_t> index_;
    std::string name_;
    std::string value_;
};


} // namespace hlasm_plugin::parser_library::debugging


#endif // !HLASMPLUGIN_PARSERLIBRARY_DEBUGGING_MACRO_PARAM_VARIABLE_H
