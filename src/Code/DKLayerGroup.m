///**********************************************************************************************************************************
///  DKLayerGroup.m
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 23/08/2007.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKLayerGroup.h"
#import "DKDrawing.h"
#import "DKDrawKitMacros.h"
#import "LogEvent.h"


#pragma mark Constants (Non-localized)
NSString*		kDKLayerGroupDidAddLayer				= @"kDKLayerGroupDidAddLayer";
NSString*		kDKLayerGroupDidRemoveLayer				= @"kDKLayerGroupDidRemoveLayer";
NSString*		kDKLayerGroupNumberOfLayersDidChange	= @"kDKLayerGroupNumberOfLayersDidChange";
NSString*		kDKLayerGroupWillReorderLayers			= @"kDKLayerGroupWillReorderLayers";
NSString*		kDKLayerGroupDidReorderLayers			= @"kDKLayerGroupDidReorderLayers";


#pragma mark -
@implementation DKLayerGroup
#pragma mark As a DKLayerGroup
+ (DKLayerGroup*)			layerGroupWithLayers:(NSArray*) layers
{
	DKLayerGroup* lg = [[self alloc] initWithLayers:layers];
	
	return [lg autorelease];
}


#pragma mark -
- (id)						initWithLayers:(NSArray*) layers
{
	self = [super init];
	if (self != nil)
	{
		m_layers = [[NSMutableArray arrayWithCapacity:4] retain];
		
		if (m_layers == nil)
		{
			[self autorelease];
			return nil;
		}
	}
	if (self != nil)
	{
		if (layers != nil)
		{
			[self setLayers:layers];
		}
		[self setSelectionColour:nil];
	}
	return self;
}


#pragma mark -
#pragma mark - layer list
- (void)				setLayers:(NSArray*) layers
{
	NSAssert( layers != nil, @"attempt to set layer groups layers to nil");
	
	if( layers != [self layers])
	{
		LogEvent_(kReactiveEvent, @"setting layer group %@, layers = %@", self, layers);

		[m_layers makeObjectsPerformSelector:@selector(setLayerGroup:) withObject:nil];
		[m_layers release];
		m_layers = [layers mutableCopy];
		
		// this is to ensure the group member is inited - older files didn't save the group ref so it will be nil
		// newer files do, but doing this anyway has no harmful effect
		
		[m_layers makeObjectsPerformSelector:@selector(setLayerGroup:) withObject:self];
		[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupNumberOfLayersDidChange object:self];
	}
}


@synthesize layers=m_layers;


- (NSInteger)			countOfLayers
{
	return[m_layers count];
}


- (NSInteger)			indexOfHighestOpaqueLayer
{
	// returns the index of the topmost layer that returns YES for isOpaque.

	NSInteger i = 0;
	
	do
	{
		if ([[self objectInLayersAtIndex:i] isOpaque])
			return i;
	}
	while( ++i < [self countOfLayers]);
	
	return [self countOfLayers] - 1;	// the bottom layer is the last
}


- (NSArray*)			flattenedLayers
{
	return [self flattenedLayersIncludingGroups:NO];
}


- (NSArray*)			flattenedLayersIncludingGroups:(BOOL) includeGroups
{
	NSEnumerator*	iter = [[self layers] objectEnumerator];
	DKLayer*		layer;
	NSMutableArray*	fLayers = [NSMutableArray array];
	
	if( includeGroups )
		[fLayers addObject:self];
	
	while(( layer = [iter nextObject]))
	{
		if([layer respondsToSelector:_cmd])
			[fLayers addObjectsFromArray:[(DKLayerGroup*)layer flattenedLayersIncludingGroups:includeGroups]];
		else
			[fLayers addObject:layer];
	}
	
	return fLayers;
}


- (NSArray*)			flattenedLayersOfClass:(Class) layerClass
{
	return [self flattenedLayersOfClass:layerClass includeGroups:NO];
}


- (NSArray*)			flattenedLayersOfClass:(Class) layerClass includeGroups:(BOOL) includeGroups
{
	NSEnumerator*	iter = [[self layers] objectEnumerator];
	DKLayer*		layer;
	NSMutableArray*	fLayers = [NSMutableArray array];
	
	if( includeGroups || [self isKindOfClass:layerClass])
		[fLayers addObject:self];
	
	while(( layer = [iter nextObject]))
	{
		if([layer respondsToSelector:_cmd])
			[fLayers addObjectsFromArray:[(DKLayerGroup*)layer flattenedLayersOfClass:layerClass includeGroups:includeGroups]];
		else if([layer isKindOfClass:layerClass])
			[fLayers addObject:layer];
	}
	
	return fLayers;
}


