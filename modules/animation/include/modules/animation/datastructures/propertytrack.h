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

#ifndef IVW_PROPERTYTRACK_H
#define IVW_PROPERTYTRACK_H

#include <modules/animation/animationmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/ordinalrefproperty.h>
#include <inviwo/core/properties/optionproperty.h>

#include <modules/animation/datastructures/valuekeyframe.h>
#include <modules/animation/datastructures/basetrack.h>
#include <modules/animation/datastructures/buttonkeyframesequence.h>
#include <modules/animation/datastructures/animationtime.h>
#include <modules/animation/datastructures/camerakeyframe.h>
#include <modules/animation/datastructures/keyframesequence.h>
#include <modules/animation/datastructures/valuekeyframesequence.h>
#include <modules/animation/interpolation/linearinterpolation.h>

namespace inviwo {

namespace animation {

namespace detail {

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
template <typename T>
void setPropertyFromKeyframeHelper(TemplateProperty<T>* property,
                                   const ValueKeyframe<T>* keyframe) {
    property->set(keyframe->getValue());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
template <typename T>
void setPropertyFromKeyframeHelper(OrdinalProperty<T>* property, const ValueKeyframe<T>* keyframe) {
    property->set(keyframe->getValue());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
template <typename T>
void setPropertyFromKeyframeHelper(OrdinalRefProperty<T>* property,
                                   const ValueKeyframe<T>* keyframe) {
    property->set(keyframe->getValue());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
template <typename T>
void setPropertyFromKeyframeHelper(TemplateOptionProperty<T>* property,
                                   const ValueKeyframe<T>* keyframe) {
    property->setSelectedValue(keyframe->getValue());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
template <typename T>
void setKeyframeFromPropertyHelper(const TemplateProperty<T>* property,
                                   ValueKeyframe<T>* keyframe) {
    keyframe->setValue(property->get());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
template <typename T>
void setKeyframeFromPropertyHelper(const OrdinalProperty<T>* property, ValueKeyframe<T>* keyframe) {
    keyframe->setValue(property->get());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
template <typename T>
void setKeyframeFromPropertyHelper(const OrdinalRefProperty<T>* property,
                                   ValueKeyframe<T>* keyframe) {
    keyframe->setValue(property->get());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
template <typename T>
void setKeyframeFromPropertyHelper(const TemplateOptionProperty<T>* property,
                                   ValueKeyframe<T>* keyframe) {
    keyframe->setValue(property->getSelectedValue());
}

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
void setPropertyFromKeyframeHelper(CameraProperty* property, const CameraKeyframe* keyframe);
/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
void setKeyframeFromPropertyHelper(const CameraProperty* property, CameraKeyframe* keyframe);

/**
 * Helper function for inviwo::animation::PropertyTrack::setPropertyFromKeyframe
 * @see inviwo::animation::BasePropertyTrack::setPropertyFromKeyframe
 */
void setPropertyFromKeyframeHelper(ButtonProperty* property, const ButtonKeyframe* keyframe);
/**
 * Helper function for inviwo::animation::PropertyTrack::setKeyframeFromProperty
 * @see inviwo::animation::BasePropertyTrack::setKeyframeFromProperty
 */
void setKeyframeFromPropertyHelper(const ButtonProperty* property, ButtonKeyframe* keyframe);

}  // namespace detail

/** \class BasePropertyTrack
 * Interface for tracks based on a Property.
 * Exposes functions for adding a KeyFrame and KeyFrameSequence
 * using the current values of the Property.
 * @see Track
 * @see PropertyTrack
 * @see Property
 */
class IVW_MODULE_ANIMATION_API BasePropertyTrack {
public:
    virtual ~BasePropertyTrack() = default;
    virtual void setProperty(Property* property) = 0;
    virtual const Property* getProperty() const = 0;
    virtual Property* getProperty() = 0;
    virtual const std::string& getIdentifier() const = 0;
    /*
     * Add KeyFrame at specified time using the current value of supplied property.
     * All keyframes in one sequence uses the same interpolation so the provided interpolation
     * will only be used if a new sequence need to be created.
     *
     * See example below on which KeyFrameSequence the KeyFrame is added:
     * Track of sequences
     *                  Sequence 1                   Sequence 2        Seq. 3
     * Track: ----------X======X====X-----------------X=========X-------X=====X--------
     *        |- case 1-|-case 2----------------------|-case 2----------|-case 3------|
     *                  |-case 2a-----------|-case 2b-|
     *
     * Case 0: No sequence exist. Create sequence at time and use provided interpolation for
     * sequence. Case 1: Time is before first sequence add KeyFrame to Sequence 1. Case 2: Time is
     * inside an existing sequence, add KeyFrame to this sequence. Case 2a: Time is closest to
     * Sequence 1, add it to this sequence. Case 2b: Time is closest to Sequence 2, add it to this
     * sequence. Case 3: Time is in, or after, last sequence, add it to this sequence.
     *
     * @param proprty Property to create key frame value from.
     * @param time at which KeyFrame should be added.
     * @param interpolation to use if a new sequence is created
     * @throw Exception If supplied property is not of same type as BasePropertyTrack::getProperty
     * @throw Exception If Interpolation type is invalid for property value.
     */
    virtual Keyframe* addKeyFrameUsingPropertyValue(
        const Property* property, Seconds time, std::unique_ptr<Interpolation> interpolation) = 0;
    /*
     * Add KeyFrame at specified time using the current value of the property.
     * @see addKeyFrameUsingPropertyValue(const Property* property, Seconds time,
     * std::unique_ptr<Interpolation> interpolation)
     * @param time at which KeyFrame should be added.
     * @param interpolation to use if a new sequence is created
     */
    virtual Keyframe* addKeyFrameUsingPropertyValue(
        Seconds time, std::unique_ptr<Interpolation> interpolation) = 0;
    /*
     * Add KeyFrameSequence at specified time using the current value of the property.
     * @param time at which KeyFrame should be added.
     * @param interpolation to use for the new sequence.
     * @throw Exception If a sequence already exist at time
     */
    virtual KeyframeSequence* addSequenceUsingPropertyValue(
        Seconds time, std::unique_ptr<Interpolation> interpolation) = 0;
    virtual Track* toTrack() = 0;

    virtual void setPropertyFromKeyframe(Property* dst, const Keyframe* src) const = 0;
    virtual void setKeyframeFromProperty(const Property* src, Keyframe* dst) = 0;
};

/** \class PropertyTrack
 * Implementation of BasePropertyTrack and TrackTyped based on templates parameter types for
 * Property, KeyFrame and KeyframeSequence.
 * Exposes functions for adding a KeyFrame and KeyFrameSequence
 * using the current values of the Property.
 * @see Track
 * @see KeyframeSequenceTyped
 * @see Property
 */
template <typename Prop, typename Key, typename Seq = KeyframeSequenceTyped<Key>>
class PropertyTrack : public BaseTrack<Seq>, public BasePropertyTrack {
public:
    static_assert(std::is_same<Key, typename Seq::key_type>::value,
                  "The KeyframeSequence must match Keyframe 'Key'");
    PropertyTrack();
    PropertyTrack(Prop* property);
    /**
     * Remove all keyframe sequences and call TrackObserver::notifyKeyframeSequenceRemoved
     */
    virtual ~PropertyTrack();

    static std::string classIdentifier();
    virtual std::string getClassIdentifier() const override;

    virtual AnimationTimeState operator()(Seconds from, Seconds to,
                                          AnimationState state) const override;

    virtual const Prop* getProperty() const override;
    virtual Prop* getProperty() override;
    virtual void setProperty(Property* property) override;
    virtual const std::string& getIdentifier() const override;

    virtual void serialize(Serializer& s) const override;
    virtual void deserialize(Deserializer& d) override;

    virtual Keyframe* addKeyFrameUsingPropertyValue(
        const Property* property, Seconds time,
        std::unique_ptr<Interpolation> interpolation) override;
    virtual Keyframe* addKeyFrameUsingPropertyValue(
        Seconds time, std::unique_ptr<Interpolation> interpolation) override;
    virtual KeyframeSequence* addSequenceUsingPropertyValue(
        Seconds time, std::unique_ptr<Interpolation> interpolation) override;

    // BasePropertyTrack overload
    virtual Track* toTrack() override;

    /**
     * \brief Helper function to set a property from a keyframe
     *
     * Called from inviwo::animation::KeyframeEditorWidget when creating the widget
     *
     * @param dstProperty The property to set
     * @param keyframe The keyframe to set from
     */
    void setPropertyFromKeyframe(Property* dstProperty, const Keyframe* keyframe) const override {
        IVW_ASSERT(dstProperty->getClassIdentifier() == PropertyTraits<Prop>::classIdentifier(),
                   "Incorrect Property type");
        detail::setPropertyFromKeyframeHelper(static_cast<Prop*>(dstProperty),
                                              static_cast<const Key*>(keyframe));
    }

    /**
     * \brief Helper function to update the value if a keyframe from a property (other than the
     * property owned by the track)
     *
     * Called from inviwo::animation::KeyframeEditorWidget when the value of the keyframe is updated
     * through the widget
     *
     * @param srcProperty The property to set from
     * @param keyframe The keyframe to set
     */
    void setKeyframeFromProperty(const Property* srcProperty, Keyframe* keyframe) override {
        ivwAssert(srcProperty->getClassIdentifier() == PropertyTraits<Prop>::classIdentifier(),
                  "Incorrect Property type");
        detail::setKeyframeFromPropertyHelper(static_cast<const Prop*>(srcProperty),
                                              static_cast<Key*>(keyframe));
    }

protected:
    /*
     * Helper function for when we know that we are between keyframes within a KeyframeSequence.
     * Called from operator()(Seconds from, Seconds to, AnimationState state) const
     * Provide template specialization of this method if you want custom property/sequence
     * behaviour.
     */
    AnimationTimeState animateSequence(const Seq& seq, Seconds from, Seconds to,
                                       AnimationState state) const;
    /*
     * Create a Key using the current property value.
     */
    virtual std::unique_ptr<Key> createKeyframe(Seconds time) override;

    /*
     * Create a Key using the provided property value.
     */
    std::unique_ptr<Key> createKeyframe(const Prop* property, Seconds time);
    /*
     * Create a KeyframeSequence using the provided keys and interpolation.
     */
    std::unique_ptr<Seq> createKeyframeSequence(std::vector<std::unique_ptr<Key>> keys,
                                                std::unique_ptr<Interpolation> interpolation);

private:
    Prop* property_;  ///< non-owning reference
};

template <typename Prop, typename Key, typename Seq>
bool operator==(const PropertyTrack<Prop, Key, Seq>& a, const PropertyTrack<Prop, Key, Seq>& b) {
    return std::equal(a.begin(), a.end(), a.begin(), b.end());
}
template <typename Prop, typename Key, typename Seq>
bool operator!=(const PropertyTrack<Prop, Key, Seq>& a, const PropertyTrack<Prop, Key, Seq>& b) {
    return !(a == b);
}

template <typename Prop, typename Key, typename Seq>
Track* PropertyTrack<Prop, Key, Seq>::toTrack() {
    return this;
}

template <typename Prop, typename Key, typename Seq>
AnimationTimeState PropertyTrack<Prop, Key, Seq>::animateSequence(const Seq& seq, Seconds from,
                                                                  Seconds to,
                                                                  AnimationState state) const {
    typename Prop::value_type v;
    seq(from, to, v);
    property_->set(v);
    return {to, state};
}

template <typename Prop, typename Key, typename Seq>
inline std::unique_ptr<Key> PropertyTrack<Prop, Key, Seq>::createKeyframe(Seconds time) {
    return createKeyframe(property_, time);
}

template <typename Prop, typename Key, typename Seq>
inline std::unique_ptr<Key> PropertyTrack<Prop, Key, Seq>::createKeyframe(const Prop* property,
                                                                          Seconds time) {
    return std::make_unique<Key>(time, property->get());
}

template <typename Prop, typename Key, typename Seq>

inline std::unique_ptr<Seq> PropertyTrack<Prop, Key, Seq>::createKeyframeSequence(
    std::vector<std::unique_ptr<Key>> keys, std::unique_ptr<Interpolation> interpolation) {
    if constexpr (std::is_same<Seq, KeyframeSequenceTyped<Key>>::value) {
        if (auto ip = dynamic_cast<InterpolationTyped<Key>*>(interpolation.get())) {
            interpolation.release();
            return std::make_unique<Seq>(std::move(keys),
                                         std::unique_ptr<InterpolationTyped<Key>>(ip));
        } else {
            throw Exception("Invalid interpolation " + interpolation->getClassIdentifier() +
                                " for " + getClassIdentifier(),
                            IVW_CONTEXT);
        }
    } else {
        return std::make_unique<Seq>(std::move(keys));
    }
}

template <typename Prop, typename Key, typename Seq>
PropertyTrack<Prop, Key, Seq>::PropertyTrack() : BaseTrack<Seq>{"", "", 100}, property_(nullptr) {}

template <typename Prop, typename Key, typename Seq>
PropertyTrack<Prop, Key, Seq>::PropertyTrack(Prop* property)
    : BaseTrack<Seq>{property->getIdentifier(), property->getDisplayName(), 100}
    , property_(property) {}

template <typename Prop, typename Key, typename Seq>
PropertyTrack<Prop, Key, Seq>::~PropertyTrack() = default;

template <typename Prop, typename Key, typename Seq>
std::string PropertyTrack<Prop, Key, Seq>::classIdentifier() {
    // Use property class identifier since multiple properties
    // may have the same key (data type)
    std::string id =
        "org.inviwo.animation.PropertyTrack.for. " + PropertyTraits<Prop>::classIdentifier();
    return id;
}

template <typename Prop, typename Key, typename Seq>
const std::string& PropertyTrack<Prop, Key, Seq>::getIdentifier() const {
    return BaseTrack<Seq>::getIdentifier();
}

template <typename Prop, typename Key, typename Seq>
std::string PropertyTrack<Prop, Key, Seq>::getClassIdentifier() const {
    return classIdentifier();
}

template <typename Prop, typename Key, typename Seq>
Prop* PropertyTrack<Prop, Key, Seq>::getProperty() {
    return property_;
}

template <typename Prop, typename Key, typename Seq>
const Prop* PropertyTrack<Prop, Key, Seq>::getProperty() const {
    return property_;
}

template <typename Prop, typename Key, typename Seq>
void PropertyTrack<Prop, Key, Seq>::setProperty(Property* property) {
    if (auto prop = dynamic_cast<Prop*>(property)) {
        property_ = prop;
        this->setIdentifier(property_->getIdentifier());
        this->setName(property_->getDisplayName());
    } else {
        throw Exception("Invalid property set to track", IVW_CONTEXT);
    }
}

/**
 * Track of sequences
 * ----------X======X====X-----------X=========X-------X=====X--------
 * |- case 1-|-case 2----------------|-case 2----------|-case 2------|
 *           |-case 2a---|-case 2b---|
 */
template <typename Prop, typename Key, typename Seq>
AnimationTimeState PropertyTrack<Prop, Key, Seq>::operator()(Seconds from, Seconds to,
                                                             AnimationState state) const {
    if (!this->isEnabled() || this->empty()) return {to, state};

    // 'it' will be the first seq. with a first time larger then 'to'.
    auto it = std::upper_bound(this->begin(), this->end(), to,
                               [](const auto& a, const auto& b) { return a < b; });

    if (it == this->begin()) {
        if (from > it->getFirstTime()) {  // case 1
            setPropertyFromKeyframe(property_, &(it->getFirst()));
        }
    } else {  // case 2
        auto& seq1 = *std::prev(it);

        if (to < seq1.getLastTime()) {  // case 2a
            return animateSequence(seq1, from, to, state);
        } else {  // case 2b
            if (from < seq1.getLastTime()) {
                // We came from before the previous key
                setPropertyFromKeyframe(property_, &(seq1.getLast()));
            } else if (it != this->end() && from > it->getFirstTime()) {
                // We came form after the next key
                setPropertyFromKeyframe(property_, &(it->getFirst()));
            }
            // we moved in an unmarked region, do nothing.
        }
    }
    return {to, state};
}

template <typename Prop, typename Key, typename Seq>
Keyframe* PropertyTrack<Prop, Key, Seq>::addKeyFrameUsingPropertyValue(
    const Property* property, Seconds time, std::unique_ptr<Interpolation> interpolation) {
    auto prop = dynamic_cast<const Prop*>(property);
    if (!prop) {
        throw Exception("Cannot add key frame from property type " +
                            property->getClassIdentifier() + " for " +
                            property_->getClassIdentifier(),
                        IVW_CONTEXT);
    }
    if (this->empty()) {
        // Use provided interpolation if we can
        std::vector<std::unique_ptr<Key>> keys;
        keys.push_back(createKeyframe(prop, time));
        auto sequence = createKeyframeSequence(std::move(keys), std::move(interpolation));
        if (auto se = this->add(std::move(sequence))) {
            return &se->getFirst();
        }
    } else {
        return this->addToClosestSequence(createKeyframe(prop, time));
    }
    return nullptr;
}

template <typename Prop, typename Key, typename Seq>
Keyframe* PropertyTrack<Prop, Key, Seq>::addKeyFrameUsingPropertyValue(
    Seconds time, std::unique_ptr<Interpolation> interpolation) {
    return addKeyFrameUsingPropertyValue(property_, time, std::move(interpolation));
}

template <typename Prop, typename Key, typename Seq>
KeyframeSequence* PropertyTrack<Prop, Key, Seq>::addSequenceUsingPropertyValue(
    Seconds time, std::unique_ptr<Interpolation> interpolation) {
    std::vector<std::unique_ptr<Key>> keys;
    keys.push_back(createKeyframe(property_, time));
    auto sequence = createKeyframeSequence(std::move(keys), std::move(interpolation));
    return this->add(std::move(sequence));
}

template <typename Prop, typename Key, typename Seq>
void PropertyTrack<Prop, Key, Seq>::serialize(Serializer& s) const {
    BaseTrack<Seq>::serialize(s);
    s.serialize("property", property_);
}

template <typename Prop, typename Key, typename Seq>
void PropertyTrack<Prop, Key, Seq>::deserialize(Deserializer& d) {
    BaseTrack<Seq>::deserialize(d);
    d.deserializeAs<Property>("property", property_);
}

// Template specialization (CameraProperty)
template <>
inline std::string PropertyTrack<CameraProperty, CameraKeyframe>::classIdentifier() {
    // Use property class identifier since multiple properties
    // may have the same key (data type)
    std::string id = "org.inviwo.animation.PropertyTrack.for." + CameraProperty::classIdentifier;
    return id;
}

template <>
inline AnimationTimeState PropertyTrack<CameraProperty, CameraKeyframe>::animateSequence(
    const KeyframeSequenceTyped<CameraKeyframe>& seq, Seconds from, Seconds to,
    AnimationState state) const {
    seq(from, to, property_->get());
    return {to, state};
}

template <>
inline AnimationTimeState
PropertyTrack<ButtonProperty, ButtonKeyframe, ButtonKeyframeSequence>::animateSequence(
    const ButtonKeyframeSequence& seq, Seconds from, Seconds to, AnimationState state) const {
    return seq(from, to, state);
}

template <>
inline std::unique_ptr<ButtonKeyframe>
PropertyTrack<ButtonProperty, ButtonKeyframe, ButtonKeyframeSequence>::createKeyframe(
    const ButtonProperty* property, Seconds time) {
    return std::make_unique<ButtonKeyframe>(time, const_cast<ButtonProperty*>(property));
}
}  // namespace animation

}  // namespace inviwo

#endif  // IVW_PROPERTYTRACK_H
