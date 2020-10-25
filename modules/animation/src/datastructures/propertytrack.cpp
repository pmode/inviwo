/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016-2020 Inviwo Foundation
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

#include <modules/animation/datastructures/propertytrack.h>
#include <inviwo/core/util/logcentral.h>

namespace inviwo {
namespace animation {
namespace detail {
/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
void setPropertyFromKeyframeHelper(CameraProperty* property, const CameraKeyframe* keyframe) {
    property->setLook(keyframe->getLookFrom(), keyframe->getLookTo(), keyframe->getLookUp());
}
/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
void setKeyframeFromPropertyHelper(const CameraProperty* property, CameraKeyframe* keyframe) {
    keyframe->updateFrom(property->get());
}

void setPropertyFromKeyframeHelper(ButtonProperty* property, const ButtonKeyframe*) {
    property->pressButton();
}

void setKeyframeFromPropertyHelper(const ButtonProperty* property, ButtonKeyframe* key) {
    if (&key->getValue() != property) {
        LogWarnCustom(
            "animation::setKeyframeFromPropertyHelper",
            "The keyframe cannot be set to another Button. Please add a new animation track for" +
                property->getDisplayName());
    }
}

}  // namespace detail

}  // namespace animation
}  // namespace inviwo