#pragma mark -
#pragma mark - adding and removing layers
- (DKLayer*)		addNewLayerOfClass:(Class) layerClass
{
	if ([layerClass isSubclassOfClass:[DKLayer class]])
	{
		DKLayer* layer = [[layerClass alloc] init];
	
		[self addLayer:layer];
		[layer release]; // retained by self
	
		return layer;
	}
	else
		return nil;
}


- (void)				addLayer:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"can't add a nil layer");
	
	[self insertObject:aLayer inLayersAtIndex:0];
}


- (void)				addLayer:(DKLayer*) aLayer aboveLayerIndex:(NSInteger) layerIndex
{
	NSAssert( aLayer != nil, @"cannot add a nil layer");

	// adds a layer above the given index - if index is 0 or 1 puts the layer on top
	
	if ( layerIndex <= 1 )
		[self insertObject:aLayer inLayersAtIndex:0];
	else
		[self insertObject:aLayer inLayersAtIndex:layerIndex];
}


- (void)				insertObject:(DKLayer*) aLayer inLayersAtIndex:(NSInteger) layerIndex
{
	NSAssert( aLayer != nil, @"cannot insert a nil layer");
	
	// check that the layer being added isn't a DKDrawing instance - that is a bad thing to attempt.
	
	if([aLayer isKindOfClass:[DKDrawing class]])
		[NSException raise:NSInternalInconsistencyException format:@"Error - attempt to add a DKDrawing instance to a layer group"];

	if( ![self locked] && ![m_layers containsObject:aLayer])
	{
		[[[self undoManager] prepareWithInvocationTarget:self] removeObjectFromLayersAtIndex:layerIndex];
		
		[m_layers insertObject:aLayer atIndex:layerIndex];
		[aLayer setLayerGroup:self];
		[aLayer drawingDidChangeToSize:[NSValue valueWithSize:[[self drawing] drawingSize]]];
		[self setNeedsDisplay:YES];
		[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupDidAddLayer object:self];
		[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupNumberOfLayersDidChange object:self];
	}
}


- (void)				removeLayer:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"cannot remove a nil layer");
	
	[self removeObjectFromLayersAtIndex:[self indexOfLayer:aLayer]];
}


- (void)				removeObjectFromLayersAtIndex:(NSInteger) layerIndex
{
	NSAssert( layerIndex < [self countOfLayers], @"layer index out of range in removeLayerFromLayersAtIndex:");

	if (![self locked])
	{
		DKLayer* aLayer = [self objectInLayersAtIndex:layerIndex];
		
		if( aLayer )
		{
			[[[self undoManager] prepareWithInvocationTarget:self] insertObject:aLayer inLayersAtIndex:layerIndex];
			[aLayer setLayerGroup:nil];
			[m_layers removeObjectAtIndex:layerIndex];
			[self setNeedsDisplay:YES];
			[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupDidRemoveLayer object:self];
			[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupNumberOfLayersDidChange object:self];
		}
	}
}


- (void)				removeAllLayers
{
	if( ![self locked])
	{
		[[self undoManager] removeAllActionsWithTarget:self];
		
		[m_layers makeObjectsPerformSelector:@selector(setLayerGroup:) withObject:nil];
		[m_layers removeAllObjects];
		[self setNeedsDisplay:YES];
		[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupDidRemoveLayer object:self];
		[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupNumberOfLayersDidChange object:self];
	}
}


- (NSString*)			uniqueLayerNameForName:(NSString*) aName
{
	NSInteger	numeral = 0;
	BOOL		found = YES;
	NSString*	temp = aName;
	NSArray*	keys = [[self layers] valueForKey:@"layerName"];
	
	while( found )
	{
		NSInteger	k = [keys indexOfObject:temp];
		
		if ( k == NSNotFound )
			found = NO;
		else
			temp = [NSString stringWithFormat:@"%@ %ld", aName, (long)++numeral];
	}
	
	return temp;
}


#pragma mark -
#pragma mark - getting layers
- (DKLayer*)		objectInLayersAtIndex:(NSInteger) layerIndex
{
	NSAssert1( layerIndex < [self countOfLayers], @"bad layer index %ld (overrange)", (long)layerIndex);
	
	return [[self layers] objectAtIndex:layerIndex];
}


- (DKLayer*)		topLayer
{
	return [[self layers] firstObject];
}


