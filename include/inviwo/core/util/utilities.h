/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2012-2017 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_UTILITIES_H
#define IVW_UTILITIES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/exception.h>
#include <inviwo/core/util/stdextensions.h>
#include <string>

namespace inviwo {

class ProcessorNetwork;

class Property;
class ProcessorWidget;

namespace util {

IVW_CORE_API void saveNetwork(ProcessorNetwork* network, std::string filename);


IVW_CORE_API void saveAllCanvases(ProcessorNetwork* network, std::string dir,
                                  std::string name = "UPN", std::string ext = ".png");

IVW_CORE_API bool isValidIdentifierCharacter(char c, const std::string& extra = "");

IVW_CORE_API void validateIdentifier(const std::string& identifier, const std::string& type,
                                     ExceptionContext context, const std::string& extra = "");

/** 
 * \brief Removes inviwo-module from module library file name.
 * Turns "/path/to/inviwo-module-yourmodule.dll" into "yourmodule".
 * Returns filename without extension if inviwo-module was not found.
 *
 * @param std::string filePath Path to module file
 * @return IVW_CORE_API std::string Module name
 */
IVW_CORE_API std::string stripModuleFileNameDecoration(std::string filePath);

IVW_CORE_API std::string stripIdentifier(std::string identifier);

namespace detail {

struct IVW_CORE_API Shower {
    void operator()(Property& p);
    void operator()(ProcessorWidget& p);
};

struct IVW_CORE_API Hideer {
    void operator()(Property& p);
    void operator()(ProcessorWidget& p);
};


} // detail namespace

template <typename... Args>
void show(Args&&... args) {
    util::for_each_argument(detail::Shower{}, std::forward<Args>(args)...);
}

template <typename... Args>
void hide(Args&&... args) {
    util::for_each_argument(detail::Hideer{}, std::forward<Args>(args)...);
}

} // util namespace

} // inviwo namespace

#endif  // IVW_UTILITIES_H
