/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 * 
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "spine/SkeletonAnimation.h"
#include "spine/spine-cocos2dx.h"
#include "spine/extension.h"
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;
using std::vector;

//From Cache Mohit
spSkeletonData *mydata[200];

namespace spine {

void animationCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((SkeletonAnimation*)state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
}

void trackEntryCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	((SkeletonAnimation*)state->rendererObject)->onTrackEntryEvent(trackIndex, type, event, loopCount);
}

typedef struct _TrackEntryListeners {
	StartListener startListener;
	EndListener endListener;
	CompleteListener completeListener;
	EventListener eventListener;
} _TrackEntryListeners;

static _TrackEntryListeners* getListeners (spTrackEntry* entry) {
	if (!entry->rendererObject) {
		entry->rendererObject = NEW(spine::_TrackEntryListeners);
		entry->listener = trackEntryCallback;
	}
	return (_TrackEntryListeners*)entry->rendererObject;
}

void disposeTrackEntry (spTrackEntry* entry) {
	if (entry->rendererObject) FREE(entry->rendererObject);
	_spTrackEntry_dispose(entry);
}

//

SkeletonAnimation* SkeletonAnimation::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData) {
	SkeletonAnimation* node = new SkeletonAnimation(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

SkeletonAnimation* SkeletonAnimation::createWithFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale) {
	SkeletonAnimation* node = new SkeletonAnimation(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

SkeletonAnimation* SkeletonAnimation::createWithFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale) {
	SkeletonAnimation* node = new SkeletonAnimation(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void SkeletonAnimation::initialize () {
	_ownsAnimationStateData = true;
	_state = spAnimationState_create(spAnimationStateData_create(_skeleton->data));
	_state->rendererObject = this;
	_state->listener = animationCallback;

	_spAnimationState* stateInternal = (_spAnimationState*)_state;
	stateInternal->disposeTrackEntry = disposeTrackEntry;
}

SkeletonAnimation::SkeletonAnimation ()
		: SkeletonRenderer() {
}

SkeletonAnimation::SkeletonAnimation (spSkeletonData *skeletonData, bool ownsSkeletonData)
		: SkeletonRenderer(skeletonData, ownsSkeletonData) {
	initialize();
}

SkeletonAnimation::SkeletonAnimation (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
		: SkeletonRenderer(skeletonDataFile, atlas, scale) {
	initialize();
}

SkeletonAnimation::SkeletonAnimation (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
		: SkeletonRenderer(skeletonDataFile, atlasFile, scale) {
	initialize();
}

SkeletonAnimation::~SkeletonAnimation () {
	if (_ownsAnimationStateData) spAnimationStateData_dispose(_state->data);
	spAnimationState_dispose(_state);
}

void SkeletonAnimation::update (float deltaTime) {
	super::update(deltaTime);

	deltaTime *= _timeScale;
	spAnimationState_update(_state, deltaTime);
	spAnimationState_apply(_state, _skeleton);
	spSkeleton_updateWorldTransform(_skeleton);
}

void SkeletonAnimation::setAnimationStateData (spAnimationStateData* stateData) {
	CCASSERT(stateData, "stateData cannot be null.");

	if (_ownsAnimationStateData) spAnimationStateData_dispose(_state->data);
	spAnimationState_dispose(_state);

	_ownsAnimationStateData = false;
	_state = spAnimationState_create(stateData);
	_state->rendererObject = this;
	_state->listener = animationCallback;
}

void SkeletonAnimation::setMix (const std::string& fromAnimation, const std::string& toAnimation, float duration) {
	spAnimationStateData_setMixByName(_state->data, fromAnimation.c_str(), toAnimation.c_str(), duration);
}

spTrackEntry* SkeletonAnimation::setAnimation (int trackIndex, const std::string& name, bool loop)
{
    SkeletonAnimation *Child = (SkeletonAnimation*)this->getChildByTag(5678);
    if (Child)
    {
        Child->setAnimation(trackIndex, name, loop);
    }
    
	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name.c_str());
	if (!animation) {
		log("Spine: Animation not found: %s", name.c_str());
		return 0;
	}
	return spAnimationState_setAnimation(_state, trackIndex, animation, loop);
}
    bool SkeletonAnimation::EyeRotation_setTexture(const char *imageName,const char* slotName,float offX,float offY)
    {
        spRegionAttachment *reg = this->EyeRotation_regionWithTexture2d(imageName,slotName,offX,offY);
        spSlot* slot = this->findSlot(slotName);
        if ((slot!=NULL)&&(reg!=NULL))
        {
            spSlot_setAttachment(slot,(spAttachment*)reg);
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }
    spRegionAttachment* SkeletonAnimation::EyeRotation_regionWithTexture2d(const char *imageName , const char* name,float offX,float offY)
    {
        Texture2D *tex2d=Director::getInstance()->getTextureCache()->addImage(imageName);
        
        spAtlasPage *atPage = spAtlasPage_create(NEW(spAtlas),name);
        atPage->rendererObject = tex2d;
        
        atPage->width = tex2d->getContentSizeInPixels().width;
        atPage->height = tex2d->getContentSizeInPixels().height;
        
        spAtlasRegion *reg = spAtlasRegion_create();
        reg->page = atPage;
        
        spRegionAttachment *region = spRegionAttachment_create(name);
        region->rendererObject = reg;
        
        Size size = tex2d->getContentSizeInPixels();
        float u = 0;
        float u2 = 1;
        float v = 0;
        float v2 = 1;
        spRegionAttachment_setUVs(region,u,v,u2,v2,0);
        
        spSlot* slot = this->findSlot(name);
        //    CCLOG("%s",slot->attachment->name);
        spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
        
        region->regionOffsetX = offX;
        region->regionOffsetY = offY;
        region->regionWidth = size.width;
        region->regionHeight = size.height;
        region->regionOriginalWidth = region->regionWidth; // same if not doing whitespace stripping
        region->regionOriginalHeight = region->regionHeight;
        
        region->x = attachment->x;
        region->y = attachment->y;
        
        //    if (isToyView)
        {
            region->scaleX =1.0;
            region->scaleY = 1.0;//this->nodeScale;//TODO find replacement
        }
        //    else
        //    {
        //        region->scaleX =0.75;
        //        region->scaleY = 0.75;//this->nodeScale;//TODO find replacement
        //
        //    }
        
        region->rotation = attachment->rotation;
        region->width = size.width;
        region->height = size.height;
        
        spRegionAttachment_updateOffset(region);
        
        return region;
    }
spTrackEntry* SkeletonAnimation::addAnimation (int trackIndex, const std::string& name, bool loop, float delay)
{
    SkeletonAnimation *Child = (SkeletonAnimation*)this->getChildByTag(5678);
    if (Child)
    {
        Child->addAnimation(trackIndex, name, loop,delay);
    }
	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name.c_str());
	if (!animation) {
		log("Spine: Animation not found: %s", name.c_str());
		return 0;
	}
	return spAnimationState_addAnimation(_state, trackIndex, animation, loop, delay);
}

spTrackEntry* SkeletonAnimation::getCurrent (int trackIndex) { 
	return spAnimationState_getCurrent(_state, trackIndex);
}

void SkeletonAnimation::clearTracks ()
{
    SkeletonAnimation *Child = (SkeletonAnimation*)this->getChildByTag(5678);
    if (Child)
    {
        Child->clearTracks();
    }
	spAnimationState_clearTracks(_state);
}

void SkeletonAnimation::clearTrack (int trackIndex) {
	spAnimationState_clearTrack(_state, trackIndex);
}

void SkeletonAnimation::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
	switch (type) {
	case SP_ANIMATION_START:
		if (_startListener) _startListener(trackIndex);
		break;
	case SP_ANIMATION_END:
		if (_endListener) _endListener(trackIndex);
		break;
	case SP_ANIMATION_COMPLETE:
		if (_completeListener) _completeListener(trackIndex, loopCount);
		break;
	case SP_ANIMATION_EVENT:
		if (_eventListener) _eventListener(trackIndex, event);
		break;
	}
}

void SkeletonAnimation::onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
	spTrackEntry* entry = spAnimationState_getCurrent(_state, trackIndex);
	if (!entry->rendererObject) return;
	_TrackEntryListeners* listeners = (_TrackEntryListeners*)entry->rendererObject;
	switch (type) {
	case SP_ANIMATION_START:
		if (listeners->startListener) listeners->startListener(trackIndex);
		break;
	case SP_ANIMATION_END:
		if (listeners->endListener) listeners->endListener(trackIndex);
		break;
	case SP_ANIMATION_COMPLETE:
		if (listeners->completeListener) listeners->completeListener(trackIndex, loopCount);
		break;
	case SP_ANIMATION_EVENT:
		if (listeners->eventListener) listeners->eventListener(trackIndex, event);
		break;
	}
}

void SkeletonAnimation::setStartListener (const StartListener& listener) {
	_startListener = listener;
}

void SkeletonAnimation::setEndListener (const EndListener& listener) {
	_endListener = listener;
}

void SkeletonAnimation::setCompleteListener (const CompleteListener& listener) {
	_completeListener = listener;
}

void SkeletonAnimation::setEventListener (const EventListener& listener) {
	_eventListener = listener;
}

void SkeletonAnimation::setTrackStartListener (spTrackEntry* entry, const StartListener& listener) {
	getListeners(entry)->startListener = listener;
}

void SkeletonAnimation::setTrackEndListener (spTrackEntry* entry, const EndListener& listener) {
	getListeners(entry)->endListener = listener;
}

void SkeletonAnimation::setTrackCompleteListener (spTrackEntry* entry, const CompleteListener& listener) {
	getListeners(entry)->completeListener = listener;
}

void SkeletonAnimation::setTrackEventListener (spTrackEntry* entry, const EventListener& listener) {
	getListeners(entry)->eventListener = listener;
}

spAnimationState* SkeletonAnimation::getState() const {
	return _state;
}

}