- (DKLayer*)		bottomLayer
{
	return [[self layers] lastObject];
}


- (NSInteger)		indexOfLayer:(DKLayer*) aLayer
{
	return [[self layers] indexOfObjectIdenticalTo:aLayer];
}


- (DKLayer*)		firstLayerOfClass:(Class) cl
{
	return [self firstLayerOfClass:cl performDeepSearch:NO];
}


- (DKLayer*)		firstLayerOfClass:(Class) cl performDeepSearch:(BOOL) deep
{
	NSArray* layers = [self layersOfClass:cl performDeepSearch:deep];
	
	if( layers && [layers count] > 0)
		return [layers objectAtIndex:0];
	else
		return nil;
}


- (NSArray*)		layersOfClass:(Class) cl
{
	return [self layersOfClass:cl performDeepSearch:NO];
}


- (NSArray*)		layersOfClass:(Class) cl performDeepSearch:(BOOL) deep

{
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		lyr;
	NSMutableArray*	layers = [NSMutableArray array];
	
	while(( lyr = [iter nextObject]))
	{
		if ([lyr isKindOfClass:cl])
			[layers addObject:lyr];
		
		if( deep && [lyr respondsToSelector:_cmd])
			[layers addObjectsFromArray:[(DKLayerGroup*)lyr layersOfClass:cl performDeepSearch:YES]];
	}
	
	return layers;
}


- (NSEnumerator*)		layerTopToBottomEnumerator
{
	return [[self layers] objectEnumerator];
}


- (NSEnumerator*)		layerBottomToTopEnumerator
{
	return [[self layers] reverseObjectEnumerator];
}


- (DKLayer*)			findLayerForPoint:(NSPoint) p
{
	if([self visible])
	{
		NSEnumerator*	iter = [self layerTopToBottomEnumerator];
		DKLayer*		layer;
		
		while(( layer = [iter nextObject]))
		{
			if([layer isKindOfClass:[DKLayerGroup class]])
			{
				layer = [(DKLayerGroup*)layer findLayerForPoint:p];
				
				if( layer )
					return layer;
			}
			else if ([layer visible] && [layer hitLayer:p])
				return layer;
		}
	}
	return nil;
}


- (BOOL)				containsLayer:(DKLayer*) aLayer
{
	if( aLayer == self )
		return NO;
	else
	{
		NSEnumerator*	iter = [self layerTopToBottomEnumerator];
		DKLayer*		layer;
		
		while(( layer = [iter nextObject]))
		{
			if( aLayer == layer )
				return YES;
			
			if([layer isKindOfClass:[DKLayerGroup class]])
			{
				if([(DKLayerGroup*)layer containsLayer:aLayer])
					return YES;
			}
		}
		
		return NO;
	}
}


- (DKLayer*)				layerWithUniqueKey:(NSString*) key
{
	NSEnumerator*	iter = [[self layers] objectEnumerator];
	DKLayer*		layer;
	
	while(( layer = [iter nextObject]))
	{
		if([[layer uniqueKey] isEqualToString:key])
			return layer;
		else if([layer isKindOfClass:[self class]])
		{
			layer = [(DKLayerGroup*)layer layerWithUniqueKey:key];
			if( layer )
				return layer;
		}
	}
	
	return nil;
}


#pragma mark -

- (void)					showAll
{
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		aLayer;
	
	while(( aLayer = [iter nextObject]))
	{
		[aLayer setVisible:YES];

		if([aLayer isKindOfClass:[DKLayerGroup class]])
			[(DKLayerGroup*)aLayer showAll];
	}
}


- (void)					hideAllExcept:(DKLayer*) aLayer
{
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		layer;
	
	while(( layer = [iter nextObject]))
	{
		if([layer isKindOfClass:[DKLayerGroup class]])
		{
			[(DKLayerGroup*)layer hideAllExcept:aLayer];
			
			// this logic keeps groups that contain the excepted layer visible if necessary
			
			if( layer == aLayer || [(DKLayerGroup*)layer containsLayer:aLayer])
				[layer setVisible:YES];
			else
				[layer setVisible:NO];
		}
		else
			[layer setVisible:layer == aLayer];
	}
}


- (BOOL)					hasHiddenLayers
{
	if( ![self visible])
		return YES;
	else
	{
		NSEnumerator*	iter = [self layerTopToBottomEnumerator];
		DKLayer*		layer;
		
		while(( layer = [iter nextObject]))
		{
			if( ![layer visible])
				return YES;

			if([layer isKindOfClass:[DKLayerGroup class]])
			{
				if([(DKLayerGroup*)layer hasHiddenLayers])
					return YES;
			}
		}
		
		return NO;
	}
}


