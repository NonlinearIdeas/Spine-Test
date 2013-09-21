/*******************************************************************************
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
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
 ******************************************************************************/

#include "ExampleLayer.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

CCScene* ExampleLayer::scene () {
	CCScene *scene = CCScene::create();
	scene->addChild(ExampleLayer::create());
	return scene;
}

bool ExampleLayer::init ()
{
	if (!CCLayerColor::initWithColor(ccc4(250, 250, 250, 250)))
       return false;

   
   skeletonNode = new CCSkeletonAnimation("Spider.json", "Spider.atlas");
   skeletonNode->setMix("WalkRight", "IdleFromRight", 0.5f);
   skeletonNode->setMix("IdleFromRight", "WalkRight", 0.5f);
	skeletonNode->setAnimation("WalkRight", true);
   skeletonNode->setScale(1.0);
   skeletonNode->setSlotsToSetupPose();

	// This shows how to setup animations to play back to back.
   //	skeletonNode->addAnimation("WalkRight", false);
   //	skeletonNode->addAnimation("IdleFromRight", false);

	skeletonNode->timeScale = 0.75f;
	skeletonNode->debugBones = false;

	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
	skeletonNode->setPosition(ccp(windowSize.width / 2, windowSize.height/2));
	addChild(skeletonNode);
	skeletonNode->release();

	scheduleUpdate();

	return true;
}

void ExampleLayer::update (float deltaTime)
{
   // We can use the update to check on the states of the animations.
   /*
   for(int idx = 0; idx < skeletonNode->states.size(); idx++)
   {
      CCLOG("");
      CCLOG("------------ Animation #%d ------------",idx);
      CCLOG("Name: %s Duration: %f Time: %f",
            skeletonNode->states[idx]->animation->name,
            skeletonNode->states[idx]->animation->duration,
            skeletonNode->states[idx]->time
            );
      CCLOG("---------------------------------------");
   }
    */
   
   
   /* A word of caution here:  The original implementation of 
    * CCSkeletonAnimation autoschedules itself to have update(...) called 
    * automatically.  This has been removed so that the call to update(...) is now
    * explicit and you have fine control over it.  If you are doing fixed step timing,
    * or using the times of the animations to drive your states, you will need this
    * to be done.
    */
    
   skeletonNode->update(deltaTime);
   if(skeletonNode->states[0]->time > skeletonNode->states[0]->animation->duration)
   {  // The current animation has finished.
      // Set the other animation.
      if(strcmp("IdleFromRight", skeletonNode->states[0]->animation->name) == 0)
      {  // This animation is finished.
         skeletonNode->addAnimation("WalkRight", false);
         CCLOG("Setting WalkRight");
      }
      if(strcmp("WalkRight", skeletonNode->states[0]->animation->name) == 0)
      {  // This animation is finished.
         skeletonNode->addAnimation("IdleFromRight", false);
         CCLOG("Setting IdleFromRight");
      }
      
   }
}
