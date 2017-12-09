///**********************************************************************************************************************************
///  NSDictionary+DeepCopy.h
///  DrawKit ©2005-2008 Apptree.net
///
///  Created by graham on 12/11/2006.
///
///	 This software is released subject to licensing conditions as detailed in DRAWKIT-LICENSING.TXT, which must accompany this source file. 
///
///**********************************************************************************************************************************

#import <Cocoa/Cocoa.h>


/*
 
 implements a deep copy of a dictionary and array. The keys are unchanged but each object is copied.
 
 if the dictionary contains another dictionary or an array, it is also deep copied.
 
 to retain the semantics of a normal copy, the object returned is not autoreleased.
 
 */
@interface NSDictionary<KeyType, ObjectType> (DeepCopy)

- (NSDictionary<KeyType, ObjectType>*)deepCopy NS_RETURNS_RETAINED;

@end


@interface NSArray<ObjectType> (DeepCopy)

- (NSArray<ObjectType>*)deepCopy NS_RETURNS_RETAINED;

@end


@interface NSObject (DeepCopy)

- (id)deepCopy NS_RETURNS_RETAINED;

@end


@interface NSMutableArray<ObjectType> (DeepCopy)

- (NSMutableArray<ObjectType>*)deepCopy NS_RETURNS_RETAINED;

@end