- (BOOL)					hasVisibleLayersOtherThan:(DKLayer*) aLayer
{
	if(![self visible] && self != aLayer )
		return NO;
	
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		layer;
	
	while(( layer = [iter nextObject]))
	{
		if( layer != aLayer )
		{
			if([layer visible])
				return YES;
	
			if([layer isKindOfClass:[DKLayerGroup class]])
			{
				if([(DKLayerGroup*)layer hasVisibleLayersOtherThan:aLayer])
					return YES;
			}
		}
	}
	
	return NO;
}



#pragma mark -
#pragma mark - layer stacking order

- (void)				moveUpLayer:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");
	
	[self moveLayer:aLayer toIndex:[self indexOfLayer:aLayer] - 1];
}


- (void)				moveDownLayer:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");

	[self moveLayer:aLayer toIndex:[self indexOfLayer:aLayer] + 1];
}


- (void)				moveLayerToTop:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");

	[self moveLayer:aLayer toIndex:0];
}


- (void)				moveLayerToBottom:(DKLayer*) aLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");

	[self moveLayer:aLayer toIndex:[self countOfLayers] - 1];
}


- (void)				moveLayer:(DKLayer*) aLayer aboveLayer:(DKLayer*) otherLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");

	if ( otherLayer == nil )
		[self moveLayerToBottom:aLayer];
	else
		[self moveLayer:aLayer toIndex:[self indexOfLayer:otherLayer]];
}


- (void)				moveLayer:(DKLayer*) aLayer belowLayer:(DKLayer*) otherLayer
{
	NSAssert( aLayer != nil, @"cannot move a nil layer");

	if ( otherLayer == nil )
		[self moveLayerToTop:aLayer];
	else
		[self moveLayer:aLayer toIndex:[self indexOfLayer:otherLayer] + 1];
}


- (void)				moveLayer:(DKLayer*) aLayer toIndex:(NSInteger) i
{
	// all other layer stacking methods call this one, which implements undo and notification
	
	NSAssert( aLayer != nil, @"trying to move nil layer");
	NSAssert( ![aLayer locked], @"trying to move a locked layer");
	
	if( ![self locked])
	{
		NSInteger k = [self indexOfLayer:aLayer];
		
		if( k == NSNotFound )
			return;
		
		i = MIN( i, [self countOfLayers] - 1 );
		
		if ( k != i )
		{
			[[[self undoManager] prepareWithInvocationTarget:self] moveLayer:aLayer toIndex:k];
			[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupWillReorderLayers object:self];
			
			[aLayer retain];
			[m_layers removeObject:aLayer];
			[m_layers insertObject:aLayer atIndex:i];
			[aLayer release];

			[self setNeedsDisplay:YES];
			[[NSNotificationCenter defaultCenter] postNotificationName:kDKLayerGroupDidReorderLayers object:self];
		}
	}
}


#pragma mark -
#pragma mark As a DKLayer
///*********************************************************************************************************************
///
/// method:			drawingHasNewUndoManager:
/// scope:			public method
/// overrides:		DKLayer
/// description:	propagates the undo manager to all contained layers
/// 
/// parameters:		<um> the drawing's undo manager
/// result:			none
///
/// notes:			
///
///********************************************************************************************************************

- (void)					drawingHasNewUndoManager:(NSUndoManager*) um
{
	[[self layers] makeObjectsPerformSelector:@selector(drawingHasNewUndoManager:) withObject:um];
}


///*********************************************************************************************************************
///
/// method:			drawRect:inView:
/// scope:			public method
/// overrides:		DKLayer
/// description:	draws the layers it contains
/// 
/// parameters:		<rect> the update area passed from the original view
/// result:			none
///
/// notes:			layers are not drawn if they lie below the highest opaque layer, or if we are printing and the layer
///					isn't printable. Otherwise they are drawn from bottom upwards.
///
///********************************************************************************************************************

