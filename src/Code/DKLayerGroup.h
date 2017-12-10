///**********************************************************************************************************************************
///  DKLayerGroup.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 23/08/2007.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKLayer.h"

NS_ASSUME_NONNULL_BEGIN

/*!
 
 A layer group is a layer which maintains a list of other layers. This permits layers to be organised hierarchically if
 the application wishes to do so.
 
 \c DKDrawing is a subclass of this, so it inherits the ability to maintain a list of layers. However it doesn't honour
 every possible feature of a layer group, particularly those the group inherits from <code>DKLayer</code>. This is because
 \c DKLayerGroup is actually a refactoring of DKDrawing and backward compatibility with existing files is required. In particular one
 should take care not to add a DKDrawing instance to a layer group belonging to another drawing (or create circular references).
 
 The stacking order of layers is arranged so that the top layer always has the index zero, and the bottom is at (count - 1).
 In general your code should minimise its exposure to the actual layer index, but the reason that layers are stacked this
 way is so that a layer UI such as a \c NSTableView doesn't have to do anything special to view layers in a natural way, with
 the top layer at the top of such a table. Prior to beta 3, layers were stacked the other way so such tables appeared to
 be upside-down. This class automatically reverses the stacking order in an archive if it detects an older version.
 
 */
@interface DKLayerGroup : DKLayer <NSCoding>
{
@private
	NSMutableArray*			m_layers;
}

/// @brief convenience method for building a new layer group from an existing list of layers
///
/// @param layers a list of existing layers
/// @return A new layer group containing the passed layers.
///
/// @discussion The group must be added to a drawing to be useful. If the layers are already part of a drawing,
/// or other group, they need to be removed first. It is an error to attach a layer in more than one
/// group (or drawing, which is a group) at a time.
/// Layers should be stacked with the top at index #0, the bottom at #(count - 1).
+ (nullable instancetype)	layerGroupWithLayers:(nullable NSArray<DKLayer*>*) layers;

/// @brief Initialize a layer group.
///
/// @param layers A list of existing layers.
/// @return A new layer group.
///
/// @discussion A layer group must be added to another group or drawing before it can be used.
- (nullable instancetype)	initWithLayers:(nullable NSArray<DKLayer*>*) layers;

// layer list

/// @brief The drawing's layers.
/// @discussion Layers are usually added one at a time through some user interface, but this setter allows them to
/// be set all at once, as when unarchiving. Not recorded for undo.
@property (nonatomic, copy) NSArray<DKLayer*>* layers;												// KVC/KVO compliant
//! The number of layers.
@property (readonly) NSInteger countOfLayers;														// KVC/KVO compliant

/// @brief returns the index of the topmost layer that returns \c YES for <code>isOpaque</code>.
/// @discussion Used for optimising drawing - layers below the highest opaque layer are not drawn (because they can't
/// be seen "through" the opaque layer). A layer decides itself if it's opaque by returning \c YES or \c NO for
/// <code>isOpaque</code>. If no layers are opaque, returns the index of the bottom layer.
@property (readonly) NSInteger indexOfHighestOpaqueLayer;

/// @brief returns all of the layers in this group and all groups below it
///
/// @return a list of layers
///
/// @discussion The returned list does not contain any layer groups.
- (NSArray<__kindof DKLayer*>*)		flattenedLayers;

/// @brief Returns all of the layers in this group and all groups below it.
///
/// @param includeGroups If <code>YES</code>, list includes the groups, \c NO only returns actual layers.
/// @return A list of layers.
- (NSArray<__kindof DKLayer*>*)		flattenedLayersIncludingGroups:(BOOL) includeGroups;

/// @brief Returns all of the layers in this group and all groups below it having the given class.
///
/// @param layerClass A \c Class indicating the kind of layer of interest.
/// @return A list of matching layers.
///
/// @discussion Does not include groups unless the class is <code>DKLayerGroup</code>.
- (NSArray<__kindof DKLayer*>*)		flattenedLayersOfClass:(Class) layerClass;

