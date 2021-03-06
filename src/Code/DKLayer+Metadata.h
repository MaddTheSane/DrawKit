///**********************************************************************************************************************************
///  DKLayer+Metadata.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 19/03/2007.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import "DKLayer.h"
#import "DKMetadataItem.h"

typedef NS_ENUM(NSInteger, DKLayerMetadataSchema)
{
	kDKLayerMetadataOriginalSchema			= 1,
	kDKLayerMetadataCaseInsensitiveSchema	= 2,
	kDKLayerMetadata107Schema				= 3
};


@interface DKLayer (Metadata)

@property (class) BOOL metadataChangesAreUndoable;

- (void)					setupMetadata;
- (NSMutableDictionary<NSString*,id>*)	metadata;
@property (readonly) DKLayerMetadataSchema schema;
@property (readonly, copy) NSArray<NSString*> *metadataKeys;

- (void)		addMetadata:(NSDictionary<NSString*,id>*) dict;
- (void)		setMetadata:(NSDictionary<NSString*,id>*) dict;

- (void)		setMetadataItem:(DKMetadataItem*) item forKey:(NSString*) key;
- (DKMetadataItem*)	metadataItemForKey:(NSString*) key;
- (void)		setMetadataItemValue:(id) value forKey:(NSString*) key;
- (void)		setMetadataItemType:(DKMetadataType) type forKey:(NSString*) key;

- (id)			metadataObjectForKey:(NSString*) key;

- (BOOL)		hasMetadataForKey:(NSString*) key;
- (void)		removeMetadataForKey:(NSString*) key;

- (void)		setFloatValue:(float) val forKey:(NSString*) key;
- (CGFloat)		floatValueForKey:(NSString*) key;

- (void)		setIntValue:(int) val forKey:(NSString*) key;
- (NSInteger)	intValueForKey:(NSString*) key;

- (void)		setString:(NSString*) string forKey:(NSString*) key;
- (NSString*)	stringForKey:(NSString*) key;

- (void)		setColour:(NSColor*) colour forKey:(NSString*) key;
- (NSColor*)	colourForKey:(NSString*) key;

- (void)		setSize:(NSSize) size forKey:(NSString*) key;
- (NSSize)		sizeForKey:(NSString*) key;

- (void)		updateMetadataKeys;
@property (readonly) NSUInteger metadataChecksum;

@property (readonly) BOOL supportsMetadata;
- (void)		metadataWillChangeKey:(NSString*) key;
- (void)		metadataDidChangeKey:(NSString*) key;

@end

extern NSString*	kDKLayerMetadataUserInfoKey;
extern NSString*	kDKLayerMetadataUndoableChangesUserDefaultsKey;
extern NSNotificationName	kDKMetadataWillChangeNotification;
extern NSNotificationName	kDKMetadataDidChangeNotification;



@interface DKLayer (MetadataDeprecated)

- (void)		setMetadataObject:(id) obj forKey:(id) key DEPRECATED_ATTRIBUTE;

@end

/* adds some convenience methods for standard meta data attached to a graphic object. By default the metadata is just an uncomitted
id, but using this sets it to be a mutable dictionary. You can then easily get and set values in that dictionary.

*/