- (void)				drawRect:(NSRect) rect inView:(DKDrawingView*) aView
{
	if ([self countOfLayers] > 0 )
	{
		// if clipping to the interior, set up that clip now
		
		SAVE_GRAPHICS_CONTEXT			//[NSGraphicsContext saveGraphicsState];

		if ([self clipsDrawingToInterior])
			[NSBezierPath clipRect:[[self drawing] interior]];

		NSUInteger	bottom;
		NSInteger			n;
		BOOL		printing = ![NSGraphicsContext currentContextDrawingToScreen];
		DKLayer*	layer;
		
		bottom = [self indexOfHighestOpaqueLayer];
		
		for( n = bottom; n >= 0; --n )
		{
			layer = [self objectInLayersAtIndex:n];
			
			if ([layer visible] && !( printing && ![layer shouldDrawToPrinter]))
			{
				@try
				{
					[NSGraphicsContext saveGraphicsState];
				
					if ([layer clipsDrawingToInterior])
						[NSBezierPath clipRect:[[self drawing] interior]];
					
					[layer beginDrawing];
					[layer drawRect:rect inView:aView];
					[layer endDrawing];
				}
				@catch( id exc )
				{
					NSLog(@"exception while drawing layer %@ [%ld of %ld in group %@](%@ - ignored)", layer, (long)n, (long)[self countOfLayers], self, exc );
				}
				@finally
				{
					[NSGraphicsContext restoreGraphicsState];
				}
			}
		}
		RESTORE_GRAPHICS_CONTEXT	//[NSGraphicsContext restoreGraphicsState];
	}
}


///*********************************************************************************************************************
///
/// method:			layerMayBecomeActive
/// scope:			public class method
/// description:	returns whether the layer can become the active layer
/// 
/// parameters:		none
/// result:			YES if the layer can become active, NO to not become active
///
/// notes:			The default for groups is NO. Discrete layers should be activated, not groups.
///
///********************************************************************************************************************

- (BOOL)			layerMayBecomeActive
{
	return NO;
}


///*********************************************************************************************************************
///
/// method:			drawingDidChangeToSize:
/// scope:			public class method
/// description:	propagate the message to all contained layers
/// 
/// parameters:		<sizeVal> the new size
/// result:			none
///
/// notes:			
///
///********************************************************************************************************************

- (void)			drawingDidChangeToSize:(NSValue*) sizeVal
{
	[[self layers] makeObjectsPerformSelector:@selector(drawingDidChangeToSize:) withObject:sizeVal];
}

///*********************************************************************************************************************
///
/// method:			drawingDidChangeMargins:
/// scope:			public instance method
/// description:	propagate the message to all contained layers
/// 
/// parameters:		<oldInterior> the old interior rect of the drawing
/// result:			none
///
/// notes:			
///
///********************************************************************************************************************

- (void)			drawingDidChangeMargins:(NSValue*) oldInterior
{
	[[self layers] makeObjectsPerformSelector:@selector(drawingDidChangeMargins:) withObject:oldInterior];
}


///*********************************************************************************************************************
///
/// method:			hitLayer:
/// scope:			public instance method
/// description:	see if any enclosed layer is hit by the point
/// 
/// parameters:		<p> the point to test
/// result:			YES if any layer within this group was hit, otherwise NO
///
/// notes:			
///
///********************************************************************************************************************

- (BOOL)			hitLayer:(NSPoint) p
{
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		layer;
	
	while(( layer = [iter nextObject]))
	{
		if([layer hitLayer:p])
			return YES;
	}
	return NO;
}


///*********************************************************************************************************************
///
/// method:			wasAddedToDrawing:
/// scope:			public instance method
/// description:	notifies the layer that it or a group containing it was added to a drawing.
/// 
/// parameters:		<aDrawing> the drawing that added the layer
/// result:			none
///
/// notes:			propagates the message to all contained layers
///
///********************************************************************************************************************

- (void)			wasAddedToDrawing:(DKDrawing*) aDrawing
{
	[[self layers] makeObjectsPerformSelector:_cmd withObject:aDrawing];
}


- (NSInteger)				level
{
	if([self layerGroup] == nil )
		return 0;
	else
		return [[self layerGroup] level] + 1;
}


#pragma mark -
#pragma mark - style utilities

///*********************************************************************************************************************
///
/// method:			allStyles
/// scope:			public method
/// overrides:
/// description:	return all of styles used by layers in this group
/// 
/// parameters:		none
/// result:			a set containing the union of sets returned by all similar methods of individual layers
///
/// notes:			
///
///********************************************************************************************************************