/// @brief Returns all of the layers in this group and all groups below it having the given class.
///
/// @param layerClass A \c Class indicating the kind of layer of interest
/// @param includeGroups If YES, includes groups as well as the requested class
/// @return A list of matching layers.
- (NSArray<__kindof DKLayer*>*)		flattenedLayersOfClass:(Class) layerClass includeGroups:(BOOL) includeGroups;


/// @brief returns the hierarchical level of this group, i.e. how deeply nested it is.
///
/// @discussion the root group returns 0, next level is 1 and so on.
@property (readonly) NSInteger level;

// adding and removing layers

/// @brief creates and adds a layer to the drawing
///
/// @param layerClass the class of some kind of layer
/// @return the layer created.
///
/// @discussion layerClass must be a valid subclass of <code>DKLayer</code>, otherwise does nothing and \c nil is returned.
- (nullable __kindof DKLayer*)addNewLayerOfClass:(Class) layerClass;

/// @brief Adds a layer to the group.
///
/// @param aLayer A \c DKLayer object, or subclass thereof.
///
/// @discussion The added layer is placed above all other layers.
- (void)					addLayer:(DKLayer*) aLayer;

/// @brief adds a layer above a specific index position in the stack
///
/// @param aLayer A \c DKLayer object, or subclass thereof.
/// @param layerIndex the index number of the layer the new layer should be placed in front of.
///
/// @discussion Layer indexes run from 0 being the top layer to (count - 1), being the bottom layer.
- (void)					addLayer:(DKLayer*) aLayer aboveLayerIndex:(NSInteger) layerIndex;

/// @brief Adds a layer at a specific index position in the stack.
///
/// @param aLayer A \c DKLayer object, or subclass thereof.
/// @param layerIndex the index number of the layer inserted.
///
/// @discussion All other addLayer methods call this, which permits the operation to be undone including restoring
/// the layer's index. KVC/KVO compliant.<br>
/// Layer indexes run from 0 being the top layer to (count - 1), being the bottom layer.
- (void)					insertObject:(DKLayer*) aLayer inLayersAtIndex:(NSInteger) layerIndex;	// KVC/KVO compliant

/// @brief Remove the layer with a particular index number from the layer.
///
/// @param layerIndex The index number of the layer to remove.
///
/// @discussion All other removeLayer methods call this, which permits the operation to be undone including restoring
/// the layer's index. KVC/KVO compliant.<br>
/// Layer indexes run from 0 being the top layer to (count -1), being the bottom layer.
- (void)					removeObjectFromLayersAtIndex:(NSInteger) layerIndex;					// KVC/KVO compliant

/// @brief Removes the layer from the drawing.
///
/// @param aLayer A \c DKLayer object, or subclass thereof, that already exists in the group.
///
/// @discussion Disposes of the layer if there are no other references to it.
- (void)					removeLayer:(DKLayer*) aLayer;

/// @brief removes all of the group's layers
///
/// @discussion This method is not undoable. To undoably remove a layer, remove them one at a time. KVO observers
/// will not be notified by this method.
- (void)					removeAllLayers;


/// @brief Disambiguates a layer's name by appending digits until there is no conflict.
///
/// @param aName A string containing the proposed name.
/// @return A string, either the original string or a modified version of it.
///
/// @discussion It is not important that layer's have unique names, but a UI will usually want to do this, thus
/// when using the \c addLayer:andActivateIt: method, the name of the added layer is disambiguated.
- (NSString*)				uniqueLayerNameForName:(NSString*) aName;

// getting layers

/// @brief Returns the layer object at the given index.
///
/// @param layerIndex The index number of the layer of interest.
/// @return A \c DKLayer object or subclass.
///
/// @discussion Layer indexes run from 0 being the top layer to (count - 1), being the bottom layer. KVC/KVO compliant.
- (__kindof DKLayer*)		objectInLayersAtIndex:(NSInteger) layerIndex;							// KVC/KVO compliant

