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
 
 DKDrawing is a subclass of this, so it inherits the ability to maintain a list of layers. However it doesn't honour
 every possible feature of a layer group, particularly those the group inherits from DKLayer. This is because
 DKLayerGroup is actually a refactoring of DKDrawing and backward compatibility with existing files is required. In particular one
 should take care not to add a DKDrawing instance to a layer group belonging to another drawing (or create circular references).
 
 The stacking order of layers is arranged so that the top layer always has the index zero, and the bottom is at (count -1).
 In general your code should minimise its exposure to the actual layer index, but the reason that layers are stacked this
 way is so that a layer UI such as a NSTableView doesn't have to do anything special to view layers in a natural way, with
 the top layer at the top of such a table. Prior to beta 3, layers were stacked the other way so such tables appeared to
 be upside-down. This class automatically reverses the stacking order in an archive if it detects an older version.
 
 */
@interface DKLayerGroup : DKLayer <NSCoding>
{
@private
	NSMutableArray*			m_layers;
}

+ (nullable instancetype)	layerGroupWithLayers:(nullable NSArray<DKLayer*>*) layers;

- (nullable instancetype)	initWithLayers:(nullable NSArray<DKLayer*>*) layers;

// layer list

@property (copy) NSArray<DKLayer*>*		layers;														// KVC/KVO compliant
@property (readonly) NSInteger countOfLayers;														// KVC/KVO compliant
@property (readonly) NSInteger indexOfHighestOpaqueLayer;

- (NSArray<__kindof DKLayer*>*)		flattenedLayers;
- (NSArray<__kindof DKLayer*>*)		flattenedLayersIncludingGroups:(BOOL) includeGroups;
- (NSArray<__kindof DKLayer*>*)		flattenedLayersOfClass:(Class) layerClass;
- (NSArray<__kindof DKLayer*>*)		flattenedLayersOfClass:(Class) layerClass includeGroups:(BOOL) includeGroups;

@property (readonly) NSInteger level;

// adding and removing layers

- (nullable __kindof DKLayer*)addNewLayerOfClass:(Class) layerClass;
- (void)					addLayer:(DKLayer*) aLayer;
- (void)					addLayer:(DKLayer*) aLayer aboveLayerIndex:(NSInteger) layerIndex;
- (void)					insertObject:(DKLayer*) aLayer inLayersAtIndex:(NSInteger) layerIndex;	// KVC/KVO compliant
- (void)					removeObjectFromLayersAtIndex:(NSInteger) layerIndex;					// KVC/KVO compliant
- (void)					removeLayer:(DKLayer*) aLayer;
- (void)					removeAllLayers;
- (NSString*)				uniqueLayerNameForName:(NSString*) aName;

// getting layers

- (__kindof DKLayer*)		objectInLayersAtIndex:(NSInteger) layerIndex;							// KVC/KVO compliant
@property (readonly, retain, nullable) __kindof DKLayer *topLayer;
@property (readonly, retain) __kindof DKLayer *bottomLayer;
- (NSInteger)				indexOfLayer:(DKLayer*) aLayer;
- (nullable __kindof DKLayer*)firstLayerOfClass:(Class) cl;
- (nullable __kindof DKLayer*)firstLayerOfClass:(Class) cl performDeepSearch:(BOOL) deep;
- (NSArray<__kindof DKLayer*>*)layersOfClass:(Class) cl;
- (NSArray<__kindof DKLayer*>*)layersOfClass:(Class) cl performDeepSearch:(BOOL) deep;
- (NSEnumerator*)			layerTopToBottomEnumerator;
- (NSEnumerator*)			layerBottomToTopEnumerator;

- (nullable __kindof DKLayer*)findLayerForPoint:(NSPoint) aPoint;
- (BOOL)					containsLayer:(DKLayer*) aLayer;

- (nullable __kindof DKLayer*)layerWithUniqueKey:(NSString*) key;

// showing and hiding

- (void)					showAll;
- (void)					hideAllExcept:(DKLayer*) aLayer;
@property (readonly) BOOL hasHiddenLayers;
- (BOOL)					hasVisibleLayersOtherThan:(DKLayer*) aLayer;

// layer stacking order

- (void)					moveUpLayer:(DKLayer*) aLayer;
- (void)					moveDownLayer:(DKLayer*) aLayer;
- (void)					moveLayerToTop:(DKLayer*) aLayer;
- (void)					moveLayerToBottom:(DKLayer*) aLayer;
- (void)					moveLayer:(DKLayer*) aLayer aboveLayer:(DKLayer*) otherLayer;
- (void)					moveLayer:(DKLayer*) aLayer belowLayer:(DKLayer*) otherLayer;
- (void)					moveLayer:(DKLayer*) aLayer toIndex:(NSInteger) i;


@end


extern NSNotificationName		kDKLayerGroupDidAddLayer;
extern NSNotificationName		kDKLayerGroupDidRemoveLayer;
extern NSNotificationName		kDKLayerGroupNumberOfLayersDidChange;
extern NSNotificationName		kDKLayerGroupWillReorderLayers;
extern NSNotificationName		kDKLayerGroupDidReorderLayers;

NS_ASSUME_NONNULL_END
