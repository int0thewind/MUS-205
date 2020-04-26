#include <climits>
//==============================================================================
/// @file ValueTreeWrapper.h
/// Class wrapper enables getter and setter methods to be defined on ValueTrees
/// rather than using the more clunky JUCE getProperty() & setProperty() APIs.

#pragma once

#include <utility>

#include "../JuceLibraryCode/JuceHeader.h"

/** A base class for wrapping a value tree. Subclass this to
    define a value tree that provides getters and setters rather
    than calling getProperty() and setProperty() to access data.
    For each property in your tree provide juce Identifiers for
    property names and one for your subclass's type id. Example:
    @code
    struct MyData : public ValueTreeWapper {
      static const juce::Identifier MY_TREE_TYPEID ("MyData");
      static const juce::Identifier GLYPH_FONT_SIZE ("glyphSize");
      MyData()
        : ValueTreeWrapper(juce::ValueTree(MY_TREE_TYPEID)) {
      }
      int getFontSize() const {
        return data.getProperty(GLYPH_FONT_SIZE_PROPID, 12);
      }
    };
    @endcode
 */

struct ValueTreeWrapper {
    mutable juce::ValueTree valueTreeData;

    __unused explicit ValueTreeWrapper(const juce::Identifier& id) : valueTreeData(id) {}
    explicit ValueTreeWrapper(juce::ValueTree data) : valueTreeData(std::move(data)) {}
    virtual ~ValueTreeWrapper() = default;

    /**
     * Value tree listener is directly connected with the value tree itself.
     * Our class is a wrapper class. We add support to directly add listener to our value tree data
     * @param listener the listener class that we want to connect directly to the value tree.
     */
    void addListener(juce::ValueTree::Listener* listener) {
        valueTreeData.addListener(listener);
    }

    /**
     * Value tree listener is directly connected with the value tree itself.
     * Our class is a wrapper class. We add support to directly remove listener to our value tree data
     * @param listener the listener class that we want to remove directly from the value tree.
     */
    void removeListener(juce::ValueTree::Listener* listener) {
        valueTreeData.removeListener(listener);
    }

    /**
     * Our class wraps a value tree.
     * This conversion operator allows this wrapper class can also be treated as a value tree
     * @return The value tree data in the class
     */
    explicit operator juce::ValueTree() const {
        return valueTreeData;
    }

    __unused /**
     * Check whether the value tree is valid or not
     * @return true if the value tree is valid
     */
    virtual bool isValid() const {
        return valueTreeData.isValid();
    }

    __unused /**
     * Clear all the properties in the value tree
     */
    void clear () {
        valueTreeData.removeAllProperties(nullptr);
        valueTreeData.removeAllChildren(nullptr);
    }
};