- (NSSet*)			allStyles
{
	// returns the union of all sublayers that return something for this method
	
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		layer;
	NSSet*			styles;
	NSMutableSet*	unionOfAllStyles = nil;
	
	while(( layer = [iter nextObject]))
	{
		styles = [layer allStyles];
		
		if ( styles != nil )
		{
			// we got one - make a set to union them with if necessary
			
			if ( unionOfAllStyles == nil )
				unionOfAllStyles = [styles mutableCopy];
			else
				[unionOfAllStyles unionSet:styles];
		}
	}
	
	return [unionOfAllStyles autorelease];
}


///*********************************************************************************************************************
///
/// method:			allRegisteredStyles
/// scope:			public method
/// overrides:
/// description:	return all of registered styles used by the layers in this group
/// 
/// parameters:		none
/// result:			a set containing the union of sets returned by all similar methods of individual layers
///
/// notes:			
///
///********************************************************************************************************************

- (NSSet*)			allRegisteredStyles
{
	// returns the union of all sublayers that return something for this method
	
	NSEnumerator*	iter = [self layerTopToBottomEnumerator];
	DKLayer*		layer;
	NSSet*			styles;
	NSMutableSet*	unionOfAllStyles = nil;
	
	while(( layer = [iter nextObject]))
	{
		styles = [layer allRegisteredStyles];
		
		if ( styles != nil )
		{
			// we got one - make a set to union them with if necessary
			
			if ( unionOfAllStyles == nil )
				unionOfAllStyles = [styles mutableCopy];
			else
				[unionOfAllStyles unionSet:styles];
		}
	}
	
	return [unionOfAllStyles autorelease];
}


///*********************************************************************************************************************
///
/// method:			replaceMatchingStylesFromSet:
/// scope:			public method
/// overrides:
/// description:	substitute styles with those in the given set
/// 
/// parameters:		<aSet> a set of style objects
/// result:			none
///
/// notes:			This is an important step in reconciling the styles loaded from a file with the existing
///					registry. Implemented by DKObjectOwnerLayer, etc. Groups propagate the change to all sublayers.
///
///********************************************************************************************************************

- (void)			replaceMatchingStylesFromSet:(NSSet*) aSet
{
	[[self layers] makeObjectsPerformSelector:@selector(replaceMatchingStylesFromSet:) withObject:aSet];
}



#pragma mark -
#pragma mark As an NSObject
- (void)					dealloc
{
	// set group ref to nil in case someone else is retaining any layer
	
	[m_layers makeObjectsPerformSelector:@selector(setLayerGroup:) withObject:nil];
	[m_layers release];
	[super dealloc];
}


- (id)						init
{
	return [self initWithLayers:nil];
}


#pragma mark -
#pragma mark As part NSCoding Protocol
- (void)					encodeWithCoder:(NSCoder*) coder
{
	NSAssert(coder != nil, @"Expected valid coder");
	[super encodeWithCoder:coder];
	
	// store a flag to say that we now store layers the other way up - this triggers older files that lack this
	// to have their layer order reversed when loaded
	
	[coder encodeBool:YES forKey:@"DKLayerGroup_invertedStack"];
	[coder encodeObject:[self layers] forKey:@"DKLayerGroup_layers"];
}


- (id)						initWithCoder:(NSCoder*) coder
{
	NSAssert(coder != nil, @"Expected valid coder");
	LogEvent_(kFileEvent, @"decoding layer group %@", self);
	
	self = [super initWithCoder:coder];
	if (self != nil)
	{
		// prior to beta 3, layers were stored in the inverse order, so those files need to have their layers stacked
		//  the other way up so they come up true in the current model.
		
		BOOL hasInvertedLayerStack = [coder decodeBoolForKey:@"DKLayerGroup_invertedStack"];
		
		if( !hasInvertedLayerStack )
		{
			NSArray*		layerStack = [coder decodeObjectForKey:@"layers"];
			
			if([layerStack count] > 1)
			{
				NSMutableArray* temp = [NSMutableArray array];
				NSEnumerator*	iter = [layerStack objectEnumerator];
				DKLayer*		layer;
				
				while(( layer = [iter nextObject]))
					[temp insertObject:layer atIndex:0];
					
				[self setLayers:temp];
			}
			else
				[self setLayers:layerStack];
		}
		else
		{
			NSArray*	layers = [coder decodeObjectForKey:@"DKLayerGroup_layers"];
			
			if( layers == nil )
				layers = [coder decodeObjectForKey:@"layers"];
			
			LogEvent_(kFileEvent, @"decoding layers in group: %@", layers);
			[self setLayers:layers];
		}
		
		
		if (m_layers == nil)
		{
			[self autorelease];
			return nil;
		}
	}
	return self;
}


@end