/// @brief returns the topmost layer
///
/// @discussion the topmost \c DKLayer object or subclass, or \c nil if there are no layers.
///
/// Ignores opacity of layers in the stack - this is the one on the top, regardless.
@property (readonly, retain, nullable) __kindof DKLayer *topLayer;

/// @brief The bottom layer.
///
/// @discussion the bottom \c DKLayer object or subclass, or \c nil if there are no layers.
///
/// Ignores opacity of layers in the stack - this is the one on the bottom, regardless.
@property (readonly, retain, nullable) __kindof DKLayer *bottomLayer;

/// @brief Returns the stack position of a given layer.
///
/// @param aLayer A \c DKLayer object, or subclass thereof, that already exists in the drawing.
/// @return The stack index position of the layer.
///
/// @discussion layer indexes run from 0 being the top layer to (count -1), being the bottom layer. If the group does
/// not contain the layer, returns <code>NSNotFound</code>. See also <code>-containsLayer:</code>.
- (NSInteger)				indexOfLayer:(DKLayer*) aLayer;

/// @brief returns the uppermost layer matching class, if any
///
/// @param cl The class of layer to seek.
/// @return The uppermost layer of the given class, or <code>nil</code>.
///
/// @discussion does not perform a deep search
- (nullable __kindof DKLayer*)firstLayerOfClass:(Class) cl;

/// @brief returns the uppermost layer matching class, if any
///
/// @param cl The class of layer to seek.
/// @param deep If <class>YES</class>, will search all subgroups below this one. If <code>NO</code>, only this level is searched
/// @return The uppermost layer of the given class, or <code>nil</code>.
- (nullable __kindof DKLayer*)firstLayerOfClass:(Class) cl performDeepSearch:(BOOL) deep;

/// @brief returns a list of layers of the given class
///
/// @param cl The class of layer to seek.
/// @return A list of layers. May be empty.
///
/// @discussion does not perform a deep search
- (NSArray<__kindof DKLayer*>*)layersOfClass:(Class) cl;

/// @brief returns a list of layers of the given class
///
/// @param cl The class of layer to seek.
/// @param deep If <class>YES</class>, will search all subgroups below this one. If <code>NO</code>, only this level is searched
/// @return A list of layers. May be empty.
- (NSArray<__kindof DKLayer*>*)layersOfClass:(Class) cl performDeepSearch:(BOOL) deep;

/// @brief returns an enumerator that can be used to iterate over the layers in top to bottom order
///
/// @return An \c NSEnumerator object.
///
/// @discussion this is provided as a convenience so you don't have to worry about the implementation detail of
/// which way round layers are ordered to give the top to bottom visual stacking.
- (NSEnumerator*)			layerTopToBottomEnumerator;

/// @brief Returns an enumerator that can be used to iterate over the layers in bottom to top order.
///
/// @return An \c NSEnumerator object.
///
/// @discussion This is provided as a convenience so you don't have to worry about the implementation detail of
/// which way round layers are ordered to give the top to bottom visual stacking.
- (NSEnumerator*)			layerBottomToTopEnumerator;


/// @brief find the topmost layer in this group that is 'hit' by the given point
///
/// @param aPoint A point in drawing coordinates.
/// @return A layer, or <code>nil</code>.
///
/// @discussion A layer must implement \c hitLayer: sensibly for this to operate. This recurses down
/// through any groups contained within. See also <code>-hitLayer:</code>.
- (nullable __kindof DKLayer*)findLayerForPoint:(NSPoint) aPoint;

/// @brief returns whether this group, or any subgroup within, contains the layer
///
/// @param  aLayer A layer of interest.
/// @return \c YES if the group contains the layer.
///
/// @discussion Unlike <code>-indexOfLayer:</code>, considers nested subgroups.  If the layer is the group, returns \c NO
/// (doesn't contain itself).
- (BOOL)					containsLayer:(DKLayer*) aLayer;

