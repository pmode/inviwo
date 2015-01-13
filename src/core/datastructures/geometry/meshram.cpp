/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
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

#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/geometry/meshram.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM() {
}

MeshRAM::MeshRAM(const MeshRAM& rhs)
    : GeometryRAM(rhs) {
    update(true);
}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {}

MeshRAM* MeshRAM::clone() const {
    return new MeshRAM(*this);
}

void MeshRAM::update(bool editable) {
    attributesRAM_.clear();

    Mesh* owner = static_cast<Mesh*>(this->getOwner());
    if (editable) {
        for (std::vector<Buffer*>::const_iterator it = owner->getBuffers().begin();
             it != owner->getBuffers().end(); ++it)
            attributesRAM_.push_back((*it)->getEditableRepresentation<BufferRAM>());
    } else {
        for (std::vector<Buffer*>::const_iterator it = owner->getBuffers().begin();
             it != owner->getBuffers().end(); ++it)
            attributesRAM_.push_back(const_cast<BufferRAM*>((*it)->getRepresentation<BufferRAM>()));
    }
}

} // namespace