/// @brief returns a layer or layer group having the given unique key
///
/// @param key The layer's key.
/// @return The layer if found, \c nil otherwise.
///
/// @discussion unique keys are assigned to layers for the lifetime of the app. They are not persistent and must only
/// be used to find layers in the case where a layer pointer/address would be unreliable.
- (nullable __kindof DKLayer*)layerWithUniqueKey:(NSString*) key;

// showing and hiding

/// @brief makes all layers in the group and in any subgroups visible
///
/// @discussion recurses when nested groups are found
- (void)					showAll;

/// @brief Makes all layers in the group and in any subgroups hidden except <aLayer>, which is made visible.
///
/// @param aLayer A layer to leave visible.
///
/// @discussion \c aLayer may be \c nil in which case this performs a <code>hideAll</code>. Recurses on any subgroups.
- (void)					hideAllExcept:(DKLayer*) aLayer;

/// @brief Is \c YES if there are hidden layers below this, or this is hidden itself.
/// @discussion Recurses on any subgroups.
@property (readonly) BOOL hasHiddenLayers;

/// @brief Returns \c YES if the receiver or any of its contained layers is visible, ignoring the one passed.
///
/// @param aLayer A layer to exclude when testing this.
/// @return \c YES if there are visible layers below this, or this is visible itself
///
/// @discussion Recurses on any subgroups. Typically \c aLayer is the active layer - may be <code>nil</code>.
- (BOOL)					hasVisibleLayersOtherThan:(DKLayer*) aLayer;

// layer stacking order

/// @brief Moves the layer one place towards the top of the stack.
///
/// @param aLayer the layer to move up.
///
/// @discussion If already on top, does nothing.
- (void)					moveUpLayer:(DKLayer*) aLayer;

/// @brief Moves the layer one place towards the bottom of the stack.
///
/// @param aLayer The layer to move down.
///
/// @discussion If already at the bottom, does nothing.
- (void)					moveDownLayer:(DKLayer*) aLayer;

/// @brief Moves the layer to the top of the stack.
///
/// @param aLayer The layer to move up.
///
/// @discussion If already on top, does nothing.
- (void)					moveLayerToTop:(DKLayer*) aLayer;

/// @brief Moves the layer to the bottom of the stack.
///
/// @param aLayer The layer to move down.
///
/// @discussion If already at the bottom, does nothing.
- (void)					moveLayerToBottom:(DKLayer*) aLayer;

/// @brief Changes a layer's z-stacking order so it comes before (above) <code>otherLayer</code>.
///
/// @param aLayer The layer to move - may not be <code>nil</code>.
/// @param otherLayer Move above this layer. May be <code>nil</code>, which moves the layer to the bottom.
- (void)					moveLayer:(DKLayer*) aLayer aboveLayer:(DKLayer*) otherLayer;

/// @brief Changes a layer's z-stacking order so it comes after (below) <otherLayer>
///
/// @param aLayer The layer to move - may not be <code>nil</code>.
/// @param otherLayer Move below this layer. May be <code>nil</code>, which moves the layer to the top.
- (void)					moveLayer:(DKLayer*) aLayer belowLayer:(DKLayer*) otherLayer;

/*!
 @brief Moves a layer to the index position given. This is called by all if the other \c moveLayer... methods.
 @param aLayer The layer to move.
 @param i The index position to move it to.
 @discussion If the layer can't be moved, does nothing. The action is recorded for undo if there is an undoManager attached.
 */
- (void)					moveLayer:(DKLayer*) aLayer toIndex:(NSInteger) i;


@end


extern NSNotificationName		kDKLayerGroupDidAddLayer;
extern NSNotificationName		kDKLayerGroupDidRemoveLayer;
extern NSNotificationName		kDKLayerGroupNumberOfLayersDidChange;
extern NSNotificationName		kDKLayerGroupWillReorderLayers;
extern NSNotificationName		kDKLayerGroupDidReorderLayers;

NS_ASSUME_NONNULL_END
